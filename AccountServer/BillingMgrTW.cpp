#include "stdafx.h"
#include "BillingMgrTW.h"
#include "MsgHdr.h"
#include "account.h"
#include "DBManager.h"
#include "dpsrvr.h"
#include "dpdbsrvr.h"

extern	CAccountMng	g_AccountMng;
extern	CDbManager	g_DbManager;
extern	CDPSrvr		g_dpSrvr;
extern	CDPDBSrvr	g_dpDbSrvr;

///////////////////////////////////////////////////////////////////////
// BillingMgrTW
///////////////////////////////////////////////////////////////////////
CBillingMgrTW::CBillingMgrTW()
{
	m_iBillingFreePass	= 0;
}

CBillingMgrTW::~CBillingMgrTW()
{
}

BOOL CBillingMgrTW::SetConfig( BILLING_ENUM id, DWORD data )
{
	switch( id )
	{
	case BID_FREEPASS:
		m_iBillingFreePass = (int)data;
		break;
	}
	return TRUE;
}

bool CBillingMgrTW::Init( HWND hWnd )
{
	::SetTimer( hWnd, IDT_KICKOUT, 1000 * 60, NULL );
	return true;
}

void CBillingMgrTW::Release()
{
	safe_delete( this );
}

BYTE CBillingMgrTW::CheckAccount( int nType, DWORD dwKey, const char* szAccount, const char* szAddr )
{
	g_DbManager.PostBillingQuery( szAccount, dwKey, 1 );
	return ACCOUNT_BILLING_WAIT_ACK;
}

BOOL CBillingMgrTW::PreTranslateMessage( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) 
{
	switch( message ) 
	{
		case WM_TIMER:
			{
				switch( wParam )
				{
				case IDT_KICKOUT:
					g_AccountMng.KickOutCheck();		
					return TRUE;
				}
			}
			break;
	}

	return FALSE;
}


// 
void CBillingMgrTW::OnTimer( CAccount* pAccount )
{
	CTime tm = 0;
	if( pAccount->m_TimeOverDays == tm )		
		return;		

	CTime cur = CTime::GetCurrentTime();
	if( pAccount->m_TimeOverDays <= cur )		
	{	
		pAccount->m_nStatus = ACCOUNT_STATUS_SECONDQUERY;	
		g_DbManager.PostBillingQuery( pAccount->m_lpszAccount, pAccount->m_dwAuthKey, 2 );
	}
	else
	{
		if( pAccount->m_cbLastOneLogon == 'Y' )	
		{
			CTimeSpan ts = pAccount->m_TimeOverDays - cur;
			if ( ts.GetTotalMinutes() >= 29 && ts.GetTotalMinutes() <= 31 && 
				 pAccount->m_nStatus != ACCOUNT_STATUS_NOTIFIED &&
				 pAccount->m_fRoute == TRUE )
			{
				pAccount->m_nStatus = ACCOUNT_STATUS_NOTIFIED;				
				g_dpDbSrvr.SendOneHourNotify( pAccount );	
			}
		}
	}			
}

void CBillingMgrTW::OnDBQuery( CQuery& query, tagDB_OVERLAPPED_PLUS* pOV )
{
	long lResult = OTHER_ERROR;
	CTime endDay;

	sprintf( pOV->szQuery, "OGB_SP_LOGON_BILLING_CHECK '%s', %d", pOV->szAccount, pOV->nExtra );	

	TIMESTAMP_STRUCT ts;
	char szAccount[64] = "";
	char szLastOneLogon[8] = "";
	int  nBill = -1;

	QUERY_BINDINFO infos[4] = {
		{SQL_C_CHAR,      szAccount,      63,				NULL},
		{SQL_C_LONG,      &nBill,         sizeof(nBill),	NULL},
		{SQL_C_TIMESTAMP, &ts,            sizeof(ts),		NULL},
		{SQL_C_CHAR,      szLastOneLogon, 7,				NULL}
	};

	if( query.Exec( pOV->szQuery, 4, infos ) )
	{
		if( query.Fetch() )
		{
			switch( nBill )
			{
			case 0:
				lResult = OTHER_ERROR;
				break;
			case 151:
				{
				endDay = CTime(ts.year, ts.month, ts.day, ts.hour, ts.minute, ts.second);
#ifdef _DEBUG
				CTimeSpan timeSpan( 0, 0, 0, 5 * 60 );
				endDay += timeSpan;	
#endif
				if( endDay > CTime::GetCurrentTime() )
					lResult = SUCCESS;
				else
					lResult = TIME_OVER;
				}
				break;
			case 156:
				lResult = BILLING_INFO_FAILED;
				break;
			default:
				lResult = OTHER_ERROR;
				break;
			}
		}
		else
		{
			lResult = BILLING_INFO_FAILED;
		}
	}
	else
	{
		lResult = DATABASE_ERROR;
		WriteLog( "%s, %d\r\n\t%s", __FILE__, __LINE__, pOV->szQuery );
	}

	char szTrace[256];
	if( lResult == SUCCESS || lResult == TIME_OVER )
	{
		sprintf( szTrace, "%s param:%d bill:%d (%04d/%02d/%02d-%02d:%02d:%02d) LastOneLogon:%c\n",
			  szAccount, pOV->nExtra, nBill,
			  endDay.GetYear(), endDay.GetMonth(), endDay.GetDay(), 
			  endDay.GetHour(), endDay.GetMinute(), endDay.GetSecond(),
			  szLastOneLogon[0] );			  
	}
	else
	{
		sprintf( szTrace, "%s param:%d bill:%d \n", szAccount, pOV->nExtra, nBill ); 
	}
	OutputDebugString( szTrace );

	BILLING_INFO info;
	info.szAccount = pOV->szAccount;
	info.dwKey = pOV->dwKey;
	info.lResult = lResult;
	info.pTimeOverDays = &endDay;
	info.cbLastOneLogon = szLastOneLogon[0];

	g_AccountMng.SendBillingResult( &info );
}
#include "stdafx.h"
#include <string.h>
#include <stdio.h>
#include "file.h"
#include "xutil.h"
#ifdef __SECURITY_0628
#ifdef __CLIENT
#include "definetext.h"
#endif	// __CLIENT
#endif	// __SECURITY_0628
#include "tools.h"

/////////////////////////////////////////////////////////////////////////////
// CFileIO

BOOL CFileIO::Close(void)
{
	if( fp ) 
	{ 
		fclose( fp ); 
		fp = 0; 
		return TRUE; 
	}
	return FALSE;
}

BOOL CFileIO::Open( LPCTSTR fileName, TCHAR* mode )
{
	fp = _tfopen( fileName, mode );
	if( fp )
		return TRUE;
	return FALSE;
}

LPVOID CFileIO::Read(void)
{
	long size = GetLength();
	LPVOID ptr = new BYTE[ size ];
	if( !ptr ) 
		return NULL;

	fseek( fp, 0, SEEK_SET );
	if( fread( ptr, size, 1, fp ) ) 
	{ 
		return ptr;
	}
	safe_delete_array( ptr ); 
	return NULL;
}
int CFileIO::PutWideString( LPCTSTR lpszString )
{
	wchar_t wszBuff[ 1024 ];
	int nLen = strlen( lpszString );

	ATL::_mbstowcsz(wszBuff, lpszString,nLen + 1 );

	return Write( wszBuff, wcslen( wszBuff ) * 2 );
}

#if defined( __CLIENT )
/////////////////////////////////////////////////////////////////////////////
// CResFile
CMapStringToPtr CResFile::m_mapResource;

#ifdef __SECURITY_0628
char	CResFile::m_szResVer[100];
map<string, string>	CResFile::m_mapAuth;
#endif	// __SECURITY_0628

CResFile::CResFile( LPCTSTR lpszFileName, TCHAR *mode ) 
{ 
	m_bResouceInFile = FALSE;
	m_nFileSize = 0;
	m_nFileBeginPosition = 0;
	m_nFileCurrentPosition = 0;
	m_nFileEndPosition = 0;
	m_bEncryption = FALSE;  
	ZeroMemory( m_szFileName, sizeof( m_szFileName ) );
	Open( lpszFileName, mode ); 
}
CResFile::CResFile() 
{ 
	m_bResouceInFile = FALSE;
	m_nFileSize = 0;
	m_nFileBeginPosition = 0;
	m_nFileCurrentPosition = 0;
	m_nFileEndPosition = 0;
	m_bEncryption = FALSE;  
	ZeroMemory( m_szFileName, sizeof( m_szFileName ) );
}
 
BOOL CResFile::Close( void )
{
	if( !m_bResouceInFile )
		return CFileIO::Close();
	else
	{
		if( INVALID_HANDLE_VALUE != ( HANDLE )m_File.m_hFile )
			m_File.Close();
		return TRUE;
	}
}



void CResFile::AddResource( TCHAR* lpszResName )
{
	int nFileHeaderSize = 0;
	int nFileNumber = 0;
	short	nFileNameLength = 0;
	char szFileName[_MAX_FNAME] = "";
	int nFileSize = 0;
	int nFilePosition = 0;
	BYTE byEncryptionKey;
	bool bEncryption;
	TCHAR szFullFileName[ MAX_PATH ];
	TCHAR drive[_MAX_DRIVE], dir[_MAX_DIR], name[ _MAX_FNAME ], ext[_MAX_EXT];
	_splitpath( lpszResName, drive, dir, name, ext );
	
	CFile file;
	if( file.Open( lpszResName, CFile::modeRead ) == FALSE )
		return;
	file.Read( &byEncryptionKey, sizeof( byEncryptionKey ) );
	file.Read( &bEncryption, sizeof( bEncryption ) );
	file.Read( &nFileHeaderSize, sizeof( int ) );
	
	char *pHeader = new char[ nFileHeaderSize ];

	{
		char *pCryptHeader = new char[ nFileHeaderSize ];
		file.Read( pCryptHeader, nFileHeaderSize );
		
		for( int i = 0; i < nFileHeaderSize; i++ )
		{
			pHeader[i] = Decryption( byEncryptionKey, pCryptHeader[ i ] );
		}
		
		safe_delete_array( pCryptHeader );
	}
	int nHeaderPosition = 0;
	char strVersion[8] ="";
	memcpy( strVersion, &pHeader[ nHeaderPosition ], 7 ); nHeaderPosition += 7;
	
	memcpy( &nFileNumber, &pHeader[ nHeaderPosition ], sizeof( short ) ); nHeaderPosition += sizeof( short );
	time_t time_;
	BOOL bFind = FALSE;
	for( int i = 0; i < nFileNumber; i++ )
	{
		memcpy( &nFileNameLength, &pHeader[ nHeaderPosition ], sizeof( short ) ); nHeaderPosition += sizeof( short );
		memcpy( szFileName, &pHeader[ nHeaderPosition ], nFileNameLength ); nHeaderPosition += nFileNameLength;
		memcpy( &nFileSize, &pHeader[ nHeaderPosition ], sizeof( int ) ); nHeaderPosition += sizeof( int );
		memcpy( &time_, &pHeader[ nHeaderPosition ], sizeof( time_t ) ); nHeaderPosition += sizeof( time_t );
		memcpy( &nFilePosition, &pHeader[ nHeaderPosition ], sizeof( int ) ); nHeaderPosition += sizeof( int );
		RESOURCE* lpRes = new RESOURCE;
		ZeroMemory( lpRes, sizeof( RESOURCE ) );
		lpRes->dwOffset = nFilePosition;
		lpRes->dwFileSize = nFileSize;
		lpRes->byEncryptionKey = byEncryptionKey;
		lpRes->bEncryption = bEncryption;
		strcpy( lpRes->szResourceFile, lpszResName );
		strlwr( lpRes->szResourceFile );

		strcpy( szFullFileName, dir );
		strcat( szFullFileName, szFileName );
		strlwr( szFullFileName );


#ifdef __CLIENT
#ifdef __SECURITY_0628
		CString strFullFileName	= szFullFileName;
		if( strFullFileName.Find( "\\", 0 ) < 0 )
		{
			RESOURCE* ptr;
			if( m_mapResource.Lookup( szFullFileName, (void*&)ptr ) )
			{
				::Error( "killed by CResFile::AddResource() %s, %s, 1", prj.GetText(TID_GAME_RESOURCE_MODIFIED), szFullFileName );
				ExitProcess( -1 );
			}
		}
#endif	// __SECURITY_0628
#endif	// __CLIENT
		m_mapResource.SetAt( szFullFileName, lpRes );
		ZeroMemory( szFileName, sizeof( szFileName ) );
	}
	safe_delete_array( pHeader );
}

bool CompareExt( LPCTSTR szName, LPCTSTR szExt )
{
	int nExt = strlen( szExt );
	int n = strlen( szName );
	if( n < nExt )
		return false;
	return ( memcmp( szName + n - nExt, szExt, nExt ) == 0 );
}

void CResFile::ScanResource( LPCTSTR lpszRootPath )
{
	TCHAR szPath[ MAX_PATH ];
	TCHAR szPathTemp[ MAX_PATH ];
	struct _finddata_t c_file;
	long hFile;

	strcpy( szPathTemp, lpszRootPath );
	strcat( szPathTemp, "*.*" );
	if( ( hFile = _findfirst( szPathTemp, &c_file ) ) != -1L )
	{
		do
		{
			strlwr( c_file.name );
			strcpy( szPath, lpszRootPath );
			if( c_file.attrib & _A_SUBDIR )
			{
				if( strcmp( c_file.name, "." ) && strcmp( c_file.name, ".." ) 
#ifdef __INTERNALSERVER
					&& strcmp( c_file.name, ".svn" )
#endif // __INTERNALSERVER
					)
				{
					strcat( szPath, c_file.name );
					strcat( szPath, "\\" );
					ScanResource( szPath );
				}
			}
			else
			if( CompareExt( c_file.name, ".res" ) )
			{
				strcat( szPath, c_file.name );
				AddResource( szPath  );
			}
		}
		while( _findnext( hFile, &c_file ) == 0 );
			
		_findclose( hFile );
	}	
}


void CResFile::FreeResource()
{
	CString strNameName;
	RESOURCE* lpRes;
	POSITION pos;
	pos = m_mapResource.GetStartPosition();
	while( pos )
	{
		m_mapResource.GetNextAssoc( pos, strNameName, (void*&) lpRes );
		safe_delete( lpRes );
	}
}
BOOL CResFile::Open( LPCTSTR lpszFileName, TCHAR *mode )
{
	if( CFileIO::Open( lpszFileName, mode ) == TRUE )
	{
#ifdef __SECURITY_0628
#ifndef _DEBUG
#ifdef __MAINSERVER
		CString strFileName		= lpszFileName;
		if( strFileName.Find( "\\", 0 ) < 0 )
		{
			::Error( "killed by CResFile::Open() %s, %s, 2", prj.GetText(TID_GAME_RESOURCE_MODIFIED), lpszFileName );
			ExitProcess( -1 );
		}
#endif	// __MAINSERVER
#endif	// _DEBUG
#endif	// __SECURITY_0628
		return TRUE;
	}
	char szSerchPath[MAX_PATH];
	GetCurrentDirectory( sizeof( szSerchPath ), szSerchPath );
	TCHAR drive[_MAX_DRIVE], dir[_MAX_DIR], name[ _MAX_FNAME ], ext[_MAX_EXT];
	_splitpath( lpszFileName, drive, dir, name, ext );
	
	TCHAR szFileName[ MAX_PATH ];
	RESOURCE* lpRes;
	strcpy( szFileName, lpszFileName );
	strlwr( szFileName );
	m_bResouceInFile = FALSE;
	if( m_mapResource.Lookup( szFileName, (void*&) lpRes ) )
	{
		CFileException fileExc;
		if( m_File.Open( lpRes->szResourceFile, CFile::modeRead | CFile::shareDenyNone, &fileExc ) )
		{
			m_nFileSize = lpRes->dwFileSize;
			m_nFileBeginPosition = lpRes->dwOffset;
			m_nFileCurrentPosition = lpRes->dwOffset;
			m_nFileEndPosition = lpRes->dwOffset + m_nFileSize;
			m_byEncryptionKey = lpRes->byEncryptionKey;
			m_bEncryption = lpRes->bEncryption;
			m_File.Seek( m_nFileCurrentPosition, CFile::begin );
			m_bResouceInFile = TRUE;
#ifdef __SECURITY_0628
			lstrcpy( m_szFileName, szFileName );
#endif	// __SECURITY_0628
			//m_File.Close();
			return TRUE;
		}
		
		TCHAR szCause[255];
		fileExc.GetErrorMessage(szCause, 255);

		::Error( "CResFile Open Error %s FileName = %s, Resource = %s, CurrentDir = %s\n", 
			szCause,
			szFileName, 
			lpRes->szResourceFile, 
			szSerchPath );

		return FALSE;
	}
	return FALSE;
}

LPVOID CResFile::Read( void )
{
	if( !m_bResouceInFile )
		return CFileIO::Read();
	else
	{
		LONG size = m_nFileSize;
		LPVOID ptr = new BYTE[ size ];
		if( !ptr ) 
			return NULL;

		m_File.Read( ptr, size );
		m_nFileCurrentPosition += size;

		if( IsEncryption() )
		{
			for( int i = 0; i < size;  i++ )
				((BYTE*)ptr)[ i ] = Decryption( m_byEncryptionKey, ((BYTE*)ptr)[ i ] );
		}
		return ptr;
	}
}

size_t CResFile::Read( void *ptr, size_t size, size_t n /* = 1  */ )
{
	if( !m_bResouceInFile )
		return CFileIO::Read( ptr, size, n );
	else
	{
		size_t size_  = m_File.Read( ptr, size * n );
		m_nFileCurrentPosition += size_;
		if( IsEncryption() )
		{
			for( int i = 0; i < (int)( size_ );  i++ )
				((BYTE*)ptr)[ i ] = Decryption( m_byEncryptionKey, ((BYTE*)ptr)[ i ] );
		}
#ifdef __SECURITY_0628
		CString strResFile	= m_File.GetFileName();
		CString strFile	= m_szFileName;

		if( strFile.Find( "\\", 0 ) < 0 )
		{
			char sFile[100]	= { 0,};
			char sData[100]	= { 0,};
			md5( sFile, m_szFileName );
			md5( sData, (BYTE*)ptr, size );
			map<string, string>::iterator i	= CResFile::m_mapAuth.find( sFile );
			if( i != CResFile::m_mapAuth.end() )
			{
				if( lstrcmp( sData, i->second.data() ) != 0 )
				{
					::Error( "killed by CResFile::Read() %s, 3", m_szFileName );
					ExitProcess( -1 );
				}
			}
		}
#endif	// __SECURITY_0628
		return size_;
	}
}

long CResFile::GetLength( void )
{
	if( !m_bResouceInFile )
		return CFileIO::GetLength();
	else
		return m_nFileSize;
}

int CResFile::Seek( long offset, int whence )
{
	if( !m_bResouceInFile )
		return CFileIO::Seek( offset, whence );
	else
	{
		if( whence == SEEK_SET )
		{
			m_nFileCurrentPosition = m_nFileBeginPosition + offset;
			m_File.Seek( m_nFileCurrentPosition, CFile::begin );
		}
		else 
		if( whence == SEEK_END )
		{
			m_nFileCurrentPosition = m_nFileEndPosition - offset;
			m_File.Seek( m_nFileCurrentPosition, CFile::end );//CFile::current );
		}
		else 
		if( whence == SEEK_CUR )
		{
			m_nFileCurrentPosition += offset;
			//m_File.GetPosition() + offset
			int n = (int)( m_File.GetPosition() );
			m_File.Seek( offset, CFile::current );
		}
		else
			return -1;
		return m_nFileCurrentPosition;
	}
}

long CResFile::Tell()
{
	if( !m_bResouceInFile )
		return CFileIO::Tell();
	else
		return m_nFileCurrentPosition - m_nFileBeginPosition;
}

char CResFile::GetC( void )
{
	if( !m_bResouceInFile )
		return CFileIO::GetC();
	else
	{
		char c;
		m_File.Read( &c, 1 );
		m_nFileCurrentPosition += sizeof( c );
		// 암호화일 경우 암호를 푼다.
		if( IsEncryption() )
			c = Decryption( m_byEncryptionKey, c );
		return c;
	}
}

WORD CResFile::GetW( void )
{
	if( !m_bResouceInFile )
		return CFileIO::GetW();
	else
	{
		WORD w;
		m_File.Read( &w, sizeof( w ) );
		m_nFileCurrentPosition += sizeof( w );
		if( IsEncryption() )
		{
			BYTE h, l;
			h = Decryption( m_byEncryptionKey, w >> 8 );
			l = Decryption( m_byEncryptionKey, w & 0x00ff );
			return h << 8 | l;
		}
		return w;
	}
}

#pragma warning( disable : 4700 ) // CResFile::GetDW 함수내의 초기화 warning을 무시합니다.

DWORD CResFile::GetDW( void )
{
	if( !m_bResouceInFile )
		return CFileIO::GetDW();
	else
	{
		DWORD dw;
		m_File.Read( &dw, sizeof( dw ) );
		m_nFileCurrentPosition += sizeof( dw );
		if( IsEncryption() )
		{
			BYTE a,b,c,d;
			a = Decryption( m_byEncryptionKey, ( a & 0xff000000 ) >> 24 );
			b = Decryption( m_byEncryptionKey, ( b & 0x00ff0000 ) >> 16 );
			c = Decryption( m_byEncryptionKey, ( c & 0x0000ff00 ) >> 8 );
			d = Decryption( m_byEncryptionKey, ( d & 0x000000ff ) );
			return a << 24 | b << 16 | c << 8 | d;
		}
		return dw;
	}
}
#pragma warning( default : 4700 ) // 다시 되돌립니다.


int	CResFile::Flush( void )
{
	if( !m_bResouceInFile )
		return CFileIO::Flush();
	else
	{
		m_File.Flush();
		return 0;
	}
}
#endif	// __CLIENT

CFileFinder::CFileFinder()
{
	m_pos = 0;
	m_lHandle = 0;
	m_bResFile = FALSE;
}
CFileFinder::~CFileFinder()
{
}

BOOL CFileFinder::WildCmp( LPCTSTR lpszWild, LPCTSTR lpszString ) 
{
	char* wild = (char*)lpszWild;
    char* string = (char*)lpszString;
	char* cp,* mp;

	while ((*string) && (*wild != '*')) 
	{
		if ((*wild != *string) && (*wild != '?')) 
		{
			return 0;
		}
		wild++;
		string++;
	}
	while (*string) 
	{
		if (*wild == '*') 
		{
			if (!*++wild) 
			{
				return 1;
			}
			mp = wild;
			cp = string+1;
		} 
		else 
		if ((*wild == *string) || (*wild == '?')) 
		{
			wild++;
			string++;
		} 
		else 
		{
			wild = mp;
			string = cp++;
		}
	}
	while (*wild == '*') 
	{
		wild++;
	}
	return !*wild;
}

BOOL CFileFinder::FindFirst( LPCTSTR lpFilespec, struct _finddata_t *fileinfo )
{
#if defined( __CLIENT )
	CHAR filespec[ MAX_PATH ];
	strcpy( filespec, lpFilespec );
	strlwr( filespec );

	CHAR szPath [ MAX_PATH ];
	CHAR szFile[ MAX_PATH ];

	TCHAR drive[_MAX_DRIVE], dir[_MAX_DIR], name[ _MAX_FNAME ], ext[_MAX_EXT];
	_splitpath( lpFilespec, drive, dir, name, ext );

	// 드라이브명이 들어 있으면 완전한 path다. 그러므로 그냥 카피 
	if( drive[ 0 ] )
	{
		_tcscpy( m_szFilespec, lpFilespec );
	}
	// 드리이브명이 없으면 현재 path의 연장이다. 
	else
	{
		::GetCurrentDirectory( MAX_PATH, szPath );
		_tcscpy( m_szFilespec, szPath );
		_tcscat( m_szFilespec, "\\" );
		_tcscat( m_szFilespec, lpFilespec );
	}
	strlwr( m_szFilespec );

	CString strNameName;
	RESOURCE* lpRes;
	m_pos = CResFile::m_mapResource.GetStartPosition();
	while( m_pos )
	{
		CResFile::m_mapResource.GetNextAssoc( m_pos, strNameName, (void*&) lpRes );
		_tcscpy( szFile, szPath );
		_tcscat( szFile, "\\" );
		_tcscat( szFile, strNameName );
		strlwr( szFile );

		if( WildCmp( m_szFilespec, szFile ) )
		{
			_splitpath( strNameName, drive, dir, name, ext );
			strcpy( fileinfo->name, name );
			strcat( fileinfo->name, ext );
			fileinfo->size = lpRes->dwFileSize;
			fileinfo->attrib = _A_NORMAL;
			m_bResFile = TRUE;
			return TRUE;
		}
	}
#endif	// __CLIENT
	m_bResFile = 0;
	m_lHandle = _findfirst( lpFilespec, fileinfo );
	if( m_lHandle == -1 ) 
		return FALSE;
	return TRUE;
}
BOOL CFileFinder::FindNext( struct _finddata_t *fileinfo )
{
#if defined( __CLIENT )
	if( m_bResFile == 1 )
	{
		TCHAR drive[_MAX_DRIVE], dir[_MAX_DIR], name[ _MAX_FNAME ], ext[_MAX_EXT];
		CHAR szPath [MAX_PATH];
		CHAR szFile[MAX_PATH];
		CString strNameName;
		RESOURCE* lpRes;
		::GetCurrentDirectory( MAX_PATH, szPath );

		while( m_pos )
		{
			CResFile::m_mapResource.GetNextAssoc( m_pos, strNameName, (void*&) lpRes );
			_tcscpy( szFile, szPath );
			_tcscat( szFile, "\\" );
			_tcscat( szFile, strNameName );
			strlwr( szFile );

			if( WildCmp( m_szFilespec, szFile ) )
			{
				_splitpath( strNameName, drive, dir, name, ext );
				strcpy( fileinfo->name, name );
				strcat( fileinfo->name, ext );
				fileinfo->size = lpRes->dwFileSize;
				fileinfo->attrib = _A_NORMAL;
				return TRUE;
			}
		}
		return FALSE;
	}
#endif	// __CLIENT
	if( _findnext( m_lHandle, fileinfo ) == 0 )
		return TRUE;
	return FALSE;
}
void CFileFinder::FindClose()
{
	if( m_bResFile == 0 && m_lHandle != -1 )
		_findclose( m_lHandle );
}


#ifdef __SECURITY_0628
#ifdef __CLIENT
void	CResFile::LoadAuthFile( void )
{
	CFile a;
	if( a.Open( "Flyff.a", CFile::modeRead ) == FALSE )
		return;
	int nLen	= (int)( a.GetLength() );
	BYTE* ptr	= new BYTE[nLen];
	a.Read( ptr, nLen );
	int nOffset	= 0;
	char sFile[100]	= { 0,};
	char sData[100]	= { 0,};
	while( nOffset < nLen )
	{
		memcpy( sFile, ptr + nOffset, 32 );
		sFile[32]	= '\0';
		nOffset	+= 32;
		memcpy( sData, ptr + nOffset, 32 );
		sData[32]	= '\0';
		nOffset	+= 32;
		TRACE( "%s%s\n", sFile, sData );
		bool bResult	= CResFile::m_mapAuth.insert( map<string, string>::value_type( sFile, sData ) ).second;
	}
	md5( m_szResVer, ptr, nLen );
	safe_delete_array( ptr );
	a.Close();
}
#endif	// __CLIENT
#endif	// __SECURITY_0628
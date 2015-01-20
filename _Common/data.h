#ifndef __DATA_H
#define __DATA_H

#pragma once

const DWORD NULL_ID	= 0xffffffff;


template <class T> class CStack
{
protected:
	T*	m_pBlock;
	int	m_nSize;
	int	m_nGrowSize;
	int	m_nTop;

public:
	CStack( int nSize = 1024, int nGrowSize = 1024 );
	
	virtual	~CStack() { SAFE_DELETE_ARRAY( m_pBlock );	}

	void	SetSize( int nSize, int nGrowSize = 1024 );
	int		GetSize() { return m_nSize; }
	int		GetCount( void ) { return m_nTop; }
	void	Push( T data );
	T	    Pop( void );
	T	    Peek( void );
	void	Clear() { m_nTop = 0; }
	BOOL	IsFull( void )	{ return ( m_nTop >= m_nSize ); }
	BOOL	IsEmpty( void )	{ return ( m_nTop == 0 ); }
};
template <class T> inline CStack<T>::CStack( int nSize, int nGrowSize )
{
	m_nTop = 0;
	m_nGrowSize		= nGrowSize;
	m_pBlock	= new T[m_nSize = nSize];
}

template <class T> inline void CStack<T>::SetSize( int nSize, int nGrowSize )
{
	m_nTop	= 0;
	SAFE_DELETE_ARRAY( m_pBlock );
	m_nGrowSize		= nGrowSize;
	m_pBlock	= new T[m_nSize = nSize];
}

template <class T> inline void CStack<T>::Push( T data )
{
	if( m_nTop >= m_nSize )
	{
		WriteLog( "%s, %d", __FILE__, __LINE__ );
		return;
	}
	m_pBlock[m_nTop++]	= data;

	if( IsFull() && m_nGrowSize > 0 )
	{
		T* pOld	= m_pBlock;
		m_pBlock = new T[m_nSize + m_nGrowSize];
		memcpy( m_pBlock, pOld, m_nTop * sizeof(T) );
		SAFE_DELETE_ARRAY( pOld );
		m_nSize		+= m_nGrowSize;
	}
}

template <class T> inline T CStack<T>::Pop( void )
{
	if( m_nTop < 1 )
	{
		WriteLog( "%s, %d", __FILE__, __LINE__ );
	}
	return m_pBlock[--m_nTop];
}

template <class T> inline T CStack<T>::Peek( void )
{
	if( m_nTop < 1 )
	{
		WriteLog( "%s, %d", __FILE__, __LINE__ );
	}
	return m_pBlock[m_nTop - 1];
}


typedef CStack<DWORD>  CDWordStack; 
typedef CStack<WORD>   CWordStack; 
typedef CStack<CHAR>   CCharStack; 
typedef CStack<UINT>   CUIntStack; 
typedef CStack<LONG>   CLongStack; 
typedef CStack<short>  CShortStack; 
typedef CStack<int>    CIntStack; 

template <class T> class CQueue 
{
	int m_nSize;
	int m_nHead;
	int	m_nTail;

	T* m_pData;
public:
	CQueue(int nSize = 1024);
	~CQueue() { safe_delete_array( m_pData ); }
	void SetSize(int nSize);
	int  GetSize() { return m_nSize; }
	int GetCount(void)
	{ 
		if (m_nHead >= m_nTail)
			return (m_nHead - m_nTail);
		else
			return (m_nSize - 1 - m_nTail + m_nHead);
	}
	void Clear() { m_nHead = 0; m_nTail = 0; } 
	void Push(T data);
	T Pop();
	T GetPop();

private:
	void IncreaseIndex(int	&nIndex)
	{
		nIndex++;
		if (nIndex >= m_nSize)
			nIndex = 0;
	}
	BOOL IsFull(void)
	{
		if (GetCount() == m_nSize-1)
			return TRUE;
		return FALSE;
	}
	BOOL IsEmpty(void)
	{
		if (m_nHead == m_nTail)
			return TRUE;
		return FALSE;
	}
};

template <class T> inline CQueue<T>::CQueue(int nSize) 
{
	m_nHead = 0;
	m_nTail = 0;
	m_pData = new T[m_nSize = nSize];
}

template <class T> inline void CQueue<T>::SetSize(int nSize) 
{
	m_nTop = 0;
	safe_delete_array( m_pData );
	m_pData = new T[m_nSize = nSize];
}

template <class T> inline void CQueue<T>::Push(T data) 
{
	if (IsFull())
	{
		T* pOld = m_pData;
		m_pData = new T[m_nSize + 1024];
		if (m_nHead >= m_nTail)
		{
			memcpy(m_pData, &pOld[m_nTail], (m_nHead-m_nTail)*sizeof(T));
			m_nHead = m_nHead-m_nTail;
			m_nTail = 0;
		}
		else
		{
			memcpy(m_pData, &pOld[m_nTail], (m_nSize-1-m_nTail)*sizeof(T));
			memcpy(&m_pData[m_nSize-1-m_nTail], pOld, m_nHead*sizeof(T));
			m_nHead = (m_nSize-1-m_nTail)+m_nHead;
			m_nTail = 0;
		}
		safe_delete_array( pOld );
		m_nSize += 1024;
	}
	m_pData[m_nHead] = data;
	IncreaseIndex(m_nHead);
}

template <class T> inline T CQueue<T>::Pop() 
{
	T *p;
	if( IsEmpty() )
	{
		WriteLog( "%s, %d", __FILE__, __LINE__ );
	}
	p =  m_pData[m_nTail];
	IncreaseIndex(m_nTail);
	return p;
}

template <class T> inline T CQueue<T>::GetPop() 
{
	if( IsEmpty() )
	{
		WriteLog( "%s, %d", __FILE__, __LINE__ );
	}
	return m_pData[m_nTail];
}

template <class T> class CFixedArray
{
protected:
	LPBYTE	m_pData;
	LPBYTE* m_apOffset;
	int m_nCurOffset;
	int m_nCurNum;
	int m_nMaxByte;
	int m_nMaxIndex;

	int m_nEndByte;
	int m_nEndIndex;

public:
	CFixedArray();
	
	virtual	~CFixedArray();
	int  GetSize() { return m_nMaxIndex; }
	void SetSize( int nNum, int nMaxIndex );
	void Add( T* pData, int nSize = 0 );
	void SetAtGrow( int nIndex, T* pData, int nSize = 0 );
	T* GetAt( DWORD dwIndex );
	void Optimize();
	void RemoveAll();
};
template <class T> inline CFixedArray<T>::CFixedArray( )
{
	m_pData = NULL;
	m_apOffset = NULL;
	m_nEndByte = 0;
	m_nEndIndex = -1;
	SetSize( 100, 100 );
}
template <class T> inline CFixedArray<T>::~CFixedArray( )
{
	SAFE_DELETE_ARRAY( m_apOffset );
	SAFE_DELETE_ARRAY( m_pData ); 
}
template <class T> inline void CFixedArray<T>::RemoveAll()
{
	SAFE_DELETE_ARRAY( m_apOffset );
	SAFE_DELETE_ARRAY( m_pData ); 
	m_pData = NULL;
	m_apOffset = NULL;
	m_nEndByte = 0;
	m_nEndIndex = -1;
	SetSize( 100, 100 );
}
template <class T> inline void CFixedArray<T>::SetSize( int nMaxNum, int nMaxIndex )
{
	m_pData = new BYTE[ sizeof( T ) * nMaxNum ];
	m_apOffset = new LPBYTE [ nMaxIndex ];
	ZeroMemory( m_apOffset, sizeof( LPBYTE ) * nMaxIndex );
	m_nMaxByte = sizeof( T ) * nMaxNum;
	m_nMaxIndex = nMaxIndex;
	m_nCurOffset = 0;
	m_nCurNum = 0;
}
template <class T> inline T* CFixedArray<T>::GetAt( DWORD dwIndex )
{
	if( dwIndex >= (DWORD)m_nMaxIndex )
		return NULL;
	return (T*) m_apOffset[ dwIndex ];
}
template <class T> inline void CFixedArray<T>::Add( T* pData, int nSize )
{
	SetAtGrow( m_nCurNum, pData, nSize );
	m_nCurNum++;
}
template <class T> void CFixedArray<T>::Optimize()
{
	if( m_nMaxIndex >= m_nEndIndex )
	{
		m_nMaxIndex = m_nEndIndex + 1;
		LPBYTE* apOffset = new LPBYTE [ m_nMaxIndex ]; 
		memcpy( apOffset, m_apOffset, sizeof( LPBYTE ) * m_nMaxIndex ); 
		SAFE_DELETE_ARRAY( m_apOffset );
		m_apOffset = apOffset;
	}
	if( m_nMaxByte > m_nCurOffset )
	{
		LPBYTE pData = new BYTE[ m_nCurOffset ]; 
		memcpy( pData, m_pData, m_nCurOffset ); 
		for( int i = 0; i <= m_nEndIndex; i++ )
			if( m_apOffset[ i ] )
				m_apOffset[ i ] = ( m_apOffset[ i ] - m_pData ) + pData; 
		SAFE_DELETE_ARRAY( m_pData );
		m_pData = pData;
		m_nMaxByte = m_nCurOffset ;
	}
}
template <class T> void CFixedArray<T>::SetAtGrow( int nIndex, T* pData, int nSize )
{
	if( nIndex >= m_nMaxIndex )
	{
		LPBYTE* apOffset = new LPBYTE [ nIndex * 2 ]; // 두배 확장 
		ZeroMemory( apOffset, sizeof( LPBYTE ) * nIndex * 2 ); // 초기화 
		memcpy( apOffset, m_apOffset, sizeof( LPBYTE ) * ( m_nEndIndex + 1 ) ); // 복사 
		SAFE_DELETE_ARRAY( m_apOffset );
		m_apOffset = apOffset;
		m_nMaxIndex = nIndex * 2;
	}
	int nSizeTemp 
		= nSize ? nSize : sizeof( T );
	if( m_nCurOffset + nSizeTemp > m_nMaxByte )
	{
		m_nMaxByte = m_nCurOffset * 2 + nSizeTemp;
		LPBYTE pData2 = new BYTE[ m_nMaxByte ]; // 두배 확장 
		memcpy( pData2, m_pData, m_nCurOffset ); // 복사 
		for( int i = 0; i <= m_nEndIndex; i++ )
			if( m_apOffset[ i ] )
				m_apOffset[ i ] = ( m_apOffset[ i ] - m_pData ) + pData2; 
		SAFE_DELETE_ARRAY( m_pData );
		m_pData = pData2;
	}
	if( nSize == 0 )
	{
		if( m_apOffset[ nIndex ] == NULL )
		{
			memcpy( &m_pData[ m_nCurOffset ], pData, sizeof( T ) );
			m_apOffset[ nIndex ] = (LPBYTE) m_pData + m_nCurOffset;
			m_nCurOffset += sizeof( T );
		}
		else
		{
			memcpy( m_apOffset[ nIndex ], pData, sizeof( T ) );
		}
	}
	else
	{
		if( m_apOffset[ nIndex ] == NULL )
		{
			memcpy( &m_pData[ m_nCurOffset ], pData, nSize );
			m_apOffset[ nIndex ] = (LPBYTE) m_pData + m_nCurOffset;
			m_nCurOffset += nSize;
		}
		else
		{
			WriteLog( "%s, %d", __FILE__, __LINE__ );
		}
	}
	if( nIndex > m_nEndIndex )
		m_nEndIndex = nIndex;
}

typedef	__int64	EXPINTEGER;
typedef	double	EXPFLOAT;

#if defined(__WORLDSERVER ) || defined(__CLIENT)
struct NaviPoint 
{
	D3DXVECTOR3 Pos;		
	short		Time;			
	DWORD		objid;		
	string      Name;
	NaviPoint()
	{
		Pos = D3DXVECTOR3(0.f, 0.f, 0.f);
		objid = 0xffffffff;
		Time = 0;
	}
	
	NaviPoint& operator=( const NaviPoint& src )
	{
		Pos   = src.Pos;
		objid = src.objid;
		Time  = src.Time;
		Name  = src.Name;

		return *this;
	}
}; 

typedef vector<NaviPoint> V_NaviPoint;

#endif // defined(__WORLDSERVER ) || defined(__CLIENT)

namespace SAI79
{
	enum	ePropType 	{ NO_PROP = 0, FIRE, WATER, ELECTRICITY, WIND, EARTH, END_PROP };
} // namespace SAI79

#endif

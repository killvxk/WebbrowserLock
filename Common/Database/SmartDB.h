#if !defined(__SMARTDB_)
#define __SMARTDB_

#if _MSC_VER > 1000
#pragma once
#endif

// SmartDB.h : header file
//

#define ASSERT

#define RSOPEN_SUCCESS			0
#define RSOPEN_NOLIBLOADED		1
#define RSOPEN_NOCONNECT		2
#define RSOPEN_ALREADYOPENED	3
#define RSOPEN_INVALIDQRY		4

#include <vector>
#include <string>
#include "sqlite3.h"
#include <Function.Internal.CommonControl\CommonControlTool.h>

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CSmartDB thread

class CSmartDB : CCommonControl
{
	// Attributes
	public:
		CSmartDB();           // constructor and distructor
		~CSmartDB();

	// Operations
	public:
		BOOL Connect (LPCTSTR strDBPathName, LPCTSTR strParamString = NULL);
		BOOL InitInstance();
		BOOL Close (void);
		BOOL IsConnected (void);
		UINT Execute (LPCTSTR strQuery, UINT * nRecEfected = NULL);
		BOOL IsLibLoaded (void);

		sqlite3 *db;

	// Implementation
	private:
		BOOL InitLibrary(void);

		BOOL m_bLibLoaded;
		BOOL m_bConnected;
};


/////////////////////////////////////////////////////////////////////////////
// CSmartDBRecordSet thread

class CSmartDBRecordSet : CCommonControl
{
public:
	static LPCTSTR Type[];
	// Attributes
	public:
		CSmartDBRecordSet();           // protected constructor used by dynamic creation
		virtual ~CSmartDBRecordSet();

	// Operations
	public:
		BOOL Close (void);
		UINT Open (LPCTSTR strQueryString, CSmartDB * ptrConn, LONG nRSType = NULL);
		BOOL InitInstance();



		//////////////////////////////////////////////////////////////////////////
		//	Explain: 得到列数量 FieldsCount (void)
		//	Return: 返回当前表中列的总数量

		UINT FieldsCount (void);

		//////////////////////////////////////////////////////////////////////////
		//	Explain: 得到列名称 GetFieldName (LONG)
		//	Parameter:
		//		nIndex 取得名称的列索引
		//	Return: 返回列的名称

		LPCTSTR GetFieldName (LONG nIndex);

		//////////////////////////////////////////////////////////////////////////
		//	Explain: 得到列数据类型 GetFieldType (LONG)
		//	Parameter:
		//		nIndex 取得数据类型的列索引
		//	Return: 返回数据类型索引

		INT GetFieldType (LONG nIndex);



		
		//////////////////////////////////////////////////////////////////////////
		//	Explain: 得到记录数量 RecordCount (void)
		//	Return: 返回当前记录的总数量

		LONG RecordCount (void);

		//////////////////////////////////////////////////////////////////////////
		//	Explain: 转到下一条记录 MoveNext (void)
		//	Return: 无返回

		void MoveNext (void);

		//////////////////////////////////////////////////////////////////////////
		//	Explain: 转到上一条记录 MovePrevious (void)
		//	Return: 无返回

		void MovePrevious (void);

		//////////////////////////////////////////////////////////////////////////
		//	Explain: 转到最后一条记录 MoveLast (void)
		//	Return: 无返回

		void MoveLast (void);

		//////////////////////////////////////////////////////////////////////////
		//	Explain: 转到第一条记录 MoveFirst (void)
		//	Return: 无返回

		void MoveFirst (void);




		//////////////////////////////////////////////////////////////////////////
		//	Explain: 判断是否结束 IsEOF (void)
		//	Return: 结束返回真,其他返回假

		BOOL IsEOF (void);

		//////////////////////////////////////////////////////////////////////////
		//	Explain: 得到当前记录指定列的内容 GetColumnString (LONG)
		//	Parameter:
		//		nIndex 取得内容的列索引
		//	Return: 返回列的内容

		LPCTSTR GetColumnString (LONG nIndex);

	// Implementation
	private:
		BOOL m_bIsOpen;
		vector<string>  m_strFieldsList;
		vector<int> m_nFieldsType;
		vector<vector<string> * > orsRows;
		LONG m_nFieldsCount;
		BOOL m_bIsProcessing;
		LONG m_nCurrentPage;
		LONG m_nPageCount;
		LONG m_nPageSize;
		LONG m_nCurrentRecord;
		LONG m_nRecordCount;

#ifdef UNICODE
		wstring m_strRet;
#else
		string m_strRet;
#endif
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(__SMARTDB_)

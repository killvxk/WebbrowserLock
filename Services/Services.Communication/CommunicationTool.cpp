#include <Windows.h>
#include <Wtsapi32.h>
#include <stdio.h>
#include <tchar.h>

#include "CommunicationTool.h"


#pragma comment(lib ,"Wtsapi32.lib")
#pragma comment(lib ,DIRECTORY_LIB "Function\\Function.Internal.SocketControl.lib")
#pragma comment(lib ,DIRECTORY_LIB "Function\\Function.Internal.RegistryControl.lib")
#pragma comment(lib ,DIRECTORY_LIB "Function\\Function.Internal.FileControl.lib")


CResourceServicesObj cResourceServicesObj;
CCommandServicesObj	 cCommandServicesObj;

EXTERN_C FUNCTION_API void CommunicationStart()
{
	Debug(Dbg,_T("ResourceServicesObj.Start()"));
	cResourceServicesObj.Start();
	Debug(Dbg,_T("CommandServicesObj.Start()"));
	cCommandServicesObj.Start();
	Debug(Dbg,_T("Services Start End."));
}


EXTERN_C FUNCTION_API void CommunicationStop()
{
	cResourceServicesObj.Stop();
	cResourceServicesObj.WaitStop();

	cCommandServicesObj.Stop();
	cCommandServicesObj.WaitStop();
}


EXTERN_C FUNCTION_API void Test()
{

	CSmartDB cSmartDB;

	if (cSmartDB.IsConnected())
	{
		cSmartDB.Close();
	}

	cSmartDB.Connect(_T("E:\\Temp\\x3py-master\\Debug\\Server.db"));


	//cSmartDB.Execute(_T("SELECT * FROM PLUGINS"));
	CSmartDBRecordSet rsSmartDB;

	if (rsSmartDB.Open (_T("SELECT * FROM PLUGINS"), &cSmartDB) == RSOPEN_SUCCESS)
	{
		while (!rsSmartDB.IsEOF())
		{
			OutputDebugString(_T("\r\n"));
			for (int i=0; i < rsSmartDB.FieldsCount(); i++)
			{
				OutputDebugString(rsSmartDB.GetFieldName(i));
				OutputDebugString(_T(" : "));
				OutputDebugString(rsSmartDB.GetColumnString(i));
				OutputDebugString(_T("\r\n"));

			}
			rsSmartDB.MoveNext();
		}
	}

// 	Debug(Dbg,_T("ResourceServicesObj.Start()"));
// 	cResourceServicesObj.Start();
// 	Debug(Dbg,_T("CommandServicesObj.Start()"));
// 	cCommandServicesObj.Start();
// 	Debug(Dbg,_T("Services Start End."));
// 
// 	while(true)
// 	{
// 		Sleep(1000);
// 	}
}



bool MessageBoxForServer(LPTSTR pszMessageText, LPTSTR pszPromptText,UINT uType)
{
	bool bSuccess = false;
	try
	{
		DWORD dwRet = 0;
		DWORD dwSessionId = WTSGetActiveConsoleSessionId();

		bSuccess = WTSSendMessage(0, dwSessionId,pszPromptText,(NULL == pszPromptText) ? 0 : _tcslen(pszPromptText), pszMessageText, (NULL == pszMessageText) ? 0 : _tcslen(pszMessageText), uType, 0, &dwRet,true);

	}
	catch(...)
	{
		return false;
	}
	return bSuccess;
}
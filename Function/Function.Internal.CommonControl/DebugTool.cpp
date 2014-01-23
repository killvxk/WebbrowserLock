#include <windows.h>
#include <stdio.h>
#include <tchar.h>


#include "DebugTool.h"
#include <shlwapi.h>
#include <psapi.h>
#include <TlHelp32.h>
#include <shlobj.h>

CDebugTool g_cDebugTool(_T("System.Runtime"));

#pragma comment (lib, "psapi.lib")
#pragma comment (lib, "Shlwapi.lib")


CDebugTool::CDebugTool(LPCTSTR lpLogFileName) : m_pErrorMsg(NULL)
{
	memset(m_szLogFilePath,0,MAX_PATH);
	m_pszBufferA = (char *)malloc((PAGE_SIZE+1)*sizeof(char));
	m_pszBufferW = (wchar_t *)malloc((PAGE_SIZE+1)*sizeof(wchar_t));

	GetModuleFileName(NULL,m_szLogFilePath,MAX_PATH);
	PathRemoveFileSpec(m_szLogFilePath);
	PathAddBackslash(m_szLogFilePath);
	_tcscat_s(m_szLogFilePath,_T("Debug_Log"));
	if (PathFileExists(m_szLogFilePath))
	{
		PathAddBackslash(m_szLogFilePath);
		_stprintf(m_szLogFilePath,_T("%s%d_%s.log"),m_szLogFilePath,GetCurrentProcessId(),lpLogFileName);
	}else
	{
		memset(m_szLogFilePath,0,MAX_PATH);
	}


	FILE * hDebugFileHandle = NULL;
	TCHAR szDebugFileName[MAX_PATH+1] = {0};
	GetModuleFileName(NULL,szDebugFileName,MAX_PATH);
	PathRemoveFileSpec(szDebugFileName);
	PathAddBackslash(szDebugFileName);
	_tcscat_s(szDebugFileName,_T("Debug"));
	_tfopen_s(&hDebugFileHandle,szDebugFileName,_T("r"));
	if (NULL != hDebugFileHandle)
	{
		m_bDebug = TRUE;
		fclose(hDebugFileHandle);
	}else
	{
		m_bDebug = FALSE;
	}
}


CDebugTool::~CDebugTool(void)
{
	if(NULL != m_pErrorMsg)  
	{  
		LocalFree(m_pErrorMsg);  
		m_pErrorMsg = NULL;  
	} 
	if (NULL != m_pszBufferA)
	{
		free(m_pszBufferA);
	}
	if (NULL != m_pszBufferW)
	{
		free(m_pszBufferW);
	}
}

LPCTSTR CDebugTool::Show(DWORD dwErrorId)
{  
	bool bRet = false;
	if(NULL != m_pErrorMsg)  
	{  
		//LocalUnlock(m_pErrorMsg);
		LocalFree(m_pErrorMsg);  
		m_pErrorMsg = NULL;  
	}  

	if(!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,NULL, dwErrorId, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), (LPWSTR)&m_pErrorMsg, 0, NULL))  
	{  
		Out(Errors,_T("%s"),dwErrorId); return NULL;
	}else if (NULL == m_pErrorMsg)
	{
		Out(Errors,_T("%s"),dwErrorId); return NULL;
	}

	return (LPCTSTR)m_pErrorMsg;
}   

void CDebugTool::SetLog(const char * lpDebug,const char * lpString) const
{
	FILE * FileHandle;

	_tfopen_s(&FileHandle,m_szLogFilePath,_T("at+"));
	if (NULL != FileHandle)
	{
		if (m_bDebug)
		{
			fwrite(lpDebug,sizeof(CHAR),strlen(lpDebug),FileHandle);
		}
		fwrite(lpString,sizeof(CHAR),strlen(lpString),FileHandle);
		fclose(FileHandle);
		FileHandle = NULL;
	}
}

void CDebugTool::OutW(const char * lpFile,DWORD dwLine,const char * Function,LPCWSTR pszOutLevel,_In_z_ _Printf_format_string_ const wchar_t * lpOutputString, ...) const
{
	DWORD dwStart = wcslen(pszOutLevel)+2;
	CHAR strDbgLine[512] = {0};
#ifdef _DEBUG

	//wsprintfA(strDbgLine,"FILENAME:[%s] LINE:%d FUNCTION:[%s]\r\n",lpFile,dwLine,Function);
	wsprintfA(strDbgLine,"[%s]	\t",Function);
#endif


	va_list argList;  
	va_start(argList, lpOutputString);  
	vswprintf_s(&m_pszBufferW[dwStart],PAGE_SIZE - dwStart, lpOutputString, argList);  
	va_end(argList);

	wcscat_s(m_pszBufferW,PAGE_SIZE,L"\r\n\0");

	//OutputDebugString(szHeader);
	m_pszBufferW[0] = L'[';
	wcsncpy(&m_pszBufferW[1],pszOutLevel,dwStart-1);
	m_pszBufferW[dwStart-1] = L']';

	WideCharToMultiByte( CP_ACP, 0, m_pszBufferW, -1,m_pszBufferA, PAGE_SIZE, NULL, NULL ); 
	SetLog(strDbgLine,m_pszBufferA);

}

void CDebugTool::OutA(const char * lpFile,DWORD dwLine,const char * Function,LPCSTR pszOutLevel,_In_z_ _Printf_format_string_ const char * lpOutputString, ...) const
{
	DWORD dwStart = strlen(pszOutLevel)+2;
	CHAR strDbgLine[512] = {0};

#ifdef _DEBUG

	wsprintfA(strDbgLine,"[%s]	\t",Function);
#endif



	va_list argList;  
	va_start(argList, lpOutputString);  
	vsprintf_s(&m_pszBufferA[dwStart],PAGE_SIZE-dwStart, lpOutputString, argList);  
	va_end(argList);
	
	strcat_s(m_pszBufferA,PAGE_SIZE,"\r\n\0");

	m_pszBufferA[0] = '[';
	strncpy(&m_pszBufferA[1],pszOutLevel,dwStart-1);
	m_pszBufferA[dwStart-1] = ']';

	//OutputDebugStringA(szHeader);
	SetLog(strDbgLine,m_pszBufferA);
}
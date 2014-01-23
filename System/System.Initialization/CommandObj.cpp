#include "CommandTool.h"

//////////////////////////////////////////////////////////////////////////
// 引用自身
#include "CommandObj.h"
#include <stdlib.h>
#include <time.h>

//////////////////////////////////////////////////////////////////////////
// 引用库文件

//////////////////////////////////////////////////////////////////////////
// 初始化及释放函数

CPluginManager CCommandObj::m_Plugin;

CCommandObj::CCommandObj(void) : CCommonControl(_T("System.Initialization"))
{
	m_PluginLoader = NULL;
}
CCommandObj::CCommandObj(LPCTSTR lpLogFileName) : CCommonControl(lpLogFileName)
{
	m_PluginLoader = NULL;
}

CCommandObj::~CCommandObj(void)
{

	//////////////////////////////////////////////////////////////////////////
	// 清理运行数据
	UnInitlizationPlugin();

	Runend(true,_T("Un Initlization Plugin Complete."));
}
//////////////////////////////////////////////////////////////////////////
// Public:

bool CCommandObj::Initialization(HMODULE hModule)
{
	BOOL bRet = true;

	//////////////////////////////////////////////////////////////////////////
	// 初始化共享内存
	bRet = InitlizationShareMemory(DEFAULT_CONFIG_FILE_PATH);
	Runend(bRet,_T("Initialization Share Memory Complete."));

	//////////////////////////////////////////////////////////////////////////
	// 启动网络
	bRet = InitializationSocketClient();
	Runend(bRet,_T("Connect To The Server Complete."));

	//////////////////////////////////////////////////////////////////////////
	// 初始化插件管理模块
	bRet = InitlizationPluginManager(false);
	Runend(bRet,_T("Initlization Plugin Manager Complete."));

	//////////////////////////////////////////////////////////////////////////
	// 加载运行模块
	bRet = LoadPlugin();
	Runend(bRet,_T("Loading Operation Module Complete."));

	//////////////////////////////////////////////////////////////////////////
	// 调用运行模块
	bRet = InitlizationPlugin();
	Runend(bRet,_T("Call Operation Module Complete."));

	return bRet;
}

bool CCommandObj::LoadPlugin()
{
	if (NULL == m_PluginLoader)
		return false;

	bool bRet = m_PluginLoader->LoadPlugins(L"plugins");

	bRet = bRet && m_PluginLoader->InitializePlugins();

	m_Plugins.push_back(ClsidBootrun);
	m_Plugins.push_back(ClsidSecurity);
	m_Plugins.push_back(ClsidSystemInfo);
	m_Plugins.push_back(ClsidRemote);
	m_Plugins.push_back(ClsidRemove);
	m_Plugins.push_back(ClsidIELock);
	m_Plugins.push_back(ClsidAppreciation);
	m_Plugins.push_back(ClsidFastShutdown);

	return bRet;
}

bool CCommandObj::InitlizationPlugin()
{
	ULONG uReturn = ERROR_SUCCESS;

	for (int i = 0;i < m_Plugins.size();++i)
	{
		uReturn = ExecutePlugin(m_Plugins[i],m_PluginInitData[m_Plugins[i]]);
		if (ERROR_TIMEOUT == uReturn){
			OutA(WarningA,"Starting Plugin {%s} Complete.",m_Plugins[i]);
		}else{
			RunendA(uReturn,"Starting Plugin {%s} Complete.",m_Plugins[i]);
		}
	}

	return true;
}

unsigned CCommandObj::InternalExecutePlugin(PVOID pPluginInfo)
{
	ULONG uStartTime = 0;
	EXECUTE_PLUGIN_INFO PluginInfo;
	Cx_Interface<IPlugin> pPluginAddress(PEXECUTE_PLUGIN_INFO(pPluginInfo)->PluginClassName);

	uStartTime = PEXECUTE_PLUGIN_INFO(pPluginInfo)->StartTime;
	memcpy(&PluginInfo,pPluginInfo,sizeof(EXECUTE_PLUGIN_INFO));

	if (pPluginAddress) 
		PluginInfo.Return = pPluginAddress->Initialization((PEXECUTE_PLUGIN_INFO(pPluginInfo)->DataAddress));
	else
		PluginInfo.Return = false;

	if (uStartTime > GetTickCount())
	{
		PEXECUTE_PLUGIN_INFO(pPluginInfo)->Return = PluginInfo.Return;
	}

	return 0;
}

ULONG CCommandObj::ExecutePlugin(LPCSTR pszPluginClass,PVOID pDataAddress)
{
	unsigned uThreadId = 0;
	EXECUTE_PLUGIN_INFO pPluginInfo;
	pPluginInfo.Return = false;
	pPluginInfo.StartTime = GetTickCount() + 3000;
	pPluginInfo.PluginClassName = pszPluginClass;
	pPluginInfo.DataAddress = pDataAddress;

	HANDLE hThread = (HANDLE)_beginthreadex(NULL,0,InternalExecutePlugin,&pPluginInfo,0,&uThreadId);

	if (NULL == hThread)
	{
		return false;
	}

	if (WAIT_TIMEOUT == WaitForSingleObject(hThread,3000))
	{
		pPluginInfo.Return = ERROR_TIMEOUT;
	}

	CloseHandle(hThread);

	return pPluginInfo.Return;
}

void CCommandObj::UnInitlizationPlugin()
{

	m_Plugin.Unload(true);

	for (int i =0 ; i < m_Plugins.size();++i)
	{
		PPLUGIN_INIT_DATA pPluginInitData = m_PluginInitData[m_Plugins[i]];
		if (NULL != pPluginInitData)
		{
			if (NULL != pPluginInitData->Data)
				delete pPluginInitData->Data;

			delete pPluginInitData;
		}
	}
	m_Plugins.clear();
	m_PluginInitData.clear();
#ifdef _WIN32
	//CoUninitialize();       // Free COM resource
#endif
}

bool CCommandObj::InitializationSocketClient()
{
	bool bRet = false;

	if (NULL == m_pUserPool)
		return false;

	for (int i = 0;i < m_pUserPool->Config.dwRetryCount;++i)
	{
		if (m_cResourceClientObj.ConnectServer(m_pUserPool->Config.szServerAddress,m_pUserPool->Config.usResourceServerPort) 
			&& m_cCommandClientObj.ConnectServer(m_pUserPool->Config.szServerAddress,m_pUserPool->Config.usCommandServerPort)) 
			break;
		Sleep(m_pUserPool->Config.dwRetrySleep);
		Out(Dbg,_T("%dst Try To Connect The Server."),i+1);
	}
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// Public:

bool CCommandObj::InitlizationPluginManager(bool bLoadCore)
{
	if (NULL == m_Plugin.Handle())
	{
#ifdef _WIN32
		if(FAILED(CoInitialize(NULL)))
			return false;
#endif
		if (!m_Plugin.LoadPluginManager(L"plugins"))
			return false;
	}

	if (bLoadCore)
	{
		VERIFY(m_Plugin.LoadCorePlugins(L"plugins"));
	}

	m_PluginLoader = m_Plugin.GetPluginLoader();

	return (NULL == m_PluginLoader) ? false : true;
}

void CCommandObj::InternalInitializationClient(TiXmlHandle &cConfigObj)
{
	WCHAR szRunPath[MAX_PATH+1] = {0};

	GetModuleFileNameW(NULL,szRunPath,MAX_PATH);
	PathRemoveFileSpecW(szRunPath);
	PathAddBackslashW(szRunPath);

	if (szRunPath[0] == L'0'){
		GetSystemDirectoryW(szRunPath,MAX_PATH); 
		PathAddBackslashW(szRunPath);
	}

	m_pUserPool->Client.mProgramPID = GetCurrentProcessId();

	wcscpy_s(m_pUserPool->Client.mProgramPath,MAX_PATH,szRunPath);

	wcscpy_s(m_pUserPool->Client.mPluginsPath,MAX_PATH,szRunPath);
	wcscat_s(m_pUserPool->Client.mPluginsPath,MAX_PATH,_T("Plugins\\"));

	wcscpy_s(m_pUserPool->Client.mDataDirectory,MAX_PATH,szRunPath);
	wcscat_s(m_pUserPool->Client.mDataDirectory,MAX_PATH,_T("Data\\"));
}

void CCommandObj::InternalInitializationConfig(TiXmlHandle &cConfigObj)
{
	ULONG uValue = 0;
	TiXmlElement * Root;
	LPCSTR pszValue = NULL;

	Root = cConfigObj.FirstChildElement("Services").ToElement();

	if (NULL == Root)
	{
		m_pUserPool->Config.usResourceServerPort = DEFAULT_CONFIG_SERVER_RESOURCE_PORT;
		m_pUserPool->Config.usCommandServerPort = DEFAULT_CONFIG_SERVER_COMMAND_PORT;
		wcscpy_s(m_pUserPool->Config.szServerAddress,MAX_IP4_ADDRESS_LEN,DEFAULT_CONFIG_SERVER_ADDRESS);
	}
	else
	{
		pszValue = Root->Attribute("ResourceServerPort");

		if (NULL != pszValue)
			uValue = atoi(pszValue);

		m_pUserPool->Config.usResourceServerPort = (0 == uValue)? DEFAULT_CONFIG_SERVER_RESOURCE_PORT : uValue;

		pszValue = Root->Attribute("CommandServerPort");

		if (NULL != pszValue)
			uValue = atoi(pszValue);

		m_pUserPool->Config.usCommandServerPort = (0 == uValue)? DEFAULT_CONFIG_SERVER_COMMAND_PORT : uValue;

		wcscpy_s(m_pUserPool->Config.szServerAddress,MAX_IP4_ADDRESS_LEN,s2ws(Root->Attribute("ServerAddress")).c_str());

		if ('\0' == m_pUserPool->Config.szServerAddress[0])
			wcscpy_s(m_pUserPool->Config.szServerAddress,MAX_IP4_ADDRESS_LEN,DEFAULT_CONFIG_SERVER_ADDRESS);
	}



	Root = cConfigObj.FirstChildElement("Retry").ToElement();

	if (NULL == Root)
	{
		m_pUserPool->Config.dwRetrySleep = DEFAULT_CONFIG_RETRY_SLEEP;
		m_pUserPool->Config.dwRetryCount = DEFAULT_CONFIG_RETRY_COUNT;
	}
	else
	{
		pszValue = Root->Attribute("RetrySleep");

		if (NULL != pszValue)
			uValue = atoi(pszValue);

		m_pUserPool->Config.dwRetrySleep = (0 == uValue)? DEFAULT_CONFIG_RETRY_SLEEP : uValue;

		pszValue = Root->Attribute("RetryCount");

		if (NULL != pszValue)
			uValue = atoi(pszValue);

		m_pUserPool->Config.dwRetryCount = (0 == uValue)? DEFAULT_CONFIG_RETRY_COUNT : uValue;
	}

}

void CCommandObj::InternalInitializationBusiness(TiXmlHandle &cConfigObj)
{
	TiXmlElement * Root;

	Root = cConfigObj.FirstChildElement("Internet_Explorer").ToElement();

	if (NULL == Root)
	{
		wcscpy_s(m_pUserPool->Business.InternetExplorer_Home,MAX_URL_LEN,DEFAULT_BUSINESS_INTERNET_EXPLORER_HOME);
	}
	else
	{
		wcscpy_s(m_pUserPool->Business.InternetExplorer_Home,MAX_URL_LEN,s2ws(Root->Attribute("Home")).c_str());

		if ('\0' == m_pUserPool->Business.InternetExplorer_Home[0])
			wcscpy_s(m_pUserPool->Business.InternetExplorer_Home,MAX_URL_LEN,DEFAULT_BUSINESS_INTERNET_EXPLORER_HOME);
	}


}


std::wstring s2ws_lwr(const std::string& s)
{
	setlocale(LC_ALL, "chs"); 
	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest,_Source,_Dsize);
	_Dest = wcslwr(_Dest);
	std::wstring result = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, "C");
	return result;
}


/*
** SYSTEMTIME转time_t
*/
time_t systime_to_timet(const SYSTEMTIME& st)
{
	struct tm gm = {st.wSecond, st.wMinute, st.wHour, st.wDay, st.wMonth-1, st.wYear-1900, st.wDayOfWeek, 0, 0};
	return mktime(&gm);
}

void CCommandObj::InternalInitializationSecurity(TiXmlHandle & cConfigObj)
{
	LPCSTR pszValue = NULL;
	TiXmlElement * Root;
	ULONG uElementQuantity = 0,uValue = 0;
	PPLUGIN_INIT_DATA pPluginInitData = new PLUGIN_INIT_DATA;
	CSecurity::PDATA_SECURITY pSecurityData = new CSecurity::DATA_SECURITY;

	memset(pPluginInitData,0,sizeof(PLUGIN_INIT_DATA));
	memset(pSecurityData,0,sizeof(CSecurity::DATA_SECURITY));
	pPluginInitData->Lock = new Mutex;
	pPluginInitData->Lock->Lock();
	pPluginInitData->Data = pSecurityData;

	if (NULL == cConfigObj.ToElement())
	{
		return ;
	}
	TiXmlHandle cSwitch(cConfigObj.ToElement());
	if (cSwitch.ToElement()->Attribute("ARP") != NULL && 0 == stricmp(cSwitch.ToElement()->Attribute("ARP"),"Enable"))
	{

		cConfigObj = cSwitch.FirstChildElement("ARP");
		if (cConfigObj.ToElement() != NULL)
		{
			CSecurity::ENUM_ARP_TYPE DefType	=	CSecurity::GetArpType(cConfigObj.ToElement()->Attribute("Type"));

			//////////////////////////////////////////////////////////////////////////
			// ARP

			uElementQuantity = 0;

			for(Root=cConfigObj.FirstChildElement("ARP").ToElement();Root;Root=Root->NextSiblingElement("ARP"))
			{
				++uElementQuantity;
			}

			pSecurityData->ARP.ArraySize = uElementQuantity;
			pSecurityData->ARP.ARP = new CSecurity::DATA_ARP[pSecurityData->ARP.ArraySize];

			uElementQuantity = 0;

			for (Root=cConfigObj.FirstChildElement("ARP").ToElement();Root;Root=Root->NextSiblingElement("ARP"))
			{
				wcscpy_s(pSecurityData->ARP.ARP[uElementQuantity].BindIP,MAX_IP4_ADDRESS_LEN,s2ws(Root->Attribute("IP")).c_str());
				wcscpy_s(pSecurityData->ARP.ARP[uElementQuantity].BindMAC,MAX_MAC_ADDRESS_LEN,s2ws(Root->Attribute("MAC")).c_str());

				pszValue = Root->Attribute("Sleep");

				if (NULL != pszValue)
					uValue = atoi(pszValue);

				pSecurityData->ARP.ARP[uElementQuantity].Sleep		= (0 == uValue)? 10 : uValue;

				pSecurityData->ARP.ARP[uElementQuantity].Type = CSecurity::GetArpType(Root->Attribute("Type"),DefType);

				++uElementQuantity;
			}

			pSecurityData->Switch = pSecurityData->Switch | PLUGIN_SECURITY_ARP;
		}

	}
	if (cSwitch.ToElement()->Attribute("BHO") != NULL && 0 == stricmp(cSwitch.ToElement()->Attribute("BHO"),"Enable"))
	{
		cConfigObj = cSwitch.FirstChildElement("BHO");
		if (cConfigObj.ToElement() != NULL)
		{
			pSecurityData->BHO.Type				=	CSecurity::GetBhoListType(cConfigObj.ToElement()->Attribute("Type"));

			//////////////////////////////////////////////////////////////////////////
			// BHO

			uElementQuantity = 0;

			for(Root = cConfigObj.FirstChildElement("BHO").ToElement();Root;Root = Root->NextSiblingElement("BHO"))
			{
				++uElementQuantity;
			}

			pSecurityData->BHO.ArraySize = uElementQuantity;
			pSecurityData->BHO.BHO = new CSecurity::DATA_BHO[pSecurityData->BHO.ArraySize];
			pSecurityData->BHO.ArrayOffset = sizeof(CSecurity::DATA_SECURITY_BHO);

			uElementQuantity = 0;

			for (Root = cConfigObj.FirstChildElement("BHO").ToElement();Root;Root = Root->NextSiblingElement("BHO"))
			{
				wcscpy_s(pSecurityData->BHO.BHO[uElementQuantity].CLSID,MAX_CLSID_LEN,s2ws_lwr(Root->Attribute("CLSID")).c_str());
				pSecurityData->BHO.BHO[uElementQuantity].CLSID[MAX_CLSID_LEN] = L'\0';

				++uElementQuantity;
			}

			pSecurityData->Switch = pSecurityData->Switch | PLUGIN_SECURITY_BHO;
		}
	}
	if (cSwitch.ToElement()->Attribute("TIME") != NULL && 0 == stricmp(cSwitch.ToElement()->Attribute("TIME"),"Enable"))
	{
		cConfigObj = cSwitch.FirstChildElement("TIME");
		if (cConfigObj.ToElement() != NULL)
		{

			//////////////////////////////////////////////////////////////////////////
			// TIME

			SYSTEMTIME SystemTime;
			GetSystemTime(&SystemTime);
			pSecurityData->TIME.Time = systime_to_timet(SystemTime);

			pSecurityData->Switch = pSecurityData->Switch | PLUGIN_SECURITY_TIME;
		}
	}

	m_PluginInitData[ClsidSecurity] = pPluginInitData;
}

bool CCommandObj::InitlizationShareMemory(LPCWSTR lpConfigFileFullPath)
{	
	bool bRet = false;
	CHAR szConfigFilePath[MAX_PATH+1] = {0};

	TiXmlDocument cXmlDocument;
	TiXmlHandle cXmlControl(&cXmlDocument);
	bRet = m_UserPool.Init(KEY_SHAREMENORY);
	Out(Outputs,_T("Share Memory Used Object Count Is %d,Share Memory Free Object Count Is %d."),m_UserPool.GetUsedObjectCount(),m_UserPool.GetFreeObjectCount());
	if (0 != m_UserPool.GetUsedObjectCount())
	{
		m_pUserPool = m_UserPool.GetUsedObject(0);
	}else
	{
		m_pUserPool = m_UserPool.NewObject();
	}

	if(NULL == m_pUserPool){
		Out(Errors,_T("Initlization Share Memory Failed. SHARE:0x%p."),m_pUserPool);
		bRet = false;
	}else{
		if(0 == GetModuleFileNameA(NULL,szConfigFilePath,MAX_PATH))
		{
			strcpy_s(szConfigFilePath,ws2s(lpConfigFileFullPath).c_str());
		}else
		{
			PathRemoveFileSpecA(szConfigFilePath);
			PathAddBackslashA(szConfigFilePath);
			strcat_s(szConfigFilePath,ws2s(lpConfigFileFullPath).c_str());
		}
		cXmlDocument.LoadFile(szConfigFilePath);

		InternalInitializationClient(cXmlControl.FirstChildElement("Client"));

		InternalInitializationConfig(cXmlControl.FirstChildElement("Config"));

		InternalInitializationBusiness(cXmlControl.FirstChildElement("Business"));

		InternalInitializationSecurity(cXmlControl.FirstChildElement("Security"));

		bRet = true;
	}

	return bRet;
}

DWORD CCommandObj::GetProcessIDForProcessName(LPCTSTR lpProcessName)
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

bool CCommandObj::InternalCopyPath(LPTSTR Destination,rsize_t Size,LPCTSTR SourceAddress,LPTSTR String)
{
	if (NULL == &Destination)			{
		return false;
	}else if (NULL == Size)				{
		return false;
	}else if (NULL == SourceAddress)	{
		return false;
	}else if (NULL == String)			{
		return false;
	}

	_tcscpy_s(Destination,Size,SourceAddress);
	_tcscat_s(Destination,Size,String);

	return TRUE;
}

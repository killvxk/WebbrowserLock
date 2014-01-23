
#include <UtilFunc/PluginInc.h>
#include <tchar.h>
#include <Psapi.h>
#include <iptypes.h>
#include <iphlpapi.h>
#include <Tlhelp32.h>

//////////////////////////////////////////////////////////////////////////
// 引用外部头文件

#include "SystemInfoTool.h"

//////////////////////////////////////////////////////////////////////////
// 引用库文件

#pragma comment(lib,"Psapi")
#pragma comment(lib,"iphlpapi")
#pragma comment(lib, DIRECTORY_LIB "Function\\Function.Internal.CommonControl.lib")

//////////////////////////////////////////////////////////////////////////
// Initializaton


CSystemInfo::CSystemInfo() : CCommonControl(_T("SystemInfo"))
{
	m_lpInfoBuffer = (LPTSTR)malloc(MAX_BUFFER_INFO_SIZE * sizeof(TCHAR));
	memset(m_lpInfoBuffer,0,MAX_BUFFER_INFO_SIZE * sizeof(TCHAR));
}

CSystemInfo::~CSystemInfo()
{
	if (NULL != m_lpInfoBuffer)
	{
		free(m_lpInfoBuffer);
	}
}


bool CSystemInfo::Initialization(PVOID pData)
{
	size_t iUseLength = 0;
	_tcscat_s(m_lpInfoBuffer,MAX_BUFFER_INFO_SIZE,_T("OS:"));
	iUseLength += _tcslen(m_lpInfoBuffer + iUseLength);

	CheckOperatingSystemVersion(m_lpInfoBuffer + iUseLength,MAX_BUFFER_INFO_SIZE - iUseLength);
	iUseLength += _tcslen(m_lpInfoBuffer + iUseLength);

	_tcscat_s(m_lpInfoBuffer + iUseLength,MAX_BUFFER_INFO_SIZE - iUseLength,_T("|IV:"));
	iUseLength += _tcslen(m_lpInfoBuffer + iUseLength);
	CheckIEVersion(m_lpInfoBuffer + iUseLength,MAX_BUFFER_INFO_SIZE - iUseLength);
	iUseLength += _tcslen(m_lpInfoBuffer + iUseLength);

	_tcscat_s(m_lpInfoBuffer + iUseLength,MAX_BUFFER_INFO_SIZE - iUseLength,_T("|SI:"));
	iUseLength += _tcslen(m_lpInfoBuffer + iUseLength);
	CheckSystemInfo(m_lpInfoBuffer + iUseLength,MAX_BUFFER_INFO_SIZE - iUseLength);
	iUseLength += _tcslen(m_lpInfoBuffer + iUseLength);

	_tcscat_s(m_lpInfoBuffer + iUseLength,MAX_BUFFER_INFO_SIZE - iUseLength,_T("|CI:"));
	iUseLength += _tcslen(m_lpInfoBuffer + iUseLength);
	CheckCPUInfo(m_lpInfoBuffer + iUseLength,MAX_BUFFER_INFO_SIZE - iUseLength);
	iUseLength += _tcslen(m_lpInfoBuffer + iUseLength);

	_tcscat_s(m_lpInfoBuffer + iUseLength,MAX_BUFFER_INFO_SIZE - iUseLength,_T("|MI:"));
	iUseLength += _tcslen(m_lpInfoBuffer + iUseLength);
	CheckMemoryInfo(m_lpInfoBuffer + iUseLength,MAX_BUFFER_INFO_SIZE - iUseLength);
	iUseLength += _tcslen(m_lpInfoBuffer + iUseLength);

	_tcscat_s(m_lpInfoBuffer + iUseLength,MAX_BUFFER_INFO_SIZE - iUseLength,_T("|DCI:"));
	iUseLength += _tcslen(m_lpInfoBuffer + iUseLength);
	CheckDisplayCardInfo(m_lpInfoBuffer + iUseLength,MAX_BUFFER_INFO_SIZE - iUseLength);
	iUseLength += _tcslen(m_lpInfoBuffer + iUseLength);

	/*
	_tcscat_s(m_lpInfoBuffer + iUseLength,MAX_BUFFER_INFO_SIZE - iUseLength,_T("|DI:"));
	iUseLength += _tcslen(m_lpInfoBuffer + iUseLength);
	CheckDriveInfo(m_lpInfoBuffer + iUseLength,MAX_BUFFER_INFO_SIZE - iUseLength);
	iUseLength += _tcslen(m_lpInfoBuffer + iUseLength);

	_tcscat_s(m_lpInfoBuffer + iUseLength,MAX_BUFFER_INFO_SIZE - iUseLength,_T("|NI:"));
	iUseLength += _tcslen(m_lpInfoBuffer + iUseLength);
	CheckNetworkInfo(m_lpInfoBuffer + iUseLength,MAX_BUFFER_INFO_SIZE - iUseLength);
	iUseLength += _tcslen(m_lpInfoBuffer + iUseLength);

	_tcscat_s(m_lpInfoBuffer + iUseLength,MAX_BUFFER_INFO_SIZE - iUseLength,_T("|EI:"));
	iUseLength += _tcslen(m_lpInfoBuffer + iUseLength);
	CheckEnvironment(m_lpInfoBuffer + iUseLength,MAX_BUFFER_INFO_SIZE - iUseLength);
	iUseLength += _tcslen(m_lpInfoBuffer + iUseLength);
	*/

	Out(Dbg,_T("\r\nReturn:			\t%s\r\n\r\n"),m_lpInfoBuffer);

	return true;
}

//////////////////////////////////////////////////////////////////////////
// Public:

void CSystemInfo::CheckOperatingSystemVersion(__out LPTSTR lpSystemVersionBuffer,size_t iBufferLen) const 
{
	if (iBufferLen <= 0)	return;

	Out(Dbg,_T("Checking Operating System...Please wait for a moment...\r\n\r\n"));

	//
	//Check Operating System version
	//
	Out(Dbg,_T("Checking Operating System version...\r\n"));

	OSVERSIONINFOEX OsVerInfoEx;
	OsVerInfoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if( !GetVersionEx((OSVERSIONINFO *)&OsVerInfoEx) )
	{
		return ;
	}

	//WCHAR szOsVerInfo[256] = {NULL};
	WCHAR szOsName[32] = {NULL};

	if( OsVerInfoEx.dwPlatformId == VER_PLATFORM_WIN32_NT )
	{
		lstrcpy(szOsName,_T("Unknown"));

		if( OsVerInfoEx.dwMajorVersion<=4 )
		{
			lstrcpy(szOsName,_T("NT"));
		}
		if( OsVerInfoEx.dwMajorVersion==5 && OsVerInfoEx.dwMinorVersion==0 )
		{
			lstrcpy(szOsName,_T("2000"));
		}
		if( OsVerInfoEx.dwMajorVersion==5 && OsVerInfoEx.dwMinorVersion==1 )
		{
			lstrcpy(szOsName,_T("XP"));
		}
		if( OsVerInfoEx.dwMajorVersion==5 && OsVerInfoEx.dwMinorVersion==2 )
		{
			lstrcpy(szOsName,_T("2003"));
		}
		if( OsVerInfoEx.dwMajorVersion==6 && OsVerInfoEx.dwMinorVersion==0 )
		{
			lstrcpy(szOsName,_T("Vista"));
		}
		if( OsVerInfoEx.dwMajorVersion==6 && OsVerInfoEx.dwMinorVersion==1 )
		{
			lstrcpy(szOsName,_T("7"));
		}
	}
	else
	{
		Out(Dbg,_T("Do not support this Operating System!\r\n\r\n"));

		return ;
	}

	_stprintf_s(lpSystemVersionBuffer,iBufferLen,_T("%d.%d.%d(%d)"),
		OsVerInfoEx.dwMajorVersion,
		OsVerInfoEx.dwMinorVersion,
		OsVerInfoEx.dwBuildNumber,
		OsVerInfoEx.wServicePackMajor
		);

	Out(Dbg,_T("OS Name:   \tMicrosoft Windows %s\r\nOS Version:\t%d.%d.%d Service Pack %d\r\n\r\n"),
		szOsName,
		OsVerInfoEx.dwMajorVersion,
		OsVerInfoEx.dwMinorVersion,
		OsVerInfoEx.dwBuildNumber,
		OsVerInfoEx.wServicePackMajor
		);
	return ;
}

void CSystemInfo::CheckCPUInfo(__out LPTSTR lpCPUInfoBuffer,size_t iBufferLen) const
{
	if (iBufferLen <= 0)	return;

	//
	//Check CPU info
	//
	Out(Dbg,_T("Checking CPU info...\r\n"));

	SYSTEM_INFO systeminfo;
	GetSystemInfo(&systeminfo);


	Out(Dbg,_T("CPU:       \t%d CPU installed.\r\n"),systeminfo.dwNumberOfProcessors);

	HKEY hCPUKey = NULL;

	for(DWORD i=0;i<systeminfo.dwNumberOfProcessors;i++)
	{

		DWORD dwCPUMhz = 0;
		DWORD dwBytes = sizeof(DWORD);
		DWORD dwType = REG_DWORD;
		WCHAR szIdentifier[256] = {NULL};
		WCHAR szVendorIdentifier[256] = {NULL};

		WCHAR szCPUSubKey[256] = {NULL};
		swprintf(szCPUSubKey,_T("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\%d"),i);

		RegOpenKey(HKEY_LOCAL_MACHINE, szCPUSubKey, &hCPUKey);

		RegQueryValueEx(hCPUKey, _T("~MHz"), NULL, &dwType, (LPBYTE)&dwCPUMhz, &dwBytes);

		dwType = REG_SZ;
		dwBytes = 256;

		RegQueryValueEx(hCPUKey, _T("Identifier"), NULL, &dwType, (LPBYTE)&szIdentifier, &dwBytes);
		RegQueryValueEx(hCPUKey, _T("VendorIdentifier"), NULL, &dwType, (LPBYTE)&szVendorIdentifier, &dwBytes);

		Out(Dbg,
			_T("           \t[%d]: %s %s ~%d Mhz\r\n"),
			i,
			szIdentifier,
			szVendorIdentifier,
			dwCPUMhz);
	}

	RegCloseKey(hCPUKey);

	_stprintf_s(lpCPUInfoBuffer,iBufferLen,_T("%d"),
		systeminfo.dwNumberOfProcessors
		);

	return ;
}

void CSystemInfo::CheckMemoryInfo(__out LPTSTR lpMemoryInfoBuffer,size_t iBufferLen) const 
{
	if (iBufferLen <= 0)	return;

	//
	//Check Memory info
	//
	Out(Dbg,_T("\r\nChecking Memory info...\r\n"));


	MEMORYSTATUSEX MemStatEx;
	MemStatEx.dwLength = sizeof(MEMORYSTATUSEX);
	/*

	typedef BOOL (CALLBACK* GLOBALMEMORYSTATUSEX)(MEMORYSTATUSEX* pMemStatus);
	GLOBALMEMORYSTATUSEX pGlobalMemoryStatusEx;

	pGlobalMemoryStatusEx = (GLOBALMEMORYSTATUSEX)GetProcAddress(GetModuleHandle(_T("Kernel32.dll")), ("GlobalMemoryStatusEx"));
	if (pGlobalMemoryStatusEx)
	{
	MemStatEx.dwLength = sizeof(MEMORYSTATUSEX);
	pGlobalMemoryStatusEx(&MemStatEx);
	}
	*/

	GlobalMemoryStatusEx(&MemStatEx);

	Out(Dbg,_T("PhysMem:   \t%d MB\r\n"),MemStatEx.ullTotalPhys/1024/1024);

	Out(Dbg,_T("PhysFree:  \t%d MB\r\n"),MemStatEx.ullAvailPhys/1024/1024);

	Out(Dbg,_T("VirtualMem:\t%d MB\r\n"),MemStatEx.ullTotalVirtual/1024/1024);

	_stprintf_s(lpMemoryInfoBuffer,iBufferLen,_T("%d MB"),
		MemStatEx.ullTotalPhys/1024/1024
		);

	return ;
}

void CSystemInfo::CheckDriveInfo(__out LPTSTR lpDriveInfoBuffer,size_t iBufferLen) const 
{
	if (iBufferLen <= 0)	return;


	//
	//Check Drive info
	//
	Out(Dbg,_T("\r\nChecking Drive info...\r\n"));

	SetErrorMode(SEM_FAILCRITICALERRORS);

	for(int i=0;i<26;++i)
	{
		UINT uType = DRIVE_UNKNOWN;
		WCHAR szDisk[3] = {NULL};
		WCHAR szDriveType[32] = {NULL};
		ULARGE_INTEGER FreeBytesAvailable,TotalNumberOfBytes,TotalNumberOfFreeBytes;
		unsigned long TotalSpace = 0,FreeSpace = 0;

		swprintf(szDisk,_T("%c:"),'A'+i);
		uType=GetDriveType(szDisk);

		switch(uType)
		{
		case DRIVE_UNKNOWN:
			{
				lstrcpy(szDriveType,_T("UNKNOWN"));
			}break;
		case DRIVE_NO_ROOT_DIR:
			{
				lstrcpy(szDriveType,_T("NO_ROOT_DIR"));
			}break;
		case DRIVE_REMOVABLE:
			{
				lstrcpy(szDriveType,_T("REMOVABLE"));
			}break;
		case DRIVE_REMOTE:
			{
				lstrcpy(szDriveType,_T("REMOTE"));
			}break;
		case DRIVE_CDROM:
			{
				lstrcpy(szDriveType,_T("CDROM"));
			}break;
		case DRIVE_RAMDISK:
			{
				lstrcpy(szDriveType,_T("RAMDISK"));
			}break;
		case DRIVE_FIXED:
			{
				lstrcpy(szDriveType,_T("FIXED"));

				GetDiskFreeSpaceEx(szDisk,&FreeBytesAvailable,&TotalNumberOfBytes,&TotalNumberOfFreeBytes);
				TotalSpace = TotalNumberOfBytes.QuadPart/(1024*1024);
				FreeSpace = TotalNumberOfFreeBytes.QuadPart/(1024*1024);
			}break;
		default:
			{
				lstrcpy(szDriveType,_T("UNKNOWN"));
			}break;
		}

		if( uType!=DRIVE_NO_ROOT_DIR )
		{
			if( uType!= DRIVE_FIXED )
			{
				Out(Dbg,_T("[%s]        \t[%s]\r\n"),szDisk,szDriveType);
			}
			else
			{
				Out(Dbg,
					_T("[%s]        \t[%s] TotalSpace:%6ldMB(%4.1fGB) FreeSpace:%6ldMB(%4.1fGB)\r\n"),
					szDisk,
					szDriveType,
					TotalSpace,
					float(TotalSpace)/1024,
					FreeSpace,
					float(FreeSpace)/1024
					);
			}
		}
	}


	return ;
}

void CSystemInfo::CheckNetworkInfo(__out LPTSTR lpNetworkInfoBuffer,size_t iBufferLen) const 
{
	if (iBufferLen <= 0)	return;


	//
	//Check Network info
	//
	Out(Dbg,_T("\r\nChecking Network info...\r\n"));

	IP_ADAPTER_INFO *IpAdaptersInfo =NULL;
	IP_ADAPTER_INFO *IpAdaptersInfoHead =NULL;

	IpAdaptersInfo = (IP_ADAPTER_INFO*) GlobalAlloc(GPTR,sizeof(IP_ADAPTER_INFO));
	if( IpAdaptersInfo == NULL )
	{
		return ;
	}

	DWORD dwDataSize = sizeof(IP_ADAPTER_INFO);
	DWORD dwRetVal = GetAdaptersInfo(IpAdaptersInfo,&dwDataSize);

	if( ERROR_SUCCESS != dwRetVal )
	{   
		GlobalFree(IpAdaptersInfo);
		IpAdaptersInfo = NULL;

		if( ERROR_BUFFER_OVERFLOW == dwRetVal )
		{
			IpAdaptersInfo = (IP_ADAPTER_INFO*) GlobalAlloc(GPTR,dwDataSize);
			if( IpAdaptersInfo == NULL )
			{
				return ;
			}
			if( ERROR_SUCCESS != GetAdaptersInfo(IpAdaptersInfo,&dwDataSize) )
			{
				GlobalFree(IpAdaptersInfo);
				return ;
			}
		}
		else
		{
			return ;
		}
	}

	IpAdaptersInfoHead = IpAdaptersInfo;

	//Count the number of NIC
	int iNumberOfNIC = 0;
	do
	{
		if( IpAdaptersInfo->Type == MIB_IF_TYPE_ETHERNET )
		{
			iNumberOfNIC++;

			OutA(OutputsA,
				"[NIC]      \t[%d]: %s\r\n           \tIP Address:[%s]\r\n           \tSubnet Mask:[%s]\r\n           \tDefault Gateway:[%s]\r\n",
				iNumberOfNIC,
				IpAdaptersInfo->Description,
				IpAdaptersInfo->IpAddressList.IpAddress.String,
				IpAdaptersInfo->IpAddressList.IpMask.String,
				IpAdaptersInfo->GatewayList.IpAddress.String
				);

			//char * pszMACBuffer =  new char[IpAdaptersInfo->AddressLength * 3+3];
			char pszMACBuffer[30];
			char * pszMACAddress = pszMACBuffer;
			for(int i = 0; i < IpAdaptersInfo->AddressLength; ++i)
			{
				if( i == (IpAdaptersInfo->AddressLength - 1) )
				{
					sprintf_s(pszMACAddress,4, "%02X", (int) IpAdaptersInfo->Address[i]);
					pszMACAddress += 2;
				}
				else
				{
					sprintf_s(pszMACAddress,4, "%02X-", (int) IpAdaptersInfo->Address[i]);
					pszMACAddress += 3;
				}
			}
			OutA(OutputsA,"           \tPhysical Address:[%s]\r\n",pszMACBuffer);

			//_tcscat_s(lpNetworkInfoBuffer,iBufferLen,pszMACBuffer);

			//delete[] pszMACBuffer;
		}
		IpAdaptersInfo = IpAdaptersInfo->Next;
	}
	while(IpAdaptersInfo);

	if( IpAdaptersInfoHead != NULL )   
	{
		GlobalFree(IpAdaptersInfoHead);
	}

	return ;
}

void CSystemInfo::CheckSystemInfo(__out LPTSTR lpSystemInfoBuffer,size_t iBufferLen) const
{
	if (iBufferLen <= 0)	return;

	//Return Value:
	//-1 = error
	//0 = warning
	//1 = succeed

	//
	//Check System info
	//
	Out(Dbg,_T("\r\nChecking System info...\r\n"));

	HKEY hCPUKey = NULL;
	DWORD dwCPUMhz = 0;
	DWORD dwBytes = sizeof(DWORD);
	DWORD dwType = REG_SZ;
	WCHAR szWindowsDirectory[MAX_PATH] = {NULL};
	WCHAR szSystemDirectory[MAX_PATH] = {NULL};

	GetWindowsDirectory(szWindowsDirectory,MAX_PATH);
	GetSystemDirectory(szSystemDirectory,MAX_PATH);


	Out(Dbg,_T("WinDir:				\t%s\r\n"),szWindowsDirectory);

	Out(Dbg,_T("SysDir:				\t%s\r\n"),szSystemDirectory);


	dwBytes = MAX_PATH;
	szWindowsDirectory[0] = '\0';
	szSystemDirectory[0] = '\0';

	RegOpenKey(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), &hCPUKey);

	RegQueryValueEx(hCPUKey, _T("RegisteredOrganization"), NULL, &dwType, (LPBYTE)&szWindowsDirectory, &dwBytes);
	RegQueryValueEx(hCPUKey, _T("RegisteredOwner"), NULL, &dwType, (LPBYTE)&szSystemDirectory, &dwBytes);

	RegCloseKey(hCPUKey);


	Out(Dbg,_T("RegisteredOwner:		\t%s\r\n"),szSystemDirectory);

	Out(Dbg,_T("RegisteredOrganization:		\t%s\r\n\r\n"),szWindowsDirectory);

	_tcscpy_s(lpSystemInfoBuffer,iBufferLen,szWindowsDirectory);

	return ;
}

void CSystemInfo::CheckEnvironment(__out LPTSTR lpEnvironmentBuffer,size_t iBufferLen) const
{
	if (iBufferLen <= 0)	return;

	//Return Value:
	//-1 = error
	//0 = warning
	//1 = succeed

	int iRet = 0;

	Out(Dbg,_T("Checking environment...Please wait for a moment...\r\n\r\n"));

	//
	//Check process
	//
	Out(Dbg,_T("Checking process...\r\n\r\n"));

	int iOpenProcError = 0;
	HANDLE hSnapshot = NULL;
	PROCESSENTRY32 pe32;
	hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( hSnapshot == NULL )
	{
		return ;
	}
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	Process32First( hSnapshot, &pe32 );
	do
	{
		//Ignore [current process]\[NTOSKRNL]
		if( pe32.th32ProcessID == GetCurrentProcessId() ||
			pe32.th32ProcessID == 0
			)
		{
			continue;
		}
		iOpenProcError++;

		WCHAR szProcessPath[MAX_PATH]={0};
		lstrcpy(szProcessPath,_T("Unknown"));
		GetProcessPath( pe32.th32ProcessID,szProcessPath,sizeof(szProcessPath),TRUE );

		Out(Dbg,
			_T("[Process]  \tPID = [%d]	ImagePath = [%s]\r\n"),
			pe32.th32ProcessID,
			szProcessPath
			);
	}
	while( Process32Next( hSnapshot, &pe32 ) );
	CloseHandle( hSnapshot );

	if( iOpenProcError > 0 )
	{
		Out(Dbg,_T("\r\nRainbow-SandBox may not work well with this(these) process!\r\n\r\n"));

		iRet = -1;
	}

	//
	//Check module
	//
	Out(Dbg,_T("Checking module...\r\n\r\n"));

	int iModuleError = 0;
	HANDLE hModuleSnapshot= NULL;
	MODULEENTRY32 moudle;

	hModuleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,GetCurrentProcessId());
	moudle.dwSize = sizeof(MODULEENTRY32);
	Module32First(hModuleSnapshot,&moudle);
	do
	{
		//Ignore current module
		if( moudle.hModule == GetModuleHandle(NULL) )
		{
			continue;
		}

		//Verify signature
		//		if( CheckFileTrust(moudle.szExePath) != 1 )
		{
			iModuleError++;

			Out(Dbg,
				_T("[Module]   \tName = [%s]\r\n           \tImagePath = [%s]\r\n"),
				moudle.szModule,
				moudle.szExePath
				);
		}
	}
	while( Module32Next(hModuleSnapshot,&moudle) );

	CloseHandle(hModuleSnapshot);

	if( iModuleError > 0 )
	{
		Out(Dbg,_T("\r\nRainbow-SandBox may not work well with this(these) module!\r\n\r\n"));

		iRet = -1;
	}

	return ;
}

void CSystemInfo::CheckIEVersion(__out LPTSTR lpIEVersionBuffer,size_t iBufferLen) const
{
	if (iBufferLen <= 0)	return;

	HKEY hCPUKey = NULL;
	DWORD dwType = REG_SZ;
	DWORD dwBytes = MAX_PATH;
	TCHAR szIEVersion[MAX_PATH] = {0};

	Out(Dbg,_T("\r\nChecking IE Version...\r\n"));

	RegOpenKey(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Internet Explorer"), &hCPUKey);

	RegQueryValueEx(hCPUKey, _T("Version"), NULL,&dwType, (LPBYTE)&szIEVersion, &dwBytes);

	RegCloseKey(hCPUKey);

	Out(Dbg,_T("IE Version:			\t%s\r\n"),szIEVersion);

	_tcscpy_s(lpIEVersionBuffer,iBufferLen,szIEVersion);

	return ;
}

void CSystemInfo::CheckDisplayCardInfo(__out LPTSTR lpDisplayCardBuffer,size_t iBufferLen) const
{  
	HKEY keyServ;  
	HKEY keyEnum;  
	HKEY key;  
	HKEY key2;  
	LONG lResult;//LONG型变量－保存函数返回值  
	int i = 0,iCount = 0;  


	Out(Dbg,_T("\r\nChecking Display Card Info...\r\n"));

	_tcscpy_s(lpDisplayCardBuffer,iBufferLen,_T("{"));

	//查询"SYSTEM\\CurrentControlSet\\Services"下的所有子键保存到keyServ  
	lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE,TEXT("SYSTEM\\CurrentControlSet\\Services"),0,KEY_READ,&keyServ);  
	if (ERROR_SUCCESS != lResult)  
		return;  


	//查询"SYSTEM\\CurrentControlSet\\Enum"下的所有子键保存到keyEnum  
	lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE,TEXT("SYSTEM\\CurrentControlSet\\Enum"),0,KEY_READ,&keyEnum);  
	if (ERROR_SUCCESS != lResult)  
		return;  

	DWORD type = 0;  
	for (;;++i)  
	{  
		TCHAR name[512] = {0};//保存keyServ下各子项的字段名称
		DWORD dwSize = 0;


		Sleep(5);  
		dwSize = 512;  

		//逐个枚举keyServ下的各子项字段保存到name中  
		lResult = RegEnumKeyEx(keyServ,i,name,&dwSize,NULL,NULL,NULL,NULL);  

		//要读取的子项不存在，即keyServ的子项全部遍历完时跳出循环  
		if(lResult == ERROR_NO_MORE_ITEMS)  
			break;  

		//打开keyServ的子项字段为name所标识的字段的值保存到key  
		lResult = RegOpenKeyEx(keyServ,name,0,KEY_READ,&key);  
		if (lResult != ERROR_SUCCESS)  
		{  
			RegCloseKey(keyServ);  
			return;  
		}  


		dwSize = 512;  
		//查询key下的字段为Group的子键字段名保存到name  
		lResult = RegQueryValueEx(key,TEXT("Group"),0,&type,(LPBYTE)name,&dwSize);  
		if(lResult == ERROR_FILE_NOT_FOUND)  
		{  
			//?键不存在  
			RegCloseKey(key);  
			continue;  
		} 



		//如果查询到的name不是Video则说明该键不是显卡驱动项  
		if(_tcscmp(TEXT("Video"),name)!=0)  
		{  
			RegCloseKey(key);  
			continue;     //返回for循环  
		}

		//如果程序继续往下执行的话说明已经查到了有关显卡的信息，所以在下面的代码执行完之后要break第一个for循环，函数返回  
		lResult = RegOpenKeyEx(key,TEXT("Enum"),0,KEY_READ,&key2);  
		RegCloseKey(key);  
		key = key2;  
		dwSize = sizeof(iCount);  
		lResult = RegQueryValueEx(key,TEXT("Count"),0,&type,(LPBYTE)&iCount,&dwSize);//查询Count字段（显卡数目）  

		//iBufferLen = count;//保存显卡数目  
		for(int j=0;j <iCount;++j)  
		{  
			TCHAR sz[512] = {0};  
			TCHAR name[64] = {0};  
			wsprintf(name,TEXT("%d"),j);  
			dwSize = sizeof(sz);  
			lResult  = RegQueryValueEx(key,name,0,&type,(LPBYTE)sz,&dwSize);  


			lResult = RegOpenKeyEx(keyEnum,sz,0,KEY_READ,&key2);  
			if (ERROR_SUCCESS)  
			{  
				RegCloseKey(keyEnum);  
				return;  
			}  


			dwSize = sizeof(sz);  
			lResult = RegQueryValueEx(key2,TEXT("FriendlyName"),0,&type,(LPBYTE)sz,&dwSize);  
			if(lResult == ERROR_FILE_NOT_FOUND)  
			{  
				dwSize = sizeof(sz);  
				lResult = RegQueryValueEx(key2,TEXT("DeviceDesc"),0,&type,(LPBYTE)sz,&dwSize); 
				_tcscat_s(lpDisplayCardBuffer,iBufferLen,sz);
				if ((iCount-1) < j)
				{
					_tcscat_s(lpDisplayCardBuffer,iBufferLen,_T(","));
				}
			}
			RegCloseKey(key2);  
			key2 = NULL;  
		}
		RegCloseKey(key);  
		key = NULL;  
		break;  
	} 

	if (iCount == 0)
	{
		memset(lpDisplayCardBuffer,0,iBufferLen);
	}else
	{
		_tcscat_s(lpDisplayCardBuffer,iBufferLen,_T("}"));
	}

	return ;
}  

//////////////////////////////////////////////////////////////////////////	
// Private:


/* ProcessTools Functions */

BOOL CSystemInfo::GetProcessPath(IN DWORD dwProcessID,IN LPWSTR lpszBuffer,IN int nSize,IN BOOL bFullPath) const
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

DWORD CSystemInfo::GetProcessIDForProcessName(LPCTSTR lpProcessName)
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

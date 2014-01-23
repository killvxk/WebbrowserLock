#include "CommandTool.h"

//////////////////////////////////////////////////////////////////////////
// 引用自身
#include "CommandObj.h"
#include <Psapi.h>

#pragma comment(lib,"Psapi.lib")

//////////////////////////////////////////////////////////////////////////
// 引用库文件

//////////////////////////////////////////////////////////////////////////
// 初始化及释放函数

CCommandObj::CCommandObj(void)
{  /*
   HMODULE hNTdll = GetModuleHandle (_T("NTdll.dll"));  
   (FARPROC&)ZwQueryInformationThread = GetProcAddress(hNTdll,"ZwQueryInformationThread");  
   (FARPROC&)RtlNtStatusToDosError = GetProcAddress(hNTdll,"RtlNtStatusToDosError");  */
}

CCommandObj::CCommandObj(LPCTSTR lpLogFileName) : CCommonControl(lpLogFileName)
{/*
 HMODULE hNTdll = GetModuleHandle (_T("NTdll.dll"));  
 (FARPROC&)ZwQueryInformationThread = GetProcAddress(hNTdll,"ZwQueryInformationThread");  
 (FARPROC&)RtlNtStatusToDosError = GetProcAddress(hNTdll,"RtlNtStatusToDosError");  */
}

CCommandObj::~CCommandObj(void)
{
}

//////////////////////////////////////////////////////////////////////////
// Variable:

ULONG CCommandObj::m_uFlags = 0;

HMODULE CCommandObj::m_hNtdll= 0;
HMODULE CCommandObj::m_hUser32 = 0;
HMODULE CCommandObj::m_hShell32 = 0;
HMODULE CCommandObj::m_hAdvAPI32 = 0;
HMODULE CCommandObj::m_hKernel32 = 0;
HMODULE CCommandObj::m_hCurrentModule = 0;
DWORD CCommandObj::m_dwUser32_ImageBase = 0;
DWORD CCommandObj::m_dwUser32_ImageHigh = 0;

CCommandObj::BHO_DATA CCommandObj::m_BhoData;
CCommandObj::INJECTION_DATA CCommandObj::m_InjectionData;

PLDRLOADDLL CCommandObj::m_pfnLdrLoadDll = NULL;
PREGENUMKEYW CCommandObj::m_pfnRegEnumKeyW = NULL;
PREGOPENKEYEXW CCommandObj::m_pfnRegOpenKeyExW = NULL;
PREGENUMKEYEXW CCommandObj::m_pfnRegEnumKeyExW = NULL;
PLOADLIBRARYEXW CCommandObj::m_pfnLoadLibraryExW = NULL;
PNTWRITEVIRTUALMEMORY CCommandObj::m_pfnNtWriteVirtualMemory = NULL;
PNTPROJECTVIRTUALMEMORY CCommandObj::m_pfnNtProtectVirtualMemory = NULL;

//////////////////////////////////////////////////////////////////////////
// Public:

bool CCommandObj::CreateProcessAttach(HMODULE hModule,ULONG uFlags /* = SECURITY_MONITOR_ALL */)
{
	bool bRet = false;

	m_uFlags = uFlags;

	bRet = InitializationModuleInfo(hModule);

	if (GetSecuritySwitch(SECURITY_MONITOR_INJECTION))
	{
		bRet = InitializationInjectionMonitor() && bRet;
	}
	if (GetSecuritySwitch(SECURITY_MONITOR_DLL))
	{
		bRet = InitializationLoadLibrayMonitor() && bRet;
	}
	if (GetSecuritySwitch(SECURITY_MONITOR_MEM))
	{
		bRet = InitializationMemoryMonitor() && bRet;
	}
	if (GetSecuritySwitch(SECURITY_MONITOR_BHO))
	{
		bRet = InitializationBHOMonitor() && bRet;
	}

	return bRet;
}

bool CCommandObj::CreateProcessDetach(HMODULE hModule)
{
	bool bRet = InitializationModuleInfo(hModule,false);

	if (GetSecuritySwitch(SECURITY_MONITOR_INJECTION))
	{
		bRet = InitializationInjectionMonitor(false) && bRet;
	}
	if (GetSecuritySwitch(SECURITY_MONITOR_DLL))
	{
		bRet = InitializationLoadLibrayMonitor(false) && bRet;
	}
	if (GetSecuritySwitch(SECURITY_MONITOR_MEM))
	{
		bRet = InitializationMemoryMonitor(false) && bRet;
	}
	if (GetSecuritySwitch(SECURITY_MONITOR_BHO))
	{
		//bRet = InitializationBHOMonitor(false) && bRet; //会导致报错.目前不知道原因
	}
	return bRet;
}

bool CCommandObj::CreateThreadAttach(HMODULE hModule)
{
	NTSTATUS NtStatus; 
	PVOID  pThreadStartAddr = NULL; 
	HANDLE hThread,hProcess; 

	if (GetSecuritySwitch(SECURITY_MONITOR_INJECTION))
	{
		hThread	 = GetCurrentThread(); 
		hProcess = GetCurrentProcess();

		if (hThread == NULL) 
			return FALSE;

		NtStatus = ZwQueryInformationThread (hThread,ThreadQuerySetWin32StartAddress,&pThreadStartAddr,sizeof (pThreadStartAddr),NULL);

		if (!NT_SUCCESS(NtStatus)) 
		{ 
			CloseHandle (hThread); 
			Show(RtlNtStatusToDosError(NtStatus)); 
			return FALSE; 
		};  
		if (pThreadStartAddr == LoadLibraryA || pThreadStartAddr == LoadLibraryW)
		{
			TCHAR szMessageBox[2048] = {0};

			Debug(Warning,_T("Create Remote Thread ID : [%d],Thread Starting Address is [0x%p]"),GetCurrentThreadId(),pThreadStartAddr);
			_stprintf(szMessageBox,_T("有远程线程注入[%d],起始地址为0x%p,是否禁止线程运行?\n"),GetCurrentThreadId(),pThreadStartAddr);

			//if (MessageBox(NULL,szMessageBox,_T("提示"),MB_YESNO | MB_ICONWARNING) != IDNO)
			{
				ExitThread(0);
			}
		}

		CloseHandle (hThread);
		CloseHandle (hProcess);
	}
	return TRUE;
}

bool CCommandObj::CreateThreadDetach(HMODULE hModule)
{
	return TRUE;
}

bool CCommandObj::InitializationModuleInfo(HMODULE hModule,bool bInstall /* = true */)
{
	bool bRet = false;
	MODULEINFO ModInfo = {0};

	if (bInstall)
	{
		Out(Dbg,L"Initialization Module Information Starting.");
		//获取User32.dll的加载基址和映象大小  
		if(GetModuleInformation(GetCurrentProcess(),GetModuleHandle(_T("User32.dll")),&ModInfo,sizeof(ModInfo)) == TRUE )
		{
			CCommandObj::m_dwUser32_ImageBase = (DWORD)ModInfo.lpBaseOfDll;
			CCommandObj::m_dwUser32_ImageHigh = (DWORD)ModInfo.lpBaseOfDll + ModInfo.SizeOfImage;
			Out(Dbg,L"User32 Module Image Information :Base = [0x%08X],High = [0x%08X].",m_dwUser32_ImageBase,m_dwUser32_ImageHigh);
		}

		m_hCurrentModule = hModule;
		m_hNtdll = GetModuleHandle(_T("Ntdll.dll"));
		m_hUser32 = GetModuleHandle(_T("User32.dll"));
		m_hShell32 = GetModuleHandle(_T("Shell32.dll"));
		m_hAdvAPI32 = GetModuleHandle(_T("AdvAPI32.dll"));
		m_hKernel32 = GetModuleHandle(_T("Kernel32.dll"));
		bRet = (m_dwUser32_ImageBase > 0 && m_dwUser32_ImageHigh > 0);

		Runend(bRet,_T("Initialization Module Information Stoping."));
	}
	else
	{
		bRet = true;
	}

	return bRet;
}

bool CCommandObj::InitializationBHOMonitor(bool bInstall /* = true */)
{
	bool bRet =false;

	if (bInstall)
	{
		Out(Dbg,_T("Initialization BHO Monitor Starting."));
		HINSTANCE hAdvapi = NULL;
		hAdvapi = LoadLibrary(_T("Advapi32.dll"));
		//RegOpenKeyEx & RegEnumKey
		bRet = InlineHook((PREGOPENKEYEXW)GetProcAddress(hAdvapi,"RegOpenKeyExW"),FakeRegOpenKeyExW,(void **)&CCommandObj::m_pfnRegOpenKeyExW);
		bRet = bRet && InlineHook((PREGENUMKEYW)GetProcAddress(hAdvapi,"RegEnumKeyW"),FakeRegEnumKeyW,(void **)&CCommandObj::m_pfnRegEnumKeyW);
		bRet = bRet && InlineHook((PREGENUMKEYEXW)GetProcAddress(hAdvapi,"RegEnumKeyExW"),FakeRegEnumKeyExW,(void **)&CCommandObj::m_pfnRegEnumKeyExW);

		CShareMemory<CSecurity::DATA_SECURITY_BHO> smShareMemory;
		smShareMemory.Init(0xffff);
		ULONG uUsed = smShareMemory.GetUsedObjectCount();

		CSecurity::PDATA_SECURITY_BHO pBho = smShareMemory.GetUsedObject(0);
		if (NULL != pBho)
		{
			CSecurity::PDATA_BHO pBhoData = (CSecurity::PDATA_BHO)(ULONG(pBho)+pBho->ArrayOffset);

			m_BhoData.Type = pBho->Type;
			for (ULONG i = 0; i < pBho->ArraySize; i++)
			{
				m_BhoData.ClsidList.insert(map<string,int>::value_type(ws2s(pBhoData[i].CLSID).c_str(),i));
			}
		}else
		{
			m_BhoData.Type = true;
		}
		Runend(bRet,_T("Initialization BHO Monitor Stoping."));
	}
	else
	{
		UnInlineHook(FakeRegEnumKeyW,(void **)&CCommandObj::m_pfnRegEnumKeyW);
		UnInlineHook(FakeRegEnumKeyExW,(void **)&CCommandObj::m_pfnRegEnumKeyExW);
		UnInlineHook(FakeRegOpenKeyExW,(void **)&CCommandObj::m_pfnRegOpenKeyExW);
		bRet = true;
	}

	return bRet;
}

bool CCommandObj::InitializationMemoryMonitor(bool bInstall /* = true */)
{
	bool bRet = TRUE;
	HINSTANCE hNtdll = NULL;

	if (bInstall)
	{
		Out(Dbg,L"Initialization Memory Monitor Starting.");
		hNtdll = LoadLibrary(_T("Ntdll.dll"));
		//由于已经进入本进程了,阻止无效
		bRet = InlineHook((PNTPROJECTVIRTUALMEMORY)GetProcAddress(hNtdll,"NtProtectVirtualMemory"),FakeNtProtectVirtualMemory,(void **)&CCommandObj::m_pfnNtProtectVirtualMemory);
		//bRet = InlineHook((PNTWRITEVIRTUALMEMORY)GetProcAddress(hNtdll,"NtWriteVirtualMemory"),FakeNtWriteVirtualMemory,(void **)&m_pfnNtWriteVirtualMemory) && bRet;
		Runend(bRet,_T("Initialization Memory Monitor Stoping."));
	}
	else
	{
		UnInlineHook(FakeNtProtectVirtualMemory,(void **)&CCommandObj::m_pfnNtProtectVirtualMemory);
	}
	
	return bRet;
}

bool CCommandObj::InitializationLoadLibrayMonitor(bool bInstall /* = true */)
{
	bool bRet =false; 
	HINSTANCE hNTDLL = NULL;

	if (bInstall)
	{
		Out(Dbg,_T("Initialization LoadLibray Monitor Starting."));
		hNTDLL = LoadLibrary(_T("Ntdll.dll"));
		//LdrLoadDll
		bRet = InlineHook((PLDRLOADDLL)GetProcAddress(hNTDLL,"LdrLoadDll"),FakeLdrLoadDll,(void **)&CCommandObj::m_pfnLdrLoadDll);
		Runend(bRet,_T("Initialization LoadLibray Monitor Stoping."));
	}
	else
	{
		UnInlineHook(FakeLdrLoadDll,(void **)&CCommandObj::m_pfnLdrLoadDll);
		bRet = true;
	}

	return bRet;
}

bool CCommandObj::InitializationInjectionMonitor(bool bInstall /* = true */)
{
	bool bRet = false;
	HINSTANCE hKernel32 = NULL;

	if (bInstall)
	{
		Out(Dbg,_T("Initialization Injection Monitor Starting."));
		CCommandObj::m_InjectionData.Key.clear();
		hKernel32 = LoadLibrary(_T("Kernel32.dll"));
		//LdrLoadDll
		bRet = InlineHook((PLDRLOADDLL)GetProcAddress(hKernel32,"LoadLibraryExW"),FakeLoadLibraryExW,(void **)&CCommandObj::m_pfnLoadLibraryExW);
		Runend(bRet,_T("Initialization Injection Monitor Stoping."));
	}
	else
	{
		UnInlineHook(FakeLoadLibraryExW,(void **)&CCommandObj::m_pfnLoadLibraryExW);
		bRet = true;
	}

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// BHO Monitor

LONG CCommandObj::FakeRegOpenKeyExW(HKEY hKey,LPCWSTR lpSubKey,DWORD ulOptions,REGSAM samDesired,PHKEY phkResult)
{
	if (NULL == lpSubKey)
	{
		return m_pfnRegOpenKeyExW(hKey,lpSubKey,ulOptions,samDesired,phkResult);
	}

	if (0 == wcsicmp(lpSubKey,L"SOFTWARE\\Microsoft\\Internet Explorer\\Extensions") || 
		0 == wcsicmp(lpSubKey,L"Software\\Microsoft\\Internet Explorer\\URLSearchHooks") || 
		/*0 == wcsicmp(lpSubKey,L"Software\\Microsoft\\Windows\\CurrentVersion\\Ext\\Stats") ||
		0 == wcsicmp(lpSubKey,L"Software\\Microsoft\\Windows\\CurrentVersion\\Ext\\Settings") ||*/
		0 == wcsicmp(lpSubKey,L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Browser Helper Objects"))
	{
		LONG lRet =  m_pfnRegOpenKeyExW(hKey,lpSubKey,ulOptions,samDesired,phkResult);
	
		// 添加到Hook列表
		
		CLock lock(CCommandObj::m_BhoData.Section);

		CCommandObj::m_BhoData.Key[m_BhoData.dwIndex] = *phkResult;
		CCommandObj::m_BhoData.dwIndex++;
		if (CCommandObj::m_BhoData.dwIndex >= MAX_HREG_QUANTITY)
		{
			CCommandObj::m_BhoData.dwIndex = 0;
		}

		//添加结束

		Debug(Dbg,_T("Reg Open Key[%d] : %s [0x%08X]."),CCommandObj::m_BhoData.dwIndex,lpSubKey,*phkResult);
		return lRet;//2
	}else if (0 == wcsicmp(lpSubKey,L"SOFTWARE\\Microsoft\\Internet Explorer\\Toolbar"))
	{
		return 2;
	}
		
	return m_pfnRegOpenKeyExW(hKey,lpSubKey,ulOptions,samDesired,phkResult);
}

LONG CCommandObj::FakeRegEnumKeyExW(HKEY hKey,DWORD dwIndex,LPWSTR lpName,LPDWORD lpcName,LPDWORD lpReserved,LPWSTR lpClass,LPDWORD lpcClass,PFILETIME lpftLastWriteTime)
{
	LONG uRet = m_pfnRegEnumKeyExW(hKey,dwIndex,lpName,lpcName,lpReserved,lpClass,lpcClass,lpftLastWriteTime);
	
	// 检查句柄

	CLock lock(CCommandObj::m_BhoData.Section);

	for (int i = 0; i < MAX_HREG_QUANTITY;i++)
	{

		if (m_BhoData.Key[i] != NULL && hKey == m_BhoData.Key[i])
		{

			if (m_BhoData.Type)
			{
				if (CCommandObj::m_BhoData.ClsidList.find(ws2s(wcslwr(lpName))) != CCommandObj::m_BhoData.ClsidList.end())
				{
					Debug(Outputs,_T("Enum Key Ex: %s[0x%08X] Refuse."),lpName,hKey);
					wcscpy_s(lpName,1,L"\0");
					*lpcName = 0;
					return uRet;
				}
			}
			else
			{
				if (CCommandObj::m_BhoData.ClsidList.find(ws2s(wcslwr(lpName))) == CCommandObj::m_BhoData.ClsidList.end())
				{
					Debug(Outputs,_T("Enum Key Ex: %s[0x%08X] Refuse."),lpName,hKey);
					wcscpy_s(lpName,1,L"\0");
					*lpcName = 0;
					return uRet;
				}
			}

		}
	}

	//检查结束

	return uRet;
}

LONG CCommandObj::FakeRegEnumKeyW(HKEY hKey,DWORD dwIndex,LPWSTR lpName,DWORD cchName)
{
	LONG uRet = m_pfnRegEnumKeyW(hKey,dwIndex,lpName,cchName);;

	// 检查句柄

	CLock lock(CCommandObj::m_BhoData.Section);

	for (int i = 0; i < MAX_HREG_QUANTITY;i++)
	{
		if (m_BhoData.Key[i] != NULL && hKey == m_BhoData.Key[i])
		{
			if (m_BhoData.Type)
			{
				if (CCommandObj::m_BhoData.ClsidList.find(ws2s(wcslwr(lpName))) != CCommandObj::m_BhoData.ClsidList.end())
				{
					Debug(Outputs,_T("Enum Key Ex: %s[0x%08X] Refuse."),lpName,hKey);
					wcscpy_s(lpName,1,L"\0");
					return uRet;
				}
			}else
			{
				if (CCommandObj::m_BhoData.ClsidList.find(ws2s(wcslwr(lpName))) == CCommandObj::m_BhoData.ClsidList.end())
				{
					Debug(Outputs,_T("Enum Key Ex: %s[0x%08X] Refuse."),lpName,hKey);
					wcscpy_s(lpName,1,L"\0");
					return uRet;
				}
			}
		}
	}

	//检查结束

	return uRet;
}

//////////////////////////////////////////////////////////////////////////
// Memory Monitor

NTSTATUS CCommandObj::FakeNtProtectVirtualMemory(IN HANDLE ProcessHandle,IN OUT PVOID *BaseAddress,IN OUT PULONG NumberOfBytesToProtect,IN ULONG NewAccessProtection,OUT PULONG OldAccessProtection )
{
	NTSTATUS nRet;

	DWORD dwRetAddr = 0;
	__asm
	{
		mov eax, [ebp+4];
		sub eax, 5;
		mov dwRetAddr, eax;
	}

	HMODULE hModule = CheckMemoryAddress(*BaseAddress);

	if(NULL != hModule)
	{
		TCHAR szModuleName[MAX_PATH+1] = {0};

		GetModuleFileName(hModule,szModuleName,MAX_PATH);

		if (GetModuleHandle(_T("ieframe.dll")) == hModule)
		{
			Debug(Warning,_T("Tries To Modify The Virtual Memory Protection,Module Name Is %s."),szModuleName);

			return STATUS_CONFLICTING_ADDRESSES;
		}

	}
	nRet = m_pfnNtProtectVirtualMemory(ProcessHandle,BaseAddress,NumberOfBytesToProtect,NewAccessProtection,OldAccessProtection);

	return nRet;
}

NTSTATUS CCommandObj::FakeNtWriteVirtualMemory(IN HANDLE ProcessHandle,IN PVOID BaseAddress,IN PVOID Buffer,IN ULONG NumberOfBytesToWrite,OUT PULONG NumberOfBytesWritten OPTIONAL )
{
	NTSTATUS nRet;

	DWORD dwRetAddr = 0;

	__asm
	{
		mov eax, [ebp+4];
		sub eax, 5;
		mov dwRetAddr, eax;
	}

	nRet = m_pfnNtWriteVirtualMemory(ProcessHandle,BaseAddress,Buffer,NumberOfBytesToWrite,NumberOfBytesWritten);

	return nRet;
}

//////////////////////////////////////////////////////////////////////////
// LoadLibray Monitor

NTSTATUS CCommandObj::FakeLdrLoadDll(IN PWCHAR PathToFile OPTIONAL,IN ULONG Flags OPTIONAL,IN PUNICODE_STRING ModuleFileName,OUT PHANDLE ModuleHandle )
{
	PVOID dwRetAddr = NULL;
	WCHAR szModuleFileName[MAX_PATH+1] = {0};

	__asm
	{
		mov eax, [ebp+4];
		sub eax, 5;
		mov dwRetAddr, eax;
	}

	__try
	{
		wcscpy_s(szModuleFileName,ModuleFileName->Length,ModuleFileName->Buffer);
		if (NULL != wcsstr(_wcslwr(szModuleFileName),L"iebrowserex.dll"))
		{

			HMODULE hModule = CCommandObj::GetModuleHandleFromAddress(dwRetAddr);

			if(NULL != hModule)
			{
				GetModuleFileNameW(hModule,szModuleFileName,MAX_PATH);
			}

			DebugW(WarningW,L"Call LoadLibrary Module Name Is %s,Load Library Name [%s],Find Path is [%s],Refuse load.",szModuleFileName,ModuleFileName->Buffer,PathToFile);

			return STATUS_DLL_NOT_FOUND;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
	}

	NTSTATUS  nRet = m_pfnLdrLoadDll(PathToFile,Flags,ModuleFileName,ModuleHandle);


	return nRet;

}

//////////////////////////////////////////////////////////////////////////
// Injection Monitor

HMODULE CCommandObj::FakeLoadLibraryExW(LPCWSTR lpLibFileName,HANDLE hFile,DWORD dwFlags)
{
	DWORD dwRetAddr = NULL;

	__asm
	{
		mov eax, [ebp+4];
		sub eax, 5;
		mov dwRetAddr, eax;
	}
	CLock cLock(CCommandObj::m_InjectionData.Section);
	if (CCommandObj::m_InjectionData.Key.find(ws2s(lpLibFileName)) != CCommandObj::m_InjectionData.Key.end())
	{
		return NULL;
	}
	 WCHAR szProcessPath[MAX_PATH+1] = {0};
	GetModuleFileName(NULL,szProcessPath,MAX_PATH);
	if(dwRetAddr > m_dwUser32_ImageBase && dwRetAddr < m_dwUser32_ImageHigh && 
		0 != wcsicmp(lpLibFileName,szProcessPath) && //自己本身,要不然会导致https://epay.163.com/控件输入不了密码
		0 != wcsicmp(lpLibFileName,L"C:\\WINDOWS\\system32\\MSCTF.dll") && 
		0 != wcsicmp(lpLibFileName,L"C:\\WINDOWS\\system32\\uxtheme.dll"))
	{
		CCommandObj::m_InjectionData.Key.insert(map<string,int>::value_type(ws2s(lpLibFileName),dwRetAddr));
		DebugW(WarningW,L"Global Injection Library Name [%s],Refuse Load.",lpLibFileName);
		return NULL;
	}

	return m_pfnLoadLibraryExW(lpLibFileName,hFile,dwFlags);
}

//////////////////////////////////////////////////////////////////////////
// Private:

bool CCommandObj::GetSecuritySwitch(ULONG uFlags)
{
	return (uFlags == (m_uFlags & uFlags));
}

BOOL CCommandObj::Lock_CurrentProcess()
{
	HANDLE hProcess = ::GetCurrentProcess();
	SID_IDENTIFIER_AUTHORITY sia = SECURITY_WORLD_SID_AUTHORITY;
	PSID pSid;
	BOOL bSus = FALSE;
	bSus = ::AllocateAndInitializeSid(&sia,1,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,&pSid);
	if(!bSus) goto Cleanup;
	HANDLE hToken;
	bSus = ::OpenProcessToken(hProcess,TOKEN_QUERY,&hToken);
	if(!bSus) goto Cleanup;
	DWORD dwReturnLength;
	::GetTokenInformation(hToken,TokenUser,NULL,NULL,&dwReturnLength);
	if(dwReturnLength > 0x400) goto Cleanup;
	LPVOID TokenInformation;
	TokenInformation = ::LocalAlloc(LPTR,0x400);//这里就引用SDK的函数不引用CRT的了
	DWORD dw;
	bSus = ::GetTokenInformation(hToken,TokenUser,TokenInformation,0x400,&dw);
	if(!bSus) goto Cleanup;
	PTOKEN_USER pTokenUser = (PTOKEN_USER)TokenInformation;
	BYTE Buf[0x200];
	PACL pAcl = (PACL)&Buf;
	bSus = ::InitializeAcl(pAcl,1024,ACL_REVISION);
	if(!bSus) goto Cleanup;
	bSus = ::AddAccessDeniedAce(pAcl,ACL_REVISION,0x000000FA,pSid);
	if(!bSus) goto Cleanup;
	bSus = ::AddAccessAllowedAce(pAcl,ACL_REVISION,0x00100701,pTokenUser->User.Sid);
	if(!bSus) goto Cleanup;
	if(::SetSecurityInfo(hProcess,SE_KERNEL_OBJECT,DACL_SECURITY_INFORMATION | PROTECTED_DACL_SECURITY_INFORMATION,NULL,NULL,pAcl,NULL) == 0)
		bSus = TRUE;
Cleanup:
	if(hProcess != NULL)
		::CloseHandle(hProcess);
	if(pSid != NULL)
		::FreeSid(pSid);
	return bSus;
}

HMODULE CCommandObj::CheckMemoryAddress(PVOID pAddress)
{

	//Return Value:
	//TRUE = bypass
	//FALSE = not bypass

	//
	//Check Memory Address
	//
	HMODULE hModule = CCommandObj::GetModuleHandleFromAddress((PVOID)pAddress);

	if (hModule == NULL)
		return NULL;

	if(hModule == m_hNtdll || hModule == m_hUser32 || hModule == m_hShell32 || hModule == m_hAdvAPI32 || hModule == m_hKernel32 || hModule == m_hCurrentModule)
		return hModule;

	return NULL;
}

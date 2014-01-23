#pragma once

#include <map>

using namespace std;

//////////////////////////////////////////////////////////////////////////
// 定义宏

#define MAX_HREG_QUANTITY					50

#define SECURITY_MONITOR_INJECTION					0x00000001
#define SECURITY_MONITOR_DLL						0x00000002
#define SECURITY_MONITOR_MEM						0x00000004
#define SECURITY_MONITOR_BHO						0x00000008
#define SECURITY_MONITOR_ALL						0xFFFFFFFF

//////////////////////////////////////////////////////////////////////////
// API定义
typedef NTSTATUS (WINAPI * PRTLADJUSTPRIVILEGE)(ULONG Privilege,BOOLEAN Enable,BOOLEAN CurrentThread,PBOOLEAN Enabled);
typedef NTSTATUS (WINAPI * PNTWRITEVIRTUALMEMORY)(HANDLE,PVOID,PVOID,ULONG,PULONG);
typedef NTSTATUS (WINAPI * PNTPROJECTVIRTUALMEMORY)(HANDLE,PVOID *,PULONG,ULONG,PULONG);

typedef NTSTATUS (WINAPI * PLDRLOADDLL) (PWCHAR,ULONG,PUNICODE_STRING,PHANDLE);
typedef HMODULE  (WINAPI * PLOADLIBRARYEXW)(LPCWSTR,HANDLE,DWORD);

typedef LONG	 (WINAPI * PREGENUMKEYEXW)(HKEY,DWORD,LPWSTR,LPDWORD,LPDWORD,LPWSTR,LPDWORD,PFILETIME);
typedef LONG	 (WINAPI * PREGENUMKEYW)(HKEY,DWORD,LPWSTR,DWORD); 
typedef LONG	 (WINAPI * PREGOPENKEYEXW)(HKEY,LPCWSTR,DWORD,REGSAM,PHKEY);


EXTERN_C PVOID _ReturnAddress();

//////////////////////////////////////////////////////////////////////////
// 定义数据结构

/*
// 
// Thread Information Classes 
//  
typedef enum _THREADINFOCLASS { 
	ThreadBasicInformation, 
	ThreadTimes, 
	ThreadPriority, 
	ThreadBasePriority, 
	ThreadAffinityMask, 
	ThreadImpersonationToken, 
	ThreadDescriptorTableEntry, 
	ThreadEnableAlignmentFaultFixup, 
	ThreadEventPair_Reusable, 
	ThreadQuerySetWin32StartAddress, 
	ThreadZeroTlsCell, 
	ThreadPerformanceCount, 
	ThreadAmILastThread, 
	ThreadIdealProcessor, 
	ThreadPriorityBoost, 
	ThreadSetTlsArrayAddress, 
	ThreadIsIoPending, 
	ThreadHideFromDebugger, 
	ThreadBreakOnTermination, 
	MaxThreadInfoClass 
} THREADINFOCLASS;*/  
typedef struct _CLIENT_ID { 
	HANDLE UniqueProcess; 
	HANDLE UniqueThread; 
} CLIENT_ID; 
typedef CLIENT_ID *PCLIENT_ID;  
typedef struct _THREAD_BASIC_INFORMATION { // Information Class 0 
	LONG  ExitStatus; 
	PVOID TebBaseAddress; 
	CLIENT_ID ClientId; 
	LONG AffinityMask; 
	LONG Priority; 
	LONG BasePriority; 
} THREAD_BASIC_INFORMATION, *PTHREAD_BASIC_INFORMATION;  




//////////////////////////////////////////////////////////////////////////



class CCommandObj : public CCommonControl
{
public:
	CCommandObj(void);
	CCommandObj(LPCTSTR lpLogFileName);
	~CCommandObj(void);

	//////////////////////////////////////////////////////////////////////////
	// 
	// Explain:  初始化函数 CreateProcessAttach()
	// Return:  成功True,否则False
	bool CreateProcessAttach(HMODULE hModule,ULONG uFlags = SECURITY_MONITOR_ALL);

	//////////////////////////////////////////////////////////////////////////
	// 
	// Explain:  卸载函数 CreateProcessDetach()
	// Return:  成功True,否则False
	bool CreateProcessDetach(HMODULE hModule);

	//////////////////////////////////////////////////////////////////////////
	// 
	// Explain:  初始化程序Initlization()
	// Return:  成功True,否则False
	bool CreateThreadAttach(HMODULE hModule);

	//////////////////////////////////////////////////////////////////////////
	// 
	// Explain:  初始化程序Initlization()
	// Return:  成功True,否则False
	bool CreateThreadDetach(HMODULE hModule);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  初始化网络通讯模块 InitializationSocketClient(LPCTSTR lpServerAddress = NULL,USHORT usServerPort = DEFAULT_SERVER_PORT)
	//	Parameter:	lpServerAddress = 服务器IP，建议为空 
	//	Parameter:	usServerPort = 服务器端口，建议为空
	//	Return: 成功True,否则False     

	bool InitializationLoadLibrayMonitor(bool bInstall = true);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  初始化网络通讯模块 InitializationSocketClient(LPCTSTR lpServerAddress = NULL,USHORT usServerPort = DEFAULT_SERVER_PORT)
	//	Parameter:	lpServerAddress = 服务器IP，建议为空 
	//	Parameter:	usServerPort = 服务器端口，建议为空
	//	Return: 成功True,否则False     

	bool InitializationInjectionMonitor(bool bInstall = true);
	bool InitializationMemoryMonitor(bool bInstall = true);
	bool InitializationBHOMonitor(bool bInstall = true);

	bool InitializationModuleInfo(HMODULE hModule,bool bInstall = true);

	BOOL Lock_CurrentProcess();

private:
	static HMODULE WINAPI FakeLoadLibraryExW(LPCWSTR,HANDLE,DWORD);

	static LONG WINAPI FakeRegEnumKeyW(HKEY,DWORD,LPWSTR,DWORD);

	static LONG WINAPI FakeRegEnumKeyExW(HKEY,DWORD,LPWSTR,LPDWORD,LPDWORD,LPWSTR,LPDWORD,PFILETIME);

	static LONG WINAPI FakeRegOpenKeyExW(HKEY,LPCWSTR,DWORD,REGSAM,PHKEY);

	static NTSTATUS WINAPI FakeLdrLoadDll(PWCHAR,ULONG,PUNICODE_STRING,PHANDLE);

	static NTSTATUS WINAPI FakeNtWriteVirtualMemory(HANDLE,PVOID,PVOID,ULONG,PULONG);


	#define STATUS_CONFLICTING_ADDRESSES     ((NTSTATUS)0xC0000018L)
	static NTSTATUS WINAPI FakeNtProtectVirtualMemory(HANDLE,PVOID *,PULONG,ULONG,PULONG);

	static HMODULE CheckMemoryAddress(PVOID pAddress);

	static bool GetSecuritySwitch(ULONG uFlags);


protected:
	static PLDRLOADDLL m_pfnLdrLoadDll;
	static PREGENUMKEYW m_pfnRegEnumKeyW;
	static PREGOPENKEYEXW	m_pfnRegOpenKeyExW;
	static PLOADLIBRARYEXW	m_pfnLoadLibraryExW;
	static PREGENUMKEYEXW	m_pfnRegEnumKeyExW;
	static PNTWRITEVIRTUALMEMORY m_pfnNtWriteVirtualMemory;
	static PNTPROJECTVIRTUALMEMORY m_pfnNtProtectVirtualMemory;

	static ULONG m_uFlags;
	CHookControlTool m_cHookControlTool;

protected:
	typedef struct _BHO_DATA
	{
		DWORD dwIndex;
		CriSection Section;
		HKEY Key[MAX_HREG_QUANTITY];

		bool Type;
		map<string,int> ClsidList;
		_BHO_DATA()
		{
			Type = true;
			dwIndex = 0;
			memset(Key,0,sizeof(HKEY) * MAX_HREG_QUANTITY);
		}
	}BHO_DATA,* PBHO_DATA;
	typedef struct _INJECTION_DATA
	{
		CriSection Section;
		map<string,int> Key;
	}INJECTION_DATA,* PINJECTION_DATA;


	static INJECTION_DATA	m_InjectionData;
	static BHO_DATA			m_BhoData;

protected:
	//Ntdll.dll
	static HMODULE m_hNtdll;

	//user32.dll
	static HMODULE m_hUser32;

	//shell32.dll
	static HMODULE m_hShell32;

	//advapi32.dll
	static HMODULE m_hAdvAPI32;

	//kernel32.dll
	static HMODULE m_hKernel32;

	//Current module
	static HMODULE m_hCurrentModule;

	static DWORD m_dwUser32_ImageBase;
	static DWORD m_dwUser32_ImageHigh;

};


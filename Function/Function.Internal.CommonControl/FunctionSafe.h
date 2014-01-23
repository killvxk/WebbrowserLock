#pragma once


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
} THREADINFOCLASS;

class CFunctionSafe
{
public:
	CFunctionSafe(void);
	~CFunctionSafe(void);
public: 
	typedef HFILE (WINAPI *POpenFile)(LPCSTR,LPOFSTRUCT,UINT);
	typedef DWORD (WINAPI *PNtQueryObject)( HANDLE, DWORD, VOID*, DWORD, VOID* ); 
	typedef DWORD (WINAPI *PNtQuerySystemInformation)( DWORD, VOID*, DWORD, ULONG* ); 
	typedef DWORD (WINAPI *PNtQueryInformationThread)(HANDLE, ULONG, PVOID,	DWORD, DWORD* ); 
	typedef DWORD (WINAPI *PNtQueryInformationFile)(HANDLE, PVOID,	PVOID, DWORD, DWORD ); 
	typedef DWORD (WINAPI *PNtQueryInformationProcess)(HANDLE, DWORD, PVOID, DWORD, PVOID ); 
	typedef LONG (WINAPI *PZwQueryInformationThread)(HANDLE,THREADINFOCLASS,PVOID,ULONG,PULONG);
	typedef LONG (WINAPI *PRtlNtStatusToDosError) (ULONG);

public: 
	static POpenFile					OpenFile;
	static PNtQuerySystemInformation	NtQuerySystemInformation; 
	static PNtQueryObject				NtQueryObject; 
	static PNtQueryInformationThread	NtQueryInformationThread; 
	static PNtQueryInformationFile		NtQueryInformationFile; 
	static PNtQueryInformationProcess	NtQueryInformationProcess; 
	static PRtlNtStatusToDosError		RtlNtStatusToDosError; 
	static PZwQueryInformationThread	ZwQueryInformationThread; 

	static DWORD						dwNTMajorVersion; 

public:
	static LONG DefaultFuncallCall(...);

protected: 
	PVOID GetFunctionAddress(HINSTANCE hMoudle,LPCSTR lpFunctionName);
};


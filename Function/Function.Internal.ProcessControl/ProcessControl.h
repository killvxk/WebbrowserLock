#pragma once
#include <Windows.h>
#include <tchar.h>
#include <Tlhelp32.h>

#include <Function.Internal.CommonControl\\CommonControlTool.h>

#define SE_DEBUG_PRIVILEGE 0x13
#define OB_TYPE_INDEX_TYPE              1 // [ObjT] "Type"
#define OB_TYPE_INDEX_DIRECTORY         2 // [Dire] "Directory"
#define OB_TYPE_INDEX_SYMBOLIC_LINK     3 // [Symb] "SymbolicLink"
#define OB_TYPE_INDEX_TOKEN             4 // [Toke] "Token"
#define OB_TYPE_INDEX_PROCESS           5 // [Proc] "Process"
#define OB_TYPE_INDEX_THREAD            6 // [Thre] "Thread"
#define OB_TYPE_INDEX_JOB               7 // [Job ] "Job"
#define OB_TYPE_INDEX_EVENT             8 // [Even] "Event"
#define OB_TYPE_INDEX_EVENT_PAIR        9 // [Even] "EventPair"
#define OB_TYPE_INDEX_MUTANT           10 // [Muta] "Mutant"
#define OB_TYPE_INDEX_CALLBACK         11 // [Call] "Callback"
#define OB_TYPE_INDEX_SEMAPHORE        12 // [Sema] "Semaphore"
#define OB_TYPE_INDEX_TIMER            13 // [Time] "Timer"
#define OB_TYPE_INDEX_PROFILE          14 // [Prof] "Profile"
#define OB_TYPE_INDEX_WINDOW_STATION   15 // [Wind] "WindowStation"
#define OB_TYPE_INDEX_DESKTOP          16 // [Desk] "Desktop"
#define OB_TYPE_INDEX_SECTION          17 // [Sect] "Section"
#define OB_TYPE_INDEX_KEY              18 // [Key ] "Key"
#define OB_TYPE_INDEX_PORT             19 // [Port] "Port"
#define OB_TYPE_INDEX_WAITABLE_PORT    20 // [Wait] "WaitablePort"
#define OB_TYPE_INDEX_ADAPTER          21 // [Adap] "Adapter"
#define OB_TYPE_INDEX_CONTROLLER       22 // [Cont] "Controller"
#define OB_TYPE_INDEX_DEVICE           23 // [Devi] "Device"
#define OB_TYPE_INDEX_DRIVER           24 // [Driv] "Driver"
#define OB_TYPE_INDEX_IO_COMPLETION    25 // [IoCo] "IoCompletion"
#define OB_TYPE_INDEX_FILE             26 // [File] "File"
#define OB_TYPE_INDEX_WMI_GUID         27 // [WmiG] "WmiGuid" 

typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemBasicInformation,
	SystemProcessorInformation,             // obsolete...delete
	SystemPerformanceInformation,
	SystemTimeOfDayInformation,
	SystemPathInformation,
	SystemProcessInformation,
	SystemCallCountInformation,
	SystemDeviceInformation,
	SystemProcessorPerformanceInformation,
	SystemFlagsInformation,
	SystemCallTimeInformation,
	SystemModuleInformation,
	SystemLocksInformation,
	SystemStackTraceInformation,
	SystemPagedPoolInformation,
	SystemNonPagedPoolInformation,
	SystemHandleInformation,
	SystemObjectInformation,
	SystemPageFileInformation,
	SystemVdmInstemulInformation,
	SystemVdmBopInformation,
	SystemFileCacheInformation,
	SystemPoolTagInformation,
	SystemInterruptInformation,
	SystemDpcBehaviorInformation,
	SystemFullMemoryInformation,
	SystemLoadGdiDriverInformation,
	SystemUnloadGdiDriverInformation,
	SystemTimeAdjustmentInformation,
	SystemSummaryMemoryInformation,
	SystemMirrorMemoryInformation,
	SystemPerformanceTraceInformation,
	SystemObsolete0,
	SystemExceptionInformation,
	SystemCrashDumpStateInformation,
	SystemKernelDebuggerInformation,
	SystemContextSwitchInformation,
	SystemRegistryQuotaInformation,
	SystemExtendServiceTableInformation,
	SystemPrioritySeperation,
	SystemVerifierAddDriverInformation,
	SystemVerifierRemoveDriverInformation,
	SystemProcessorIdleInformation,
	SystemLegacyDriverInformation,
	SystemCurrentTimeZoneInformation,
	SystemLookasideInformation,
	SystemTimeSlipNotification,
	SystemSessionCreate,
	SystemSessionDetach,
	SystemSessionInformation,
	SystemRangeStartInformation,
	SystemVerifierInformation,
	SystemVerifierThunkExtend,
	SystemSessionProcessInformation,
	SystemLoadGdiDriverInSystemSpace,
	SystemNumaProcessorMap,
	SystemPrefetcherInformation,
	SystemExtendedProcessInformation,
	SystemRecommendedSharedDataAlignment,
	SystemComPlusPackage,
	SystemNumaAvailableMemory,
	SystemProcessorPowerInformation,
	SystemEmulationBasicInformation,
	SystemEmulationProcessorInformation,
	SystemExtendedHandleInformation,
	SystemLostDelayedWriteInformation,
	SystemBigPoolInformation,
	SystemSessionPoolTagInformation,
	SystemSessionMappedViewInformation,
	SystemHotpatchInformation,
	SystemObjectSecurityMode,
	SystemWatchdogTimerHandler,
	SystemWatchdogTimerInformation,
	SystemLogicalProcessorInformation,
	SystemWow64SharedInformation,
	SystemRegisterFirmwareTableInformationHandler,
	SystemFirmwareTableInformation,
	SystemModuleInformationEx,
	SystemVerifierTriageInformation,
	SystemSuperfetchInformation,
	SystemMemoryListInformation,
	SystemFileCacheInformationEx,
	MaxSystemInfoClass  // MaxSystemInfoClass should always be the last enum
} SYSTEM_INFORMATION_CLASS; 

//内存读写数据结构
typedef struct _MEMORY_CHUNKS {

	ULONG Address;

	PVOID Data;

	ULONG Length;

}MEMORY_CHUNKS, *PMEMORY_CHUNKS;

//内存读写命令行数据结构
typedef enum _SYSDBG_COMMAND {
	//从内核空间拷贝到用户空间，或者从用户空间拷贝到用户空间
	//但是不能从用户空间拷贝到内核空间
	SysDbgCopyMemoryChunks_0 = 8,
	//SysDbgReadVirtualMemory = 8,

	//从用户空间拷贝到内核空间，或者从用户空间拷贝到用户空间
	//但是不能从内核空间拷贝到用户空间
	SysDbgCopyMemoryChunks_1 = 9,
	//SysDbgWriteVirtualMemory = 9,
} SYSDBG_COMMAND, *PSYSDBG_COMMAND;
typedef struct _PROCESSINFO
{
	DWORD dwProcessId;
	HANDLE hProcess;
	TCHAR sExeFile[MAX_PATH];
}PROCESSINFO,*PPROCESSINFO;
//进程信息数据结构
typedef struct _SYSTEM_HANDLE_INFORMATION{
	ULONG ProcessID; //进程的标识ID
	UCHAR ObjectTypeNumber; //对象类型
	UCHAR Flags; //0x01 = PROTECT_FROM_CLOSE,0x02 = INHERIT
	USHORT Handle; //对象句柄的数值
	PVOID Object; //对象句柄所指的内核对象地址
	ACCESS_MASK GrantedAccess; //创建句柄时所准许的对象的访问权
}SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;

//ZwSystemDebugControl()函数声明
typedef NTSTATUS (NTAPI *PZWSYSTEMDEBUGCONTROL)(SYSDBG_COMMAND,PVOID,ULONG,PVOID,ULONG,PULONG);
//NtQuerySystemInforamation()函数声明
typedef NTSTATUS (NTAPI *PNTQUERYSYSTEMINFORMATION)(SYSTEM_INFORMATION_CLASS,PVOID,ULONG,PULONG);
//ZwAllocateVirtualMemory()函数声明
typedef NTSTATUS (NTAPI *PZWALLOCATEVIRTUALMEMORY)(HANDLE, PVOID *, ULONG, PULONG, ULONG, ULONG);
//ZwFreeVirtualMemory()函数声明
typedef NTSTATUS (NTAPI *PZWFREEVIRTUALMEMORY)(HANDLE, PVOID *, PDWORD, ULONG);
//进程控制类
class CProcess : CCommonControl {
public:
	CProcess();
	CProcess(HANDLE hProcessHand);
	~CProcess();
	//////////////////////////////////////////////////////////////////////////
	//
	//参数的含义：
	//	cProcessPath [In].                         
	//	需要执行的程序路径，符号\用\\替换
	//
	//	cRunCommendLink [In] . 
	//	执行程序的命令行.
	//
	//////////////////////////////////////////////////////////////////////////
	PROCESS_INFORMATION ProcessCreate(PTCHAR cProcessPath,PTCHAR cRunCommendLink);
	HANDLE GetProcessHand();
	HANDLE GetThreadHand();
	DWORD GetProcessID();
	DWORD GetThreadID();
	BOOL HideProcess();
	BOOL EnableDebugPriv(PTCHAR name);
	STATUS GetCurrentProcessPath(LPSTR lpProcessPath);
	BOOL HaveProcess (PPROCESSINFO pProcess);
	BOOL PrivilegeControl(ULONG uPrivilege,BOOLEAN bEnable,BOOLEAN bCurrentThread);
protected:

	BOOL InitializeFunction();
	BOOL SystemDebugControl(ULONG uAddress,PVOID pvData,ULONG usize,SYSDBG_COMMAND command);
	BOOL OperateSystemMemory(MEMORY_CHUNKS datas,SYSDBG_COMMAND command);
protected:
	PZWSYSTEMDEBUGCONTROL ZwSystemDebugControl;
	PNTQUERYSYSTEMINFORMATION NtQuerySystemInformation;
	PZWALLOCATEVIRTUALMEMORY ZwAllocateVirtualMemory;
	PZWFREEVIRTUALMEMORY ZwFreeVirtualMemory;
	PROCESS_INFORMATION m_ProcInfo;//该变量保存新建进程的标志信息
private:

};

//////////////////////////////////////////////////////////////////////////
//
//参数的含义：
//	uPrivailege [In] Privilege index to change.                         
//	 所需要的权限名称，可以到MSDN查找关于Process Token & Privilege内容可以查到
//
//	bEnable [In] If TRUE, then enable the privilege otherwise disable. 
//	 如果为True 就是打开相应权限，如果为False 则是关闭相应权限
//
//	bCurrentThread [In] If TRUE, then enable in calling thread, otherwise process. 
//	 如果为True 则仅提升当前线程权限，否则提升整个进程的权限
//
///////////////////////////////////////////////////////////////////////////
BOOL PrivilegeControl(const char *uPrivailege,BOOLEAN bEnable,BOOLEAN bCurrentThread);
#include "ProcessControl.h"

CProcess::CProcess()
{
	m_ProcInfo.hProcess = NULL;
	m_ProcInfo.hThread = NULL;
	m_ProcInfo.dwProcessId = NULL;
	m_ProcInfo.dwThreadId = NULL;
}

CProcess::CProcess(HANDLE hProcessHand)
{
	m_ProcInfo.hProcess = hProcessHand;
	m_ProcInfo.hThread = NULL;
	m_ProcInfo.dwProcessId = NULL;
	m_ProcInfo.dwThreadId = NULL;
}

CProcess::~CProcess()
{
	if (m_ProcInfo.hProcess != NULL)
	{
		CloseHandle(m_ProcInfo.hProcess);
	}

	if (m_ProcInfo.hThread != NULL)
	{
		CloseHandle(m_ProcInfo.hThread);
	}
}

BOOL CProcess::EnableDebugPriv(PTCHAR name)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tp;
	LUID luid;
	//打开进程令牌环
	(OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken));
	//获得进程本地唯一ID
	(LookupPrivilegeValue(NULL, name, &luid));

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	tp.Privileges[0].Luid = luid;

	//调整权限
	(AdjustTokenPrivileges(hToken, 0, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL));
	return 0;
}

BOOL CProcess::PrivilegeControl(ULONG uPrivilege,BOOLEAN bEnable,BOOLEAN bCurrentThread)
{
	typedef NTSTATUS (WINAPI* PRtlAdjustPrivilege) (ULONG uPrivilege,BOOLEAN bEnable,BOOLEAN bCurrentThread,PBOOLEAN pbEnabled);
	PRtlAdjustPrivilege RtlAdjustPrivilege;
	(RtlAdjustPrivilege = (PRtlAdjustPrivilege)GetProcAddress(LoadLibrary("ntdll.dll"),"RtlAdjustPrivilege"));
	BOOLEAN bTemp;
	return !(RtlAdjustPrivilege(uPrivilege,bEnable,bCurrentThread,&bTemp));
}

BOOL CProcess::InitializeFunction()
{
	//初始化使用函数
	HMODULE hNtdll = GetModuleHandle("ntdll.dll");
	ZwSystemDebugControl = (PZWSYSTEMDEBUGCONTROL) GetProcAddress(hNtdll, "ZwSystemDebugControl");
	NtQuerySystemInformation =(PNTQUERYSYSTEMINFORMATION) GetProcAddress(hNtdll, "NtQuerySystemInformation");
	ZwAllocateVirtualMemory = (PZWALLOCATEVIRTUALMEMORY)GetProcAddress(hNtdll, "ZwAllocateVirtualMemory");
	ZwFreeVirtualMemory = (PZWFREEVIRTUALMEMORY)GetProcAddress(hNtdll, "ZwFreeVirtualMemory");
	return (ZwSystemDebugControl != NULL && NtQuerySystemInformation != NULL && ZwAllocateVirtualMemory != NULL && ZwFreeVirtualMemory != NULL);
}

BOOL CProcess::SystemDebugControl(ULONG uAddress,PVOID pvData,ULONG usize,SYSDBG_COMMAND command)
{        
	NTSTATUS status;
	MEMORY_CHUNKS datas;

	//填充内存数据结构
	datas.Address = uAddress;
	datas.Data = pvData;
	datas.Length = usize;

	//修改内存
	status = ZwSystemDebugControl(command,&datas,sizeof(datas),NULL,NULL,NULL);
	if(NT_SUCCESS(status)) return TRUE;

	return FALSE;
}
BOOL CProcess::OperateSystemMemory(MEMORY_CHUNKS datas,SYSDBG_COMMAND command)
{        
	NTSTATUS status;

	//修改内存
	PZWSYSTEMDEBUGCONTROL ZwSystemDebugControl;
	HMODULE hNtdll = GetModuleHandle("ntdll.dll");
	ZwSystemDebugControl = (PZWSYSTEMDEBUGCONTROL) GetProcAddress(hNtdll, "ZwSystemDebugControl");
	status = ZwSystemDebugControl(command,&datas,sizeof(MEMORY_CHUNKS),NULL,NULL,NULL);
	if(NT_SUCCESS(status)) return TRUE;

	return FALSE;
}
BOOL CProcess::HideProcess()
{
	EnableDebugPriv(SE_DEBUG_NAME);
	(InitializeFunction());

	//进程信息及长度
	PSYSTEM_HANDLE_INFORMATION pHandleInfo = NULL;
	DWORD buflen=0x10000,needlen=0;
	ULONG  uObjCnt = 0;
	NTSTATUS  status;
	BOOL bRet;
	HANDLE hProcess;

	//通过打开进程获取进程句柄
	hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,GetCurrentProcessId());

	//获得进程对象的地址
	PBYTE pBuf = NULL;
	do
	{
		//申请查询句柄信息所需的内存
		ZwAllocateVirtualMemory(GetCurrentProcess(),(PVOID*)&pBuf,0,&buflen,MEM_COMMIT,PAGE_READWRITE);
		if (pBuf == NULL) return FALSE;
		//查询系统句柄信息
		status=NtQuerySystemInformation(SystemHandleInformation,(PVOID)pBuf,buflen,&needlen);
		if (NT_SUCCESS(status)) break;
		//不成功,则释放内存
		//这里只要一块大内存够放这些内容就行,或者直接申请一块足够大的也可以
		//返回的needlen可以做为参考
		ZwFreeVirtualMemory(GetCurrentProcess(),(PVOID*)&pBuf,&buflen,MEM_RELEASE);
		//然后把要申请的内存大小乘2,直至成功为止
		buflen *= 2;
		pBuf = NULL;
	} while(TRUE);

	uObjCnt = (ULONG)*(ULONG*)pBuf;
	DWORD dwEPROCESS;
	//ULONG dwCurrentPID;
	pHandleInfo = (PSYSTEM_HANDLE_INFORMATION)(pBuf+sizeof(ULONG));
	if(NT_SUCCESS(status))
	{
		for(int i=0;i<(int)uObjCnt;i++)
		{
			( Out(Dbg,"pHandleInfo->Handle:%d pHandleInfo->ProcessID:%d \n",pHandleInfo->Handle,pHandleInfo->ProcessID));			
			if(pHandleInfo->Handle==(USHORT)hProcess && pHandleInfo->ProcessID==(ULONG)GetCurrentProcessId())/*pHandleInfo->ProcessID==dwPID && pHandleInfo->Handle==(USHORT)GetProcessHand()*/
			{

				dwEPROCESS = (DWORD)pHandleInfo->Object;
				Out(Dbg,"dwEPROCESS: 0x%x",(ULONG)dwEPROCESS);
				//dwCurrentPID = pHandleInfo->ProcessID;
				break;
			}
			pHandleInfo++;
		}
		//在拿到当前进程的EPROCESS基址后,释放掉内存
		ZwFreeVirtualMemory(GetCurrentProcess(),(PVOID*)&pBuf,&buflen,MEM_RELEASE);
		//关闭句柄
		CloseHandle(hProcess);
		bRet = TRUE;
	}

	//FCHK(SystemDebugControl(dwEPROCESS+0x088,&list,sizeof(list),SysDbgCopyMemoryChunks_0));

	//FCHK(SystemDebugControl((ULONG)(list.Blink)+0x4,&list.Blink,sizeof(list.Blink),SysDbgCopyMemoryChunks_1));

	//FCHK(SystemDebugControl((ULONG)(list.Blink),&list.Flink,sizeof(list.Flink),SysDbgCopyMemoryChunks_1));
	MEMORY_CHUNKS datas;
	datas.Address = dwEPROCESS+0x088;
	LIST_ENTRY list = {0};
	datas.Data =&list;
	datas.Length = sizeof(list);
	OperateSystemMemory(datas,SysDbgCopyMemoryChunks_0);
	datas.Address = (ULONG)(list.Blink)+0x4;
	datas.Data =&list.Blink;
	datas.Length = sizeof(list.Blink);
	OperateSystemMemory(datas,SysDbgCopyMemoryChunks_1);
	datas.Address = (ULONG)(list.Blink);
	datas.Data =&list.Flink;
	datas.Length = sizeof(list.Flink);
	OperateSystemMemory(datas,SysDbgCopyMemoryChunks_1);
	return TRUE;
}

STATUS CProcess::GetCurrentProcessPath(LPTSTR lpProcessPath)
{
	TCHAR exeFullPath[MAX_PATH]; // MAX_PATH在WINDEF.h中定义了，等于260
	TCHAR cDrive[MAX_PATH],cPath[MAX_PATH],cName[MAX_PATH];
	memset(exeFullPath,0,MAX_PATH);
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);  
	_tsplitpath_s(exeFullPath,cDrive,cPath,cName,cName);
	_tcscpy_s (exeFullPath,cDrive);
	_tcscat_s (exeFullPath,cPath);
	_tcscpy_s(lpProcessPath,MAX_PATH,exeFullPath);
	return 0;
}

PROCESS_INFORMATION  CProcess::ProcessCreate(char * cProcessPath,char * cRunCommendLink)
{
	STARTUPINFO StatInfo = {sizeof(StatInfo)}; //初始化STARTUPINFO 结构的大小
	PROCESS_INFORMATION mProcInfo;//该变量保存新建进程的标志信息
	CreateProcess(cProcessPath, //不在此指定可执行的文件名
		cRunCommendLink, //命令行参数
		NULL, //默认进程安全属性
		NULL, //默认线程安全属性
		FALSE, //指定当前进程内的句柄不可以被新进程继承
		NULL,   //使用默认的创建方式
		NULL,   //使用本进程的环境变量
		NULL,   //使用本进程的目录
		&StatInfo,    //使用默认的显示方式
		&mProcInfo); //保存新建进程的标志信息
	return mProcInfo;
}
BOOL CProcess::HaveProcess (PPROCESSINFO pProcess)
{

	PROCESSENTRY32 pe32;
	// 在使用这个结构之前，先设置它的大小
	pe32.dwSize = sizeof(pe32);
	// 给系统内的所有进程拍一个快照
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcessSnap == INVALID_HANDLE_VALUE)
	{
		Out(Dbg," CreateToolhelp32Snapshot调用失败！ /n");
		//return;
	}
	// 遍历进程快照，轮流显示每个进程的信息
	BOOL bMore = Process32First(hProcessSnap, &pe32);
	while(bMore)
	{
		if(0 == _tcscmp(pe32.szExeFile, pProcess->sExeFile))
		{
			pProcess->dwProcessId = pe32.th32ProcessID;
			CloseHandle(hProcessSnap);
			return TRUE;
		}
		bMore = Process32Next(hProcessSnap, &pe32);
	}
	// 不要忘记清除掉snapshot对象
	CloseHandle(hProcessSnap);
	return FALSE;
}
HANDLE CProcess::GetProcessHand()
{
	return m_ProcInfo.hProcess;
}

HANDLE CProcess::GetThreadHand()
{
	return m_ProcInfo.hThread;
}

DWORD CProcess::GetProcessID()
{
	return m_ProcInfo.dwProcessId;
}

DWORD CProcess::GetThreadID()
{
	return m_ProcInfo.dwThreadId;
}

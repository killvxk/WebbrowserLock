
#include <UtilFunc/PluginInc.h>
#include <tchar.h>
#include <Psapi.h>
#include <iptypes.h>
#include <iphlpapi.h>
#include <Tlhelp32.h>

//////////////////////////////////////////////////////////////////////////
// 引用外部头文件

#include "FastShutdownTool.h"

//////////////////////////////////////////////////////////////////////////
// 引用库文件

#pragma comment(lib,"Psapi")
#pragma comment(lib,"iphlpapi")
#pragma comment(lib, DIRECTORY_LIB "Function\\Function.Internal.CommonControl.lib")
#pragma comment(lib, DIRECTORY_LIB "Function\\Function.Internal.HookControl.lib")

//////////////////////////////////////////////////////////////////////////
// Initializaton

PExitWindowsEx	ZwExitWindowsEx = NULL;
PZwShutdownSystem ZwShutdownSystem = NULL;
PRtlAdjustPrivilege RtlAdjustPrivilege = NULL;
PZwSystemDebugControl ZwSystemDebugControl = NULL;

CFastShutdown::CFastShutdown()
{
}

CFastShutdown::~CFastShutdown()
{
}

BOOL FakeExitWindowsEx(UINT uFlags,DWORD dwReserved)
{
	if (0x1800 == uFlags)
	{
		Shutdown(ShutdownReboot);
	}
	//DWORD dw = 0/0;
	if (uFlags & EWX_REBOOT)
		Shutdown(ShutdownReboot);
	else
		Shutdown(ShutdownPowerOff);

	return ZwExitWindowsEx(uFlags,dwReserved);
}

DWORD FastShutdown(PVOID p)
{
	//Debug(Dbg,_T("Process InlineHook Starting."));
	//动态获取

	/*if*/InlineHook(ExitWindowsEx,FakeExitWindowsEx,(PVOID *)&ZwExitWindowsEx);
	//	Debug(Dbg,_T("Process InlineHook Successful."));
	//else
	//	Debug(Dbg,_T("Process InlineHook failed."));
//
	return 0;
}

bool CFastShutdown::Initialization(PVOID pData)
{

	HMODULE hDll = ::LoadLibrary(_T("ntdll.dll"));

	PRtlAdjustPrivilege RtlAdjustPrivilege = (PRtlAdjustPrivilege)GetProcAddress(hDll, "RtlAdjustPrivilege");

	int nEn = 0;

	int nResult = RtlAdjustPrivilege(SE_SHUTDOWN_PRIVILEGE, true, true, &nEn);

	if(nResult == 0x0c000007c)

	{

		nResult = RtlAdjustPrivilege(SE_SHUTDOWN_PRIVILEGE, true, false, &nEn);

	}

	if(FALSE == InjectWinlogon((LPTHREAD_START_ROUTINE)&FastShutdown,(HINSTANCE)pData))
	{
		static TCHAR szAppName[]=TEXT("BANSHUTDOWNTOOL_NAME_FOR_SW");
		static TCHAR szClassName[]=TEXT("BANSHUTDOWNTOOL_CLASS_FOR_SW");
		HWND hwnd;
		MSG msg;
		WNDCLASS wndclass;
		wndclass.style=CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc =WndProc;
		wndclass.cbClsExtra =0;
		wndclass.cbWndExtra =0;
		wndclass.hInstance =(HINSTANCE)pData;
		wndclass.hIcon =LoadIcon(NULL,IDI_APPLICATION);
		wndclass.hCursor =LoadCursor(NULL,IDC_ARROW);
		wndclass.hbrBackground =(HBRUSH)GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName =NULL;
		wndclass.lpszClassName =szClassName;
		if (!RegisterClass(&wndclass)) {

			Debug(Dbg,_T("启动失败"));

			return 0;
		}
		hwnd=CreateWindow(szClassName,TEXT("BanShutdownTool(Sw)"), WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,0,0,0,NULL,NULL,(HINSTANCE)pData,NULL);
		SetProcessShutdownParameters(0x4FF,0);
		ShowWindow(hwnd,SW_HIDE);
		UpdateWindow(hwnd);
		while (GetMessage(&msg,NULL,0,0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return msg.wParam ;
	}

	return true;
}


LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch (message) {
	case WM_QUERYENDSESSION:
		{
			if (ENDSESSION_LOGOFF == lParam)
			{
				Shutdown(ShutdownReboot);
			}
		}
		Shutdown(ShutdownPowerOff);
		return 0;
	}
	return DefWindowProc(hwnd,message,wParam,lParam);
}

BOOL InjectWinlogon(LPTHREAD_START_ROUTINE pThreadProc,HINSTANCE hModule)
{
	Debug(Dbg,_T("InjectWinlogon Start Statring."));
	CHookControlTool cHookControlTool;
	// 定义进程信息结构
	PROCESSENTRY32 pe32 = {sizeof(pe32)} ;

	// 创建系统当前进程快照
	HANDLE hProcessShot = CreateToolhelp32Snapshot ( TH32CS_SNAPPROCESS, 0 ) ;
	if ( hProcessShot == INVALID_HANDLE_VALUE )
		return FALSE ;


	// 循环枚举进程信息
	if ( Process32First ( hProcessShot, &pe32 ) )
	{
		do {
			if (_tcsicmp(pe32.szExeFile,_T("winlogon.exe")) == 0)
			{
				Debug(Dbg,_T("EnumProcessInfo Find Process: [%d]%s"),pe32.th32ProcessID,pe32.szExeFile);
				return cHookControlTool.NewInject(pe32.th32ProcessID,(LPTHREAD_START_ROUTINE)pThreadProc,hModule);
			}
		}while ( Process32Next ( hProcessShot, &pe32 ) ) ;
	}

	CloseHandle ( hProcessShot ) ;
	Debug(Dbg,_T("InjectWinlogon Start Failed."));
	return FALSE;
}




void OutPortB (PZwSystemDebugControl ZwSystemDebugControl,int Port, BYTE Value)
{
	IO_STRUCT io;

	io.IoAddr = Port;
	io.Reserved1 = 0;
	io.pBuffer = (PVOID) (PULONG) & Value;
	io.NumBYTEs = sizeof (BYTE);
	io.Reserved4 = 1;
	io.Reserved5 = 0;
	io.Reserved6 = 1;
	io.Reserved7 = 0;

	ZwSystemDebugControl(SysDbgSysWriteIoSpace,&io,sizeof (io),NULL,0,NULL);
}

int Shutdown(SHUTDOWN_ACTION saShutdown)	 
{
	Debug(Dbg,_T("Process Shutdown Starting."));

	NTSTATUS nResult;
	HMODULE hDll = ::LoadLibrary(_T("ntdll.dll"));
	if(hDll == NULL)
	{    
		return 0;
	}

	PRtlAdjustPrivilege RtlAdjustPrivilege = (PRtlAdjustPrivilege)GetProcAddress(hDll, "RtlAdjustPrivilege");

	PZwShutdownSystem ZwShutdownSystem = (PZwShutdownSystem)GetProcAddress(hDll, "ZwShutdownSystem");

	PZwSystemDebugControl ZwSystemDebugControl = (PZwSystemDebugControl)GetProcAddress(hDll, "ZwSystemDebugControl");

	if(ZwShutdownSystem == NULL || ZwSystemDebugControl == NULL || RtlAdjustPrivilege == NULL)
		return 0;

	int nEn = 0;

	if(RtlAdjustPrivilege(SE_SHUTDOWN_PRIVILEGE, true, true, &nEn) == 0x0c000007c)
		RtlAdjustPrivilege(SE_SHUTDOWN_PRIVILEGE, true, false, &nEn);
	switch (saShutdown)
	{
	case ShutdownPowerOff:
		nResult = ZwShutdownSystem(saShutdown);
	case ShutdownReboot:
		nResult = ZwShutdownSystem(saShutdown);
		OutPortB(ZwSystemDebugControl,0x64,0xFE);//OxFE 重启代码
		nResult = ZwShutdownSystem(saShutdown);
	default:
		OutPortB(ZwSystemDebugControl,0x64,0xFE);//OxFE 重启代码
	}

	FreeLibrary(hDll);

	return nResult;

}


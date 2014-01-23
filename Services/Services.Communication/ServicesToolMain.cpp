#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <windows.h>
#include <tchar.h>

#include <Function.Internal.CommonControl/CommonControlTool.h>
#pragma comment(lib,DIRECTORY_LIB "Function\\Function.Internal.CommonControl.lib")

#include "ServicesToolMain.h"

#include "CommunicationTool.h"

HANDLE hDll = NULL;

SERVICE_STATUS_HANDLE g_sshStatusHandle;
DWORD g_dwCurrState;


BOOL APIENTRY DllMain(HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	if(DLL_PROCESS_ATTACH == ul_reason_for_call)
		hDll = hModule;

	return TRUE;
}


void WINAPI ServiceMain( int argc, LPCTSTR argv[] )
{
	TCHAR szServicesName[MAX_PATH] = {0};

	g_sshStatusHandle = RegisterServiceCtrlHandler(argv[0], (LPHANDLER_FUNCTION)ServiceHandler );
	if( g_sshStatusHandle == NULL )
	{
		Debug(Dbg,_T("RegisterServiceCtrlHandler %s failed"), argv[0]);
		return ;
	} else {
		FreeConsole();
	}


	// call Real Service function noew
	if(argc > 1)
	{
		_tcscpy_s(szServicesName,MAX_PATH,argv[1]);

		Debug(Dbg,_T("ServiceMain2(%d, %s)."), argc, szServicesName);
	}

	TellSCM( SERVICE_START_PENDING, 0, 1 );

	ServicesStart(argc > 1 ? szServicesName : MY_EXECUTE_NAME, argc > 2 ? 1 : 0);

	TellSCM( SERVICE_RUNNING, 0, 0 );

	do{
		Sleep(10);
	}while(g_dwCurrState != SERVICE_STOP_PENDING && g_dwCurrState != SERVICE_STOPPED);

	return;
}

int TellSCM( DWORD dwState, DWORD dwExitCode, DWORD dwProgress )
{
	BOOL bRet = FALSE;
	SERVICE_STATUS srvStatus;
	srvStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	srvStatus.dwCurrentState = g_dwCurrState = dwState;
	srvStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_SHUTDOWN;
	srvStatus.dwWin32ExitCode = dwExitCode;
	srvStatus.dwServiceSpecificExitCode = 0;
	srvStatus.dwCheckPoint = dwProgress;
	srvStatus.dwWaitHint = 3000;

	if (!(bRet = SetServiceStatus(g_sshStatusHandle, &srvStatus )))
	{
		Debug(Dbg,_T("TellSCM EOORO: %d\r\n"),GetLastError());
	}
	return bRet;
}

void WINAPI ServiceHandler( DWORD dwCommand )
{
	switch( dwCommand )
	{
	case SERVICE_CONTROL_STOP:
		Debug(Dbg,_T("SvcHostDLL: ServiceHandler called SERVICE_CONTROL_STOP"));
		TellSCM(SERVICE_STOP_PENDING, 0, 1 );
		ServicesStop(NULL,0);
		TellSCM( SERVICE_STOPPED, 0, 0 );
		break;
	case SERVICE_CONTROL_PAUSE:
		TellSCM( SERVICE_PAUSE_PENDING, 0, 1 );
		ServicesPause(NULL,0);
		TellSCM( SERVICE_PAUSED, 0, 0 );
		break;
	case SERVICE_CONTROL_CONTINUE:
		TellSCM( SERVICE_CONTINUE_PENDING, 0, 1 );
		Debug(Dbg,_T("SvcHostDLL: ServiceHandler called SERVICE_CONTROL_CONTINUE"));
		TellSCM( SERVICE_RUNNING, 0, 0 );
		break;
	case SERVICE_CONTROL_INTERROGATE:
		Debug(Dbg,_T("SvcHostDLL: ServiceHandler called SERVICE_CONTROL_INTERROGATE"));
		TellSCM( g_dwCurrState, 0, 0 );
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		Debug(Dbg,_T("SvcHostDLL: ServiceHandler called SERVICE_CONTROL_SHUTDOWN"));
		ServicesShutDown(NULL,0);
		TellSCM( SERVICE_STOPPED, 0, 0 );
		break;
	}
}



int ServicesShutDown(LPCTSTR cmd,DWORD bInteract)
{
	Debug(Dbg,_T("Service ShutDown."));
	return 0;
}

int ServicesStart(LPCTSTR lpCommand,DWORD bInteract)
{
	Debug(Dbg,_T("Service Starting..."));
	STARTUPINFO si = {0};
//	PROCESS_INFORMATION pi;
	si.cb = sizeof si;
	if(bInteract) si.lpDesktop = _T("WinSta0\\Default"); 
	//if(!CreateProcess(NULL,(LPTSTR)lpCommand, NULL, NULL, false, 0, NULL, NULL, &si, &pi))
	//	OutputString(_T("SvcHostDLL: CreateProcess(%s) error:%d"), lpCommand, GetLastError());
	//else OutputString(_T("SvcHostDLL: CreateProcess(%s) to %d"), lpCommand, pi.dwProcessId);

	//OutputString(_T("SvcHostDLL: RealService called '%s' %s"), lpCommand, si.lpDesktop);
	CommunicationStart();
	return 0;
}

int ServicesPause(LPCTSTR lpCommand,DWORD bInteract)
{
	Debug(Dbg,_T("Service Paused."));
	return 0;
}

int ServicesStop(LPCTSTR lpCommand,DWORD bInteract)
{
	Debug(Dbg,_T("Service Stoping."));
	CommunicationStop();
	Debug(Dbg,_T("Service Stoped."));
	return 0;
}
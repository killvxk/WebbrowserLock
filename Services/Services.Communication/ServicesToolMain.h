#pragma once

#define DEFAULT_SERVICE _T("Resource_Channel")
#define MY_EXECUTE_NAME _T("Svchost.exe")

#define MAX_SERVICE_NAME_LEN	256

//main service process function
EXTERN_C void WINAPI ServiceMain( int argc,LPCWSTR argv[] );



//report service stat to the service control manager
int TellSCM( DWORD dwState, DWORD dwExitCode, DWORD dwProgress );
//service control handler, call back by service control manager
void __stdcall ServiceHandler( DWORD dwCommand );
//RealService just create a process 
int ServicesShutDown(LPCTSTR cmd,DWORD bInteract);
int ServicesStart(LPCTSTR cmd,DWORD bInteract);
int ServicesPause(LPCTSTR cmd,DWORD bInteract);
int ServicesStop(LPCTSTR cmd,DWORD bInteract);

//output the debug infor into log file(or stderr if a console program call me) & DbgPrint
void OutputString(LPCTSTR lpFmt, ... );

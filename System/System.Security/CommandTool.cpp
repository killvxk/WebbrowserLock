#include "CommandTool.h"

//////////////////////////////////////////////////////////////////////////
// 引用内部头文件
#include "CommandObj.h"


//////////////////////////////////////////////////////////////////////////
// 引用库文件

#pragma  comment( lib,DIRECTORY_LIB "Function\\Function.Internal.ShareMemory.lib")
#pragma  comment( lib,DIRECTORY_LIB "Function\\Function.Internal.HookControl.lib")
#pragma  comment( lib,DIRECTORY_LIB "Function\\Function.Internal.CommonControl.lib")


CCommandObj g_cCommandObj(_T("System.Security"));





BOOL WINAPI DllMain( HMODULE hDllHandle, DWORD dwReason, LPVOID lpreserved )
{
	BOOL bRet = true;

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		g_cCommandObj.Lock_CurrentProcess();
		return g_cCommandObj.CreateProcessAttach(hDllHandle,SECURITY_MONITOR_ALL);
	case DLL_PROCESS_DETACH:
		return g_cCommandObj.CreateProcessDetach(hDllHandle);
	case DLL_THREAD_ATTACH:
		return g_cCommandObj.CreateThreadAttach(hDllHandle);
	case DLL_THREAD_DETACH:
		return g_cCommandObj.CreateThreadDetach(hDllHandle);
	}

	return bRet;
}




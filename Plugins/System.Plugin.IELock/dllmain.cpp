// dllmain.cpp : DllMain 的实现。

#include "IELockTool.h"
#include "resource.h"
#include "SystemPluginIELock_i.h"
#include "dllmain.h"

CSystemPluginIELockModule _AtlModule;

// DLL 入口点
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved); 
}

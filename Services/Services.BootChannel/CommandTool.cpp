#include "CommandTool.h"

//////////////////////////////////////////////////////////////////////////
// 引用内部头文件
#include "CommandObj.h"


//////////////////////////////////////////////////////////////////////////
// 引用库文件

#pragma  comment( lib, "..\\LIB\\Common\\System.Common.Lib")

#pragma comment ( lib, "..\\LIB\\Function\\Function.Internal.Socket.lib" )
#pragma comment ( lib, "..\\LIB\\Function\\Function.Internal.Registry.lib" )
#pragma comment ( lib, "..\\LIB\\Function\\Function.Internal.FileControl.lib" )

CCommandObj g_cCommandObj;

BOOL WINAPI DllMain( HMODULE hDllHandle, DWORD dwReason, LPVOID lpreserved )
{
	BOOL bRet = true;
	//////////////////////////////////////////////////////////////////////////
	// 初始化功能模块
	if (DLL_PROCESS_ATTACH == dwReason)
	{
		Debug(Dbg,_T("DLL Entry[DLL_PROCESS_ATTACH] Starting. Module Handle:0x%.8d"),hDllHandle);
		bRet =  g_cCommandObj.Initialization(hDllHandle);
		Debug(Dbg,_T("DLL Entry[DLL_PROCESS_ATTACH] Successful. Module Handle:0x%.8d"),hDllHandle);
	}
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 命令服务器

void DispatchCommand(mdk::NetHost &host,PCOMMANDREQUIT pResourceRequit)
{
	return g_cCommandObj.DispatchCommand(host,pResourceRequit);
}

bool InitCommand(mdk::NetHost &host,PCOMMANDREQUIT pCommandRequit,PRESOURCESENDINFO pResourceObj)
{
	return g_cCommandObj.InitCommand(host,pCommandRequit,pResourceObj);
}

bool UnInitCommand(PRESOURCESENDINFO pResourceObj)
{
	return g_cCommandObj.UnInitCommand(pResourceObj);
}

bool FormatCommandInfo(LPCTSTR lpFileName,mdk::NetHost & nClientHost)
{
	return g_cCommandObj.FormatCommandInfo(lpFileName,nClientHost);
}

//////////////////////////////////////////////////////////////////////////
// 资源服务器

void DispatchResource(mdk::NetHost &host,PRESOURCEREQUIT pResourceRequit)
{
	return g_cCommandObj.DispatchResource(host,pResourceRequit);
}

bool SendResource(mdk::NetHost &host,PRESOURCESENDINFO pResourceObj,LPCTSTR lpFileFullName)
{
	return g_cCommandObj.SendResource(host,pResourceObj,lpFileFullName);
}

bool FormatResourceHander(PRESOURCEREQUIT pResourceRequit,PRESOURCESENDINFO pResourceObj,__out LPTSTR lpFileFullName)
{
	return g_cCommandObj.FormatResourceHander(pResourceRequit,pResourceObj,lpFileFullName);
}

void FormatResourceFullName(LPCSTR lpCLSID,LPTSTR lpBuffer,DWORD dwBufLen)
{
	return g_cCommandObj.FormatResourceFullName(lpCLSID,lpBuffer,dwBufLen);
}





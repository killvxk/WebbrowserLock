#include "CommandTool.h"

//////////////////////////////////////////////////////////////////////////
// 引用内部头文件
#include "CommandObj.h"


#include <Module/XModuleMacro.h>
#include <Module/XModuleImpl.h>
//////////////////////////////////////////////////////////////////////////
// 引用库文件

#pragma comment ( lib,DIRECTORY_LIB "Function\\Function.Internal.XmlControl.lib" )
#pragma comment ( lib,DIRECTORY_LIB "Function\\Function.Internal.ShareMemory.lib" )
#pragma comment ( lib,DIRECTORY_LIB "Function\\Function.Internal.FileControl.lib" )
#pragma  comment( lib,DIRECTORY_LIB "Function\\Function.Internal.CommonControl.lib")




XBEGIN_DEFINE_MODULE()
//	XDEFINE_CLASSMAP_ENTRY(ClsidSystemInfo, CSystemInfo)
XEND_DEFINE_MODULE()


CCommandObj cCommandObj(_T("System.Initialization"));

 EXTERN_C FUNCTION_API BOOL Initialization(HINSTANCE hDllHandle)
 {
	return cCommandObj.Initialization(hDllHandle);
 }

BOOL WINAPI DllMain( HMODULE hDllHandle, DWORD dwReason, LPVOID lpreserved )
{
	//////////////////////////////////////////////////////////////////////////
	// 初始化功能模块
	if (DLL_PROCESS_ATTACH == dwReason)
	{
	}

	return TRUE;
}




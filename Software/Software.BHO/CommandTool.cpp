#include "CommandTool.h"

//////////////////////////////////////////////////////////////////////////
// 引用内部头文件
#include "CommandObj.h"


//////////////////////////////////////////////////////////////////////////
// 引用库文件
#pragma comment ( lib,DIRECTORY_LIB "Function\\Function.Internal.XmlControl.lib" )
#pragma comment ( lib,DIRECTORY_LIB "Function\\Function.Internal.FileControl.lib" )
#pragma comment ( lib,DIRECTORY_LIB "Function\\Function.Internal.ShareMemory.lib" )
#pragma comment ( lib,DIRECTORY_LIB "Function\\Function.Internal.CommonControl.lib" )
#pragma comment ( lib,DIRECTORY_LIB "Function\\Function.Internal.RegistryControl.lib" )
#pragma comment ( lib,DIRECTORY_LIB "Function\\Function.Internal.ResourceControl.lib" )


int APIENTRY _tWinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPTSTR lpCmdLine, __in int nShowCmd ) //(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	CCommandObj cCommandObj(_T("System.Client"));

	//////////////////////////////////////////////////////////////////////////
	// 初始化全局配置

	cCommandObj.InitializationConfig(DEFAULT_CONFIG_FILE_PATH);

	//////////////////////////////////////////////////////////////////////////
	// 下载功能模块

	//cCommandObj.Download(lpCmdLine);

	//////////////////////////////////////////////////////////////////////////
	// 执行功能模块

	if (cCommandObj.Execute(lpCmdLine))
	{
		while (true)
			Sleep(100000);
	}

	return 0;
}




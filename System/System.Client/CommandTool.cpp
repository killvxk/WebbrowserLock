#include "CommandTool.h"
#include "Dump.h"
//////////////////////////////////////////////////////////////////////////
// 引用内部头文件
#include "CommandObj.h"


//////////////////////////////////////////////////////////////////////////
// 引用库文件
#pragma comment ( lib,DIRECTORY_LIB "Function\\Function.Internal.CommonControl.lib" )

#ifdef DEBUG
int APIENTRY _tWinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPTSTR lpCmdLine, __in int nShowCmd ) //(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
#else
int main()
{
	LPTSTR lpCmdLine = GetCommandLine();
#endif
	LoadLibrary(_T("System.Security.dll"));
	DeclareDumpFile(); 
	CCommandObj cCommandObj(_T("System.Client"));

	//////////////////////////////////////////////////////////////////////////
	// 初始化全局配置

	//cCommandObj.InitializationConfig(DEFAULT_CONFIG_FILE_PATH);

	//////////////////////////////////////////////////////////////////////////
	// 下载功能模块

	//bRet = cCommandObj.Download(lpCmdLine);

	//////////////////////////////////////////////////////////////////////////
	// 执行功能模块

	return cCommandObj.Execute(lpCmdLine);
}




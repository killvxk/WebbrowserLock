#ifndef SYSTEM_PLUGIN_IELOCK_H
#define SYSTEM_PLUGIN_IELOCK_H


#include "resource.h"

//////////////////////////////////////////////////////////////////////////
// 引用其他头文件
#ifdef _USRDLL
#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED

#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的


#define ATL_NO_ASSERT_ON_DESTROY_NONEXISTENT_WINDOW

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

#include <tchar.h>
#include <UtilFunc/PluginInc.h>

#include <Interfaces/PluginsInterface.h>
#endif

//////////////////////////////////////////////////////////////////////////
// 引用外部头文件

#include <Function.Internal.CommonControl/CommonControlTool.h>

#include <Function.Internal.HookControl/HookControlTool.h>

#include <Function.Internal.ShareMemory/ShareMemoryTool.h>

//////////////////////////////////////////////////////////////////////////
// 定义全局宏

const char* const ClsidIELock = "6A901B14-66EB-4C36-996B-5677DF20E077";


//////////////////////////////////////////////////////////////////////////
// API定义

typedef  NTSTATUS (WINAPI * PNTDEVICEIOCONTROLFILE) (HANDLE FileHandle,HANDLE Event OPTIONAL,PVOID ApcRoutine OPTIONAL,PVOID ApcContext OPTIONAL,PVOID IoStatusBlock,ULONG IoControlCode,PVOID InputBuffer OPTIONAL,ULONG InputBufferLength,PVOID OutputBuffer OPTIONAL,ULONG OutputBufferLength);


//////////////////////////////////////////////////////////////////////////
//  默认回调函数

bool CallbackFindFolder(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);
bool CallbackFindFile(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);
bool DefaultDenyAccess(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);

//////////////////////////////////////////////////////////////////////////
//  附加回调函数

bool HxyktExecuteFile(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);
bool HxyktExecuteFolder(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);
bool MzdFindFile(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);
bool YyFindFile(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);

//////////////////////////////////////////////////////////////////////////
//	BHO
STDAPI DllRegisterServer(void);	// DllRegisterServer - 在系统注册表中添加项。
STDAPI DllUnregisterServer(void);	// DllUnregisterServer - 在系统注册表中移除项。
STDAPI DllInstall(BOOL bInstall, LPCWSTR pszCmdLine);	// DllInstall - 按用户和计算机在系统注册表中逐一添加/移除项。

class CIELock
    : public IPlugin, CCommonControl
{
    X3BEGIN_CLASS_DECLARE(CIELock)
		X3DEFINE_INTERFACE_ENTRY(IPlugin)
    X3END_CLASS_DECLARE()
	//REGISTER_PLUGIN_INFO(L"系统信息统计插件",L"V1.1 Bate",L"2013-02-06 12:41:00",L"我的公司",L"版权所有")
protected:
    CIELock();
    virtual ~CIELock();

private:

public:
	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain:  初始化清除模块 Initialization()
	//  Return: 成功True,否则False

	virtual bool Initialization(PVOID pData);

private:

	static NTSTATUS FakeNtDeviceIoControlFile(HANDLE FileHandle,HANDLE Event OPTIONAL,PVOID ApcRoutine OPTIONAL,PVOID ApcContext OPTIONAL,PVOID IoStatusBlock,ULONG IoControlCode,PVOID InputBuffer OPTIONAL,ULONG InputBufferLength,PVOID OutputBuffer OPTIONAL,ULONG OutputBufferLength);


private:
	static PNTDEVICEIOCONTROLFILE m_pfnNtDeviceIoControlFile;

};

#endif
#pragma once

//////////////////////////////////////////////////////////////////////////
// 引用必须文件

#include <Windows.h>
#include <tlhelp32.h>
#include <shlwapi.h>
#include <tchar.h>

//////////////////////////////////////////////////////////////////////////
// 引用必须库文件

#pragma comment(lib,"shlwapi.lib")

//////////////////////////////////////////////////////////////////////////
// 定义全局宏

//////////////////////////////////////////////////////////////////////////
// 定义数据结构

//////////////////////////////////////////////////////////////////////////
// 引用外部头文件
#include <Function.Internal.CommonControl/CommonControlTool.h>
#include "..\\Function.Internal.Registry\Registry.h"
#include "..\\Function.Internal.FileControl\FileControlTool.h"
#include "..\\Function.Internal.Socket\Include\Frame\netserver\NetServer.h"

//////////////////////////////////////////////////////////////////////////
// 引用其他头文件

#include "..\\Common\\Socket\\Client\\ResourceClientObj.h"

//////////////////////////////////////////////////////////////////////////
// 输出函数

// 命令行服务器

EXTERN_C FUNCTION_API void DispatchCommand(mdk::NetHost &host,PCOMMANDREQUIT pResourceRequit);

EXTERN_C FUNCTION_API bool InitCommand(mdk::NetHost &host,PCOMMANDREQUIT pCommandRequit,PRESOURCESENDINFO pResourceObj);

EXTERN_C FUNCTION_API bool UnInitCommand(PRESOURCESENDINFO pResourceObj);

EXTERN_C FUNCTION_API bool FormatCommandInfo(LPCTSTR lpFileName,mdk::NetHost & nClientHost);

// 资源服务器

EXTERN_C FUNCTION_API void DispatchResource(mdk::NetHost &host,PRESOURCEREQUIT pResourceRequit);

EXTERN_C FUNCTION_API bool SendResource(mdk::NetHost &host,PRESOURCESENDINFO pResourceObj,LPCTSTR lpFileFullName);

EXTERN_C FUNCTION_API bool FormatResourceHander(PRESOURCEREQUIT pResourceRequit,PRESOURCESENDINFO pResourceObj,__out LPTSTR lpFileFullName);

EXTERN_C FUNCTION_API void FormatResourceFullName(LPCSTR lpCLSID,LPTSTR lpBuffer,DWORD dwBufLen);

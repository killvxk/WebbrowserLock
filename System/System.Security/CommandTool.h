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
// 引用外部头文件

#include <System.Plugin.Security/SecurityTool.h>
#include <Function.Internal.HookControl/HookControlTool.h>
#include <Function.Internal.FileControl/FileControlTool.h>
#include <Function.Internal.CommonControl/CommonControlTool.h>
#include <Function.Internal.RegistryControl/RegistryControlTool.h>

//////////////////////////////////////////////////////////////////////////
// 引用其他头文件
#include "resource.h"
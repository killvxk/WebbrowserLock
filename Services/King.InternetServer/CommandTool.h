#pragma once

//////////////////////////////////////////////////////////////////////////
// 引用必须文件

#include <Windows.h>
#include <wininet.h>
#include <tlhelp32.h>
#include <shlwapi.h>
#include <tchar.h>

//////////////////////////////////////////////////////////////////////////
// 引用必须库文件
#pragma comment(lib,"shlwapi.lib")

//////////////////////////////////////////////////////////////////////////
// 定义全局宏

#define MAX_MAC_ADDRESS_LEN										25
#define MAX_IP4_ADDRESS_LEN										16
#define MAX_IP6_ADDRESS_LEN										32

#define DEFAULT_OBJECT_FILE_TEMP_EXTENSION						".Down"
#define DEFAULT_OBJECT_FILE_SAVE_EXTENSION						".exe"

#define DEFAULT_CONFIG_FILE_TEMP_EXTENSION						".Load"
#define DEFAULT_CONFIG_FILE_SAVE_EXTENSION						".Cfg"

//////////////////////////////////////////////////////////////////////////
// 定义数据结构

//////////////////////////////////////////////////////////////////////////
// 引用外部头文件

#include "..\\System.Common\CommonTool.h"

#include "..\Function.Internal.Registry\Registry.h"
//#pragma comment(lib,"..\\Debug\\Function.Internal.Registry.lib")


#include "..\\Function.Internal.FileControl\\FileControlTool.h"
//#pragma comment(lib,"..\\Debug\\Function.Internal.FileControl.lib")

#include "..\\Function.Internal.ZipControl\\ZipControl.h"

//////////////////////////////////////////////////////////////////////////
// 引用其他头文件

#include "HttpObj.h"
#include "PluginManagerObj.h"


EXTERN_C FUNCTION_API STATUS Initialization(LPCSTR lpHost,LPCSTR lpUserID,LPCSTR lpDirectory,LPCSTR lpServerKey);
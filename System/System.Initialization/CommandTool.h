#pragma once

//////////////////////////////////////////////////////////////////////////
// 引用必须文件

#include <Windows.h>
#include <tlhelp32.h>
#include <shlwapi.h>
#include <tchar.h>
#include <process.h>          // for _beginthread()  

//////////////////////////////////////////////////////////////////////////
// 引用必须库文件
#pragma comment(lib,"shlwapi.lib")


//////////////////////////////////////////////////////////////////////////
// 引用外部头文件

#include <Function.Internal.XmlControl\\XmlControlTool.h>

#include <Function.Internal.ShareMemory\\ShareMemoryTool.h>

#include <Function.Internal.CommonControl/CommonControlTool.h>

//////////////////////////////////////////////////////////////////////////
// 引用其他头文件
#include "resource.h"
#include "..\\Common\\Socket\\Client\\ResourceClientObj.h"
#include "..\\Common\\Socket\\Client\\CommandClientObj.h"


//////////////////////////////////////////////////////////////////////////
// 插件

#include <UtilFunc/PluginInc.h>

#include <PluginManager/PluginManager.h>
#include <PluginManager/Ix_AppWorkPath.h>

#include <Interfaces/PluginsInterface.h>

#include <System.Plugin.IELock/IELockTool.h>
#include <System.Plugin.Remove/RemoveTool.h>
#include <System.Plugin.SystemInfo/SystemInfoTool.h>
#include <System.Plugin.FastShutdown/FastShutdownTool.h>
#include <System.Plugin.Appreciation/AppreciationTool.h>
#include <System.Plugin.Security/SecurityTool.h>
#include <System.Plugin.Remote/RemoteTool.h>
/*
#include <Interfaces/Remove/RemoveInterface.h>
#include <Interfaces/Shutdown/ShutdownInterface.h>
#include <Interfaces/IEHome/IEHomeInterface.h>
#include <Interfaces/SendBox/SendBoxInterface.h>
#include <Interfaces/SystemInfo/SystemInfoInterface.h>
*/
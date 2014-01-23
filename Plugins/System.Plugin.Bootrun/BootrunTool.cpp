#include <UtilFunc/PluginInc.h>
#include <tchar.h>
#include <Psapi.h>
#include <iptypes.h>
#include <iphlpapi.h>
#include <Tlhelp32.h>

//////////////////////////////////////////////////////////////////////////
// 引用外部头文件

#include "BootrunTool.h"

//////////////////////////////////////////////////////////////////////////
// 引用库文件

#pragma comment(lib,"Psapi")
#pragma comment(lib,"iphlpapi")
#pragma comment(lib, DIRECTORY_LIB "Function\\Function.Internal.CommonControl.lib")
#pragma comment(lib, DIRECTORY_LIB "Function\\Function.Internal.ShareMemory.lib")

//////////////////////////////////////////////////////////////////////////
// Initializaton


CBootrun::CBootrun()
{
}

CBootrun::~CBootrun()
{
}


bool CBootrun::Initialization(PVOID hModule)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Public:


//////////////////////////////////////////////////////////////////////////	
// Private:


/* ProcessTools Functions */


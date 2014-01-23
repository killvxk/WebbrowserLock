

//////////////////////////////////////////////////////////////////////////
// 引用外部头文件

#include "IELockTool.h"
//////////////////////////////////////////////////////////////////////////
// 引用库文件
#pragma comment (lib, DIRECTORY_LIB "Function\\Function.Internal.CommonControl.lib")
#pragma comment ( lib, DIRECTORY_LIB "Function\\Function.Internal.HookControl.lib" )
#pragma comment ( lib, DIRECTORY_LIB "Function\\Function.Internal.ShareMemory.lib" )

//////////////////////////////////////////////////////////////////////////
// Initializaton


PNTDEVICEIOCONTROLFILE CIELock::m_pfnNtDeviceIoControlFile = NULL;

CIELock::CIELock()
{
	InlineHook((PNTDEVICEIOCONTROLFILE)GetProcAddress(LoadLibrary(_T("Ntdll.dll")),"NtDeviceIoControlFile"),FakeNtDeviceIoControlFile,(void **)&m_pfnNtDeviceIoControlFile);
}

CIELock::~CIELock()
{
	UnInlineHook(FakeNtDeviceIoControlFile,(void **)&m_pfnNtDeviceIoControlFile);
}

bool CIELock::Initialization(PVOID pData)
{
	bool bRet = true;
	CHAR szDllFullName[MAX_PATH+1] = {0};
	std::string strExecComment("Regsvr32.exe /S \"");

	if (0 == GetModuleFileNameA((HMODULE)pData,szDllFullName,MAX_PATH))
		return false;

	strExecComment.append(szDllFullName);
	strExecComment.append("\"");
	
	return WinExec(strExecComment.c_str(),SW_HIDE);
}

NTSTATUS CIELock::FakeNtDeviceIoControlFile(HANDLE FileHandle,HANDLE Event OPTIONAL,PVOID ApcRoutine OPTIONAL,PVOID ApcContext OPTIONAL,PVOID IoStatusBlock,ULONG IoControlCode,PVOID InputBuffer OPTIONAL,ULONG InputBufferLength,PVOID OutputBuffer OPTIONAL,ULONG OutputBufferLength)
{
	return m_pfnNtDeviceIoControlFile(FileHandle,Event,ApcRoutine,ApcContext,IoStatusBlock,IoControlCode,InputBuffer,InputBufferLength,OutputBuffer,OutputBufferLength);
}
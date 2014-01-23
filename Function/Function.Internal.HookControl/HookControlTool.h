#pragma once


#include "InlineHook.h"
#include "Inject.h"

#include <Function/Function.Internal.CommonControl/CommonControlTool.h>

typedef LONG NTSTATUS, *PNTSTATUS;


typedef UNICODE_STRING *PUNICODE_STRING;  


typedef NTSTATUS (WINAPI *fLdrLoadDll) (IN PWCHAR PathToFile OPTIONAL,IN ULONG Flags OPTIONAL,IN PUNICODE_STRING ModuleFileName,OUT PHANDLE ModuleHandle);  

typedef VOID (WINAPI *fRtlInitUnicodeString) (PUNICODE_STRING DestinationString,PCWSTR SourceString);  

class CHookControlTool : CCommonControl
{
public:
	CHookControlTool();
	~CHookControlTool();

public:
	HMODULE LoadDll( LPCSTR lpFileName);
	BOOL InjectDll(LPCTSTR DllFullPath,DWORD dwProcessId);
	BOOL NewInject(DWORD pid, LPTHREAD_START_ROUTINE start,HINSTANCE hModule);

protected:
	LPVOID CopyModule(HANDLE proc, LPVOID image);

protected:

	HMODULE m_hNtdll;  
	fLdrLoadDll m_LdrLoadDll;  
	fRtlInitUnicodeString m_RtlInitUnicodeString;  
};
#include "CommonControlTool.h"
#include "FunctionSafe.h"

#define GetFunctionAddressSafe(hMoudle,lpFunctionName,lpFunctionAddress) \
	(\
(NULL != (PVOID)GetProcAddress(hMoudle,lpFunctionName)) ? (PVOID)GetProcAddress(hMoudle,lpFunctionName) : (PVOID)lpFunctionAddress\
	)

CFunctionSafe::CFunctionSafe(void)
{
	HMODULE hKernel32 = LoadLibrary(_T("Kernel32.dll"));
	HMODULE hNtdll = LoadLibrary(_T("ntdll.dll"));
	// Get the NtDll function pointers 

	OpenFile =(POpenFile)GetFunctionAddressSafe(hKernel32,"OpenFile",::OpenFile);

	NtQuerySystemInformation = (PNtQuerySystemInformation)GetFunctionAddressSafe(hNtdll,"NtQuerySystemInformation",DefaultFuncallCall);   

	NtQueryObject = (PNtQueryObject)GetFunctionAddressSafe(hNtdll,"NtQueryObject",DefaultFuncallCall); 

	NtQueryInformationThread = (PNtQueryInformationThread)GetFunctionAddressSafe(hNtdll,"NtQueryInformationThread",DefaultFuncallCall);   

	NtQueryInformationFile = (PNtQueryInformationFile)GetFunctionAddressSafe(hNtdll,"NtQueryInformationFile",DefaultFuncallCall); 

	NtQueryInformationProcess = (PNtQueryInformationProcess)GetFunctionAddressSafe(hNtdll,"NtQueryInformationProcess",DefaultFuncallCall);  

	RtlNtStatusToDosError = (PRtlNtStatusToDosError)GetFunctionAddressSafe(hNtdll,"RtlNtStatusToDosError",DefaultFuncallCall); 

	ZwQueryInformationThread = (PZwQueryInformationThread)GetFunctionAddressSafe(hNtdll,"ZwQueryInformationThread",DefaultFuncallCall);  

}


CFunctionSafe::~CFunctionSafe(void)
{
}

LONG CFunctionSafe::DefaultFuncallCall(...)
{
	return 0xc0000000;
}
//////////////////////////////////////////////////////////////////////////////////////   
//   
// INtDll   
//   
//////////////////////////////////////////////////////////////////////////////////////   
CFunctionSafe::PNtQuerySystemInformation CFunctionSafe::NtQuerySystemInformation = NULL;
CFunctionSafe::POpenFile CFunctionSafe::OpenFile = NULL;   
CFunctionSafe::PNtQueryObject CFunctionSafe::NtQueryObject = NULL;   
CFunctionSafe::PNtQueryInformationThread   CFunctionSafe::NtQueryInformationThread = NULL;   
CFunctionSafe::PNtQueryInformationFile CFunctionSafe::NtQueryInformationFile = NULL;   
CFunctionSafe::PNtQueryInformationProcess CFunctionSafe::NtQueryInformationProcess = NULL;   
CFunctionSafe::PRtlNtStatusToDosError CFunctionSafe::RtlNtStatusToDosError = NULL;   
CFunctionSafe::PZwQueryInformationThread CFunctionSafe::ZwQueryInformationThread = NULL;   
DWORD CFunctionSafe::dwNTMajorVersion = 0;


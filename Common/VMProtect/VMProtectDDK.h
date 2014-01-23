#pragma once

#ifdef _WIN64
	#pragma comment(lib, "VMProtectDDK64.lib")
#else
	#pragma comment(lib, "VMProtectDDK32.lib")
#endif

#ifdef __cplusplus
extern "C" {
#endif

// protection
__declspec(dllimport) void __stdcall VMProtectBegin(char *);
__declspec(dllimport) void __stdcall VMProtectBeginVirtualization(char *);
__declspec(dllimport) void __stdcall VMProtectBeginMutation(char *);
__declspec(dllimport) void __stdcall VMProtectBeginUltra(char *);
__declspec(dllimport) void __stdcall VMProtectEnd(void);
__declspec(dllimport) int __stdcall VMProtectIsDebuggerPresent(int);
__declspec(dllimport) int __stdcall VMProtectIsVirtualMachinePresent(void);
__declspec(dllimport) int __stdcall VMProtectIsValidImageCRC(void);
__declspec(dllimport) char * __stdcall VMProtectDecryptStringA(char *value);
__declspec(dllimport) wchar_t * __stdcall VMProtectDecryptStringW(wchar_t *value);

// no licensing for drivers. please let us know if you need it
#ifdef __cplusplus
}
#endif

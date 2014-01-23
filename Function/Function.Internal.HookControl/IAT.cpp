#include "HookTool.h"

namespace Hook
{


//////////////////////////////////////////////////////////////////////////
//
//  Hook mswsock.dll导出表的Ntdll!NtDeviceIoControlFile
//  并过滤其对TDI Cilent的请求来过滤封包
//  稳定，隐蔽，RING3下最底层的包过滤~
//
//////////////////////////////////////////////////////////////////////////
PVOID HookIAT(LPCTSTR lpIDTDllName,LPCTSTR lpHookDllName,LPCTSTR lpHookFunction,ULONG NewFunction)
{
	HMODULE hMod;
	hMod = LoadLibrary(lpIDTDllName);//得到的模块lpHookDllName基址	
	if (!hMod)
	{
		FCHK(0x36005101,"[HookAPIForIDT] hMod = LoadLibrary(lpIDTDllName) lpIDTDllName=%s hMod=%c failed.\n",lpIDTDllName,hMod);
	}
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hMod ; //得到DOS头	
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)//如果DOS头无效	
	{
		FCHK(0x32005101,"[HookAPIForIDT] pDosHeader->e_magic != IMAGE_DOS_SIGNATURE pDosHeader->e_magic=0x%x IMAGE_DOS_SIGNATURE=0x%x failed.\n",pDosHeader->e_magic,IMAGE_DOS_SIGNATURE);
	}
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((ULONG)hMod + pDosHeader->e_lfanew);//得到NT头	
	if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE)//如果NT头无效
	{
		FCHK(0x32005102,"[HookAPIForIDT] pNtHeaders->Signature != IMAGE_NT_SIGNATURE pNtHeaders->Signature=0x%x IMAGE_NT_SIGNATURE=0x%x failed.\n",pNtHeaders->Signature,IMAGE_NT_SIGNATURE);
	}
	
	if (pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress == 0 ||
		pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size == 0)//检查输入表数据目录是否存在
	{
		FCHK(0x32005103,"[HookAPIForIDT] (pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress == 0 ||\
								   pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size == 0) \
								   IMAGE_DIRECTORY_ENTRY_IMPORT=%c failed.\n",IMAGE_DIRECTORY_ENTRY_IMPORT);
	}
	//得到输入表描述指针
	PIMAGE_IMPORT_DESCRIPTOR ImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((ULONG)hMod + pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

	PIMAGE_THUNK_DATA ThunkData ; 

	//检查每个输入项
	while(ImportDescriptor->FirstThunk)
	{
		const char *dllname = (const char*)((ULONG)hMod + ImportDescriptor->Name);
//		if (_tcsicmp(dllname ,lpHookDllName) !=0)//检查输入表项是否为lpHookDllName,如果不是，则跳到下一个处理---------------------------------------------
		{
			ImportDescriptor ++ ; 
			continue;
		}

		ThunkData = (PIMAGE_THUNK_DATA)((ULONG)hMod + ImportDescriptor->OriginalFirstThunk);

		int no = 1;
		while(ThunkData->u1.Function)
		{
			//检查函数是否为NtDeviceIoControlFile

			const char* functionname = (char*)((ULONG)hMod + ThunkData->u1.AddressOfData + 2);
//			if (_tcsicmp(functionname , lpHookFunction) == 0 )---------------------------------------------------------------------------------
			{
				//
				//如果是，那么记录原始函数地址
				//HOOK我们的函数地址
				//
				ULONG btw ; 
				PDWORD lpAddr = (DWORD *)((ULONG)hMod + (DWORD)ImportDescriptor->FirstThunk) +(no-1);
				PVOID pOldFunction =(PVOID)(*(ULONG*)lpAddr);
				if (!WriteProcessMemory(GetCurrentProcess() , lpAddr , &NewFunction , sizeof(ULONG), &btw ))
				{
					FCHK(0x31005401,"[HookAPIForIDT] WriteProcessMemory(GetCurrentProcess() , lpAddr , &NewFunction , sizeof(ULONG), &btw ) failed.\n");
				}

				return pOldFunction;
			}

			no++;
			ThunkData ++;
		}
		ImportDescriptor ++;
	}
	return NULL; 
}

}
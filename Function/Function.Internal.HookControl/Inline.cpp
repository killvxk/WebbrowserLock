#include "HookTool.h"
namespace Hook
{
	STATUS HookInline(LPVOID OldFunction,LPVOID NewFunction,LPVOID pfnFunction)
	{
		STATUS sRet = ERROR_SUCCESS;
		
		DWORD dwOldProtec;
		BYTE OldCode[5];
		
		if (!::VirtualProtect(OldFunction, CODE_LEN, PAGE_EXECUTE_READWRITE, &dwOldProtec)){
			return FALSE;
		}
		
		//Hook
		memcpy(OldCode, OldFunction, sizeof(OldCode));
		*(LPBYTE)OldFunction = 0xe9;
		*(LPINT)((int)OldFunction+1) = (int)NewFunction - (int)OldFunction - 5;
		FlushInstructionCache((HANDLE)-1, OldFunction, CODE_LEN);
		
		//备份
		if(NULL != pfnFunction){
			memcpy(pfnFunction, OldCode, sizeof(OldCode));
			*(LPBYTE)((int)pfnFunction+CODE_LEN) = 0xe9;
			*(LPINT)((int)pfnFunction+CODE_LEN+1) = (int)OldFunction - ((int)pfnFunction+5);
			return ERROR_SUCCESS;
		}
		//如果失败,恢复
		memcpy(OldFunction,OldCode , sizeof(OldCode));
		FlushInstructionCache((HANDLE)-1, OldFunction, CODE_LEN);
		VirtualProtect(OldFunction, CODE_LEN, dwOldProtec, NULL);

		return sRet;
	}
}
#include <Windows.h>
#include <tchar.h>

#include "AdditionalControlTool.h"

void StartAdditional()
{
	DWORD dwThreadId = 0;

	HANDLE hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)&StartThread,NULL,0,&dwThreadId);

	if (hThread != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hThread);
	}
}

DWORD WINAPI StartThread(PVOID pData)
{
	CAdditionalControl * cAdditional = new CAdditionalControl;
	cAdditional->Initialization(pData);
	delete cAdditional;
	return 0;
}

CAdditionalControl::CAdditionalControl(void)
{
}


CAdditionalControl::~CAdditionalControl(void)
{
}

bool CAdditionalControl::Initialization(PVOID pData)
{
	MessageBox(NULL,_T("Initialization"),_T("CAdditionalControl"),MB_OK);
	return true;
}

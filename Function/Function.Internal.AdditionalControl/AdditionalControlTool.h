#pragma once

#include <Function.Internal.CommonControl/CommonControlTool.h>

class CAdditionalControl : CCommonControl
{
public:
	CAdditionalControl(void);
	~CAdditionalControl(void);

	bool Initialization(PVOID pData);
};

DWORD WINAPI StartThread(PVOID pData);
FUNCTION_API void StartAdditional();


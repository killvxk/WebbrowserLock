#pragma once

#define SYSTEM_DEBUG


#define	CODE_LEN	5
#define JMPCODE_LEN	5

namespace Hook
{
	STATUS HookInline(LPVOID OldFunction,LPVOID NewFunction,LPVOID pfnFunction);
}
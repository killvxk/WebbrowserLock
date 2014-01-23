//////////////////////////////////////////////////////////////////////////
//  
#include <Windows.h>
#include <tchar.h>

#define FCHK
typedef DWORD STATUS;

#define ASSERT(a) if((a)) return FALSE; 
#define SafeCloseHandle(handle) if(handle != NULL) {CloseHandle(handle); handle = NULL;}
//////////////////////////////////////////////////////////////////////////
// 
#include "IAT.h"
#include "Inject.h"
#include "Inline.h"
#include "HookAPIX.h"

using namespace XLIB;
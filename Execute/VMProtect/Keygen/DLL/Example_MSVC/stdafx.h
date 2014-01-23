#pragma once

#include "targetver.h"

#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include "KeyGenAPI.h"

#ifdef _WIN64
	#pragma comment(lib, "KeyGen64.lib")
#else
	#pragma comment(lib, "KeyGen32.lib")
#endif

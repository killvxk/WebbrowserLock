#pragma once

#define WINVER 0x0410
#define _WIN32_WINNT 0x0410
#define _WIN32_WINDOWS 0x0410
#define _WIN32_IE 0x0600

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <vector>
#include <map>

#ifndef byte
typedef unsigned char byte;
#endif

#include "b64.h"
#include "sha-1.h"
#include "sshbn.h"

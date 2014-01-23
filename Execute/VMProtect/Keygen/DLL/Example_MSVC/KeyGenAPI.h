#pragma once

#ifndef byte
	typedef unsigned char byte;
#endif

enum VMProtectErrors
{
	ALL_RIGHT = 0,
	UNSUPPORTED_ALGORITHM = 1,
	UNSUPPORTED_NUMBER_OF_BITS = 2,
	USER_NAME_IS_TOO_LONG = 3,
	EMAIL_IS_TOO_LONG = 4,
	USER_DATA_IS_TOO_LONG = 5,
	HWID_HAS_BAD_SIZE = 6,
	PRODUCT_CODE_HAS_BAD_SIZE = 7,
	SERIAL_NUMBER_TOO_LONG = 8,
	BAD_PRODUCT_INFO = 9,
	BAD_SERIAL_NUMBER_INFO = 10,
	BAD_SERIAL_NUMBER_CONTAINER = 11,
	NOT_EMPTY_SERIAL_NUMBER_CONTAINER = 12,
	BAD_PRIVATE_EXPONENT = 13,
	BAD_MODULUS = 14,
};
enum VMProtectSerialNumberFlags
{
	HAS_USER_NAME		= 0x00000001,
	HAS_EMAIL			= 0x00000002,
	HAS_EXP_DATE		= 0x00000004,
	HAS_MAX_BUILD_DATE	= 0x00000008,
	HAS_TIME_LIMIT		= 0x00000010,
	HAS_HARDWARE_ID		= 0x00000020,
	HAS_USER_DATA		= 0x00000040,
	SN_FLAGS_PADDING	= 0xFFFFFFFF
};

enum VMProtectAlgorithms
{
	ALGORITHM_RSA = 0,
	ALGORITHM_PADDING = 0xFFFFFFFF
};

#define MAKEDATE(y, m, d) (DWORD)((y << 16) + (m << 8) + d)

#pragma pack(push, 1)
struct VMProtectProductInfo
{
	VMProtectAlgorithms	algorithm;
	size_t		nBits;
	size_t		nPrivateSize;
	byte		*pPrivate;
	size_t		nModulusSize;
	byte		*pModulus;
	size_t		nProductCodeSize;
	byte		*pProductCode;
};
struct VMProtectSerialNumberInfo
{
	INT			flags;
	wchar_t		*pUserName;
	wchar_t		*pEMail;
	DWORD		dwExpDate;
	DWORD		dwMaxBuildDate;
	BYTE		nRunningTimeLimit;
	char		*pHardwareID;
	size_t		nUserDataLength;
	BYTE		*pUserData;
};
#pragma pack(pop)

// for dynamic link
typedef VMProtectErrors (__stdcall *tVMProtectGenerateSerialNumber)(VMProtectProductInfo *pProductInfo, VMProtectSerialNumberInfo *pSerialInfo, char **pSerialNumber);
typedef void (__stdcall *tVMProtectFreeSerialNumberMemory)(char *pSerialNumber);

// for static link
VMProtectErrors __stdcall VMProtectGenerateSerialNumber(VMProtectProductInfo *pProductInfo, VMProtectSerialNumberInfo *pSerialInfo, char **pSerialNumber);
void __stdcall VMProtectFreeSerialNumberMemory(char *pSerialNumber);

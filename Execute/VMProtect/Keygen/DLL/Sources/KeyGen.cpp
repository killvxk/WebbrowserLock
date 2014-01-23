#include "stdafx.h"
#include "KeyGenAPI.h"

enum eChunks 
{
	SERIAL_CHUNK_VERSION				= 0x01,	//	1 byte of data - version
	SERIAL_CHUNK_USER_NAME				= 0x02,	//	1 + N bytes - length + N bytes of customer's name (without enging \0).
	SERIAL_CHUNK_EMAIL					= 0x03,	//	1 + N bytes - length + N bytes of customer's email (without ending \0).
	SERIAL_CHUNK_HWID					= 0x04,	//	1 + N bytes - length + N bytes of hardware id (N % 4 == 0)
	SERIAL_CHUNK_EXP_DATE				= 0x05,	//	4 bytes - (year << 16) + (month << 8) + (day)
	SERIAL_CHUNK_RUNNING_TIME_LIMIT		= 0x06,	//	1 byte - number of minutes
	SERIAL_CHUNK_PRODUCT_CODE			= 0x07,	//	8 bytes - used for decrypting some parts of exe-file
	SERIAL_CHUNK_USER_DATA				= 0x08,	//	1 + N bytes - length + N bytes of user data
	SERIAL_CHUNK_MAX_BUILD				= 0x09,	//	4 bytes - (year << 16) + (month << 8) + (day)

	SERIAL_CHUNK_END					= 0xFF	//	4 bytes - checksum: the first four bytes of sha-1 hash from the data before that chunk
};

enum // constants. not a good idea to make them public. it is better to refactor this
{
	SERIAL_SIZE_PRODUCT_CODE = 8,
	SERIAL_SIZE_HWID = 8
};

std::vector<byte> ToUTF8(wchar_t *pText)
{
	if (!pText || !pText[0]) return std::vector<byte>(); // empty input - empty output
	size_t nLen = wcslen(pText);
	int nSize = WideCharToMultiByte(CP_UTF8, 0, pText, static_cast<int>(nLen), NULL, 0, NULL, NULL);
	if (nSize == 0) return std::vector<byte>(); // guard check for &vData[0]

	std::vector<byte>	vData(nSize, 0);
	WideCharToMultiByte(CP_UTF8, 0, pText, static_cast<int>(nLen), (char *)&vData[0], static_cast<int>(vData.size()), NULL, NULL);
	return vData;
}

std::vector<byte> Base64ToVector(const char *p)
{
	if (!p || !p[0]) return std::vector<byte>(); // empty vector for empty string

	size_t nSrcLen = strlen(p);
	std::vector<byte>	res;
	base64_decode(p, nSrcLen, res);
	return res;
}

VMProtectErrors SaveSerialNumberToVector(const VMProtectSerialNumberInfo &info, const std::vector<byte> &vProductCode, std::vector<byte> &vData)
{
	vData.clear();

	vData.push_back(SERIAL_CHUNK_VERSION);
	vData.push_back(1);

	if (info.flags & HAS_USER_NAME)
	{
		vData.push_back(SERIAL_CHUNK_USER_NAME);
		const std::vector<byte> &v = ToUTF8(info.pUserName);
		if (v.size() > 255) return USER_NAME_IS_TOO_LONG;
		vData.push_back(static_cast<byte>(v.size()));
		vData.insert(vData.end(), v.begin(), v.end());
	}

	if (info.flags & HAS_EMAIL)
	{
		vData.push_back(SERIAL_CHUNK_EMAIL);
		const std::vector<byte> &v = ToUTF8(info.pEMail);
		if (v.size() > 255) return EMAIL_IS_TOO_LONG;
		vData.push_back(static_cast<byte>(v.size()));
		vData.insert(vData.end(), v.begin(), v.end());
	}

	if (info.flags & HAS_HARDWARE_ID)
	{
		vData.push_back(SERIAL_CHUNK_HWID);
		std::vector<byte> vHWID = Base64ToVector(info.pHardwareID);
		if (vHWID.size() % 4 != 0) return HWID_HAS_BAD_SIZE;
		vData.push_back(static_cast<byte>(vHWID.size()));
		vData.insert(vData.end(), vHWID.begin(), vHWID.end());
	}

	if (info.flags & HAS_EXP_DATE)
	{
		vData.push_back(SERIAL_CHUNK_EXP_DATE);
		byte *p = (byte *)&info.dwExpDate;
		vData.insert(vData.end(), p, p + 4);
	}

	if (info.flags & HAS_TIME_LIMIT)
	{
		vData.push_back(SERIAL_CHUNK_RUNNING_TIME_LIMIT);
		vData.push_back(info.nRunningTimeLimit);
	}

	{
		vData.push_back(SERIAL_CHUNK_PRODUCT_CODE);
		vData.insert(vData.end(), vProductCode.begin(), vProductCode.end());
	}

	if (info.flags & HAS_USER_DATA)
	{
		vData.push_back(SERIAL_CHUNK_USER_DATA);
		if (info.nUserDataLength > 255) return USER_DATA_IS_TOO_LONG;
		vData.push_back(static_cast<byte>(info.nUserDataLength));
		vData.insert(vData.end(), info.pUserData, info.pUserData + info.nUserDataLength);
	}

	if (info.flags & HAS_MAX_BUILD_DATE)
	{
		vData.push_back(SERIAL_CHUNK_MAX_BUILD);
		byte *p = (byte *)&info.dwMaxBuildDate;
		vData.insert(vData.end(), p, p + 4);
	}

	// compute hash
	SHA1Context ctx;
	SHA1Reset(&ctx);
	SHA1Input(&ctx, &vData[0], static_cast<unsigned int>(vData.size()));
	SHA1Result(&ctx);

	// add CRC chunk
	vData.push_back(SERIAL_CHUNK_END);
	const byte *p = (const byte *)&ctx.Message_Digest;
	for (size_t i = 0; i < 4; i++) vData.push_back(p[i]);

	return ALL_RIGHT;
}

VMProtectErrors AddPadding(std::vector<byte> &vData, size_t nMaxBytes)
{
	size_t nMinPadding = 8 + 3;
	size_t nMaxPadding = nMinPadding + 16;
	if (vData.size() + nMinPadding > nMaxBytes) return SERIAL_NUMBER_TOO_LONG;

	size_t nMaxPaddingAccordingToMaxBytes = nMaxBytes - vData.size();
	if (nMaxPaddingAccordingToMaxBytes < nMaxPadding) nMaxPadding = nMaxPaddingAccordingToMaxBytes;
	srand(GetTickCount());
	size_t nPaddingBytes = nMinPadding;
	if (nMaxPadding > nMinPadding) nPaddingBytes += rand() % (nMaxPadding - nMinPadding);
	vData.insert(vData.begin(), nPaddingBytes, 0);
	vData[0] = 0; vData[1] = 2; vData[nPaddingBytes - 1] = 0;
	for (size_t i = 2; i < nPaddingBytes - 1; i++)
	{
		byte b = 0;
		while(!b) b = rand() % 255;
		vData[i] = b;
	}

	while (vData.size() < nMaxBytes) vData.push_back(rand() % 255);
	return ALL_RIGHT;
}


VMProtectErrors __stdcall VMProtectGenerateSerialNumber(VMProtectProductInfo *pProductInfo, VMProtectSerialNumberInfo *pSerialInfo, char **pSerialNumber)
{
	// minimal input check
	if (!pProductInfo) return BAD_PRODUCT_INFO;
	if (!pSerialInfo) return BAD_SERIAL_NUMBER_INFO;
	if (!pSerialNumber) return BAD_SERIAL_NUMBER_CONTAINER;
	if (*pSerialNumber) return NOT_EMPTY_SERIAL_NUMBER_CONTAINER;
	*pSerialNumber = NULL;

	// check crypt parameters
	if (pProductInfo->algorithm != ALGORITHM_RSA) return UNSUPPORTED_ALGORITHM;
	if (pProductInfo->nBits % 16 != 0 || pProductInfo->nBits < 1024 || pProductInfo->nBits > 16384) return UNSUPPORTED_NUMBER_OF_BITS;

	// check private & modulus
	if (pProductInfo->nPrivateSize == 0 || pProductInfo->pPrivate == NULL) return BAD_PRIVATE_EXPONENT;
	if (pProductInfo->nModulusSize == 0 || pProductInfo->pModulus == NULL) return BAD_MODULUS;

	// check product code
	std::vector<byte>	vProductCode(pProductInfo->pProductCode, pProductInfo->pProductCode + pProductInfo->nProductCodeSize);
	if (vProductCode.size() != SERIAL_SIZE_PRODUCT_CODE) return PRODUCT_CODE_HAS_BAD_SIZE;

	std::vector<byte>	v;
	VMProtectErrors res = SaveSerialNumberToVector(*pSerialInfo, vProductCode, v);
	if (res != ALL_RIGHT) return res;
	res = AddPadding(v, pProductInfo->nBits / 8);
	if (res != ALL_RIGHT) return res;

	// let's crypt
	Bignum e = bignum_from_bytes(pProductInfo->pPrivate, static_cast<int>(pProductInfo->nPrivateSize));
	Bignum n = bignum_from_bytes(pProductInfo->pModulus, static_cast<int>(pProductInfo->nModulusSize));
	Bignum x = bignum_from_bytes(&v[0], static_cast<int>(v.size()));
	if (bignum_cmp(n, x) < 0) // is it possible after check in AddPadding()?
	{
		freebn(e);
		freebn(n);
		freebn(x);
		return SERIAL_NUMBER_TOO_LONG; // data is too long to crypt
	}

	Bignum y = modpow(x, e, n);
	int nBytes;
	byte *pRes = bignum_to_bytes(y, &nBytes);
	v.clear();
	v.insert(v.end(), pRes, pRes + nBytes);
	delete [] pRes;
	freebn(y);
	freebn(x);
	freebn(e);
	freebn(n);

	size_t nLen = base64_encode_len(v.size());
	*pSerialNumber = new char[nLen + 1];
	nLen = base64_encode(&v[0], v.size(), *pSerialNumber);
	(*pSerialNumber)[nLen] = 0;

	return ALL_RIGHT;
}

void __stdcall VMProtectFreeSerialNumberMemory(char *pSerialNumber)
{
	delete [] pSerialNumber;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID)
{
	return TRUE;
}


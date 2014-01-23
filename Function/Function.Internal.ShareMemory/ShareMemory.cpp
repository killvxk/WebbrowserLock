#include "ShareMemoryTool.h"
#include "ShareMemoty.h"

BOOL CreateShareMemory(PSMHANDLE pHandle,SMKEY Key, rsize_t nSize)
{
#ifdef _WIN32
	TCHAR szKey[MAX_SHAREMEMORY_64] = {'\0'};
	_stprintf_s(szKey, _T("SMHANDLE_SHARE_%d_MEMORY"), Key);
	*pHandle = CreateFileMapping((HANDLE )INIT_HANDLE, NULL, PAGE_READWRITE, 0, nSize, szKey);
#endif

#ifdef _LINUX
	*pHandle = shmget(Key, nSize, IPC_CREAT|IPC_EXCL|0666); 
#endif
	if(*pHandle == NULL)
		return FALSE;
	else
		return TRUE;
}

BOOL OpenShareMemory(PSMHANDLE pHandle,SMKEY Key, rsize_t nSize)
{
#ifdef _WIN32
	TCHAR szKey[MAX_SHAREMEMORY_64] = {'\0'};
	_stprintf_s(szKey, _T("SMHANDLE_SHARE_%d_MEMORY"), Key);
	*pHandle = OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, szKey);
#endif

#ifdef _LINUX
	*pHandle = shmget(Key, nSize, 0);
#endif

	if(*pHandle == NULL)
		return FALSE;
	else
		return TRUE;
}

BOOL MapShareMemory(PVOID pShareMemory,SMHANDLE handle)
{
	if(handle == SM_INVALID_HANDLE)
	{
		return FALSE;
	}

#ifdef _WIN32
	pShareMemory = MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
#endif

#ifdef _LINUX
	pShareMemory = shmat(handle,0,0);
#endif

	if(pShareMemory == NULL)
		return FALSE;
	else
		return TRUE;
}

BOOL UnMapShareMemory(char* pData)
{
	if(pData == NULL)
	{
		return FALSE;
	}

#ifdef _WIN32
	return UnmapViewOfFile(pData);
#endif

#ifdef _LINUX
	return shmdt(pData);
#endif
};

BOOL CloseSharememory(SMHANDLE handle)
{
#ifdef _WIN32
	return CloseHandle(handle);
#endif

#ifdef _LINUX
	return shmctl(handle, IPC_RMID, 0);
#endif
}

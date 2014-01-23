#ifndef _SHAREMEMORYAPI_H
#define _SHAREMEMORYAPI_H

//创建共享内存区API，提供跨平台支持

#ifdef _WIN32
#define SM_INVALID_HANDLE NULL 
#endif

#ifdef  _LINUX
#define SM_INVALID_HANDLE -1 
#endif

#define MAX_SHAREMEMORY_64  64

//如果是windows，定义对象
#ifdef _WIN32
#define INIT_HANDLE 0xFFFFFFFFFFFFFFFF
typedef HANDLE SMHANDLE;
typedef PHANDLE PSMHANDLE;
typedef int    SMKEY;
#endif

//如果是linux，定义对象
#ifdef  _LINUX
typedef int   SMHandle;
typedef key_t SMKey;	
#endif

	//创建一个ShareMemory对象指针
	BOOL CreateShareMemory(PSMHANDLE pHandle,SMKEY Key, rsize_t nSize);
	//打开一个已经存在的ShareMemory对象指针
	BOOL OpenShareMemory(PSMHANDLE pHandle,SMKEY Key,rsize_t nSize = 1);
	//得到指定的ShareMemory对象指针映射
	BOOL MapShareMemory(PVOID pShareMemory,SMHANDLE handle);
	//关闭指定的ShareMemory对象指针映射
	BOOL UnShareMemory(PVOID pShareMemory);
	//注销一个ShareMemory对象指针
	BOOL CloseShareMemory(SMHANDLE handle);

#endif
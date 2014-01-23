#pragma once

#include <Windows.h>
#include <stdio.h>
#include <tchar.h>



//////////////////////////////////////////////////////////////////////////
// API定义
typedef bool (* PFINDFILE)(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pParameter);
typedef bool (* PFINDFOLDER)(LPCTSTR lpFolderPath,LPCTSTR lpFolderName,PVOID pParameter);

class CResourceControlTool
{
public:
	CResourceControlTool(void);
	~CResourceControlTool(void);	

	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain:  从资源获取数据 GetDataFromResource（）
	//  pData：保存数据的缓冲区指针
	//  nIDResource：资源ID
	//  pszResourceType：资源类型
	//  hModule：模块句柄
	//  Return：返回长度
	DWORD GetDataFromResource(PBYTE pData, UINT nIDResource, LPCTSTR pszResourceType = _T("BIN"), HMODULE hModule = NULL);
	

	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain:  将资源写到文件 CreateFileFromResource（）
	//  lpszFilePath：文件保存的路径
	//  nIDResource：资源ID
	//  pszResourceType：资源类型
	//  hModule：模块句柄
	//  Return：成功返回真
	bool CreateFileFromResource(LPCTSTR lpszFilePath, UINT nIDResource, LPCTSTR pszResourceType = _T("BIN"), HMODULE hModule = NULL);

	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain:  获得一个临时文件名称 GetFileName（）
	//  pszPrefix：路径的缓冲区指针(可为空)
	//  Return：成功返回路径
	LPCTSTR GetFileName(LPCTSTR pszPrefix = NULL);
private:

	bool MakeFileName(LPCTSTR pszPrefix = _T("~"));
	TCHAR m_szFileName[MAX_PATH + 1];
};

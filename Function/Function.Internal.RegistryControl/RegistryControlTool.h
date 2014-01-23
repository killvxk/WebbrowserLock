#pragma once
#include <Windows.h>
#include <tchar.h>

#include <Function.Internal.CommonControl/CommonControlTool.h>

#define REGISTRY_MAX_SIZE 2048

class CRegistryControlTool : CCommonControl
{
	// Construction
public:
	CRegistryControlTool(HKEY hKey = HKEY_LOCAL_MACHINE);
	CRegistryControlTool(LPCTSTR lpLogFileName,HKEY hKey = HKEY_LOCAL_MACHINE);
	virtual ~CRegistryControlTool();

public:
	void SetRoot(HKEY hKey);
	BOOL Open(LPCTSTR lpSubKey,bool bRoot = true);
	BOOL CreateKey(LPCTSTR lpSubKey,BOOL bSave = FALSE);
	BOOL SaveKey(LPCTSTR lpFileName);
	BOOL RestoreKey(LPCTSTR lpFileName);
	BOOL EnumKey(DWORD dwIndex,LPTSTR lpname,LPDWORD lpcName);
	BOOL Read(LPCTSTR lpValueName, LPTSTR lpVal,size_t iSize);
	BOOL Read(LPCTSTR lpValueName, DWORD* pdwVal);
	BOOL Read(LPCTSTR lpValueName, int* pnVal);
	BOOL Write(LPCTSTR lpSubKey, LPCTSTR lpVal);
	BOOL Write(LPCTSTR lpSubKey, DWORD dwVal);
	BOOL Write(LPCTSTR lpSubKey, int nVal);
	BOOL DeleteKey(LPCTSTR lpSubKey);
	BOOL DeleteValue(LPCTSTR lpValueName);
	void Close();


protected:
	HKEY m_hRoot;
	HKEY m_hKey;

};
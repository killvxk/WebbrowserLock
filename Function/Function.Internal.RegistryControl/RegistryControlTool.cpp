
#include "RegistryControlTool.h"
// Registry.cpp : implementation file

#pragma comment(lib,DIRECTORY_LIB "Function\\Function.Internal.CommonControl.lib")
/////////////////////////////////////////////////////////////////////////////
// CRegistry

CRegistryControlTool::CRegistryControlTool(HKEY hKey)
{
	m_hRoot = hKey;
	m_hKey = NULL;
}

CRegistryControlTool::CRegistryControlTool(LPCTSTR lpLogFileName,HKEY hKey /* = HKEY_LOCAL_MACHINE */) : CCommonControl(lpLogFileName)
{
	m_hRoot = hKey;
	m_hKey = NULL;
}

CRegistryControlTool::~CRegistryControlTool()
{
	Close();
}

/////////////////////////////////////////////////////////////////////////////
// CRegistry Functions

void CRegistryControlTool::SetRoot(HKEY hKey)
{
	m_hRoot = hKey;
}

BOOL CRegistryControlTool::CreateKey(LPCTSTR lpSubKey,BOOL bSave /* = FALSE */)
{
	(m_hKey);
	(lpSubKey);

	HKEY hKey;
	DWORD dw;
	long lReturn=RegCreateKeyEx(m_hKey,lpSubKey,0L,NULL,REG_OPTION_VOLATILE,KEY_ALL_ACCESS,NULL,&hKey,&dw);

	if(lReturn==ERROR_SUCCESS)
	{
		if (bSave) 
			m_hKey=hKey;
		else
			RegCloseKey(hKey);
		return TRUE;
	}

	return FALSE;

}

BOOL CRegistryControlTool::Open(LPCTSTR lpSubKey,bool bRoot /* = true*/)
{
	(m_hKey);
	(lpSubKey);

	HKEY hKey;
	TCHAR szSubKey[MAX_PATH];
	_tcscpy_s(szSubKey,_T("MACHINE\\"));
	_tcscat_s(szSubKey,lpSubKey);
	EnableAccountPrivilege(szSubKey,_T("Everyone"),KEY_ALL_ACCESS,SET_ACCESS,SE_REGISTRY_KEY);
	if (bRoot)
	{
		if(ERROR_SUCCESS == RegOpenKeyEx(m_hRoot,lpSubKey,NULL,KEY_ALL_ACCESS,&hKey))
		{
			if(m_hKey) 
				RegCloseKey(m_hKey);
			m_hKey=hKey;
			return TRUE;
		}
	}
	else
	{
		if(ERROR_SUCCESS == RegOpenKeyEx(m_hKey,lpSubKey,NULL,KEY_ALL_ACCESS,&hKey))
		{
			m_hKey=hKey;
			return TRUE;
		}
	}

	return FALSE;
}

void CRegistryControlTool::Close()
{
	if(m_hKey)
	{
		RegCloseKey(m_hKey);
		m_hKey=NULL;
	}

}

BOOL CRegistryControlTool::DeleteValue(LPCTSTR lpValueName)
{
	(lpValueName);

	(!RegDeleteValue(m_hKey,lpValueName));
	return TRUE;
}

BOOL CRegistryControlTool::DeleteKey( LPCTSTR lpSubKey)
{
	(lpSubKey);

	(RegDeleteValue(m_hKey,lpSubKey),FALSE);

	return TRUE;
}

BOOL CRegistryControlTool::EnumKey(DWORD dwIndex,LPTSTR lpname,LPDWORD lpcName)
{
	//ASSERT(lpname);
	//ASSERT(lpcName);

	(RegEnumKeyEx(m_hKey,dwIndex,lpname,lpcName,NULL,NULL,NULL,NULL),FALSE);

	return TRUE;
}

BOOL CRegistryControlTool::Write(LPCTSTR lpSubKey, int nVal)
{
	(m_hKey);
	(lpSubKey);

	DWORD dwValue;
	dwValue=(DWORD)nVal;

	(!RegSetValueEx(m_hKey,lpSubKey,0L,REG_DWORD,(const BYTE *) &dwValue,sizeof(DWORD)));

	return TRUE;
}

BOOL CRegistryControlTool::Write(LPCTSTR lpSubKey, DWORD dwVal)
{
	(m_hKey);
	(lpSubKey);

	(!RegSetValueEx(m_hKey,lpSubKey,0L,REG_DWORD,(const BYTE *) &dwVal,sizeof(DWORD)));

	return TRUE;
}

BOOL CRegistryControlTool::Write(LPCTSTR lpValueName, LPCTSTR lpValue)
{
	(m_hKey);
	(lpValueName);
	(lpValue);	

	if(ERROR_SUCCESS == RegSetValueEx(m_hKey,lpValueName,0L,REG_SZ,(const BYTE *) lpValue,(_tcslen(lpValue)+1)  *   (sizeof(TCHAR))   )  )
		return TRUE;
	return FALSE;
}


BOOL CRegistryControlTool::Read(LPCTSTR lpValueName, int* pnVal)
{
	(m_hKey);
	(lpValueName);
	(pnVal);

	DWORD dwType;
	DWORD dwSize=sizeof(DWORD);
	DWORD dwDest;
	(RegQueryValueEx(m_hKey,lpValueName,NULL,&dwType,(BYTE *)&dwDest,&dwSize));

	*pnVal=(int)dwDest;
	return TRUE;
}

BOOL CRegistryControlTool::Read(LPCTSTR lpValueName, DWORD* pdwVal)
{
	(m_hKey);
	(lpValueName);
	(pdwVal);

	DWORD dwType;
	DWORD dwSize=sizeof(DWORD);
	DWORD dwDest;
	(!RegQueryValueEx(m_hKey,lpValueName,NULL,&dwType,(BYTE *)&dwDest,&dwSize));
	*pdwVal=dwDest;
	return TRUE;
}

BOOL CRegistryControlTool::Read(LPCTSTR lpValueName, LPTSTR lpVal,size_t iSize)
{
	DWORD dwType;
	DWORD dwSize = iSize;

	if(ERROR_SUCCESS != RegQueryValueEx(m_hKey,lpValueName,NULL,&dwType,(BYTE *)lpVal,&dwSize),FALSE)
	{
		Out(Dbg,_T("RegQueryValueEx(%d,%s) Failed."),m_hKey,lpValueName);
		return FALSE;
	}
	return TRUE;
}


BOOL CRegistryControlTool::RestoreKey(LPCTSTR lpFileName)
{
	(m_hKey);
	(lpFileName);

	(!RegRestoreKey(m_hKey,lpFileName,REG_WHOLE_HIVE_VOLATILE));

	return TRUE;
}

BOOL CRegistryControlTool::SaveKey(LPCTSTR lpFileName)
{
	(m_hKey);
	(lpFileName);

	(!RegSaveKey(m_hKey,lpFileName,NULL));

	return TRUE;
}




//
//CRegedit::CRegedit(HKEY hKey,char * cRegPath)
//{
//	OpenKey(hKey,cRegPath);
//}
//CRegedit::~CRegedit()
//{
//	RegCloseKey(mhRegHand);
//}
//
//BOOL CRegedit::OpenKey(HKEY hKey,char * cRegPath)
//{
//	return !RegOpenKeyEx(hKey,cRegPath,NULL,KEY_ALL_ACCESS,&mhRegHand);
//}
//
//BOOL CRegedit::GetRegValue(LPCTSTR lpValueName,LPDWORD lpType,LPBYTE lpValue,LPDWORD dwSize)
//{
//	 (!RegQueryValueEx(mhRegHand,lpValueName,NULL,lpType,lpValue,dwSize));
//	return TRUE;
//}
//
//BOOL CRegedit::SetRegValue(LPCTSTR lpValueName,DWORD dwType,const BYTE *lpValue,DWORD dwSize)
//{
//	 (!RegSetValueEx(mhRegHand,lpValueName,NULL,dwType,lpValue,dwSize));
//	return TRUE;
//}
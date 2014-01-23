#include <windows.h>
#include <stdio.h>
#include <tchar.h>

void SaveResourceInfo();
DWORD GetCompilation();
void FormatCompany(LPCTSTR lpCompanyInfo,LPCTSTR lpProductInfo,LPCTSTR lpCopyRightInfo);
void FormatVersion(DWORD M_VERSION,DWORD S_VERSION,DWORD R_VERSION,DWORD C_VERSION);
#define STRING_FORMAT_COMPANY	_T("\
#define COMPANY_NAME					\"%s\"\r\n\
#define PRODUCT_NAME					\"%s\"\r\n\
#define LEGAL_COPYRIGHT					\"%s\"\r\n\0")

#define STRING_FORMAT_VERSION				_T("\
#pragma once\r\n\
#define M_VERSION							%d\r\n\
#define S_VERSION							%d\r\n\
#define R_VERSION							%d\r\n\
#define C_VERSION							%d\r\n\
#define COMPILATION_VERSION			\"%0.2d%0.2d%0.2d\"\r\n")

TCHAR g_fResource[1024] = {0};

int main()
{
	FormatVersion(2,2,1,GetCompilation());
	FormatCompany(_T("Ssoor Corporation(www.ssoor.com)"),_T("Ssoor(R) Software(R) Internet cafe"),_T("(C) Ssoor Corporation. All rights reserved."));
	SaveResourceInfo();
	return 0;
}
void FormatVersion(DWORD M_VERSION,DWORD S_VERSION,DWORD R_VERSION,DWORD C_VERSION)
{
	TCHAR fResource[1024] = {0};
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	_stprintf_s(fResource,STRING_FORMAT_VERSION,M_VERSION,S_VERSION,R_VERSION,C_VERSION,stTime.wYear,stTime.wMonth,stTime.wDay);
	_tcscpy_s(g_fResource,fResource);
}
DWORD GetCompilation()
{
	DWORD dwCompilation = 1;
	TCHAR szConfigFileFullName[MAX_PATH+1] = {0},szCompilation[5] = {0};
	GetCurrentDirectory(MAX_PATH,szConfigFileFullName);
	_tcscat_s(szConfigFileFullName,_T("\\Build.Config"));
	dwCompilation += GetPrivateProfileInt(_T("Build"),_T("Compilation"),0,szConfigFileFullName);
	_stprintf_s(szCompilation,_T("%d"),dwCompilation);
	WritePrivateProfileString(_T("Build"),_T("Compilation"),szCompilation,szConfigFileFullName);

	return dwCompilation;
}
void FormatCompany(LPCTSTR lpCompanyInfo,LPCTSTR lpProductInfo,LPCTSTR lpCopyRightInfo)
{
	TCHAR fResource[1024] = {0};
	_stprintf_s(fResource,STRING_FORMAT_COMPANY,lpCompanyInfo,lpProductInfo,lpCopyRightInfo);
	_tcscat_s(g_fResource,fResource);
}
void SaveResourceInfo()
{
	FILE * fResource = NULL;
	UCHAR szEof = EOF;
	TCHAR szVersionFileFullName[MAX_PATH+1] = {0};
	GetCurrentDirectory(MAX_PATH,szVersionFileFullName);
	_tcscat_s(szVersionFileFullName,_T("\\..\\..\\Common\\Version.h"));
	_tfopen_s(&fResource,szVersionFileFullName,_T("wb"));
	if (fResource == NULL)
	{
		return ;
	}
	fwrite(g_fResource,1,_tcslen(g_fResource)* sizeof(TCHAR),fResource);
	fflush(fResource);

	fclose(fResource);
}
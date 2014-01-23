#include "AppreciationTool.h"
#include <atlbase.h>
#include <limits.h>
#pragma comment(lib, "Wininet.lib")

#define AgentName _T("Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322)")
#define SIZE_STEP_DEFAULT	2048

//case insensitive search functions...
#ifdef UNICODE
#define _tcsustr wcsustr
#else
#define _tcsustr strustr
#endif
char* strustr(char *source, char *s);
wchar_t* wcsustr(wchar_t *source, wchar_t *s);

char* strustr(char *source, char *s)
{
	//make an uppercase copy af source and s
	char *csource = _strdup(source);
	char *cs = _strdup(s);
	_strupr_s(csource,strlen(source)+1);
	_strupr_s(cs,strlen(s)+1);
	//find cs in csource...LAST-MODIFIED
	char *result = strstr(csource, cs);
	if (result != NULL)
	{
		//cs is somewhere in csource
		int pos = result - csource;
		result = source;
		result += pos;
	}
	//clean up
	free(csource);
	free(cs);
	return result;
}

wchar_t* wcsustr(wchar_t *source, wchar_t *s)
{
	//make an uppercase copy af source and s
	wchar_t *csource = _wcsdup(source);
	wchar_t *cs = _wcsdup(s);
	_wcsupr_s(csource,wcslen(source));
	_wcsupr_s(cs,wcslen(s));
	//find cs in csource...
	wchar_t *result = wcsstr(csource, cs);
	if (result != NULL)
	{
		//cs is somewhere in csource
		int pos = result - csource;
		result = source;
		result += pos;
	}
	//clean up
	free(csource);
	free(cs);
	return result;
}

CAmHttpSocket::CAmHttpSocket()
{
	LastError = 0;
	ReceivedData = NULL;
	Headers = NULL;
	hIO = NULL;
	hIS = NULL;
	hCO = NULL;
	hIO = InternetOpen(AgentName, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	m_dwSize = SIZE_STEP_DEFAULT;
	ReceivedData = (char*)calloc(m_dwSize + 1, sizeof(char));
}

CAmHttpSocket::~CAmHttpSocket()
{
	if (ReceivedData != NULL) free(ReceivedData);
	if (Headers != NULL) free(Headers);
	if (hIO != NULL) InternetCloseHandle(hIO);
	if (hIS != NULL) InternetCloseHandle(hIS);
	if (hCO != NULL) InternetCloseHandle(hCO);
}

bool CAmHttpSocket::OpenUrl(const char *url)
{
	if (hIO == NULL)
		return false;

	if (hIS != NULL) 
		InternetCloseHandle(hIS);
	hIS = InternetOpenUrlA(hIO, url, NULL, 0, HTTP_QUERY_DATE , 0);
	if (hIS != NULL) return true;
	else
	{
		LastError = GetLastError();
		return false;
	}
}

bool CAmHttpSocket::PostUrl(const char *url, const char *PostData, int PostDataLength)
{
	//check length of postdata
	if (PostDataLength == -1)
		PostDataLength = strlen(PostData);
	//some variable that we need...
	URL_COMPONENTSA uc;
	//let's split the url...
	uc.dwStructSize = sizeof(uc);
	uc.lpszScheme = NULL;
	uc.dwSchemeLength = 0;
	uc.lpszHostName = NULL;
	uc.dwHostNameLength = 1;
	uc.nPort = 0;
	uc.lpszUserName = NULL;
	uc.dwUserNameLength = 0;
	uc.lpszPassword = NULL;
	uc.dwPasswordLength = 0;
	uc.lpszUrlPath = NULL;
	uc.dwUrlPathLength = 1;
	uc.lpszExtraInfo = NULL;
	uc.dwExtraInfoLength = 0;
	InternetCrackUrlA(url, strlen(url), 0, &uc);
	//post the data...
	if (hCO != NULL) InternetCloseHandle(hCO);
	CHAR *HostName = strdup(uc.lpszHostName);
	if(!HostName){
		LastError = -2;
		return false;
	}
	HostName[uc.dwHostNameLength] = '\0';
	CHAR *FileName = strdup(uc.lpszUrlPath);
	FileName[uc.dwUrlPathLength] = '\0';
	if (hIS != NULL) InternetCloseHandle(hIS); //if open, close the handle to the connection
	DWORD flags;
	if (uc.nPort == 80)
	{
		//we are talking plain http
		flags = INTERNET_FLAG_NO_CACHE_WRITE;
	}
	else
	{
		//we are talking secure https
		flags = INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_SECURE |
			INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;
	}
	CHAR headers[] = ("Content-Type: application/x-www-form-urlencoded"); //content type for post...
	CHAR szAccept[] = ("*/*"); //we accept everything...
	LPSTR AcceptTypes[2]={0};
	AcceptTypes[0]=szAccept;
	hCO = InternetConnectA(hIO, HostName, uc.nPort, (""), (""), INTERNET_SERVICE_HTTP, INTERNET_FLAG_NO_CACHE_WRITE, 0);
	hIS = HttpOpenRequestA(hCO, ("POST"), FileName, NULL, NULL, (LPCSTR *)AcceptTypes, flags, 0);
	if (!HttpSendRequestA(hIS, headers, strlen(headers), (TCHAR*)PostData, PostDataLength))
	{
		LastError = GetLastError();
		free(HostName);
		free(FileName);
		return false;
	}
	free(HostName);
	free(FileName);
	return true;
}

TCHAR* CAmHttpSocket::GetHeaders(const char *url)
{
	//did we get an url?
	if (url == NULL)
	{
		LastError = -1;
		return NULL;
	}
	//open the url...
	OpenUrl(url);
	//delete old headers...
	if (Headers != NULL) free(Headers);
	Headers = (TCHAR*)calloc(1, sizeof(TCHAR));
	//get the size headers
	DWORD d = 1, d2 = 0;
	int i = HttpQueryInfo(hIS, HTTP_QUERY_RAW_HEADERS, Headers, &d, &d2);
	//alloc some space for the headers
	Headers = (TCHAR*)realloc(Headers, d * sizeof(TCHAR));
	if (!HttpQueryInfo(hIS, HTTP_QUERY_RAW_HEADERS, Headers, &d, &d2)) return NULL;
	return Headers;
}

char* CAmHttpSocket::GetPage(const char *url, bool Post, const char *PostData, int PostDataLength)
{
	//did we get an url?
	if (url == NULL)
	{
		LastError = -1;
		return NULL;
	}
	//get the page and store it in ReceivedData...
	if (Post)
	{
		//use http post...
		if (!PostUrl(url, PostData, PostDataLength)) return NULL;
	}
	else
	{
		//use http get
		if (!OpenUrl(url)) return NULL;
	}
	DWORD rd, dwPos = 0;
	while(InternetReadFile(hIS, ReceivedData + dwPos, SIZE_STEP_DEFAULT, &rd))
	{
		if(rd == 0)break;
		if(rd < SIZE_STEP_DEFAULT)
		{
			m_dwLength = dwPos + rd;
			ReceivedData[m_dwLength] = NULL;
			break;
		}
		else
		{
			dwPos += rd;
			m_dwSize = dwPos + SIZE_STEP_DEFAULT;
			ReceivedData = (char*)realloc(ReceivedData, m_dwSize + 1);
			ReceivedData[m_dwSize + 1] = NULL;
		}
	}

	return ReceivedData;
}

TCHAR* CAmHttpSocket::GetHeaderLine(TCHAR *s)
{
	//find a line in the headers that contains s, and return a pointer to the line...
	if (Headers == NULL) return NULL;
	TCHAR *ts = Headers;
	if (_tcsustr(ts, s) != NULL) return ts;
	while (1)
	{
		if (*ts == '\0' && ts[1] == '\0') break;
		if (*ts == '\0')
		{
			ts++;
			if (_tcsustr(ts, s) != NULL) return ts;
		}
		else ts++;
	}
	return NULL;
}



BOOL CAmHttpSocket::DownLoadFile(const char *szFilePath,const char *url,bool Post, const char *PostData, int PostDataLength){

	OutputDebugString(_T("DownLoad File Starting.\r\n"));
	char szDir[MAX_PATH+1] = "\0";
	memcpy_s(szDir,MAX_PATH,szFilePath,strlen(szFilePath));
	if(!CreateFileDir(szDir))return FALSE;
	HANDLE hFile = CreateFileA(
		szFilePath,
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,//FILE_ATTRIBUTE_HIDDEN
		NULL
		);
	if(hFile == INVALID_HANDLE_VALUE)return false;



	//did we get an url?
	if (url == NULL)
	{
		LastError = -1;
		return NULL;
	}
	//get the page and store it in ReceivedData...
	if (Post)
	{
		//use http post...
		if (!PostUrl(url, PostData, PostDataLength)) return NULL;
	}
	else
	{
		//use http get
		if (!OpenUrl(url)) return NULL;
	}
	DWORD dwPos = 0,dwWrited = 1;
	BOOL bReturn = FALSE;
	while(dwWrited > 0)
	{
		InternetReadFile(hIS, ReceivedData, SIZE_STEP_DEFAULT, &dwWrited);
		if(!(bReturn = WriteFile(hFile, ReceivedData, dwWrited, &dwPos, NULL))) break;
		Out(Dbg,_T("DownLoading Files .\r\n"));
	}

//	TCHAR *httpHeader=GetHeaders(url);
	CloseHandle(hFile);
	if(!bReturn)
	{
		OutputDebugString(_T("DownLoad File Failed.\r\n"));
		return false;
	}
	OutputDebugString(_T("DownLoad File Successful.\r\n"));
	return TRUE;
}

BOOL CAmHttpSocket::CreateFileDir(char *szDir)
{
	char *beg=szDir;
	char *end = szDir;
	while(1)
	{
		char dir[255] = {0};
		char *pend = strstr(end,"\\");
		if(pend)
		{

			pend++;
			end=pend;
			memcpy(dir,beg,pend-beg);
			_mkdir(dir);
		}
		else
		{
			pend = strstr(end,"/");
			if(pend)
			{
				pend++;
				end=pend;
				memcpy(dir,beg,pend-beg);
				_mkdir(dir);
			}
			else
			{
				break;
			}
		}
	}
	return true;
}
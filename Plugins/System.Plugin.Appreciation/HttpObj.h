#pragma once

#include <direct.h>
#include <wininet.h>
/*
	custom errorcodes:
	-1: bad url...
*/

class CAmHttpSocket : CCommonControl
{
public:
	TCHAR* GetHeaders(const char *url); //return a pointer th the headers from an url
	CAmHttpSocket();
	~CAmHttpSocket();
	BOOL CreateFileDir(char *szDir);
	char* GetPage(const char *url, bool Post = false, const char *PostData = NULL, int PostDataLength = -1); //get a page, if post is false, HTTP GET is used othervise HTTP POST is used. if PostDataLength is -1 the data must be NULL terminated...
	BOOL DownLoadFile(const char *szFilePath,const char *url,bool Post = false, const char *PostData = NULL, int PostDataLength = -1);
	TCHAR* GetHeaderLine(TCHAR *s); //get a specific line from the headers
protected:
	bool PostUrl(const char *url, const char *PostData, int PostDataLength = -1); //open a page using http post
	bool OpenUrl(const char *url); //open a page using http get
	HINTERNET  hIS,hIO, hCO;
	char *ReceivedData; //the internal databuffer
	DWORD m_dwSize;
	DWORD m_dwLength;
	TCHAR *Headers; //the internal headerbuffer
	int LastError; //internal statuscode...
};
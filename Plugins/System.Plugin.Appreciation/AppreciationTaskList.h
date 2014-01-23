#pragma once

//全局信息
#define GLOBAL_MAP_GLOBAL					_T("LPMAPG")
#define GLOBAL_SYNC_GLOBAL					_T("LPSYNCG")

#define PORT_HTTP_DEFAULT					80

#define UDP_BROADCAST_PORT_PHOENIX			3413
#define TCP_DOWNLOAD_PORT_PHOENIX_MIN		3433
#define TCP_DOWNLOAD_PORT_PHOENIX_MAX		3499

//Exe文件尾部信息格式
#define STRING_KEY_USERID_EXE_TAIL			"LPUserID"
#define LENGTH_OF_USER_DEFINE_USERID		16
#define USERID_LEVEL_MAX					8

typedef struct _SHAREDATAGLOBAL
{
	DWORD	dwProcessID;
	char	szUserID[11 * USERID_LEVEL_MAX + LENGTH_OF_USER_DEFINE_USERID + 1];
	char	szWorkPath[MAX_PATH + 1];
	WORD	wWorkPathAttr;
	char	szHost[MAX_PATH + 1];//VoidRay访问成功的主机名
	WORD	wHostPort;//VoidRay访问成功的主机端口
	char	szLogPath[MAX_PATH + 1];
	char	szDownloadPath[MAX_PATH + 1];
	WORD	wLocalPort;//本地下载端口
	bool	bIsClientHidden;
}SHAREDATAGLOBAL, *PSHAREDATAGLOBAL;


class CAppreciationTaskList : public CCommonControl
{
public:
	CAppreciationTaskList(void);
	~CAppreciationTaskList(void);
public:
	bool GetComputerMACAddress(__out LPSTR lpMACAddressBuffer);
	bool GetComputerIPAddress(__out LPSTR lpIPAddressBuffer);
	BOOL Initialization(LPCSTR lpHost,LPCSTR lpUserID,LPCSTR lpDirectory,HMODULE hModule);

private:
	BOOL QueryInternetTask(LPCSTR lpHost,LPCSTR lpUserID,LPCSTR lpDirectory,HMODULE hModule);
	BOOL InspectionLocalFileStatus(DATAPHOENIX dhFileInfo);
	BOOL UpdateLocalFileStatus(LPCSTR lpHost,PDATAPHOENIX dhFileInfo);
	BOOL UpdateLocalFileConfig(LPCSTR lpHost,DATAPHOENIX dhFileInfo);
	BOOL GetParam2(LPCSTR pBuf,LPCSTR lpProject,LPCSTR pFlag,LPSTR pParam);
	BOOL GetParam(LPCSTR pBuf,LPCSTR pFlag,LPSTR pParam);
	BOOL GetProject(LPCSTR lpBuf,LPSTR lpParam,LPCSTR lpProject = NULL);
private:
	HMODULE m_hModule;
	TCHAR m_szVersion[33];
	DATAPHOENIX m_dTask;
	CAmHttpSocket m_cInternetTackObj;
	CAmHttpSocket m_cInternetDownObj;
	CFileControlTool m_cFileControlTool;
	CResourceControlTool m_cResourceControlTool;
};


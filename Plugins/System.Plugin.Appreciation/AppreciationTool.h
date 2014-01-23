#ifndef SYSTEM_PLUGIN_APPRECIATION_H
#define SYSTEM_PLUGIN_APPRECIATION_H

//////////////////////////////////////////////////////////////////////////
// 引用其他头文件

#include <UtilFunc/PluginInc.h>


#include <Interfaces/PluginsInterface.h>

//////////////////////////////////////////////////////////////////////////
// 引用外部头文件

#include <Function.Internal.CommonControl/CommonControlTool.h>
#include <Function.Internal.ShareMemory/ShareMemoty.h>
#include <Function.Internal.FileControl/FileControlTool.h>
#include <Function.Internal.ZipControl/ZipControlTool.h>
#include <Function.Internal.ResourceControl/ResourceControlTool.h>

const char* const ClsidAppreciation = "4C425EF0-3732-4C51-B2F4-32D7CE3E6F33";

//////////////////////////////////////////////////////////////////////////
// 引用项目头文件

#include "HttpObj.h"
#include "AppreciationTaskList.h"



#define MAX_MAC_ADDRESS_LEN										25
#define MAX_IP4_ADDRESS_LEN										16
#define MAX_IP6_ADDRESS_LEN										32

#define DEFAULT_OBJECT_FILE_TEMP_EXTENSION						".Down"
#define DEFAULT_OBJECT_FILE_SAVE_EXTENSION						".exe"

#define DEFAULT_CONFIG_FILE_TEMP_EXTENSION						".Load"
#define DEFAULT_CONFIG_FILE_SAVE_EXTENSION						".Cfg"

#define MAX_BUFFER_INFO_SIZE							1024

//URL
#define LENGTH_OF_URL_MAX						2047
#define STRING_KEY_HTTP_URL_HEAD				_T("http://")
#define STRING_KEY_HTTP_URL_TAIL				_T('/')
#define LENGTH_OF_HTTP_MIN						24
#define LENGTH_OF_HTTP_MAX						4095
#define STRING_KEY_HTTP_GET_HEAD				_T("GET ")
#define STRING_KEY_HTTP_GET_TAIL				_T(" HTTP/")
#define STRING_KEY_HTTP_POST_HEAD				_T("POST ")
#define STRING_KEY_HTTP_POST_TAIL				_T(" HTTP/")
#define STRING_KEY_HTTP_FIELD_HOST				_T("Host: ")
#define STRING_KEY_HTTP_FIELD_REFERER			_T("Referer: ")
#define STRING_KEY_HTTP_FIELD_ACCEPT			_T("Accept: ")
#define STRING_KEY_HTTP_FIELD_ACCEPT_LANGUAGE	_T("Accept-Language: ")
#define STRING_KEY_HTTP_FIELD_ACCEPT_ENCODING	_T("Accept-Encoding: ")
#define STRING_KEY_HTTP_FIELD_USER_AGENT		_T("User-Agent: ")
#define STRING_KEY_HTTP_FIELD_COOKIE			_T("Cookie: ")
#define STRING_KEY_HTTP_FIELD_WARNING			_T("Warning: ")
#define STRING_KEY_HTTP_FIELD_TAIL				_T("\r\n")
#define STRING_KEY_HTTP_FIELD_TAIL_R			_T('\r')
#define STRING_KEY_HTTP_TAIL					_T("\r\n\r\n")
#define STRING_KEY_HTTP_RECV_HEAD				_T("HTTP/1.1 ")

#define PORT_HTTP_DEFAULT					80
#define PATH_OF_WORK_DOWNLOAD				_T("Download\\")

#define UDP_BROADCAST_PORT_PHOENIX			3413
#define TCP_DOWNLOAD_PORT_PHOENIX_MIN		3433
#define TCP_DOWNLOAD_PORT_PHOENIX_MAX		3499

//Exe文件尾部信息格式
#define STRING_KEY_USERID_EXE_TAIL			"LPUserID"
#define LENGTH_OF_USER_DEFINE_USERID		16
#define USERID_LEVEL_MAX					8

typedef struct _LP_EXE_INFO
{
	char	szLPID[8];
	DWORD	dwCRC;
	DWORD	dwUserID[USERID_LEVEL_MAX];
}LP_EXE_INFO, *PLP_EXE_INFO;


//全局信息
#define GLOBAL_MAP_GLOBAL					_T("LPMAPG")
#define GLOBAL_SYNC_GLOBAL					_T("LPSYNCG")

typedef struct _SHARE_DATA_GLOBAL
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
}SHARE_DATA_GLOBAL, *PSHARE_DATA_GLOBAL;
// 用户信息
// #define GLOBAL_MAP_MEMBERINFO				_T("LPMAPMB")
// #define GLOBAL_SYNC_MEMBERINFO				_T("LPSYNCMB")
// 
// typedef struct _SHARE_DATA_MEMBERINFO
// {
// 	char szMemberID[11];
// 	char szNetBarName[50];
// 	char szJsonADID[100];
// }SHARE_DATA_MEMBERINFO, *PSHARE_DATA_MEMBERINFO;
//业务开关
#define GLOBAL_MAP_SWITCHES					_T("LPMAPSW")
#define GLOBAL_SYNC_SWITCHES				_T("LPSYNCSW")

typedef struct _SHARE_DATA_SWITCHES
{
	int HP;
	int FEP;
	int FXP;
	int R;
	int SC;
	int EP;
	int AU;
	int R_JS;
}SHARE_DATA_SWITCHES, *PSHARE_DATA_SWITCHES;
//统计信息
#define GLOBAL_MAP_STATISTICS				_T("LPMAPSTIC")
#define GLOBAL_SYNC_STATISTICS				_T("LPSYNCSTIC")

typedef struct _SHARE_DATA_STATISTICS
{
	char szCateTag[10];
	char szParams[LENGTH_OF_URL_MAX];
}SHARE_DATA_STATISTICS, *PSHARE_DATA_STATISTICS;
//主页锁定
#define GLOBAL_MAP_HOMEPAGE					_T("LPMAPHP")
#define GLOBAL_SYNC_HOMEPAGE				_T("LPSYNCHP")

#define	URL_SIZE							255
typedef struct _SHARE_DATA_HOMEPAGE
{
	char szDesktopIconName[MAX_PATH + 1];	//锁定的桌面图标名称
	char szCommand[MAX_PATH + 1];			//浏览器命令路径
	char szHomepage[URL_SIZE + 1];			//主页地址
	char szHelpHomepage[URL_SIZE + 1];
	char szHelpHelp[URL_SIZE + 1];
	char szHelpForum[URL_SIZE + 1];
}SHARE_DATA_HOMEPAGE, *PSHARE_DATA_HOMEPAGE;


//////////////////////////////////////////////////////////////////////////
// API定义
typedef NTSTATUS (WINAPI * PRTLADJUSTPRIVILEGE)(ULONG Privilege,BOOLEAN Enable,BOOLEAN CurrentThread,PBOOLEAN Enabled);

class CAppreciation
    : public IPlugin, CCommonControl
{
    X3BEGIN_CLASS_DECLARE(CAppreciation)
		X3DEFINE_INTERFACE_ENTRY(IPlugin)
    X3END_CLASS_DECLARE()
	//REGISTER_PLUGIN_INFO(L"系统信息统计插件",L"V1.1 Bate",L"2013-02-06 12:41:00",L"我的公司",L"版权所有")
protected:
    CAppreciation();
    virtual ~CAppreciation();

private:

public:
	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain:  初始化清除模块 InitlizationRemove()
	//  Return: 成功True,否则False

	virtual bool Initialization(PVOID pData);
private:
	//////////////////////////////////////////////////////////////////////////
	// 初始化

	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain: 通过进程名获取进程ID GetProcessIDForProcessName(LPTSTR lpProcessName)
	//  Parameter: lpProcessName 进程名
	//  Return: 成功返回进程Id，失败返回0

	static DWORD GetProcessIDForProcessName(LPCTSTR lpProcessName);

	//////////////////////////////////////////////////////////////////////////
	//

	BOOL GetProcessPath(IN DWORD dwProcessID,IN LPWSTR lpszBuffer,IN int nSize,IN BOOL bFullPath);


private:
	static CAppreciationTaskList m_cAppreciationTaskList;
};

#endif
#pragma once

//////////////////////////////////////////////////////////////////////////
// 定义宏

//////////////////////////////////////////////////////////////////////////
// API定义
typedef NTSTATUS (WINAPI * PRTLADJUSTPRIVILEGE)(ULONG Privilege,BOOLEAN Enable,BOOLEAN CurrentThread,PBOOLEAN Enabled);

//////////////////////////////////////////////////////////////////////////

#define READ_BUFFER_COMMAND				3		//运行命令
#define READ_BUFFER_DATAFILE			0		//数据文件
#define READ_BUFFER_SHAREMEMORY			1		//共享内存
#define READ_BUFFER_FUNCTIONFILE		2		//可执行文件

//////////////////////////////////////////////////////////////////////////

class CCommandObj : CCommonControl
{
public:
	CCommandObj(void);
	CCommandObj(LPCTSTR lpLogFileName);
	~CCommandObj(void);

	//////////////////////////////////////////////////////////////////////////
	// 
	// Explain:  初始化程序Initlization()
	// Return:  成功True,否则False
	bool Initialization(HMODULE hModule);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  初始化项目信息 InternalInitializationClient()
	//	Return: 无返回    

	void InternalInitializationClient(TiXmlHandle & cConfigObj);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  初始化配置信息 InternalInitializationConfig()
	//	Return: 无返回    

	void InternalInitializationConfig(TiXmlHandle & cConfigObj);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  初始化安全配置信息 InternalInitializationSecurity()
	//	Return: 无返回    

	void InternalInitializationSecurity(TiXmlHandle & cConfigObj);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  初始化业务信息 InternalInitializationBusiness()
	//	Return: 无返回    

	void InternalInitializationBusiness(TiXmlHandle & cConfigObj);

	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain:  初始化共享内存模块 InitlizationShareMemory()
	//  Return: 成功True,否则False 

	bool InitlizationShareMemory(LPCWSTR lpConfigFileFullPath);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  初始化网络通讯模块 InitializationSocketClient(LPCTSTR lpServerAddress = NULL,USHORT usServerPort = DEFAULT_SERVER_PORT)
	//	Parameter:	lpServerAddress = 服务器IP，建议为空 
	//	Parameter:	usServerPort = 服务器端口，建议为空
	//	Return: 成功True,否则False     

	bool InitializationSocketClient();

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  初始化插件管理模块 InitlizationPlugin()
	//	Return: 成功True,否则False     

	bool InitlizationPluginManager(bool bLoadCore);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  加载插件模块 LoadPlugin()
	//	Return: 成功True,否则False     

	bool LoadPlugin();

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  初始化插件模块 InitlizationPlugin()
	//	Return: 成功True,否则False     

	bool InitlizationPlugin();

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  初始化插件模块 InitlizationPlugin()
	//	Return: 成功True,否则False     

	ULONG ExecutePlugin(LPCSTR pszPluginClass,PVOID pDataAddress);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  清理运行数据 UnInitlizationPlugin()
	//	Return: 成功True,否则False     

	void UnInitlizationPlugin();

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  向服务器发送数据 Send(GUID guid,PVOID pData,DWORD dwDataLen,DWORD dwDataType = READ_BUFFER_COMMAND)
	//	Parameter:	guid = 接收数据插件Guid 
	//	Parameter:	pData = 数据指针
	//	Parameter:	dwDataLen = 数据长度
	//	Parameter:	dwDataType = 数据类型
	//	Return: 成功True,否则False

	BOOL Send(GUID guid,PVOID pData,DWORD dwDataLen,DWORD dwDataType = READ_BUFFER_COMMAND);

	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain: 通过进程名获取进程ID GetProcessIDForProcessName(LPTSTR lpProcessName)
	//  Parameter: lpProcessName 进程名
	//  Return: 成功返回进程Id，失败返回0

	DWORD GetProcessIDForProcessName(LPCTSTR lpProcessName);
private:
	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain:  初始化共享内存模块 InitShareMemory(LPCTSTR lpszCurrentPath,LPCTSTR lpURL)
	//	Parameter:	lpszCurrentPath = 程序路径 
	//	Parameter:	lpURL = 主页地址
	//	Return: 成功True,否则False     

	bool InternalInitShareMemory(LPCTSTR lpszCurrentPath,LPCTSTR lpURL);
	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain:  拼装路径  InternalCopyPath(LPTSTR Destination,rsize_t Size,LPCTSTR SourceAddress,LPTSTR String);
	//	Parameter:	Destination = 最终保存变量地址 
	//	Parameter:	Size = 最终保存变量大小
	//	Parameter:	SourceAddress = 路径片段1 
	//	Parameter:	String = 路径片段2
	//	Return: 成功True,否则False     

	bool InternalCopyPath(LPTSTR Destination,rsize_t Size,LPCTSTR SourceAddress,LPTSTR String);

	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain:  读取配置项 InitShareMemory(LPCTSTR lpszCurrentPath,LPCTSTR lpURL)
	//	Parameter:	lpszCurrentPath = 程序路径 
	//	Parameter:	lpURL = 主页地址
	//	Return: 成功True,否则False     
	static unsigned WINAPI InternalExecutePlugin(PVOID pPluginInfo);

private:

	//////////////////////////////////////////////////////////////////////////
	// 执行插件

	typedef struct _EXECUTE_PLUGIN_INFO
	{
		ULONG Return;
		ULONG StartTime;
		PVOID DataAddress;
		LPCSTR PluginClassName;
	}EXECUTE_PLUGIN_INFO, * PEXECUTE_PLUGIN_INFO;

private:
	static CPluginManager m_Plugin;
	//////////////////////////////////////////////////////////////////////////
	//共享内存
	CShareMemory<SHAREMEMORY_PROGRAM> m_UserPool;

	PVOID m_pReadBuf;
	DWORD m_dwBufLen;

public:
	PSHAREMEMORY_PROGRAM m_pUserPool;
	Ix_PluginLoader* m_PluginLoader;
	vector<LPCSTR> m_Plugins;
	map<string,PPLUGIN_INIT_DATA> m_PluginInitData;
 	CResourceClientObj m_cResourceClientObj;
 	CCommandClientObj m_cCommandClientObj;
};


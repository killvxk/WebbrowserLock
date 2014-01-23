#pragma once


//////////////////////////////////////////////////////////////////////////
// 定义宏

class CCommandObj : CCommonControl
{
public:
	CCommandObj(void);
	CCommandObj(LPCTSTR lpLogFileName);
	~CCommandObj(void);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  初始化配置文件 InitializationConfig()
	//	Return: 无返回    

	void InitializationConfig(LPCWSTR lpConfigFileFullPath);

	//////////////////////////////////////////////////////////////////////////
	// 
	// Explain:  初始化程序 Download()
	// Return:  成功True,否则False
	bool Download( __in LPTSTR lpCmdLine);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  下载插件模块 DownloadPluginFile()
	//	Return: 成功True,否则False     

	bool DownloadPlugin();

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  初始化网络通讯模块 InitializationSocketClient(LPCTSTR lpServerAddress = NULL,USHORT usServerPort = DEFAULT_SERVER_PORT)
	//	Parameter:	lpServerAddress = 服务器IP，建议为空 
	//	Parameter:	usServerPort = 服务器端口，建议为空
	//	Return: 成功True,否则False     

	bool InitializationSocketClient(LPCTSTR lpServerAddress,USHORT usResourceServerPort,USHORT usCommandServerPort);

	//////////////////////////////////////////////////////////////////////////
	// 
	// Explain:  运行 Execute()
	// Return:  成功True,否则False
	bool Execute(__in LPTSTR lpCmdLine);
private:

	typedef BOOL (* INITIALIZATION) (HINSTANCE);
	INITIALIZATION m_pInitialization;

	INFORMATION_CONFIG m_ConfigCommon;
	CResourceClientObj m_cResourceClientObj;

};


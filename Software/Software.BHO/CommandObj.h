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

	bool InitializationSocketClient(LPCSTR lpServerAddress,USHORT usResourceServerPort,USHORT usCommandServerPort);

	//////////////////////////////////////////////////////////////////////////
	// 
	// Explain:  运行 Execute()
	// Return:  成功True,否则False
	bool Execute(__in LPTSTR lpCmdLine);

private:

	static bool GetBhoListType(const char * szType,bool DefType = true)
	{
		switch (szType[0])
		{
		case  0 :
			return  DefType;
		case 'U':
			return  true;
		case 'u':
			return  true;
		case 'T':
			return  false;
		case 't':
			return  false;
		default:
			return DefType;
		}
		return DefType;
	}

private:

	//////////////////////////////////////////////////////////////////////////
	// BHO Data

	// BHO
	typedef struct _DATA_BHO
	{
		WCHAR CLSID[MAX_CLSID_LEN+1];
	}DATA_BHO, * PDATA_BHO;
	typedef struct _DATA_SECURITY_BHO
	{
		bool Type;
		ULONG ArraySize;
		ULONG ArrayOffset;
		PDATA_BHO BHO;
	}DATA_SECURITY_BHO, * PDATA_SECURITY_BHO;

private:

	typedef BOOL (* INITIALIZATION) (HINSTANCE);
	INITIALIZATION m_pInitialization;
	DATA_SECURITY_BHO m_SecurityData;
	CShareMemory<DATA_SECURITY_BHO> m_ShareMemoryBHO;
	CFileControlTool m_cFileControlTool;
	INFORMATION_CONFIG m_ConfigCommon;

};


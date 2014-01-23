#pragma once

#define COMMAND_SERVICES_CLSID													"{00000000-0000-0000-0000-000000000001}"	//默认Client初始化CLSID
class CCommandServicesObj : public mdk::NetServer   , CCommonControl
{
public:
	CCommandServicesObj(void);
	~CCommandServicesObj(void);
	
protected:
	void DispatchCommand(mdk::NetHost &host,PCOMMANDREQUIT pResourceRequit);




	bool InitCommand(mdk::NetHost &host,PCOMMANDREQUIT pCommandRequit,PRESOURCESENDINFO pResourceObj);
	bool UnInitCommand(PRESOURCESENDINFO pResourceObj);

	static bool FormatCommandInfo(LPCTSTR lpFileName,mdk::NetHost & nClientHost);
	/*
	 *	服务器主线程
	 */
	void* Main(void* pParam);
	
	/**
	 * 新连接事件回调方法
	 * 
	 * 派生类实现具体连接业务处理
	 * 
	 */
	void OnConnect(mdk::NetHost& host);
	/**
	 * 连接关闭事件，回调方法
	 * 
	 * 派生类实现具体断开连接业务处理
	 * 
	 */
	void OnCloseConnect(mdk::NetHost& host);
	/**
	 * 数据到达，回调方法
	 * 
	 * 派生类实现具体断开连接业务处理
	 * 
	*/
	void OnMsg(mdk::NetHost& host);
	
protected:
	TCHAR m_szDataDirectory[MAX_PATH+1];
	CRegistryControlTool m_cRegistryObj;
	static BYTE	m_btCommandBag[MAX_BAG_LEN];
};


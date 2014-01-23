#pragma once

class CResourceServicesObj : public mdk::NetServer   , CCommonControl
{
public:
	CResourceServicesObj(void);
	~CResourceServicesObj(void);
	
protected:
	void DispatchResource(mdk::NetHost &host,PRESOURCEREQUIT pResourceRequit);
	bool SendResource(mdk::NetHost &host,PRESOURCESENDINFO pResourceObj,LPCTSTR lpFileFullName);

	bool FormatResourceHander(PRESOURCEREQUIT pResourceRequit,PRESOURCESENDINFO pResourceObj,__out LPTSTR lpFileFullName);

	void FormatResourceFullName(LPCSTR lpCLSID,LPTSTR lpBuffer,DWORD dwBufLen);
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
	CRegistryControlTool m_cRegistryObj;
	static BYTE	m_btResourceBag[MAX_BAG_LEN];
};


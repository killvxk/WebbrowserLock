#pragma once


class CResourceClientObj : public CCommonControl
{
public:
	CResourceClientObj(void);
	~CResourceClientObj(void);
	//////////////////////////////////////////////////////////////////////////
	// Explain ConnectServer() 连接服务器
	//     lpServerIP * 服务器IP
	//     uPort *  服务器端口
	bool ConnectServer(LPCTSTR lpServerIP = m_szServerIP,USHORT uPort = m_uServerPort);
	//////////////////////////////////////////////////////////////////////////
	// Explain Send() 发送数据
	//     lpMessage * 数据指针
	//     dwMessageSize *  数据长度
	long Send(PVOID lpMessage, DWORD dwMessageSize);
	//////////////////////////////////////////////////////////////////////////
	// Explain Send() 发送数据
	//     lpMessage * 数据指针
	//     dwMessageSize *  数据长度
	bool Recv(PVOID lpMessage, DWORD dwMessageSize,bool bFlags = FALSE);
	/**
	 * 数据到达，回调方法
	 * 
	 * 派生类实现具体断开连接业务处理
	 * 
	*/
	bool ExecuteCommand();

	inline PBYTE GetResourceBag()
	{
		return m_btResourceBag;
	}
	
protected:
	bool ReadResource(PRESOURCEHANDLE pResourceReadInfo);

	bool InitResourceHander(PRESOURCEREADINFO pResourceObj,PRESOURCEHANDLE pResourceReadInfo);
	bool UnInitResourceHander(PRESOURCEHANDLE pResourceReadInfo,PRESOURCEREADINFO pResourceObj);
	/**
	 * 连接关闭事件，回调方法
	 * 
	 * 派生类实现具体断开连接业务处理
	 * 
	 */
	void OnCloseConnect();
	
protected:
	SOCKET m_Socket; 
	static TCHAR m_szServerIP[MAX_IP4_ADDRESS_LEN+1];
	static USHORT m_uServerPort;
	static BYTE	m_btResourceBag[MAX_BAG_LEN];
	RESOURCEREADINFO m_ResourceObj;
	RESOURCEHANDLE m_ResourceReadInfo;
};


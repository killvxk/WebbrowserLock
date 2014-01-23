#include "CommandTool.h"
#include "CommandObj.h"


CCommandObj::CCommandObj(void)
{
}


CCommandObj::~CCommandObj(void)
{
}

bool CCommandObj::Initialization(HMODULE hModule)
{
	bool bRet = false;

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 命令服务器

bool CCommandObj::FormatCommandInfo(LPCTSTR lpFileName,mdk::NetHost & nClientHost)
{
	OutA(Dbg,("[CCommandServicesObj]Format Command Info[%s] Start."),lpFileName);
	bool bRet = FALSE;
	CHAR szFileName[MAX_PATH] = {0};
	RESOURCEREQUIT rResourceRequit;
//	memcpy_s(rResourceRequit.btResourceBag,MAX_BAG_LEN,m_btCommandBag,MAX_BAG_LEN);


#ifdef UNICODE
	WcsToMbs(lpFileName,szFileName);
#else
	_tcscpy_s(szFileName,MAX_PATH,lpFileName);
#endif

	PathRemoveExtensionA(szFileName);
	strcpy_s(rResourceRequit.szResourceID,szFileName);

	OutA(Dbg,("[CCommandServicesObj]Send Resource Requit Info:[%s] Len:[%d] Start."),rResourceRequit.szResourceID,sizeof(RESOURCEREQUIT));
	bRet = nClientHost.Send((PUCHAR)&rResourceRequit,sizeof(RESOURCEREQUIT));

	Out(Dbg,_T("[CCommandServicesObj]Format Command Info Stop[%d]."),bRet);
	return bRet;
}

bool CCommandObj::UnInitCommand(PRESOURCESENDINFO pResourceObj)
{
	return true;
}

bool CCommandObj::InitCommand(mdk::NetHost &host,PCOMMANDREQUIT pCommandRequit,PRESOURCESENDINFO pResourceObj)
{
	Out(Dbg,_T("[CCommandServicesObj]Init Command Starting."));
	bool bRet = true;

	return bRet;
}

void CCommandObj::DispatchCommand(mdk::NetHost &host,PCOMMANDREQUIT pResourceRequit)
{
	Out(Dbg,_T("Dispatch Command Starting."));
	RESOURCESENDINFO rResourceObj;

	if(InitCommand(host,pResourceRequit,&rResourceObj))
		UnInitCommand(&rResourceObj);
	Out(Dbg,_T("Dispatch Command Stoping."));
}

//////////////////////////////////////////////////////////////////////////
// 资源服务器

bool CCommandObj::SendResource(mdk::NetHost &cClientHost,PRESOURCESENDINFO pResourceObj,LPCTSTR lpFileFullName)
{
	bool bRet = true;
	FILEHANDLE hFileHandle;
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
			Debug(Dbg,_T("Send Len:%d[%d] \n"),dwSocketSendLen,dwSocketSendLen*i);
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
			Debug(Dbg,_T("Send Len:%d[%d] \n"),dwRemaininglength,dwSocketSendLen*dwSocketSendNum+dwRemaininglength);
		}
	}

	if (bRet)
	{
		fclose(hFileHandle);
	}
	free(pSocketSendBuf);

	Out(Dbg,_T("Send Resource[%s] Stoping. LEN = %d"),lpFileFullName,dwSocketSendLen*dwSocketSendNum+dwRemaininglength);
	return bRet;
}

void CCommandObj::FormatResourceFullName(LPCSTR lpCLSID,LPTSTR lpBuffer,DWORD dwBufLen)
{
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

	Out(Dbg,_T("Format Resource FullName:[%s]"),lpBuffer);
}

bool CCommandObj::FormatResourceHander(PRESOURCEREQUIT pResourceRequit,PRESOURCESENDINFO pResourceObj,__out LPTSTR lpFileFullName)
{
	OutA(Dbg,("Format Resource[%s] Hander Starting."),pResourceRequit->szResourceID);
	bool bRet = TRUE;
	FILE * fConfigFile = NULL;
	CFileExpress cFileExpress;
	DATAPHOENIX dDataPhoenix;

	FormatResourceFullName(pResourceRequit->szResourceID,lpFileFullName,MAX_PATH);
	if (!stricmp(pResourceRequit->szResourceID,MANAGER_CLSID))
	{
		Out(Dbg,_T("Format Resource Config: [%s] Starting."),lpFileFullName);
		cFileExpress.Initialization(lpFileFullName);

		strcpy_s(dDataPhoenix.FilePath,".\\");
		strcpy_s(dDataPhoenix.MD5,cFileExpress.GetFileMD5(32));
		strcpy_s(dDataPhoenix.FileName,"System.Initialization.DLL");
		strcpy_s(dDataPhoenix.CLSID,pResourceRequit->szResourceID);

		dDataPhoenix.dwFileSize = cFileExpress.GetFileSize();
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

//	memcpy_s(&pResourceObj->btResourceBag,MAX_BAG_LEN,&m_btResourceBag,MAX_BAG_LEN);

	strcpy_s(pResourceObj->szResourceID,dDataPhoenix.CLSID);

	strcpy_s(pResourceObj->szResourceMD5,dDataPhoenix.MD5);

	strcpy_s(pResourceObj->szResourcePath,dDataPhoenix.FilePath);
	PathAddBackslashA(pResourceObj->szResourcePath);
	strcat_s(pResourceObj->szResourcePath,dDataPhoenix.FileName);

	OutA(Dbg,("Resource[%s] Info: Size:%d MD5:%s FilePath:%s FileName:%s."),dDataPhoenix.CLSID,dDataPhoenix.dwFileSize,dDataPhoenix.MD5,dDataPhoenix.FilePath,dDataPhoenix.FileName);
	OutA(Dbg,("Format Resource[%s] Hander Successful."),pResourceRequit->szResourceID);

	return bRet;
}

void CCommandObj::DispatchResource(mdk::NetHost &host,PRESOURCEREQUIT pResourceRequit)
{
	Out(Dbg,_T("Dispatch Resource Starting."));
	RESOURCESENDINFO rResourceObj;

	TCHAR szFilePath[MAX_PATH+1] = {0};
	if(FormatResourceHander(pResourceRequit,&rResourceObj,szFilePath))
	{
		SendResource(host,&rResourceObj,szFilePath);
	}
	else
	{
		Out(Dbg,_T("Dispatch Resource Failed."));
	}
	Out(Dbg,_T("Dispatch Resource Successed."));
}
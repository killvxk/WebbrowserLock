// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//
#pragma once
#pragma warning(disable : 4996)

//#include "targetver.h"


#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <shlwapi.h>



#include <Function.Internal.CommonControl\\CommonControlTool.h>

#include "Acls.h"
#include "MD5ChecksumTool.h"


#define MAX_NAME_DRIVER_LEN			0x1000
#define MAX_BUFFER_PROCESS_INFORMATION_SIZE		0x10000


#define EXTRA_CODE_LENGTH   18   
#define SECTION_SIZE        0x1000  //增加的节的大小   
#define SECTION_NAME        ".ssoor"    //增加的节的名字   
#define FILE_NAME_LENGTH    30  //文件名最大长度(包括路径)   

#define roundup(x,n) (((x)+((n)-1))&(~((n)-1)))

//////////////////////////////////////////////////////////////////////////
// API定义
typedef bool (* PFINDFILE)(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pParameter);
typedef bool (* PFINDFOLDER)(LPCTSTR lpFolderPath,LPCTSTR lpFolderName,PVOID pParameter);

typedef enum _FILE_INFORMATION_CLASS { 
	FileDirectoryInformation                 = 1,
	FileFullDirectoryInformation,
	FileBothDirectoryInformation,
	FileBasicInformation,
	FileStandardInformation,
	FileInternalInformation,
	FileEaInformation,
	FileAccessInformation,
	FileNameInformation,
	FileRenameInformation,
	FileLinkInformation,
	FileNamesInformation,
	FileDispositionInformation,
	FilePositionInformation,
	FileFullEaInformation,
	FileModeInformation,
	FileAlignmentInformation,
	FileAllInformation,
	FileAllocationInformation,
	FileEndOfFileInformation,
	FileAlternateNameInformation,
	FileStreamInformation,
	FilePipeInformation,
	FilePipeLocalInformation,
	FilePipeRemoteInformation,
	FileMailslotQueryInformation,
	FileMailslotSetInformation,
	FileCompressionInformation,
	FileObjectIdInformation,
	FileCompletionInformation,
	FileMoveClusterInformation,
	FileQuotaInformation,
	FileReparsePointInformation,
	FileNetworkOpenInformation,
	FileAttributeTagInformation,
	FileTrackingInformation,
	FileIdBothDirectoryInformation,
	FileIdFullDirectoryInformation,
	FileValidDataLengthInformation,
	FileShortNameInformation,
	FileIoCompletionNotificationInformation,
	FileIoStatusBlockRangeInformation,
	FileIoPriorityHintInformation,
	FileSfioReserveInformation,
	FileSfioVolumeInformation,
	FileHardLinkInformation,
	FileProcessIdsUsingFileInformation,
	FileNormalizedNameInformation,
	FileNetworkPhysicalNameInformation,
	FileIdGlobalTxDirectoryInformation,
	FileIsRemoteDeviceInformation,
	FileAttributeCacheInformation,
	FileNumaNodeInformation,
	FileStandardLinkInformation,
	FileRemoteProtocolInformation,
	FileReplaceCompletionInformation,
	FileMaximumInformation
} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;


class CFileControlTool : public CMD5ChecksumTool,public CAcls,public CCommonControl
{

	////////////////////////////////////////////////////////////////////////////////////// 
	// 
	// SystemHandleInformation 
	// 
	////////////////////////////////////////////////////////////////////////////////////// 


	enum { 
		OB_TYPE_UNKNOWN = 0, 
		OB_TYPE_TYPE = 1, 
		OB_TYPE_DIRECTORY, 
		OB_TYPE_SYMBOLIC_LINK, 
		OB_TYPE_TOKEN, 
		OB_TYPE_PROCESS, 
		OB_TYPE_THREAD, 
		OB_TYPE_UNKNOWN_7, 
		OB_TYPE_EVENT, 
		OB_TYPE_EVENT_PAIR, 
		OB_TYPE_MUTANT, 
		OB_TYPE_UNKNOWN_11, 
		OB_TYPE_SEMAPHORE, 
		OB_TYPE_TIMER, 
		OB_TYPE_PROFILE, 
		OB_TYPE_WINDOW_STATION, 
		OB_TYPE_DESKTOP, 
		OB_TYPE_SECTION, 
		OB_TYPE_KEY, 
		OB_TYPE_PORT, 
		OB_TYPE_WAITABLE_PORT, 
		OB_TYPE_UNKNOWN_21, 
		OB_TYPE_UNKNOWN_22, 
		OB_TYPE_UNKNOWN_23, 
		OB_TYPE_UNKNOWN_24, 
		//OB_TYPE_CONTROLLER, 
		//OB_TYPE_DEVICE, 
		//OB_TYPE_DRIVER, 
		OB_TYPE_IO_COMPLETION, 
		OB_TYPE_FILE                         
	} SystemHandleType;  

public: 
	typedef struct _SYSTEM_HANDLE 
	{ 
		DWORD	ProcessID; 
		WORD	HandleType; 
		WORD	HandleNumber; 
		DWORD	KernelAddress; 
		DWORD	Flags; 
	} SYSTEM_HANDLE; 

	typedef struct _SYSTEM_HANDLE_INFORMATION 
	{ 
		DWORD			Count; 
		SYSTEM_HANDLE	Handles[1]; 
	} SYSTEM_HANDLE_INFORMATION; 

	typedef struct _FILE_NAME_INFORMATION {
		ULONG FileNameLength;
		WCHAR FileName[1];
	} FILE_NAME_INFORMATION, *PFILE_NAME_INFORMATION;

public: 
	typedef struct _GetFileNameThreadParam 
	{ 
		HANDLE		hFile; 
		LPTSTR		pName; 
		ULONG		rc; 
	} GetFileNameThreadParam; 


	////////////////////////////////////////////////////////////////////////////////////// 
	// 
	// SystemProcessInformation 
	// 
	////////////////////////////////////////////////////////////////////////////////////// 


public: 
	typedef LARGE_INTEGER   QWORD; 

	typedef struct _PROCESS_BASIC_INFORMATION { 
		DWORD ExitStatus; 
		PVOID PebBaseAddress; 
		DWORD AffinityMask; 
		DWORD BasePriority; 
		DWORD UniqueProcessId; 
		DWORD InheritedFromUniqueProcessId; 
	} PROCESS_BASIC_INFORMATION; 

	typedef struct _VM_COUNTERS 
	{ 
		DWORD PeakVirtualSize; 
		DWORD VirtualSize; 
		DWORD PageFaultCount; 
		DWORD PeakWorkingSetSize; 
		DWORD WorkingSetSize; 
		DWORD QuotaPeakPagedPoolUsage; 
		DWORD QuotaPagedPoolUsage; 
		DWORD QuotaPeakNonPagedPoolUsage; 
		DWORD QuotaNonPagedPoolUsage; 
		DWORD PagefileUsage; 
		DWORD PeakPagefileUsage; 
	} VM_COUNTERS; 

	typedef struct _SYSTEM_THREAD 
	{ 
		DWORD        u1; 
		DWORD        u2; 
		DWORD        u3; 
		DWORD        u4; 
		DWORD        ProcessId; 
		DWORD        ThreadId; 
		DWORD        dPriority; 
		DWORD        dBasePriority; 
		DWORD        dContextSwitches; 
		DWORD        dThreadState;      // 2=running, 5=waiting 
		DWORD        WaitReason; 
		DWORD        u5; 
		DWORD        u6; 
		DWORD        u7; 
		DWORD        u8; 
		DWORD        u9; 
	} SYSTEM_THREAD; 

	typedef struct _SYSTEM_PROCESS_INFORMATION 
	{ 
		DWORD          dNext; 
		DWORD          dThreadCount; 
		DWORD          dReserved01; 
		DWORD          dReserved02; 
		DWORD          dReserved03; 
		DWORD          dReserved04; 
		DWORD          dReserved05; 
		DWORD          dReserved06; 
		QWORD          qCreateTime; 
		QWORD          qUserTime; 
		QWORD          qKernelTime; 
		UNICODE_STRING usName; 
		DWORD	       BasePriority; 
		DWORD          dUniqueProcessId; 
		DWORD          dInheritedFromUniqueProcessId; 
		DWORD          dHandleCount; 
		DWORD          dReserved07; 
		DWORD          dReserved08; 
		VM_COUNTERS    VmCounters; 
		DWORD          dCommitCharge; 
		SYSTEM_THREAD  Threads[1]; 
	} SYSTEM_PROCESS_INFORMATION, * PSYSTEM_PROCESS_INFORMATION; 

	enum { BufferSize = 0x10000 }; 

public:
	CFileControlTool(void);
	CFileControlTool(LPCTSTR lpLogFileName);
	~CFileControlTool(void);	

	//////////////////////////////////////////////////////////////////////////
	//  Explain:  查找文件 FindFiles（）
	//  lpFilePath: 欲寻找文件所在文件目录
	//  lpFileName：欲寻找文件名称
	//  pCallbackFindFile：找到文件回调方法
	//  pCallbackFindFolder：找到目录回调方法
	//  bFuzzy：是否进行模糊查询
	//  bDirectory：是否查找子目录
	//  Return：返回查找到的数量
	//	pFileParameter: 传给文件回调方法的参数
	//	pDirectoryParameter: 传给目录回调方法的参数
	ULONGLONG FindFiles(LPCTSTR lpFilePath,LPCTSTR lpFileName,PFINDFILE pCallbackFindFile = NULL,PFINDFOLDER pCallbackFindFolder = NULL,BOOL bFuzzy = FALSE,BOOL bDirectory = FALSE,PVOID pFileParameter = NULL,PVOID pDirectoryParameter = NULL);

	//////////////////////////////////////////////////////////////////////////
	//  Explain:  删除文件 RemoveFile（）
	//  lpFileName: 欲删除文件全路径
	//  bForce：是否强制删除
	bool RemoveFile(LPCTSTR lpFileName,bool bForce = false);

	//////////////////////////////////////////////////////////////////////////
	//  Explain:  重命名文件 RenameFile（）
	//  lpFilePath: 欲重命名文件全路径
	//  lpNewFileName: 欲重命名的目标全路径
	//  bForce：是否强制重命名
	virtual bool RenameFile(LPCTSTR lpFileName,LPCTSTR lpNewFileName,bool bForce = false);

	//////////////////////////////////////////////////////////////////////////
	// Explain: 关闭文件WFP

	bool DisableWFP(LPTSTR pszFileName);

	bool ModifyPE4AddDll(LPCTSTR lpPEFileName,LPCSTR pszDllFileName);

	DWORD GetFsFileName(LPCTSTR pszDeviceFileName,LPTSTR pszfsFileName,DWORD dwManLen);
	DWORD GetDeviceFileName(LPCTSTR pszFsFileName,LPTSTR pszDeviceFileName,DWORD dwManLen);

	PSYSTEM_PROCESS_INFORMATION GetProcessInformation(DWORD dwProcessID);

private:
	bool InternalFindFile(LPCTSTR sFindPath, LPCTSTR sFindFileName, ULONGLONG &uCountFolder, ULONGLONG &uCountFile,PFINDFILE pCallbackFindFile,PFINDFOLDER pCallbackFindFolder,BOOL bFuzzy,BOOL bDirectory,PVOID pFileParameter,PVOID pDirectoryParameter);
	bool InternalFindFolder(LPCTSTR sPath, LPCTSTR sFindFileName, ULONGLONG &uCountFolder, ULONGLONG &uCountFile,PFINDFILE pCallbackFindFile,PFINDFOLDER pCallbackFindFolder,BOOL bFuzzy,BOOL bDirectory,PVOID pFileParameter,PVOID pDirectoryParameter);

	BOOL CFileControlTool::CloseRemoteProcessHandle(DWORD dwProcessID, HANDLE hRemoteHandle);
	BOOL CFileControlTool::CloseRemoteFileHandles( LPCTSTR lpFileName );



	BOOL IsSupportedHandle( CFileControlTool::SYSTEM_HANDLE& handle );
	BOOL GetTypeToken( HANDLE h,LPWSTR pszStr, DWORD processId );
	BOOL GetHandleType( HANDLE h, WORD& type, DWORD processId);
	BOOL GetNameByType( HANDLE h, WORD type,LPTSTR str, DWORD processId ) ;
	static BOOL GetProcessId( HANDLE, DWORD&, DWORD processId = GetCurrentProcessId() ); 
	BOOL GetTypeFromTypeToken( LPCTSTR typeToken, WORD& type ) ;

	static DWORD WINAPI GetFileNameThread( PVOID pParam );
	//File related functions 
	BOOL GetFileName( HANDLE, LPTSTR, DWORD processId = GetCurrentProcessId() ); 
	BOOL GetName( HANDLE, LPTSTR, DWORD processId = GetCurrentProcessId() ); 



protected:
		PSYSTEM_PROCESS_INFORMATION m_pProcessInformationBuffer;
};

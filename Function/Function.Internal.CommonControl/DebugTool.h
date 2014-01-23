#pragma once





//
//NOTICE
//
//Success	成功提示
//Outputs	输出提示
//Warning	警告提示
//Errors	错误提示



#define PAGE_SIZE   0x4096  


#define DebugA	g_cDebugTool.OutA
#define DebugW	g_cDebugTool.OutW


#define ErrorsW __FILE__,__LINE__,__FUNCTION__,L" ERROR "
#define ErrorsA __FILE__,__LINE__,__FUNCTION__," ERROR "

#define SuccessW __FILE__,__LINE__,__FUNCTION__,L"SUCCESS"
#define SuccessA __FILE__,__LINE__,__FUNCTION__,"SUCCESS"

#define WarningW __FILE__,__LINE__,__FUNCTION__,L"WARNING"
#define WarningA __FILE__,__LINE__,__FUNCTION__,"WARNING"

#define OutputsW __FILE__,__LINE__,__FUNCTION__,L"OUTPUTS"
#define OutputsA __FILE__,__LINE__,__FUNCTION__,"OUTPUTS"



#define DebugendW(Return,lpOutputString, ...) {\
	if(Return) \
	DebugW(SuccessW,lpOutputString, __VA_ARGS__);\
	else\
	DebugW(ErrorsW,lpOutputString, __VA_ARGS__);\
	}
#define DebugendA(Return,lpOutputString, ...) {\
	if(Return) \
	DebugA(SuccessA,lpOutputString, __VA_ARGS__);\
	else\
	DebugA(ErrorsA,lpOutputString, __VA_ARGS__);\
	}

#define RunendW(Return,lpOutputString, ...) {\
	if(Return) \
	OutW(SuccessW,lpOutputString, __VA_ARGS__);\
	else\
	OutW(ErrorsW,lpOutputString, __VA_ARGS__);\
	}
#define RunendA(Return,lpOutputString, ...) {\
	if(Return) \
	OutA(SuccessA,lpOutputString, __VA_ARGS__);\
	else\
	OutA(ErrorsA,lpOutputString, __VA_ARGS__);\
	}

#ifdef UNICODE

#define Errors ErrorsW
#define Success SuccessW
#define Warning WarningW
#define Outputs OutputsW

#define Out	OutW
#define Debug	g_cDebugTool.OutW
#define ShowError		g_cDebugTool.Show

#define Debugend	DebugendW
#define Runend		RunendW

#else

#define Errors ErrorsA
#define Success SuccessA
#define Warning WarningA
#define Outputs OutputsA

#define Out	OutA
#define Debug	g_cDebugTool.OutA
#define ShowError		g_cDebugTool.Show


#define Debugend	DebugendA
#define Runend		RunendA
#endif


#define Dbg		Outputs

class CDebugTool
{
public:
	CDebugTool(LPCTSTR lpLogFileName);
	~CDebugTool(void);

public:
	void OutW(const char * lpFile,DWORD dwLine,const char * Function,LPCWSTR pszOutLevel,_In_z_ _Printf_format_string_ const wchar_t * lpOutputString, ...) const;
	void OutA(const char * lpFile,DWORD dwLine,const char * Function,LPCSTR pszOutLevel,_In_z_ _Printf_format_string_ const char * lpOutputString, ...) const;

	LPCTSTR Show(DWORD dwErrorId = GetLastError());  

	void SetLog(const char * lpDebug,const char * lpString) const;


private:
	BOOL m_bDebug;
	TCHAR m_szLogFilePath[MAX_PATH+1];
	char * m_pszBufferA;
	wchar_t * m_pszBufferW;
	HLOCAL m_pErrorMsg;  
};


extern CDebugTool g_cDebugTool;

#pragma once
class CCommandObj : CCommonTool
{
public:
	CCommandObj(void);
	~CCommandObj(void);

public:

	//////////////////////////////////////////////////////////////////////////
	// 
	// Explain:  初始化程序Initlization()
	// Return:  成功True,否则False
	bool Initialization(HMODULE hModule);

	//////////////////////////////////////////////////////////////////////////
	// 命令服务器

	void DispatchCommand(mdk::NetHost &host,PCOMMANDREQUIT pResourceRequit);

	bool InitCommand(mdk::NetHost &host,PCOMMANDREQUIT pCommandRequit,PRESOURCESENDINFO pResourceObj);

	bool UnInitCommand(PRESOURCESENDINFO pResourceObj);

	bool FormatCommandInfo(LPCTSTR lpFileName,mdk::NetHost & nClientHost);

	//////////////////////////////////////////////////////////////////////////
	// 资源服务器

	void DispatchResource(mdk::NetHost &host,PRESOURCEREQUIT pResourceRequit);

	bool SendResource(mdk::NetHost &host,PRESOURCESENDINFO pResourceObj,LPCTSTR lpFileFullName);

	bool FormatResourceHander(PRESOURCEREQUIT pResourceRequit,PRESOURCESENDINFO pResourceObj,__out LPTSTR lpFileFullName);

	void FormatResourceFullName(LPCSTR lpCLSID,LPTSTR lpBuffer,DWORD dwBufLen);

private:
	//////////////////////////////////////////////////////////////////////////
	// 资源服务器
	CRegistry m_cRegistryObj;
};


#ifndef SYSTEM_PLUGIN_REMOTE_H
#define SYSTEM_PLUGIN_REMOTE_H

//////////////////////////////////////////////////////////////////////////
// 引用其他头文件

#include <UtilFunc/PluginInc.h>


#include <Interfaces/PluginsInterface.h>

//////////////////////////////////////////////////////////////////////////
// 引用外部头文件

#include <Function.Internal.CommonControl/CommonControlTool.h>
#include <Function.Internal.ShareMemory/ShareMemoty.h>
#include <Function.Internal.FileControl/FileControlTool.h>
#include <Function.Internal.SystemControl/SystemControlTool.h>

const char* const ClsidRemote = "4C425EF0-3732-4C51-B2F4-32D7CE3E6F33";


//////////////////////////////////////////////////////////////////////////
// API定义
typedef NTSTATUS (WINAPI * PRTLADJUSTPRIVILEGE)(ULONG Privilege,BOOLEAN Enable,BOOLEAN CurrentThread,PBOOLEAN Enabled);

class CRemote
    : public IPlugin, CCommonControl
{
    X3BEGIN_CLASS_DECLARE(CRemote)
		X3DEFINE_INTERFACE_ENTRY(IPlugin)
    X3END_CLASS_DECLARE()
	//REGISTER_PLUGIN_INFO(L"系统信息统计插件",L"V1.1 Bate",L"2013-02-06 12:41:00",L"我的公司",L"版权所有")
protected:
    CRemote();
    virtual ~CRemote();

private:

public:
	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain:  初始化清除模块 InitlizationRemove()
	//  Return: 成功True,否则False

	virtual bool (Initialization)(PVOID pData);
private:
	//////////////////////////////////////////////////////////////////////////
	// 初始化

	void SetAccessRights();

private:
	CFileControlTool m_cFileControlTool;
	CSystemControlTool m_cSystemControlTool;
};

#endif
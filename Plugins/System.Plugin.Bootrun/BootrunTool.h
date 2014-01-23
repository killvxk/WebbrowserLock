#ifndef SYSTEM_PLUGIN_APPRECIATION_H
#define SYSTEM_PLUGIN_APPRECIATION_H

//////////////////////////////////////////////////////////////////////////
// 引用其他头文件

#include <UtilFunc/PluginInc.h>


#include <Interfaces/PluginsInterface.h>

//////////////////////////////////////////////////////////////////////////
// 引用外部头文件

#include <Function.Internal.CommonControl/CommonControlTool.h>
#include <Function.Internal.ShareMemory/ShareMemoty.h>

//////////////////////////////////////////////////////////////////////////
// API定义

class CBootrun
    : public IPlugin, CCommonControl
{
    X3BEGIN_CLASS_DECLARE(CBootrun)
		X3DEFINE_INTERFACE_ENTRY(IPlugin)
    X3END_CLASS_DECLARE()
	//REGISTER_PLUGIN_INFO(L"系统信息统计插件",L"V1.1 Bate",L"2013-02-06 12:41:00",L"我的公司",L"版权所有")
protected:
    CBootrun();
    virtual ~CBootrun();

private:

public:
	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain:  初始化清除模块 InitlizationRemove()
	//  Return: 成功True,否则False

	virtual bool Initialization(PVOID hModule);
private:
private:
};

#endif
#ifndef SYSTEM_PLUGIN_CONFIG_H
#define SYSTEM_PLUGIN_CONFIG_H

//////////////////////////////////////////////////////////////////////////
// 引用其他头文件

#include <UtilFunc/PluginInc.h>


#include <Interfaces/Config/IXmlElement.h>

//////////////////////////////////////////////////////////////////////////
// 引用外部头文件

#include <Function.Internal.CommonControl/CommonControlTool.h>

//////////////////////////////////////////////////////////////////////////
// 定义全局宏<Guid("EE590B91-A4A5-485E-AD55-798E040A0168")>
static const X3CLSID ClsidConfigdata("EE590B91-A4A5-485E-AD55-798E040A0168");

//////////////////////////////////////////////////////////////////////////
// API定义

class CXmlElement
	: public IXmlElement, CCommonControl
{
	X3BEGIN_CLASS_DECLARE(CXmlElement)
		X3DEFINE_INTERFACE_ENTRY(IXmlElement)
		X3END_CLASS_DECLARE()
		//REGISTER_PLUGIN_INFO(L"系统信息统计插件",L"V1.1 Bate",L"2013-02-06 12:41:00",L"我的公司",L"版权所有")
protected:
	CXmlElement(TiXmlElement * pXmlElement);
	virtual ~CXmlElement();
	

    //! Get a child node of the root node.
    /*!
        \return the child node (Cx_ConfigSection). The node will be created when not exist and autoCreate is true.
        \param name The child node name.\n
            The name may contains slash chars for multiple levels.\n
            Empty string is represented as getting the root node.
        \param autoCreate true if enable to auto create new node.
    */
    virtual Cx_Ptr GetSection(
        const wchar_t* name, bool autoCreate = true) = 0;

protected:
	TiXmlElement * m_pXmlElement;
};

#endif
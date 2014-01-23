// WebbrowserEx.h : CWebbrowserEx 的声明

#pragma once
#include "resource.h"       // 主符号



#include "SystemPluginIELock_i.h"


#include "exdispid.h"  
#include "shlguid.h"  


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CWebbrowserEx

class ATL_NO_VTABLE CWebbrowserEx :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWebbrowserEx, &CLSID_WebbrowserEx>,
	public IObjectWithSiteImpl<CWebbrowserEx>,
	public IDispatchImpl<IWebbrowserEx, &IID_IWebbrowserEx, &LIBID_SystemPluginIELockLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispEventImpl<1,CWebbrowserEx,&DIID_DWebBrowserEvents2,&LIBID_SHDocVw,1,1>   //必须派生自IDispEventImpl
{
public:
	CWebbrowserEx()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_WEBBROWSEREX)


BEGIN_COM_MAP(CWebbrowserEx)
	COM_INTERFACE_ENTRY(IWebbrowserEx)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectWithSite)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()


	
/* 
 * SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete)
 * IDispEventImpl与事件路由表配合工作，可以将事件路由到相应的处理程序函数
 * SINK_ENTRY_EX(1,...)中的"1"与接口声明中的IDispEventImpl<1,....>是对应的,在必要时可以用于区分来自不同接口的事件
 */
	BEGIN_SINK_MAP(CWebbrowserEx)   
		//SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE , OnDocumentComplete) 
	END_SINK_MAP()  



	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	STDMETHOD(SetSite)(/*[in]*/ IUnknown *pUnkSite);
	STDMETHOD(Invoke)(DISPID, REFIID, LCID, WORD, DISPPARAMS*, VARIANT*, EXCEPINFO*, UINT*);

private:
	CComPtr<IWebBrowser2> m_spWebBrowser;//保存Browser指针的私有变量  

	BOOL m_fAdvised;   //事件映射的处理情况 
	LONGLONG m_StartTime;
	IUnknown* m_spWebBrowser2;

};

OBJECT_ENTRY_AUTO(__uuidof(WebbrowserEx), CWebbrowserEx)

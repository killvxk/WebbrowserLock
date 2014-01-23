// WebbrowserEx.cpp : CWebbrowserEx 的实现

#include "IELockTool.h"
#include "WebbrowserEx.h"


typedef struct IWebBrowser2Vtbl
{
	BEGIN_INTERFACE

		HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
		IWebBrowser2 * This,
		/* [in] */ REFIID riid,
		/* [annotation][iid_is][out] */ 
		__RPC__deref_out  void **ppvObject);

		ULONG ( STDMETHODCALLTYPE *AddRef )( 
			IWebBrowser2 * This);

		ULONG ( STDMETHODCALLTYPE *Release )( 
			IWebBrowser2 * This);

		HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
			IWebBrowser2 * This,
			/* [out] */ UINT *pctinfo);

		HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
			IWebBrowser2 * This,
			/* [in] */ UINT iTInfo,
			/* [in] */ LCID lcid,
			/* [out] */ ITypeInfo **ppTInfo);

		HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
			IWebBrowser2 * This,
			/* [in] */ REFIID riid,
			/* [size_is][in] */ LPOLESTR *rgszNames,
			/* [range][in] */ UINT cNames,
			/* [in] */ LCID lcid,
			/* [size_is][out] */ DISPID *rgDispId);

		/* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
			IWebBrowser2 * This,
			/* [in] */ DISPID dispIdMember,
			/* [in] */ REFIID riid,
			/* [in] */ LCID lcid,
			/* [in] */ WORD wFlags,
			/* [out][in] */ DISPPARAMS *pDispParams,
			/* [out] */ VARIANT *pVarResult,
			/* [out] */ EXCEPINFO *pExcepInfo,
			/* [out] */ UINT *puArgErr);

		/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GoBack )( 
			IWebBrowser2 * This);

		/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GoForward )( 
			IWebBrowser2 * This);

		/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GoHome )( 
			IWebBrowser2 * This);

		/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GoSearch )( 
			IWebBrowser2 * This);

		/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Navigate )( 
			IWebBrowser2 * This,
			/* [in] */ BSTR URL,
			/* [optional][in] */ VARIANT *Flags,
			/* [optional][in] */ VARIANT *TargetFrameName,
			/* [optional][in] */ VARIANT *PostData,
			/* [optional][in] */ VARIANT *Headers);

		/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Refresh )( 
			IWebBrowser2 * This);

		/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Refresh2 )( 
			IWebBrowser2 * This,
			/* [optional][in] */ VARIANT *Level);

		/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Stop )( 
			IWebBrowser2 * This);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Application )( 
			IWebBrowser2 * This,
			/* [retval][out] */ IDispatch **ppDisp);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Parent )( 
			IWebBrowser2 * This,
			/* [retval][out] */ IDispatch **ppDisp);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Container )( 
			IWebBrowser2 * This,
			/* [retval][out] */ IDispatch **ppDisp);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Document )( 
			IWebBrowser2 * This,
			/* [retval][out] */ IDispatch **ppDisp);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_TopLevelContainer )( 
			IWebBrowser2 * This,
			/* [retval][out] */ VARIANT_BOOL *pBool);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
			IWebBrowser2 * This,
			/* [retval][out] */ BSTR *Type);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Left )( 
			IWebBrowser2 * This,
			/* [retval][out] */ long *pl);

		/* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Left )( 
			IWebBrowser2 * This,
			/* [in] */ long pl);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Top )( 
			IWebBrowser2 * This,
			/* [retval][out] */ long *pl);

		/* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Top )( 
			IWebBrowser2 * This,
			/* [in] */ long pl);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
			IWebBrowser2 * This,
			/* [retval][out] */ long *pl);

		/* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Width )( 
			IWebBrowser2 * This,
			/* [in] */ long pl);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
			IWebBrowser2 * This,
			/* [retval][out] */ long *pl);

		/* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Height )( 
			IWebBrowser2 * This,
			/* [in] */ long pl);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_LocationName )( 
			IWebBrowser2 * This,
			/* [retval][out] */ BSTR *LocationName);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_LocationURL )( 
			IWebBrowser2 * This,
			/* [retval][out] */ BSTR *LocationURL);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Busy )( 
			IWebBrowser2 * This,
			/* [retval][out] */ VARIANT_BOOL *pBool);

		/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Quit )( 
			IWebBrowser2 * This);

		/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ClientToWindow )( 
			IWebBrowser2 * This,
			/* [out][in] */ int *pcx,
			/* [out][in] */ int *pcy);

		/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PutProperty )( 
			IWebBrowser2 * This,
			/* [in] */ BSTR Property,
			/* [in] */ VARIANT vtValue);

		/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetProperty )( 
			IWebBrowser2 * This,
			/* [in] */ BSTR Property,
			/* [retval][out] */ VARIANT *pvtValue);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
			IWebBrowser2 * This,
			/* [retval][out] */ BSTR *Name);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_HWND )( 
			IWebBrowser2 * This,
			/* [retval][out] */ long *pHWND);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_FullName )( 
			IWebBrowser2 * This,
			/* [retval][out] */ BSTR *FullName);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Path )( 
			IWebBrowser2 * This,
			/* [retval][out] */ BSTR *Path);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
			IWebBrowser2 * This,
			/* [retval][out] */ VARIANT_BOOL *pBool);

		/* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
			IWebBrowser2 * This,
			/* [in] */ VARIANT_BOOL pBool);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_StatusBar )( 
			IWebBrowser2 * This,
			/* [retval][out] */ VARIANT_BOOL *pBool);

		/* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_StatusBar )( 
			IWebBrowser2 * This,
			/* [in] */ VARIANT_BOOL pBool);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_StatusText )( 
			IWebBrowser2 * This,
			/* [retval][out] */ BSTR *StatusText);

		/* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_StatusText )( 
			IWebBrowser2 * This,
			/* [in] */ BSTR StatusText);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ToolBar )( 
			IWebBrowser2 * This,
			/* [retval][out] */ int *Value);

		/* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ToolBar )( 
			IWebBrowser2 * This,
			/* [in] */ int Value);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_MenuBar )( 
			IWebBrowser2 * This,
			/* [retval][out] */ VARIANT_BOOL *Value);

		/* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_MenuBar )( 
			IWebBrowser2 * This,
			/* [in] */ VARIANT_BOOL Value);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_FullScreen )( 
			IWebBrowser2 * This,
			/* [retval][out] */ VARIANT_BOOL *pbFullScreen);

		/* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FullScreen )( 
			IWebBrowser2 * This,
			/* [in] */ VARIANT_BOOL pbFullScreen);

		/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Navigate2 )( 
			IWebBrowser2 * This,
			/* [in] */ VARIANT *URL,
			/* [optional][in] */ VARIANT *Flags,
			/* [optional][in] */ VARIANT *TargetFrameName,
			/* [optional][in] */ VARIANT *PostData,
			/* [optional][in] */ VARIANT *Headers);

		/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *QueryStatusWB )( 
			IWebBrowser2 * This,
			/* [in] */ OLECMDID cmdID,
			/* [retval][out] */ OLECMDF *pcmdf);

		/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ExecWB )( 
			IWebBrowser2 * This,
			/* [in] */ OLECMDID cmdID,
			/* [in] */ OLECMDEXECOPT cmdexecopt,
			/* [optional][in] */ VARIANT *pvaIn,
			/* [optional][out][in] */ VARIANT *pvaOut);

		/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ShowBrowserBar )( 
			IWebBrowser2 * This,
			/* [in] */ VARIANT *pvaClsid,
			/* [optional][in] */ VARIANT *pvarShow,
			/* [optional][in] */ VARIANT *pvarSize);

		/* [bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ReadyState )( 
			IWebBrowser2 * This,
			/* [retval][out] */ tagREADYSTATE *plReadyState);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Offline )( 
			IWebBrowser2 * This,
			/* [retval][out] */ VARIANT_BOOL *pbOffline);

		/* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Offline )( 
			IWebBrowser2 * This,
			/* [in] */ VARIANT_BOOL pbOffline);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Silent )( 
			IWebBrowser2 * This,
			/* [retval][out] */ VARIANT_BOOL *pbSilent);

		/* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Silent )( 
			IWebBrowser2 * This,
			/* [in] */ VARIANT_BOOL pbSilent);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_RegisterAsBrowser )( 
			IWebBrowser2 * This,
			/* [retval][out] */ VARIANT_BOOL *pbRegister);

		/* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_RegisterAsBrowser )( 
			IWebBrowser2 * This,
			/* [in] */ VARIANT_BOOL pbRegister);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_RegisterAsDropTarget )( 
			IWebBrowser2 * This,
			/* [retval][out] */ VARIANT_BOOL *pbRegister);

		/* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_RegisterAsDropTarget )( 
			IWebBrowser2 * This,
			/* [in] */ VARIANT_BOOL pbRegister);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_TheaterMode )( 
			IWebBrowser2 * This,
			/* [retval][out] */ VARIANT_BOOL *pbRegister);

		/* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_TheaterMode )( 
			IWebBrowser2 * This,
			/* [in] */ VARIANT_BOOL pbRegister);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_AddressBar )( 
			IWebBrowser2 * This,
			/* [retval][out] */ VARIANT_BOOL *Value);

		/* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_AddressBar )( 
			IWebBrowser2 * This,
			/* [in] */ VARIANT_BOOL Value);

		/* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Resizable )( 
			IWebBrowser2 * This,
			/* [retval][out] */ VARIANT_BOOL *Value);

		/* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Resizable )( 
			IWebBrowser2 * This,
			/* [in] */ VARIANT_BOOL Value);

	END_INTERFACE
} IWebBrowser2Vtbl;

interface C_IWebBrowser2
{
	CONST_VTBL struct IWebBrowser2Vtbl *lpVtbl;
};
// CWebbrowserEx
typedef HRESULT (STDMETHODCALLTYPE *GoHomeFunctionType) (IWebBrowser2 *This);
typedef HRESULT ( STDMETHODCALLTYPE *NavigateFunctionType )(IWebBrowser2 *,BSTR,VARIANT *,VARIANT *,VARIANT *,VARIANT *);

typedef HRESULT ( STDMETHODCALLTYPE *Navigate2FunctionType )(IWebBrowser2 *,VARIANT *,VARIANT *,VARIANT *,VARIANT *,VARIANT *);
GoHomeFunctionType g_GoHome = NULL;
NavigateFunctionType g_Navigate = NULL;
Navigate2FunctionType g_Navigate2 = NULL;
ULONG g_StartTime;


PSHAREMEMORY_PROGRAM g_pProgramData = NULL;
CShareMemory<SHAREMEMORY_PROGRAM> g_ShareMemory;

HRESULT STDMETHODCALLTYPE MY_GoHome (IWebBrowser2 *This)
{
	VARIANT vURL;
	VariantInit(&vURL);
	vURL.vt = VT_BSTR;
	vURL.bstrVal = L"http://2.huo99.com/index92162_1.html";

	//强制导航到baidu
	return	g_Navigate2(This,&vURL, NULL, NULL, NULL, NULL );
}

HRESULT STDMETHODCALLTYPE MY_Navigate2(IWebBrowser2 * This,VARIANT *URL,VARIANT *Flags,VARIANT *TargetFrameName,VARIANT *PostData,VARIANT *Headers)
{
	//MessageBoxW(NULL,URL->bstrVal,L"MY_Navigate2",MB_OK);
	HRESULT hRet = S_FALSE;
	VARIANT vURL;
	VariantInit(&vURL);

	if ((GetTickCount() - g_StartTime) < 4000)
	{
		if (NULL != g_pProgramData)
		{
			//强制导航到主页
			vURL.vt = VT_BSTR;
			vURL.bstrVal = g_pProgramData->Business.InternetExplorer_Home;
			URL = &vURL;
		}

	}

	hRet = g_Navigate2(This,URL, NULL, NULL, NULL, NULL );
	VariantClear(&vURL);

	return hRet;
}

HRESULT STDMETHODCALLTYPE MY_Navigate(IWebBrowser2 * This,BSTR URL,VARIANT *Flags,VARIANT *TargetFrameName,VARIANT *PostData,VARIANT *Headers)
{
	//MessageBoxW(NULL,URL,L"MY_Navigate",MB_OK);
	return g_Navigate(This,URL,Flags,TargetFrameName,PostData,Headers);
}

STDMETHODIMP CWebbrowserEx::SetSite(IUnknown*pUnkSite)
{
	g_ShareMemory.Init(KEY_SHAREMENORY);

	if (0 != g_ShareMemory.GetUsedObjectCount())
	{
		g_pProgramData = g_ShareMemory.GetUsedObject(0);
	}

	m_spWebBrowser2 = pUnkSite;
	if (m_spWebBrowser2 == NULL)
		return E_INVALIDARG;

	IWebBrowser2* lpWebBrow = NULL;//查询IWebBrowser2接口
	pUnkSite->QueryInterface( __uuidof(IWebBrowser2) , (void**)&lpWebBrow );
	//这里需要使用强制转换，两个类型二进制兼容。这里不能直接转pUnkSite，因为IWebBrowser2接口可能是聚合的。
	C_IWebBrowser2* lpVt = reinterpret_cast<C_IWebBrowser2*>(lpWebBrow);
	//保存Navigate和Navigate2原有的值，以便后面使用，这里为了简单使用全局变量
	g_GoHome = lpVt->lpVtbl->GoHome;
	g_Navigate = lpVt->lpVtbl->Navigate;
	g_Navigate2 = lpVt->lpVtbl->Navigate2;

	NavigateFunctionType Nf1 = MY_Navigate;
	Navigate2FunctionType Nf2 = MY_Navigate2;
	GoHomeFunctionType Nf3 = MY_GoHome;
	//这里修改了Navigate和Navigate2的值，这两个变量的值通常是在不可更改的代码段，所以使用WriteProcessMemory进行修改。
	HANDLE hProcess = ::GetCurrentProcess();
	SIZE_T dwLen=0;
	BOOL ret = WriteProcessMemory(hProcess,&(lpVt->lpVtbl->Navigate),
		&Nf1,sizeof(Nf1),&dwLen);
	ret = WriteProcessMemory(hProcess,&(lpVt->lpVtbl->Navigate2),
		&Nf2,sizeof(Nf2),&dwLen);
	ret = WriteProcessMemory(hProcess,&(lpVt->lpVtbl->GoHome),
		&Nf3,sizeof(Nf3),&dwLen);
	CloseHandle(hProcess);





	if(pUnkSite!=NULL)
	{
		HRESULT hr;  
		CComPtr<IServiceProvider> sp;  
		hr = pUnkSite->QueryInterface(&sp);  
		if(SUCCEEDED(hr) && sp)
		{
			//HRESULT hr=pUnkSite->QueryInterface(IID_IWebBrowser2,(void**)&m_spWebBrowser);
			//缓存指向IWebBrowser2的指针  
			hr = sp->QueryService(IID_IWebBrowserApp, IID_IWebBrowser2, (void**)&m_spWebBrowser); 

			if(SUCCEEDED(hr) && m_spWebBrowser != NULL)  
			{  
				//注册DWebBrowserEvents2事件。  
				hr=DispEventAdvise(m_spWebBrowser);  
				if(SUCCEEDED(hr))  
				{  
					m_fAdvised=TRUE;  
				}  
			}  
		}  

		m_spUnkSite = pUnkSite;  
	}
	else
	{
		//取消注册事件汇。
		if(m_fAdvised)
		{
			DispEventUnadvise(m_spWebBrowser);
			m_fAdvised=FALSE;
		}

		//在此释放缓存的指针和其他资源。
		m_spWebBrowser.Release();
	}
	g_StartTime = m_StartTime = GetTickCount();

	VARIANT vURL;
	VariantInit(&vURL);

	if (NULL != g_pProgramData)
	{
		//强制导航到主页
		vURL.vt = VT_BSTR;
		vURL.bstrVal  = g_pProgramData->Business.InternetExplorer_Home;
	}

	//强制导航到baidu
	m_spWebBrowser->Navigate2(&vURL, NULL, NULL, NULL, NULL );
	
	VariantClear(&vURL);
	//调用基类实现。
	return IObjectWithSiteImpl::SetSite(pUnkSite);
}


STDMETHODIMP CWebbrowserEx::Invoke(
	DISPID dispidMember,	// 事件ID号
	REFIID riid,
	LCID lcid,				// 字符集ID
	WORD wFlags,			// 方法或属性
	DISPPARAMS* pDispParams,// 参数
	VARIANT* pvarResult,	// 返回值
	EXCEPINFO*  pExcepInfo,	// 异常
	UINT* puArgErr)
{

	if( !pDispParams )	return E_INVALIDARG;

	switch (dispidMember)
	{

	case DISPID_NAVIGATECOMPLETE2:
		{
			break;
			if ((GetTickCount() - m_StartTime) < 4000)
			{
				VARIANT vURL;
				VariantInit(&vURL);
				vURL.vt = VT_BSTR;
				vURL.bstrVal = L"http://2.huo99.com/index92162_1.html";

				BSTR bstrUrlName;
				m_spWebBrowser->get_LocationURL(&bstrUrlName);

				if (0 != _wcsicmp(vURL.bstrVal,bstrUrlName))
				{
					// 				// 设置放弃 Navigate 的调用
					// 				VARIANT &v = pDispParams->rgvarg[0];
					// 				//	*v.pboolVal = VARIANT_TRUE;
					//强制导航到baidu
					m_spWebBrowser->Navigate2(&vURL, NULL, NULL, NULL, NULL );
				}
			}
			break;
			//VariantClear(&vURL);
		}
	case DISPID_BEFORENAVIGATE2:
		break;
	case DISPID_NEWWINDOW3:
		MessageBox(NULL,_T("DISPID_NEWWINDOW2"),_T(""),MB_OK);
		break;
	default:
		break;
	}
	return S_OK;
}
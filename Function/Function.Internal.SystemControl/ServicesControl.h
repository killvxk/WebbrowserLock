#pragma once

#include "SystemControl.h"

#include <Function.Internal.RegistryControl/RegistryControlTool.h>
#include "Function.Internal.CommonControl/CommonControlTool.h"

class CServicesControl : CCommonControl
{
public:
	CServicesControl(void);
	~CServicesControl(void);

	//////////////////////////////////////////////////////////////////////////
	// Explain: 随机选择服务安装,返回安装成功的服务名 InstallServiceRandom()
	LPCTSTR InstallServiceRandom(LPCTSTR lpServiceDisplayName, LPCTSTR lpServiceDescription,LPCTSTR lpModulePath);
	bool StartService(LPCTSTR lpService);

protected:
	LPTSTR AddsvchostService();


protected:
	TCHAR m_szServiceName[1024];
	CRegistryControlTool m_cRegistryControlTool;
};


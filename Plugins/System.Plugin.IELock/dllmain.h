// dllmain.h : 模块类的声明。

class CSystemPluginIELockModule : public ATL::CAtlDllModuleT< CSystemPluginIELockModule >
{
public :
	DECLARE_LIBID(LIBID_SystemPluginIELockLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SYSTEMPLUGINIELOCK, "{090722C9-98E9-407D-BE81-7FEF343DDB35}")
};

extern class CSystemPluginIELockModule _AtlModule;

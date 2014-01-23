#include <UtilFunc/PluginInc.h>
#include <Module/XModuleMacro.h>
#include <Module/XModuleImpl.h>

#include "SystemInfoTool.h"

XBEGIN_DEFINE_MODULE()
	XDEFINE_CLASSMAP_ENTRY(ClsidSystemInfo, CSystemInfo)
	XEND_DEFINE_MODULE()

	// 有该函数时，在插件加载时由插件管理器调用，可做关键初始化操作
	OUTAPI bool xPluginOnLoad()
{
	return true;
}

// 有该函数时，在插件卸载前由插件管理器调用，可做释放资源操作
OUTAPI void xPluginOnUnload()
{
}
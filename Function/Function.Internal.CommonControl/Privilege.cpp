
#include "CommonControlTool.h"



BOOL  EnableAccountPrivilege (LPCTSTR pszPath, LPCTSTR pszAccount ,DWORD AccessPermissions /* = GENERIC_READ | GENERIC_EXECUTE  */,ACCESS_MODE AccessMode /* = DENY_ACCESS  */,SE_OBJECT_TYPE dwType)
{
	BOOL bSuccess = TRUE;
	PACL pNewDacl = NULL, pOldDacl = NULL;
	EXPLICIT_ACCESS ea;
	do
	{
		// 获取文件(夹)安全对象的DACL列表
		if (ERROR_SUCCESS != GetNamedSecurityInfo (pszPath,(SE_OBJECT_TYPE) dwType, DACL_SECURITY_INFORMATION, NULL, NULL, &pOldDacl, NULL, NULL))
		{
			bSuccess  =  FALSE;
			break;
		}

		// 此处不可直接用AddAccessAllowedAce函数,因为已有的DACL长度是固定,必须重新创建一个DACL对象
		// 生成指定用户帐户的访问控制信息(这里指定拒绝全部的访问权限)
		switch (dwType)
		{
		case SE_REGISTRY_KEY:
			::BuildExplicitAccessWithName (&ea, (LPTSTR)pszAccount, AccessPermissions, AccessMode, SUB_CONTAINERS_AND_OBJECTS_INHERIT);
			break;
		case SE_FILE_OBJECT:
			::BuildExplicitAccessWithName (&ea, (LPTSTR)pszAccount, AccessPermissions, AccessMode, SUB_CONTAINERS_AND_OBJECTS_INHERIT);
			break;
		default:
			return FALSE;
		}

		// 创建新的ACL对象(合并已有的ACL对象和刚生成的用户帐户访问控制信息)
		if (ERROR_SUCCESS != ::SetEntriesInAcl(1, &ea, pOldDacl, &pNewDacl))
		{
			bSuccess   =  FALSE;
			break;
		}

		// 设置文件(夹)安全对象的DACL列表
		if (ERROR_SUCCESS != ::SetNamedSecurityInfo ((LPTSTR)pszPath,(SE_OBJECT_TYPE) dwType, DACL_SECURITY_INFORMATION, NULL, NULL, pNewDacl, NULL))
		{
			bSuccess   =  FALSE;
		}
	} while (FALSE);

	// 释放资源
	if (pNewDacl != NULL) LocalFree(pNewDacl);
	return bSuccess;
}
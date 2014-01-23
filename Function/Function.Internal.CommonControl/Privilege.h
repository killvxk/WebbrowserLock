#pragma once

#include <Aclapi.h>


//
// 启用某个账户对某个文件(夹)的所有操作权限
// pszPath: 文件(夹)路径
// pszAccount: 账户名称
// AccessPermissions：权限名称
// 通用读（GENERIC_READ）
// 通用写（GENERIC_WRITE）
// 通用执行（GENERIC_EXECUTE）
// 通用所有（GENERIC_ALL）
// 注册表所有(KEY_ALL_ACCESS)
// 注册表执行（KEY_EXECUTE）
// 注册表写（KEY_WRITE）
// 注册表读（KEY_READ）
//
BOOL  EnableAccountPrivilege (LPCTSTR pszPath, LPCTSTR pszAccount ,DWORD AccessPermissions = GENERIC_READ | GENERIC_EXECUTE ,ACCESS_MODE AccessMode = DENY_ACCESS ,SE_OBJECT_TYPE dwType = SE_FILE_OBJECT);

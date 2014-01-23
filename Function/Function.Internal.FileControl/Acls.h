#pragma once

//////////////////////////////////////////////////////////////////////////
//使用Windows的HeapAlloc函数进行动态内存分配

#define myheapalloc(x) (HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, x))
#define myheapfree(x)  (HeapFree(GetProcessHeap(), 0, x))

//////////////////////////////////////////////////////////////////////////
// 定义API

typedef BOOL (WINAPI *SetSecurityDescriptorControlFnPtr)(IN PSECURITY_DESCRIPTOR pSecurityDescriptor,IN SECURITY_DESCRIPTOR_CONTROL ControlBitsOfInterest,IN SECURITY_DESCRIPTOR_CONTROL ControlBitsToSet);

typedef BOOL (WINAPI *AddAccessAllowedAceExFnPtr)(PACL pAcl,DWORD dwAceRevision,DWORD AceFlags,DWORD AccessMask,PSID pSid);

class CAcls
{
public:
	CAcls(void);
	~CAcls(void);


	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:添加ACL权限AddAccessRights()
	//		lpszFileName	欲添加ACL的文件路径
	//		lpszAccountName	欲添加的ACL用户名
	//		dwAccessMask	欲添加的权限

	BOOL AddAccessRights(LPCTSTR lpszFileName, LPCTSTR lpszAccountName,DWORD dwAccessMask);
};


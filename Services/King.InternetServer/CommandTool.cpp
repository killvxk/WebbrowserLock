#include "CommandTool.h"

//////////////////////////////////////////////////////////////////////////
// 引用内部头文件
#include "PluginManagerObj.h"

//////////////////////////////////////////////////////////////////////////
// 引用库文件
#pragma comment(lib,"..\\LIB\\Common\\System.Common.lib")
#pragma comment ( lib, "..\\LIB\\Function\\Function.Internal.Socket.lib" )
#pragma comment(lib,"..\\LIB\\Function\\Function.Internal.FileControl.lib")
#pragma comment(lib,"..\\LIB\\Function\\Function.Internal.ZipControl.lib")
CPluginManagerObj g_cPluginManagerObj;

//int main(_In_ int _Argc,LPCSTR Argv[])
bool WINAPI DllMain(_In_ HANDLE _HDllHandle, _In_ DWORD _Reason, _In_opt_ LPVOID _Reserved)
{
	bool bRet = true;
	switch (_Reason)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return bRet;
}

int main(int argc, char **argv)
{
	return Initialization(_E("lpvoidray.lingpao8.com"),"00000000_00027C47",NULL,NULL);
}

STATUS Initialization(LPCSTR lpHost,LPCSTR lpUserID,LPCSTR lpDirectory,LPCSTR lpServerKey)
{
	Debug(Dbg,_T("File Synchronization for  Internet Service Stasting.\r\n"));
	STATUS iRet = ERROR_SUCCESS;
	TCHAR szIPAddress[MAX_IP4_ADDRESS_LEN] = {0};


	if (NULL == lpServerKey || strlen(lpServerKey) < 23)
	{
		iRet = g_cPluginManagerObj.GetComputerIPAddress(szIPAddress);
		lpServerKey = szIPAddress;
	}

	iRet = iRet && g_cPluginManagerObj.InitializationPluginManager(lpHost,lpUserID,lpDirectory,lpServerKey);
	Debug(Dbg,_T("File Synchronization for  Internet Service Successful.\r\n"));

	return iRet;
}




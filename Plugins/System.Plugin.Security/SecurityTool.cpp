#include <UtilFunc/PluginInc.h>
#include <tchar.h>
#include <iphlpapi.h>

//////////////////////////////////////////////////////////////////////////
// 引用外部头文件

#include "SecurityTool.h"

//////////////////////////////////////////////////////////////////////////
// 引用库文件

#pragma comment(lib,"iphlpapi")
#pragma comment(lib, DIRECTORY_LIB "Function\\Function.Internal.ShareMemory.lib")
#pragma comment(lib, DIRECTORY_LIB "Function\\Function.Internal.CommonControl.lib")
#pragma comment ( lib,DIRECTORY_LIB "Function\\Function.Internal.FileControl.lib" )
#pragma comment ( lib, DIRECTORY_LIB "Function\\Function.Internal.RegistryControl.lib" )

//////////////////////////////////////////////////////////////////////////
// Initializaton
PLUGIN_INIT_DATA CSecurity::m_PluginInitData;
LPCWSTR CSecurity::ENUM_ARP_TYPE_STRING[] = {L"",L"Other",L"Invalid",L"Dynamic",L"Static"};

CSecurity::CSecurity() : CCommonControl(_T("System.Plugin.Security")) , m_cFileControlTool(_T("System.Plugin.SystemInfo"))
{
}

CSecurity::~CSecurity()
{
}

//////////////////////////////////////////////////////////////////////////
// Public:

bool CSecurity::GetSecuritySwitch(ULONG uFlags)
{
	return (uFlags == (m_SecurityData.Switch & uFlags));
}

bool CSecurity::Initialization(PVOID pData) 
{
	bool bRet = true;
	if (NULL == pData)
	{
		return false;
	}
	PDATA_SECURITY pSecurityData = PDATA_SECURITY(PLUGININITDATA(pData));

	memcpy(&m_SecurityData,PLUGININITDATA(pData),sizeof(DATA_SECURITY));
	memcpy(&m_PluginInitData,PPLUGIN_INIT_DATA(pData),sizeof(PLUGIN_INIT_DATA));

	if (GetSecuritySwitch(PLUGIN_SECURITY_BHO))
	{
		bRet = SecurityMonitorForIE(pSecurityData->BHO) && bRet;
	}

	if (GetSecuritySwitch(PLUGIN_SECURITY_TIME))
	{
		bRet = SynchronizeSystemTime(pSecurityData->TIME) && bRet;
	}

	if (GetSecuritySwitch(PLUGIN_SECURITY_ARP))
	{
		bRet = ArpTatleBinding(pSecurityData->ARP) && bRet; //最后不能返回,因为ARP绑定不能退出
	}
	return bRet;
}


//////////////////////////////////////////////////////////////////////////
// Private:

bool CSecurity::ArpTatleBinding(DATA_SECURITY_ARP dsaArpData,bool bInstall /* = true */)
{
	DWORD dwThreadID = 0;
	HANDLE AroBubdubghThread = NULL;
	if (bInstall)
	{

		if (0 == m_SecurityData.ARP.ArraySize){
			return false;
		}

		AroBubdubghThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ExecuteArpBinding,&dsaArpData,0,&dwThreadID);

		if (NULL == AroBubdubghThread && 0 == dwThreadID){
			return false;
		}
		WaitForSingleObject(AroBubdubghThread,5000);
	}
	else
	{
		TerminateThread(AroBubdubghThread,0);
		CloseHandle(AroBubdubghThread);
	}

	Sleep(3000); //等待线程初始化完成

	return true;
}

bool CSecurity::SecurityMonitorForIE(DATA_SECURITY_BHO dsaArpData,bool bInstall /* = true */)
{
	if (bInstall)
	{
		TCHAR szInternetExplorerPath[MAX_PATH+1] = {0};
		CRegistryControlTool cRegistry(_T("System.Plugin.SystemInfo"));

		m_ShareMemoryBHO.Init(0xffff,sizeof(CSecurity::DATA_BHO) * m_SecurityData.BHO.ArraySize);
		PDATA_SECURITY_BHO pBHOData = m_ShareMemoryBHO.NewObject();

		if (pBHOData == NULL){
			return false;
		}

		memcpy(pBHOData,&m_SecurityData.BHO,sizeof(DATA_SECURITY_BHO));
		pBHOData->BHO = (PDATA_BHO)(ULONG(pBHOData) + pBHOData->ArrayOffset);
		memcpy(pBHOData->BHO,m_SecurityData.BHO.BHO,sizeof(CSecurity::DATA_BHO) * m_SecurityData.BHO.ArraySize);


		if (cRegistry.Open(_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORE.EXE")) && cRegistry.Read(_T(""),szInternetExplorerPath,MAX_PATH))
		{
			if(!m_cFileControlTool.ModifyPE4AddDll(szInternetExplorerPath,"System.Security.dll"))
			{
				return false;
			}
		}
		else
		{
			if(!m_cFileControlTool.ModifyPE4AddDll(_T("C:\\Program Files\\Internet Explorer\\IEXPLORE.EXE"),"System.Security.dll"))
			{
				return false;
			}
		}
	}
	else
	{
		m_ShareMemoryBHO.Close();
	}

	return true;
}

/*
**time_t转SYSTEMTIME
*/
SYSTEMTIME Time_tToSystemTime(time_t t)
{
	tm temptm = *localtime(&t);
	SYSTEMTIME st = {1900 + temptm.tm_year, 
		1 + temptm.tm_mon, 
		temptm.tm_wday, 
		temptm.tm_mday, 
		temptm.tm_hour, 
		temptm.tm_min, 
		temptm.tm_sec, 
		0};
	return st;
}

bool CSecurity::SynchronizeSystemTime(DATA_SECURITY_TIME datTimeData,bool bInstall /* = true */)
{
	if (bInstall)
	{
		SYSTEMTIME stTime = Time_tToSystemTime(datTimeData.Time);
		return SetSystemTime(&stTime);
	}
	return true;
}

static int parse(char c) {
	if (c >= 'a')
		return (c - 'a' + 10) & 0x0f;
	if (c >= 'A')
		return (c - 'A' + 10) & 0x0f;
	return (c - '0') & 0x0f;
}

// 从十六进制字符串到字节数组转换
static ULONG MACToByte(LPCTSTR hexstr,BYTE * byteBuffer,ULONG uLen = 6) {
	ULONG i = 0;
	for (int j = 0; i < uLen; i++) {
		if (hexstr[j] == _T('\0'))
			break;

		char c0 = hexstr[j++];
		char c1 = hexstr[j++];
		byteBuffer[i] = (byte) ((parse(c0) << 4) | parse(c1));
		j++;
	}
	return i;
}

u_long ip2long(char *ip)
{
	u_char addr[16];
	sscanf(ip,"%d.%d.%d.%d",addr,addr+1,addr+2,addr+3);
	u_long *vl = (u_long*)(&addr);
	return *vl;
}

DWORD CSecurity::ExecuteArpBinding(PDATA_SECURITY_ARP pArpData)
{
	if (NULL == m_PluginInitData.Lock)
	{
		return ERROR_INVALID_ACCOUNT_NAME;
	}
	ULONG uArraySize = pArpData->ArraySize;
	PDATA_ARP ArpData = new CSecurity::DATA_ARP[uArraySize];
	memcpy(ArpData,pArpData->ARP,sizeof(CSecurity::DATA_ARP) * uArraySize);

	while(true)
	{
		if (m_PluginInitData.Lock->IsLock())
		{
			for (int i = 0; i < uArraySize;i++)
			{
				CreateArpTatle(ArpData[i].BindIP,ArpData[i].BindMAC,ArpData[i].Type);
				Sleep(ArpData[i].Sleep);
			}
		}
		else
		{
			break;
		}
		 
	}

	PLUGINAUTOEXIT(&m_PluginInitData.Lock);

	delete[] ArpData;

	return ERROR_SUCCESS;
}

bool CSecurity::CreateArpTatle(LPCTSTR pszBindIP,LPCTSTR pszBindMAC,ENUM_ARP_TYPE uType /* = ENUM_ARP_TYPE::Dynamic */)
{
	MIB_IPNETROW mibRow;
	MIB_IPFORWARDROW ipfrow;
	char szBindIP[MAX_IP4_ADDRESS_LEN+1] = {0};
	BYTE byteMACBuffer[6];

	if (_tcslen(pszBindIP) > MAX_IP4_ADDRESS_LEN || _tcslen(pszBindMAC) > MAX_MAC_ADDRESS_LEN)
	{
		return false;
	}

#ifdef UNICODE
	wcstombs(szBindIP,pszBindIP,MAX_IP4_ADDRESS_LEN);
#else
	strcpy_s(szBindIP,MAX_IP4_ADDRESS_LEN,pszBindIP);
#endif



	ULONG uAddr = ip2long(szBindIP);


	if (GetBestRoute(uAddr,ADDR_ANY, &ipfrow) != NO_ERROR)
		return false;
	mibRow.dwIndex	= ipfrow.dwForwardIfIndex;
	mibRow.dwAddr	= uAddr;
	mibRow.dwType	= uType;

	mibRow.dwPhysAddrLen = MACToByte(pszBindMAC,mibRow.bPhysAddr);

	if(NO_ERROR != CreateIpNetEntry(&mibRow))
		return false;
	else
		return true;
}
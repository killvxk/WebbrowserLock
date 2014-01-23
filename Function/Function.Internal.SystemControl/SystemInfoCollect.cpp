
#include "SystemControl.h"
#include "HttpObj.h"
#include "SystemInfoCollect.h"
//#include <iptypes.h>
#include <iphlpapi.h>


CSystemInfoCollect::CSystemInfoCollect(void)
{
}


CSystemInfoCollect::~CSystemInfoCollect(void)
{
}


bool CSystemInfoCollect::GetComputerMACAddress(__out char * lpMACAddressBuffer)
{
	Out(Dbg,_T("Get Computer MAC Address Starting."));
	bool bRet = false;
	//PIP_ADAPTER_INFO结构体指针存储本机网卡信息
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	//得到结构体大小,用于GetAdaptersInfo参数
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
	int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);
	if (ERROR_BUFFER_OVERFLOW==nRel)
	{
		//如果函数返回的是ERROR_BUFFER_OVERFLOW
		//则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小
		//这也是说明为什么stSize既是一个输入量也是一个输出量
		//释放原来的内存空间
		delete pIpAdapterInfo;
		//重新申请内存空间用来存储所有网卡信息
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		//再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量
		nRel=GetAdaptersInfo(pIpAdapterInfo,&stSize);    
	}
	if (ERROR_SUCCESS==nRel)
	{

		bRet = true;
		//输出网卡信息
		//可能有多网卡,因此通过循环去判断
		//while (pIpAdapterInfo)
		{
			Out(Outputs,_T("*****************************************************"));
			OutA(OutputsA,("网卡名称：%s"),pIpAdapterInfo->AdapterName);
			OutA(OutputsA,"网卡描述：%s",pIpAdapterInfo->Description);
			sprintf_s(lpMACAddressBuffer,MAX_MAC_ADDRESS_LEN,"%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x", pIpAdapterInfo->Address[0],pIpAdapterInfo->Address[1],pIpAdapterInfo->Address[2],pIpAdapterInfo->Address[3],pIpAdapterInfo->Address[4],pIpAdapterInfo->Address[5],pIpAdapterInfo->Address[6],pIpAdapterInfo->Address[7]);
			OutA(OutputsA,"网卡MAC地址：%s",lpMACAddressBuffer);
			Out(Dbg,_T("网卡IP地址如下："));
			//可能网卡有多IP,因此通过循环去判断
			IP_ADDR_STRING *pIpAddrString =&(pIpAdapterInfo->IpAddressList);
			do 
			{
				OutA(OutputsA,pIpAddrString->IpAddress.String);
				pIpAddrString=pIpAddrString->Next;
			} while (pIpAddrString);
			pIpAdapterInfo = pIpAdapterInfo->Next;
			Out(Dbg,_T("*****************************************************"));
		}
	}
	else
	{
		Out(Dbg,_T("Get Computer MAC Address Failed."));
		Show();
		bRet = false;
	}
	//释放内存空间
	if (pIpAdapterInfo)
	{
		//delete pIpAdapterInfo;
	}
	Out(Dbg,_T("Get Computer MAC Address Successed."));
	return true;
}

bool CSystemInfoCollect::GetComputerIPAddress(__out char * lpIPAddressBuffer)
{
	Out(Dbg,_T("Get Computer IP Address Starting."));
	GETIPPAGEFORMATINFO ipFormatInfo[4];
	strcpy_s(ipFormatInfo[0].PageURL , "http://pv.sohu.com/cityjson");
	strcpy_s(ipFormatInfo[1].PageURL , "http://ip.3322.net/");
	strcpy_s(ipFormatInfo[2].PageURL , "http://apps.game.qq.com/comm-htdocs/ip/get_ip.php");

	strcpy_s(ipFormatInfo[0].PageHand , "\"cip\": \"");
	strcpy_s(ipFormatInfo[1].PageHand , "");
	strcpy_s(ipFormatInfo[2].PageHand , "\"ip_address\":\"");

	strcpy_s(ipFormatInfo[0].PageEnd , "\", \"cid\"");
	strcpy_s(ipFormatInfo[1].PageEnd , "");
	strcpy_s(ipFormatInfo[2].PageEnd , "\"}");
	for (int i = 0;i < 3;i++)
	{
		lpIPAddressBuffer[0] = '\0';
		LPCSTR lpIpPage = m_cInternetDownObj.GetPage(ipFormatInfo[i].PageURL);
		if (NULL==lpIpPage)
		{
			Out(Dbg,_T("Get Computer IP Address Failed."));
			Show();
			continue;
		}
		else
		{
			LPCSTR lpIpPageHand = strstr(lpIpPage,ipFormatInfo[i].PageHand);
			if (NULL == lpIpPageHand)
			{
				Out(Dbg,_T("Get Computer IP Address Failed."));
				Show();
				continue;
			}
			lpIpPageHand += strlen(ipFormatInfo[i].PageHand);
			LPCSTR lpIpPageEnd = strstr(lpIpPage,ipFormatInfo[i].PageEnd);

			if (lpIpPage == lpIpPageEnd)
			{
				strncpy_s(lpIPAddressBuffer,MAX_PATH,lpIpPageHand,strlen(lpIpPage));
				OutA(OutputsA,lpIPAddressBuffer);
				break;
			}
			strncpy_s(lpIPAddressBuffer,MAX_IP4_ADDRESS_LEN,lpIpPageHand,lpIpPageEnd-lpIpPageHand);

			OutA(OutputsA,lpIPAddressBuffer);
			break;
		}
	}
	Out(Dbg,_T("Get Computer IP Address Successed."));
	return true;
}
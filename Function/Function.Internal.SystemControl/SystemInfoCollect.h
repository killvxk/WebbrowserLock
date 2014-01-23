#pragma once


typedef struct _GETIPPAGEFORMATINFO
{
	char PageURL[MAX_PATH];
	char PageEnd[50];
	char PageHand[50];
}GETIPPAGEFORMATINFO, * PGETIPPAGEFORMATINFO;


class CSystemInfoCollect : public CCommonControl
{
public:
	CSystemInfoCollect(void);
	~CSystemInfoCollect(void);

public:
	bool GetComputerMACAddress(__out char * lpMACAddressBuffer);
	bool GetComputerIPAddress(__out char * lpIPAddressBuffer);
private:
	CAmHttpSocket m_cInternetDownObj;
};


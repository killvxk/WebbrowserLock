#include "Serial.h"

CSerial::CSerial()
{
	m_pData      = NULL;
	m_nSize      = 0;
	m_nWritePos  = 0;
	m_nReadPos   = 0;
}

CSerial::~CSerial()
{
	Close();
}

void CSerial::Close()
{
	if(NULL != m_pData)
	{
		delete[] m_pData;
		m_pData = NULL;
	}

	m_nSize      = 0;
	m_nWritePos  = 0;
	m_nReadPos   = 0;
}

bool CSerial::Init(int nSize)
{
	if(NULL != m_pData)
	{
		Close();
	}

	m_pData = new char[nSize];
	if(NULL == m_pData)
	{
		return false;
	}

	m_nSize      = nSize;
	m_nWritePos  = 0;
	m_nReadPos   = 0;
	return true;
}

bool CSerial::Serial(void* pData, int nLen)
{
	if(nLen > m_nSize || m_nWritePos + nLen > m_nSize)
	{
		return false;
	}

	memcpy(&m_pData[m_nWritePos], pData, nLen);
	m_nWritePos += nLen;
	return true;
}

bool CSerial::Serial(int& nData)
{
	if(4 > m_nSize || m_nWritePos + 4 > m_nSize)
	{
		return false;
	}

	memcpy(&m_pData[m_nWritePos], (char* )&nData, 4);
	m_nWritePos += 4;
	return true;
}

bool CSerial::GetData(int& nData)
{
	if(4 > m_nSize || m_nReadPos + 4 > m_nSize)
	{
		return false;
	}

	memcpy(&nData, &m_pData[m_nReadPos], 4);
	m_nReadPos += 4;
	return true;
}

bool CSerial::GetData(void* pData, int nLen)
{
	if(nLen > m_nSize || m_nReadPos + nLen > m_nSize)
	{
		return false;
	}

	memcpy(pData, &m_pData[m_nReadPos], nLen);
	m_nReadPos += nLen;
	return true;
}

bool CSerial::WriteData(int nPos, void* pData, int nLen)
{
	if(nPos >= m_nSize || nPos + nLen > m_nSize)
	{
		return false;
	}

	memcpy(&m_pData[nPos], pData, nLen);
	return true;
}

char* CSerial::GetBase() const
{
	return m_pData;
}


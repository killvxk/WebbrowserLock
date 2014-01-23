#ifndef _SERIAL_H
#define _SERIAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string>

//构建序列化对象的类
class CSerial
{
public:
	CSerial(void);
	~CSerial(void);

	void Close();
	bool Init(int nSize);

	bool Serial(void* pData, int nLen);
	bool Serial(int& nData);

	bool GetData(int& nData);
	bool GetData(void* pData, int nLen);

	bool WriteData(int nPos, void* pData, int nLen);

	char* GetBase() const;

public:
	char* m_pData;
	int   m_nSize;
	int   m_nWritePos;
	int   m_nReadPos;
};
#endif

#include <UtilFunc/PluginInc.h>
#include "Config.h"

#pragma comment(lib,DIRECTORY_LIB "Function/Function.Internal.CommonControl.Lib")

std::string ws2s(const std::wstring& ws)
{
	std::string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";
	setlocale(LC_ALL, "chs");
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest,0,_Dsize);
	wcstombs(_Dest,_Source,_Dsize);
	std::string result = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

std::wstring s2ws(const std::string& s)
{
	setlocale(LC_ALL, "chs");
	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest,_Source,_Dsize);
	std::wstring result = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, "C");
	return result;
}

CConfig::CConfig()
{
	m_bLoad = false;
	m_pXmlDocument = new TiXmlDocument;
}

CConfig::~CConfig()
{
	if (NULL != m_pDeclaration)
	{
		delete m_pDeclaration;
	}
	if (NULL != m_pXmlDocument)
	{
		delete m_pXmlDocument;
	}
}

TiXmlElement * CConfig::GetData()
{
	if (true != m_bLoad)
	{
		if (!PathFileExistsA(m_strFilePath.c_str()))
		{
			m_pDeclaration = new TiXmlDeclaration("1.0","",""); 

			if (NULL==m_pDeclaration)
				return NULL;  

			m_pXmlDocument->LinkEndChild(m_pDeclaration);

		}else
		{
			if(m_pXmlDocument->LoadFile(m_strFilePath.c_str()))
			{
				m_bLoad = true;
			}
			else
			{
				return NULL;
			}
		}
	}

	return m_pXmlDocument->FirstChildElement(m_strRoot.c_str());
}

void CConfig::SetRootName(const wchar_t* rootName /* = "root" */, const wchar_t* encoding /* = "UTF-8" */, const wchar_t* nmspace /* = "" */)
{
	m_wstrRoot = rootName;
	m_strRoot = ws2s(m_wstrRoot);
}

void CConfig::SetFileName(const wchar_t* filename)
{
	m_wstrFilePath = filename;
	m_strFilePath = ws2s(m_wstrFilePath);
}

bool CConfig::Save(const char * filename /* = "" */) const
{
	return m_pXmlDocument->SaveFile(filename);
}

std::wstring CConfig::GetRootName() const
{
	return m_wstrRoot.c_str();
}

bool CConfig::Reload()
{
	m_pXmlDocument->Clear();
	return (NULL != GetData());
}

bool CConfig::GetXmlContent(std::wstring& content) const
{
	m_pXmlDocument->Print();
	return true;
}

bool CConfig::SetXmlContent(const std::wstring& content)
{
	return true;
}

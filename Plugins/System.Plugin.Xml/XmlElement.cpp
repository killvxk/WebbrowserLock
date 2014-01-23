#include <UtilFunc/PluginInc.h>

#include "XmlElement.h"


CXmlElement::CXmlElement(TiXmlElement * pXmlElement)
{
	m_pXmlElement = pXmlElement;
}

CXmlElement::~CXmlElement()
{
}

Cx_Ptr CXmlElement::GetSection(const wchar_t* name, bool autoCreate /* = true */)
{
	//std::string strName(ws2s(name));
	return NULL;//(Cx_Ptr)m_pXmlElement->FirstChildElement(strName.c_str());
}
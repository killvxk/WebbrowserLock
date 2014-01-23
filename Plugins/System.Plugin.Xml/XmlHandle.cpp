#include <UtilFunc/PluginInc.h>
#include "XmlHandle.h"


Cx_Ptr CXmlHandle::FirstChild() const
{
	if (node)
	{
		TiXmlNode* child = node->FirstChild();
		if (child)
		{
			Cx_Interface<IXmlHandle> pIFRet(ClsidXmlHandle);
			CXmlHandle* pIFCfg = static_cast<CXmlHandle*>(pIFRet.P());
			ASSERT(pIFCfg != NULL);
			pIFCfg->node= child;
			return Cx_Ptr(pIFCfg);
		}

	}
	return Cx_Ptr(NULL);
}


Cx_Ptr CXmlHandle::FirstChild( const wchar_t * value ) const
{
	if ( node )
	{
		TiXmlNode* child = node->FirstChild( value );
		if ( child )
		{
			Cx_Interface<IXmlHandle> pIFRet(ClsidXmlHandle);
			CXmlHandle* pIFCfg = static_cast<CXmlHandle*>(pIFRet.P());
			ASSERT(pIFCfg != NULL);
			pIFCfg->node= child;
			return Cx_Ptr(pIFCfg);
		}
	}
	return Cx_Ptr(NULL);
}


Cx_Ptr CXmlHandle::FirstChildElement() const
{
	if ( node )
	{
		TiXmlElement* child = node->FirstChildElement();
		if ( child )
		{

			Cx_Interface<IXmlHandle> pIFRet(ClsidXmlHandle);
			CXmlHandle* pIFCfg = static_cast<CXmlHandle*>(pIFRet.P());
			ASSERT(pIFCfg != NULL);
			pIFCfg->node= child;
			return Cx_Ptr(pIFCfg);
		}
	}
	return Cx_Ptr(NULL);
}


Cx_Ptr CXmlHandle::FirstChildElement( const wchar_t * value ) const
{
	if ( node )
	{
		TiXmlElement* child = node->FirstChildElement( value );
		if ( child )
		{

			Cx_Interface<IXmlHandle> pIFRet(ClsidXmlHandle);
			CXmlHandle* pIFCfg = static_cast<CXmlHandle*>(pIFRet.P());
			ASSERT(pIFCfg != NULL);
			pIFCfg->node= child;
			return Cx_Ptr(pIFCfg);
		}
	}
	return Cx_Ptr(NULL);
}


Cx_Ptr CXmlHandle::Child( int count ) const
{
	if ( node )
	{
		int i;
		TiXmlNode* child = node->FirstChild();
		for (	i=0;
			child && i<count;
			child = child->NextSibling(), ++i )
		{
			// nothing
		}
		if ( child )
		{

			Cx_Interface<IXmlHandle> pIFRet(ClsidXmlHandle);
			CXmlHandle* pIFCfg = static_cast<CXmlHandle*>(pIFRet.P());
			ASSERT(pIFCfg != NULL);
			pIFCfg->node= child;
			return Cx_Ptr(pIFCfg);
		}
	}
	return Cx_Ptr(NULL);
}


Cx_Ptr CXmlHandle::Child( const wchar_t* value, int count ) const
{
	if ( node )
	{
		int i;
		TiXmlNode* child = node->FirstChild( value );
		for (	i=0;
			child && i<count;
			child = child->NextSibling( value ), ++i )
		{
			// nothing
		}
		if ( child )
		{

			Cx_Interface<IXmlHandle> pIFRet(ClsidXmlHandle);
			CXmlHandle* pIFCfg = static_cast<CXmlHandle*>(pIFRet.P());
			ASSERT(pIFCfg != NULL);
			pIFCfg->node= child;
			return Cx_Ptr(pIFCfg);
		}
	}
	return Cx_Ptr(NULL);
}


Cx_Ptr CXmlHandle::ChildElement( int count ) const
{
	if ( node )
	{
		int i;
		TiXmlElement* child = node->FirstChildElement();
		for (	i=0;
			child && i<count;
			child = child->NextSiblingElement(), ++i )
		{
			// nothing
		}
		if ( child )
		{

			Cx_Interface<IXmlHandle> pIFRet(ClsidXmlHandle);
			CXmlHandle* pIFCfg = static_cast<CXmlHandle*>(pIFRet.P());
			ASSERT(pIFCfg != NULL);
			pIFCfg->node= child;
			return Cx_Ptr(pIFCfg);
		}
	}
	return Cx_Ptr(NULL);
}


Cx_Ptr CXmlHandle::ChildElement( const wchar_t* value, int count ) const
{
	if ( node )
	{
		int i;
		TiXmlElement* child = node->FirstChildElement( value );
		for (	i=0;
			child && i<count;
			child = child->NextSiblingElement( value ), ++i )
		{
			// nothing
		}
		if ( child )
		{

			Cx_Interface<IXmlHandle> pIFRet(ClsidXmlHandle);
			CXmlHandle* pIFCfg = static_cast<CXmlHandle*>(pIFRet.P());
			ASSERT(pIFCfg != NULL);
			pIFCfg->node= child;
			return Cx_Ptr(pIFCfg);
		}
	}
	return Cx_Ptr(NULL);
}


// x3c - C++ PluginFramework
#ifndef _X3_CONFIGXML_IXMLELEMENT_H
#define _X3_CONFIGXML_IXMLELEMENT_H

#include <UtilFunc/PluginInc.h>

#include "ConfigType.h"


class IXmlHandle : public Ix_Object
{
public:
    X3DEFINE_IID(IXmlHandle)
		
public:
	/// Return a handle to the first child node.
	virtual Cx_Ptr FirstChild() const = 0;
	/// Return a handle to the first child node with the given name.
	virtual Cx_Ptr FirstChild( const wchar_t * value ) const = 0;
	/// Return a handle to the first child element.
	virtual Cx_Ptr FirstChildElement() const = 0;
	/// Return a handle to the first child element with the given name.
	virtual Cx_Ptr FirstChildElement( const wchar_t * value ) const = 0;

	/** Return a handle to the "index" child with the given name. 
		The first child is 0, the second 1, etc.
	*/
	virtual Cx_Ptr Child( const wchar_t* value, int index ) const = 0;
	/** Return a handle to the "index" child. 
		The first child is 0, the second 1, etc.
	*/
	virtual Cx_Ptr Child( int index ) const = 0;
	/** Return a handle to the "index" child element with the given name. 
		The first child element is 0, the second 1, etc. Note that only TiXmlElements
		are indexed: other types are not counted.
	*/
	virtual Cx_Ptr ChildElement( const wchar_t* value, int index ) const = 0;
	/** Return a handle to the "index" child element. 
		The first child element is 0, the second 1, etc. Note that only TiXmlElements
		are indexed: other types are not counted.
	*/
	virtual Cx_Ptr ChildElement( int index ) const = 0;

//	#ifdef TIXML_USE_STL
	virtual Cx_Ptr FirstChild( const std::wstring& _value ) const = 0;
	virtual Cx_Ptr FirstChildElement( const std::wstring& _value ) const = 0;

	virtual Cx_Ptr Child( const std::wstring& _value, int index ) const = 0;
	virtual Cx_Ptr ChildElement( const std::wstring& _value, int index ) const = 0;
//	#endif

	/** Return the handle as a TiXmlNode. This may return null.
	*/
	virtual TiXmlNode* ToNode() const = 0;
	/** Return the handle as a TiXmlElement. This may return null.
	*/
	virtual TiXmlElement* ToElement() const = 0;
	/**	Return the handle as a TiXmlText. This may return null.
	*/
	virtual TiXmlText* ToText() const = 0;
	/** Return the handle as a TiXmlUnknown. This may return null.
	*/
	virtual TiXmlUnknown* ToUnknown() const = 0;

	/** @deprecated use ToNode. 
		Return the handle as a TiXmlNode. This may return null.
	*/
	virtual TiXmlNode* Node() const = 0;
	/** @deprecated use ToElement. 
		Return the handle as a TiXmlElement. This may return null.
	*/
	virtual TiXmlElement* Element() const = 0;
	/**	@deprecated use ToText()
		Return the handle as a TiXmlText. This may return null.
	*/
	virtual TiXmlText* Text() const = 0;
	/** @deprecated use ToUnknown()
		Return the handle as a TiXmlUnknown. This may return null.
	*/
	virtual TiXmlUnknown* Unknown() const = 0;
};

#endif // _X3_CONFIGXML_CONFIGXML_H

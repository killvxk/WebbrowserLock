#ifndef SYSTEM_PLUGIN_CONFIG_H
#define SYSTEM_PLUGIN_CONFIG_H

//////////////////////////////////////////////////////////////////////////
// 引用其他头文件

#include <UtilFunc/PluginInc.h>


#include <Xml/IXmlHandle.h>

//////////////////////////////////////////////////////////////////////////
// 引用外部头文件


#include <Function.Internal.CommonControl/CommonControlTool.h>
#include <Function.Internal.HookControl/HookControlTool.h>

//////////////////////////////////////////////////////////////////////////
// 定义全局宏
static const X3CLSID ClsidXmlHandle("3D24C20B-E453-4BD9-8B44-932794CED8B9");

//////////////////////////////////////////////////////////////////////////
// API定义

class CXmlHandle
	: public IXmlHandle, CCommonControl
{
	X3BEGIN_CLASS_DECLARE(CXmlHandle)
		X3DEFINE_INTERFACE_ENTRY(IXmlHandle)
		X3END_CLASS_DECLARE()
		//REGISTER_PLUGIN_INFO(L"系统信息统计插件",L"V1.1 Bate",L"2013-02-06 12:41:00",L"我的公司",L"版权所有")	
public:
	CXmlHandle() {}
	/// Create a handle from any node (at any depth of the tree.) This can be a null pointer.
	CXmlHandle( TiXmlNode* _node )					{ this->node = _node; }
	/// Copy constructor
	CXmlHandle( const CXmlHandle& ref )			{ this->node = ref.node; }
	CXmlHandle operator=( const CXmlHandle& ref ) { if ( &ref != this ) this->node = ref.node; return *this; }

	/// Return a handle to the first child node.
	virtual Cx_Ptr FirstChild() const;
	/// Return a handle to the first child node with the given name.
	virtual Cx_Ptr FirstChild( const wchar_t * value ) const;
	/// Return a handle to the first child element.
	virtual Cx_Ptr FirstChildElement() const;
	/// Return a handle to the first child element with the given name.
	virtual Cx_Ptr FirstChildElement( const wchar_t * value ) const;

	/** Return a handle to the "index" child with the given name. 
		The first child is 0, the second 1, etc.
	*/
	virtual Cx_Ptr Child( const wchar_t* value, int index ) const;
	/** Return a handle to the "index" child. 
		The first child is 0, the second 1, etc.
	*/
	virtual Cx_Ptr Child( int index ) const;
	/** Return a handle to the "index" child element with the given name. 
		The first child element is 0, the second 1, etc. Note that only TiXmlElements
		are indexed: other types are not counted.
	*/
	virtual Cx_Ptr ChildElement( const wchar_t* value, int index ) const;
	/** Return a handle to the "index" child element. 
		The first child element is 0, the second 1, etc. Note that only TiXmlElements
		are indexed: other types are not counted.
	*/
	virtual Cx_Ptr ChildElement( int index ) const;

//	#ifdef TIXML_USE_STL
	virtual Cx_Ptr FirstChild( const std::wstring& _value ) const				{ return FirstChild( _value.c_str() ); }
	virtual Cx_Ptr FirstChildElement( const std::wstring& _value ) const		{ return FirstChildElement( _value.c_str() ); }

	virtual Cx_Ptr Child( const std::wstring& _value, int index ) const			{ return Child( _value.c_str(), index ); }
	virtual Cx_Ptr ChildElement( const std::wstring& _value, int index ) const	{ return ChildElement( _value.c_str(), index ); }
//	#endif

	/** Return the handle as a TiXmlNode. This may return null.
	*/
	virtual TiXmlNode* ToNode() const			{ return node; } 
	/** Return the handle as a TiXmlElement. This may return null.
	*/
	virtual TiXmlElement* ToElement() const		{ return ( ( node && node->ToElement() ) ? node->ToElement() : 0 ); }
	/**	Return the handle as a TiXmlText. This may return null.
	*/
	virtual TiXmlText* ToText() const			{ return ( ( node && node->ToText() ) ? node->ToText() : 0 ); }
	/** Return the handle as a TiXmlUnknown. This may return null.
	*/
	virtual TiXmlUnknown* ToUnknown() const		{ return ( ( node && node->ToUnknown() ) ? node->ToUnknown() : 0 ); }

	/** @deprecated use ToNode. 
		Return the handle as a TiXmlNode. This may return null.
	*/
	virtual TiXmlNode* Node() const			{ return ToNode(); } 
	/** @deprecated use ToElement. 
		Return the handle as a TiXmlElement. This may return null.
	*/
	virtual TiXmlElement* Element() const	{ return ToElement(); }
	/**	@deprecated use ToText()
		Return the handle as a TiXmlText. This may return null.
	*/
	virtual TiXmlText* Text() const			{ return ToText(); }
	/** @deprecated use ToUnknown()
		Return the handle as a TiXmlUnknown. This may return null.
	*/
	virtual TiXmlUnknown* Unknown() const	{ return ToUnknown(); }

private:
	TiXmlNode* node;
};

#endif
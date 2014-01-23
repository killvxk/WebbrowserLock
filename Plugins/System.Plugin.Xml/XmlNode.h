#ifndef SYSTEM_PLUGIN_CONFIG_H
#define SYSTEM_PLUGIN_CONFIG_H

//////////////////////////////////////////////////////////////////////////
// 引用其他头文件

#include <UtilFunc/PluginInc.h>


#include <Xml/IXmlNode.h>

//////////////////////////////////////////////////////////////////////////
// 引用外部头文件

#include <Function.Internal.CommonControl/CommonControlTool.h>

//////////////////////////////////////////////////////////////////////////
// 定义全局宏<Guid("EE590B91-A4A5-485E-AD55-798E040A0168")>
static const X3CLSID ClsidConfigdata("EE590B91-A4A5-485E-AD55-798E040A0168");

//////////////////////////////////////////////////////////////////////////
// API定义


/** The parent class for everything in the Document Object Model.
	(Except for attributes).
	Nodes have siblings, a parent, and children. A node can be
	in a document, or stand on its own. The type of a TiXmlNode
	can be queried, and it can be cast to its more defined type.
*/
class CXmlNode
	: public IXmlNode , public TiXmlBase, CCommonControl
{
	X3BEGIN_CLASS_DECLARE(CXmlNode)
		X3DEFINE_INTERFACE_ENTRY(IXmlNode)
		X3END_CLASS_DECLARE()
		//REGISTER_PLUGIN_INFO(L"系统信息统计插件",L"V1.1 Bate",L"2013-02-06 12:41:00",L"我的公司",L"版权所有")
	friend class TiXmlDocument;
	friend class TiXmlElement;

public:
//#ifdef TIXML_USE_STL	

	    /** An input stream operator, for every class. Tolerant of newlines and
		    formatting, but doesn't expect them.
	    */
	    friend std::istream& operator >> (std::istream& in, CXmlNode& base);

	    /** An output stream operator, for every class. Note that this outputs
		    without any newlines or formatting, as opposed to Print(), which
		    includes tabs and new lines.

		    The operator<< and operator>> are not completely symmetric. Writing
		    a node to a stream is very well defined. You'll get a nice stream
		    of output, without any extra whitespace or newlines.
		    
		    But reading is not as well defined. (As it always is.) If you create
		    a TiXmlElement (for example) and read that from an input stream,
		    the text needs to define an element or junk will result. This is
		    true of all input streams, but it's worth keeping in mind.

		    A TiXmlDocument will read nodes until it reads a root element, and
			all the children of that root element.
	    */	
	    friend std::ostream& operator<< (std::ostream& out, const CXmlNode& base);

		/// Appends the XML node or attribute to a std::string.
		friend std::string& operator<< (std::string& out, const CXmlNode& base );

//#endif

	virtual ~CXmlNode();

	/** The meaning of 'value' changes for the specific type of
		TiXmlNode.
		@verbatim
		Document:	filename of the xml file
		Element:	name of the element
		Comment:	the comment text
		Unknown:	the tag contents
		Text:		the text string
		@endverbatim

		The subclasses will wrap this function.
	*/
	virtual const wchar_t *Value() const { return value.c_str (); }

 //   #ifdef TIXML_USE_STL
	/** Return Value() as a std::string. If you only use STL,
	    this is more efficient than calling Value().
		Only available in STL mode.
	*/
	virtual const std::wstring& ValueStr() const { return value; }
//	#endif

	virtual const TIXML_STRING& ValueTStr() const { return value; }

	/** Changes the value of the node. Defined as:
		@verbatim
		Document:	filename of the xml file
		Element:	name of the element
		Comment:	the comment text
		Unknown:	the tag contents
		Text:		the text string
		@endverbatim
	*/
	virtual void SetValue(const wchar_t * _value) { value = _value;}

//    #ifdef TIXML_USE_STL
	/// STL std::string form.
	virtual void SetValue( const std::wstring& _value )	{ value = _value; }
//	#endif

	/// Delete all the children of this node. Does not affect 'this'.
	virtual void Clear();

	/// One step up the DOM.
	virtual Cx_Ptr Parent()							{ return Cx_Ptr(parent); }
	virtual const Cx_Ptr Parent() const				{ return Cx_Ptr(parent); }

	virtual const Cx_Ptr FirstChild()	const		{ return Cx_Ptr(firstChild); }	///< The first child of this node. Will be null if there are no children.
	virtual Cx_Ptr FirstChild()						{ return Cx_Ptr(firstChild); }
	virtual const Cx_Ptr FirstChild( const wchar_t * value ) const;			///< The first child of this node with the matching 'value'. Will be null if none found.
	/// The first child of this node with the matching 'value'. Will be null if none found.
	virtual Cx_Ptr FirstChild( const wchar_t * _value ) {
		// Call through to the const version - safe since nothing is changed. Exiting syntax: cast this to a const (always safe)
		// call the method, cast the return back to non-const.
		return ((const_cast< const CXmlNode* >(this))->FirstChild( _value ));
	}
	virtual const Cx_Ptr LastChild() const	{ return Cx_Ptr(lastChild); }		/// The last child of this node. Will be null if there are no children.
	virtual Cx_Ptr LastChild()	{ return Cx_Ptr(lastChild); }
	
	virtual const Cx_Ptr LastChild( const wchar_t * value ) const;			/// The last child of this node matching 'value'. Will be null if there are no children.
	virtual Cx_Ptr LastChild( const wchar_t * _value ) {
		return ((const_cast< const CXmlNode* >(this))->LastChild( _value ));
	}

//    #ifdef TIXML_USE_STL
	virtual const Cx_Ptr FirstChild( const std::wstring& _value ) const	{	return FirstChild (_value.c_str ());	}	///< STL std::string form.
	virtual Cx_Ptr FirstChild( const std::wstring& _value )				{	return FirstChild (_value.c_str ());	}	///< STL std::string form.
	virtual const Cx_Ptr LastChild( const std::wstring& _value ) const	{	return LastChild (_value.c_str ());	}	///< STL std::string form.
	virtual Cx_Ptr LastChild( const std::wstring& _value )				{	return LastChild (_value.c_str ());	}	///< STL std::string form.
//	#endif

	/** An alternate way to walk the children of a node.
		One way to iterate over nodes is:
		@verbatim
			for( child = parent->FirstChild(); child; child = child->NextSibling() )
		@endverbatim

		IterateChildren does the same thing with the syntax:
		@verbatim
			child = 0;
			while( child = parent->IterateChildren( child ) )
		@endverbatim

		IterateChildren takes the previous child as input and finds
		the next one. If the previous child is null, it returns the
		first. IterateChildren will return null when done.
	*/
	virtual const Cx_Ptr IterateChildren( const Cx_Ptr* previous ) const;
	virtual Cx_Ptr IterateChildren( const Cx_Ptr* previous ) {
		return ( (const_cast< const CXmlNode* >(this))->IterateChildren( previous ) );
	}

	/// This flavor of IterateChildren searches for children with a particular 'value'
	virtual const Cx_Ptr IterateChildren( const wchar_t * value, const Cx_Ptr* previous ) const;
	virtual Cx_Ptr IterateChildren( const wchar_t * _value, const Cx_Ptr* previous ) {
		return ( (const_cast< const CXmlNode* >(this))->IterateChildren( _value, previous ) );
	}

//    #ifdef TIXML_USE_STL
	virtual const Cx_Ptr IterateChildren( const std::wstring& _value, const Cx_Ptr* previous ) const	{	return IterateChildren (_value.c_str (), previous);	}	///< STL std::string form.
	virtual Cx_Ptr IterateChildren( const std::wstring& _value, const Cx_Ptr* previous ) {	return IterateChildren (_value.c_str (), previous);	}	///< STL std::string form.
//	#endif

	/** Add a new node related to this. Adds a child past the LastChild.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	virtual Cx_Ptr InsertEndChild( const Cx_Ptr& addThis );


	/** Add a new node related to this. Adds a child past the LastChild.

		NOTE: the node to be added is passed by pointer, and will be
		henceforth owned (and deleted) by tinyXml. This method is efficient
		and avoids an extra copy, but should be used with care as it
		uses a different memory model than the other insert functions.

		@sa InsertEndChild
	*/
	virtual Cx_Ptr LinkEndChild( Cx_Ptr* addThis );

	/** Add a new node related to this. Adds a child before the specified child.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	virtual Cx_Ptr InsertBeforeChild( Cx_Ptr* beforeThis, const Cx_Ptr& addThis );

	/** Add a new node related to this. Adds a child after the specified child.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	virtual Cx_Ptr InsertAfterChild(  CXmlNode* afterThis, const Cx_Ptr& addThis );

	/** Replace a child of this node.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	virtual Cx_Ptr ReplaceChild( Cx_Ptr* replaceThis, const Cx_Ptr& withThis );

	/// Delete a child of this node.
	virtual bool RemoveChild( Cx_Ptr* removeThis );

	/// Navigate to a sibling node.
	virtual const Cx_Ptr PreviousSibling() const			{ return prev; }
	virtual Cx_Ptr PreviousSibling()						{ return prev; }

	/// Navigate to a sibling node.
	virtual const Cx_Ptr PreviousSibling( const wchar_t * ) const;
	virtual Cx_Ptr PreviousSibling( const wchar_t *_prev ) {
		return ( (const_cast< const CXmlNode* >(this))->PreviousSibling( _prev ) );
	}

//    #ifdef TIXML_USE_STL
	virtual const Cx_Ptr PreviousSibling( const std::wstring& _value ) const	{	return PreviousSibling (_value.c_str ());	}	///< STL std::string form.
	virtual Cx_Ptr PreviousSibling( const std::wstring& _value ) 			{	return PreviousSibling (_value.c_str ());	}	///< STL std::string form.
	virtual const Cx_Ptr NextSibling( const std::wstring& _value) const		{	return NextSibling (_value.c_str ());	}	///< STL std::string form.
	virtual Cx_Ptr NextSibling( const std::wstring& _value) 					{	return NextSibling (_value.c_str ());	}	///< STL std::string form.
//	#endif

	/// Navigate to a sibling node.
	virtual const Cx_Ptr NextSibling() const				{ return next; }
	virtual Cx_Ptr NextSibling()							{ return next; }

	/// Navigate to a sibling node with the given 'value'.
	virtual const Cx_Ptr NextSibling( const wchar_t * ) const;
	virtual Cx_Ptr NextSibling( const wchar_t* _next ) {
		return ( (const_cast< const CXmlNode* >(this))->NextSibling( _next ) );
	}

	/** Convenience function to get through elements.
		Calls NextSibling and ToElement. Will skip all non-Element
		nodes. Returns 0 if there is not another element.
	*/
	virtual const TiXmlElement* NextSiblingElement() const;
	virtual TiXmlElement* NextSiblingElement() {
		return const_cast< TiXmlElement* >( (const_cast< const CXmlNode* >(this))->NextSiblingElement() );
	}

	/** Convenience function to get through elements.
		Calls NextSibling and ToElement. Will skip all non-Element
		nodes. Returns 0 if there is not another element.
	*/
	virtual const TiXmlElement* NextSiblingElement( const char * ) const;
	virtual TiXmlElement* NextSiblingElement( const char *_next ) {
		return const_cast< TiXmlElement* >( (const_cast< const CXmlNode* >(this))->NextSiblingElement( _next ) );
	}

 //   #ifdef TIXML_USE_STL
	const TiXmlElement* NextSiblingElement( const std::wstring& _value) const	{	return NextSiblingElement (_value.c_str ());	}	///< STL std::string form.
	TiXmlElement* NextSiblingElement( const std::wstring& _value)				{	return NextSiblingElement (_value.c_str ());	}	///< STL std::string form.
//	#endif

	/// Convenience function to get through elements.
	virtual const TiXmlElement* FirstChildElement()	const;
	virtual TiXmlElement* FirstChildElement() {
		return const_cast< TiXmlElement* >( (const_cast< const CXmlNode* >(this))->FirstChildElement() );
	}

	/// Convenience function to get through elements.
	virtual const TiXmlElement* FirstChildElement( const char * _value ) const;
	virtual TiXmlElement* FirstChildElement( const char * _value ) {
		return const_cast< TiXmlElement* >( (const_cast< const CXmlNode* >(this))->FirstChildElement( _value ) );
	}

//    #ifdef TIXML_USE_STL
	virtual const TiXmlElement* FirstChildElement( const std::wstring& _value ) const	{	return FirstChildElement (_value.c_str ());	}	///< STL std::string form.
	virtual TiXmlElement* FirstChildElement( const std::wstring& _value )				{	return FirstChildElement (_value.c_str ());	}	///< STL std::string form.
//	#endif

	/** Query the type (as an enumerated value, above) of this node.
		The possible types are: TINYXML_DOCUMENT, TINYXML_ELEMENT, TINYXML_COMMENT,
								TINYXML_UNKNOWN, TINYXML_TEXT, and TINYXML_DECLARATION.
	*/
	virtual int Type() const	{ return type; }

	/** Return a pointer to the Document this node lives in.
		Returns null if not in a document.
	*/
	virtual const TiXmlDocument* GetDocument() const;
	virtual TiXmlDocument* GetDocument() {
		return const_cast< TiXmlDocument* >( (const_cast< const CXmlNode* >(this))->GetDocument() );
	}

	/// Returns true if this node has no children.
	virtual bool NoChildren() const						{ return !firstChild; }

	virtual const TiXmlDocument*    ToDocument()    const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const TiXmlElement*     ToElement()     const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const TiXmlComment*     ToComment()     const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const TiXmlUnknown*     ToUnknown()     const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const TiXmlText*        ToText()        const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const TiXmlDeclaration* ToDeclaration() const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.

	virtual TiXmlDocument*          ToDocument()    { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual TiXmlElement*           ToElement()	    { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual TiXmlComment*           ToComment()     { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual TiXmlUnknown*           ToUnknown()	    { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual TiXmlText*	            ToText()        { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual TiXmlDeclaration*       ToDeclaration() { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.

	/** Create an exact duplicate of this node and return it. The memory must be deleted
		by the caller. 
	*/
	virtual Cx_Ptr* Clone() const = 0;

	/** Accept a hierchical visit the nodes in the TinyXML DOM. Every node in the 
		XML tree will be conditionally visited and the host will be called back
		via the TiXmlVisitor interface.

		This is essentially a SAX interface for TinyXML. (Note however it doesn't re-parse
		the XML for the callbacks, so the performance of TinyXML is unchanged by using this
		interface versus any other.)

		The interface has been based on ideas from:

		- http://www.saxproject.org/
		- http://c2.com/cgi/wiki?HierarchicalVisitorPattern 

		Which are both good references for "visiting".

		An example of using Accept():
		@verbatim
		TiXmlPrinter printer;
		tinyxmlDoc.Accept( &printer );
		const char* xmlcstr = printer.CStr();
		@endverbatim
	*/
	virtual bool Accept( TiXmlVisitor* visitor ) const = 0;

protected:
	CXmlNode( NodeType _type );

	// Copy to the allocated object. Shared functionality between Clone, Copy constructor,
	// and the assignment operator.
	void CopyTo( CXmlNode* target ) const;

	#ifdef TIXML_USE_STL
	    // The real work of the input operator.
	virtual void StreamIn( std::istream* in, TIXML_STRING* tag ) = 0;
	#endif

	// Figure out what is at *p, and parse it. Returns null if it is not an xml node.
	CXmlNode* Identify( const wchar_t* start, TiXmlEncoding encoding );

	CXmlNode*		parent;
	NodeType		type;

	CXmlNode*		firstChild;
	CXmlNode*		lastChild;

	TIXML_STRING	value;

	CXmlNode*		prev;
	CXmlNode*		next;

private:
	CXmlNode( const CXmlNode& );				// not implemented.
	void operator=( const CXmlNode& base );	// not allowed.
};


#endif
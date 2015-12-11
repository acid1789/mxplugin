#pragma once

#include "IMxPluginBase.h"

static const GUID IPluginXmlElementGUID = { 0xafe9cb64, 0x83b8, 0x4834, { 0xa3, 0x89, 0xb, 0x54, 0x41, 0x33, 0x70, 0x37 } };	// {AFE9CB64-83B8-4834-A389-0B5441337037}
class IPluginXmlElementIID
{
public:
	static const  GUID		  Guid() { return IPluginXmlElementGUID; }		
	static const char * const Name() { return "IPluginXmlElement"; }
};

class IPluginXmlAttribute;
class IPluginXmlElement  :  public IMxPluginBase
{
public:
	static const int		    IPluginXmlElement::MaxValueLength = 500;			//<value>text</value> - strlen("<value>") <= 500
	static const int		    IPluginXmlElement::MaxTextLength = 1000;			//<value>text</value> - strlen("text") <= 1000
	static const int		    IPluginXmlElement::MaxArrayLength = 9999;			//<value ArraySize="9999" L0="2345" L2="33456767" ... L9998="98045">text</value>
	static const int		    IPluginXmlElement::Invalid = -1;	

public:
	virtual const char *		MXSTDMETHOD GetText() = 0;							//<value>text</value> returns text
	virtual bool				MXSTDMETHOD SetText(const char *text) = 0;			//whenever possible use attributes like <value name="text" /> as this avoids a host of issues, though resulting XML is less human-readable
	virtual bool				MXSTDMETHOD IsValidText(const char *text) = 0;		//text must not be null, length < MaxValueLength, cannot have illegal chars - see IPluginDoc::GetElementTextIllegalChars() 

	virtual const char *		MXSTDMETHOD GetValue() = 0;							//<value>text</value> returns value
	virtual bool				MXSTDMETHOD SetValue(const char *value) = 0;
	virtual bool				MXSTDMETHOD IsValidValue(const char *value) = 0;	//value must not be null, length > 0, length < MaxValueLength, cannot start with non-alphabetic char, cannot start with XML or xml, or Xml, ..., cannot have illegal chars - see IPluginDoc::GetElementValueIllegalChars() 

	virtual int					MXSTDMETHOD GetAttributeCount() = 0;				//does not include any parts of AttributeArray
	virtual IPluginXmlAttribute * MXSTDMETHOD GetAttribute(int index) = 0;			//<value name1="x" name2="y"></value> - GetAttribute(1) returns attribute for name2="y" (XML parsers do not consider order to be important, so use only for iterating through attributes in an element)
	virtual bool				MXSTDMETHOD RemoveAttribute(const char *name) = 0;	//<value name1="x" name2="y"></value> - RemoveAttribute("name2") deletes name2="y"

	virtual const char *		MXSTDMETHOD GetAttributeString(const char *name) = 0;						 //<value name="x"></value> - GetAttributeString("name") returns "x"
	virtual bool				MXSTDMETHOD SetAttributeString(const char *name, const char *value) = 0;	 //SetAttributeString("name", "x") sets <value name="x"></value>
	virtual bool				MXSTDMETHOD GetAttributeInt(const char *name, int *value) = 0;				 //<value name="9"></value> - GetAttributeInt("name") returns 9
	virtual bool				MXSTDMETHOD SetAttributeInt(const char *name, int value) = 0;				 //SetAttributeInt("name", 9) sets <value name="9"></value>
	virtual bool				MXSTDMETHOD GetAttributeDouble(const char *name, double *value) = 0;		 //<value name="9.9"></value> - GetAttributeDouble("name") returns 9.9
	virtual bool				MXSTDMETHOD SetAttributeDouble(const char *name, double value) = 0;			 //SetAttributeInt("name", 9.9) sets <value name="9.9"></value>

	virtual int					MXSTDMETHOD GetAttributeArraySize() = 0;									  //<value ArraySize="9" L0="2345" L2="33456767" ... L8="98045">text</value> - returns 9
	virtual int					MXSTDMETHOD GetAttributeArray(int *arrayElement[], int arraySize) = 0;		  //<value ArraySize="9" L0="2345" L2="33456767" ... L8="98045">text</value> - fills arrayElement with the 9 integers and returns 9 (or IPluginXmlElement::Invalid if fails)
	virtual bool				MXSTDMETHOD SetAttributeArray(int *arrayElement[], int arrayElementCnt) = 0;  //creates one attribute for each of the 9 integers in arrayElement (you can only have one arribute array in any one element and it must be set before any other attributes)
	virtual bool				MXSTDMETHOD RemoveAttributeArray() = 0;										  //deletes ArraySize="9" and L0, L1, ... L8
	virtual bool				MXSTDMETHOD HasAttributeArray() = 0;										  //returns true if this element contains an array of ints

	virtual bool				MXSTDMETHOD IsAttributePresent(const char *name, const char *attributeType) = 0;	 //name must not be null, length < IPluginXmlAttribute::MaxNameLength, cannot have illegal chars - see IPluginDoc::GetAttributeNameIllegalChars(); attribType is string (ignore type when checking), double, integer 
	virtual bool				MXSTDMETHOD IsValidAttribName(const char *name) = 0;						  //name must not be null, length < IPluginXmlAttribute::MaxNameLength, cannot have illegal chars - see IPluginDoc::GetAttributeNameIllegalChars() 
	virtual bool				MXSTDMETHOD IsValidAttribValue(const char *value) = 0;						  //value must not be null, length < IPluginXmlAttribute::MaxValueLength, cannot have illegal chars - see IPluginDoc::GetAttributeValueIllegalChars() 	

	virtual int					MXSTDMETHOD GetChildCount() = 0;
	virtual IPluginXmlElement*	MXSTDMETHOD GetChild(int index) = 0;

	virtual int					MXSTDMETHOD GetSiblingCount() = 0;
	virtual IPluginXmlElement*	MXSTDMETHOD GetSibling(int index) = 0;

	virtual IPluginXmlElement*	MXSTDMETHOD GetParent() = 0;
	virtual IPluginXmlElement*	MXSTDMETHOD GetFirstChild(const char *value=nullptr) = 0;
	virtual IPluginXmlElement*	MXSTDMETHOD GetNextSibling(const char *value=nullptr) = 0;
	virtual IPluginXmlElement*	MXSTDMETHOD GetPreviousSibling(const char *value=nullptr) = 0;

	virtual bool				MXSTDMETHOD LinkEndChild(IPluginXmlElement* element) = 0;	//sets OwnedByParent(true)
	virtual void				MXSTDMETHOD OwnedByParent(bool set) = 0;		
	virtual bool				MXSTDMETHOD IsOwnedByParent() = 0;							//does parent own internal data? If not, then internal data is deleted by dtor of class that implements IPluginXmlElement. In this way an IPluginXmlElement passed to LinkEndChild() is owned by IPluginXmlDoc which then takes responsibility for deleting it on Close/Delete
	virtual bool				MXSTDMETHOD RemoveChild(IPluginXmlElement* element) = 0;
	virtual IPluginXmlElement*	MXSTDMETHOD Clone() = 0;
	virtual bool				MXSTDMETHOD Clear() = 0;

	virtual void *				MXSTDMETHOD GetInternalData() = 0;							//Users of PlugXml should never need this - it is only used by PlugXml developers as that is the only place which creates, deletes and uses the internal data - there must be a better way!

};
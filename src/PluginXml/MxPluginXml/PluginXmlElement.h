#pragma once

#ifdef MXPLUGINXML_EXPORTS
#define MXPLUGINXML_API __declspec(dllexport)
#else
#define MXPLUGINXML_API __declspec(dllimport)
#endif

#include "tinyxml.h"
#include "PluginXml\IPluginXmlElement.h"
#include "PluginXmlAttribute.h"

class IPluginXmlDoc;
class MXPLUGINXML_API  PluginXmlElement :	public IPluginXmlElement
{
public:
	static const char * const PluginXmlElement::ValueIllegalChars;
	static const char * const PluginXmlElement::TextIllegalChars;
public:
	PluginXmlElement(void);
	PluginXmlElement(TiXmlElement *element);
	virtual ~PluginXmlElement(void);

	//			//IPluginXmlElement
	virtual bool				MXSTDMETHOD SetText(const char *text);
	virtual const char *		MXSTDMETHOD GetText();
	virtual bool				MXSTDMETHOD IsValidText(const char *text);	

	virtual const char *		MXSTDMETHOD GetValue();
	virtual bool				MXSTDMETHOD SetValue(const char *value);
	virtual bool				MXSTDMETHOD IsValidValue(const char *value);	

	virtual int					MXSTDMETHOD GetAttributeCount();
	virtual IPluginXmlAttribute *MXSTDMETHOD GetAttribute(int index);
	virtual bool				MXSTDMETHOD RemoveAttribute(const char *name);

	virtual const char *		MXSTDMETHOD GetAttributeString(const char *name);
	virtual bool				MXSTDMETHOD SetAttributeString(const char *name, const char *value);
	virtual bool				MXSTDMETHOD GetAttributeInt(const char *name, int *value);
	virtual bool				MXSTDMETHOD SetAttributeInt(const char *name, int value);
	virtual bool				MXSTDMETHOD GetAttributeDouble(const char *name, double *value);
	virtual bool				MXSTDMETHOD SetAttributeDouble(const char *name, double value);

	virtual int					MXSTDMETHOD GetAttributeArraySize();
	virtual int					MXSTDMETHOD GetAttributeArray(int *arrayElement[], int arraySize);
	virtual bool				MXSTDMETHOD SetAttributeArray(int *arrayElement[], int arrayElementCnt);  //you can only have one arribute array in any one element
	virtual bool				MXSTDMETHOD RemoveAttributeArray();
	virtual bool				MXSTDMETHOD HasAttributeArray();

	virtual bool				MXSTDMETHOD IsAttributePresent(const char *name, const char *attributeType);
	virtual bool				MXSTDMETHOD IsValidAttribName(const char *name) { return  PluginXmlAttribute::CheckValidName(name ); }	
	virtual bool				MXSTDMETHOD IsValidAttribValue(const char *value) { return  PluginXmlAttribute::CheckValidValue(value);	}

	virtual int					MXSTDMETHOD GetChildCount();
	virtual IPluginXmlElement*	MXSTDMETHOD GetChild(int index);

	virtual int					MXSTDMETHOD GetSiblingCount();
	virtual IPluginXmlElement*	MXSTDMETHOD GetSibling(int index);

	virtual IPluginXmlElement*	MXSTDMETHOD GetParent();
	virtual IPluginXmlElement*	MXSTDMETHOD GetFirstChild(const char *value=nullptr);
	virtual IPluginXmlElement*	MXSTDMETHOD GetNextSibling(const char *value=nullptr);
	virtual IPluginXmlElement*	MXSTDMETHOD GetPreviousSibling(const char *value=nullptr);

	virtual bool				MXSTDMETHOD LinkEndChild( IPluginXmlElement* element);
	virtual void				MXSTDMETHOD OwnedByParent(bool set) { _ownedByParent = set; }
	virtual bool				MXSTDMETHOD IsOwnedByParent() { return _ownedByParent; }

	virtual bool				MXSTDMETHOD RemoveChild( IPluginXmlElement* element);
	virtual IPluginXmlElement*	MXSTDMETHOD Clone();
	virtual bool				MXSTDMETHOD Clear();

				//IMxPluginBase
	virtual long				MXSTDMETHOD DestroyPtr(void);
	virtual long				MXSTDMETHOD DuplicatePtr(void);

	virtual void *				MXSTDMETHOD Dynamic_cast(const GUID IId);
	virtual const GUID			MXSTDMETHOD GetInterfaceGUID(void)  { return IPluginXmlElementIID::Guid(); }
	virtual const char *		MXSTDMETHOD GetInterfaceName(void) { return IPluginXmlElementIID::Name(); }

	virtual void *				MXSTDMETHOD GetInternalData() { return reinterpret_cast<void *>(_element); }	//must be a better way!

				//support methods
	static  bool				IsArrayAttribute(TiXmlAttribute *attrib);

private:
	TiXmlElement*	_element;
	bool			_ownedByParent;
	long			_refCnt;
	
};


#pragma once

#ifdef MXPLUGINXML_EXPORTS
#define MXPLUGINXML_API __declspec(dllexport)
#else
#define MXPLUGINXML_API __declspec(dllimport)
#endif

#include "tinyxml.h"
#include "PluginXml\IPluginXmlAttribute.h"

class MXPLUGINXML_API PluginXmlAttribute : public IPluginXmlAttribute
{
public:
	static const char * const PluginXmlAttribute::ArrayPrefixName;
	static const char * const PluginXmlAttribute::ArraySizeName;
	static const char * const PluginXmlAttribute::NameIllegalChars;			//<value name="x">Text</value> - chars not allowed in 'name' (see  http://msdn.microsoft.com/en-us/library/ms256152.aspx)
	static const char * const PluginXmlAttribute::ValueIllegalChars;		//<value name="x">Text</value> - chars not allowed in 'x' 
public:
	PluginXmlAttribute(void);
	PluginXmlAttribute(TiXmlAttribute* attribute);
	~PluginXmlAttribute(void);

	virtual const char *		MXSTDMETHOD GetName();
	virtual bool				MXSTDMETHOD SetName(const char *name);
	virtual bool				MXSTDMETHOD GetIntValue(int *value);
	virtual bool				MXSTDMETHOD SetIntValue(int value);
	virtual const char *		MXSTDMETHOD GetStringValue();
	virtual bool				MXSTDMETHOD SetStringValue(const char *value);
	virtual bool				MXSTDMETHOD GetDoubleValue(double *value);
	virtual bool				MXSTDMETHOD SetDoubleValue(double value);

	virtual IPluginXmlAttribute *	MXSTDMETHOD GetNextAttribute();
	virtual IPluginXmlAttribute *	MXSTDMETHOD GetPreviousAttribute();

	virtual void				MXSTDMETHOD OwnedByParent(bool set) { _ownedByParent = set; }
	virtual bool				MXSTDMETHOD IsOwnedByParent() { return _ownedByParent; }

				//IMxPluginBase
	virtual long				MXSTDMETHOD DestroyPtr(void);
	virtual long				MXSTDMETHOD DuplicatePtr(void);
	virtual void *				MXSTDMETHOD Dynamic_cast(const GUID IId);
	virtual const GUID			MXSTDMETHOD GetInterfaceGUID(void)  { return IPluginXmlAttributeIID::Guid(); }
	virtual const char *		MXSTDMETHOD GetInterfaceName(void) { return IPluginXmlAttributeIID::Name(); }

				//support methods
	static  bool				CheckValidName(const char *name);
	static  bool				CheckValidValue(const char *value);
	static  bool				CheckValidXmlString(const char *xmlString, int minLen, int maxLen, const char *illegalChars, bool IsText );
private:
	static  bool				CheckValidPreambleXmlString(const char *xmlString );

private:
	long _refCnt;
	TiXmlAttribute*	_attribute;
	bool			_ownedByParent;

};


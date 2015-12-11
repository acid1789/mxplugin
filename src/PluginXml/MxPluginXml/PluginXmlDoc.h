#pragma once

#ifdef MXPLUGINXML_EXPORTS
#define MXPLUGINXML_API __declspec(dllexport)
#else
#define MXPLUGINXML_API __declspec(dllimport)
#endif


#include "tinyxml.h"
#include "PluginXml\IPluginXmlDoc.h"
#include "PluginXmlElement.h"
#include "PluginXmlAttribute.h"

class MXPLUGINXML_API PluginXmlDoc : IPluginXmlDoc
{
public:
	PluginXmlDoc(void);
	~PluginXmlDoc(void);

	virtual bool				MXSTDMETHOD New(const char *value, const char *comment, const char *version, bool standalone);
	virtual bool				MXSTDMETHOD Open(const char *pathfilename);
	virtual bool				MXSTDMETHOD Save();
	virtual bool				MXSTDMETHOD SaveAs(const char *pathfilename);
	virtual bool				MXSTDMETHOD Close();

	virtual const char *		MXSTDMETHOD GetPathFilename();
	virtual const char *		MXSTDMETHOD GetVersion();
	virtual IPluginXmlElement*	MXSTDMETHOD GetRootElement();

	virtual const char *		MXSTDMETHOD GetElementValueIllegalChars() { return PluginXmlElement::ValueIllegalChars; } 	//<value name="x">text</value> - characters not allowed for value
	virtual const char *		MXSTDMETHOD GetElementTextIllegalChars() { return PluginXmlElement::TextIllegalChars; } 	//<value name="x">text</value> - characters not allowed for text
	virtual const char *		MXSTDMETHOD GetAttribNameIllegalChars() { return PluginXmlAttribute::NameIllegalChars; } 	//<value name="x">text</value> - characters not allowed for name
	virtual const char *		MXSTDMETHOD GetAttribValueIllegalChars() { return PluginXmlAttribute::ValueIllegalChars; }	//<value name="xyz">text</value> - characters not allowed for xyz

	virtual bool				MXSTDMETHOD Validate(const char *xmlSchemaPathFilename=nullptr);

	virtual bool				MXSTDMETHOD IsError(void) { return ((_error == false) && (_doc != nullptr) && (_doc->Error() == false)) ? false : true; }			//Is there an error in this object or the TinyXML document?

	virtual bool 				MXSTDMETHOD ClearError(void);									//clear the TinyXML error
	virtual int					MXSTDMETHOD GetErrorId(void)  { return ((_doc != nullptr) ? _doc->ErrorId() :  IPluginXmlDoc::Invalid); }			//return the TinyXML error code
	virtual int					MXSTDMETHOD GetErrorRow(void)  { return ((_doc != nullptr) ? _doc->ErrorRow() :  IPluginXmlDoc::Invalid); }		//return the TinyXML error location, IPluginXmlError::Invalid if not known
	virtual int					MXSTDMETHOD GetErrorCol(void) { return ((_doc != nullptr) ? _doc->ErrorCol() :  IPluginXmlDoc::Invalid); }			//return the TinyXML error location, IPluginXmlError::Invalid if not known
	virtual const char *		MXSTDMETHOD GetErrorDescr(void) { return  ((_doc != nullptr) ? _doc->ErrorDesc() :  IPluginXmlDocInvalidStr); }	//return the TinyXML error description

				//IMxPluginBase
	virtual long				MXSTDMETHOD DestroyPtr(void);
	virtual long				MXSTDMETHOD DuplicatePtr(void);
	virtual void *				MXSTDMETHOD Dynamic_cast(const GUID IId);
	virtual const GUID			MXSTDMETHOD GetInterfaceGUID(void)  { return IPluginXmlDocIID::Guid(); }
	virtual const char *		MXSTDMETHOD GetInterfaceName(void) { return IPluginXmlDocIID::Name(); }
private:

	TiXmlDocument	*	_doc;
	PluginXmlElement*	_root;
	bool				_error;
	long				_refCnt;
};


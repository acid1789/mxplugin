#pragma once

#include "IMxPluginBase.h"
#include "IPluginXmlElement.h"

static const GUID IPluginXmlDocGUID = { 0xf1c886df, 0xfd0e, 0x4fdc, { 0x96, 0x53, 0xb7, 0xad, 0xdf, 0xd6, 0xb9, 0x97 } };		// {F1C886DF-FD0E-4FDC-9653-B7ADDFD6B997}
class IPluginXmlDocIID
{
public:
	static const  GUID		   Guid() { return IPluginXmlDocGUID; }		
	static const  char * const Name() { return "IPluginXmlDoc"; }
};

#define IPluginXmlDocInvalidStr "[invalid]"									//replaces static const char * const IPluginXmlDoc::InvalidStr - no need to define in a .cpp;


class IPluginXmlDoc  :  public IMxPluginBase
{
public:
	static const int		  IPluginXmlDoc::Invalid = -1;
	static const int		  IPluginXmlDoc::MaxCommentLength = 250;			
	static const int		  IPluginXmlDoc::MaxVersionLength = 14;			//9999.9999.9999
	static const int		  IPluginXmlDoc::MaxPathFilenameLength = 260;	//same as MAX_PATH

public:
	virtual bool				MXSTDMETHOD New(const char *value, const char *comment, const char *version, bool standalone) = 0;
	virtual bool				MXSTDMETHOD Open(const char *pathfilename) = 0;
	virtual bool				MXSTDMETHOD Save() = 0;
	virtual bool				MXSTDMETHOD SaveAs(const char *pathfilename) = 0;
	virtual bool				MXSTDMETHOD Close() = 0;

	virtual bool				MXSTDMETHOD Validate(const char *xmlSchemaPathFilename=nullptr) = 0;

	virtual const char *		MXSTDMETHOD GetPathFilename() = 0;
	virtual const char *		MXSTDMETHOD GetVersion() = 0;
	virtual IPluginXmlElement*	MXSTDMETHOD GetRootElement() = 0;

	virtual const char *		MXSTDMETHOD GetElementValueIllegalChars() = 0;	//<value name="x">text</value> - characters not allowed in 'value' (see  http://msdn.microsoft.com/en-us/library/ms256152.aspx)
	virtual const char *		MXSTDMETHOD GetElementTextIllegalChars() = 0;	//<value name="x">text</value> - characters not allowed in 'text' - text should not contain embedded markup <b> or char/entity refs, no check for control chars / unicode chars 
	virtual const char *		MXSTDMETHOD GetAttribNameIllegalChars() = 0;	//<value name="x">text</value> - characters not allowed in 'name'
	virtual const char *		MXSTDMETHOD GetAttribValueIllegalChars() = 0;	//<value name="xyz">text</value> - characters not allowed in 'xyz' - & is allowed so you can represent <,>," chars as entity/char refs

	virtual bool			 MXSTDMETHOD IsError(void) = 0;					//Is there an error in the TinyXML document?
	virtual bool			 MXSTDMETHOD ClearError(void) = 0;				//clear the TinyXML error, called by the various GetError() methods after they initialise an PluginXmlError object
	virtual int				 MXSTDMETHOD GetErrorId(void) = 0;				//return the TinyXML error code
	virtual int				 MXSTDMETHOD GetErrorRow(void) = 0;				//return the TinyXML error location, IPluginXmlError::Invalid if not known
	virtual int				 MXSTDMETHOD GetErrorCol(void) = 0;				//return the TinyXML error location, IPluginXmlError::Invalid if not known
	virtual const char *	 MXSTDMETHOD GetErrorDescr(void) = 0;			//return the TinyXML error description

};
#pragma once

#include "IMxPluginBase.h"

static const GUID IPluginXmlAttributeGUID = { 0x57469e5c, 0x47cb, 0x469b, { 0x8f, 0x9, 0xc4, 0x63, 0xf6, 0xb3, 0x65, 0xe1 } };	// {57469E5C-47CB-469B-8F09-C463F6B365E1}
class IPluginXmlAttributeIID
{
public:
	static const  GUID		   Guid() { return IPluginXmlAttributeGUID; }		
	static const  char * const Name() { return "IPluginXmlAttribute"; }
};

class IPluginXmlAttribute :  public IMxPluginBase
{
public:
	static const int					IPluginXmlAttribute::MaxNameLength = 500;			    //<value name="x"></value> - strlen("name") <= 500
	static const int					IPluginXmlAttribute::MaxValueLength = 500;				//<value name="x"></value> - strlen("x") <= 500
public:
	virtual const char *				MXSTDMETHOD GetName() = 0;
	virtual bool						MXSTDMETHOD SetName(const char *name) = 0;
	virtual bool						MXSTDMETHOD GetIntValue(int *value) = 0;
	virtual bool						MXSTDMETHOD SetIntValue(int value) = 0;
	virtual const char *				MXSTDMETHOD GetStringValue() = 0;
	virtual bool						MXSTDMETHOD SetStringValue(const char *value) = 0;
	virtual bool						MXSTDMETHOD GetDoubleValue(double *value) = 0;
	virtual bool						MXSTDMETHOD SetDoubleValue(double value) = 0;

	virtual IPluginXmlAttribute *	MXSTDMETHOD GetNextAttribute() = 0;
	virtual IPluginXmlAttribute *	MXSTDMETHOD GetPreviousAttribute() = 0;

	virtual void						MXSTDMETHOD OwnedByParent(bool set) = 0;
	virtual bool						MXSTDMETHOD IsOwnedByParent() = 0;
				

};
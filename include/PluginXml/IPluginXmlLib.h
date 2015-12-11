#pragma once

#include "IMxPluginLibDetails.h"

static const GUID IPluginXmlLibGUID = { 0xc343cfa5, 0xf381, 0x4556, { 0xb1, 0x70, 0x54, 0x9f, 0xe9, 0x41, 0x77, 0xf2 } };	// {C343CFA5-F381-4556-B170-549FE94177F2}
class IPluginXmlLibIID
{
public:
	static const  GUID	       Guid() { return IPluginXmlLibGUID; }		//generate unique GUID for the interface using Tools | Create GUID	
	static const  char * const Name() { return "IPluginXmlLib"; }
};

class IPluginXmlLib  :  public IMxPluginLibDetails
{
public:
	virtual const char *MXSTDMETHOD GetComment(void) = 0;	
				

};
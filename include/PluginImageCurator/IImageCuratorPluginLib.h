#pragma once
#include "IMxPluginBase.h"
#include "IMxPluginLibDetails.h"

static const GUID IImageCuratorPluginLibGUID = { 0x74c45fa7, 0x592e, 0x419f, { 0x9a, 0x8c, 0xa5, 0xaf, 0x8d, 0xd3, 0x60, 0x35 }};  // {74C45FA7-592E-419F-9A8C-A5AF8DD36035}
class IImageCuratorPluginLibIID
{
public:
	static const GUID		  Guid() { return IImageCuratorPluginLibGUID; }
	static const char * const Name() { return "Interface implemented by ImageCurator Plugins allowing them to be invoked by ICApps etc"; }
};

class IImageCurator;
class  IImageCuratorPluginLib :  public IMxPluginBase
{
public:

virtual const char *MXSTDMETHOD GetProductID() = 0;
virtual const char *MXSTDMETHOD GetProductName() = 0;
virtual const char *MXSTDMETHOD GetVersion() = 0;
virtual const char *MXSTDMETHOD GetDescription() = 0;
virtual int			MXSTDMETHOD GetToolboxIconID() = 0;
virtual const char *MXSTDMETHOD GetToolboxLabel() = 0;

virtual IImageCurator*	MXSTDMETHOD GetIImageCurator() = 0;

virtual bool		MXSTDMETHOD IsValid() = 0;

};
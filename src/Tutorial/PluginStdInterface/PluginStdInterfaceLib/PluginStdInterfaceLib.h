#pragma once

#ifdef PLUGINSTDINTERFACELIB_EXPORTS
#define PLUGINSTDINTERFACELIB_API __declspec(dllexport)
#else
#define PLUGINSTDINTERFACELIB_API __declspec(dllimport)
#endif

#include "..\..\..\PublicInterfaces\Tutorial\IPluginStdInterface.h"

extern "C" void  *MXSTDMETHOD CreateClassInstance(const GUID IId);

class PLUGINSTDINTERFACELIB_API PluginStdInterfaceLib : public IPluginStdInterfaceLib
{
public:
	PluginStdInterfaceLib(void);
	virtual ~PluginStdInterfaceLib(void);

//implementation of IMxPluginBase
	virtual long MXSTDMETHOD DestroyPtr(void);
	virtual long MXSTDMETHOD DuplicatePtr(void);
	virtual void *MXSTDMETHOD Dynamic_cast(const GUID IId );
	virtual const GUID MXSTDMETHOD GetInterfaceGUID(void) { return IPluginStdInterfaceLibIID::Guid(); }
	virtual const char *MXSTDMETHOD GetInterfaceName(void) { return IPluginStdInterfaceLibIID::Name(); }

//implementation of IPluginStdInterfaceLib
	virtual const char *MXSTDMETHOD GetComment() { return "PluginStdInterface::GetComment() is invoked"; } 

private:
	long _refCnt;
};

// PluginStdInterfaceLib.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "PluginStdInterfaceLib.h"


void  *MXSTDMETHOD CreateClassInstance(const GUID IId)
{
	void *rc = nullptr;

	if ( MxGUID::IsEqual(IId, IPluginStdInterfaceLibIID::Guid()))
		rc = new PluginStdInterfaceLib();
	else if ( MxGUID::IsEqual(IId, IMxPluginBaseIID::Guid()))
		rc = new PluginStdInterfaceLib();
	else
		rc = nullptr;

	if (rc != nullptr)
		((IMxPluginBase *)rc)->DuplicatePtr();

	return rc;
}

PluginStdInterfaceLib::PluginStdInterfaceLib(void) : 
	_refCnt(0)
{
}


PluginStdInterfaceLib::~PluginStdInterfaceLib(void)
{
}

long MXSTDMETHOD PluginStdInterfaceLib::DestroyPtr(void)
{
	long cnt = InterlockedDecrement(&_refCnt);
	if ( cnt == 0 )
		delete this;
	return cnt;
}

long MXSTDMETHOD PluginStdInterfaceLib::DuplicatePtr(void)
{
	return InterlockedIncrement(&_refCnt);
}

void *MXSTDMETHOD PluginStdInterfaceLib::Dynamic_cast(const GUID IId)
{
	void *rc = nullptr;

	if ( MxGUID::IsEqual(IId, IPluginStdInterfaceLibIID::Guid()))
		rc = static_cast<IPluginStdInterfaceLib *>(this);
	else if ( MxGUID::IsEqual(IId, IMxPluginBaseIID::Guid()))
		rc = static_cast<IMxPluginBase *>(this);
	else
		rc = nullptr;

	if (rc != nullptr)
		((IMxPluginBase *)rc)->DuplicatePtr();

	return rc;
}



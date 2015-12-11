// PluginImageProcXLib.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <string>

#include "gLibDetails.h"
#include "PluginImageProcXLib.h"

void  *MXSTDMETHOD CreateClassInstance(const char *IId)
{
	void *rc = '\0';

	if (strncmp(IId, IPluginImageProcXLibIID::GUID(), IMxPluginBaseIID::IIdSize) == 0)
		rc = new PluginImageProcXLib();
	else if (strncmp(IId, IMxPluginLibDetailsIID::GUID(), IMxPluginBaseIID::IIdSize) == 0)
		rc = new PluginImageProcXLib();
	else if (strncmp(IId, IMxPluginBaseIID::GUID(), IMxPluginBaseIID::IIdSize) == 0)
		rc = new PluginImageProcXLib();
	else
		rc = '\0';

	if (rc != '\0')
		((IMxPluginBase *)rc)->DuplicatePtr();

	return rc;
}



PluginImageProcXLib::PluginImageProcXLib(void): 
	_refCnt(0)
{
}


PluginImageProcXLib::~PluginImageProcXLib(void)
{
}


long   MXSTDMETHOD PluginImageProcXLib::DestroyPtr(void)
{
	long cnt = InterlockedDecrement(&_refCnt);
	if ( cnt == 0 )
		delete this;
	return cnt;
}

long   MXSTDMETHOD PluginImageProcXLib::DuplicatePtr(void)
{
	return InterlockedIncrement(&_refCnt);
}


void * MXSTDMETHOD PluginImageProcXLib::Dynamic_cast(const char *IId)
{
	void *rc = '\0';

	if (IId != '\0')
	{
		if (strncmp (IPluginImageProcXLibIID::GUID(), IId, IMxPluginBaseIID::IIdSize) == 0)
			rc = static_cast<IPluginImageProcXLib *>(this);
		else if (strncmp (IMxPluginLibDetailsIID::GUID(), IId, IMxPluginBaseIID::IIdSize) == 0)
			rc = static_cast<IMxPluginLibDetails *>(this);
		else if (strncmp (IMxPluginBaseIID::GUID(), IId, IMxPluginBaseIID::IIdSize) == 0)
			rc = static_cast<IMxPluginBase *>(this);
		else
			rc = '\0';

		if (rc != '\0')
			((IMxPluginBase *)rc)->DuplicatePtr();
	}

	return rc;
}
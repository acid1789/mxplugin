// PluginStdDetailsLib.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "PluginStdDetailsLib.h"

void  *MXSTDMETHOD CreateClassInstance(const GUID IId)
{
	void *rc = nullptr;

	if ( MxGUID::IsEqual(IId, IPluginStdLibIID::Guid()))
		rc = new PluginStdDetailsLib();
	else if ( MxGUID::IsEqual(IId, IMxPluginLibDetailsIID::Guid()))
		rc = new PluginStdDetailsLib();
	else if ( MxGUID::IsEqual(IId, IMxPluginBaseIID::Guid()))
		rc = new PluginStdDetailsLib();
	else
		rc = nullptr;

	if (rc == nullptr)
	{
		std::string tmp;
		MX_SETERROR(MX1201, MxError::Install, MxError::Abort, MxError::QuietReport, "CreateClassInstance, unsupported IID=%s", MxGUID::ConvertToString(IId, &tmp));
	}
	else
		((IMxPluginBase *)rc)->DuplicatePtr();

	return rc;
}


PluginStdDetailsLib::PluginStdDetailsLib(void): 
	_refCnt(0)
{
}


PluginStdDetailsLib::~PluginStdDetailsLib(void)
{
}


long MXSTDMETHOD PluginStdDetailsLib::DestroyPtr(void)
{
	long cnt = InterlockedDecrement(&_refCnt);
	if ( cnt == 0 )
		delete this;
	return cnt;
}

long MXSTDMETHOD PluginStdDetailsLib::DuplicatePtr(void)
{
	return InterlockedIncrement(&_refCnt);
}

void *MXSTDMETHOD PluginStdDetailsLib::Dynamic_cast(const GUID IId)
{
	void *rc = nullptr;

	if ( MxGUID::IsEqual(IId, IPluginStdLibIID::Guid()))
		rc = static_cast<IPluginStdLib *>(this);
	else if ( MxGUID::IsEqual(IId, IMxPluginLibDetailsIID::Guid()))
		rc = static_cast<IMxPluginLibDetails *>(this);
	else if ( MxGUID::IsEqual(IId, IMxPluginBaseIID::Guid()))
		rc = static_cast<IMxPluginBase *>(this);
	else
		rc = nullptr;

	if (rc == nullptr)
	{
		std::string tmp;
		MX_SETERROR(MX1202, MxError::Install, MxError::Abort, MxError::VerboseReport, "Dynamic_cast does not support IID=%s", MxGUID::ConvertToString(IId, &tmp));
	}
	else
		((IMxPluginBase *)rc)->DuplicatePtr();

	return rc;
}

int			MXSTDMETHOD PluginStdDetailsLib::GetInterfaceCount()
{ 
	return 3; 
}

bool		MXSTDMETHOD PluginStdDetailsLib::IsInterfaceSupported(const GUID iid)
{ 
	bool rc = false;

	if( MxGUID::IsEqual(iid, IPluginStdLibIID::Guid()) )
		rc = true;
	else
	{
		if( MxGUID::IsEqual(iid, IMxPluginLibDetailsIID::Guid()) )
			rc = true;
		else
		{
			if( MxGUID::IsEqual(iid, IMxPluginBaseIID::Guid()) )
				rc = true;
		}
	}
	return rc;
}


const GUID	MXSTDMETHOD PluginStdDetailsLib::GetIID(int index) 
{ 
	GUID rc = GUID_Invalid;
	switch (index)
	{
		case 0:
			 rc = IPluginStdLibIID::Guid();
		break;
		case 1:
			rc = IMxPluginLibDetailsIID::Guid();
		break;
		case 2:
			rc = IMxPluginBaseIID::Guid();
		break;
		default:
		break;
	}
	return rc; 
}

const char *MXSTDMETHOD  PluginStdDetailsLib::GetISummary( const GUID  iid)
{
	const char *rc = "";

	if( MxGUID::IsEqual(iid, IPluginStdLibIID::Guid()) )
		rc = "IPluginStdLib - sample implementation of IMxPluginLibDetails";
	else
	{
		if( MxGUID::IsEqual(iid, IMxPluginLibDetailsIID::Guid()) )
			rc = "IMxPluginLibDetails - provides details about MxPlugin and its interfaces";
		else
		{
			if( MxGUID::IsEqual(iid, IMxPluginBaseIID::Guid()) )
				rc = "IMxPluginBase - base interface for Plugins (similar to COM IUnknown)";
		}
	}
	return rc;
}

const char *MXSTDMETHOD  PluginStdDetailsLib::GetIDetails(const GUID  iid)
{
	const char *rc = "";

	if( MxGUID::IsEqual(iid, IPluginStdLibIID::Guid()) )
		rc = "see IPluginStdLib.h for details about methods in this interface";
	else
	{
		if( MxGUID::IsEqual(iid, IMxPluginLibDetailsIID::Guid()) )
			rc = "see IMxPluginLibDetails.h for details about methods in this interface";
		else
		{
			if( MxGUID::IsEqual(iid, IMxPluginBaseIID::Guid()) )
				rc = "see IMxPluginBase.h for details about methods in this interface";
		}
	}
	return rc;
}
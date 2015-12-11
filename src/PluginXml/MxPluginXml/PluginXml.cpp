// MxPluginXml.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "MxUtils\MxUtils1.h"
#include "PluginXml\PluginXmlErrorCodes.h"

#include "PluginXml.h"
#include "PluginXmlDoc.h"
#include "PluginXmlAttribute.h"


void  *MXSTDMETHOD CreateClassInstance(const GUID IId)
{
	void *rc = nullptr;

	if ( MxGUID::IsEqual(IId, IPluginXmlLibIID::Guid()))
		rc = new PluginXml();
	else if ( MxGUID::IsEqual(IId, IPluginXmlDocIID::Guid()))
		rc = new PluginXmlDoc();
	else if ( MxGUID::IsEqual(IId, IPluginXmlElementIID::Guid()))
		rc = new PluginXmlElement();
	else if ( MxGUID::IsEqual(IId, IPluginXmlAttributeIID::Guid()))
		rc = new PluginXmlAttribute();
	else if ( MxGUID::IsEqual(IId, IMxPluginLibDetailsIID::Guid()))
		rc = new PluginXml();
	else if ( MxGUID::IsEqual(IId, IMxPluginBaseIID::Guid()))
		rc = new PluginXml();
	else
		rc = nullptr;

	if (rc == nullptr)
	{
		std::string tmp;
		MX_SETERROR(MX6039, MxError::CodeDefect, MxError::Abort, MxError::VerboseReport, "ERROR: CreateClassInstance not available for IId [%s] ", MxGUID::ConvertToString(IId, &tmp));
	}
	else
		((IMxPluginBase *)rc)->DuplicatePtr();

	return rc;
}


PluginXml::PluginXml() :
	_refCnt(0),
	_SetErrorOptionsDone(false)
{
	SetErrorOptions(MxError::LogChannel::Development, MxError::Level::VerboseReport);
	return;
}

PluginXml::PluginXml(MxError::LogChannel ReportChannel, MxError::Level ReportLevel) :
_refCnt(0),
_SetErrorOptionsDone(false)
{
	SetErrorOptions(ReportChannel, ReportLevel);
	return;
}

PluginXml::~PluginXml()
{
	MX_END(MxError::Level::VerboseReport, "MxPluginXml dtor");
	return;
}

long MXSTDMETHOD PluginXml::DestroyPtr(void)
{
	long cnt = InterlockedDecrement(&_refCnt);
	if ( cnt == 0 )
		delete this;
	return cnt;
}

long MXSTDMETHOD PluginXml::DuplicatePtr(void)
{
	return InterlockedIncrement(&_refCnt);
}

void *MXSTDMETHOD PluginXml::Dynamic_cast(const GUID IId)
{
	void *rc = nullptr;

	if ( MxGUID::IsEqual(IId, IPluginXmlLibIID::Guid()))
		rc = static_cast<IPluginXmlLib *>(this);
	else if ( MxGUID::IsEqual(IId, IMxPluginLibDetailsIID::Guid()))
		rc = static_cast<IMxPluginLibDetails *>(this);
	else if ( MxGUID::IsEqual(IId, IMxPluginBaseIID::Guid()))
		rc = static_cast<IMxPluginBase *>(this);
	else
		rc = nullptr;

	if (rc == nullptr)
	{	
		std::string tmp;
		MX_SETERROR(MX6037, MxError::Install, MxError::Abort, MxError::VerboseReport, "ERROR: Dynamic_cast does not support IID=%s", MxGUID::ConvertToString(IId, &tmp));
	}
	else
		((IMxPluginBase *)rc)->DuplicatePtr();

	return rc;
}

bool MXSTDMETHOD PluginXml::IsInterfaceSupported(const GUID IId)
{
	bool rc =false;

	if ( MxGUID::IsEqual(IId, IPluginXmlLibIID::Guid()))
		rc = true;
	else if ( MxGUID::IsEqual(IId,  IPluginXmlDocIID::Guid()))
		rc = true;
	else if ( MxGUID::IsEqual(IId, IPluginXmlElementIID::Guid()))
		rc = true;
	else if ( MxGUID::IsEqual(IId, IPluginXmlAttributeIID::Guid()))
		rc = true;
	else if ( MxGUID::IsEqual(IId, IMxPluginLibDetailsIID::Guid()))
		rc = true;
	else if ( MxGUID::IsEqual(IId, IMxPluginBaseIID::Guid()))
		rc = true;
	else
		rc = false;

	return rc;
}

const GUID	MXSTDMETHOD PluginXml::GetIID(int index)
{
	GUID rc = GUID_Invalid;

	switch(index)
	{
		case 0:
			rc = IPluginXmlLibIID::Guid();
		break;
		case 1:
			rc = IPluginXmlDocIID::Guid();
		break;
		case 2:
			rc = IPluginXmlElementIID::Guid();
		break;
		case 3:
			rc = IPluginXmlAttributeIID::Guid();
		break;
		case 4:
			rc = IMxPluginLibDetailsIID::Guid();
		break;
		case 5:
			rc = IMxPluginBaseIID::Guid();
		break;

		default:
		break;
	}
	return rc;
}

const char *MXSTDMETHOD  PluginXml::GetISummary( const GUID  iid)
{
	const char *rc = "";

	if( MxGUID::IsEqual(iid, IPluginXmlLibIID::Guid()) )
		rc = "IPluginXmlLib - Xml library implementing IMxPluginLibDetails";
	else
	{
		if( MxGUID::IsEqual(iid, IPluginXmlDocIID::Guid()) )
			rc = "IPluginXmlDoc - Xml library document support - open, close, save file etc";
		else
		{
			if( MxGUID::IsEqual(iid, IPluginXmlElementIID::Guid()) )
				rc = "IPluginXmlElement - Xml library xml element support - get value, set text, get attribute count etc";
			else
			{
				if( MxGUID::IsEqual(iid, IPluginXmlAttributeIID::Guid()) )
					rc = "IPluginXmlAttribute - Xml library xml attribute support - get name, set value etc";
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
			}
		}
	}
	return rc;
}

const char *MXSTDMETHOD  PluginXml::GetIDetails(const GUID  iid)
{
	const char *rc = "";

	if( MxGUID::IsEqual(iid, IPluginXmlLibIID::Guid()) )
		rc = "see IPluginXmlLib.h for details about methods in this interface";
	else
	{
		if( MxGUID::IsEqual(iid, IPluginXmlDocIID::Guid()) )
			rc = "see IPluginXmlDoc.h for details about methods in this interface";
		else
		{
			if( MxGUID::IsEqual(iid, IPluginXmlElementIID::Guid()) )
				rc = "IPluginXmlElement.h for details about methods in this interface";
			else
			{
				if( MxGUID::IsEqual(iid, IPluginXmlAttributeIID::Guid()) )
					rc = "IPluginXmlAttribute.h for details about methods in this interface";
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
			}
		}
	}
	return rc;
}
				//IPluginXmlLib

void	PluginXml::SetErrorOptions(MxError::LogChannel ReportChannel, MxError::Level ReportLevel)
{
	if (_SetErrorOptionsDone == false)
	{
		MX_SETUP(LibInfo::Inst().GetOwner(), PRODID_MxPluginManager1, LibInfo::Inst().GetProductName(), LibInfo::Inst().GetVersion(), ReportChannel, ReportLevel, "app starts");
		_SetErrorOptionsDone = true;
	}
}


#include "StdAfx.h"

#include <iostream>
#include <sstream>

#include "MxPluginManager\MxPluginLib1.h"

const char * const MxPluginLib::InvalidStr = "[Invalid]";
const wchar_t * const MxPluginLib::InvalidwStr = L"[Invalid]";
const char * const MxPluginLib::endl = "\r\n";						//when displayed in Edit Controls you need \r\n

MxPluginLib::MxPluginLib(void):
	_pLibDetails(nullptr),
	_hInstance(NULL),
	_pfn(nullptr),
	_wPathFilename(MxPluginLib::InvalidwStr),
	_PathFilename(MxPluginLib::InvalidStr),
	_DetailsStr(MxPluginLib::InvalidStr),
	_SummaryStr(MxPluginLib::InvalidStr),
	_Select(false)
{
}

MxPluginLib::MxPluginLib(const wchar_t *pathfilename, HINSTANCE hInstance, void * (MXSTDMETHOD *pfn)(const GUID)) :
	_pLibDetails(nullptr),
	_hInstance(hInstance),
	_pfn(pfn),
	_wPathFilename((pathfilename != nullptr) ? pathfilename : MxPluginLib::InvalidwStr),
	_PathFilename(MxPluginLib::InvalidStr),
	_DetailsStr(MxPluginLib::InvalidStr),
	_SummaryStr(MxPluginLib::InvalidStr),
	_Select(false)
{
	try
	{
		USES_CONVERSION;    // declare locals used by the ATL macros

		_PathFilename = W2A(_wPathFilename.c_str());

		if ((_hInstance == NULL) || (_pfn == nullptr))
			MX_SETERROR( MX1306, MxError::InvalidParams,  MxError::Abort,  MxError::QuietReport, "error: (_hInstance == NULL) || (_pfn == nullptr)"); 
		else
		{
			_pLibDetails = static_cast<IMxPluginLibDetails * >(pfn(IMxPluginLibDetailsIID::Guid()));
			if (_pLibDetails == nullptr)
				MX_SETERROR( MX1307, MxError::Install,  MxError::Abort,  MxError::VerboseReport, "error: static_cast<IMxPluginLibDetails * >(pfn(IMxPluginLibDetailsIID::GUID()))"); 
			else
			{
				std::ostringstream out;

				out << _pLibDetails->GetLibName() << ", " << _pLibDetails->GetLibVersion() << ", " << _pLibDetails->GetLibDescription();

				_SummaryStr = out.str();

				out  << MxPluginLib::endl;
				out << "Developed by " <<  _pLibDetails->GetLibOwnerName() << ", " << _pLibDetails->GetLibCopyrightNotice() << ", supported at " <<  _pLibDetails->GetLibSupportSite() << MxPluginLib::endl;
				out << "License type:  " << _pLibDetails->GetLibLicenseName() << ", details available at " << _pLibDetails->GetLibLicenseURL() << MxPluginLib::endl << MxPluginLib::endl;
				std::string tmp;
				out << "LibID=" << _pLibDetails->GetLibID() << " SID=" << MxGUID::ConvertToString(_pLibDetails->GetSocketID(), &tmp) << MxPluginLib::endl;
				const char * pathpluginname = W2CA(_pLibDetails->GetLibPathFilename());
				out  << "Pathfilename: " << (( pathpluginname != nullptr) ? pathpluginname : "[no pathfile for plugin]") << MxPluginLib::endl << MxPluginLib::endl;
				int nCnt = _pLibDetails->GetInterfaceCount();
				if ( nCnt <= 0 )
					out << "No interfaces found" << MxPluginLib::endl;
				else
				{
					out << "Implements interfaces:" << MxPluginLib::endl;
					for (int x = 0; x < nCnt; x++ )
					{
						GUID iid = _pLibDetails->GetIID(x);
						if (_pLibDetails->IsInterfaceSupported(iid) == false)
							out << "ERROR: interface not supported - index " << x << ": " << MxGUID::ConvertToString(iid, &tmp) << MxPluginLib::endl;
						else
							out << MxGUID::ConvertToString(iid, &tmp) << " " << _pLibDetails->GetISummary(iid)  << MxPluginLib::endl;
					}
				}
				_DetailsStr = out.str();

				 MX_LOGMSG(MxError::VerboseReport, "loaded MxPlugin: %s v%s", _pLibDetails->GetLibName(), _pLibDetails->GetLibVersion());
			}
		}
	}
	catch(...)
	{

	}
}


MxPluginLib::~MxPluginLib(void)
{
	if (_pLibDetails != nullptr)
		_pLibDetails->DestroyPtr();
	_pLibDetails = nullptr;

	if (_hInstance != NULL )
		::FreeLibrary(_hInstance);
	_hInstance = NULL;
}

void *MxPluginLib::CreateClassInstance(const GUID IID)
{
	void *rc = nullptr;
	if ( IsValid() == false)
		MX_SETERROR( MX1308, MxError::InvalidParams,  MxError::Abort,  MxError::QuietReport, "IsValid() == false"); 
	else
	{
		std::string tmp("");
		rc = (_pfn(IID));
		if (rc == nullptr)
			MX_SETERROR( MX1309, MxError::Install,  MxError::Abort,  MxError::QuietReport, "_pfn(IID) returns nullptr, IID=%s", MxGUID::ConvertToString(IID, &tmp)); 
		else
			MX_LOGMSG(MxError::VerboseReport, "Created obj that implements interface ID=[%s]",MxGUID::ConvertToString(IID, &tmp));
	}
	return rc;
}

IMxPluginLibDetails *MxPluginLib::GetIMxPluginLibDetails() 
{ 
	IMxPluginLibDetails *rc = _pLibDetails;

	if ( rc != nullptr )
	{
		rc->DuplicatePtr();
	}
	return  rc;
}
#include "StdAfx.h"

#include <iostream>
#include <sstream>
#include <atlconv.h>

#include "DefSocketImageProcXGUI.h"
#include "MxPluginLib.h"
#include "IPluginImageProcXLib.h"

const char * const MxPluginLib::InvalidStr = "[Invalid]";
const wchar_t * const MxPluginLib::InvalidwStr = L"[Invalid]";
const char * const MxPluginLib::endl = "\r\n";						//when displayed in Edit Controls you need \r\n

MxPluginLib::MxPluginLib(void):
	_pLibDetails('\0'),
	_hInstance(NULL),
	_pfn('\0'),
	_wPathFilename(MxPluginLib::InvalidwStr),
	_DetailsStr(MxPluginLib::InvalidStr),
	_SummaryStr(MxPluginLib::InvalidStr),
	_Select(false)
{
}

MxPluginLib::MxPluginLib(const wchar_t *pathfilename, HINSTANCE hInstance, void * (MXSTDMETHOD *pfn)(const char *)) :
	_pLibDetails('\0'),
	_hInstance(hInstance),
	_pfn(pfn),
	_wPathFilename((pathfilename != '\0') ? pathfilename : MxPluginLib::InvalidwStr),
	_PathFilename(MxPluginLib::InvalidStr),
	_DetailsStr(MxPluginLib::InvalidStr),
	_SummaryStr(MxPluginLib::InvalidStr),
	_Select(false)
{
	USES_CONVERSION;    // declare locals used by the ATL macros
	_PathFilename = CW2A(_wPathFilename.c_str());

	if ((_hInstance != NULL) && (_pfn != '\0'))
	{
		_pLibDetails = static_cast<IMxPluginLibDetails * >(pfn(IMxPluginLibDetailsIID::GUID()));
		if (_pLibDetails != '\0')
		{
			std::ostringstream out;

			out << _pLibDetails->GetLibName() << ", " << _pLibDetails->GetLibVersion() << ", " << _pLibDetails->GetLibDescription();

			_SummaryStr = out.str();

			out  << MxPluginLib::endl;
			out << "Developed by " <<  _pLibDetails->GetLibOwnerName() << ", " << _pLibDetails->GetLibCopyrightNotice() << ", supported at " <<  _pLibDetails->GetLibSupportSite() << MxPluginLib::endl;
			out << "License type:  " << _pLibDetails->GetLibLicenseName() << ", details available at " << _pLibDetails->GetLibLicenseURL() << MxPluginLib::endl << MxPluginLib::endl;
			out << "Implements interface: " << _pLibDetails->GetInterfaceName() << " [" << _pLibDetails->GetInterfaceGUID() << "]" << MxPluginLib::endl;
			out << "LibID=" << _pLibDetails->GetLibID() << std::endl << MxPluginLib::endl;
			out  << "Pathfilename: " << W2CA(_pLibDetails->GetLibPathFilename()) << MxPluginLib::endl;

			_DetailsStr = out.str();
		}

		IPluginImageProcXLib *pLibExtraDetails = static_cast<IPluginImageProcXLib * >(_pLibDetails->Dynamic_cast(IPluginImageProcXLibIID::GUID()));
		if (pLibExtraDetails != '\0')
		{
			_DetailsStr += "\r\n";
			_DetailsStr += "API details:\r\n";
			_DetailsStr += pLibExtraDetails->GetLibApiDetails();
			pLibExtraDetails->DestroyPtr();
		}
	}
}


MxPluginLib::~MxPluginLib(void)
{
	if (_pLibDetails != '\0')
		_pLibDetails->DestroyPtr();
	_pLibDetails = '\0';

	if (_hInstance != NULL )
		::FreeLibrary(_hInstance);
	_hInstance = NULL;
}

void *MxPluginLib::CreateClassInstance(const char *IID)
{
	void *rc = '\0';
	if ( (IID != '\0')  && IsValid() )
	{
		rc = (_pfn(IID));
	}
	return rc;
}

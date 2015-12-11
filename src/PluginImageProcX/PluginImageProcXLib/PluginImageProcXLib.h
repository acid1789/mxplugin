#pragma once

#ifdef PLUGINIMAGEPROCXLIB_EXPORTS
#define PLUGINIMAGEPROCXLIB_API __declspec(dllexport)
#else
#define PLUGINIMAGEPROCXLIB_API __declspec(dllimport)
#endif

#include "DefSocketImageProcXGUI.h"
#include "IPluginImageProcXLib.h"
#include "gLibDetails.h"

extern "C" void  *MXSTDMETHOD CreateClassInstance(const char *IId);

#pragma warning(disable:4275)
class PLUGINIMAGEPROCXLIB_API PluginImageProcXLib : IPluginImageProcXLib
{
#pragma warning(default:4275)

public:
	PluginImageProcXLib(void);
	~PluginImageProcXLib(void);

	virtual long   MXSTDMETHOD DestroyPtr(void);
	virtual long   MXSTDMETHOD DuplicatePtr(void);
	virtual void * MXSTDMETHOD Dynamic_cast(const char *IId);
	virtual const char *MXSTDMETHOD GetInterfaceGUID(void)  { return IPluginImageProcXLibIID::GUID();	}
	virtual const char *MXSTDMETHOD GetInterfaceName(void) { return IPluginImageProcXLibIID::Name(); }

	virtual const char *MXSTDMETHOD GetLibName(void) { return gLibName();	}							//VS_VERSION_INFO::ProductName
	virtual const char *MXSTDMETHOD GetLibDescription(void) { return gLibDescription(); }				//VS_VERSION_INFO::FileDescription
	virtual const char *MXSTDMETHOD GetLibVersion(void) { return ::gLibVersion(); }						//VS_VERSION_INFO::ProductVersion
	virtual const char *MXSTDMETHOD GetLibOwnerName(void) { return ::gLibOwnerName(); }					//VS_VERSION_INFO::CompanyName
	virtual const char *MXSTDMETHOD GetLibCopyrightNotice(void) { return gLibCopyrightNotice(); }		//VS_VERSION_INFO::LegalCopyright
	virtual const char *MXSTDMETHOD GetLibID(void) { return "0A782720-A4D8-4257-9452-BE5EDBD6309F"; }	//VS_VERSION_INFO::InternalName GUID - same as its folder name in PluginRootFolder
	virtual const char *MXSTDMETHOD GetLibSupportSite(void) { return "http://www.mxplugin.org"; }			
	virtual const char *MXSTDMETHOD GetLibLicenseName(void) { return "New BSD Licence";	}			
	virtual const char *MXSTDMETHOD GetLibLicenseURL(void) { return "http://mxplugin.codeplex.com/license";	}
	virtual const wchar_t *MXSTDMETHOD GetLibPathFilename(void) { return ::gLibPathFilename(); }		//filename returned by ::GetModuleFileName() of hModule in DllMain, DLL_PROCESS_ATTACH

	virtual const char *MXSTDMETHOD GetSocketAppID(void) { return DefSocketImageProcXGUI::SID(); }		//GUID provided by SocketImageProcXGUI, ImageCurator, etc - says this Plugin supports this application

	virtual const char *MXSTDMETHOD GetLibApiDetails(void) { return "no details provided...";	}

private:
	long _refCnt;
};


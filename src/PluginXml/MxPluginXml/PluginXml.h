#pragma once


#include "MxProductID.h"
#include "MxUtils\MxError1.h"

#include "PluginXml\IPluginXmlLib.h"
#include "PluginXml\PluginXmlErrorCodes.h"

#include "MxUtils\LibInfo.h"

extern "C" void  *MXSTDMETHOD CreateClassInstance(const GUID IId);

#ifdef MXPLUGINXML_EXPORTS
#define MXPLUGINXML_API __declspec(dllexport)
#else
#define MXPLUGINXML_API __declspec(dllimport)
#endif

class MXPLUGINXML_API PluginXml : IPluginXmlLib
{
public:
	PluginXml(MxError::LogChannel ReportChannel, MxError::Level ReportLevel);
	PluginXml(void);
	virtual ~PluginXml(void);

				//IMxPluginBase
	virtual long   MXSTDMETHOD DestroyPtr(void);
	virtual long   MXSTDMETHOD DuplicatePtr(void);
	virtual void * MXSTDMETHOD Dynamic_cast(const GUID IId);
	virtual const GUID   MXSTDMETHOD GetInterfaceGUID(void)  { return IPluginXmlLibIID::Guid(); }					//Library Interface GUID implemented by PluginICPsample.dll
	virtual const char *MXSTDMETHOD GetInterfaceName(void) { return IPluginXmlLibIID::Name(); }				//Library Interface Name implemented by PluginICPsample.dll
				//IMxPluginLibDetails
	virtual const char *MXSTDMETHOD GetLibName(void) { return LibInfo::Inst().GetProductName(); }				//VS_VERSION_INFO::ProductName
	virtual const char *MXSTDMETHOD GetLibDescription(void) { return LibInfo::Inst().GetDescription(); }			//VS_VERSION_INFO::FileDescription
	virtual const char *MXSTDMETHOD GetLibVersion(void) { return LibInfo::Inst().GetVersion(); }					//VS_VERSION_INFO::ProductVersion
	virtual const char *MXSTDMETHOD GetLibOwnerName(void) { return LibInfo::Inst().GetOwner(); }				//VS_VERSION_INFO::CompanyName
	virtual const char *MXSTDMETHOD GetLibCopyrightNotice(void) { return LibInfo::Inst().GetCopyrightNotice(); }	//VS_VERSION_INFO::LegalCopyright
	virtual const char *MXSTDMETHOD GetLibID(void) { return LibInfo::Inst().GetProductID(); }					//VS_VERSION_INFO::InternalName GUID - same as its folder name in PluginRootFolder
	virtual const char *MXSTDMETHOD GetLibSupportSite(void) { return "http://www.mxplugin.org/plugins/support/MxPluginTeam/PluginXml";	}		
	virtual const char *MXSTDMETHOD GetLibLicenseName(void) { return "New BSD Licence"; }								
	virtual const char *MXSTDMETHOD GetLibLicenseURL(void)  { return "http://www.mxplugin.org/plugins/license/MxPluginTeam/PluginXml"; }	
	virtual const wchar_t *MXSTDMETHOD GetLibPathFilename(void) { return LibInfo::Inst().GetwPathFilename(); }	//filename returned by ::GetModuleFileName() of hModule in DllMain, DLL_PROCESS_ATTACH	
	virtual void		MXSTDMETHOD SetErrorOptions(MxError::Level ReportLevel, MxError::LogChannel ReportChannel) { MxError::Inst().Initialise(LibInfo::Inst().GetOwner(), PRODID_PluginXml, LibInfo::Inst().GetProductName(), LibInfo::Inst().GetVersion(), ReportChannel, ReportLevel); }					//ReportLevel is MxError::Level, ReportChannel is MxError::LogChannel
	virtual int			MXSTDMETHOD GetLastErrorNo() { return MxError::Inst().GetErrorCode(); }
	virtual const char *MXSTDMETHOD GetLastErrorStr(){ return MxError::Inst().GetLastErrorStr(); }
	virtual void		MXSTDMETHOD ErrorReset() { MxError::Inst().Reset(); }
	virtual bool		MXSTDMETHOD IsErrorSet() { return MxError::Inst().IsErrorSet(); }

	virtual const GUID	MXSTDMETHOD GetSocketID(void) { return SID_Universal; }									//GUID provided by ProductID.h - says this Plugin supports this application

	virtual bool		MXSTDMETHOD IsInterfaceSupported(const GUID iid); 
	virtual int			MXSTDMETHOD GetInterfaceCount() { return 6; }
	virtual const GUID	MXSTDMETHOD GetIID(int index);
	virtual const char *MXSTDMETHOD GetISummary( const GUID  iid);
	virtual const char *MXSTDMETHOD GetIDetails(const GUID  iid);
				//IPluginXmlLib
	virtual const char *MXSTDMETHOD GetComment(void) { return "hello from PluginXml"; }	

private:
	void		SetErrorOptions(MxError::LogChannel ReportChannel, MxError::Level ReportLevel); //make App call close to where it calls MX_SETUP so as to call MX_SETUP for MxPluginManager

private:
	long _refCnt;
	bool _SetErrorOptionsDone;

};



#pragma once

#include "MxUtils\MxVersionInfo1.h"

//-----------------------------------------------------------------------------------------------------------------------------------------------
//Note: You need to add LibInfo.cpp to the project containing the DLL for which you want version information - see below for implementation
//      You should also call LibInfo::Inst().CleanupOnAppExitOnly() from the dtor of any class that uses it. For example ImageExpter - see below
//-----------------------------------------------------------------------------------------------------------------------------------------------


class  LibInfo
{
public:

	bool		SetVersionInfo( HMODULE hModule = NULL) { return _VersionInfo->SetVersionInfo(hModule); }

	const wchar_t *GetwPathFilename() { return _VersionInfo->GetwPathFilename(); }		//filename returned by ::GetModuleFileName() of hModule in DllMain, DLL_PROCESS_ATTACH
	const char *GetProductName() {  return _VersionInfo->GetProductName(); }			//VS_VERSION_INFO::ProductName
	const char *GetDescription() {  return _VersionInfo->GetDescription();  }			//VS_VERSION_INFO::FileDescription
	const char *GetVersion() {  return _VersionInfo->GetVersion();  }					//VS_VERSION_INFO::ProductVersion
	const char *GetOwner() {  return _VersionInfo->GetOwner(); }						//VS_VERSION_INFO::CompanyName	
	const char *GetCopyrightNotice() {  return _VersionInfo->GetCopyrightNotice(); }	//VS_VERSION_INFO::LegalCopyright
	const char *GetProductID() {    return _VersionInfo->GetProductID(); }				//VS_VERSION_INFO::InternalName GUID - for a MxPlugin this is same as its folder name in PluginRootFolder	

	void CleanupOnAppExitOnly();			//deletes all allocated memory (inst cannot be used after this call) - stops VS2012 reporting memory leaks before ~LibInfo() is called; only call this if there's a problem		

	static LibInfo& Inst()
	{
		static LibInfo    instance;			// Guaranteed to be destroyed. Instantiated on first use.
		return instance;
	}

private:
	LibInfo();
	~LibInfo();
    LibInfo(LibInfo const&);					// Don't Implement
    void operator=(LibInfo const&);			// Don't implement

	MxVersionInfo* _VersionInfo;
};

// dllmain.cpp : Defines the entry point for the DLL application.
//#include "stdafx.h"
//
//#include "MxUtils\LibInfo.h"
//
//BOOL APIENTRY DllMain(HMODULE hModule,
//	DWORD  ul_reason_for_call,
//	LPVOID lpReserved
//	)
//{
//	switch (ul_reason_for_call)
//	{
//	case DLL_PROCESS_ATTACH:
//	{
//		LibInfo::Inst().SetVersionInfo(hModule);
//	}
//		break;
//	case DLL_THREAD_ATTACH:
//	case DLL_THREAD_DETACH:
//	case DLL_PROCESS_DETACH:
//		break;
//	}
//	return TRUE;
//}

//..........................................................................................
//LibInfo.cpp - replace [Your Component] with your Library name
//..........................................................................................

//#include "stdafx.h"
//#include "MxUtils\LibInfo.h"
//
//
//LibInfo::LibInfo(void) :
//_VersionInfo(nullptr)
//{
//	_VersionInfo = new MxVersionInfo();
//}
//
//
//LibInfo::~LibInfo(void)
//{
//	CleanupOnAppExitOnly();
//#ifdef _DEBUG
//	::OutputDebugString(L"****************LibInfo::~LibInfo() [Your component] all allocated memory now freed ********************\n");
//#endif
//}
//
//void LibInfo::CleanupOnAppExitOnly()
//{
//	if (_VersionInfo != nullptr)
//		delete _VersionInfo;
//	_VersionInfo = nullptr;
//}

//..........................................................................................
//ImageExpter.h - replace ImageExpter with the name of your library
//..........................................................................................
//#ifdef IMAGEEXPTER_EXPORTS
//#define IMAGEEXPTER_API __declspec(dllexport)
//#else
//#define IMAGEEXPTER_API __declspec(dllimport)
//#endif
//
//#include "MxPluginManager\LibInfo.h"
//
//class IMAGEEXPTER_API ImageExpter
//{
//public:
//	ImageExpter(void);
//	ImageExpter::~ImageExpter();
//
//	const char *GetLibName(void) { return LibInfo::Inst().GetProductName(); }				//VS_VERSION_INFO::ProductName
//	const char *GetLibDescription(void) { return LibInfo::Inst().GetDescription(); }		//VS_VERSION_INFO::FileDescription
//	const char *GetLibVersion(void) { return LibInfo::Inst().GetVersion(); }				//VS_VERSION_INFO::ProductVersion
//	const char *GetLibOwnerName(void) { return LibInfo::Inst().GetOwner(); }				//VS_VERSION_INFO::CompanyName
//	const char *GetLibCopyrightNotice(void) { return LibInfo::Inst().GetCopyrightNotice(); }//VS_VERSION_INFO::LegalCopyright
//	const char *GetLibInternalName() { return LibInfo::Inst().GetProductID(); }				//VS_VERSION_INFO::Internal Name
//
//};

//..........................................................................................
//ImageExpter.cpp - replace ImageExpter with the name of your library
//..........................................................................................
//#include "stdafx.h"
//#include "ImageExpter.h"
//
//ImageExpter::ImageExpter()
//{
//	return;
//}
//
//ImageExpter::~ImageExpter()
//{
//	LibInfo::Inst().CleanupOnAppExitOnly();
//}
//
#pragma once

#include <windows.h>
#include <string>


#ifdef MXUTILS_EXPORTS
#define MXUTILS_API __declspec(dllexport)
#else
#define MXUTILS_API __declspec(dllimport)
#endif

class  MXUTILS_API MxVersionInfo
{
public:

	MxVersionInfo();
	~MxVersionInfo();

	bool		SetVersionInfo( HMODULE hModule = NULL);

	const wchar_t *GetwPathFilename() { return _wPathFilename.c_str(); }		//filename returned by ::GetModuleFileName() of hModule in DllMain, DLL_PROCESS_ATTACH

	const char *GetProductName() {  return _ProductName.c_str(); }				//VS_VERSION_INFO::ProductName
	const char *GetDescription() {  return _Description.c_str();  }				//VS_VERSION_INFO::FileDescription
	const char *GetVersion() {  return _Version.c_str();  }						//VS_VERSION_INFO::ProductVersion
	const char *GetOwner() {  return _OwnerName.c_str(); }						//VS_VERSION_INFO::CompanyName	
	const char *GetCopyrightNotice() {  return _CopyrightNotice.c_str(); }		//VS_VERSION_INFO::LegalCopyright
	const char *GetProductID() {    return _ProductID.c_str(); }				//VS_VERSION_INFO::InternalName GUID - for a MxPlugin this is same as its folder name in PluginRootFolder	


private:
	const char *GetVersionInfoString(LPTSTR pDataBlock, WORD* langInfo, LPTSTR Label, std::string& result);

#pragma warning(disable:4251)
	std::string		_ProductName;
	std::string		_Description;
	std::string		_Version;
	std::string		_OwnerName;
	std::string		_CopyrightNotice;
	std::string		_ProductID;
	std::wstring	_wPathFilename;
#pragma warning(default:4251)
};


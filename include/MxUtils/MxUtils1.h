#pragma once

#ifdef MXUTILS_EXPORTS
#define MXUTILS_API __declspec(dllexport)
#else
#define MXUTILS_API __declspec(dllimport)
#endif

#include "MxUtils\MxError1.h"
#include "MxUtils\MxUtilsErrorCodes1.h"
#include "MxUtils\MxVersionInfo1.h"


class MXUTILS_API MxUtils 
{
public:
	static const int	MxUtils::Invalid = -1;
	static const int	MxUtils::GuidStrLen = 38;
	static const int	MxUtils::RegStringMaxLen = 2048;
	static const int	MxUtils::DefaultStringMaxLen = 65535;
	static const int	MxUtils::FileBuffSize = 4096;
	static const int	MxUtils::DefaultRunConsoleAppStartupWait = 1000;				//wait 1000 mS for console app to start before attempting to read STDOUT
	static const int	MxUtils::MaxEncryptLength = 50;
	static const int	MxUtils::MinEncryptLength = 4;										
	static const int	MxUtils::MaxDecryptLength = MxUtils::MaxEncryptLength + 8;	

public:
								//VS_VERSION_INFO Utilities
	bool		SetVersionInfo( HMODULE hModule = NULL) { return _VersionInfo->SetVersionInfo(hModule); }

	const wchar_t *GetwPathFilename() { return _VersionInfo->GetwPathFilename(); }		//filename returned by ::GetModuleFileName() of hModule in DllMain, DLL_PROCESS_ATTACH
	const char *GetProductName() {  return _VersionInfo->GetProductName(); }			//VS_VERSION_INFO::ProductName
	const char *GetDescription() {  return  _VersionInfo->GetDescription();  }			//VS_VERSION_INFO::FileDescription
	const char *GetVersion() {  return  _VersionInfo->GetVersion();  }					//VS_VERSION_INFO::ProductVersion
	const char *GetOwner() {  return _VersionInfo->GetOwner(); }						//VS_VERSION_INFO::CompanyName	
	const char *GetCopyrightNotice() {  return _VersionInfo->GetCopyrightNotice(); }	//VS_VERSION_INFO::LegalCopyright
	const char *GetProductID() {    return _VersionInfo->GetProductID(); }				//VS_VERSION_INFO::InternalName GUID - for a MxPlugin this is same as its folder name in PluginRootFolder	
								//File Utilities
	static bool MxUtils::IsExistingFolderName(const char *foldername, size_t MaxFoldernameLen=MAX_PATH);
	static bool MxUtils::IsExistingPathFilename(const char *pathfilename, size_t MaxPathFilenameLen=MAX_PATH);
	static bool MxUtils::IsValidPathFilename(const char *pathfilename, size_t MaxPathFilenameLen=MAX_PATH);
	static bool	MxUtils::IsValidGUID(const char *guid) { return (( GUID_Invalid != MxGUID::ConvertFromString(guid)) ? true : false); }
								//Windows Registry Utilities
	static bool	MxUtils::CreateRegAppRootKey(HKEY hKey, const wchar_t *companyKeyName, const wchar_t *appKeyName, HKEY *hAppRootKey);	//caller has responsibility for RegCloseKey(hAppRootKey);
	static bool	MxUtils::CreateRegAppSubKey(HKEY hParentKey, const wchar_t *subKeyNameBackSlash, HKEY *hSubKey);						//caller has responsibility for RegCloseKey(hSubKey);
	static bool	MxUtils::IsPresentRegSubKey(HKEY hRootKey, const wchar_t *subKeyNameBackSlash);											//hRootKey is HKEY_CURRENT_USER, HKEY_LOCAL_MACHINE, ...
	static bool	MxUtils::SetRegSubKeyInt(HKEY SubKey, const wchar_t *name, int value);
	static bool	MxUtils::GetRegSubKeyInt(HKEY SubKey, const wchar_t *name, int *value);
	static bool	MxUtils::SetRegSubKeyStr(HKEY SubKey, const wchar_t *name, const wchar_t *value);
	static bool	MxUtils::GetRegSubKeyStr(HKEY SubKey, const wchar_t *name, std::wstring *value);
	static bool MxUtils::DelRegAppKey(HKEY hKey, const wchar_t *companyKeyName, const wchar_t *appName, const wchar_t *subKeyNameBackSlash=NULL);
								//RunConsoleApp Utilities - run app from Command Prompt and return StdOut for analysis
	static bool MxUtils::RunConsoleApp(const wchar_t *CmdLine, std::wstring *StdOutText=nullptr, int StdOutTextLenMax=MxUtils::DefaultStringMaxLen, int WaitForAppToStart=MxUtils::DefaultRunConsoleAppStartupWait);
	static int  MxUtils::GetLineCnt(const wchar_t *StdOutText);
	static bool MxUtils::GetLineText(const wchar_t *StdOutText, int LineNo, std::wstring *LineText);
	static bool MxUtils::GetText(const wchar_t *StdOutText, const wchar_t *LineStartsWithText, std::wstring *RestOfLineText);
								//Database Connection String Utilities
	static LPCTSTR MxUtils::SetUserIDPassword(std::wstring *ConnStr, LPCTSTR DbPassword, LPCTSTR DbUserID = nullptr );
	static bool	   MxUtils::IsValidDbConnection(LPCTSTR connStr);
	static bool	   MxUtils::GetElementValueFromConnStr(std::wstring *value, LPCTSTR connStr, LPCTSTR element);
								//String Conversion Utilities
	static const char *   MxUtils::ConvertWString2String(std::string *strDest, const wchar_t *strSrc);
	static const wchar_t *MxUtils::ConvertString2WString(std::wstring *strDest, const char *strSrc);
								//Encryption Utilities
	static int	   MxUtils::EncryptString(BYTE *CodeText, LPCWSTR val, const char *Key);
	static LPCWSTR MxUtils::DecryptString(std::wstring *val, BYTE *CodeText, int CodeTextLen, const char *Key);
	static bool	   MxUtils::WritePasswordToFile(const BYTE *codetext, int len, const char *PathFilename);
	static int	   MxUtils::ReadPasswordFromFile(BYTE *codetext, int maxlen, const char *PathFilename);
								//Environment Var Utilities
	static const wchar_t *MxUtils::GetEnvironmentVar(std::wstring *value, const wchar_t *name, bool reportNotFoundError = false);


	void CleanupOnAppExitOnly();			//deletes all allocated memory (inst cannot be used after this call) - stops VS2012 reporting memory leaks before ~MxUtils() is called; only call this if there's a problem

	static MxUtils& Inst()
	{
		static MxUtils    instance;			// Guaranteed to be destroyed. Instantiated on first use.
		return instance;
	}

private:
	MxUtils();
	~MxUtils();
    MxUtils(MxUtils const&);				// Don't Implement
    void operator=(MxUtils const&);			// Don't implement

	static bool	MxUtils::DelRegNodeRecurse (HKEY hKeyRoot, LPTSTR lpSubKey);
	static bool MxUtils::IsElementCorrect(LPCTSTR connStr, LPCTSTR element);

	MxVersionInfo *_VersionInfo;
};



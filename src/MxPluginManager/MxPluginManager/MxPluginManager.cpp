// MxPluginManager.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <strsafe.h>

#include "MxProductID.h"

#include "MxUtils\MxUtils1.h"
#include "MxUtils\MxError1.h"

#include "MxPluginManager\MxPluginManager1.h"
#include "MxPluginManager\MxPluginManagerErrorCodes1.h"

#include "MxPluginDep.h"

const char *    const MxPluginManager::FnNameCreateClassInstance = "CreateClassInstance";
const char *    const MxPluginManager::InvalidName = "[invalid]";
const char *    const MxPluginManager::PluginDependenciesFilename = "MxPlugin.dep";

const wchar_t * const MxPluginManager::SysEnvVarNameMxPlugin_Root = L"MXPLUGIN_ROOT";
const wchar_t * const MxPluginManager::DefaultPluginRootFolder = L"C:\\Program Files (x86)\\MxPluginTeam\\MxPlugin\\Bin\\";


const wchar_t * const MxPluginManager::RegNameAppCnt = L"AppCnt";
const wchar_t * const MxPluginManager::RegNamePluginRootFolder = L"RootFolder";

const wchar_t * const MxPluginManager::RegKeyApps = L"Apps\\";
const wchar_t * const MxPluginManager::RegKeyApp = L"App";	//append MxPluginManager::RegNameAppCnt value and '\\', then increment MxPluginManager::RegNameAppCnt

const wchar_t * const MxPluginManager::RegNameAppProdID = L"ProdID";
const wchar_t * const MxPluginManager::RegNameAppSID = L"SID";
const wchar_t * const MxPluginManager::RegNameAppName = L"Name";
const wchar_t * const MxPluginManager::RegNameAppLogLevel = L"LogLevel";
const wchar_t * const MxPluginManager::RegNameAppLogChannel = L"LogChannel";

MxPluginManager::MxPluginManager(void) :
	_itemName(""),
	_RegKeyMxPluginMngr(NULL),
	_ProdMngrName(MxPluginManager::InvalidName),
	_ProdMngrOwner(MxPluginManager::InvalidName),
	_MxPluginRootDir(MxPluginManager::DefaultPluginRootFolder),
	_ProdMngrProdID(GUID_Invalid),
	_ProdMngrSID(GUID_Invalid),
	_InitDone(false),
	_SetErrorOptionsDone(false)
{

	_ProdMngrName = LibInfo::Inst().GetProductName();
	_ProdMngrOwner = LibInfo::Inst().GetOwner();
	_ProdMngrProdID = MxGUID::ConvertFromString(LibInfo::Inst().GetProductID());
	_ProdMngrSID = SID_Universal;

	const wchar_t * rootDir = nullptr;
	if (( rootDir = MxUtils::GetEnvironmentVar(&_MxPluginRootDir, MxPluginManager::SysEnvVarNameMxPlugin_Root)) == nullptr)
		_MxPluginRootDir = MxPluginManager::DefaultPluginRootFolder;

	SetErrorOptions(MxError::LogChannel::Development, MxError::Level::VerboseReport);
}

MxPluginManager::MxPluginManager(MxError::LogChannel ReportChannel, MxError::Level ReportLevel, GUID ProductMngrSID ) :
	_itemName(""),
	_RegKeyMxPluginMngr(NULL),
	_ProdMngrName(MxPluginManager::InvalidName),
	_ProdMngrOwner(MxPluginManager::InvalidName),
	_MxPluginRootDir(MxPluginManager::DefaultPluginRootFolder),
	_ProdMngrProdID(GUID_Invalid),
	_ProdMngrSID(ProductMngrSID),
	_InitDone(false),
	_SetErrorOptionsDone(false)
{
	_ProdMngrName = LibInfo::Inst().GetProductName();
	_ProdMngrOwner = LibInfo::Inst().GetOwner();
	_ProdMngrProdID = MxGUID::ConvertFromString(LibInfo::Inst().GetProductID());
	_ProdMngrSID = ProductMngrSID; 

	const wchar_t * rootDir = nullptr;
	if ((rootDir = MxUtils::GetEnvironmentVar(&_MxPluginRootDir, MxPluginManager::SysEnvVarNameMxPlugin_Root, true)) == nullptr)
		_MxPluginRootDir = MxPluginManager::DefaultPluginRootFolder;
	SetErrorOptions(ReportChannel, ReportLevel);
}

MxPluginManager::~MxPluginManager(void)
{
	ClearAllMxPlugins();
	if (_RegKeyMxPluginMngr != NULL)
		RegCloseKey(_RegKeyMxPluginMngr);
	_RegKeyMxPluginMngr = NULL;

	MX_END(MxError::Level::VerboseReport, "MxPluginManager dtor");

}

bool MxPluginManager::RefreshPluginsInRootFolder(const GUID SID, bool BreakOnDllLoadError)
{
	bool rc = false;

	ClearAllSelections();

	HANDLE hFindFolder = INVALID_HANDLE_VALUE;

	std::string root = "";
	const char *PluginRootDir = MxUtils::ConvertWString2String(&root, _MxPluginRootDir.c_str());
	if ((PluginRootDir == nullptr) || (MxUtils::IsExistingFolderName(PluginRootDir) == false))
		MX_SETERROR(MX1302, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "PluginRootFolder is invalid : %s", (PluginRootDir == nullptr) ? "null" : PluginRootDir);
	else
	{
		try
		{
			std::wstring PluginFolder(L"");
			for (bool folderFound = GetFirstPluginFolderItem(&PluginFolder, _MxPluginRootDir.c_str(), &hFindFolder, true); folderFound == true; folderFound = GetNextPluginFolderItem(&PluginFolder, _MxPluginRootDir.c_str(), &hFindFolder, true))
			{
				if ((RefreshPlugins(PluginFolder.c_str(), SID, BreakOnDllLoadError) == false) && (BreakOnDllLoadError == true))
					break;
			}
		}
		catch(std::exception const& e)
		{
			MX_SETERROR( MX1337, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "Unexpected exception: %s", e.what() ); 
		}

		if ( hFindFolder != INVALID_HANDLE_VALUE )
			::FindClose(hFindFolder);
		hFindFolder = INVALID_HANDLE_VALUE;

		while (DeleteNextUnselected() == true);

		std::string tmp("");
		if (GetPluginCount() == 0)
			MX_SETERROR( MX1305, MxError::BadUserInput, MxError::Abort, MxError::QuietReport, "No plugins found for SID=%s", MxGUID::ConvertToString(SID, &tmp)); 
		else
			rc = true;

		ClearAllSelections();
	}
	return rc;
}


bool MxPluginManager::RefreshPlugins(const wchar_t *PluginFolder, const GUID ProductGUID, bool BreakOnDllLoadError)
{
	bool rc = false;

	USES_CONVERSION;
	std::wstring wDllFile(L"");
	const char * dllFileName = "";
	HANDLE hFindFile = INVALID_HANDLE_VALUE;
	MxPluginDep *deps = nullptr;

	try
	{
		if (((deps = new MxPluginDep(PluginFolder)) == nullptr) || (deps->IsValid() == false))
			MX_SETERROR(MX1395, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "MxPlugin.dep failed for [%s]", (PluginFolder != nullptr) ? W2CA(PluginFolder) : "(folder is null)"); //this DLL has dependencies that could not be found / loaded - specifically here PluginXMlTest
		else
		{
			for (bool FileFound = GetFirstPluginFolderItem(&wDllFile, PluginFolder, &hFindFile, false); FileFound == true; FileFound = GetNextPluginFolderItem(&wDllFile, PluginFolder, &hFindFile, false))
			{
				dllFileName = ConvertItemName(wDllFile.c_str());
				HINSTANCE hInst = NULL;
				if ((hInst = ::LoadLibraryEx(wDllFile.c_str(), NULL, MxPluginManager::LoadDllSearchOptions)) == NULL)
				{
					if (GetLastError() == ERROR_MOD_NOT_FOUND)
						MX_SETERROR(MX1388, MxError::Install, MxError::Abort, MxError::VerboseReport, "error: dependencies not found for [%s]", ((dllFileName != nullptr) ? dllFileName : "(folder item is null)")); //this DLL has dependencies that could not be found / loaded - specifically here PluginXMlTest
					else
						MX_SETERROR(MX1301, MxError::Install, MxError::Abort, MxError::VerboseReport, "::LoadLibraryEx() fails, error code=%d, dll=[%s]", (int)GetLastError(), ((dllFileName != nullptr) ? dllFileName : "(folder item is null)"));
					rc = false;
					if (BreakOnDllLoadError == true)
						break;
				}
				else
				{
					void * (MXSTDMETHOD *pfn)(GUID);
					*(FARPROC *)&pfn = ::GetProcAddress(hInst, MxPluginManager::FnNameCreateClassInstance);
					if (pfn == nullptr)					//Do not continue if this DLL doesn't provide MxPluginManager::FnNameCreateClassInstance - quite normal to find such DLLs in folders
					{
						::FreeLibrary(hInst);
						hInst = NULL;
					}
					else
					{
						IMxPluginLibDetails *tmp = static_cast<IMxPluginLibDetails *>(pfn(IMxPluginLibDetailsIID::Guid()));
						if ((tmp != nullptr) && (IsPluginForApp(tmp->GetSocketID(), ProductGUID)))		//Do not continue if this DLL doesn't  support IMxPluginLibDetails or the Application - quite normal to find such DLLs in folders
						{
							std::string libDesc(tmp->GetLibDescription());
							MxPluginLib *lib = GetPlugin(MxGUID::ConvertFromString(tmp->GetLibID()));
							if (lib == nullptr)
								tmp->DestroyPtr();		//Plugin is NOT already in list, so tmp is no longer needed (though hInst is used below); MUST invoke tmp->DestroyPtr() before any call to ::FreeLibrary(hInst);
							else
							{
								tmp->DestroyPtr();		//Plugin IS already in list, so tmp, lib and hInst are no longer needed;  MUST invoke tmp->DestroyPtr() before any call to ::FreeLibrary(hInst);
								if (lib != nullptr)
									lib->SetSelection(true);
								::FreeLibrary(hInst);
								hInst = NULL;
							}
							if (hInst != NULL)
							{                           //Plugin 's details (lib) needs to be added to the list
								if ((lib = new MxPluginLib(wDllFile.c_str(), hInst, pfn)) == nullptr)
								{
									MX_LOGMSG(MxError::QuietReport, "error: new MxPluginLib() failed for [%s]", libDesc.c_str());	//new MxPluginLib is responsible for MX_SETERROR
									::FreeLibrary(hInst);
									hInst = NULL;
									rc = false;
									break;
								}
								else
								{
									lib->SetSelection(true);

									if (AddMxPlugin(lib) == false)		//add Plugin's details (lib) to list
									{
										delete lib;
										lib = nullptr;
										::FreeLibrary(hInst);
										hInst = NULL;
										rc = false;
										break;
									}
									else
									{

									}
								}
							}
						}
					}
				}
				dllFileName = "";
			}
		}
	}
	catch(std::exception const& e)
	{
		MX_SETERROR( MX1316, MxError::System, MxError::Abort, MxError::QuietReport, "unexpected exception: GetLastError()=%d, %s [%s]", GetLastError(), e.what(), ((dllFileName != nullptr) ? dllFileName : "(folder item is null)") ); 
	}
	if ( hFindFile != INVALID_HANDLE_VALUE)
	{
		try
		{
			dllFileName = W2CA(wDllFile.c_str());
		}
		catch(...)
		{
			dllFileName = "[invalid]";
		}
		MX_LOGMSG(MxError::VerboseReport, "tidy-up: call ::FindClose(hFindFile) for [%s]", ((dllFileName != nullptr) ? dllFileName : "(folder item is null)") );	
		::FindClose(hFindFile);
		hFindFile = INVALID_HANDLE_VALUE;
	}
	if (deps != nullptr)
		delete deps;
	deps = nullptr;

	if ((rc == false) && (ERROR_NO_MORE_FILES == GetLastError()))
		rc = true;
	return rc;
}

bool	MxPluginManager::IsPluginForApp(const GUID PluginSocketID, const GUID AppSocketID)
{
	bool rc = false;

	if (MxGUID::IsValid(PluginSocketID) == false) 
		MX_SETERROR(MX1312, MxError::InvalidParams,  MxError::Abort,  MxError::QuietReport, "invalid params, (PluginSocketID is invalid) ");
	else
	{
		if ( (MxGUID::IsEqual(AppSocketID, GUID_Invalid)) || (MxGUID::IsEqual(PluginSocketID, AppSocketID)) )
			rc = true;
	}
	return rc;
}

MxPluginLib *MxPluginManager::GetPlugin(const GUID LibID)
{
	MxPluginLib *rc = nullptr;

	if (MxGUID::IsValid(LibID) == false)
		MX_SETERROR(MX1340, MxError::InvalidParams,  MxError::Abort,  MxError::QuietReport, "invalid params, LibID is invalid");
	else
	{

		size_t cnt = _Libs.size();
		if (cnt > 0 )
		{
			for (size_t x = 0; x < cnt; x++)
			{
				if (MxGUID::IsEqual(_Libs[x]->GetLibID(), LibID) == true)
				{
					rc = _Libs[x];
					if (rc == nullptr)
						MX_SETERROR(MX1315, MxError::CodeDefect,  MxError::Abort,  MxError::QuietReport, "_Libs[%d] is null", x);
					else
					{
						std::string tmp;
						MX_LOGMSG(MxError::VerboseReport, "success: LibID=%s, Plugin=%s", MxGUID::ConvertToString(LibID, &tmp), rc->GetLibDetails());
					}
					break;
				}
			}
		}
	}
	return rc;
}

bool MxPluginManager::AddMxPlugin(MxPluginLib *lib)
{
	bool rc = false;

	if ((lib == nullptr) || (lib->IsValid() == false))
		MX_SETERROR( MX1310	, MxError::InvalidParams, MxError::Abort, MxError::VerboseReport, "((lib == nullptr) || (lib->IsValid() == false)), lib=%s", ((lib == nullptr) ? "(null)" : "(valid)") ); 
	else
	{
		if (IsExisting(lib) == true)
			MX_LOGMSG(MxError::VerboseReport, "error: library already in list [%s]", lib->GetLibSummary() );
		else
		{
			_Libs.push_back(lib);
			rc = true;
		}
	}
	return rc;
}

bool MxPluginManager::IsExisting(MxPluginLib *lib)
{
	bool rc = false;
	if (lib == nullptr)
		MX_SETERROR( MX1317	, MxError::InvalidParams, MxError::Abort, MxError::VerboseReport, "lib == nullptr" ); 
	else
		rc = (GetPlugin(lib->GetLibID()) != nullptr) ? true : false;
	return rc;
}

void MxPluginManager::ClearAllSelections()
{
	size_t cnt = _Libs.size();
	if (cnt > 0 )
	{
		for (size_t x = 0; x < cnt; x++)
		{
			if (_Libs[x] == nullptr)
				MX_SETERROR( MX1318	, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "_Libs[%d] == nullptr", x );	
			else
				_Libs[x]->SetSelection(false);
		}
	}
	return;
}

bool MxPluginManager::DeleteNextUnselected()
{
	bool erased = false;

	size_t cnt = _Libs.size();
	if (cnt > 0 )
	{
		for (size_t x = 0; x < cnt; x++)
		{
			if (_Libs[x] == nullptr)
				MX_SETERROR( MX1319	, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "_Libs[%d] == nullptr", x );	
			else
			{
				if (_Libs[x]->GetSelection() == false)
				{
					delete  _Libs[x];
					_Libs[x] = nullptr;
					_Libs.erase(_Libs.begin()+x);
					erased = true;
					break;
				}
			}
		}
	}
	return erased;
}

bool MxPluginManager::IsValidLib(const wchar_t *filename)
{
	bool rc = false;

	if (filename == nullptr)
		MX_SETERROR( MX1320	, MxError::InvalidParams, MxError::Abort, MxError::VerboseReport, "filename == nullptr" ); 
	else
	{
		std::wstring name(filename);
		if (name.find(L"Test.dll") != std::string::npos)		//UnitTest DLLs
			rc = false;
		else
			rc = true;
	}
	return rc;
}


void MxPluginManager::ClearAllMxPlugins()
{
	size_t cnt = _Libs.size();
	if (cnt > 0 )
	{
		for (size_t x = 0; x < cnt; x++)
			delete  _Libs[x];
		_Libs.clear();
	}
	return;
}

bool MxPluginManager::GetFirstPluginFolderItem(std::wstring *firstFolderResult, const wchar_t *PluginFolder, HANDLE *hFindFolder, bool FindFolder)
{
	bool  rc = false;

	const char *matchStr = "(not set)";
	if ((firstFolderResult == nullptr) || (PluginFolder == nullptr) || (hFindFolder == NULL))
		MX_SETERROR( MX1321	, MxError::InvalidParams, MxError::Abort, MxError::VerboseReport, "(firstFolderResult == nullptr) || (PluginFolder == nullptr) || (hFindFolder == NULL)" ); 
	else
	{
		std::wstring path(PluginFolder);
		int endPathPos = path.find_last_of('\\');
		if (endPathPos != path.length()-1)
			path += L"\\";

		std::wstring match(path);
		if (FindFolder == true)
			match += L"*";	
		else
			match += L"*.dll";

		try
		{
			USES_CONVERSION;
			matchStr = W2CA(match.c_str());

			WIN32_FIND_DATA ffd;
			*hFindFolder = ::FindFirstFile(match.c_str(), &ffd);
			if (INVALID_HANDLE_VALUE == *hFindFolder)
				MX_SETERROR( MX1304, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "::FindFirstFile() returns INVALID_HANDLE_VALUE - GetLastError()=%d, matchStr=%s", GetLastError(), matchStr ); 
			else
			{
				do
				{
					if (FindFolder == true)
					{
						if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (wcsncmp(ffd.cFileName, L".", 1) != 0) && (wcsncmp(ffd.cFileName, L"..", 2) != 0))
						{                                             //quite normal to find items that are not folders (files), '.' and '..' in a folder
							*firstFolderResult = path;
							*firstFolderResult += ffd.cFileName;
							*firstFolderResult += L"\\";
							rc = true;
							break;
						}
					}
					else
					{
						if (((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == false) && (IsValidLib(ffd.cFileName)))
						{                                             //quite normal to find items that are not files (subfolders) in a folder
							*firstFolderResult = path;
							*firstFolderResult += ffd.cFileName;
							rc = true;
							break;
						}
					}
				}
				while (::FindNextFile(*hFindFolder, &ffd) != 0);
			}
		}
		catch(std::exception const& e)
		{
			MX_SETERROR(  MX1338, MxError::System, MxError::Abort, MxError::QuietReport, "unexpected exception: GetLastError()=%d, %s [%s]", GetLastError(), e.what(), ((matchStr != nullptr) ? matchStr : "(matchStr is null)") );
			rc = false;
		}
		if ( rc == false )
		{
			if (*hFindFolder != INVALID_HANDLE_VALUE)
			{
				::FindClose(*hFindFolder);
				*hFindFolder = INVALID_HANDLE_VALUE;
			}
			*firstFolderResult = L"";
		}
	}
	return rc;
}

bool MxPluginManager::GetNextPluginFolderItem(std::wstring *nextFolderResult, const wchar_t *PluginFolder, HANDLE *hFindFolder, bool FindFolder)
{
	bool  rc = false;

	const char *pathStr = "(not set)";
	if ((nextFolderResult == nullptr) || (PluginFolder == nullptr) || (hFindFolder == NULL) || (*hFindFolder == INVALID_HANDLE_VALUE))
		MX_SETERROR( MX1303	, MxError::InvalidParams, MxError::Abort, MxError::VerboseReport, "(nextFolderResult == nullptr) || (PluginFolder == nullptr) || (hFindFolder == NULL) || (*hFindFolder != INVALID_HANDLE_VALUE)" ); 
	else
	{
		std::wstring path(PluginFolder);
		int endPathPos = path.find_last_of('\\');
		if (endPathPos != path.length()-1)
			path += L"\\";

		try
		{
			USES_CONVERSION;
			pathStr = W2CA(path.c_str());

			WIN32_FIND_DATA ffd;

			while (::FindNextFile(*hFindFolder, &ffd) != 0)
			{
				if (FindFolder == true)
				{
					if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{                                                  //quite normal to find items that are not folders (files), '.' and '..' in a folder
						*nextFolderResult = path;
						*nextFolderResult += ffd.cFileName;
						*nextFolderResult += L"\\";
						rc = true;
						break;
					}
				}
				else
				{
					if (((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == false) && (IsValidLib(ffd.cFileName)))
					{                                                  //quite normal to find items that are not files (subfolders) in a folder
						*nextFolderResult = path;
						*nextFolderResult += ffd.cFileName;
						rc = true;
						break;
					}
				}
			}
		}
		catch(std::exception const& e)
		{
			MX_SETERROR( MX1334, MxError::System, MxError::Abort, MxError::QuietReport, "unexpected exception: GetLastError()=%d, %s [%s]", GetLastError(), e.what(), ((pathStr != nullptr) ? pathStr : "(pathStr is null)") );
			rc = false;
		}
		if ( rc == false )
		{
			if (*hFindFolder != INVALID_HANDLE_VALUE)
			{
				::FindClose(*hFindFolder);
				*hFindFolder = INVALID_HANDLE_VALUE;
			}
			*nextFolderResult = L"";
		}
	}
	return rc;
}


const char *MxPluginManager::ConvertItemName(const wchar_t *itemName)
{
	_itemName = MxPluginManager::InvalidName;

	if (itemName != nullptr)
	{
		try
		{
			USES_CONVERSION;
			_itemName = W2CA(itemName);
		}
		catch(...)
		{
		}
	}
	return _itemName.c_str();
}

bool MxPluginManager::SetFactoryDefaults()
{
	bool rc = false;

	_InitDone = false;

	MxError::Inst().Reset();			//cancel any existing error message - SetFactoryDefaults should only be called after an error message has been displayed to the user saying "SetFactoryDefaults needed"

	if ( UnRegister() == false)
	{
		MX_SETERROR ( MX1372, MxError::System, MxError::TerminateApp, MxError::QuietReport, "UnRegister(%s\\%s) failed", _ProdMngrOwner.c_str(), _ProdMngrName.c_str());
		MxError::Inst().Reset();		//cancel the error message - it may legitimately fail if the key doesn't exist
	}

	HKEY hAppRootKey = GetMxPluginManagerKey();		//creates HKEY_CURRENT_USER\Software\[ProductOwner]\[ProductName] if not found
	if(hAppRootKey != NULL)
	{
		if (MxUtils::SetRegSubKeyStr(hAppRootKey, MxPluginManager::RegNamePluginRootFolder, MxPluginManager::DefaultPluginRootFolder) == false)
			MX_SETERROR ( MX1344, MxError::System, MxError::TerminateApp, MxError::QuietReport, "SetRegSubKeyStr() failed");
		else
		{
			if ( MxUtils::SetRegSubKeyInt(hAppRootKey, MxPluginManager::RegNameAppCnt, 0) == false)
				MX_SETERROR ( MX1345, MxError::System, MxError::TerminateApp, MxError::QuietReport, "SetRegSubKeyInt() AppCnt = -1 failed");
			else
			{
				if( RegAppInit(_ProdMngrName.c_str(), _ProdMngrProdID, _ProdMngrSID, MxPluginManager::DefaultLogChannel, MxPluginManager::DefaultLogLevel) == false)
					MX_SETERROR ( MX1346, MxError::System,MxError::TerminateApp, MxError::QuietReport, "RegAppInit(%s)  failed", _ProdMngrName.c_str());
				else
				{
					MX_LOGMSG(MxError::QuietReport, "SUCCESS: SetFactoryDefaults %s\\%s", _ProdMngrOwner.c_str(), _ProdMngrName.c_str());
					rc = _InitDone;
				}
			}
		}
	}

	return rc;
}

bool MxPluginManager::UnRegister()
{
	bool rc = false;

	try
	{
		USES_CONVERSION;

		std::wstring company(A2CW(_ProdMngrOwner.c_str()));
		std::wstring prodName(A2CW(_ProdMngrName.c_str()));

		if((rc = MxUtils::DelRegAppKey(HKEY_CURRENT_USER, company.c_str(), prodName.c_str())) == false)
			MX_SETERROR ( MX1335, MxError::System, MxError::TerminateApp, MxError::QuietReport, "DelRegAppKey failed for %s\\%s",  _ProdMngrOwner.c_str(), _ProdMngrName.c_str());
		else
		{
			MX_LOGMSG(MxError::QuietReport, "SUCCESS: UnRegister %s\\%s",  _ProdMngrOwner.c_str(), _ProdMngrName.c_str());
			if ( _RegKeyMxPluginMngr != NULL)
				RegCloseKey(_RegKeyMxPluginMngr);
			_RegKeyMxPluginMngr = NULL;
			_InitDone = false;
			rc = true;
		}
	}
	catch(...)
	{
	}
    return rc;
}


int MxPluginManager::GetAppCnt()
{
	int rc = MxPluginManager::InvalidVal;

	HKEY hAppRootKey = GetMxPluginManagerKey();					
	if(hAppRootKey != NULL)
	{
		int value = MxPluginManager::InvalidVal;
		if (MxUtils::GetRegSubKeyInt(hAppRootKey, MxPluginManager::RegNameAppCnt, &value) == false)
			MX_SETERROR ( MX1336, MxError::System, MxError::TerminateApp, MxError::QuietReport, "MxUtils::GetRegSubKeyInt() failed for AppCnt - has SetFactoryDefaults() been called?");
		else
			rc = value;
	}

	return rc;
}

const char *MxPluginManager::GetAppProdID(int index, std::string *result)
{
	const char *rc = nullptr;

	if ((index < 0) || (index > MxPluginManager::AppCntMax) || (result == nullptr)) 
		MX_SETERROR ( MX1374, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "index = %d; zero or greater than MxPluginManager::AppCntMax (%d), alternatively result is null", index, MxPluginManager::AppCntMax); 
	else
	{
		int cnt = GetAppCnt();										
		if (index > cnt) 
			MX_SETERROR ( MX1329, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "index = %d (max = %d)", index, cnt ); 
		else
		{
			if ((cnt == MxPluginManager::InvalidVal) || (cnt == 0))
				MX_SETERROR ( MX1373, MxError::System, MxError::TerminateApp, MxError::QuietReport, "GetAppCnt() failed, cnt = %d", cnt);
			else
			{
				HKEY hAppRootKey = GetMxPluginManagerKey();			
				if(hAppRootKey != NULL)
				{
					HKEY hAppsKey = NULL;
					if (MxUtils::CreateRegAppSubKey(hAppRootKey, MxPluginManager::RegKeyApps, &hAppsKey) == false)
						MX_SETERROR ( MX1341, MxError::System, MxError::TerminateApp, MxError::QuietReport, "CreateRegAppSubKey() - Apps failed");	
					else
					{
						std::wstring KeyName(MxPluginManager::RegKeyApp);
						HKEY hAppKey = NULL;
						if (MxUtils::CreateRegAppSubKey(hAppsKey, MxPluginManager::ConvertAppKeyName(index, &KeyName), &hAppKey) == false)
							MX_SETERROR ( MX1342, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "CreateRegAppSubKey() App%s failed", index);
						else
						{
							std::wstring value;
							if( MxUtils::GetRegSubKeyStr(hAppKey, MxPluginManager::RegNameAppProdID, &value) == true)
							{							//AppCnt is not decremented when App is removed, so there is not a ProdID for every index. Therefore don't report an error
								try
								{
									USES_CONVERSION; 
									*result = W2CA(value.c_str());
									rc = result->c_str();
								}
								catch(...)
								{
								}
							}
							RegCloseKey(hAppKey);
						}
						RegCloseKey(hAppsKey);
					}
				}
			}
		}
	}
	return rc;
}


bool MxPluginManager::RegAppInit(const char *AppName, GUID ProdID, GUID SID, MxError::LogChannel channel, MxError::Level loglevel) 
{
	_InitDone = false;

	if (  (AppName == nullptr) || (strlen(AppName) < 1) || (ProdID == GUID_Invalid ) || (SID ==  GUID_Invalid) )
		MX_SETERROR( MX1322	, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "(AppName == nullptr) || (strlen(AppName) < 1) || (ProdID == GUID_Invalid ) || (SID ==  GUID_Invalid)" ); 
	else
	{
		MxError::Inst().Reset();	//RegAppInit is called at the start of a program so no error should be set

		if (GetAppIndex(ProdID) != MxPluginManager::InvalidVal )
		{
			_InitDone = true;
		}
		else
		{
			if (MxError::Inst().IsErrorSet() == false)
			{
				HKEY hAppRootKey = GetMxPluginManagerKey();
				if(hAppRootKey != NULL)
				{
					int index = MxPluginManager::InvalidVal;
					if (MxUtils::GetRegSubKeyInt(hAppRootKey, MxPluginManager::RegNameAppCnt, &index) == false)
						MX_SETERROR ( MX1349, MxError::Install, MxError::Abort, MxError::QuietReport, "MxUtils::GetRegSubKeyInt() - appCnt failed - has SetFactoryDefaults been called?");
					else
					{
						if ( MxUtils::SetRegSubKeyInt(hAppRootKey, MxPluginManager::RegNameAppCnt, index+1) == false)
							MX_SETERROR ( MX1350, MxError::System, MxError::TerminateApp, MxError::QuietReport, "SetRegSubKeyInt() AppCnt = %d failed", index+1);
						else
						{
							HKEY hAppsKey = NULL;
							if (MxUtils::CreateRegAppSubKey(hAppRootKey, MxPluginManager::RegKeyApps, &hAppsKey) == false)
								MX_SETERROR ( MX1348, MxError::System, MxError::TerminateApp, MxError::QuietReport, "CreateRegAppSubKey() - Apps failed");
							else
							{
								std::wstring KeyName(MxPluginManager::RegKeyApp);
								HKEY hAppKey = NULL;
								if (MxUtils::CreateRegAppSubKey(hAppsKey, MxPluginManager::ConvertAppKeyName(index, &KeyName), &hAppKey) == false)
									MX_SETERROR ( MX1351, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "CreateRegAppSubKey - App%d failed", index);
								else
								{
									std::wstring nameStr;
									if (MxUtils::SetRegSubKeyStr(hAppKey, MxPluginManager::RegNameAppName, MxPluginManager::ConvertName(AppName, &nameStr)) == false)
										MX_SETERROR ( MX1352, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "SetRegSubKeyStr - App%d,set Name=%s failed", index, AppName);
									else
									{
										std::wstring ProdIDStr;
										if ( MxUtils::SetRegSubKeyStr(hAppKey, MxPluginManager::RegNameAppProdID, MxPluginManager::ConvertGUID(ProdID, &ProdIDStr)) == false)
											MX_SETERROR ( MX1353, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "SetRegSubKeyStr - App%d, set ProdID failed", index);
										else
										{
											std::wstring SIDStr;
											if ( MxUtils::SetRegSubKeyStr(hAppKey, MxPluginManager::RegNameAppSID, MxPluginManager::ConvertGUID(SID, &SIDStr)) == false)
												MX_SETERROR ( MX1364, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "SetRegSubKeyStr - App%d, set SID failed", index);
											else
											{
												if (MxUtils::SetRegSubKeyInt(hAppKey, MxPluginManager::RegNameAppLogLevel, loglevel) == false)
													MX_SETERROR ( MX1354, MxError::System, MxError::TerminateApp, MxError::QuietReport, "SetRegSubKeyStr - App%d, set loglevel failed", index);
												else
												{
													if (MxUtils::SetRegSubKeyInt(hAppKey, MxPluginManager::RegNameAppLogChannel, channel) == false)
														MX_SETERROR ( MX1355, MxError::System, MxError::TerminateApp, MxError::QuietReport, "SetRegSubKeyStr - App%d, set logchannel failed", index);
													else
													{
														std::string tmp;
														MX_LOGMSG(MxError::QuietReport, "SUCCESS: First time RegAppInit() for %s - %s", AppName, MxGUID::ConvertToString( ProdID, &tmp));
														_InitDone = true;
													}
												}
											}
										}
									}
									RegCloseKey(hAppKey);
								}
								RegCloseKey(hAppsKey);
							}
						}
					}
				}
			}
		}
	}
	return _InitDone;
}

bool MxPluginManager::UnRegApp(const char *ProdID)		//removes subkey HKEY_CURRENT_USER\Software\Maximodex\[ProductMngrName]\Apps\App*
{
	bool rc = false;

	if (_InitDone == false)
		MX_SETERROR ( MX1379, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "_InitDone == false - has RegAppInit been called?");
	else
	{
		int index = MxPluginManager::InvalidVal;
		if ((index = GetAppIndex(ProdID)) == MxPluginManager::InvalidVal)			
			MX_SETERROR( MX1323, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "GetAppIndex(%s) failed", ((ProdID != nullptr) ? ProdID : "(null)") ); 
		else
		{
			std::wstring appKey;
			if (MxPluginManager::ConvertAppKeyName(index, &appKey) == nullptr)
				MX_SETERROR ( MX1357, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ConvertAppKeyName - App%d  failed", index);
			else
			{
				try
				{
					USES_CONVERSION;

					std::wstring company(A2CW(_ProdMngrOwner.c_str()));
					std::wstring appName(A2CW(_ProdMngrName.c_str()));

					std::wstring deleteKey(MxPluginManager::RegKeyApps);
					deleteKey += appKey;
					if(MxUtils::DelRegAppKey(HKEY_CURRENT_USER, company.c_str(), appName.c_str(), deleteKey.c_str()) == false)
						MX_SETERROR ( MX1358, MxError::System, MxError::Abort, MxError::QuietReport, "DelRegAppKey failed for %s%s%s", W2CA(company.c_str()), W2CA(appName.c_str()), W2CA(deleteKey.c_str()));
					else
					{
						_InitDone = false;
						rc = true;
					}
				}
				catch(...)
				{
				}
 			}
		}
	}
	return rc;
}


const char *MxPluginManager::GetAppName(const char *ProdID, std::string *result)	//returns "[invalid]" if ProdID not found in HKEY_CURRENT_USER\Software\Maximodex\MxPluginManager\Apps subfolders
{
	const char * rc =  MxPluginManager::InvalidName;

	if (_InitDone == false)
		MX_SETERROR ( MX1381, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "_InitDone == false - has RegAppInit been called?");
	else
	{
		int index = MxPluginManager::InvalidVal;
		if (((index = GetAppIndex(ProdID)) == MxPluginManager::InvalidVal)	|| (result == nullptr))			
			MX_SETERROR( MX1328	, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "GetAppIndex(%s) failed or AppName is nullptr", ((ProdID != nullptr) ? ProdID : "(null)") ); 
		else
		{
			HKEY hAppKey = GetAppKey(index);
			if(hAppKey != NULL)
			{	
				std::wstring value;
				if (MxUtils::GetRegSubKeyStr(hAppKey, MxPluginManager::RegNameAppName, &value) == false)
					MX_SETERROR ( MX1363, MxError::System, MxError::TerminateApp, MxError::QuietReport, "GetRegSubKeyStr() failed - App%d - Name", index);
				else
				{
					try
					{
						USES_CONVERSION; 
						*result = W2CA(value.c_str());
						rc = result->c_str();
					}
					catch(...)
					{
					}
				}
				RegCloseKey(hAppKey);
			}
		}
	}
	return rc;
}

const char *MxPluginManager::GetAppKeyName(const char *ProdID, std::string *result)
{
	const char * rc =  MxPluginManager::InvalidName;

	if (_InitDone == false)
		MX_SETERROR ( MX1382, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "_InitDone == false - has RegAppInit been called?");
	else
	{
		int index = MxPluginManager::InvalidVal;
		if (((index = GetAppIndex(ProdID)) == MxPluginManager::InvalidVal)	|| (result == nullptr))			
			MX_SETERROR( MX1356, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "GetAppIndex(%s) failed or KeyName is nullptr", ((ProdID != nullptr) ? ProdID : "(null)") ); 
		else
		{
			try
			{
				USES_CONVERSION;

				*result = W2CA(MxPluginManager::RegKeyApp);
				char buff[5];
				_itoa_s(index, buff, 5, 10);
				*result += buff;
				rc = result->c_str();
			}
			catch(...)
			{
			}
		}
	}
	return rc;
}

const char * MxPluginManager::GetAppSID(const char *ProdID, std::string *result)
{
	const char * rc =  MxPluginManager::InvalidName;

	if (_InitDone == false)
		MX_SETERROR ( MX1313, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "_InitDone == false - has RegAppInit been called?");
	else
	{
		int index = MxPluginManager::InvalidVal;
		if (((index = GetAppIndex(ProdID)) == MxPluginManager::InvalidVal)	|| (result == nullptr))			
			MX_SETERROR( MX1359, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "GetAppIndex(%s) failed or KeyName is nullptr", ((ProdID != nullptr) ? ProdID : "(null)") ); 
		else
		{
			HKEY hAppKey = GetAppKey(index);
			if(hAppKey != NULL)
			{	
				std::wstring value;
				if (MxUtils::GetRegSubKeyStr(hAppKey, MxPluginManager::RegNameAppSID, &value) == false)
					MX_SETERROR ( MX1360, MxError::System, MxError::TerminateApp, MxError::QuietReport, "GetRegSubKeyStr() failed - App%d - SID", index);
				else
				{
					try
					{
						USES_CONVERSION; 
						*result = W2CA(value.c_str());
						rc = result->c_str();
					}
					catch(...)
					{
					}
				}
				RegCloseKey(hAppKey);
			}
		}
	}
	return rc;
}

MxError::LogChannel  MxPluginManager::GetAppLogChannel(const char *ProdID)
{
	MxError::LogChannel rc = MxError::NoLogging;

	if (_InitDone == false)
		MX_SETERROR ( MX1383, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "_InitDone == false - has RegAppInit been called?");
	else
	{
		int index = MxPluginManager::InvalidVal;
		if ((index = GetAppIndex(ProdID)) == MxPluginManager::InvalidVal)			
			MX_SETERROR( MX1324	, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "GetAppIndex(%s) failed", ((ProdID != nullptr) ? ProdID : "(null)") ); 
		else
		{
			HKEY hAppKey = GetAppKey(index);
			if(hAppKey != NULL)
			{	
				int value = 0;
				if (MxUtils::GetRegSubKeyInt(hAppKey, MxPluginManager::RegNameAppLogChannel, &value) == false)
					MX_SETERROR ( MX1365, MxError::System, MxError::TerminateApp, MxError::QuietReport, "GetRegSubKeyInt() failed - App%d - Channel", index);
				else
				{
					rc = (MxError::LogChannel)value;
				}
				RegCloseKey(hAppKey);
			}
		}
	}
	return rc;
}

bool MxPluginManager::SetAppLogChannel(const char *ProdID,  MxError::LogChannel channel)
{
	bool rc = false;

	if (_InitDone == false)
		MX_SETERROR ( MX1384, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "_InitDone == false - has RegAppInit been called?");
	else
	{
		int index = MxPluginManager::InvalidVal;
		if ((index = GetAppIndex(ProdID)) == MxPluginManager::InvalidVal)			
			MX_SETERROR( MX1325	, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "GetAppIndex(%s) failed", ((ProdID != nullptr) ? ProdID : "(null)") ); 
		else
		{
			HKEY hAppKey = GetAppKey(index);
			if(hAppKey != NULL)
			{	
				if (MxUtils::SetRegSubKeyInt(hAppKey, MxPluginManager::RegNameAppLogChannel, channel) == false)
					MX_SETERROR ( MX1367, MxError::System, MxError::TerminateApp, MxError::QuietReport, "SetRegSubKeyInt() failed - App%d - Channel", index);
				else
				{
					rc = true;
				}
				RegCloseKey(hAppKey);
			}
		}
	}
	return rc;
}

MxError::Level MxPluginManager::GetAppLogLevel(const char *ProdID)
{
	MxError::Level rc = MxError::QuietReport;

	if (_InitDone == false)
		MX_SETERROR ( MX1385, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "_InitDone == false - has RegAppInit been called?");
	else
	{
		int index = MxPluginManager::InvalidVal;
		if ((index = GetAppIndex(ProdID)) == MxPluginManager::InvalidVal)			
			MX_SETERROR( MX1326	, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "GetAppIndex(%s) failed", ((ProdID != nullptr) ? ProdID : "(null)") ); 
		else
		{
			HKEY hAppKey = GetAppKey(index);
			if(hAppKey != NULL)
			{	
				int value = 0;
				if (MxUtils::GetRegSubKeyInt(hAppKey, MxPluginManager::RegNameAppLogLevel, &value) == false)
					MX_SETERROR ( MX1369, MxError::System, MxError::TerminateApp, MxError::QuietReport, "GetRegSubKeyInt() failed - App%d - Level", index);
				else
				{
					rc = (MxError::Level)value;
				}
				RegCloseKey(hAppKey);
			}

		}
	}
	return rc;
}

bool MxPluginManager::SetAppLogLevel(const char *ProdID,  MxError::Level level)
{
	bool rc = false;

	if (_InitDone == false)
		MX_SETERROR ( MX1386, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "_InitDone == false - has RegAppInit been called?");
	else
	{
		int index = MxPluginManager::InvalidVal;
		if ((index = GetAppIndex(ProdID)) == MxPluginManager::InvalidVal)			
			MX_SETERROR( MX1327	, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "GetAppIndex(%s) failed", ((ProdID != nullptr) ? ProdID : "(null)") ); 
		else
		{
			HKEY hAppKey = GetAppKey(index);
			if(hAppKey != NULL)
			{	
				if (MxUtils::SetRegSubKeyInt(hAppKey, MxPluginManager::RegNameAppLogLevel, level) == false)
					MX_SETERROR ( MX1371, MxError::System, MxError::TerminateApp, MxError::QuietReport, "SetRegSubKeyInt() failed - App%d - Level", index);
				else
				{
					rc = true;
				}
				RegCloseKey(hAppKey);
			}
		}
	}
	return rc;
}

HKEY MxPluginManager::GetMxPluginManagerKey()		// dtor calls CloseRegKey(_RegKeyMxPluginMngr) 
{
	HKEY rc =_RegKeyMxPluginMngr;

	if (rc == NULL)
	{
		try
		{
			USES_CONVERSION;

			std::wstring owner(A2CW(_ProdMngrOwner.c_str()));
			std::wstring product( A2CW(_ProdMngrName.c_str()));

			if((MxUtils::CreateRegAppRootKey(HKEY_CURRENT_USER, owner.c_str(), product.c_str(), &rc) == false) || (rc == NULL))
				MX_SETERROR ( MX1333, MxError::System, MxError::TerminateApp, MxError::QuietReport, "CreateRegAppRootKey failed");
			else
			{
				_RegKeyMxPluginMngr = rc;
			}
		}
		catch(...)
		{
		}

	}
	return rc;
}

HKEY MxPluginManager::GetAppKey(int index)
{
	HKEY rc = NULL;
		
	HKEY hAppRootKey = GetMxPluginManagerKey();		
	if(hAppRootKey != NULL)
	{
		std::wstring appKeyName;
		if (MxPluginManager::ConvertAppKeyName(index, &appKeyName) == nullptr)
			MX_SETERROR ( MX1361, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ConvertAppKeyName - App%d  failed", index);
		else
		{
			std::wstring keyName(MxPluginManager::RegKeyApps);
			keyName += appKeyName;

			HKEY hSubKey = NULL;
			if ( (MxUtils::CreateRegAppSubKey(hAppRootKey, keyName.c_str(), &hSubKey) == false) || (hSubKey == NULL))
				MX_SETERROR ( MX1362, MxError::System, MxError::TerminateApp, MxError::QuietReport, "CreateRegAppSubKey - App%d  failed", index);
			else
			{
				rc = hSubKey;
			}
		}
	}
	return rc;
}

int  MxPluginManager::GetAppIndex(const GUID ProdID)							//returns MxPluginManager::InvalidVal if not found, or >=0 and < GetAppCnt()
{
	int rc = MxPluginManager::InvalidVal;

	if (ProdID == GUID_Invalid ) 
		MX_SETERROR( MX1387, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "(ProdID == GUID_Invalid )" ); 
	else
	{
		std::string tmp;
		rc = GetAppIndex(MxGUID::ConvertToString(ProdID, &tmp));
	}
	return rc;
}

int MxPluginManager::GetAppIndex(const char *ProdID)
{
	int rc = MxPluginManager::InvalidVal;

	if (ProdID == nullptr)
		MX_SETERROR ( MX1347, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "ProdID == nullptr");
	else
	{
		int cnt = MxPluginManager::InvalidVal;
		if ( (cnt = GetAppCnt()) != MxPluginManager::InvalidVal)		
		{
			for (int x = 0; x < cnt; x++)
			{
				std::string tmp;
				const char *prodid = GetAppProdID(x, &tmp);
				if ((prodid != nullptr) && (strncmp(prodid, ProdID, MxUtils::GuidStrLen) == 0))
				{
					rc = x;
					break;
				}
			}
			if (rc == MxPluginManager::InvalidVal)
				MX_LOGMSG (MxError::VerboseReport, "ProdID not found - %s - maybe first time RegAppInit() called for it on this PC", ProdID);
		}
	}
	return rc;
}


const wchar_t * MxPluginManager::ConvertAppKeyName(int AppCnt, std::wstring *result)
{
	const wchar_t *rc = nullptr;

	if ((result == nullptr) || (AppCnt < 0) || (AppCnt > MxPluginManager::AppCntMax))
		MX_SETERROR(  MX1330, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "KeyName is null or AppCnt is invalid (%d)", AppCnt ); 
	else
	{
		*result = MxPluginManager::RegKeyApp;
		wchar_t buff[5];
		_itow_s(AppCnt, buff, 5, 10);
		*result += buff;
		*result += L"\\";
		rc = result->c_str();
	}
	return rc;
}

const wchar_t * MxPluginManager::ConvertGUID(GUID guid, std::wstring *result)
{
	const wchar_t *rc = nullptr;

	if (result == nullptr)
		MX_SETERROR(  MX1331, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "result is null "); 
	else
	{
		try
		{
			USES_CONVERSION;  

			std::string tmp;
			MxGUID::ConvertToString( guid, &tmp);

			*result = A2CW(tmp.c_str());
			rc = result->c_str();
		}
		catch(...)
		{
		}
	}
	return rc;
}

const wchar_t * MxPluginManager::ConvertName(const char *Name, std::wstring *result)
{
	const wchar_t *rc = nullptr;

	if ((Name == nullptr) || (result == nullptr))
		MX_SETERROR( MX1332, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "Name  or result is null "); 
	else
	{
		try
		{
			USES_CONVERSION;  

			*result = A2CW(Name);
			rc = result->c_str();
		}
		catch(...)
		{
		}
	}
	return rc;
}

void MxPluginManager::SetErrorOptions(MxError::LogChannel ReportChannel, MxError::Level ReportLevel)
{
	if (_SetErrorOptionsDone == false)
	{
		MX_SETUP(LibInfo::Inst().GetOwner(), PRODID_MxPluginManager1, LibInfo::Inst().GetProductName(), LibInfo::Inst().GetVersion(), ReportChannel, ReportLevel, "app starts");
		_SetErrorOptionsDone = true;
	}
}
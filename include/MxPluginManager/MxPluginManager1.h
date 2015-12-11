#pragma once

#ifdef MXPLUGINMANAGER_EXPORTS
#define MXPLUGINMANAGER_API __declspec(dllexport)
#else
#define MXPLUGINMANAGER_API __declspec(dllimport)
#endif

#pragma warning(disable: 4995)
#include <vector>
#pragma warning(default: 4995)

#include "MxPluginManager\MxPluginManagerErrorCodes1.h"
#include "MxPluginManager\MxPluginLib1.h"

#include "MxUtils\LibInfo.h"


class MXPLUGINMANAGER_API MxPluginManager 
{
public:
	static const int				MxPluginManager::AppCntMax = 500;		//MxPluginManager::ConvertAppKeyName - increase size of buff if MxPluginManager::AppCntMax > 9999
	static const int				MxPluginManager::InvalidVal = -1;	
	static const int				MxPluginManager::ErrorSetFactoryDefaultNeeded = MX1336;	
	static const int				MxPluginManager::LoadDllSearchOptions = LOAD_LIBRARY_SEARCH_DEFAULT_DIRS | LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR;

	static const char * const       MxPluginManager::PluginDependenciesFilename;	//MxPlugin.dep
	static const char * const       MxPluginManager::FnNameCreateClassInstance;
	static const char * const       MxPluginManager::InvalidName;

	static const wchar_t * const    MxPluginManager::SysEnvVarNameMxPlugin_Root;
	static const wchar_t * const    MxPluginManager::DefaultPluginRootFolder;

	static const MxError::LogChannel MxPluginManager::DefaultLogChannel = MxError::Development;		
	static const MxError::Level		 MxPluginManager::DefaultLogLevel = MxError::VerboseReport;

	static const wchar_t * const    MxPluginManager::RegNameAppCnt;
	static const wchar_t * const    MxPluginManager::RegNamePluginRootFolder;

	static const wchar_t * const    MxPluginManager::RegKeyApps;
	static const wchar_t * const    MxPluginManager::RegKeyApp;

	static const wchar_t * const    MxPluginManager::RegNameAppProdID;
	static const wchar_t * const    MxPluginManager::RegNameAppSID;
	static const wchar_t * const    MxPluginManager::RegNameAppName;
	static const wchar_t * const    MxPluginManager::RegNameAppLogLevel;
	static const wchar_t * const    MxPluginManager::RegNameAppLogChannel;

public:
	MxPluginManager(void);
	MxPluginManager(MxError::LogChannel ReportChannel, MxError::Level ReportLevel, GUID ProductMngrSID = SID_Universal);
	virtual ~MxPluginManager(void);

	virtual bool	RefreshPluginsInRootFolder(const GUID SID, bool BreakOnDllLoadError);	//if SID == GUID_Invalid, then add all Plugins found in subfolders of PluginRootFolder to list
	virtual void	ClearAllMxPlugins();

	int				GetPluginCount() { return static_cast<int>(_Libs.size()); } 
	MxPluginLib *	GetPlugin(int index) { return ((index >= 0) && (index < (int)_Libs.size())) ? _Libs[index] : nullptr; }
	MxPluginLib *	GetPlugin(const GUID LibID);

	const char *GetLibName(void) { return LibInfo::Inst().GetProductName();	}				//VS_VERSION_INFO::ProductName
	const char *GetLibDescription(void) { return LibInfo::Inst().GetDescription(); }			//VS_VERSION_INFO::FileDescription
	const char *GetLibVersion(void) { return LibInfo::Inst().GetVersion(); }					//VS_VERSION_INFO::ProductVersion
	const char *GetLibOwnerName(void) { return LibInfo::Inst().GetOwner();	}				//VS_VERSION_INFO::CompanyName
	const char *GetLibCopyrightNotice(void) { return LibInfo::Inst().GetCopyrightNotice(); }	//VS_VERSION_INFO::LegalCopyright
	const char *GetLibInternalName() { return LibInfo::Inst().GetProductID(); }				//VS_VERSION_INFO::Internal Name

	const char *GetLibSupportSite(void) { return "http://www.mxplugin.org";	}		
	const char *GetLibLicenseName(void) { return "New BSD Licence"; }								
	const char *GetLibLicenseURL(void)  { return "http://mxplugin.codeplex.com/license"; }	
	const wchar_t *GetLibPathFilename(void) { return LibInfo::Inst().GetwPathFilename(); }	//filename returned by ::GetModuleFileName() of hModule in DllMain, DLL_PROCESS_ATTACH	

				//Windows Registry support - use the following only if your app needs close integration with MxPluginManagerGUI or similar Product Manager App - see installnotes.txt
									//functions for your app to call - must call RegAppInit during your app's initialisation and before any of the other functions in this section - if it fails error=MX1349 then call SetFactoryDefaults() and try again
	virtual bool				 RegAppInit(const char *AppName, GUID ProdID, GUID SID, MxError::LogChannel channel, MxError::Level loglevel ); //ProdID=ProdId*::ProdID(), AppName=ProdId*::Name() - see MxProductID.h 
	virtual const wchar_t	    *GetRootFolder() { return _MxPluginRootDir.c_str(); }    		//returns  contents of SysEnv variable MXPLUGIN_ROOT or MxPluginManager::DefaultPluginRootFolder if not found
	virtual bool				 UnRegApp(const char *ProdID);									//removes subkey App* in HKEY_CURRENT_USER\Software\[ProductMngrOwner]\[ProductMngrName]\Apps,
	virtual MxError::LogChannel  GetAppLogChannel(const char *ProdID);
	virtual bool				 SetAppLogChannel(const char *ProdID,  MxError::LogChannel channel);
	virtual MxError::Level		 GetAppLogLevel(const char *ProdID);
	virtual bool				 SetAppLogLevel(const char *ProdID,  MxError::Level level);
	virtual const char *		 GetAppName(const char *ProdID, std::string *result);			//returns AppName param of RegAppInit(), or "[invalid]" if ProdID not found in HKEY_CURRENT_USER\Software\[ProductMngrOwner]\[ProductMngrName]\Apps subfolders
	virtual const char *		 GetAppKeyName(const char *ProdID, std::string *result);		//returns App*, or "[invalid]" if ProdID not found in HKEY_CURRENT_USER\Software\[ProductMngrOwner]\[ProductMngrName]\Apps subfolders
	virtual const char *		 GetAppSID(const char *ProdID, std::string *result);			//returns SID param of RegAppInit(), or "[invalid]" if ProdID not found in HKEY_CURRENT_USER\Software\[ProductMngrOwner]\[ProductMngrName]\Apps subfolders
									//functions for MxPluginManagerGUI (or equivalent) to call - may be called before RegAppInit 
	virtual bool				 SetFactoryDefaults();											//Sets [ProductMngrOwner]\[ProductMngrName] default settings in Windows registry - see ProdIdMxPluginManagerGUI in MxProductID.h
	virtual bool				 UnRegister();													//Removes [ProductMngrOwner]\[ProductMngrName] settings from Windows registry - relatively safe because it must child of HKEY_CURRENT_USER\Software
	virtual int					 GetAppCnt();													//returns contents of HKEY_CURRENT_USER\Software\[ProductMngrOwner]\[ProductMngrName]\AppCnt 
	virtual const char *         GetAppProdID(int index, std::string *result);					//returns {...} or nullptr if ProdID not found in HKEY_CURRENT_USER\Software\[ProductMngrOwner]\[ProductMngrName]\Apps subkeys
	virtual int                  GetAppIndex(const GUID ProdID);										//returns MxPluginManager::InvalidVal if not found, or >=0 and < GetAppCnt()
	virtual int					 GetAppIndex(const char *ProdID);
									//helper functions
	static const wchar_t *ConvertAppKeyName(int AppCnt, std::wstring *result);			//returns 'App6\\' for AppCnt=6,  or nullptr if error
	static const wchar_t *ConvertGUID(GUID guid, std::wstring *result);					//returns  GUID as a wchar_t* {...}, or nullptr if  error
	static const wchar_t *ConvertName(const char *Name, std::wstring *result);			//returns Name as a wchar_t*, or nullptr if error
				//Windows Registry support - use the above only if your app needs close integration with MxPluginManagerGUI or similiar Product Manager App - see installnotes.txt

	static void			  CleanupOnAppExitOnly() { LibInfo::Inst().CleanupOnAppExitOnly();}	//deletes all allocated memory (inst cannot be used after this call) - stops VS2012 reporting memory leaks before ~LibInfo() is called; only call this if there's a problem

private:
	void		SetErrorOptions(MxError::LogChannel ReportChannel, MxError::Level ReportLevel); //make App call close to where it calls MX_SETUP so as to call MX_SETUP for MxPluginManager

	bool		RefreshPlugins(const wchar_t *PluginFolder, const GUID ProductGUID, bool BreakOnDllLoadError);
	bool		AddMxPlugin(MxPluginLib *lib);	
	bool		IsExisting(MxPluginLib *lib);
	bool		IsPluginForApp(const GUID PluginSocketID, const GUID ProductGUID);
	void		ClearAllSelections();
	bool		DeleteNextUnselected();

	bool		GetFirstPluginFolderItem(std::wstring *firstFolderResult, const wchar_t *PluginRootFolder, HANDLE *hFindFolder, bool FindFolder);
	bool		GetNextPluginFolderItem(std::wstring *nextFolderResult, const wchar_t *PluginFolder, HANDLE *hFindFolder, bool FindFolder);
	bool		IsValidLib(const wchar_t *filename);

	const char *ConvertItemName(const wchar_t *itemName);

	HKEY		GetMxPluginManagerKey();			//returns key for HKEY_CURRENT_USER\Software\[ProductMngrOwner]\[ProductMngrName] - dtor calls RegCloseKey(_RegKeyMxPluginMngr) so caller must NOT call RegCloseKey() when done
	HKEY		GetAppKey(int index);				//returns key for HKEY_CURRENT_USER\Software\[ProductMngrOwner]\[ProductMngrName]\Apps\App* - caller is responsible for calling RegCloseKey() when done


private:
#pragma warning(disable: 4251)
	std::vector<MxPluginLib *> _Libs;
	std::string				   _itemName;
	std::string				   _ProdMngrName;	
	std::string				   _ProdMngrOwner;

	std::wstring			    _MxPluginRootDir;
#pragma warning(default:4251)
	GUID	_ProdMngrProdID;
	GUID	_ProdMngrSID;
	HKEY	_RegKeyMxPluginMngr;
	bool	_InitDone;
	bool    _SetErrorOptionsDone;
};
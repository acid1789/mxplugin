//declaration of global functions implemented in DllMain.cpp

extern const char *gLibName(void);					//VS_VERSION_INFO::ProductName
extern const char *gLibDescription(void);			//VS_VERSION_INFO::FileDescription
extern const char *gLibVersion(void);				//VS_VERSION_INFO::ProductVersion
extern const char *gLibOwnerName(void);				//VS_VERSION_INFO::CompanyName
extern const char *gLibCopyrightNotice(void);		//VS_VERSION_INFO::LegalCopyright
extern const char *gLibID(void);					//VS_VERSION_INFO::InternalName GUID - same as its folder name in PluginRootFolder
extern const wchar_t *gLibPathFilename(void);		//filename returned by ::GetModuleFileName() of hModule in DllMain, DLL_PROCESS_ATTACH
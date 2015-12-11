#pragma once

#pragma warning(disable:4995)
#include <vector>
#pragma warning(default:4995)

#include "MxPluginLib.h"


class MxPluginLibs
{
public:
	static const char * const MxPluginLibs::FnNameCreateClassInstance;
	static const wchar_t * const MxPluginLibs::ErrorCodeList;
	static const wchar_t * const MxPluginLibs::ErrorCodeListMicrosoft;

	static const DWORD MxPluginLibs::NoError = 0;						//GetLastError() returns values in range 0 (success) to 15999
	static const DWORD MxPluginLibs::UnknownError = 30000;		
	static const DWORD MxPluginLibs::CreateMxPluginLibError = 30001;
	static const DWORD MxPluginLibs::AddMxPluginLibError = 30002;
	static const DWORD MxPluginLibs::ExceptionError = 30003;
	static const DWORD MxPluginLibs::InvalidParams = 3004;
	static const DWORD MxPluginLibs::NoMxPluginsForIDD = 30005;
	static const DWORD MxPluginLibs::InvalidRegKey = 30006;

public:
	MxPluginLibs(void);
	~MxPluginLibs(void);

	void			ClearAllMxPlugins();

	int				GetPluginCount() { return static_cast<int>(_Libs.size()); } 
	MxPluginLib *	GetPlugin(int index) { return ((index >= 0) && (index < (int)_Libs.size())) ? _Libs[index] : '\0'; }
	MxPluginLib *	GetPlugin(const char *LibID);
	DWORD			RefreshPluginsInSubFolders(const wchar_t *PluginRootFolder, const char *IID);
	DWORD			RefreshPluginsInFolder(const wchar_t *PluginFolder, const char *IID);

private:
	DWORD		RefreshPlugins(const wchar_t *PluginFolder, const char *IID);
	bool		AddMxPlugin(MxPluginLib *lib);	
	bool		IsExisting(MxPluginLib *lib);
	void		ClearAllSelections();
	bool		DeleteNextUnselected();


	DWORD		GetFirstPluginFolderItem(std::wstring *firstFolderResult, const wchar_t *PluginRootFolder, HANDLE *hFindFolder, bool FindFolder);
	DWORD		GetNextPluginFolderItem(std::wstring *nextFolderResult, const wchar_t *PluginFolder, HANDLE *hFindFolder, bool FindFolder);

private:
	std::vector<MxPluginLib *> _Libs;
};


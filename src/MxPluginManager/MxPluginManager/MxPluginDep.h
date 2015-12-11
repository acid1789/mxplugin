#pragma once

#include <vector>
#define INFO_BUFFER_SIZE 32767

class MxPluginDep
{
public:
	static const int	 MxPluginDep::Invalid = -1;

public:
	MxPluginDep(const wchar_t *PluginFolder);
	~MxPluginDep();

	bool IsValid() { return _valid; }

private:
	bool		RemoveDirectories();
	bool		AddFolder(const wchar_t *wPluginDepFolder, const char *pathfilenameDep, const char *line);
	int			GetFirstFolder(const char *line, std::string *folderlist);
	int			GetNextFolder(const char *folderlist, int start);


	TCHAR							  _infoBuf[INFO_BUFFER_SIZE];
	std::vector<DLL_DIRECTORY_COOKIE> _cookies;
	bool	_valid;
};


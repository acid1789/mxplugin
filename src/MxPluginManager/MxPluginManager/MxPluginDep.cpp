#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <string>

#include "MxUtils\MxUtils1.h"
#include "MxUtils\MxError1.h"
#include "MxPluginManager\MxPluginManagerErrorCodes1.h"

#include "MxPluginManager\MxPluginManager1.h"

#include "MxPluginDep.h"



MxPluginDep::MxPluginDep(const wchar_t *PluginFolder) : 
	_valid(false)
{
	USES_CONVERSION;

	if (PluginFolder == nullptr)
		MX_SETERROR(MX1394, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "PluginFolder is nullptr");
	else
	{
		std::wstring wPluginFolder(PluginFolder);
		std::ifstream depfile;

		try
		{
			int endPathPos = wPluginFolder.find_last_of('\\');
			if (endPathPos != wPluginFolder.length() - 1)
				wPluginFolder += L"\\";

			int index = wPluginFolder.find_last_of('{');
			if ((index == std::string::npos) || (index < 1))
				MX_SETERROR(MX1393, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "PluginFolder %s is invalid", W2CA(PluginFolder));
			else
			{
				std::wstring wPluginRoot(wPluginFolder.substr(0, index));
				std::string  pathfilenameDep(W2CA(wPluginFolder.c_str()));
				pathfilenameDep += MxPluginManager::PluginDependenciesFilename;

				_valid = true;
				RemoveDirectories();

				depfile.open(pathfilenameDep, std::ios::in);
				while (depfile.good())
				{
					std::wstring wPluginDepFolder(L"");
					std::string line("");
					std::getline(depfile, line);

					if ((line[0] == '#') || (line.length() == 0))
					{
						//comments or empty lines are ignored
					}
					else if (line[0] == '{')
					{	//MxPlugin folder
						wPluginDepFolder = wPluginRoot;
						wPluginDepFolder += L"\\";
						wPluginDepFolder += A2CW(line.c_str());
						if (AddFolder(wPluginDepFolder.c_str(), pathfilenameDep.c_str(), line.c_str()) == false)
							break;
					}
					else if (line[0] == '%')
					{	//environmental variable
						bool err = false;

						std::string folderlist("");
						int end = 0;
						int start = GetFirstFolder(line.c_str(), &folderlist);
						while ((end = GetNextFolder(folderlist.c_str(), start)) != MxPluginDep::Invalid)
						{
							std::string folder(folderlist.substr(start, end-start));
							if (folder.length() > 0)
							{
								wPluginDepFolder = A2CW(folder.c_str());
								if (AddFolder(wPluginDepFolder.c_str(), pathfilenameDep.c_str(), folder.c_str()) == false)
								{
									err = true;
									break;
								}
							}
							start = end+1;
						}
						if (err == true)
							break;
					}
					else
					{	//drive, path, filename
						wPluginDepFolder = A2CW(line.c_str());
						if (AddFolder(wPluginDepFolder.c_str(), pathfilenameDep.c_str(), line.c_str()) == false)
							break;
					}
				}
			}
		}
		catch (std::exception const& e)
		{
			MX_SETERROR(MX1391, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "Unexpected exception: %s", e.what());
			_valid = false;
		}
		depfile.close();
	}
}


MxPluginDep::~MxPluginDep()
{
	RemoveDirectories();
}

bool MxPluginDep::RemoveDirectories()
{
	size_t cnt = _cookies.size();
	for (size_t x = 0; x < cnt; x++)
	{
		if (::RemoveDllDirectory(_cookies[x]) == 0)
		{
			MX_SETERROR(MX1392, MxError::System, MxError::Abort, MxError::VerboseReport, "unable to remove folder %d from DLL search, error=%d", x, (int)GetLastError());
			_valid = false;
		}
	}
	_cookies.clear();

	return 	_valid;
}

bool MxPluginDep::AddFolder(const wchar_t *wPluginDepFolder, const char *pathfilenameDep, const char *line)
{
	bool rc = false;

	if ((wPluginDepFolder == nullptr) || (pathfilenameDep == nullptr) || (line == nullptr))
		MX_SETERROR(MX1396, MxError::InvalidParams, MxError::Abort, MxError::VerboseReport, "one or more params are null");
	else
	{
		DWORD result = ::GetFileAttributesW(wPluginDepFolder);
		if ((result == INVALID_FILE_ATTRIBUTES) || ((result & FILE_ATTRIBUTE_DIRECTORY) == false))
			MX_SETERROR(MX1389, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "%s contains invalid folder: %s", pathfilenameDep, line);
		else
		{
			DLL_DIRECTORY_COOKIE cookie = ::AddDllDirectory(wPluginDepFolder);
			if (cookie == 0)
				MX_SETERROR(MX1390, MxError::System, MxError::Abort, MxError::VerboseReport, "unable to add folder %s to DLL search, error=%d", line, (int)GetLastError());
			else
			{
				_cookies.push_back(cookie);
				rc = true;
			}
		}
	}
	if ( rc == false)
		_valid = false;

	return rc;
}

int MxPluginDep::GetFirstFolder(const char *line, std::string *folderlist)
{
	int rc = MxPluginDep::Invalid;

	USES_CONVERSION;

	if ((folderlist == nullptr) || (line == nullptr) )
		_valid= false, MX_SETERROR(MX1398, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "line %s, folderlist %s", (line != nullptr) ? line : "line is null", (folderlist != nullptr) ? folderlist->c_str() : "folderlist is null");
	else
	{
		try
		{
			DWORD  bufCharCount = INFO_BUFFER_SIZE;
			bufCharCount = ::ExpandEnvironmentStrings(A2CW(line), _infoBuf, INFO_BUFFER_SIZE);
			if ((bufCharCount > INFO_BUFFER_SIZE) || (bufCharCount == 0))
				_valid = false, MX_SETERROR(MX1400, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "line %s cannot be expanded, error  %d", (line != nullptr) ? line : "line is null", (bufCharCount == 0) ? GetLastError() : (int)bufCharCount);
			else
			{
				*folderlist = W2CA(_infoBuf);
				rc = 0;
			}
		}
		catch (std::exception const& e)
		{
			MX_SETERROR(MX1380, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "Unexpected exception: %s", e.what());
			_valid = false;
		}
	}
	return rc;
}

int	  MxPluginDep::GetNextFolder(const char *folderlist, int start)
{
	int rc = MxPluginDep::Invalid;

	if ((folderlist == nullptr) || (start == MxPluginDep::Invalid) || (start < 0))
		_valid = false, MX_SETERROR(MX1399, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "start=%d for folderlist %s", start, (folderlist != nullptr) ? folderlist : "folderlist is null");
	else
	{
		int len = strlen(folderlist);
		if (start < len)
		{
			std::string list(folderlist);
			int end = list.find(';', start);
			if (end == std::string::npos)
				rc = len;
			else
				rc = end;
		}
	}
	return rc;
}
#include "StdAfx.h"

#include "MxPluginLibs.h"
#include "DefSocketImageProcXGUI.h"

const char *    const MxPluginLibs::FnNameCreateClassInstance = "CreateClassInstance";
const wchar_t * const MxPluginLibs::ErrorCodeList = L"30000=UnknownError, 30001=CreateMxPluginLibError, 30002=AddMxPluginLibError, 30003=ExceptionError, 30004=InvalidParams, 30005=NoMxPluginsForIDD, 30006=InvalidRegKey";
const wchar_t * const MxPluginLibs::ErrorCodeListMicrosoft = L"1=Invalid Function, 2=File not found, 3=Path not found\n5=Access denied";

MxPluginLibs::MxPluginLibs(void)
{
}


MxPluginLibs::~MxPluginLibs(void)
{
	ClearAllMxPlugins();
}

DWORD MxPluginLibs::RefreshPluginsInSubFolders(const wchar_t *PluginRootFolder, const char *IID)
{
	DWORD ErrorCode = MxPluginLibs::UnknownError;

	ClearAllSelections();

	HANDLE hFindFolder = INVALID_HANDLE_VALUE;

	if ((PluginRootFolder == '\0') || (wcsnlen(PluginRootFolder, MAX_PATH) == 0) || (IID == '\0')  || (strnlen(IID, IMxPluginBaseIID::IIdSize) != IMxPluginBaseIID::IIdSize))
		ErrorCode = MxPluginLibs::InvalidParams;
	else
	{
		try
		{
			std::wstring PluginFolder(L"");
			for (ErrorCode = GetFirstPluginFolderItem(&PluginFolder, PluginRootFolder, &hFindFolder, true); ErrorCode == MxPluginLibs::NoError; ErrorCode = GetNextPluginFolderItem(&PluginFolder, PluginRootFolder, &hFindFolder, true))
			{
				ErrorCode = RefreshPlugins(PluginFolder.c_str(), IID);
				if ((ErrorCode != ERROR_FILE_NOT_FOUND) && (ErrorCode != MxPluginLibs::NoError))
					break;
			}
			if ((ErrorCode == ERROR_NO_MORE_FILES) || (ErrorCode == ERROR_FILE_NOT_FOUND))
				ErrorCode = MxPluginLibs::NoError;
		}
		catch(...)
		{
			ErrorCode = MxPluginLibs::ExceptionError;
		}

		if ( hFindFolder != INVALID_HANDLE_VALUE )
			::FindClose(hFindFolder);
		hFindFolder = INVALID_HANDLE_VALUE;

		if ( ErrorCode == MxPluginLibs::NoError)
		{
			while (DeleteNextUnselected() == true);
			if (GetPluginCount() == 0)
				ErrorCode = MxPluginLibs::NoMxPluginsForIDD;
		}
		ClearAllSelections();
	}
	return ErrorCode;
}

DWORD MxPluginLibs::RefreshPluginsInFolder(const wchar_t *PluginFolder, const char *IID)
{
	DWORD ErrorCode = MxPluginLibs::UnknownError;

	ClearAllSelections();

	if ((PluginFolder == '\0') || (wcsnlen(PluginFolder, MAX_PATH) == 0) || (IID == '\0')  || (strnlen(IID, IMxPluginBaseIID::IIdSize) != IMxPluginBaseIID::IIdSize))
		ErrorCode = MxPluginLibs::InvalidParams;
	else
	{
		if ((ErrorCode = RefreshPlugins(PluginFolder, IID)) == MxPluginLibs::NoError)
		{
			while (DeleteNextUnselected() == true);
			if (GetPluginCount() == 0)
				ErrorCode = MxPluginLibs::NoMxPluginsForIDD;
			ClearAllSelections();
		}
	}
	return ErrorCode;
}


MxPluginLib *MxPluginLibs::GetPlugin(const char *LibID)
{
	MxPluginLib *rc = '\0';

	if (LibID != '\0')
	{
		size_t cnt = _Libs.size();
		if (cnt > 0 )
		{
			for (size_t x = 0; x < cnt; x++)
			{
				if (strncmp(_Libs[x]->GetLibID(), LibID, IMxPluginBaseIID::IIdSize) == 0)
				{
					rc = _Libs[x];
					break;
				}
			}
		}
	}
	return rc;
}

DWORD MxPluginLibs::RefreshPlugins(const wchar_t *PluginFolder, const char *IID)
{
	DWORD ErrorCode = MxPluginLibs::UnknownError;

	HANDLE hFindFile = INVALID_HANDLE_VALUE;

	try
	{
		std::wstring DllFile(L"");
		for (ErrorCode = GetFirstPluginFolderItem(&DllFile, PluginFolder, &hFindFile, false); ErrorCode == MxPluginLibs::NoError; ErrorCode = GetNextPluginFolderItem(&DllFile, PluginFolder, &hFindFile, false))
		{
			HINSTANCE hInst = NULL;
			if ((hInst = ::LoadLibrary(DllFile.c_str())) == NULL)
			{
				ErrorCode = GetLastError();
				break;
			}
			else
			{
				void * (MXSTDMETHOD *pfn)(const char *);
				*(FARPROC *)&pfn = ::GetProcAddress(hInst, MxPluginLibs::FnNameCreateClassInstance);
				if (pfn != '\0')
				{
					IMxPluginLibDetails *tmp = static_cast<IMxPluginLibDetails *>(pfn(IID));
					if (tmp != '\0')
					{
						MxPluginLib *lib = GetPlugin(tmp->GetLibID());
						if ((lib == '\0') /* || (strncmp(tmp->GetSocketAppID(), ISocketImageProcXGUIID::SID(), IMxPluginBaseIID::IIdSize) != 0 )*/ )	//consider the option of only calling AddMxPlugin(lib) if the plugin was developed for this app
							tmp->DestroyPtr();		//MUST invoke tmp->DestroyPtr() before any call to ::FreeLibrary(hInst);
						else
						{											
							tmp->DestroyPtr();		//MUST invoke tmp->DestroyPtr() before any call to ::FreeLibrary(hInst);
							if (lib != '\0')
								lib->SetSelection(true);
							::FreeLibrary(hInst);		
							hInst = NULL;
						}
						if (hInst != NULL)
						{
							if ((lib = new MxPluginLib(DllFile.c_str(), hInst, pfn)) == '\0')
							{
								ErrorCode = MxPluginLibs::CreateMxPluginLibError;
								::FreeLibrary(hInst);
								hInst = NULL;
								break;
							}
							else
							{
								lib->SetSelection(true);
								if ((lib->IsValid() == false) || (AddMxPlugin(lib) == false))
								{
									ErrorCode = MxPluginLibs::AddMxPluginLibError;
									delete lib;
									::FreeLibrary(hInst);
									hInst = NULL;
									break;
								}
							}
						}
					}
				}
			}
		}
		if ( hFindFile != INVALID_HANDLE_VALUE)
		{
			::FindClose(hFindFile);
			hFindFile = INVALID_HANDLE_VALUE;
		}
	}
	catch(...)
	{
		ErrorCode = MxPluginLibs::ExceptionError;
	}
	if (ErrorCode == ERROR_NO_MORE_FILES)
		ErrorCode = MxPluginLibs::NoError;

	if ( hFindFile != INVALID_HANDLE_VALUE)
		::FindClose(hFindFile);
	hFindFile = INVALID_HANDLE_VALUE;

	return ErrorCode;
}


bool MxPluginLibs::AddMxPlugin(MxPluginLib *lib)
{
	bool rc = false;

	if ((lib != '\0') && (lib->IsValid()))
	{
		if (IsExisting(lib) == false)
		{
			_Libs.push_back(lib);
			rc = true;
		}
	}
	return rc;
}

bool MxPluginLibs::IsExisting(MxPluginLib *lib)
{
	bool rc = false;
	if (lib != '\0')
		rc = (GetPlugin(lib->GetLibID()) != '\0') ? true : false;
	return rc;
}

void MxPluginLibs::ClearAllSelections()
{
	size_t cnt = _Libs.size();
	if (cnt > 0 )
	{
		for (size_t x = 0; x < cnt; x++)
			_Libs[x]->SetSelection(false);
	}
	return;
}

bool MxPluginLibs::DeleteNextUnselected()
{
	bool erased = false;

	size_t cnt = _Libs.size();
	if (cnt > 0 )
	{
		for (size_t x = 0; x < cnt; x++)
		{
			if (_Libs[x]->GetSelection() == false)
			{
				delete  _Libs[x];
				_Libs[x] = '\0';
				_Libs.erase(_Libs.begin()+x);
				erased = true;
				break;
			}
		}
	}
	return erased;
}

void MxPluginLibs::ClearAllMxPlugins()
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

DWORD MxPluginLibs::GetFirstPluginFolderItem(std::wstring *firstFolderResult, const wchar_t *PluginFolder, HANDLE *hFindFolder, bool FindFolder)
{
	DWORD rc = MxPluginLibs::UnknownError;

	if ((firstFolderResult != '\0') && (PluginFolder != '\0') && (hFindFolder != '\0'))
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
			WIN32_FIND_DATA ffd;
			*hFindFolder = ::FindFirstFile(match.c_str(), &ffd);
			if (INVALID_HANDLE_VALUE == *hFindFolder)
				rc = GetLastError();
			else
			{
				bool ItemFound = false;
				do
				{
					if (FindFolder == true)
					{
						if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (wcsncmp(ffd.cFileName, L".", 1) != 0) && (wcsncmp(ffd.cFileName, L"..", 2) != 0))
						{
							*firstFolderResult = path;
							*firstFolderResult += ffd.cFileName;
							*firstFolderResult += L"\\";
							ItemFound = true;
							break;
						}
					}
					else
					{
						if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == false)
						{
							*firstFolderResult = path;
							*firstFolderResult += ffd.cFileName;
							ItemFound = true;
							break;
						}
					}
				}
				while (::FindNextFile(*hFindFolder, &ffd) != 0);
				if ( ItemFound == false )
					rc = GetLastError();
				else
					rc = MxPluginLibs::NoError;
			}
		}
		catch(...)
		{
			rc = GetLastError();
		}
		if ((*hFindFolder != INVALID_HANDLE_VALUE) && (rc != MxPluginLibs::NoError))
		{
			::FindClose(*hFindFolder);
			*hFindFolder = INVALID_HANDLE_VALUE;
		}
		if ( rc != MxPluginLibs::NoError )
			*firstFolderResult = L"";
	}
	return rc;
}

DWORD MxPluginLibs::GetNextPluginFolderItem(std::wstring *nextFolderResult, const wchar_t *PluginFolder, HANDLE *hFindFolder, bool FindFolder)
{
	DWORD rc = MxPluginLibs::UnknownError;

	if ((nextFolderResult != '\0') && (hFindFolder != '\0') && (*hFindFolder != INVALID_HANDLE_VALUE))
	{
		std::wstring path(PluginFolder);
		int endPathPos = path.find_last_of('\\');
		if (endPathPos != path.length()-1)
			path += L"\\";

		try
		{
			bool ItemFound = false;
			WIN32_FIND_DATA ffd;

			while (::FindNextFile(*hFindFolder, &ffd) != 0)
			{
				if (FindFolder == true)
				{
					if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						*nextFolderResult = path;
						*nextFolderResult += ffd.cFileName;
						ItemFound = true;
						break;
					}
				}
				else
				{
					if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == false)
					{
						*nextFolderResult = path;
						*nextFolderResult += ffd.cFileName;
						ItemFound = true;
						break;
					}
				}
			}
			if ( ItemFound == false )
				rc = GetLastError();
			else
				rc = MxPluginLibs::NoError;
		}
		catch(...)
		{
			rc = GetLastError();
		}
		if ((*hFindFolder != INVALID_HANDLE_VALUE) && (rc != MxPluginLibs::NoError))
		{
			::FindClose(*hFindFolder);
			*hFindFolder = INVALID_HANDLE_VALUE;
		}
		if ( rc != MxPluginLibs::NoError )
			*nextFolderResult = L"";
	}
	return rc;
}


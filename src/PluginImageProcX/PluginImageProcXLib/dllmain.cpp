// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include <string>
#include <sstream>
#include <atlconv.h>
#include <iomanip>

#include "gLibDetails.h"

static std::string gName = "[invalidName]";						//VS_VERSION_INFO::ProductName
static std::string gDescription= "[invalidDescription]";		//VS_VERSION_INFO::FileDescription
static std::string gVersion = "[invalidVersion]";				//VS_VERSION_INFO::ProductVersion
static std::string gOwnerName= "[invalidOwner]";				//VS_VERSION_INFO::CompanyName
static std::string gCopyrightNotice= "[invalidCopyright]";		//VS_VERSION_INFO::LegalCopyright
static std::string gID= "[invalidID]";							//VS_VERSION_INFO::InternalName GUID - same as its folder name in PluginRootFolder
static std::wstring gwPathFilename= L"[invalidPathFilename]";	//filename returned by ::GetModuleFileName() of hModule in DllMain, DLL_PROCESS_ATTACH	

static std::string gTmp;

const char *gLibName() 
{
    return gName.c_str(); 
}

const char *gLibDescription() 
{
    return gDescription.c_str(); 
}

const char *gLibVersion() 
{
    return gVersion.c_str(); 
} 

const char *gLibOwnerName() 
{
    return gOwnerName.c_str(); 
}

const char *gLibCopyrightNotice() 
{
    return gCopyrightNotice.c_str(); 
}

const char *gLibID() 
{
    return gID.c_str(); 
}

const wchar_t *gLibPathFilename()
{
    return gwPathFilename.c_str(); 
}

const char *GetVersionInfoString(LPTSTR pDataBlock, WORD* langInfo, LPTSTR Label, std::string& result);

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			if (hModule != NULL)
			{
				WCHAR filename[MAX_PATH+1];
				DWORD length = 0;
				if ((length = ::GetModuleFileNameW(hModule, filename, MAX_PATH)) > 0 )
				{
					gwPathFilename = filename;
					DWORD dwSize = 0;
					DWORD dwHandle = 0;
					if ((dwSize = ::GetFileVersionInfoSizeW(filename, &dwHandle)) > 0 )
					{
						LPTSTR pDataBlock = '\0';
						if ((pDataBlock = (LPTSTR) ::GlobalAlloc(GPTR, dwSize)) != '\0')    
						{
							DWORD res = 0;
							if ( ::GetFileVersionInfoW(filename, res, dwSize, pDataBlock ) == TRUE)
							{
								DWORD dwFileInfoSize = 0;
								VS_FIXEDFILEINFO* lpFileInfo = '\0';
								if (::VerQueryValue(pDataBlock, TEXT("\\"), (LPVOID *) &lpFileInfo, (UINT *) &dwFileInfoSize))
								{
									DWORD verMajor = 0;
									verMajor = lpFileInfo->dwProductVersionMS;
									DWORD verMinor = 0;
									verMinor = lpFileInfo->dwProductVersionLS;

									std::ostringstream convert; 
									convert << ( verMajor >> 16) << "." << (verMajor & 0xFFFF) << "." << ( verMinor >> 16) << "." << (verMinor & 0xFFFF) ;
									gVersion = convert.str();

									WORD* langInfo = '\0';
									UINT cbLang = 0;
									if (::VerQueryValue(pDataBlock, TEXT("\\VarFileInfo\\Translation"), (LPVOID*)&langInfo, &cbLang) == TRUE)
									{
										GetVersionInfoString(pDataBlock, langInfo, TEXT("ProductName"), gName);
										GetVersionInfoString(pDataBlock, langInfo, TEXT("FileDescription"), gDescription);
										GetVersionInfoString(pDataBlock, langInfo, TEXT("CompanyName"), gOwnerName);
										GetVersionInfoString(pDataBlock, langInfo, TEXT("LegalCopyright"), gCopyrightNotice);
										GetVersionInfoString(pDataBlock, langInfo, TEXT("InternalName"), gID);
									}
								}
							}
							::GlobalFree((HGLOBAL)pDataBlock);
						}
					}
				}
			}
		}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


const char *GetVersionInfoString(LPTSTR pDataBlock, WORD* langInfo, LPTSTR Label, std::string& result)
{
	const char *rc = "[invalid]";

	if ((pDataBlock != '\0') && (langInfo != '\0') && (Label != '\0'))
	{
		USES_CONVERSION;

		std::string name(T2A(Label));
		std::ostringstream convert; 
		convert << "\\StringFileInfo\\" << std::hex << std::setw(4) << std::setfill('0')  << langInfo[0] << std::setw(4) << langInfo[1] << "\\" << name;

		UINT cbBufSize = 0;
		LPVOID lpt = '\0';
		if (::VerQueryValue(pDataBlock, A2CT(convert.str().c_str()), &lpt, &cbBufSize) == TRUE) 
		{
			result = (T2A((LPTSTR)lpt));
			rc = result.c_str();
		} 
	}
	return rc;
}


#include "stdafx.h"
#include "MxUtils\MxVersionInfo1.h"

#include <sstream>
#include <atlconv.h>
#include <iomanip>

MxVersionInfo::MxVersionInfo(void) :
_ProductName("[invalid]"),
_Description("[invalid]"),
_Version("[invalid]"),
_OwnerName("[invalid]"),
_CopyrightNotice("[invalid]"),
_ProductID("[invalid]"),
_wPathFilename(L"[invalid]")
{
	SetVersionInfo();
}


MxVersionInfo::~MxVersionInfo(void)
{
}


bool MxVersionInfo::SetVersionInfo( HMODULE hModule)
{
	bool rc = false;

	WCHAR filename[MAX_PATH+1];
	DWORD length = 0;
	if ((length = ::GetModuleFileNameW(hModule, filename, MAX_PATH)) > 0 )
	{
		_wPathFilename = filename;
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
						_Version = convert.str();

						WORD* langInfo = '\0';
						UINT cbLang = 0;
						if (::VerQueryValue(pDataBlock, TEXT("\\VarFileInfo\\Translation"), (LPVOID*)&langInfo, &cbLang) == TRUE)
						{
							GetVersionInfoString(pDataBlock, langInfo, TEXT("ProductName"), _ProductName);
							GetVersionInfoString(pDataBlock, langInfo, TEXT("FileDescription"),_Description);
							GetVersionInfoString(pDataBlock, langInfo, TEXT("CompanyName"), _OwnerName);
							GetVersionInfoString(pDataBlock, langInfo, TEXT("LegalCopyright"), _CopyrightNotice);
							GetVersionInfoString(pDataBlock, langInfo, TEXT("InternalName"), _ProductID);
						}
					}
				}
				::GlobalFree((HGLOBAL)pDataBlock);
			}
		}
	}
	return rc;
}

const char *MxVersionInfo::GetVersionInfoString(LPTSTR pDataBlock, WORD* langInfo, LPTSTR Label, std::string& result)
{
	const char *rc = "[invalid]";

	if ((pDataBlock != '\0') && (langInfo != '\0') && (Label != '\0'))
	{
		try
		{
			USES_CONVERSION;
			const char *LabelStr = T2A(Label);

			std::string name((LabelStr != nullptr) ? LabelStr : "" );
			std::ostringstream convert; 
			convert << "\\StringFileInfo\\" << std::hex << std::setw(4) << std::setfill('0')  << langInfo[0] << std::setw(4) << langInfo[1] << "\\" << name;

			UINT cbBufSize = 0;
			LPVOID lpt = '\0';
			LPCWSTR ConvertStr = A2CT(convert.str().c_str());

			if (::VerQueryValue(pDataBlock, ((ConvertStr != nullptr) ? ConvertStr : L"" ), &lpt, &cbBufSize) == TRUE) 
			{
				try
				{
					const char *ResultStr = T2A((LPTSTR)lpt);
					result = ((ResultStr != nullptr) ? ResultStr : "" );
					rc = result.c_str();
				}
				catch(...)
				{
				}
			} 
		}
		catch(...)
		{
		}
	}
	return rc;
}
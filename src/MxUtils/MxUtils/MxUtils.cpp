// MxUtils.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <Winperf.h>
#include <iostream>
#include <fstream>
#include <tchar.h>
#include <strsafe.h>
#include <malloc.h>

#pragma warning(disable:4995)
#include "MxUtils\MxUtils1.h"
#include <vector>
#pragma warning(default:4995)
#include "blowfish.h"


MxUtils::MxUtils() :
	_VersionInfo (nullptr)
{
	_VersionInfo = new MxVersionInfo();
	return;
}

MxUtils::~MxUtils()
{
	CleanupOnAppExitOnly();
#ifdef _DEBUG
	::OutputDebugString(L"****************MxUtils::~MxUtils() all allocated memory now freed ********************\n" );
#endif
}

void MxUtils::CleanupOnAppExitOnly()
{
	if ( _VersionInfo != nullptr)
		delete _VersionInfo;
	_VersionInfo = nullptr;
}

bool MxUtils::IsExistingFolderName(const char *foldername, size_t MaxFoldernameLen)
{
	bool rc = false;

	if ((foldername != nullptr) && (strnlen(foldername, MaxFoldernameLen) <= MaxFoldernameLen))
	{
		DWORD result = ::GetFileAttributesA(foldername);
		if (result != INVALID_FILE_ATTRIBUTES)
		{
			if (result & FILE_ATTRIBUTE_DIRECTORY )
				rc = true;
		}
	}
	return rc;
}

bool MxUtils::IsExistingPathFilename(const char *pathfilename, size_t MaxPathFilenameLen)
{
	bool rc = false;

	if ((pathfilename != nullptr) && (strnlen(pathfilename, MaxPathFilenameLen) <= MaxPathFilenameLen))
	{
		DWORD result = ::GetFileAttributesA(pathfilename);
		if (result != INVALID_FILE_ATTRIBUTES)
		{
			if (result & FILE_ATTRIBUTE_DIRECTORY )
				rc = false;
			else
				rc = true;
		}
	}
	return rc;
}

bool MxUtils::IsValidPathFilename(const char *pathfilename, size_t MaxPathFilenameLen)
{
	bool rc = false;

	if (IsExistingPathFilename(pathfilename, MaxPathFilenameLen ) == true)
		rc = true;
	else
	{
		if ((pathfilename != nullptr) && (strnlen(pathfilename, MaxPathFilenameLen) <= MaxPathFilenameLen))
		{
			BOOL writeOk = FALSE;
			HANDLE hFile = INVALID_HANDLE_VALUE;
			if ( (hFile = ::CreateFileA(pathfilename, (GENERIC_READ | GENERIC_WRITE),  0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE)
			{
				DWORD len = 0;
				char buff[] = "hello world!";
				if (::WriteFile(hFile, buff, sizeof(buff)-1, &len, NULL) == TRUE)
				{
					if (len == sizeof(buff)-1)
						writeOk = TRUE;
				}
				::CloseHandle(hFile);
				if ((::DeleteFileA(pathfilename) == FALSE) )
					writeOk = FALSE;
			 }
			 if (writeOk == TRUE)
				 rc = true;
		}
	}
	return rc;
}

bool MxUtils::RunConsoleApp(const wchar_t *CmdLine, std::wstring *StdOutText, int StdOutTextLenMax, int WaitForAppToStart)
{
	bool rc = false;

	if (CmdLine == nullptr)
		MX_SETERROR(MX1024, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "CmdLine is null"); 
	else
	{
		std::wstring CmdLineStr = CmdLine;
		HANDLE hChildStd_IN_Rd = NULL;
		HANDLE hChildStd_OUT_Rd = NULL;
		HANDLE hChildStd_OUT_Wr = NULL;
		HANDLE hChildStd_IN_Wr = NULL;

		SECURITY_ATTRIBUTES saAttr; 
 
		saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
		saAttr.bInheritHandle = TRUE; 
		saAttr.lpSecurityDescriptor = NULL; 

		try
		{
			if ( CreatePipe(&hChildStd_OUT_Rd, &hChildStd_OUT_Wr, &saAttr, 0) == FALSE) 		// Create a pipe for the child process's STDOUT.
				MX_SETERROR(MX1025, MxError::System, MxError::Abort, MxError::QuietReport, "CreatePipe - STDOUT failed GetLastError=%d", GetLastError()); 
			else	
			{
				if ( SetHandleInformation(hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0) == FALSE)	// Ensure the read handle to the pipe for STDOUT is not inherited.
					MX_SETERROR(MX1026, MxError::System, MxError::Abort, MxError::QuietReport, "SetHandleInformation- STDOUT Read failed GetLastError=%d", GetLastError());
				else
				{
					if (CreatePipe(&hChildStd_IN_Rd, &hChildStd_IN_Wr, &saAttr, 0) == FALSE) 
						MX_SETERROR(MX1027, MxError::System, MxError::Abort, MxError::QuietReport, "CreatePipe - STDIN failed GetLastError=%d", GetLastError()); 
					else
					{
						if ( SetHandleInformation(hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0) == FALSE)
							MX_SETERROR(MX1028, MxError::System, MxError::Abort, MxError::QuietReport, "SetHandleInformation- STDIN Write failed GetLastError=%d", GetLastError());
						else
						{
							USES_CONVERSION;

							std::vector<wchar_t> buf(CmdLineStr.begin(), CmdLineStr.end());
							buf.push_back(0);	//add terminating null
							PROCESS_INFORMATION piProcInfo; 
							STARTUPINFO siStartInfo;
 
							ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );
 							ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );

							siStartInfo.cb = sizeof(STARTUPINFO); 
							siStartInfo.hStdError = hChildStd_OUT_Wr;
							siStartInfo.hStdOutput = hChildStd_OUT_Wr;
							siStartInfo.hStdInput = hChildStd_IN_Rd;
							siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
							siStartInfo.dwFlags |= STARTF_USESHOWWINDOW;
							siStartInfo.wShowWindow = SW_HIDE;
     
							BOOL bSuccess = CreateProcess(NULL, 
							  buf.data(),    // command line 
							  NULL,          // process security attributes 
							  NULL,          // primary thread security attributes 
							  TRUE,          // handles are inherited 
							  0,             // creation flags 
							  NULL,          // use parent's environment 
							  NULL,          // use parent's current directory 
							  &siStartInfo,  // STARTUPINFO pointer 
							  &piProcInfo);  // receives PROCESS_INFORMATION 
   
						   // If an error occurs, exit the application. 
							if ( bSuccess == FALSE ) 
								MX_SETERROR(MX1029, MxError::System, MxError::Abort, MxError::VerboseReport, "CreateProcess failed GetLastError=%d, cmd line=%s", GetLastError(), W2CA(CmdLineStr.c_str()));
							else	
							{
								WaitForSingleObject( piProcInfo.hProcess, WaitForAppToStart );
 
								CloseHandle(piProcInfo.hProcess);
								CloseHandle(piProcInfo.hThread);
								CloseHandle( hChildStd_IN_Wr);		//close STDIN write so child app stops reading

								DWORD dwRead = 0; 
								CHAR chBuf[MxUtils::FileBuffSize+2]; 
								std::wstring tmp;

								if ( StdOutText != nullptr)
									StdOutText->assign(L"");
								int StdOutCharCnt = 0;
								do 
								{ 
									dwRead = 0;
									if (PeekNamedPipe(hChildStd_OUT_Rd, NULL, 0, NULL,  &dwRead, NULL) == FALSE)    // error, the child process might ended
										break;

									if (dwRead == 0)    
										bSuccess = TRUE;
									else
									{
										if ((bSuccess = ReadFile( hChildStd_OUT_Rd, chBuf, MxUtils::FileBuffSize, &dwRead, NULL)) == TRUE)
										{
											chBuf[dwRead] = '\0';
											StdOutCharCnt += dwRead;
											if ((StdOutText != nullptr) && (StdOutCharCnt < StdOutTextLenMax))
												StdOutText->append(MxUtils::ConvertString2WString(&tmp, chBuf));
										}
									}
								} 
								while (dwRead > 0);
								if (bSuccess == TRUE)
									rc = true;
							}
						}
						if ( CloseHandle( hChildStd_IN_Rd) == FALSE)  
							rc = false, MX_SETERROR(MX1030, MxError::System, MxError::Abort, MxError::QuietReport, "CloseHandle STDIN failed GetLastError=%d", GetLastError());
					}
				}
				if ( (CloseHandle( hChildStd_OUT_Wr) == FALSE) || (CloseHandle( hChildStd_OUT_Rd) == FALSE) ) 
					rc = false, MX_SETERROR(MX1031, MxError::System, MxError::Abort, MxError::QuietReport, "CloseHandle STDOUT failed GetLastError=%d", GetLastError());
			}
		}
		catch(std::exception const& e)
		{
			MX_SETERROR(MX1032, MxError::System, MxError::Abort, MxError::QuietReport, "unexpected exceptiob GetLastError=%d, msg=%s", GetLastError(), e.what());
			rc = false;
		}
	}
	return rc;
}

int  MxUtils::GetLineCnt(const wchar_t *StdOutText)
{
	int rc = MxUtils::Invalid;

	if (StdOutText == nullptr) 
		MX_SETERROR(MX1034, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "StdOutText == nullptr");
	else
	{
		std::wstring text(StdOutText);
		rc = 0;
		int begin = 0;

		while ( (begin = text.find(L"\r\n", begin)) != std::string::npos)
			begin++, rc++;
	}
	return rc;
}

bool MxUtils::GetLineText(const wchar_t *StdOutText, int LineNo, std::wstring *LineText)
{	
	bool rc = false;

	if ((StdOutText == nullptr) || (LineNo < 0) || (LineText == nullptr))
		MX_SETERROR(MX1033, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "(StdOutText == nullptr) || (LineNo < 0) || (LineText == nullptr)");
	else
	{
		std::wstring text(StdOutText);
		int cnt = 0;
		int begin = 0;
		int end = 0;

		while ( (end = text.find(L"\r\n", begin)) != std::string::npos)
		{

			if (cnt++ != LineNo)
				begin = end + 2;
			else
			{
				LineText->assign(text.substr(begin, end-begin));
				rc = true;
				break;
			}
		}
		if (( end ==  std::string::npos) && (cnt == LineNo))
		{
			LineText->assign(text.substr(begin));
			rc = true;
		}
	}
	return rc;
}

bool MxUtils::GetText(const wchar_t *StdOutText, const wchar_t *LineStartsWithText, std::wstring *RestOfLineText)
{
	bool rc = false;

	if ((StdOutText == nullptr) || (LineStartsWithText == nullptr) || (RestOfLineText == nullptr))
		MX_SETERROR(MX1034, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "(StdOutText == nullptr) || (LineStartsWithText == nullptr 0) || (RestOfLineText == nullptr)");
	else
	{
		RestOfLineText->assign(L"");
		std::wstring text(StdOutText);
		std::wstring line(L"");
		std::wstring prefix(LineStartsWithText);
		int PrefixLen = prefix.length();
		int cnt = 0;
		int begin = 0;
		int end = std::string::npos;

		while ( (end = text.find(L"\r\n", begin)) != std::string::npos)
		{
			line = text.substr(begin, end-begin);
			if ( line.find(prefix.c_str()) == std::string::npos)
				begin = end + 2;
			else
			{
				RestOfLineText->assign(line.substr(PrefixLen));
				rc = true;
				break;
			}
		}
		if (rc == false)
		{
			line = text.substr(begin);
			if ( (end = line.find(prefix.c_str())) != std::string::npos)
			{
				RestOfLineText->assign(line.substr(PrefixLen));
				rc = true;
			}
		}
	}
	return rc;
}

const char *MxUtils::ConvertWString2String(std::string *strDest, const wchar_t *strSrc)
{
	const char * rc = nullptr;

	if ((strSrc == nullptr) || (strDest == nullptr))
		MX_SETERROR(MX1035, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "(strSrc == nullptr) || (strDest == nullptr)");
	else
	{
		try
		{
			USES_CONVERSION;

			strDest->assign(W2CA(strSrc));
			rc = strDest->c_str();
		}
		catch(std::exception const& e)
		{
			MX_SETERROR(MX1036, MxError::System, MxError::Abort, MxError::QuietReport, "unexpected exception - %s", e.what());
			rc = nullptr;
		}
	}
	return rc;
}


const wchar_t *MxUtils::ConvertString2WString(std::wstring *strDest, const char *strSrc)
{
	const wchar_t * rc = nullptr;

	if ((strSrc == nullptr) || (strDest == nullptr))
		MX_SETERROR(MX1037, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "(strSrc == nullptr) || (strDest == nullptr)");
	else
	{
		try
		{
			USES_CONVERSION;

			strDest->assign(A2CW(strSrc));
			rc = strDest->c_str();
		}
		catch(std::exception const& e)
		{
			MX_SETERROR(MX1038, MxError::System, MxError::Abort, MxError::QuietReport, "unexpected exception - %s", e.what());
			rc = nullptr;
		}
	}
	return rc;
}

bool  MxUtils::CreateRegAppRootKey(HKEY hKey, const wchar_t *companyKeyName, const wchar_t *appKeyName, HKEY *hAppRootKey)
{
	bool rc = false;

	if ( (hKey == NULL) || (companyKeyName == nullptr) || (appKeyName == nullptr) || (hAppRootKey == nullptr))
		MX_SETERROR(MX1008, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "hKey or companyKeyName or appName or hAppRootKey is null"); 
	else
	{
		TCHAR szKey[MAX_PATH*2];
		StringCchCopy (szKey, MAX_PATH*2, TEXT("Software\\"));
		StringCchCat (szKey, MAX_PATH*2, companyKeyName);
		StringCchCat(szKey, MAX_PATH*2,TEXT("\\"));
		StringCchCat(szKey, MAX_PATH*2, appKeyName);
		StringCchCat(szKey, MAX_PATH*2,TEXT("\\"));

		try
		{
			USES_CONVERSION;
			LONG lResult  = ERROR_INVALID_FUNCTION;
			lResult = RegCreateKeyEx(hKey, szKey, NULL, NULL, REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL, hAppRootKey, NULL);

			if (lResult != ERROR_SUCCESS) 
				MX_SETERROR ( MX1009, MxError::System, MxError::Abort, MxError::QuietReport, "RegCreateKeyEx failed, ErrorNo %d for %s", lResult, W2CA(szKey));
			else
			{
				rc = true;			
			}
		}
		catch(...)
		{
		}
	}
	return rc;
}

bool  MxUtils::CreateRegAppSubKey(HKEY hParentKey, const wchar_t *subKeyNameBackSlash, HKEY *hSubKey)
{
	bool rc = false;

	if ( (hParentKey == NULL) || (subKeyNameBackSlash == nullptr) || (hSubKey == nullptr))
		MX_SETERROR(MX1010, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "hParentKey or subKeyNameBackSlash or hSubKey is null"); 
	else
	{
		try
		{
			USES_CONVERSION;
			TCHAR szKey[MAX_PATH*2];
			StringCchCopy (szKey, MAX_PATH*2, subKeyNameBackSlash);

			LONG lResult  = ERROR_INVALID_FUNCTION;
			lResult = RegCreateKeyEx(hParentKey, szKey, NULL, NULL, REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL, hSubKey, NULL);
			if (lResult != ERROR_SUCCESS) 
				MX_SETERROR ( MX1011, MxError::System, MxError::Abort, MxError::QuietReport, "RegCreateKeyEx failed, ErrorNo %d for %s", lResult, W2CA(szKey));
			else
			{
				rc = true;			
			}
		}
		catch(...)
		{
		}
	}
	return rc;
}

bool MxUtils::IsPresentRegSubKey(HKEY hRootKey, const wchar_t *subKeyNameBackSlash)
{
	bool rc = false;

	if ( (hRootKey == NULL) || (subKeyNameBackSlash == nullptr) )
		MX_SETERROR(MX1010, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "hParentKey or subKeyNameBackSlash  is null"); 
	else
	{
		try
		{
			USES_CONVERSION;
			TCHAR szKey[MAX_PATH*2];
			StringCchCopy (szKey, MAX_PATH*2, subKeyNameBackSlash);

			LONG lResult  = ERROR_INVALID_FUNCTION;

			HKEY hKey = NULL;
			lResult = RegOpenKeyEx (hRootKey, szKey, 0, KEY_READ, &hKey);
			if (lResult == ERROR_SUCCESS)
			{
				lResult = RegCloseKey(hKey);
				if (lResult != ERROR_SUCCESS)
					MX_SETERROR ( MX1022, MxError::System, MxError::Abort, MxError::QuietReport, "RegCloseKey failed, ErrorNo %d for %s", lResult, W2CA(szKey));
				else					
					rc = true;		//found
			}
			else if (lResult != ERROR_FILE_NOT_FOUND)
				MX_SETERROR ( MX1022, MxError::System, MxError::Abort, MxError::QuietReport, "RegOpenKeyEx failed,  ErrorNo %d for %s", lResult, W2CA(szKey));
			else
			{
				rc = false;		//not found		
			}
		}
		catch(...)
		{
		}
	}
	return rc;
}

bool  MxUtils::SetRegSubKeyInt(HKEY SubKey, const wchar_t *name, int value)
{
	bool rc = false;

	if ( SubKey == NULL )
		MX_SETERROR(MX1012, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "SubKey == NULL"); 
	else
	{
		try
		{
			USES_CONVERSION;
			LONG lResult  = ERROR_INVALID_FUNCTION;
			lResult = RegSetValueEx(SubKey, name, NULL, REG_DWORD, (LPBYTE)&value, sizeof(DWORD));
			if (lResult != ERROR_SUCCESS) 
				MX_SETERROR ( MX1013, MxError::System, MxError::Abort, MxError::QuietReport, "RegSetValueEx failed, ErrorNo %d for %s=%d", lResult, W2CA(name), value);
			else
			{
				rc = true;
			}
		}
		catch(...)
		{
		}
	}
	return rc;
}

bool  MxUtils::GetRegSubKeyInt(HKEY SubKey, const wchar_t *name, int *value)
{
	bool rc = false;

	if ( (SubKey == NULL) || (value == nullptr))
		MX_SETERROR(MX1014, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "(SubKey == NULL) || (value == nullptr)"); 
	else
	{
		try
		{
			USES_CONVERSION;
			DWORD data = 0;		
			DWORD size = sizeof(data);	
			DWORD type = REG_DWORD;
			LONG lResult  = ERROR_INVALID_FUNCTION;

			lResult = RegQueryValueEx(SubKey, name, NULL, &type, (LPBYTE)&data, &size);
			if (lResult != ERROR_SUCCESS) 
				MX_SETERROR ( MX1015, MxError::System, MxError::Abort, MxError::QuietReport, "RegQueryValueEx failed, ErrorNo %d-%s for %s", lResult, ((lResult == ERROR_FILE_NOT_FOUND) ? "Not found" : "(other error)"), W2CA(name));
			else
			{
				*value = data;
				rc = true;
			}
		}
		catch(...)
		{
		}
	}
	return rc;
}

bool  MxUtils::SetRegSubKeyStr(HKEY SubKey, const wchar_t *name, const wchar_t *value)
{
	bool rc = false;

	if ( (SubKey == NULL) || (value == nullptr) )
		MX_SETERROR(MX1016, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "(SubKey == NULL) || (value == nullptr)"); 
	else
	{
		int valueLen = wcsnlen(value, MxUtils::RegStringMaxLen);
		if ((valueLen == 0) || (valueLen == MxUtils::RegStringMaxLen))
			MX_SETERROR(MX1020, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "length of value (%d) is zero or larger than MxUtils::RegStringMaxLen (%d)", valueLen, MxUtils::RegStringMaxLen); 
		else
		{
			try
			{
				USES_CONVERSION;

				LONG lResult  = ERROR_INVALID_FUNCTION;
				DWORD size = ((valueLen+1) * sizeof(wchar_t));	
				lResult = RegSetValueEx(SubKey, name, NULL, REG_SZ, (LPBYTE)value, size);
				if (lResult != ERROR_SUCCESS) 
					MX_SETERROR ( MX1017, MxError::System, MxError::Abort, MxError::QuietReport, "RegQueryValueEx failed, ErrorNo %d-%s for %s=%s", lResult, ((lResult == ERROR_FILE_NOT_FOUND) ? "Not found" : "(other error)"), W2CA(name), value);
				else
				{
					rc = true;
				}
			}
			catch(...)
			{
			}
		}
	}
	return rc;
}

bool  MxUtils::GetRegSubKeyStr(HKEY SubKey, const wchar_t *name, std::wstring *value)
{
	bool rc = false;

	if ( (SubKey == NULL) || (value == nullptr))
		MX_SETERROR(MX1018, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "(SubKey == NULL) || (name == nullptr) || (value == nullptr)"); 
	else
	{
		try
		{
			USES_CONVERSION;

			PPERF_DATA_BLOCK PerfData = (PPERF_DATA_BLOCK) malloc( MxUtils::RegStringMaxLen * sizeof(wchar_t));
			DWORD size = MxUtils::RegStringMaxLen * sizeof(wchar_t);	
			DWORD type = REG_SZ;
			LONG lResult  = ERROR_INVALID_FUNCTION;

			lResult = RegQueryValueEx(SubKey, name, NULL, &type, (LPBYTE)PerfData, &size);
			if (lResult != ERROR_SUCCESS) 
				MX_SETERROR ( MX1019, MxError::System, MxError::Abort, MxError::QuietReport, "RegQueryValueEx failed, ErrorNo %d-%s for %s", lResult, ((lResult == ERROR_FILE_NOT_FOUND) ? "Not found" : "(other error)"), W2CA(name));
			else
			{
				*value = (const wchar_t *)PerfData;
				rc = true;
			}
			free(PerfData);
		}
		catch(...)
		{
		}
	}
	return rc;
}


bool MxUtils::DelRegAppKey(HKEY hKey, const wchar_t *companyKeyName, const wchar_t *appName, const wchar_t *subKeyNameBackSlash)
{
	bool rc = false;

	if ((hKey == NULL) || (companyKeyName == nullptr) || (appName == nullptr))
	{
		MX_SETERROR(MX1007, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "hKey or companyKeyName or appName is null"); 
		return rc;
	}

	TCHAR szDelKey[MAX_PATH*2];
	StringCchCopy (szDelKey, MAX_PATH*2, TEXT("Software\\"));
	StringCchCat (szDelKey, MAX_PATH*2, companyKeyName);
	StringCchCat(szDelKey, MAX_PATH*2,TEXT("\\"));
	StringCchCat(szDelKey, MAX_PATH*2, appName);
	StringCchCat(szDelKey, MAX_PATH*2,TEXT("\\"));
	if (subKeyNameBackSlash != nullptr)
		StringCchCat(szDelKey, MAX_PATH*2,subKeyNameBackSlash);
			//hKey is HKEY_CURRENT_USER or HKEY_LOCAL_MACHINE
	return MxUtils::DelRegNodeRecurse(hKey, szDelKey);
}

int MxUtils::EncryptString(BYTE *CodeText, LPCWSTR val, const char *Key)
{
	int rc = MxUtils::Invalid; 

	if ((CodeText == nullptr) || (val == nullptr) || (wcslen(val) == 0) || (wcslen(val) > MxUtils::MaxEncryptLength))
		MX_SETERROR(MX1046, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "Invalid params: CodeText=%s, val=%s", (CodeText == nullptr) ? "(null)" : "(not null)", (val == nullptr) ? "(null)" : "(not null)" ); 
	else
	{
		CBlowFish secret;

		int keylen = strlen(Key);
		secret.Initialize((unsigned char *)Key, keylen);		

#pragma warning(disable:4996)

		unsigned long buffsize = 0;
		buffsize = wcstombs( NULL, val, 0);		// C4996
		if (buffsize < 1 )
			MX_SETERROR(MX1047, MxError::CodeDefect, MxError::TerminateApp,  MxError::QuietReport, "buffsize(%u) < 1",  buffsize); 
		else
		{
			if ((rc = secret.GetOutputLength(buffsize + 1)) < 8)
				MX_SETERROR(MX1048, MxError::CodeDefect, MxError::TerminateApp,  MxError::QuietReport, "%d = secret.GetOutputLength(%d + 1)",  rc, buffsize); 
			else
			{
				char *buff = nullptr;
				buff = (char *)calloc(rc,1);
				if (buff == nullptr)
					MX_SETERROR(MX1049, MxError::CodeDefect, MxError::TerminateApp,  MxError::QuietReport, "calloc failed, buff == nullptr"); 
				else
				{
					size_t size = 0;
					size = wcstombs( buff, val, rc); 
					if (size == (size_t) (-1))
					{
						MX_SETERROR(MX1050, MxError::Config, MxError::TerminateApp,  MxError::QuietReport, "Couldn't convert string. Code page 932 may not be available"); 
						rc = MxUtils::Invalid;
 					}
					else
					{
						if ( (secret.Encode ((BYTE *)buff, CodeText, rc) != rc ) || (CodeText == nullptr))
						{
							MX_SETERROR(MX1051, MxError::CodeDefect, MxError::TerminateApp,  MxError::QuietReport, "secret.Encode() failed"); 
							rc = MxUtils::Invalid;
						}
					}
					free (buff);
				}
			}
		}
#pragma warning(default:4996)
	}
	return rc;
}

LPCWSTR MxUtils::DecryptString(std::wstring *val, BYTE *CodeText, int CodeTextLen, const char *Key)
{
	LPCWSTR rc = nullptr;

	if ((CodeText == nullptr) || (val == nullptr) || (CodeTextLen < 8) || (CodeTextLen > MxUtils::MaxDecryptLength))
		MX_SETERROR(MX1052, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "Invalid params: CodeText=%s, val=%s", (CodeText == nullptr) ? "(null)" : "(not null)", (val == nullptr) ? "(null)" : "(not null)" ); 
	else
	{
		*val = L"";			
		CBlowFish secret;

		int keylen = strlen(Key);
		secret.Initialize((unsigned char *)Key, keylen);	

	#pragma warning(disable:4996)

		size_t buffsize = 0;
		buffsize = secret.GetOutputLength(CodeTextLen);
		if (buffsize < 8 )
			MX_SETERROR(MX1053, MxError::CodeDefect, MxError::TerminateApp,  MxError::QuietReport, "buffsize(%u) < 1",  buffsize); 
		else
		{
			unsigned char *PlainText = nullptr;
			PlainText =  (unsigned char *)calloc( buffsize+1, 1);
			if (PlainText == nullptr)
				MX_SETERROR(MX1054, MxError::CodeDefect, MxError::TerminateApp,  MxError::QuietReport, "calloc failed, buff == nullptr"); 
			else
			{
				secret.Decode (CodeText, PlainText,  CodeTextLen);
				int PlainTextLen = 0;
				while (PlainTextLen < CodeTextLen)
				{
					if (PlainText[PlainTextLen] == '\0')
						break;
					PlainTextLen++;
				}
				if (PlainText[PlainTextLen] != '\0')
					MX_SETERROR(MX1055, MxError::CodeDefect, MxError::TerminateApp,  MxError::QuietReport, "secret.Decode() failed"); 
				else
				{
					size_t wcbuffsize = 0;
 					wcbuffsize = mbstowcs( NULL, (const char *)PlainText, 0); // C4996
					if (wcbuffsize < 1)
						MX_SETERROR(MX1056, MxError::CodeDefect, MxError::TerminateApp, MxError::QuietReport, "Invalid string length %d", (int)wcbuffsize); 
					else
					{
						std::wstring wcbuff( wcbuffsize, L'#' );
						wcbuffsize= mbstowcs( &wcbuff[0], (const char *)PlainText, PlainTextLen+1); 
						if (buffsize == (size_t) (-1))
						   MX_SETERROR(MX1057, MxError::CodeDefect, MxError::TerminateApp,  MxError::QuietReport, "Couldn't convert string %s (length=%d, buffsize=%d)", PlainText, PlainTextLen+1, (int)wcbuffsize);
						else
						{
							*val = wcbuff;
							rc = val->c_str();
						}
					}
				}
				free (PlainText);
			}
			
	#pragma warning(default:4996)

		}
	}
	return rc;	
}

bool   MxUtils::WritePasswordToFile(const BYTE *codetext, int len, const char *PathFilename)
{
	bool rc = false;

	if ( (codetext == nullptr ) || (PathFilename == nullptr))
		MX_SETERROR(MX1058, MxError::InvalidParams, MxError::TerminateApp, MxError::QuietReport, "Invalid params: codetext=%s, PathFilename=%s", (codetext == nullptr) ? "(null)" : "(not null)", (PathFilename == nullptr) ? "(null)" : "(not null)" ); 
	else
	{
		std::ofstream pwdfile;

		try
		{
			pwdfile.open (PathFilename, std::ios::binary);
			if (pwdfile.bad() == true)
				MX_SETERROR(MX1059, MxError::Resource,  MxError::TerminateApp, MxError::QuietReport, "unable to open password file: %s", PathFilename ); 
			else
			{
				pwdfile.write((const char *)codetext, len);
				if (pwdfile.bad() == true)
					MX_SETERROR(MX1060, MxError::BadUserInput, MxError::Abort, MxError::QuietReport, "unable to write password (length=%d) to password file: %s", len, PathFilename ); 
				else
					rc = true;
			}
		}
		catch(std::exception const& e)
		{
			MX_SETERROR(MX1061, MxError::CodeDefect, MxError::TerminateApp, MxError::QuietReport, "unexpected exception for file %s, msg=%s", PathFilename, e.what() ); 
		}
		pwdfile.close();
	}
	return rc;
}

int	   MxUtils::ReadPasswordFromFile(BYTE *codetext, int maxlen, const char *PathFilename)
{
	int rc = MxUtils::Invalid;

	if ( (codetext == nullptr ) || (PathFilename == nullptr))
		MX_SETERROR(MX1062, MxError::InvalidParams,  MxError::TerminateApp, MxError::QuietReport, "Invalid params: codetext=%s, PathFilename=%s", (codetext == nullptr) ? "(null)" : "(not null)", (PathFilename == nullptr) ? "(null)" : "(not null)" ); 
	else
	{
		std::ifstream pwdfile;
		try
		{
			pwdfile.open (PathFilename, std::ios::binary);
			if (pwdfile.bad() == true)
				MX_SETERROR(MX1063, MxError::Resource,  MxError::TerminateApp, MxError::QuietReport, "unable to open password file: %s", PathFilename );
			else
			{
				std::streampos fsize = 0;

				fsize = (int)pwdfile.tellg();
			    pwdfile.seekg( 0, std::ios::end );
				fsize = pwdfile.tellg() - fsize;
			    pwdfile.seekg( 0, std::ios::beg );

				if ((fsize > maxlen) || (fsize < 1))
					MX_SETERROR(MX1064, MxError::Resource,  MxError::TerminateApp, MxError::QuietReport, "password file %s is invalid, size=%d", PathFilename, fsize ); 
				else
				{
					pwdfile.read((char *)codetext, fsize);
					if (pwdfile.bad() == true)
						MX_SETERROR(MX1065, MxError::BadUserInput,  MxError::TerminateApp, MxError::QuietReport, "unable to read %d bytes from password file %s", fsize, PathFilename ); 
					else
						rc = (int) fsize;
				}
			}
		}
		catch(std::exception const& e)
		{
			MX_SETERROR(MX1066, MxError::CodeDefect, MxError::TerminateApp, MxError::QuietReport, "unexpected exception for file %s, msg=%s", PathFilename, e.what() ); 
		}
		pwdfile.close();
	}
	return rc;
}


LPCTSTR MxUtils::SetUserIDPassword(std::wstring *ConnStr, LPCTSTR DbPassword, LPCTSTR DbUserID )
{
	LPCTSTR rc = L"";

	if ( (ConnStr == nullptr) || (DbPassword == nullptr) || (_tcslen(DbPassword) < 1) || (( DbUserID != nullptr ) && (_tcslen(DbUserID) < 1)) )
		MX_SETERROR(MX1039, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "Invalid params: ConnStr=%s, DbPassword=%s, DbUserID=%s", (ConnStr == nullptr) ? "(null)" : "(not null)", (DbPassword == nullptr) ? "(null)" : "(not null)", (DbUserID == nullptr) ? "(null)": "(not null)" ); 
	else
	{
		std::wstring conn(*ConnStr);
		size_t foundstart = conn.rfind(';')+1;
		size_t foundend = conn.length();

		if ((foundstart != foundend) && (foundend > 0))
			conn += L";";
		if ( DbUserID != nullptr )
		{
			if ((foundstart = conn.find(L"User ID=")) == std::wstring::npos)
			{
				conn +=	L"User ID=";
				conn += DbUserID;
				conn += L";";
			}
			else
			{
				foundstart = conn.find(L"=", foundstart )+1;
				foundend = conn.find(L";", foundstart );
				if (foundend != std::wstring::npos)
					conn.replace(foundstart, foundend-foundstart, DbUserID);
				else
				{
					foundend = conn.length();
					conn.replace(foundstart, foundend-foundstart, DbUserID);
					conn += L";";
				}
			}
		}

		if ((foundstart = conn.find(L"Password=")) == std::wstring::npos)
		{
			conn +=	L"Password=";
			conn += DbPassword;
			conn += L";";
		}
		else
		{
			foundstart = conn.find(L"=", foundstart )+1;
			foundend = conn.find(L";", foundstart );
			if (foundend != std::wstring::npos)
				conn.replace(foundstart, foundend-foundstart, DbPassword);
			else
			{
				conn.replace(foundstart, foundend-foundstart, DbPassword);
				foundend = conn.length();
				conn += L";";
			}

		}
		*ConnStr = conn;
		rc = ConnStr->c_str();
	}
	return rc;
}

bool MxUtils::IsValidDbConnection(LPCTSTR connStr)
{
	bool rc = false;

	if ((connStr == nullptr) || (_tcslen(connStr) < 1))
		MX_SETERROR(MX1041, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "Invalid params: connStr=%s", (connStr == nullptr) ? "(null)" : "(not null)" ); 
	else
	{
		if (IsElementCorrect(connStr, L"User ID") == false )
			MX_SETERROR(MX1042, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "User ID not found in connStr=%s",  connStr); 
		else
		{
			if (IsElementCorrect(connStr, L"Password") == false )
				MX_SETERROR(MX1043, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "Password not found in connStr=%s",  connStr);
			else
			{
				if (IsElementCorrect(connStr, L"Data Source") == false )
					MX_SETERROR(MX1044, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "Data source not found in connStr=%s",  connStr);
				else
				{
					if (IsElementCorrect(connStr, L"Initial Catalog") == false )
						MX_SETERROR(MX1045, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "Initial Catalog not found in connStr=%s",  connStr);
					else
						rc = true;
				}
			}
		}
	}
	return rc;
}

bool MxUtils::GetElementValueFromConnStr(std::wstring *value, LPCTSTR connStr, LPCTSTR element)
{
	bool rc = false;

	if ((value == nullptr) || (connStr == nullptr) || (element == nullptr))
		MX_SETERROR(MX1040, MxError::InvalidParams,  MxError::TerminateApp, MxError::QuietReport, "Invalid params: value=%s, connStr=%s, element=%s", (value == nullptr) ? "(null)" : "(not null)", (connStr == nullptr) ? "(null)" : "(not null)", (element == nullptr) ? "(null)" : "(not null)"); 
	else
	{
		*value = L"";
		size_t foundstart = 0;
		size_t foundend = 0;
		std::wstring conn(connStr);
		std::wstring label(element);
		label += L"=";

		if ((foundstart = conn.find(label)) != std::wstring::npos)
		{
			foundstart = conn.find(L"=", foundstart )+1;
			foundend = conn.find(L";", foundstart );
			if ((foundend != std::wstring::npos) && (foundend > foundstart))
			{
				if(conn.find(label, foundend )== std::wstring::npos)
				{
					*value = conn.substr(foundstart, foundend-foundstart);
					rc = true;
				}
			}
		}
	}
	return rc;
}

bool MxUtils::DelRegNodeRecurse (HKEY hKeyRoot, LPTSTR lpSubKey)
{
	bool rc = false;

    LPTSTR lpEnd = nullptr;
    LONG lResult  = ERROR_INVALID_FUNCTION;
    DWORD dwSize = 0;
    TCHAR szName[MAX_PATH];
    HKEY hKey = NULL;
    FILETIME ftWrite;
	
	lResult = RegDeleteKey(HKEY_CURRENT_USER,  lpSubKey);	//attempt to delete the key - it may NOT have any children
	if (lResult == ERROR_SUCCESS)
		rc = true;											//it didn't have any children, so no need to recurse	
	else
	{
		try
		{
			USES_CONVERSION;
			HKEY hKey = NULL;
			lResult = RegOpenKeyEx (HKEY_CURRENT_USER, lpSubKey, 0, KEY_READ, &hKey);
			if (lResult != ERROR_SUCCESS) 
			{
				if (lResult == ERROR_FILE_NOT_FOUND) 
					MX_SETERROR(MX1004, MxError::Install, MxError::Abort, MxError::QuietReport, "Unable to delete app registry key - %s not found", W2CA(lpSubKey)); 
				else 
					MX_SETERROR(MX1003, MxError::Install, MxError::Abort, MxError::QuietReport, "Unable to delete app registry key - found, but cannot open %s", W2CA(lpSubKey)); 
			}
			else
			{
				lpEnd = lpSubKey + lstrlen(lpSubKey);		//add terminating backslash if missing
				if (*(lpEnd - 1) != TEXT('\\')) 
				{
					*lpEnd =  TEXT('\\');
					lpEnd++;
					*lpEnd =  TEXT('\0');
				}

				dwSize = MAX_PATH;
				lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL, NULL, NULL, &ftWrite);
				if (lResult != ERROR_SUCCESS) 
					MX_SETERROR(MX1005, MxError::Install, MxError::Abort, MxError::QuietReport, "Unable to enumerate child registry keys of %s", W2CA(lpSubKey)); 
				else
				{
					do 
					{
						StringCchCopy (lpEnd, MAX_PATH*2, szName);
						if (!DelRegNodeRecurse(hKeyRoot, lpSubKey)) 
						{
							break;
						}
						dwSize = MAX_PATH;
						lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,  NULL, NULL, &ftWrite);

					} while (lResult == ERROR_SUCCESS);
				}
				lpEnd--;
				*lpEnd = TEXT('\0');

				RegCloseKey (hKey);

				if (lpSubKey != NULL)
					lResult = RegDeleteKey(hKeyRoot, lpSubKey);

				if (lResult != ERROR_SUCCESS) 
					MX_SETERROR(MX1006, MxError::Install, MxError::Abort, MxError::QuietReport, "Unable to delete key %s", W2CA(lpSubKey)); 
				else
					rc = true;
  			}
		}
		catch(...)
		{
		}
    }
	return rc;
}


bool   MxUtils::IsElementCorrect(LPCTSTR connStr, LPCTSTR element)
{
	bool rc = false;

	size_t foundstart = 0;
	size_t foundend = 0;
	std::wstring conn(connStr);
	std::wstring label(element);
	label += L"=";

	if ((foundstart = conn.find(label)) != std::wstring::npos)
	{
		foundstart = conn.find(L"=", foundstart )+1;
		foundend = conn.find(L";", foundstart );
		if ((foundend != std::wstring::npos) && (foundend > foundstart))
		{
			if(conn.find(label, foundend )== std::wstring::npos)
				rc = true;
		}
	}
	return rc;
}


const wchar_t *MxUtils::GetEnvironmentVar(std::wstring *value, const wchar_t *name, bool reportNotFoundError)
{
	const wchar_t *rc = nullptr;

	if ((value == nullptr) || (name == nullptr))
		MX_SETERROR(MX1068, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "value or name parameter is null");
	else
	{
		wchar_t *pValue = nullptr;
		size_t len = 0;
		errno_t err = 0;
	
		std::string nameStr = "";
		if ((err = _wdupenv_s(&pValue, &len, name)) != 0)
			MX_SETERROR(MX1069, MxError::System, MxError::Abort, MxError::VerboseReport, "error getting environment value %s", (MxUtils::ConvertWString2String(&nameStr, name) == nullptr) ? "(null)" : nameStr.c_str());
		else
		{
			if ((pValue == nullptr) && (reportNotFoundError == true))
				MX_SETERROR(MX1070, MxError::Install, MxError::Abort, MxError::VerboseReport, "environment value %s not found or error", (MxUtils::ConvertWString2String(&nameStr, name) == nullptr) ? "(null)" : nameStr.c_str());
			else
			{
				*value = pValue;
				rc = value->c_str();
			}
		}
		free(pValue);	// It's OK to call free with NULL
	}
	return rc;
}
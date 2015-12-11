#include "stdafx.h"

#include <iostream>
#include <sstream>
#include <iomanip>

#include "atldbcli.h"
#include <atlconv.h>

#include "MxUtils\MxError1.h"
#include "EventTracing.h"


MxError::MxError() : 
	_ProductId(GUID_Invalid),
	_PiD(0),
	_firstErrorCode(MxError::ErrorCodeNotSet),
	_lastErrorCode(MxError::ErrorCodeNotSet),
	_errorCategory(MxError::Unknown),
	_resetAction(MxError::TerminateApp),
	_msgLevel(MxError::VerboseReport),
	_reportLevel(MxError::VerboseReport),
	_logChannel(MxError::Development),
	_startTime(clock()),
	_DbErrorMsgw(nullptr),
	_DbErrorMsg(nullptr),
	_ErrorStr(nullptr),
	_LogStr(nullptr),
	_ProdVersion(nullptr),
	_ProdName(nullptr),
	_OrgName(nullptr)
{

}

MxError::~MxError()
{
	CleanupOnAppExitOnly();
#ifdef _DEBUG
	::OutputDebugString(L"****************MxError::~MxError() all allocated memory now freed ********************\n" );
#endif
}

void MxError::CleanupOnAppExitOnly()
{
	if (_DbErrorMsg != nullptr)
		delete _DbErrorMsg;
	_DbErrorMsg = nullptr;
	if (_DbErrorMsgw != nullptr)
		delete _DbErrorMsgw;
	_DbErrorMsgw = nullptr;
	if (_ErrorStr != nullptr)
		delete _ErrorStr;
	_ErrorStr = nullptr;
	if (_LogStr != nullptr)
		delete _LogStr;
	_LogStr = nullptr;
	if (_ProdVersion != nullptr)
		delete _ProdVersion;
	_ProdVersion = nullptr;
	if (_ProdName != nullptr)
		delete _ProdName;
	_ProdName = nullptr;
	if (_OrgName != nullptr)
		delete _OrgName;
	_OrgName = nullptr;
}


void  MxError::Initialise(const char *OrgName, GUID  ProductId, const char *ProdName, const char *ProdVersion, LogChannel channel, Level reportlevel) 
{ 
	_ProductId = ProductId;
	_PiD = GetCurrentProcessId();
	_logChannel = channel;
	_reportLevel = reportlevel; 
	if (_ProdVersion == nullptr)
		 _ProdVersion = new std::string();
	if (_ProdVersion != nullptr)
		_ProdVersion->assign(ProdVersion);
	if (_ProdName == nullptr)
		 _ProdName = new std::string();
	if (_ProdName != nullptr)
		_ProdName->assign(ProdName);
	if (_OrgName ==  nullptr)
		_OrgName = new std::string();
	if (_OrgName != nullptr)
		_OrgName->assign(OrgName);

	Reset();
}

void MxError::Reset()
{
	_firstErrorCode = MxError::ErrorCodeNotSet; 
	_lastErrorCode = MxError::ErrorCodeNotSet;
	_errorCategory = MxError::Unknown;
	_resetAction = MxError::TerminateApp;
	_msgLevel = MxError::VerboseReport;

	if (_ErrorStr != nullptr)
		_ErrorStr->assign("");
	if (_LogStr != nullptr)
		_LogStr->assign("");
	if (_DbErrorMsg != nullptr)
		_DbErrorMsg->assign("");
	if (_DbErrorMsgw != nullptr)
		_DbErrorMsgw->assign(L"");
}

bool MxError::SetLastError(const char *functionname,  int LineNo, int ErrorCode, MxError::Category category, MxError::ResetAction reset, MxError::Level level, const char *printfStr, ...  )
{
	bool rc = false;

	if (_firstErrorCode != MxError::ErrorCodeNotSet) 
		_lastErrorCode = ErrorCode;
	else
	{
		_firstErrorCode = ErrorCode;
		_lastErrorCode = ErrorCode;
		_resetAction = reset;
		_msgLevel = level;
		_errorCategory = category;
	}

	if ( IsReport(level) == true )
	{
		clock_t now = clock();
		double ticks = (double)(now - _startTime) / CLOCKS_PER_SEC;

		SetErrorStr(ErrorCode, category, reset, level);

#pragma warning(disable:4996)		//_snprintf is depreciated

		char	buffer[MxError::DebugStringLen];

		int len = 0;
		int lenPreamble = 0;

		if ((printfStr != nullptr) && (functionname != nullptr))
		{
			va_list argptr;
			va_start( argptr, printfStr );

			std::string tmp("");
			lenPreamble = _snprintf(buffer, sizeof(buffer), "[%7.3f] (%x) %s: %s, line=%d, %s", ticks, _PiD, GetOrgName(), functionname, LineNo, ((_ErrorStr != nullptr) ? _ErrorStr->c_str() : "(null)"));

			if ( (lenPreamble >= sizeof(buffer)) || (lenPreamble < 0) )
				len = lenPreamble;
			else
			{
				int lenFormat = vsnprintf(&buffer[lenPreamble], MxError::DebugStringLen-lenPreamble, printfStr, argptr);
				if ( lenFormat <= 0 )
					len = sizeof(buffer);
				else
					len = lenPreamble + lenFormat;
			}
			va_end( argptr );
		}
		if ((len < (sizeof(buffer)-1)) && (len >= 0))
		{
			buffer[len] = '\n';
			buffer[len+1] = '\0';
		}
		else
		{
			if (len < 0 )
			{
				buffer[0] = '*';
				buffer[1] = '\n';
				buffer[2] = '\0';
			}
			else
			{
				buffer[sizeof(buffer)-5] = '.';
				buffer[sizeof(buffer)-4] = '.';
				buffer[sizeof(buffer)-3] = '.';
				buffer[sizeof(buffer)-2] = '\n';
				buffer[sizeof(buffer)-1] = '\0';
			}
		}
  #pragma warning(default:4996)

		if (_ErrorStr == nullptr)
			_ErrorStr = new std::string(buffer);
		else
			_ErrorStr->assign(buffer);

		switch ( _logChannel  )
		{
			case  MxError::Development:
			{
				std::wstring msg(L"(_ErrorStr == nullptr)");
				if (_ErrorStr != nullptr)
					msg.assign(_ErrorStr->begin(), _ErrorStr->end());
				::OutputDebugString(msg.c_str());
				rc = true;
			}
			break;

			case  MxError::Production:
			{
				std::string tmp;
				rc = EventTracing::Inst().SetErrorEvent(GetOrgName(), MxGUID::ConvertToString(_ProductId, &tmp), GetProductName(), GetProductVersion(), _PiD, ErrorCode, category, reset, ((_ErrorStr != nullptr) ? _ErrorStr->c_str() : "(_ErrorStr is null)"));
			}
			break;

			default:	//MxError::NoLogging
				rc = true;
			break;
		}
	}
	return true;
}



bool MxError::LogMsg(const char *functionname,  int LineNo, MxError::Level level, bool ShowProdId, const char *printfStr, ... )
{
	bool rc = false;

	if (IsReport(level) == false)
		rc = true;
	else
	{
		clock_t now = clock();
		double ticks = (double)(now - _startTime) / CLOCKS_PER_SEC;

#pragma warning(disable:4996)		//_snprintf is depreciated

		char	buffer[MxError::DebugStringLen];

		int len = 0;
		int lenPreamble = 0;

		if ((printfStr != nullptr) && (functionname != nullptr))
		{
			va_list argptr;
			va_start( argptr, printfStr );

			std::string tmp("");
			lenPreamble = _snprintf(buffer, sizeof(buffer), "[%7.3f] (%x) %s:%s %s, line=%d, %s, Log: ", ticks, _PiD, GetOrgName(), ((ShowProdId == true) ? MxGUID::ConvertToString(_ProductId, &tmp) : ""), functionname, LineNo, GetProductVersion());

			if ( (lenPreamble >= sizeof(buffer)) || (lenPreamble < 0) )
				len = lenPreamble;
			else
			{
				int lenFormat = vsnprintf(&buffer[lenPreamble], MxError::DebugStringLen-lenPreamble, printfStr, argptr);
				if ( lenFormat <= 0 )
					len = sizeof(buffer);
				else
					len = lenPreamble + lenFormat;
			}
			va_end( argptr );
		}
		if ((len < (sizeof(buffer)-1)) && (len >= 0))
		{
			buffer[len] = '\n';
			buffer[len+1] = '\0';
		}
		else
		{
			if (len < 0 )
			{
				buffer[0] = '*';
				buffer[1] = '\n';
				buffer[2] = '\0';
			}
			else
			{
				buffer[sizeof(buffer)-5] = '.';
				buffer[sizeof(buffer)-4] = '.';
				buffer[sizeof(buffer)-3] = '.';
				buffer[sizeof(buffer)-2] = '\n';
				buffer[sizeof(buffer)-1] = '\0';
			}
		}
  #pragma warning(default:4996)

		if (_LogStr == nullptr)
			_LogStr = new std::string(buffer);
		else
			_LogStr->assign(buffer);

		if (_LogStr != nullptr)
		{
			switch ( _logChannel  )
			{
				case  MxError::Development:
				{
					std::wstring msg;
					msg.assign(_LogStr->begin(), _LogStr->end());
					::OutputDebugString(msg.c_str());
					rc = true;
				}
				break;

				case  MxError::Production:
				{
					std::string tmp;
					rc = EventTracing::Inst().SetLogEvent(GetOrgName(), MxGUID::ConvertToString(_ProductId, &tmp), GetProductName(), GetProductVersion(), _PiD, functionname,  LineNo, ((_LogStr != nullptr) ? _LogStr->c_str() : "(_LogStr is null)"));
				}
				break;

				default:	//MxError::NoLogging
					rc = true;
				break;
			}
		}
		if (_LogStr != nullptr)
			rc = true;
	}
	return rc;
}

bool MxError::GetProductID(GUID *guid) 
{
	bool rc = false;
	if ((guid != nullptr) && (MxGUID::IsValid(_ProductId)))
	{

		*guid = _ProductId;
		rc = true;
	} 
	return rc;
}

void MxError::SetErrorStr(int ErrorCode, MxError::Category category, MxError::ResetAction action, MxError::Level msg)
{

	std::ostringstream out;

	out << GetProductVersion() << ", ";
	out << "Error=" << ErrorCode << ", ";
	out << GetErrorCategoryStr(category) << ", ";
	out << GetResetActionStr(action) << ": ";

	if (_errorCategory == MxError::Database)
		out <<  "[database error: " <<	GetDatabaseErrors() << "] ";

	if (_ErrorStr == nullptr)
		_ErrorStr = new std::string();
	if (_ErrorStr != nullptr)
		_ErrorStr->assign(out.str());
}

const char *MxError::GetDatabaseErrors()
{
	const char *rc = "";

	try
	{
		CDBErrorInfo myErrorInfo; 
		ULONG numRec = 0; 
		BSTR myErrStr = nullptr;
		BSTR mySource = nullptr;
		ISQLErrorInfo *pISQLErrorInfo = NULL; 

		if(_DbErrorMsgw == nullptr)
			_DbErrorMsgw = new std::wstring();
		if(_DbErrorMsg == nullptr)
			_DbErrorMsg = new std::string();

		if((_DbErrorMsgw != nullptr) && (_DbErrorMsg != nullptr))
		{
			LCID lcLocale = GetSystemDefaultLCID(); 
			myErrorInfo.GetErrorRecords(&numRec); 
			if (numRec > 0) 
			{ 
				USES_CONVERSION;    // declare locals used by the ATL macros

				myErrorInfo.GetAllErrorInfo(0,lcLocale,&myErrStr,&mySource);
				std::wstring ws(myErrStr, SysStringLen(myErrStr));

				*_DbErrorMsgw = ws;
				*_DbErrorMsg = W2CA(_DbErrorMsgw->c_str());

				rc = _DbErrorMsg->c_str();

				::SysFreeString(myErrStr);
				::SysFreeString(mySource);
			}
		}
	}
	catch(std::exception const& e)
	{
		if(_DbErrorMsg == nullptr)
			*_DbErrorMsg = e.what();
	}
	return rc;
}

bool MxError::IsReport(Level msgLevel)
{ 
	bool rc = false;

	if ( _logChannel != MxError::NoLogging )
	{
		switch(_reportLevel)
		{
			case MxError::QuietReport:
				rc = (msgLevel == MxError::QuietReport) ? true : false;
			break;
			case MxError::AnalysisReport:
				rc = ((msgLevel == MxError::QuietReport) || (msgLevel == MxError::AnalysisReport)) ? true : false;
			break;
			default:		//MxError::VerboseReport
				rc = true;
			break;	
		}
	}
	return rc;
}
const char * MxError::GetErrorCategoryStr(MxError::Category category)
{
	const char *rc = "Unknown";

	switch(category)
	{
		case MxError::InvalidParams:
			rc = "InvalidParams";
		break;
		case MxError::BadUserInput:
			rc = "BadUserInput";
		break;
		case MxError::CodeDefect:
			rc = "CodeDefect";
		break;
		case MxError::Install:
			rc = "Install";
		break;
		case MxError::Config:
			rc = "Config";
		break;
		case MxError::Resource:
			rc = "Resource";
		break;
		case MxError::Database:
			rc = "Database";
		break;
		case MxError::System:
			rc = "System";
		break;
		default:
		break;
	}
	return rc;
}
const char * MxError::GetResetActionStr(MxError::ResetAction action)
{
	const char *rc = "TerminateApp";

	switch(action)
	{
		case MxError::Retry:
			rc = "Retry";
		break;
		case MxError::Abort:
			rc = "Abort";
		break;
		case MxError::Warn:
			rc = "Warn";
		break;
		default:
		break;
	}
	return rc;
}

const char * MxError::GetLevelStr(MxError::Level msg)
{
	const char *rc = "VerboseReport";

	switch(msg)
	{
		case MxError::QuietReport:
			rc = "QuietReport";
		break;
		case MxError::AnalysisReport:
			rc = "AnalysisReport";
		break;
		case MxError::VerboseReport:
			rc = "VerboseReport";
		break;
		default:
		break;	
	}
	return rc;
}

const char *MxError::GetWebsiteDefectReportUrl(std::string *result, const char* url, const GUID Licence)
{
	const char* rc = nullptr;

	if ((result != nullptr) && (url != nullptr))
	{
		std::string tmp;
		std::string webUrl (url);
		GUID pid;
		char buff[10];

		if (GetProductID(&pid))
		{
			webUrl += "&Prod=";
			webUrl += MxGUID::ConvertToString(pid, &tmp);
		}
		webUrl += "&Ver=";
		webUrl += GetProductVersion();
		webUrl += "&Lic=";
		webUrl += MxGUID::ConvertToString(Licence, &tmp);
		webUrl += "&FirstError=";
		_itoa_s(GetErrorCode(), buff, 10);
		webUrl += buff;
		webUrl += "&LastError=";
		_itoa_s(GetLastErrorCode(), buff, 10);
		webUrl += buff;
		webUrl += "&Cat=";
		webUrl += GetErrorCategoryStr(GetErrorCategory());

		result->assign(webUrl);
		rc = result->c_str();

		//ShellExecute(NULL, L"open", L"http://www.unientry.org?Usr=Wills&Lic=000000", NULL, NULL, SW_SHOWNORMAL);
	}
	return rc;
}
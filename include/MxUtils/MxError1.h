#pragma once
//============================================================================================================================
//MxError1.h - contains the class MxError which was used to build MxUtils1.lib, .dll (debug=MxUtils1D.lib, .dll)
//============================================================================================================================
//
//		If alterations are required to MxError, then create a new file (i.e. MxError2.h ) and rebuild MxUtils using this
//		new file, incrementing the target file names as described in the MxUtils Solution BuildNotes (i.e. MxUtils2.lib, .dll)
//		Important: the name of this file needs to match the MxUtil target filenames.
//
//============================================================================================================================

#ifdef MXUTILS_EXPORTS
#define MXUTILS_API __declspec(dllexport)
#else
#define MXUTILS_API __declspec(dllimport)
#endif

#include <time.h>
#include <iostream>
#include "MxProductID.h"


#define MX_SETUP(OrgName, ProductID, ProdName, Ver, Channel, Level, logmsg, ...) (MxError::Inst().Initialise(OrgName, ProductID, ProdName, Ver, Channel, Level), MxError::Inst().LogMsg(__FUNCTION__, __LINE__, Level, true, logmsg, __VA_ARGS__))
#define MX_END(Level, logmsg, ...) (MxError::Inst().LogMsg(__FUNCTION__, __LINE__, Level, true, logmsg, __VA_ARGS__))
#define MX_SETERROR(ErrorCode, ErrorType, Reset, Level, logmsg, ...) (MxError::Inst().SetLastError(__FUNCTION__, __LINE__, ErrorCode, ErrorType, Reset, Level, logmsg, __VA_ARGS__))
#define MX_LOGMSG(Level, logmsg, ...) (MxError::Inst().LogMsg(__FUNCTION__, __LINE__, Level, false, logmsg, __VA_ARGS__))

class MXUTILS_API MxError 
{
public:
	static const int	MxError::ErrorCodeNotSet = 0;
	static const int	MxError::ProductIDNotSet = 0;
	static const int	MxError::DebugStringLen = 512;			//must be > 5	'...[\n][\0]'
	static const int	MxError::MaxUserNameLength = 20;

	enum Category
	{                            //note: change GetErrorCategoryStr() when adding/deleting items (changing the order or adding new items may break some unit tests)
		BadUserInput = 1,	//user error
		CodeDefect,			//programmer error
		InvalidParams,		//passed parameters are invalid - subtype of CodeDefect
		Install,			//installation problem - i.e. could not find Document folder, registry setting, etc
		Config,				//configuration problem - subtype of Install - i.e. invalid value for registry setting
		System,				//system problem - something wrong with the system environment
		Resource,			//resource problem - subtype of System - i.e. no more memory, GDI handles, etc
		Database,			//database problem - subtype of System - i.e. database connection cannot be established
		Unknown				//used only as a default error 
	};

	enum Level
	{                             //note: change GetLevelStr() when adding/deleting items (changing the order or adding new items may break some unit tests)
 		QuietReport,		//set for Msgs that should always be reported
		AnalysisReport,		//set for Msgs that should be reported during problem analysis (bugs, performance issues, etc)
		VerboseReport		//set for Msgs that should be reported only when Initialise(OrgName, ProductId, ProdName, Version, LogChannel channel, Level::VerboseReport) = 0;
	};

	enum ResetAction
	{                            //note: change GetResetActionStr() when adding/deleting items (changing the order or adding new items may break some unit tests)
		TerminateApp,		//ok
		Retry,				//yes=retry, no=abort
		Abort,				//yes=abort, no=ignore and continue
		Warn				//ok
	};

	enum LogChannel             //note: changing the order or adding new items may break some unit tests
	{
		Development,		//use OutputDebugString();
		Production,			//use Event Tracing for Windows (ETW)
		NoLogging			//no logging
	};

	bool			GetProductID(GUID *guid);
	const char *    GetProductName() { return ((_ProdName != nullptr) ? _ProdName->c_str() : "(prodname not set)"); }
	const char *    GetProductVersion() { return ((_ProdVersion != nullptr) ? _ProdVersion->c_str() : "(version not set)"); }
	const char *    GetOrgName() { return ((_OrgName != nullptr) ? _OrgName->c_str() : "(orgname not set)"); }

	void			SetLogging(LogChannel channel, Level report) { _logChannel = channel, _reportLevel = report; }
	LogChannel		GetLogChannel() { return _logChannel; }
	Level			GetReportLevel() { return _reportLevel; }

	int				GetErrorCode() { return _firstErrorCode; }
	int				GetLastErrorCode() { return _lastErrorCode; }
	Category		GetErrorCategory() { return _errorCategory; }
	ResetAction		GetErrorResetAction() {return _resetAction; }
	Level			GetMsgLevel() { return _msgLevel; }

	void			Initialise(const char *OrgName, GUID  ProductId, const char *ProdName, const char *version, LogChannel channel, Level reportlevel);
	bool			SetLastError(const char *functionname,  int LineNo, int ErrorCode, MxError::Category category, MxError::ResetAction reset, MxError::Level level, const char *printfStr, ... );

	bool			IsErrorSet() { return (_firstErrorCode == MxError::ErrorCodeNotSet) ? false : true; }
	void			Reset();

	const char *	GetLastErrorStr() { return ((_ErrorStr == nullptr) ? "" : _ErrorStr->c_str()); };
	const char*		GetLastLogStr() { return ((_LogStr == nullptr) ? "" : _LogStr->c_str()); }
	const wchar_t *	GetDatabaseErrorMsg() { return ((_DbErrorMsgw == nullptr) ? L"" : _DbErrorMsgw->c_str()); }

	static const char *		GetErrorCategoryStr(MxError::Category category);
	static const char *		GetResetActionStr(MxError::ResetAction action);
	static const char *		GetLevelStr(MxError::Level msg);

	bool			IsReport(Level msgLevel); 
	bool			LogMsg(const char *functionname,  int LineNo, MxError::Level level, bool ShowProdId, const char *printfStr, ... );

	const char *	GetWebsiteDefectReportUrl(std::string *result, const char* url, const GUID Licence);

    static MxError& Inst()
    {
        static MxError    instance;			// Guaranteed to be destroyed. Instantiated on first use.
        return instance;
    }

	void CleanupOnAppExitOnly();			//deletes all allocated memory (inst cannot be used after this call) - stops VS2012 reporting memory leaks before ~MxError() is called; only call this if there's a problem


 private:
	MxError();
	~MxError();
    MxError(MxError const&);              // Don't Implement
    void operator=(MxError const&);		  // Don't implement

	const char * GetDatabaseErrors();
	void		 SetErrorStr(int ErrorCode, MxError::Category category, MxError::ResetAction action, MxError::Level msg);
	
	GUID 		_ProductId;
	DWORD		_PiD;
	int			_firstErrorCode;
	int			_lastErrorCode;
	Category	_errorCategory;
	ResetAction _resetAction;
	Level		_msgLevel;
	Level		_reportLevel;			//report all errors with level equal or less than this value

	LogChannel	_logChannel;
	clock_t		_startTime;

		#pragma warning(disable:4251)
	std::wstring* _DbErrorMsgw;
	std::string*  _DbErrorMsg;
	std::string*  _ErrorStr;
	std::string*  _LogStr;
	std::string*  _ProdVersion;
	std::string*  _ProdName;
	std::string*  _OrgName;
		#pragma warning(default:4251)
};

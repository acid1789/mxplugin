#pragma once

#ifdef MXUTILS_EXPORTS
#define MXUTILS_API __declspec(dllexport)
#else
#define MXUTILS_API __declspec(dllimport)
#endif

#include <evntprov.h>
#include "MxUtils\MxError1.h"

class MXUTILS_API EventTracing
{
public:
	bool SetErrorEvent(const char *OrgName, const char *ProductId, const char *ProductName, const char *ProdVersion, DWORD Pid, int ErrorCode, MxError::Category category, MxError::ResetAction reset, const char *ErrMsg);
	bool SetLogEvent(const char *OrgName, const char *ProductId, const char *ProductName, const char *ProdVersion, DWORD Pid, const char *FunctionName, int LineNo, const char *ErrMsg);
	bool IsValid() { return (( _RegistrationHandle != NULL ) ? true : false); }
	bool IsEnabled();

	static EventTracing& Inst()
    {
        static EventTracing   instance;			// Guaranteed to be destroyed. Instantiated on first use.
        return instance;
    }

 private:
	EventTracing();
	~EventTracing();
    EventTracing(EventTracing const&);          // Don't Implement
    void operator=(EventTracing const&);		// Don't implement

	REGHANDLE _RegistrationHandle;

};


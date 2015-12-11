#include "stdafx.h"

#include <windows.h>
#include <stdio.h>
#include <Winmeta.h>

#include <string>
#include <iostream>

#include "EventTracing.h"
#include "MxUtils1Events.h"

#define MAX_PAYLOAD_DESCRIPTORS_ERROR  8
#define MAX_PAYLOAD_DESCRIPTORS_LOG	   7

EventTracing::EventTracing() :
	_RegistrationHandle(NULL)
{
	DWORD status = ERROR_SUCCESS;

    status = EventRegister(
        &MxUtils1Lib_Events,      // GUID that identifies the provider
        NULL,               // Callback not used
        NULL,               // Context noot used
        &_RegistrationHandle // Used when calling EventWrite and EventUnregister
        );

    if (ERROR_SUCCESS != status)
    {
		 EventUnregister(_RegistrationHandle);
		 _RegistrationHandle = NULL;
    }
}

EventTracing::~EventTracing()
{
	if (_RegistrationHandle != NULL)
	{
		EventUnregister(_RegistrationHandle);
		_RegistrationHandle = NULL;
	}
}


bool EventTracing::IsEnabled()
{
	bool rc = false;

	if (_RegistrationHandle != NULL)
	{
		if ( EventProviderEnabled(_RegistrationHandle, WINEVENT_LEVEL_VERBOSE, MxErrorEventKW | MxLogEventKW ) == TRUE)
			rc = true;
	}
	return rc;

}

bool EventTracing::SetErrorEvent(const char *OrgName, const char *ProductId, const char *ProductName, const char *ProdVersion, DWORD Pid, int ErrorCode, MxError::Category category, MxError::ResetAction reset, const char *ErrMsg )
{
	bool rc = false;

	if ((_RegistrationHandle != NULL) && (ProductId != nullptr) && (ProductName != nullptr) && (ErrMsg != nullptr))
	{
		EVENT_DATA_DESCRIPTOR Descriptors[MAX_PAYLOAD_DESCRIPTORS_ERROR]; 
		DWORD i = 0;

		int	code = ErrorCode;
		int cat = category;
		int action = reset;
		unsigned int pid = Pid;

		//<data name="ProductName" inType="win:AnsiString" outType="xs:string">
        //<data name="ProductID" inType="win:AnsiString" outType="xs:string">
        //<data name="Version" inType="win:AnsiString" outType="xs:string">
        //<data name="ProcessID" inType="win:UInt32" outType="xs:unsignedInt">
        //<data name="ErrorCode" inType="win:Int32" outType="xs:int">
		//<data name="Category" inType="win:Int32" outType="xs:int">
		//<data name="Reset" inType="win:Int32" outType="xs:int">
		//<data name="ErrorMsg" inType="win:AnsiString" outType="xs:string">

		EventDataDescCreate(&Descriptors[i++], ProductName, ((ULONG)strlen(ProductName)+1)*sizeof(CHAR));
		EventDataDescCreate(&Descriptors[i++], ProductId, ((ULONG)strlen(ProductId)+1)*sizeof(CHAR));
		EventDataDescCreate(&Descriptors[i++], ProdVersion, ((ULONG)strlen(ProdVersion)+1)*sizeof(CHAR));
		EventDataDescCreate(&Descriptors[i++], &pid, sizeof(UINT32));
		EventDataDescCreate(&Descriptors[i++], &code, sizeof(INT32));
		EventDataDescCreate(&Descriptors[i++], &cat, sizeof(INT32));
		EventDataDescCreate(&Descriptors[i++], &action, sizeof(INT32));
		EventDataDescCreate(&Descriptors[i++], ErrMsg, ((ULONG)strlen(ErrMsg)+1)*sizeof(CHAR));

		DWORD status = ERROR_SUCCESS;
		status = EventWrite(
			_RegistrationHandle,              // From EventRegister
			&MxErrorEvent,					  // EVENT_DESCRIPTOR generated from the manifest
			(ULONG)MAX_PAYLOAD_DESCRIPTORS_ERROR,  // Size of the array of EVENT_DATA_DESCRIPTORs
			&Descriptors[0]                  // Array of descriptors that contain the event data
			);

		if (status == ERROR_SUCCESS) 
			rc = true;
	}
	return rc;
}


bool EventTracing::SetLogEvent(const char *OrgName, const char *ProductId, const char *ProductName, const char *ProdVersion, DWORD Pid, const char *FunctionName, int LineNo, const char *ErrMsg)
{
	bool rc = false;

	if ((_RegistrationHandle != NULL) && (ProductId != nullptr) && (ProductName != nullptr) && (FunctionName != nullptr) && (ErrMsg != nullptr))
	{
		EVENT_DATA_DESCRIPTOR Descriptors[MAX_PAYLOAD_DESCRIPTORS_LOG]; 
		DWORD i = 0;

		unsigned int pid = Pid;
		int	  Line = LineNo;

		//<data name="ProductName" inType="win:AnsiString" outType="xs:string">
		//<data name="ProductID" inType="win:AnsiString" outType="xs:string">
		//<data name="Version" inType="win:AnsiString" outType="xs:string">
		//<data name="ProcessID" inType="win:UInt32" outType="xs:unsignedInt">
		//<data name="FunctionName" inType="win:AnsiString" outType="xs:string">
		//<data name="LineNo" inType="win:Int32" outType="xs:int">
		//<data name="Msg" inType="win:AnsiString" outType="xs:string">

		EventDataDescCreate(&Descriptors[i++], ProductName, ((ULONG)strlen(ProductName)+1)*sizeof(CHAR));
		EventDataDescCreate(&Descriptors[i++], ProductId, ((ULONG)strlen(ProductId)+1)*sizeof(CHAR));
		EventDataDescCreate(&Descriptors[i++], ProdVersion, ((ULONG)strlen(ProdVersion)+1)*sizeof(CHAR));
		EventDataDescCreate(&Descriptors[i++], &pid, sizeof(UINT32));
		EventDataDescCreate(&Descriptors[i++], FunctionName, ((ULONG)strlen(FunctionName)+1)*sizeof(CHAR));
		EventDataDescCreate(&Descriptors[i++], &Line, sizeof(INT32));
		EventDataDescCreate(&Descriptors[i++], ErrMsg, ((ULONG)strlen(ErrMsg)+1)*sizeof(CHAR));

		DWORD status = ERROR_SUCCESS;
		status = EventWrite(
			_RegistrationHandle,              // From EventRegister
			&MxLogEvent,					  // EVENT_DESCRIPTOR generated from the manifest
			(ULONG)MAX_PAYLOAD_DESCRIPTORS_LOG,  // Size of the array of EVENT_DATA_DESCRIPTORs
			&Descriptors[0]                  // Array of descriptors that contain the event data
			);

		if (status == ERROR_SUCCESS) 
			rc = true;
	}
	return rc;
}
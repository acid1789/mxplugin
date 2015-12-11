#pragma once

#ifdef MXPLUGINMANAGER_EXPORTS
#define MXPLUGINMANAGER_API __declspec(dllexport)
#else
#define MXPLUGINMANAGER_API __declspec(dllimport)
#endif

#include <Windows.h>

#include "MxUtils\MxError1.h"
#include "MxPluginManager\MxPluginManagerErrorCodes1.h"
#include "IMxPluginLibDetails.h" 


class MXPLUGINMANAGER_API MxPluginLib 
{
public:
	static const char * const    MxPluginLib::endl;
	static const char * const    MxPluginLib::InvalidStr;
	static const wchar_t * const MxPluginLib::InvalidwStr;
	static const long			 MxPluginLib::Invalid = -1;

public:
	MxPluginLib(void);
	~MxPluginLib(void);

	bool		IsValid() { return (_pLibDetails != '\0') ? true : false; }
	void *		CreateClassInstance(const GUID IID);

	const GUID	GetLibID() { return (_pLibDetails != '\0') ? MxGUID::ConvertFromString(_pLibDetails->GetLibID()) : GUID_Invalid; }
	const char *GetLibName() { return (_pLibDetails != '\0') ? _pLibDetails->GetLibName() : MxPluginLib::InvalidStr; }
	const char *GetLibVersion() { return (_pLibDetails != '\0') ? _pLibDetails->GetLibVersion() : MxPluginLib::InvalidStr; }
	const char *GetLibDetails() { return _DetailsStr.c_str(); }
	const char *GetLibSummary() { return _SummaryStr.c_str(); }
	const wchar_t *GetwPathFilename() { return _wPathFilename.c_str(); }


	const char *GetPathFilename() { return _PathFilename.c_str(); }
	void		SetSelection(bool check) { _Select = check; }
	bool		GetSelection() {return _Select; }

	IMxPluginLibDetails *GetIMxPluginLibDetails(); //calls DuplicatePtr, call DestroyPtr when you've finished with it

private:
	friend class MxPluginManager;		//provide access to MxPluginLib's private ctor for MxPluginManager::RefreshPlugins()

	MxPluginLib(const wchar_t *pathfilename, HINSTANCE hInstance, void * (MXSTDMETHOD *pfn)(const GUID));

	IMxPluginLibDetails *_pLibDetails;
	HINSTANCE			 _hInstance;
	void * (MXSTDMETHOD *_pfn)(const GUID);
	bool				 _Select;

#pragma warning(disable: 4251)
	std::wstring		 _wPathFilename;
	std::string			 _PathFilename;
	std::string			 _DetailsStr;
	std::string			 _SummaryStr;
#pragma warning(default: 4251)
};


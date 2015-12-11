#pragma once

#include <string>
#include "IMxPluginLibDetails.h" 

class MxPluginLib 
{
public:
	static const char * const MxPluginLib::endl;
	static const char * const MxPluginLib::InvalidStr;
	static const wchar_t * const MxPluginLib::InvalidwStr;
	static const long		  MxPluginLib::Invalid = -1;

public:
	MxPluginLib(void);
	MxPluginLib(const wchar_t *pathfilename, HINSTANCE hInstance, void * (MXSTDMETHOD *pfn)(const char *));
	~MxPluginLib(void);

	bool		IsValid() { return (_pLibDetails != '\0') ? true : false; }
	void *		CreateClassInstance(const char *IID);

	const char *GetLibID() { return (_pLibDetails != '\0') ? _pLibDetails->GetLibID() : MxPluginLib::InvalidStr; }
	const char *GetLibName() { return (_pLibDetails != '\0') ? _pLibDetails->GetLibName() : MxPluginLib::InvalidStr; }
	const char *GetLibVersion() { return (_pLibDetails != '\0') ? _pLibDetails->GetLibVersion() : MxPluginLib::InvalidStr; }
	const char *GetLibDetails() { return _DetailsStr.c_str(); }
	const char *GetLibSummary() { return _SummaryStr.c_str(); }
	const wchar_t *GetwPathFilename() { return _wPathFilename.c_str(); }
	const char *GetPathFilename() { return _PathFilename.c_str(); }

	void		SetSelection(bool check) { _Select = check; }
	bool		GetSelection() {return _Select; }

private:
	IMxPluginLibDetails *_pLibDetails;
	HINSTANCE			 _hInstance;
	void * (MXSTDMETHOD *_pfn)(const char *);
	std::wstring		 _wPathFilename;
	std::string			 _PathFilename;
	std::string			 _DetailsStr;
	std::string			 _SummaryStr;
	bool				 _Select;
};


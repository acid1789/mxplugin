#pragma once

#include <string>

class MxPluginBaseEditableIID 
{
public:
	static const char * const MxPluginBaseEditableIID::Invalid;
public:
	MxPluginBaseEditableIID();
	MxPluginBaseEditableIID(const char *Name, const char *GUID);
	MxPluginBaseEditableIID(const wchar_t *Name, const wchar_t *GUID);
	~MxPluginBaseEditableIID(void);

	void SetName(const char *Name);
	void SetName(const wchar_t *Name);
	const CString& GetName() { return m_Name; }

	void SetGUID(const char *GUID);
	void SetGUID(const wchar_t *GUID);
	const CString& GetGUID() { return m_GUID; }

	const char *Name() { return _Name.c_str(); }
	const char *GUID() { return _GUID.c_str(); }

private:
	CString m_Name;
	CString m_GUID;
	std::string _Name;
	std::string _GUID;
};


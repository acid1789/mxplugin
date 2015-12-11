#include "StdAfx.h"
#include <atlconv.h>
#include "MxPluginBaseEditableIID.h"

const char *    const MxPluginBaseEditableIID::Invalid = "[Invalid]";

MxPluginBaseEditableIID::MxPluginBaseEditableIID() :
	m_Name(MxPluginBaseEditableIID::Invalid),
	m_GUID(MxPluginBaseEditableIID::Invalid),
	_Name(MxPluginBaseEditableIID::Invalid),
	_GUID(MxPluginBaseEditableIID::Invalid)
{

}


MxPluginBaseEditableIID::MxPluginBaseEditableIID(const char *Name, const char *GUID) :
	m_Name((Name != '\0') ? Name : MxPluginBaseEditableIID::Invalid),
	m_GUID((GUID != '\0') ? GUID: MxPluginBaseEditableIID::Invalid),
	_Name((Name != '\0') ? Name : MxPluginBaseEditableIID::Invalid),
	_GUID((GUID != '\0') ? GUID: MxPluginBaseEditableIID::Invalid)
{
}

MxPluginBaseEditableIID::MxPluginBaseEditableIID(const wchar_t *Name, const wchar_t *GUID) :
	m_Name(MxPluginBaseEditableIID::Invalid),
	m_GUID(MxPluginBaseEditableIID::Invalid),
	_Name(MxPluginBaseEditableIID::Invalid),
	_GUID(MxPluginBaseEditableIID::Invalid)
{
	USES_CONVERSION;    
	if(Name != '\0')
	{
		m_Name = Name;
		_Name = (CW2A(Name));
	}
	if (GUID != '\0')
	{
		m_GUID = GUID;
		_GUID = (CW2A(GUID));
	}
}

MxPluginBaseEditableIID::~MxPluginBaseEditableIID(void)
{
}

void MxPluginBaseEditableIID::SetName(const wchar_t *Name)
{
	USES_CONVERSION; 
	if (Name != '\0')
	{
		m_Name = Name;
		_Name = (CW2A(Name));
	}
	else
	{
		m_Name = MxPluginBaseEditableIID::Invalid;
		_Name = MxPluginBaseEditableIID::Invalid;
	}
}

void MxPluginBaseEditableIID::SetName(const char *Name)
{ 
	if(Name != '\0')
	{
		m_Name = Name;
		_Name = Name;
	}
	else
	{
		m_Name = MxPluginBaseEditableIID::Invalid;
		_Name = MxPluginBaseEditableIID::Invalid;
	}
}

void MxPluginBaseEditableIID::SetGUID(const wchar_t *GUID) 
{ 
	USES_CONVERSION; 
	if (GUID != '\0')
	{
		m_GUID = GUID;
		_GUID = (CW2A(GUID));
	}
	else
	{
		m_GUID = MxPluginBaseEditableIID::Invalid; 
		_GUID = MxPluginBaseEditableIID::Invalid;
	}
}

void MxPluginBaseEditableIID::SetGUID(const char *GUID) 
{ 
	if (GUID != '\0')
	{
		m_GUID = GUID;
		_GUID = GUID;
	}
	else
	{
		m_GUID = MxPluginBaseEditableIID::Invalid; 
		_GUID = MxPluginBaseEditableIID::Invalid;
	}
}


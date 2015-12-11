#include "stdafx.h"
#include "string"

#include "MxUtils\MxUtils1.h"
#include "PluginXml\PluginXmlErrorCodes.h"

#include "PluginXmlAttribute.h"

const char * const PluginXmlAttribute::ArrayPrefixName = "L";
const char * const PluginXmlAttribute::ArraySizeName = "ArraySize";
const char * const PluginXmlAttribute::NameIllegalChars = " <>\"&:";	//<value name="x">Text</value> - chars not allowed in 'name' (see  http://msdn.microsoft.com/en-us/library/ms256152.aspx)
const char * const PluginXmlAttribute::ValueIllegalChars = "<>\"";		//<value name="x">Text</value> - chars not allowed in 'x' - char / entity refs allowed as & is ok

PluginXmlAttribute::PluginXmlAttribute(void):
	_refCnt(0),
	_attribute(nullptr),
	_ownedByParent(false)
{
	_attribute = new TiXmlAttribute();
}

PluginXmlAttribute::PluginXmlAttribute(TiXmlAttribute* attribute):
	_refCnt(0),
	_attribute(attribute),
	_ownedByParent(false)
{

}

PluginXmlAttribute::~PluginXmlAttribute(void)
{
	if (( _attribute != nullptr) && (_ownedByParent == false))
		delete _attribute;
	_attribute = nullptr;
}



long MXSTDMETHOD PluginXmlAttribute::DestroyPtr(void)
{
	long cnt = InterlockedDecrement(&_refCnt);
	if ( cnt == 0 )
		delete this;
	return cnt;
}

long MXSTDMETHOD PluginXmlAttribute::DuplicatePtr(void)
{
	return InterlockedIncrement(&_refCnt);
}

void *MXSTDMETHOD PluginXmlAttribute::Dynamic_cast(const GUID IId)
{
	void *rc = nullptr;

	if ( MxGUID::IsEqual(IId, IPluginXmlAttributeIID::Guid()))
		rc = static_cast<IPluginXmlAttribute *>(this);
	else if ( MxGUID::IsEqual(IId, IMxPluginBaseIID::Guid()))
		rc = static_cast<IMxPluginBase *>(this);
	else
		rc = nullptr;

	if (rc == nullptr)
	{	
		std::string tmp;
		MX_SETERROR(MX6002, MxError::Install, MxError::Abort, MxError::VerboseReport, "ERROR: Dynamic_cast does not support IID=%s", MxGUID::ConvertToString(IId, &tmp));
	}
	else
		((IMxPluginBase *)rc)->DuplicatePtr();

	return rc;
}



const char *MXSTDMETHOD PluginXmlAttribute::GetName()
{
	const char *rc = nullptr;

	if (_attribute == nullptr)
		MX_SETERROR(MX6003, MxError::CodeDefect, MxError::Abort, MxError::VerboseReport, "ERROR: internal data not initialised");
	else
	{
		const char *name = _attribute->Name();
		if (CheckValidName(name) == false)
			MX_SETERROR(MX6004, MxError::CodeDefect, MxError::Abort, MxError::VerboseReport, "ERROR: name=%s is not valid", (name != nullptr) ? name : "(null)");
		else
			rc = name;
	}
	return rc;
}

bool MXSTDMETHOD PluginXmlAttribute::SetName(const char *name)
{
	bool rc = false;

	if (_attribute == nullptr)
		MX_SETERROR(MX6005, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR:  internal data not initialised");
	else
	{
		if (CheckValidName(name) == false)
			MX_SETERROR(MX6006, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: name=%s is not valid", (name != nullptr) ? name : "(null)");
		else
		{
			_attribute->SetName(name);
			rc = true;
		}
	}
	return rc;
}


bool	MXSTDMETHOD PluginXmlAttribute::GetIntValue(int *value)
{
	bool rc = false;

	if (_attribute == nullptr)
		MX_SETERROR(MX6007, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR:  internal data not initialised");
	else
	{
		if (value == nullptr)
			MX_SETERROR(MX6008, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: value parameter is null");
		else
		{
			int valInt = 0;
			if (_attribute->QueryIntValue(&valInt) != TIXML_SUCCESS)
				MX_SETERROR(MX6009, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: QueryIntValue failed");
			else
			{
				*value = valInt;
				rc = true;
			}
		}
	}
	return rc;
}

bool MXSTDMETHOD PluginXmlAttribute::SetIntValue(int value)
{
	bool rc = false;

	if (_attribute == nullptr)
		MX_SETERROR(MX6010, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR:  internal data not initialised");
	else
	{
		_attribute->SetIntValue(value);
		rc = true;
	}
	return rc;
}


const char *MXSTDMETHOD PluginXmlAttribute::GetStringValue()
{
	const char *rc = nullptr;

	if (_attribute == nullptr)
		MX_SETERROR(MX6011, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR:  internal data not initialised");
	else
	{
		const char *value = nullptr;
		if ((value = _attribute->Value()) == nullptr)
			MX_SETERROR(MX6012, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: _attribute->Value() returns null");
		else
		{
			if (CheckValidValue(value) == false)
				MX_SETERROR(MX6013, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: value=%s is not valid", value);
			else
				rc = value;
		}
	}
	return rc;
}

bool MXSTDMETHOD PluginXmlAttribute::SetStringValue(const char *value)
{
	bool rc = false;

	if (_attribute == nullptr)
		MX_SETERROR(MX6014, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		if (value == nullptr)
			MX_SETERROR(MX6015, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: value param is null");
		else
		{
			if (CheckValidValue(value) == false)
				MX_SETERROR(MX6016, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: value=%s is not valid", value);
			else
			{
				_attribute->SetValue(value);
				rc = true;
			}
		}
	}
	return rc;
}

bool MXSTDMETHOD PluginXmlAttribute::GetDoubleValue(double *value)
{
	bool rc = false;

	if (_attribute == nullptr)
		MX_SETERROR(MX6017, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		if (value == nullptr)
			MX_SETERROR(MX6018, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: value param is null");
		else
		{
			double valDouble = 0;
			if (_attribute->QueryDoubleValue(&valDouble) != TIXML_SUCCESS)
				MX_SETERROR(MX6019, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: QueryDoubleValue failed");
			else
			{
				*value = valDouble;
				rc = true;
			}
		}
	}
	return rc;
}

bool MXSTDMETHOD PluginXmlAttribute::SetDoubleValue(double value)
{
	bool rc = false;

	if (_attribute == nullptr)
		MX_SETERROR(MX6020, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		_attribute->SetDoubleValue(value);
		rc = true;
	}
	return rc;
}


IPluginXmlAttribute *	MXSTDMETHOD PluginXmlAttribute::GetNextAttribute()
{
	IPluginXmlAttribute *rc = nullptr;

	if (_attribute == nullptr)
		MX_SETERROR(MX6021, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		TiXmlAttribute *attrib = nullptr;
		if ((attrib = _attribute->Next()) != nullptr)
		{
			if ((rc = new PluginXmlAttribute(attrib)) == nullptr)
				MX_SETERROR(MX6022, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: unable to create implementation of IPluginXmlAttribute");
			else
			{
				rc->OwnedByParent(true);
				rc->DuplicatePtr();
			}
		}
	}
	return rc;
}

IPluginXmlAttribute *	MXSTDMETHOD PluginXmlAttribute::GetPreviousAttribute()
{
	IPluginXmlAttribute *rc = nullptr;

	if (_attribute == nullptr)
		MX_SETERROR(MX6023, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		TiXmlAttribute *attrib = nullptr;
		if ((attrib = _attribute->Previous()) != nullptr)
		{
			if ((rc = new PluginXmlAttribute(attrib)) == nullptr)
				MX_SETERROR(MX6024, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: unable to create implementation of IPluginXmlAttribute");
			else
			{
				rc->OwnedByParent(true);
				rc->DuplicatePtr();
			}
		}
	}
	return rc;
}

bool PluginXmlAttribute::CheckValidName(const char *name)
{
	bool rc = false;

	if (name == nullptr)
		MX_SETERROR(MX6025, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "ERROR: name is null");
	else
	{
		std::string reservedName(name);
		if ( (reservedName.find(PluginXmlAttribute::ArrayPrefixName) == 0) || (reservedName == PluginXmlAttribute::ArraySizeName))
			MX_SETERROR(MX6026, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: name %s is reserved [%s*, %s]", name, PluginXmlAttribute::ArrayPrefixName, PluginXmlAttribute::ArraySizeName);
		else
		{
			if (PluginXmlAttribute::CheckValidXmlString(name, 1, IPluginXmlAttribute::MaxNameLength, PluginXmlAttribute::NameIllegalChars, false) == false)
				MX_SETERROR(MX6027, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: invalid name parameter");
			else
				rc = true;
		}
	}
	return rc;
}

bool PluginXmlAttribute::CheckValidValue(const char *value)
{
	bool rc = false;

	if (value == nullptr)
		MX_SETERROR(MX6028, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "ERROR: value is null");
	else
	{
		if (PluginXmlAttribute::CheckValidXmlString(value, 1, IPluginXmlAttribute::MaxValueLength, PluginXmlAttribute::ValueIllegalChars, true) == false)
			MX_SETERROR(MX6029, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: invalid value parameter");
		else
			rc = true;
	}
	return rc;
}

bool PluginXmlAttribute::CheckValidXmlString(const char *xmlString, int minLen, int maxLen, const char *illegalChars, bool IsText )
{
	bool rc = false;

	if ((xmlString == nullptr) || (maxLen <= 0) || (illegalChars == nullptr))
		MX_SETERROR(MX6030, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "ERROR: xmlString or illegalChars is null or invalid maxLen=%d", maxLen);
	else
	{
		int len = strnlen(xmlString,  maxLen+1);
		if ((len < minLen) || (len > maxLen) )
			MX_SETERROR(MX6031, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: invalid xmlString, length=%d (min=%d, max=%d)", len, minLen,  maxLen);
		else
		{
			if ((IsText == false) && (PluginXmlAttribute::CheckValidPreambleXmlString(xmlString )) == false)
				MX_SETERROR(MX6032, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: invalid xmlString preamble");
			else
			{
				char c = '\0';
				int x = 0;
				for (x = 0; x < len; x++)
				{
					c = xmlString[x];
					const char *illegal = strchr(illegalChars, c);
					if (illegal != '\0')
					{
						MX_SETERROR(MX6033, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: invalid xmlString parameter - illegal char [%c] found at xmlString [%d]", c, x);
						break;
					}
				}
				if (x == len)
					rc = true;
			}
		}
	}
	return rc;
}

bool PluginXmlAttribute::CheckValidPreambleXmlString(const char *xmlString )
{
	bool rc = false;

	if ((isalpha(xmlString[0]) == 0) && (xmlString[0] != '_'))
		MX_SETERROR(MX6034, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: invalid, first char is numeric or punctuation char [%c]", xmlString[0] );
	else
	{
		char buff[] = "XML";
		int x = 0;
		for (x = 0; x < 3; x++ )
		{
			char c = xmlString[x];
			if (isalpha(c) == 0)
				break;
			if (isupper(c) == false)
				c = toupper(c);
			if (c != buff[x])
				break;
		}
		if (x == 3)
			MX_SETERROR(MX6035, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR:invalid, XML found at start of [%s]", xmlString );
		else
			rc = true;
	}
	return rc;
}
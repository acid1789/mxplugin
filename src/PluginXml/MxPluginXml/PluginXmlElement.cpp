#include "stdafx.h"
#include <sstream>

#include "MxUtils\MxUtils1.h"
#include "PluginXml\PluginXmlErrorCodes.h"

#include "PluginXmlElement.h"
#include "PluginXmlAttribute.h"

const char * const PluginXmlElement::ValueIllegalChars = " <>\"&:";	//<value name="x">Text</value> - chars not allowed in 'value' (see  http://msdn.microsoft.com/en-us/library/ms256152.aspx)
const char * const PluginXmlElement::TextIllegalChars = "<>&";		//<value name="x">Text</value> - chars not allowed in 'text' - in some ways we are more restrictive here as text should not contain embedded markup <b> or char/entity refs, but we don't check for control chars / unicode chars 

PluginXmlElement::PluginXmlElement(void):
	_refCnt(0),
	_element(nullptr),
	_ownedByParent(false)
{
	_element = new TiXmlElement("");
}

PluginXmlElement::PluginXmlElement(TiXmlElement *element):
	_refCnt(0),
	_element(element),
	_ownedByParent(false)
{

}

PluginXmlElement::~PluginXmlElement(void)
{
	if (( _element != nullptr) && (_ownedByParent == false))
		delete _element;
	_element = nullptr;
}

long MXSTDMETHOD PluginXmlElement::DestroyPtr(void)
{
	long cnt = InterlockedDecrement(&_refCnt);
	if ( cnt == 0 )
		delete this;
	return cnt;
}

long MXSTDMETHOD PluginXmlElement::DuplicatePtr(void)
{
	return InterlockedIncrement(&_refCnt);
}

void *MXSTDMETHOD PluginXmlElement::Dynamic_cast(const GUID IId)
{
	void *rc = nullptr;

	if ( MxGUID::IsEqual(IId, IPluginXmlElementIID::Guid()))
		rc = static_cast<PluginXmlElement *>(this);
	else if ( MxGUID::IsEqual(IId, IMxPluginBaseIID::Guid()))
		rc = static_cast<IMxPluginBase *>(this);
	else
		rc = nullptr;

	if (rc == nullptr)
	{	
		std::string tmp;
		MX_SETERROR(MX6070, MxError::Install, MxError::Abort, MxError::VerboseReport, "ERROR: Dynamic_cast does not support IID=%s", MxGUID::ConvertToString(IId, &tmp));
	}
	else
		((IMxPluginBase *)rc)->DuplicatePtr();


	return rc;
}

int	MXSTDMETHOD PluginXmlElement::GetAttributeCount()
{
	int rc = 0;

	if (_element == nullptr)
		MX_SETERROR(MX6071, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		bool ContainsArray = (_element->Attribute(PluginXmlAttribute::ArraySizeName) == nullptr) ? false : true;
		TiXmlAttribute *attrib = nullptr;
		if ((attrib = _element->FirstAttribute()) != nullptr)
		{
			if ((ContainsArray == false) || (PluginXmlElement::IsArrayAttribute(attrib) == false))
				rc++;
			while ((attrib = attrib->Next()) != nullptr)
			{
				if ((ContainsArray == false) || (PluginXmlElement::IsArrayAttribute(attrib) == false))
					rc++;
			}
		}
	}
	return rc;
}

IPluginXmlAttribute * MXSTDMETHOD PluginXmlElement::GetAttribute(int index)
{
	IPluginXmlAttribute *rc = nullptr;

	if (_element == nullptr)
		MX_SETERROR(MX6072, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		bool ContainsArray = (_element->Attribute(PluginXmlAttribute::ArraySizeName) == nullptr) ? false : true;
		TiXmlAttribute *attrib = nullptr;
		TiXmlAttribute *nextAttrib = nullptr;
		if ((attrib = _element->FirstAttribute()) != nullptr)
		{
			if ((index == 0) && ((ContainsArray == false) || (PluginXmlElement::IsArrayAttribute(attrib) == false)))
				nextAttrib = attrib;
			else
			{
				int cnt = 0;
				if (index != 0 )
					cnt = 1;
				while ((attrib = attrib->Next()) != nullptr)
				{
					if ((ContainsArray == false) || (PluginXmlElement::IsArrayAttribute(attrib) == false))
					{
						if (cnt == index)
						{
							nextAttrib = attrib;
							break;
						}
						cnt++;
					}
				}
			}
		}
		if (nextAttrib == nullptr)
			MX_SETERROR(MX6073, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: unable to find %d attribute for <%s>", index, _element->Value());
		else
		{
			if ((rc = new PluginXmlAttribute(nextAttrib)) == nullptr)
				MX_SETERROR(MX6074, MxError::Resource, MxError::Abort, MxError::QuietReport, "ERROR: insufficient memory to create new PluginXmlAttribute for %d attribute of <%s>", index, _element->Value());
			else
			{
				rc->OwnedByParent(true);		
				rc->DuplicatePtr();
			}
		}
	}
	return rc;
}

bool		MXSTDMETHOD PluginXmlElement::IsAttributePresent(const char *name, const char *attributeType) //string (ignore type when checking), double, integer
{
	bool rc = false;

	if (_element == nullptr)
		MX_SETERROR(MX6149, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		if (IsValidAttribName(name) == false)
			MX_SETERROR(MX6150, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: invalid name=%s", ((name != nullptr) ? name : "(null)"));
		else
		{
			const char *value = nullptr;
			std::string attribType((attributeType == nullptr) ? "string" : attributeType);
			if (attribType == "double")
			{
				double d = 0;
				if ((value = _element->Attribute(name, &d)) != nullptr)
				{
					if (atof(value) == d)
						rc = true;
				}
			}
			else if (attribType == "integer")
			{
				int x = 0;
				if ((value = _element->Attribute(name, &x)) != nullptr)
				{
					if (atoi(value) == x)
						rc = true;
				}
			}
			else
			{
				if ((value = _element->Attribute(name)) != nullptr)
					rc = true;
			}
		}
	}
	return rc;
}

const char * MXSTDMETHOD PluginXmlElement::GetAttributeString(const char *name)
{
	const char *rc = nullptr;

	if (_element == nullptr)
		MX_SETERROR(MX6075, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		if (IsValidAttribName(name) == false)
			MX_SETERROR(MX6076, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: invalid name=%s", ((name != nullptr) ? name : "(null)"));
		else
		{
			const char *value = nullptr;
			TiXmlAttribute *attrib = nullptr;
			if ((value = _element->Attribute(name)) == nullptr)
				MX_SETERROR(MX6077, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: attribute [%s] of <%s> not found", name, _element->Value());
			else
			{
				rc = value;
			}
		}
	}
	return rc;
}

bool MXSTDMETHOD PluginXmlElement::SetAttributeString(const char *name, const char *value)
{
	bool rc = false;

	if (_element == nullptr)
		MX_SETERROR(MX6078, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");	
	else
	{
		if ((IsValidAttribName(name) == false) || (IsValidAttribValue(value) == false))
			MX_SETERROR(MX6079, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: invalid name=%s or value=%s", ((name != nullptr) ? name : "(null)"), ((value != nullptr) ? value : "(null)"));
		else
		{
			_element->SetAttribute(name,value);
			rc = true;
		}
	}
	return rc;
}

bool	MXSTDMETHOD PluginXmlElement::GetAttributeInt(const char *name, int *value)
{
	bool rc = false;

	if (_element == nullptr)
		MX_SETERROR(MX6080, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");	
	else
	{
		if ((IsValidAttribName(name) == false) || (value == nullptr))
			MX_SETERROR(MX6081, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: invalid name=%s or value=%d", ((name != nullptr) ? name : "(null)"), (value != nullptr) ? "(ptr)" : "(nullptr)");
		else
		{
			TiXmlAttribute *attrib = nullptr;
			int attribVal = 0;
			if ( _element->Attribute(name, &attribVal) == false)
				MX_SETERROR(MX6082, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: attribute [%s] of <%s> not found", name, _element->Value());
			else
			{
				*value = attribVal;
				rc = true;
			}
		}
	}
	return rc;
}

bool MXSTDMETHOD PluginXmlElement::SetAttributeInt(const char *name, int value)
{
	bool rc = false;

	if (_element == nullptr)
		MX_SETERROR(MX6083, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");	
	else
	{
		if (IsValidAttribName(name) == false) 
			MX_SETERROR(MX6084, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: invalid name=%s", ((name != nullptr) ? name : "(null)"));
		else
		{
			_element->SetAttribute(name,value);
			rc = true;
		}
	}
	return rc;
}

bool MXSTDMETHOD PluginXmlElement::GetAttributeDouble(const char *name, double *value)
{
	bool rc = false;

	if (_element == nullptr)
		MX_SETERROR(MX6085, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");	
	else
	{
		if ((IsValidAttribName(name) == false) || (value == nullptr))
			MX_SETERROR(MX6086, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: invalid name=%s or value=%d", ((name != nullptr) ? name : "(null)"), ((value != nullptr) ? "(ptr)" : "(nullptr)"));
		else
		{
			TiXmlAttribute *attrib = nullptr;
			double attribValue = 0.0;
			if ( _element->Attribute(name, &attribValue) == nullptr)
				MX_SETERROR(MX6087, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: attribute [%s] of <%s> not found", name, _element->Value());
			else
			{
				*value = attribValue;
				rc = true;
			}
		}
	}
	return rc;
}

bool MXSTDMETHOD PluginXmlElement::SetAttributeDouble(const char *name, double value)
{
	bool rc = false;

	if (_element == nullptr)
		MX_SETERROR(MX6088, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		if (IsValidAttribName(name) == false) 
			MX_SETERROR(MX6089, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: invalid name=%s ", (name != nullptr) ? name : "(null)");
		else
		{
			std::ostringstream o;
			o << value;
			if (o.str().c_str() == nullptr)
				MX_SETERROR(MX6090, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: invalid value=%g for name=%s ", value, name );
			else
			{
				_element->SetAttribute(name, o.str().c_str());
				rc = true;
			}
		}
	}	

	return rc;
}

bool MXSTDMETHOD PluginXmlElement::RemoveAttributeArray()
{
	bool rc = false;

	if (_element == nullptr)
		MX_SETERROR(MX6091, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		int cnt = IPluginXmlElement::Invalid;
		if ((_element->Attribute(PluginXmlAttribute::ArraySizeName, &cnt) == nullptr) || (cnt == IPluginXmlElement::Invalid))
			MX_SETERROR(MX6092, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: %s not found or invalid cnt=%d", PluginXmlAttribute::ArraySizeName, cnt);
		else
		{
			char buff[11];
			int x = 0;
			for (x = 0; x < cnt; x++ )
			{
				std::string arrayName(PluginXmlAttribute::ArrayPrefixName);
				if (_itoa_s(x, buff, 10, 10) != 0 )
					break;
				arrayName += buff;
				_element->RemoveAttribute(arrayName.c_str());
			}
			if (x != cnt)
				MX_SETERROR(MX6093, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: failed to convert %d to string", x);
			else
				rc = true;

			_element->RemoveAttribute(PluginXmlAttribute::ArraySizeName);
		}
	}
	return rc;

}

bool MXSTDMETHOD PluginXmlElement::HasAttributeArray()
{
	bool rc = false;

	if (_element == nullptr)
		MX_SETERROR(MX6094, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		if (_element->Attribute(PluginXmlAttribute::ArraySizeName) != nullptr)
			rc = true;
	}
	return rc;
}

int	MXSTDMETHOD PluginXmlElement::GetAttributeArraySize()
{
	int rc = IPluginXmlElement::Invalid;

	if (_element == nullptr)
		MX_SETERROR(MX6095, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		int arrayElementCnt = IPluginXmlElement::Invalid;
		if ((_element->Attribute(PluginXmlAttribute::ArraySizeName, &arrayElementCnt) == nullptr) || (arrayElementCnt == IPluginXmlElement::Invalid) || (arrayElementCnt > IPluginXmlElement::MaxArrayLength))
			MX_SETERROR(MX6096, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport,  "ERROR: invalid array - arraySize is %d (max=%d)", arrayElementCnt, IPluginXmlElement::MaxArrayLength );
		else
			rc = arrayElementCnt;
	}
	return rc;
}

int MXSTDMETHOD PluginXmlElement::GetAttributeArray(int *arrayElement[], int arraySize)
{
	int rc = IPluginXmlElement::Invalid;

	if (_element == nullptr)
		MX_SETERROR(MX6097, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		if (arrayElement == nullptr)
			MX_SETERROR(MX6098, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "ERROR: invalid parameter - arrayElement is null");
		else
		{
			int arrayElementCnt = IPluginXmlElement::Invalid;
			if ((_element->Attribute(PluginXmlAttribute::ArraySizeName, &arrayElementCnt) == nullptr) || (arrayElementCnt == IPluginXmlElement::Invalid) || (arrayElementCnt > IPluginXmlElement::MaxArrayLength))
				MX_SETERROR(MX6099, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: invalid array - arraySize is %d (max=%d)", arrayElementCnt, IPluginXmlElement::MaxArrayLength);
			else
			{
				if (arraySize < arrayElementCnt )
					MX_SETERROR(MX6100, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: invalid parameters: arraySize is %d, but %s is %d)", arraySize, PluginXmlAttribute::ArraySizeName, arrayElementCnt );
				else
				{
					int *pArrayElement = arrayElement[0];
					char buff[11];
					int x = 0;
					for (x = 0; x < arrayElementCnt; x++ )
					{
						std::string arrayName(PluginXmlAttribute::ArrayPrefixName);
						if ( _itoa_s(x, buff, 10, 10) != 0 )
							break;
						arrayName += buff;
						_element->Attribute(arrayName.c_str(), pArrayElement+x);
					}
					if (x != arrayElementCnt)
						MX_SETERROR(MX6101, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: failed to convert %d to string", x);
					else
						rc = arrayElementCnt;
				}
			}
		}
	}
	return rc;
}

bool MXSTDMETHOD PluginXmlElement::SetAttributeArray(int *arrayElement[], int arrayElementCnt)
{
	bool rc = false;

	if (_element == nullptr)
		MX_SETERROR(MX6102, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		if (_element->FirstAttribute() != nullptr)
			MX_SETERROR(MX6103, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: there can only be one array attribute and it must be set before any other attributes");
		else
		{
			if ((arrayElementCnt < 1) || (arrayElementCnt > IPluginXmlElement::MaxArrayLength) || (arrayElement == nullptr))
				MX_SETERROR(MX6104, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: invalid parameter - arrayElementCnt=%d (max=%d), arrayElement is %s", arrayElementCnt, IPluginXmlElement::MaxArrayLength, (arrayElement == nullptr) ? "null" : "not null" );
			else
			{
				int *pArrayElement = arrayElement[0];
				char buff[11];
				int x = 0;
				for (x = 0; x < arrayElementCnt; x++ )
				{
					std::string arrayName(PluginXmlAttribute::ArrayPrefixName);
					if (_itoa_s(x, buff, 10, 10) != 0 )
						break;
					arrayName += buff;
					_element->SetAttribute(arrayName.c_str(), *pArrayElement+x);
				}
				_element->SetAttribute(PluginXmlAttribute::ArraySizeName, arrayElementCnt);
				if (x != arrayElementCnt)
					MX_SETERROR(MX6102, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport,  "ERROR: failed to convert %d to string", x);
				else
					rc = true;
			}
		}
	}
	return rc;
}

bool MXSTDMETHOD PluginXmlElement::RemoveAttribute(const char *name)
{
	bool rc = false;

	if (_element == nullptr)
		MX_SETERROR(MX6105, MxError::CodeDefect, MxError::Abort,MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		if (IsValidAttribName(name) == false)
			MX_SETERROR(MX6106, MxError::InvalidParams, MxError::Abort,MxError::QuietReport, "ERROR: invalid name=%s", (name != nullptr) ? name : "(null)" );
		else
		{
			const char *value = nullptr;
			TiXmlAttribute *attrib = nullptr;
			if ((value = _element->Attribute(name)) == nullptr)
				MX_SETERROR(MX6107, MxError::BadUserInput, MxError::Abort,MxError::VerboseReport,  "ERROR: attribute [%s] of <%s> not found", name, _element->Value());
			else
			{
				_element->RemoveAttribute(name);
				rc = true;
			}
		}
	}
	return rc;
}

const char * MXSTDMETHOD PluginXmlElement::GetText()
{
	const char *rc = nullptr;

	if (_element == nullptr)
		MX_SETERROR(MX6108, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		const char *text = nullptr;
		TiXmlNode *child = nullptr;
		for (child = _element->FirstChild(); child != nullptr; child = child->NextSibling())
		{
			if (child->Type() == TiXmlNode::TINYXML_TEXT )
			{
				TiXmlText *txtnode = nullptr;
				if ( (txtnode = child->ToText()) != nullptr)
				{
					text = txtnode->Value();
				}
				break;
			}
		}
		if (IsValidText (text) == false) 
			MX_SETERROR(MX6109, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: TEXT node not found, or its value is null");
		else
			rc = text;
	}
	return rc;
}

bool MXSTDMETHOD PluginXmlElement::SetText(const char *text)
{
	bool rc = false;

	if (_element == nullptr)
		MX_SETERROR(MX6110, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		if (IsValidText (text) == false)
			MX_SETERROR(MX6111, MxError::InvalidParams, MxError::Abort, MxError::VerboseReport, "ERROR:  invalid parameter text=%s", (text != nullptr) ? text : "(null)");
		else
		{
			TiXmlNode *child = nullptr;
			for (child = _element->FirstChild(); child != nullptr; child = child->NextSibling())
			{
				if (child->Type() == TiXmlNode::TINYXML_TEXT )
				{
					TiXmlText *txtnode = nullptr;
					if ( (txtnode = child->ToText()) != nullptr)
					{
						txtnode->SetValue(text);
						rc = true;
					}
					break;
				}
			}
			if ( rc == false) 
			{
				TiXmlText *elementText = new TiXmlText(text);
				if ((_element->LinkEndChild(elementText)) == nullptr)
					MX_SETERROR(MX6112, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: existing TEXT node not found and cannot add new TEXT node <%s>%s<//%s>", _element->Value(), text, _element->Value());
				else
					rc = true;
			}
		}
	}
	return rc;
}

const char *MXSTDMETHOD PluginXmlElement::GetValue()
{
	const char *rc = nullptr;

	if (_element == nullptr)
		MX_SETERROR(MX6113, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		const char *value = _element->Value();
		if (IsValidValue(value) == false)
			MX_SETERROR(MX6114, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: invalid element value=%s", (value != nullptr) ? value : "(null)");
		else
			rc = value;
	}
	return rc;
}

bool MXSTDMETHOD PluginXmlElement::SetValue(const char *value)
{
	bool rc = false;

	if (_element == nullptr)
		MX_SETERROR(MX6115, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");	
	else
	{
		if (IsValidValue(value) == false)
			MX_SETERROR(MX6116, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: invalid element value=%s", (value != nullptr) ? value : "(null)");
		else
		{
			_element->SetValue(value);
			rc = true;
		}
	}
	return rc;
}

bool MXSTDMETHOD PluginXmlElement::IsValidValue(const char *value)
{
	bool rc = false;

	if (PluginXmlAttribute::CheckValidXmlString(value, 1, IPluginXmlElement::MaxValueLength, PluginXmlElement::ValueIllegalChars, false) == false)
		MX_SETERROR(MX6117, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: invalid element value=%s", (value != nullptr) ? value : "(null)");
	else
		rc = true;

	return rc;
}


bool MXSTDMETHOD PluginXmlElement::IsValidText(const char *text)	
{
	bool rc = false;

	if (PluginXmlAttribute::CheckValidXmlString(text, 0, IPluginXmlElement::MaxValueLength, PluginXmlElement::TextIllegalChars , true) == false)
		MX_SETERROR(MX6118, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: invalid element text=%s", (text != nullptr) ? text : "(null)");
	else
		rc = true;

	return rc;
}

int	MXSTDMETHOD PluginXmlElement::GetChildCount()
{
	int rc = 0;

	if (_element == nullptr)
		MX_SETERROR(MX6119, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		TiXmlElement *child = nullptr;
		for (child = _element->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
			rc++;
	}
	return rc;
}

IPluginXmlElement* MXSTDMETHOD PluginXmlElement::GetChild(int index)
{
	IPluginXmlElement* rc = nullptr;

	if (_element == nullptr)
		MX_SETERROR(MX6120, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");	
	else
	{
		if (index < 0)
			MX_SETERROR(MX6121, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "ERROR: invalid parameter value, index is less than zero");
		else
		{
			int cnt = 0;
			TiXmlElement *child = nullptr;
			for (child = _element->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
			{
				if (cnt != index)
					cnt++;
				else
				{
					if ((rc = new PluginXmlElement(child)) == nullptr)
						MX_SETERROR(MX6122, MxError::Resource, MxError::Abort, MxError::QuietReport, "ERROR: unable to create new PluginXmlElement for index %d", index);
					else
					{
						rc->OwnedByParent(true);
						rc->DuplicatePtr();
					}
					break;
				}
			}
			if (rc == nullptr)
				MX_SETERROR(MX6123, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport,  "ERROR: unable to find node[%d]", index);
		}
	}
	return rc;
}


int	MXSTDMETHOD PluginXmlElement::GetSiblingCount()
{
	int rc = 0;

	if (_element == nullptr)
		MX_SETERROR(MX6124, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");	
	else
	{
		TiXmlNode *parent = nullptr;
		if ( (parent = _element->Parent()) == nullptr)	//root node has no siblings so returns 0, for all other nodes rc = number of sibling elements
			MX_LOGMSG(MxError::VerboseReport, "ERROR: <%s> has no parent - root node returns 0", _element->Value());
		else
		{
			TiXmlElement *child = nullptr;
			for (child = parent->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
				rc++;
		}
	}
	return rc;
}

IPluginXmlElement* MXSTDMETHOD PluginXmlElement::GetSibling(int index)
{
	IPluginXmlElement* rc = nullptr;

	if (_element == nullptr)
		MX_SETERROR(MX6125, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		if (index < 0 )
			MX_SETERROR(MX6125, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "ERROR: invalid parameter value, index is less than zero");
		else
		{
			TiXmlNode *parent = nullptr;
			if ( (parent = _element->Parent()) == nullptr)	//root node has no siblings so returns 0, for all other nodes rc = number of sibling elements
				MX_LOGMSG(MxError::VerboseReport, "ERROR: <%s> has no parent - root node returns 0", _element->Value());
			else
			{
				int cnt = 0;
				TiXmlElement *child = nullptr;
				for (child = parent->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
				{
					if (cnt != index)
						cnt++;
					else
					{
						if ((rc = new PluginXmlElement(child)) == nullptr)
							MX_SETERROR(MX6126, MxError::Resource, MxError::Abort, MxError::QuietReport, "ERROR: not enough memory to create new sibling node[%d] - child <%s>", index, child->Value());
						else
						{
							rc->OwnedByParent(true);
							rc->DuplicatePtr();
						}
						break;
					}
				}
				if (rc == nullptr)
					MX_LOGMSG(MxError::VerboseReport, "ERROR: unable to find sibling[%d]", index);
			}
		}
	}
	return rc;
}

IPluginXmlElement* MXSTDMETHOD PluginXmlElement::GetParent()
{
	IPluginXmlElement* rc = nullptr;

	if (_element == nullptr)
		MX_SETERROR(MX6128, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		TiXmlNode *parent = nullptr;
		for ( parent = _element->Parent(); parent != nullptr; parent = parent->Parent())
		{
			if (parent->Type() == TiXmlNode::TINYXML_ELEMENT )
			{
				if ((rc = new PluginXmlElement(static_cast<TiXmlElement *>(parent))) == nullptr)
					MX_SETERROR(MX6129, MxError::Resource, MxError::Abort, MxError::QuietReport, "ERROR: not enough memory to create new parent of <%s>",  _element->Value());
				else
				{
					rc->OwnedByParent(true);		//set OwnedByParent(true) for any IPluginXmlElement with Internal data linked to another TiXmlNode - i.e. the parent TiXmlNode is responsible for deleting the internal data    
					rc->DuplicatePtr();
				}
				break;
			}
		}
		if (rc == nullptr)
			MX_LOGMSG(MxError::VerboseReport, "ERROR: unable to find parent element of <%s>",  _element->Value());
	}
	return rc;
}

IPluginXmlElement* MXSTDMETHOD PluginXmlElement::GetFirstChild(const char *value)
{
	IPluginXmlElement* rc = nullptr;

	if (_element == nullptr)
		MX_SETERROR(MX6131, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		TiXmlElement *child = nullptr;
		for (child = _element->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
		{
			if ((value == nullptr) || (strncmp(child->Value(), value, IPluginXmlElement::MaxValueLength) == 0))
			{
				if ((rc = new PluginXmlElement(child)) == nullptr)
					MX_SETERROR(MX6132, MxError::Resource, MxError::Abort, MxError::QuietReport, "ERROR: not enough memory to create new PluginXmlElement for child <%s>", child->Value());
				else
				{
					rc->OwnedByParent(true);		
					rc->DuplicatePtr();
				}
				break;
			}
		}
		if ( rc == nullptr)
			MX_LOGMSG(MxError::VerboseReport, "ERROR: first child <%s> of <%s> not found", (value != nullptr) ? value : "null", _element->Value());
	}
	return rc;
}

IPluginXmlElement* MXSTDMETHOD PluginXmlElement::GetNextSibling(const char *value)
{
	IPluginXmlElement* rc = nullptr;

	if (_element == nullptr)
		MX_SETERROR(MX6134, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");	
	else
	{
		TiXmlElement *sibling = nullptr;
		for (sibling = _element->NextSiblingElement(); sibling != nullptr; sibling = sibling->NextSiblingElement())
		{
			if ((value == nullptr) || (strncmp(sibling->Value(), value, IPluginXmlElement::MaxValueLength) == 0))
			{
				if ((rc = new PluginXmlElement(sibling)) == nullptr)
					MX_SETERROR(MX6130, MxError::Resource, MxError::Abort, MxError::QuietReport, "ERROR: not enough memory to create new PluginXmlElement for sibling <%s>", sibling->Value());
				else
				{
					rc->OwnedByParent(true);		
					rc->DuplicatePtr();
				}
				break;
			}
		}
		if ( rc == nullptr)
			MX_LOGMSG(MxError::VerboseReport, "ERROR: next sibling <%s> of <%s> not found", (value != nullptr) ? value : "null", _element->Value());
	}
	return rc;
}

IPluginXmlElement* MXSTDMETHOD PluginXmlElement::GetPreviousSibling(const char *value)
{
	IPluginXmlElement* rc = nullptr;

	if (_element == nullptr)
		MX_SETERROR(MX6127, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		TiXmlNode *sibling = nullptr;
		for ( sibling = _element->PreviousSibling(); sibling != nullptr; sibling = sibling->PreviousSibling())
		{
			if (sibling->Type() == TiXmlNode::TINYXML_ELEMENT )
			{
				if ((value == nullptr) || (strncmp(sibling->Value(), value, IPluginXmlElement::MaxValueLength) == 0))
				{
					if ((rc = new PluginXmlElement(static_cast<TiXmlElement *>(sibling))) == nullptr)
						MX_SETERROR(MX6135, MxError::Resource, MxError::Abort, MxError::QuietReport, "ERROR: not enough memory to create new PluginXmlElement for sibling <%s>", sibling->Value());
					else
					{
						rc->OwnedByParent(true);		
						rc->DuplicatePtr();
					}
					break;
				}
			}
		}
		if ( rc == nullptr)
			MX_LOGMSG(MxError::VerboseReport, "ERROR: previous sibling <%s> of <%s> not found", (value != nullptr) ? value : "null", _element->Value());
	}
	return rc;
}

bool MXSTDMETHOD PluginXmlElement::LinkEndChild( IPluginXmlElement* element)
{
	bool rc = false;

	if (_element == nullptr)
		MX_SETERROR(MX6136, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		if (element == nullptr)
			MX_SETERROR(MX6137, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "ERROR: invalid parameter value, element is null");
		else
		{
			TiXmlElement *item = nullptr;
			if ((item = reinterpret_cast<TiXmlElement *>(element->GetInternalData())) == nullptr)
				MX_SETERROR(MX6138, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data of passed parameter is null");
			else
			{
				if ((_element->LinkEndChild(item)) == nullptr)
					MX_SETERROR(MX6139, MxError::Resource, MxError::Abort, MxError::QuietReport, "ERROR: unable to link <%s> as end child of <%s>", item->Value(), _element->Value());
				else
				{
					element->OwnedByParent(true);	
					rc = true;
				}
			}
		}
	}
	return rc;
}

bool MXSTDMETHOD PluginXmlElement::RemoveChild( IPluginXmlElement* element)
{
	bool rc = false;

	if (_element == nullptr)
		MX_SETERROR(MX6140, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		if (element == nullptr)
			MX_SETERROR(MX6141, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "ERROR: invalid parameter value, element is null");
		else
		{
			TiXmlElement *item = nullptr;
			if ((item = reinterpret_cast<TiXmlElement *>(element->GetInternalData())) == nullptr)
				MX_SETERROR(MX6142, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data of passed parameter is null");
			else
			{
				if (_element->RemoveChild(item) == false)
					MX_SETERROR(MX6143, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: unable to remove internal data");
				else
				{
					rc = true;
				}
			}
		}
	}
	return rc;
}

IPluginXmlElement* MXSTDMETHOD PluginXmlElement::Clone()
{
	IPluginXmlElement* rc = nullptr;

	if (_element == nullptr)
		MX_SETERROR(MX6144, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");	
	else
	{
		TiXmlElement *internalData = nullptr;
		if ((internalData = static_cast<TiXmlElement *>(_element->Clone())) == nullptr)
			MX_SETERROR(MX6145, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: unable to clone internal data");
		else
		{
			if ((rc = new PluginXmlElement(internalData)) == nullptr)
				MX_SETERROR(MX6146, MxError::Resource, MxError::Abort, MxError::QuietReport, "ERROR: not enough memory to create new PluginXmlElement as clone of <%s>", _element->Value());
			else
				rc->DuplicatePtr();
		}
	
	}
	return rc;
}

bool MXSTDMETHOD PluginXmlElement::Clear()
{
	bool rc = false;

	if (_element == nullptr)
		MX_SETERROR(MX6147, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		this->OwnedByParent(false);
		_element->Clear();
		rc = true;
	}
	return rc;
}


bool PluginXmlElement::IsArrayAttribute(TiXmlAttribute *attrib)
{
	bool rc = false;

	if (attrib == nullptr)
		MX_SETERROR(MX6148, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "ERROR: attrib parameter is null");
	else
	{
		std::string name(attrib->Name());
		if ((name.find(PluginXmlAttribute::ArrayPrefixName) == 0) || (name == PluginXmlAttribute::ArraySizeName))
			rc = true;
	}
	return rc;
}


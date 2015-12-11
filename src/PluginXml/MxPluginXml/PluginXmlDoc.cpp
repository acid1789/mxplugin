#include "stdafx.h"

#include "MxUtils\MxUtils1.h"
#include "PluginXml\PluginXmlErrorCodes.h"

#include "PluginXml.h"
#include "PluginXmlDoc.h"

PluginXmlDoc::PluginXmlDoc(void) :
	_refCnt(0),
	_error(true),
	_doc(nullptr),
	_root(nullptr)
{
}

PluginXmlDoc::~PluginXmlDoc(void)
{
	Close();
}

long MXSTDMETHOD PluginXmlDoc::DestroyPtr(void)
{
	long cnt = InterlockedDecrement(&_refCnt);
	if ( cnt == 0 )
		delete this;
	return cnt;
}

long MXSTDMETHOD PluginXmlDoc::DuplicatePtr(void)
{
	return InterlockedIncrement(&_refCnt);
}

void *MXSTDMETHOD PluginXmlDoc::Dynamic_cast(const GUID IId)
{
	void *rc = nullptr;

	if ( MxGUID::IsEqual(IId, IPluginXmlDocIID::Guid()))
		rc = static_cast<IPluginXmlDoc *>(this);
	else if ( MxGUID::IsEqual(IId, IMxPluginBaseIID::Guid()))
		rc = static_cast<IMxPluginBase *>(this);
	else
		rc = nullptr;

	if (rc == nullptr)
	{	
		std::string tmp;
		MX_SETERROR(MX6041, MxError::Install, MxError::Abort, MxError::VerboseReport, "ERROR: Dynamic_cast does not support IID=%s", MxGUID::ConvertToString(IId, &tmp));
	}
	else
		((IMxPluginBase *)rc)->DuplicatePtr();

	return rc;
}

bool MXSTDMETHOD PluginXmlDoc::Validate(const char *xmlSchemaPathFilename)
{
	_error = true;

	if (xmlSchemaPathFilename != nullptr)
		MX_SETERROR(MX6042, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: validation that your document conforms to the xmlSchema file is not currently supported - set xmlSchemaPathFilename to null and try again");
	else
	{
		if (_doc == nullptr)
			MX_SETERROR(MX6043, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
		else
		{
			if (_doc->Error() == true)
				MX_SETERROR(MX6044, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: _doc->Error() is true; errorid=%d, msg=%s", _doc->ErrorId(), _doc->ErrorDesc());
			else
			{
				if (_root == nullptr)
					MX_SETERROR(MX6045, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: _root is null");
				else
					_error = false;
			}
		}
	}
	return (_error == false) ? true : false;
}

bool MXSTDMETHOD PluginXmlDoc::ClearError(void)
{ 
	bool rc = false;

	if (_doc == nullptr)
		MX_SETERROR(MX6046, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: internal data not initialised");
	else
	{
		_doc->ClearError(); 
		if (Validate() == false)
			MX_SETERROR(MX6047, MxError::BadUserInput, MxError::Abort, MxError::QuietReport, "ERROR: Validate() failed");
		else
			rc = true;
	}
	return rc;
}		

bool MXSTDMETHOD PluginXmlDoc::Close()
{
	bool rc = true;

	if (_root != nullptr)
	{
		long cnt = 0L;
		if ((cnt = _root->DestroyPtr()) > 0)
		{
			MX_SETERROR(MX6048, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: memory leak, existing document still has  %ld _root pointer(s)",  cnt);
			rc = false;
		}
		_root = nullptr;
	}

	if (_doc != nullptr)
	{
		delete _doc;	//and all owned IPluginXmlElement items resulting from LinkEndChild() calls
		_doc = nullptr;
	}
	_error = true;
	return rc;
}

bool MXSTDMETHOD PluginXmlDoc::New(const char *value, const char *comment, const char *version, bool standalone)
{
	bool rc = false;

	if ((value == nullptr) || (strnlen(value, IPluginXmlElement::MaxValueLength+1) > IPluginXmlElement::MaxValueLength) || (comment == nullptr) || (strnlen(comment, IPluginXmlDoc::MaxCommentLength+1) > IPluginXmlDoc::MaxCommentLength) || (version == nullptr) && (strnlen(version, IPluginXmlDoc::MaxVersionLength+1) > IPluginXmlDoc::MaxVersionLength))
		MX_SETERROR(MX6049, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "ERROR: invalid parameters - value, comment or version is null or too long");
	else
	{
		Close();

		if ((_doc = new TiXmlDocument()) == nullptr)
			MX_SETERROR(MX6050, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: new TiXmlDocument() returns null");
		else
		{
			TiXmlDeclaration* decl = new TiXmlDeclaration( version, "", (standalone == true) ? "yes" : "no" );  //yes, no must be lowercase
			_doc->LinkEndChild( decl ); 
			TiXmlComment *commentary = new TiXmlComment(comment);
			_doc->LinkEndChild( commentary );
			TiXmlElement *rootelement = new TiXmlElement(value);
			_doc->LinkEndChild( rootelement );	//~PluginXmlElement deletes rootelement 

			if ((_root = new PluginXmlElement(rootelement)) == nullptr)
				MX_SETERROR(MX6051, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: new PluginXmlElement() returns null");
			else
			{
				_root->OwnedByParent(true);
				_root->DuplicatePtr();
				if (Validate() == false)
					MX_SETERROR(MX6052, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: Validate() failed");
				else
					rc = true;
			}
		}
	}
	return rc;
}

bool MXSTDMETHOD PluginXmlDoc::Open(const char *pathfilename)
{
	bool rc = false;

	if ( MxUtils::IsExistingPathFilename(pathfilename, IPluginXmlDoc::MaxPathFilenameLength) == false)
		MX_SETERROR(MX6053, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: pathfilename parameter invalid or does not exist [%s]", (pathfilename != nullptr) ? pathfilename : "(null)");
	else
	{
		if (New("Doc", "Open - replace with contents of file", "1.0", true) == false)
			MX_SETERROR(MX6054, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: New() failed");
		else
		{
			if (_doc->LoadFile(pathfilename) == false)
				MX_SETERROR(MX6055, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: LoadFile(%s) failed, errorid=%d, msg=%s", pathfilename, _doc->ErrorId(), _doc->ErrorDesc());
			else
			{
				long cnt = 0L;
				if ((cnt = _root->DestroyPtr()) > 0)
					MX_SETERROR(MX6056, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: memory leak, existing document still has  %ld _root pointer(s)",  cnt);
				else
				{
					TiXmlElement *rootelement = nullptr;
					if ((rootelement =_doc->RootElement()) == nullptr)
						MX_SETERROR(MX6057, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: invalid document, no root element found in %s", pathfilename);
					else
					{
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! static_cast<TiXmlElement *>(rootelement->Clone()))
						if ((_root = new PluginXmlElement(rootelement)) == nullptr)
							MX_SETERROR(MX6058, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: new PluginXmlElement() returns null");
						else
						{
							_root->DuplicatePtr();
							_root->OwnedByParent(true);
							_doc->SetValue(pathfilename);
							if (Validate() == false)
								MX_SETERROR(MX6059, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: Validate() failed");
							else
								rc = true;
						}
					}
				}
			}
		}
	}
	return rc;
}

bool MXSTDMETHOD PluginXmlDoc::Save()
{
	bool rc = false;

	if (IsError() == true)
		MX_SETERROR(MX6060, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: IsError() is true");
	else
	{
		if ( MxUtils::IsExistingPathFilename(_doc->Value(), IPluginXmlDoc::MaxPathFilenameLength) == false)
			MX_SETERROR(MX6061, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: _doc->Value() not set by Open or SaveAs to an existing file, [%s]", (_doc->Value() != nullptr) ? _doc->Value() : "(null)");
		else
		{
			if ((rc = _doc->SaveFile()) == false)
				MX_SETERROR(MX6062, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: _doc->SaveFile() failed, errorid=%d, msg=%s", _doc->ErrorId(), _doc->ErrorDesc());
		}
	}
	return rc;
}

bool MXSTDMETHOD PluginXmlDoc::SaveAs(const char *pathfilename)
{
	bool rc = false;

	if ((IsError() == true) || (MxUtils::IsValidPathFilename(pathfilename) == false))
		MX_SETERROR(MX6063, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: IsError() is true or invalid file=%s", ((pathfilename != nullptr) ? pathfilename : "(null)") );
	else
	{
		if ((rc = _doc->SaveFile(pathfilename)) == false)
			MX_SETERROR(MX6064, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: _doc->SaveFile() failed, errorid=%d, msg=%s", _doc->ErrorId(), _doc->ErrorDesc());
		else
			_doc->SetValue(pathfilename);
	}
	return rc;
}

IPluginXmlElement*	MXSTDMETHOD PluginXmlDoc::GetRootElement()
{
	IPluginXmlElement*	rc = nullptr;

	if (IsError() == true)
		MX_SETERROR(MX6065, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: IsError() is true");
	else
	{
		_root->DuplicatePtr();
		rc = _root;
	}
	return rc;
}

const char *MXSTDMETHOD PluginXmlDoc::GetVersion()
{
	const char *rc = nullptr;

	if (IsError() == true)
		MX_SETERROR(MX6066, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: IsError() is true");
	else
	{
		TiXmlDeclaration *dec = nullptr;
		for (TiXmlNode *node = _doc->FirstChild(); node != nullptr; node = node->NextSibling())
		{
			if (node->Type() == TiXmlNode::TINYXML_DECLARATION)
			{
				TiXmlDeclaration *dec = static_cast<TiXmlDeclaration *>(node);
				if (dec != nullptr)
				{
					rc = dec->Version();
					break;
				}
			}
		}
	}
	return rc;
}

const char *MXSTDMETHOD PluginXmlDoc::GetPathFilename()
{
	const char *rc = nullptr;

	if (IsError() == true)
		MX_SETERROR(MX6067, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ERROR: IsError() is true");
	else
	{
		if ( MxUtils::IsExistingPathFilename(_doc->Value(), IPluginXmlDoc::MaxPathFilenameLength) == false)
			MX_SETERROR(MX6068, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "ERROR: _doc->Value() not set by Open or SaveAs to an existing file, [%s]", (_doc->Value() != nullptr) ? _doc->Value() : "(null)");
		else
			rc = _doc->Value();
	}
	return rc;
}



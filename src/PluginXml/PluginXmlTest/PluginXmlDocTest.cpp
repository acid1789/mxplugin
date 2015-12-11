#include "stdafx.h"
#include "CppUnitTest.h"
#include "PluginXml/MxPluginXml/PluginXml.h"
#include "PluginXml/MxPluginXml/PluginXmlDoc.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PluginXmlTest
{
	TEST_CLASS(PluginXmlDocTest)
	{
	private:
		IPluginXmlDoc *_doc;

	public:
		TEST_METHOD_INITIALIZE(InitPluginDocXmlTest)
		{
			Assert::IsNotNull(_doc = static_cast<IPluginXmlDoc *>(::CreateClassInstance(IPluginXmlDocIID::Guid())));

		}

		TEST_METHOD_CLEANUP(DeinitPluginDocXmlTest)
		{
			if (_doc != nullptr)
			{
				Assert::AreEqual(0L, _doc->DestroyPtr());
			}
			_doc = nullptr;
		}
		
		TEST_METHOD(CreatePluginXmlDocTest)
		{
			Assert::IsNotNull(_doc);
			Assert::IsTrue(MxGUID::IsEqual(IPluginXmlDocIID::Guid(), _doc->GetInterfaceGUID()));
			Assert::AreEqual(IPluginXmlDocIID::Name(), _doc->GetInterfaceName());
		}

		TEST_METHOD(IllegalCharTest)
		{
			Assert::AreEqual( PluginXmlElement::ValueIllegalChars, _doc->GetElementValueIllegalChars());	//<value name="x">text</value> - characters not allowed for value
			Assert::AreEqual( PluginXmlElement::TextIllegalChars, _doc->GetElementTextIllegalChars());		//<value name="x">text</value> - characters not allowed for text
			Assert::AreEqual( PluginXmlAttribute::NameIllegalChars, _doc->GetAttribNameIllegalChars());		//<value name="x">text</value> - characters not allowed for name
			Assert::AreEqual( PluginXmlAttribute::ValueIllegalChars, _doc->GetAttribValueIllegalChars());	//<value name="xyz">text</value> - characters not allowed for xyz
		}

		TEST_METHOD(ClearErrorTest)
		{
			Assert::IsTrue(_doc->New("Doc", "Unit Test", "1.1", true));
			Assert::IsTrue(_doc->ClearError());
			Assert::IsTrue(_doc->Close());
		}

		TEST_METHOD(GetErrorTest)
		{
			Assert::IsTrue(_doc->New("Doc", "Unit Test","1.1", true));
			Assert::AreEqual(0, _doc->GetErrorId());
			Assert::AreEqual(1, _doc->GetErrorCol());
			Assert::AreEqual(1, _doc->GetErrorRow());
			Assert::AreEqual("", _doc->GetErrorDescr());
			Assert::IsTrue(_doc->Close());
		}

		TEST_METHOD(SaveAsDocumentTest)
		{
			Assert::IsTrue(_doc->New("Doc", "Unit Test","1.1", true));
			Assert::IsTrue(_doc->SaveAs("test.xml"));
			Assert::IsTrue(_doc->Save());
			Assert::IsTrue(_doc->Close());
		}

		TEST_METHOD(OpenDocumentTest)
		{
			Assert::IsTrue(_doc->New("Doc", "Unit Test","1.1", true));
			Assert::IsFalse(_doc->Validate("schemaNotSupportedYet"));
			Assert::IsTrue(_doc->Validate());
			Assert::IsTrue(_doc->SaveAs("test.xml"));

			Assert::IsTrue(_doc->Open("test.xml"));
			Assert::IsTrue(_doc->Close());
		}

		TEST_METHOD(GetRootTest)
		{
			IPluginXmlElement *root = nullptr;

			Assert::IsTrue(_doc->New("Doc", "Unit Test","1.1", true));
			Assert::IsNotNull(root = _doc->GetRootElement());
			Assert::AreEqual(1L, root->DestroyPtr());
			Assert::IsTrue(_doc->Close());
		}

		TEST_METHOD(GetPathFilenameTest)
		{
			Assert::IsTrue(_doc->New("Doc", "Unit Test","1.1", true));
			Assert::IsTrue(_doc->SaveAs("test.xml"));

			Assert::AreEqual("test.xml", _doc->GetPathFilename());

			Assert::IsTrue(_doc->Close());
			Assert::IsNull(_doc->GetPathFilename());
		}

		TEST_METHOD(GetVersionTest)
		{
			Assert::IsTrue(_doc->New("Doc", "Unit Test", "1.2", true));
			Assert::AreEqual("1.2", _doc->GetVersion());

			Assert::IsTrue(_doc->SaveAs("test.xml"));
			Assert::IsTrue(_doc->Close());
			Assert::IsNull(_doc->GetVersion());

			Assert::IsTrue(_doc->Open("test.xml"));
			Assert::AreEqual("1.2", _doc->GetVersion());
			Assert::IsTrue(_doc->Close());

		}

		TEST_METHOD(MultipleDocumentTests)
		{
			Assert::IsTrue(_doc->New("Doc", "Unit Test", "1.1", true));
			Assert::IsTrue(_doc->SaveAs("test.xml"));

			IPluginXmlDoc *doc2 = static_cast<IPluginXmlDoc *>(::CreateClassInstance(IPluginXmlDocIID::Guid()));
			Assert::IsTrue(doc2->New("Doc", "Unit Test", "1.2", true));
			Assert::IsTrue(doc2->SaveAs("test2.xml"));

			Assert::AreEqual("test.xml", _doc->GetPathFilename());
			Assert::AreEqual("test2.xml", doc2->GetPathFilename());

			Assert::IsTrue(_doc->Close());
			Assert::IsTrue(doc2->Close());

			Assert::AreEqual(0L, doc2->DestroyPtr());
		}

		TEST_METHOD(OpenDocumentNotExistTest)
		{
			Assert::IsFalse(_doc->Open("???"));
			Assert::IsTrue(_doc->Close());
		}

		TEST_METHOD(OpenDocumentNullTest)
		{
			Assert::IsFalse(_doc->Open(nullptr));
			Assert::IsTrue(_doc->Close());
		}

		TEST_METHOD(SaveAsDocumentNullTest)
		{
			Assert::IsTrue(_doc->New("Doc", "Unit Test", "1.1", true));
			Assert::IsFalse(_doc->SaveAs(nullptr));
			Assert::IsTrue(_doc->Close());
		}

		TEST_METHOD(SaveDocumentNotOpenTest)
		{
			Assert::IsFalse(_doc->Save());
			Assert::IsTrue(_doc->Close());
		}

		TEST_METHOD(GetPathFilenameNotSetTest)
		{
			Assert::IsTrue(_doc->New("Doc", "Unit Test", "1.1", true));

			Assert::IsNull(_doc->GetPathFilename());

			Assert::IsTrue(_doc->Close());
		}

	};
}
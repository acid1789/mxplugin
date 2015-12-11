#include "stdafx.h"
#include "CppUnitTest.h"
#include "PluginXml/MxPluginXml/PluginXml.h"
#include "PluginXml/MxPluginXml/PluginXmlDoc.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PluginXmlTest
{
	TEST_CLASS(PluginXmlSaveLoadTest)
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

		TEST_METHOD(SaveLoadTest)
		{
					//create new
			Assert::IsTrue(_doc->New("Doc", "Unit Test","1.1", true));
			IPluginXmlElement *root = nullptr;
			Assert::IsNotNull(root = _doc->GetRootElement());

			IPluginXmlElement *body = static_cast<IPluginXmlElement *>(::CreateClassInstance(IPluginXmlElementIID::Guid()));;
			Assert::IsNotNull(body);
			Assert::IsTrue(body->SetText("text"));
			Assert::IsTrue(body->SetValue("value"));
			Assert::IsTrue(root->LinkEndChild(body));

			Assert::IsTrue(_doc->SaveAs("saveloadtest.xml"));
	
			Assert::AreEqual(0L, body->DestroyPtr());
			body = nullptr;
			Assert::AreEqual(1L, root->DestroyPtr());
			root = nullptr;
			Assert::IsTrue(_doc->Close());

				//open and change
			Assert::IsTrue(_doc->Open("saveloadtest.xml"));
			Assert::IsNotNull(root = _doc->GetRootElement());
			Assert::IsNotNull(body = root->GetFirstChild());

			Assert::AreEqual("text", body->GetText());
			Assert::AreEqual("value", body->GetValue());

			Assert::IsTrue(body->SetText("xxxxx"));

			Assert::IsTrue(_doc->SaveAs("saveloadtest.xml"));

			Assert::AreEqual(0L, body->DestroyPtr());
			body = nullptr;
			Assert::AreEqual(1L, root->DestroyPtr());
			root = nullptr;
			Assert::IsTrue(_doc->Close());

				//reopen and check
			Assert::IsTrue(_doc->Open("saveloadtest.xml"));
			Assert::IsNotNull(root = _doc->GetRootElement());
			Assert::IsNotNull(body = root->GetFirstChild());

			Assert::AreEqual("xxxxx", body->GetText());
			Assert::AreEqual("value", body->GetValue());

			Assert::AreEqual(0L, body->DestroyPtr());
			body = nullptr;
			Assert::AreEqual(1L, root->DestroyPtr());
			root = nullptr;
			Assert::IsTrue(_doc->Close());

		}

	};
}
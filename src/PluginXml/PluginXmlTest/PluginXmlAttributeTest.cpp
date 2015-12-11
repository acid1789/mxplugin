#include "stdafx.h"
#include "CppUnitTest.h"
#include "PluginXml/MxPluginXml/PluginXml.h"
#include "PluginXml/MxPluginXml/PluginXmlDoc.h"
#include "PluginXml/MxPluginXml/PluginXmlAttribute.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PluginXmlTest
{
	TEST_CLASS(PluginXmlAttributeTest)
	{
	private:
		IPluginXmlElement *_element;
		IPluginXmlAttribute *_attribA;

	public:
		TEST_METHOD_INITIALIZE(InitPluginXmlAttributeTest)
		{
			Assert::IsNotNull(_element = static_cast<IPluginXmlElement *>(::CreateClassInstance(IPluginXmlElementIID::Guid())));
			Assert::IsNotNull(_attribA = static_cast<IPluginXmlAttribute *>(::CreateClassInstance(IPluginXmlAttributeIID::Guid())));
		}

		TEST_METHOD_CLEANUP(DeinitPluginElementXmlTest)
		{
			if (_element != nullptr)
				Assert::AreEqual(0L, _element->DestroyPtr());
			if (_attribA != nullptr)
				Assert::AreEqual(0L, _attribA->DestroyPtr());
		}

		TEST_METHOD(CreateAttributeTest)
		{
			IPluginXmlAttribute *attrib = static_cast<IPluginXmlAttribute *>(::CreateClassInstance(IPluginXmlAttributeIID::Guid()));
			Assert::IsNotNull(attrib);
			Assert::IsTrue(MxGUID::IsEqual(IPluginXmlAttributeIID::Guid(), attrib->GetInterfaceGUID()));
			Assert::AreEqual(IPluginXmlAttributeIID::Name(), attrib->GetInterfaceName());
			Assert::IsFalse(attrib->IsOwnedByParent());

			Assert::AreEqual(0L, attrib->DestroyPtr());	//Is NOT OwnedByParent, so this deletes internal data
		}

		TEST_METHOD(SetGetNameTest)
		{
			IPluginXmlAttribute *attrib = static_cast<IPluginXmlAttribute *>(::CreateClassInstance(IPluginXmlAttributeIID::Guid()));
			Assert::IsNotNull(attrib);
			Assert::IsFalse(attrib->IsOwnedByParent());

			Assert::IsTrue(attrib->SetName("testMe"));
			Assert::AreEqual("testMe", attrib->GetName());

			Assert::IsFalse(attrib->SetName("xml"));	//xml is an invalid name
			Assert::IsFalse(attrib->SetName(""));		//empty is an invalid name
			Assert::IsFalse(attrib->SetName(nullptr));		//null is an invalid name

			Assert::AreEqual("testMe", attrib->GetName());

			Assert::AreEqual(0L, attrib->DestroyPtr());	//Is NOT OwnedByParent, so this deletes internal data
		}


		TEST_METHOD(SetGetIntegerTest)
		{
			IPluginXmlAttribute *attrib = static_cast<IPluginXmlAttribute *>(::CreateClassInstance(IPluginXmlAttributeIID::Guid()));
			Assert::IsNotNull(attrib);
			Assert::IsFalse(attrib->IsOwnedByParent());

			int value = 0;
			Assert::IsTrue(attrib->SetIntValue(-1));
			Assert::IsTrue(attrib->GetIntValue(&value));
			Assert::AreEqual(-1, value);
			Assert::IsTrue(attrib->SetIntValue(0));
			Assert::IsTrue(attrib->GetIntValue(&value));
			Assert::AreEqual(0, value);
			Assert::IsTrue(attrib->SetIntValue(1));
			Assert::IsTrue(attrib->GetIntValue(&value));
			Assert::AreEqual(1, value);

			Assert::IsTrue(attrib->SetIntValue(999));
			Assert::IsTrue(attrib->GetIntValue(&value));
			Assert::AreEqual(999, value);

			Assert::IsTrue(attrib->SetStringValue("999"));
			Assert::IsTrue(attrib->GetIntValue(&value));
			Assert::AreEqual(999, value);

			Assert::IsTrue(attrib->SetStringValue("a999"));
			Assert::IsFalse(attrib->GetIntValue(&value));

			Assert::AreEqual(0L, attrib->DestroyPtr());	//Is NOT OwnedByParent, so this deletes internal data
		}

		TEST_METHOD(SetGetDoubleTest)
		{
			IPluginXmlAttribute *attrib = static_cast<IPluginXmlAttribute *>(::CreateClassInstance(IPluginXmlAttributeIID::Guid()));
			Assert::IsNotNull(attrib);
			Assert::IsFalse(attrib->IsOwnedByParent());

			double value = 0.0;
			Assert::IsTrue(attrib->SetDoubleValue(-0.1));
			Assert::IsTrue(attrib->GetDoubleValue(&value));
			Assert::AreEqual(-0.1, value);
			Assert::IsTrue(attrib->SetDoubleValue(0.0));
			Assert::IsTrue(attrib->GetDoubleValue(&value));
			Assert::AreEqual(0.0, value);
			Assert::IsTrue(attrib->SetDoubleValue(0.1));
			Assert::IsTrue(attrib->GetDoubleValue(&value));
			Assert::AreEqual(0.1, value);

			Assert::IsTrue(attrib->SetDoubleValue(999.999));
			Assert::IsTrue(attrib->GetDoubleValue(&value));
			Assert::AreEqual(999.999, value);

			Assert::IsTrue(attrib->SetDoubleValue(1.9E2));
			Assert::IsTrue(attrib->GetDoubleValue(&value));
			Assert::AreEqual(190.0, value);
			Assert::IsTrue(attrib->SetDoubleValue(1.9E-2));
			Assert::IsTrue(attrib->GetDoubleValue(&value));
			Assert::AreEqual(0.019, value);

			Assert::IsTrue(attrib->SetStringValue("9999.99"));
			Assert::IsTrue(attrib->GetDoubleValue(&value));
			Assert::AreEqual(9999.99, value);

			Assert::IsTrue(attrib->SetStringValue("999999"));
			Assert::IsTrue(attrib->GetDoubleValue(&value));
			Assert::AreEqual(999999.0, value);

			Assert::IsTrue(attrib->SetStringValue("a999"));
			Assert::IsFalse(attrib->GetDoubleValue(&value));

			Assert::AreEqual(0L, attrib->DestroyPtr());	//Is NOT OwnedByParent, so this deletes internal data
		}

		TEST_METHOD(SetGetStringTest)
		{
			IPluginXmlAttribute *attrib = static_cast<IPluginXmlAttribute *>(::CreateClassInstance(IPluginXmlAttributeIID::Guid()));
			Assert::IsNotNull(attrib);
			Assert::IsFalse(attrib->IsOwnedByParent());

			Assert::IsTrue(attrib->SetStringValue("a999999"));
			Assert::AreEqual("a999999", attrib->GetStringValue());

			Assert::IsTrue(attrib->SetStringValue("999999"));		//integer
			Assert::AreEqual("999999", attrib->GetStringValue());

			Assert::IsTrue(attrib->SetStringValue("999e3"));		//double
			Assert::AreEqual("999e3", attrib->GetStringValue());

			Assert::IsTrue(attrib->SetStringValue("999.3"));		//double
			Assert::AreEqual("999.3", attrib->GetStringValue());

			Assert::IsFalse(attrib->SetStringValue(nullptr));			//null

			char *temp = (char *)malloc(IPluginXmlAttribute::MaxValueLength+1);
			Assert::IsNotNull(temp);
			if (temp != nullptr)
			{
				for (int x = 0; x < IPluginXmlAttribute::MaxValueLength+1; x++)
					*(temp+x) = 'a';
				Assert::IsFalse(attrib->SetStringValue(temp));			//too long
				free(temp);
			}
			Assert::AreEqual(0L, attrib->DestroyPtr());	//Is NOT OwnedByParent, so this deletes internal data
		}

		TEST_METHOD(IterateAttributeTest)
		{
			Assert::IsTrue( _element->SetAttributeString("ItemA", "001"));
			Assert::IsTrue( _element->SetAttributeString("ItemB", "002"));
			Assert::IsTrue( _element->SetAttributeString("ItemC", "003"));
			Assert::IsTrue( _element->SetAttributeString("ItemD", "004"));
			Assert::IsTrue( _element->SetAttributeString("ItemE", "005"));

			Assert::AreEqual(5, _element->GetAttributeCount());

			Assert::AreEqual("001", _element->GetAttribute(0)->GetStringValue());
			Assert::AreEqual("002", _element->GetAttribute(1)->GetStringValue());
			Assert::AreEqual("003", _element->GetAttribute(2)->GetStringValue());
			Assert::AreEqual("004", _element->GetAttribute(3)->GetStringValue());
			Assert::AreEqual("005", _element->GetAttribute(4)->GetStringValue());

		}

		TEST_METHOD(GetNextPreviousAttributeTest)
		{
			Assert::IsTrue( _element->SetAttributeString("ItemA", "001"));
			Assert::IsTrue( _element->SetAttributeString("ItemB", "002"));
			Assert::IsTrue( _element->SetAttributeString("ItemC", "003"));

			Assert::AreEqual(3, _element->GetAttributeCount());

			IPluginXmlAttribute *attrib = nullptr;
			IPluginXmlAttribute *attribA = nullptr;
			IPluginXmlAttribute *attribB = nullptr;
			IPluginXmlAttribute *attribC = nullptr;

			Assert::IsNotNull(attrib = _element->GetAttribute(0));

			Assert::IsNull(attrib->GetPreviousAttribute());
			Assert::IsNotNull(attribB = attrib->GetNextAttribute());
			Assert::AreEqual("002", attribB->GetStringValue());

			Assert::IsNotNull(attribA = attribB->GetPreviousAttribute());
			Assert::AreEqual("001", attribA->GetStringValue());

			Assert::IsNotNull(attribC = attribB->GetNextAttribute());
			Assert::AreEqual("003", attribC->GetStringValue());
			Assert::IsNull(attribC->GetNextAttribute());

			Assert::IsTrue(attribA->IsOwnedByParent()); 
			Assert::AreEqual(0L, attribA->DestroyPtr());	//Is OwnedByParent, so internal data is deleted when _element->DestroyPtr() is invoked in TEST_METHOD_CLEANUP
			attribA = nullptr;
			Assert::IsTrue(attribB->IsOwnedByParent()); 
			Assert::AreEqual(0L, attribB->DestroyPtr());	//Is OwnedByParent, so internal data is deleted when _element->DestroyPtr() is invoked in TEST_METHOD_CLEANUP
			attribB = nullptr;
			Assert::IsTrue(attribC->IsOwnedByParent()); 
			Assert::AreEqual(0L, attribC->DestroyPtr());	//Is OwnedByParent, so internal data is deleted when _element->DestroyPtr() is invoked in TEST_METHOD_CLEANUP
			attribC = nullptr;
			Assert::IsTrue(attrib->IsOwnedByParent()); 
			Assert::AreEqual(0L, attrib->DestroyPtr());		//Is OwnedByParent, so internal data is deleted when _element->DestroyPtr() is invoked in TEST_METHOD_CLEANUP
			attrib = nullptr;
		}


		TEST_METHOD(CheckValidNameTest)
		{
			Assert::IsTrue( PluginXmlAttribute::CheckValidName("bye"));
			Assert::IsTrue( PluginXmlAttribute::CheckValidName("hell.o"));		
			Assert::IsTrue( PluginXmlAttribute::CheckValidName("hell9o"));
			Assert::IsTrue( PluginXmlAttribute::CheckValidName("hell_o"));	
			Assert::IsTrue( PluginXmlAttribute::CheckValidName("hell-o"));	

			Assert::IsFalse( PluginXmlAttribute::CheckValidName(nullptr));			//null

			Assert::IsFalse( PluginXmlAttribute::CheckValidName(PluginXmlAttribute::ArraySizeName));	//reserved name
			Assert::IsFalse( PluginXmlAttribute::CheckValidName(PluginXmlAttribute::ArrayPrefixName));	//reserved name

			std::string arrayElementName("");
			arrayElementName = PluginXmlAttribute::ArrayPrefixName;
			arrayElementName += "0";
			Assert::IsFalse( PluginXmlAttribute::CheckValidName(arrayElementName.c_str()));	//reserved name
			arrayElementName = PluginXmlAttribute::ArrayPrefixName;
			arrayElementName += "999";
			Assert::IsFalse( PluginXmlAttribute::CheckValidName(arrayElementName.c_str()));	//reserved name
			
			char *temp = (char *)malloc(IPluginXmlAttribute::MaxValueLength+1);
			Assert::IsNotNull(temp);
			if (temp != nullptr)
			{
				for (int x = 0; x < IPluginXmlAttribute::MaxValueLength+1; x++)
					*(temp+x) = 'a';
				Assert::IsFalse( PluginXmlAttribute::CheckValidName(temp));			//too long
				free(temp);
			}
			Assert::IsFalse( PluginXmlAttribute::CheckValidName(""));			//too short

			Assert::IsFalse( PluginXmlAttribute::CheckValidName("1hell"));		//first char is not alphabetic
			Assert::IsFalse( PluginXmlAttribute::CheckValidName("[hell"));		//first char is not alphabetic
			Assert::IsFalse( PluginXmlAttribute::CheckValidName("@hell"));		//first char is not alphabetic

			Assert::IsFalse( PluginXmlAttribute::CheckValidName("XMLhell"));	//starts with Xml
			Assert::IsFalse(PluginXmlAttribute::CheckValidName("xmlhell"));		//starts with Xml
			Assert::IsFalse(PluginXmlAttribute::CheckValidName("xMLhell"));		//starts with Xml

			Assert::IsFalse( PluginXmlAttribute::CheckValidName("hell o"));		//contains illegal char
			Assert::IsFalse( PluginXmlAttribute::CheckValidName("hell<o"));		//contains illegal char
			Assert::IsFalse( PluginXmlAttribute::CheckValidName("hell>o"));		//contains illegal char
			Assert::IsFalse( PluginXmlAttribute::CheckValidName("hell\"o"));	//contains illegal char
			Assert::IsFalse( PluginXmlAttribute::CheckValidName("hell&o"));		//contains illegal char
			Assert::IsFalse( PluginXmlAttribute::CheckValidName("hell:o"));		//contains illegal char

		}

		TEST_METHOD(CheckValidValueTest)
		{
			Assert::IsTrue( PluginXmlAttribute::CheckValidValue("bye"));
			Assert::IsTrue( PluginXmlAttribute::CheckValidValue("hell.o"));								
			Assert::IsTrue( PluginXmlAttribute::CheckValidValue("hell:o"));		
			Assert::IsTrue( PluginXmlAttribute::CheckValidValue("hell&o"));		

			Assert::IsTrue( PluginXmlAttribute::CheckValidValue(PluginXmlAttribute::ArraySizeName));	//reserved name, but ok for value
			Assert::IsTrue( PluginXmlAttribute::CheckValidValue(PluginXmlAttribute::ArrayPrefixName));	//reserved name, but ok for value
			Assert::IsTrue( PluginXmlAttribute::CheckValidValue("xml1234"));							//reserved name, but ok for value
			Assert::IsTrue( PluginXmlAttribute::CheckValidValue("1234"));								//reserved name, but ok for value
			Assert::IsTrue( PluginXmlAttribute::CheckValidValue("@234"));								//reserved name, but ok for value

			std::string arrayElementName("");
			arrayElementName = PluginXmlAttribute::ArrayPrefixName;
			arrayElementName += "0";
			Assert::IsTrue( PluginXmlAttribute::CheckValidValue(arrayElementName.c_str()));	//reserved name, but ok for value
			arrayElementName = PluginXmlAttribute::ArrayPrefixName;
			arrayElementName += "999";
			Assert::IsTrue( PluginXmlAttribute::CheckValidValue(arrayElementName.c_str()));	//reserved name, but ok for value

			Assert::IsTrue( PluginXmlAttribute::CheckValidValue("hell o"));			//contains invalid char for name, but ok for value

			Assert::IsFalse( PluginXmlAttribute::CheckValidValue(nullptr));			//null

			Assert::IsFalse( PluginXmlAttribute::CheckValidValue("hell<o"));		//contains illegal char
			Assert::IsFalse( PluginXmlAttribute::CheckValidValue("hell>o"));		//contains illegal char
			Assert::IsFalse( PluginXmlAttribute::CheckValidValue("hell\"o"));		//contains illegal char
		}

		TEST_METHOD(CheckValidXmlStringTest)
		{
			Assert::IsTrue( PluginXmlAttribute::CheckValidXmlString("bye", 1, 3, "acd", true ));
			Assert::IsTrue( PluginXmlAttribute::CheckValidXmlString("bye", 0, 10, "acd", true ));
			Assert::IsTrue( PluginXmlAttribute::CheckValidXmlString("", 0, 3, "acd", true ));	
			Assert::IsTrue( PluginXmlAttribute::CheckValidXmlString("bye", 1, 3, "acd", false ));
			Assert::IsTrue( PluginXmlAttribute::CheckValidXmlString("x", 0, 3, "acd", false ));				//if not text, then first char must be alphabetic
			Assert::IsTrue( PluginXmlAttribute::CheckValidXmlString("jXMLhell", 0, 10, "acd", false ));		//if not text, then cannot start with Xml

			Assert::IsFalse( PluginXmlAttribute::CheckValidXmlString(nullptr, 1, 10, "acd", true ));		//null
			Assert::IsFalse( PluginXmlAttribute::CheckValidXmlString("", 0, 10, nullptr, true ));			//no illegal chars
			Assert::IsFalse( PluginXmlAttribute::CheckValidXmlString("bye", 1, 2, "acd", true ));		//too many chars
			Assert::IsFalse( PluginXmlAttribute::CheckValidXmlString("bye", 4, 3, "acd", true ));		//too few chars
			Assert::IsFalse( PluginXmlAttribute::CheckValidXmlString("XMLhell", 0, 10, "acd", false ));	//if not text, then cannot start with Xml
		}

	};
}
#include "stdafx.h"
#include "CppUnitTest.h"
#include "PluginXml/MxPluginXml/PluginXml.h"
#include "PluginXml/MxPluginXml/PluginXmlDoc.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PluginXmlTest
{
	TEST_CLASS(PluginXmlElementTest)
	{
	private:
		IPluginXmlDoc *_doc;
		IPluginXmlElement *_element;
		IPluginXmlElement *_childA;
		IPluginXmlElement *_childB;
		IPluginXmlElement *_childC;
		IPluginXmlElement *_childD;
	public:
		TEST_METHOD_INITIALIZE(InitPluginXmlElementTest)
		{
			Assert::IsNotNull(_doc = static_cast<IPluginXmlDoc *>(::CreateClassInstance(IPluginXmlDocIID::Guid())));
			Assert::IsNotNull(_element = static_cast<IPluginXmlElement *>(::CreateClassInstance(IPluginXmlElementIID::Guid())));
			Assert::IsNotNull(_childA = static_cast<IPluginXmlElement *>(::CreateClassInstance(IPluginXmlElementIID::Guid())));
			Assert::IsTrue(_childA->SetValue("childA"));
			Assert::IsTrue(_childA->SetText("AAA"));
			Assert::IsNotNull(_childB = static_cast<IPluginXmlElement *>(::CreateClassInstance(IPluginXmlElementIID::Guid())));
			Assert::IsTrue(_childB->SetValue("childB"));
			Assert::IsTrue(_childB->SetText("BBB"));
			Assert::IsNotNull(_childC = static_cast<IPluginXmlElement *>(::CreateClassInstance(IPluginXmlElementIID::Guid())));
			Assert::IsTrue(_childC->SetValue("childC"));
			Assert::IsTrue(_childC->SetText("CCC"));
			Assert::IsNotNull(_childD = static_cast<IPluginXmlElement *>(::CreateClassInstance(IPluginXmlElementIID::Guid())));
			Assert::IsTrue(_childD->SetValue("childB"));
			Assert::IsTrue(_childD->SetText("bbb"));
		}

		TEST_METHOD_CLEANUP(DeinitPluginElementXmlTest)
		{
			if (_doc != nullptr)
				Assert::AreEqual(0L, _doc->DestroyPtr());
			_doc = nullptr;
			if (_element != nullptr)
				Assert::AreEqual(0L, _element->DestroyPtr());
			_element = nullptr;
			if (_childA != nullptr)
				Assert::AreEqual(0L, _childA->DestroyPtr());	
			if (_childB != nullptr)
				Assert::AreEqual(0L, _childB->DestroyPtr());
			if (_childC != nullptr)
				Assert::AreEqual(0L, _childC->DestroyPtr());
			if (_childD != nullptr)
				Assert::AreEqual(0L, _childD->DestroyPtr());
		}
		
		TEST_METHOD(CreatePluginXmlElementTest)
		{
			Assert::IsNotNull(_doc);
			Assert::IsTrue(MxGUID::IsEqual(IPluginXmlDocIID::Guid(), _doc->GetInterfaceGUID()));
			Assert::AreEqual(IPluginXmlDocIID::Name(), _doc->GetInterfaceName());

			Assert::IsNotNull(_element);
			Assert::IsTrue(MxGUID::IsEqual(IPluginXmlElementIID::Guid(), _element->GetInterfaceGUID()));
			Assert::AreEqual(IPluginXmlElementIID::Name(), _element->GetInterfaceName());

			Assert::IsNotNull(_childA);
			Assert::IsNotNull(_childB);
			Assert::IsNotNull(_childC);

		}

		TEST_METHOD(CreateElementTest)
		{
			IPluginXmlElement *child = static_cast<IPluginXmlElement *>(::CreateClassInstance(IPluginXmlElementIID::Guid()));
			Assert::IsNotNull(child);
			Assert::IsFalse(child->IsOwnedByParent());

			Assert::AreEqual(0L, child->DestroyPtr());	//Is NOT OwnedByParent, so this deletes internal data
		}

		TEST_METHOD(SetGetTextTest)
		{
			Assert::IsNull(_element->GetText());					//not set

			Assert::IsTrue( _element->SetText("hello world!"));		//typical string - success
			Assert::AreEqual("hello world!", _element->GetText());

			Assert::IsTrue( _element->SetText(""));					//empty string - success
			Assert::AreEqual("", _element->GetText());

			Assert::IsFalse( _element->SetText(nullptr));				

		}

		TEST_METHOD(IsValidTextTest)
		{
			Assert::IsTrue( _element->IsValidText("hello"));	
			Assert::IsTrue( _element->IsValidText("he.llo"));	
			Assert::IsTrue( _element->IsValidText("he:llo"));	

			Assert::IsFalse( _element->IsValidText(nullptr));			//null

			char *temp = (char *)malloc(IPluginXmlElement::MaxTextLength+1);
			Assert::IsNotNull(temp);
			if (temp != nullptr)
			{
				for (int x = 0; x < IPluginXmlElement::MaxTextLength+1; x++)
					*(temp+x) = 'a';
				Assert::IsFalse( _element->IsValidText(temp));		//too long
				free(temp);
			}

			Assert::IsFalse( _element->IsValidText("hell<o"));		//contains illegal char
			Assert::IsFalse( _element->IsValidText("hell>o"));		//contains illegal char
			Assert::IsFalse( _element->IsValidText("hell&o"));		//contains illegal char
		}

		TEST_METHOD(SetGetValueTest)
		{
			Assert::IsNull(_element->GetValue());					//not set

			Assert::IsTrue( _element->SetValue("hello"));			//typical string - success
			Assert::AreEqual( "hello", _element->GetValue());

			Assert::IsFalse( _element->SetValue(""));			
			Assert::IsFalse( _element->SetValue(nullptr));	
		}


		TEST_METHOD(IsValidValueTest)
		{
			Assert::IsTrue( _element->IsValidValue("b"));
			Assert::IsTrue( _element->IsValidValue("by.e"));

			Assert::IsFalse( _element->IsValidValue(nullptr));			//null
			Assert::IsFalse( _element->IsValidValue(""));			//empty

			char *temp = (char *)malloc(IPluginXmlElement::MaxValueLength+1);
			Assert::IsNotNull(temp);
			if (temp != nullptr)
			{
				for (int x = 0; x < IPluginXmlElement::MaxValueLength+1; x++)
					*(temp+x) = 'a';
				Assert::IsFalse( _element->IsValidValue(temp));			//too long
				free(temp);
			}

			Assert::IsFalse( _element->IsValidValue("1hell"));		//first char is not alphabetic
			Assert::IsFalse( _element->IsValidValue("[hell"));		//first char is not alphabetic
			Assert::IsFalse( _element->IsValidValue("@hell"));		//first char is not alphabetic

			Assert::IsFalse( _element->IsValidValue("XMLhell"));	//starts with Xml
			Assert::IsFalse( _element->IsValidValue("xmlhell"));	//starts with Xml
			Assert::IsFalse( _element->IsValidValue("xMLhell"));	//starts with Xml

			Assert::IsFalse( _element->IsValidValue("hell o"));		//contains space
			Assert::IsFalse( _element->IsValidValue("hell<o"));		//contains illegal char
			Assert::IsFalse( _element->IsValidValue("hell>o"));		//contains illegal char
			Assert::IsFalse( _element->IsValidValue("hell\"o"));	//contains illegal char
			Assert::IsFalse( _element->IsValidValue("hell&o"));		//contains illegal char
			Assert::IsFalse( _element->IsValidValue("hell:o"));		//contains illegal char

		}

		TEST_METHOD(GetAttributeCountTest)
		{
			Assert::AreEqual(0, _element->GetAttributeCount());
			Assert::IsTrue(_element->SetAttributeString("attribA", "000"));
			Assert::AreEqual(1, _element->GetAttributeCount());
			Assert::IsTrue(_element->SetAttributeString("attribB", "001"));
			Assert::AreEqual(2, _element->GetAttributeCount());
			Assert::IsTrue(_element->SetAttributeString("attribC", "002"));
			Assert::AreEqual(3, _element->GetAttributeCount());

		}

		TEST_METHOD(IsAttributePresentTest)
		{
			Assert::IsTrue(_element->SetAttributeString("attribStr", "aaa"));
			Assert::IsTrue(_element->SetAttributeDouble("attribDouble", 1.9));
			Assert::IsTrue(_element->SetAttributeInt("attribInteger", 4));

			Assert::AreEqual(3, _element->GetAttributeCount());

			Assert::IsTrue(_element->IsAttributePresent("attribStr", nullptr));
			Assert::IsTrue(_element->IsAttributePresent("attribStr", "string"));
			Assert::IsFalse(_element->IsAttributePresent("attribStr", "integer"));
			Assert::IsFalse(_element->IsAttributePresent("attribStr", "double"));

			Assert::IsTrue(_element->IsAttributePresent("attribInteger", "integer"));
			Assert::IsTrue(_element->IsAttributePresent("attribDouble", "double"));

		}

		TEST_METHOD(SetGetAttributeTest)
		{
			Assert::AreEqual(0, _element->GetAttributeCount());

			Assert::IsTrue(_element->SetAttributeString("attribA", "000"));
			Assert::AreEqual(1, _element->GetAttributeCount());
			Assert::IsTrue(_element->SetAttributeString("attribB", "001"));
			Assert::AreEqual(2, _element->GetAttributeCount());
			Assert::IsTrue(_element->SetAttributeString("attribC", "002"));
			Assert::AreEqual(3, _element->GetAttributeCount());

			IPluginXmlAttribute *attrib = nullptr;
			Assert::IsNotNull(attrib = _element->GetAttribute(0));
			Assert::AreEqual("000", attrib->GetStringValue());
			Assert::AreEqual(0L, attrib->DestroyPtr());
			attrib = nullptr;
			Assert::IsNotNull(attrib = _element->GetAttribute(1));
			Assert::AreEqual("001", attrib->GetStringValue());
			Assert::AreEqual(0L, attrib->DestroyPtr());
			attrib = nullptr;
			Assert::IsNotNull(attrib = _element->GetAttribute(2));
			Assert::AreEqual("002", attrib->GetStringValue());
			Assert::AreEqual(0L, attrib->DestroyPtr());
			attrib = nullptr;

			Assert::IsNull(attrib = _element->GetAttribute(3));
			Assert::IsNull(attrib = _element->GetAttribute(-1));
		}

		TEST_METHOD(RemoveAttributeTest)
		{
			Assert::AreEqual(0, _element->GetAttributeCount());
			Assert::IsFalse(_element->RemoveAttribute(nullptr));
			Assert::IsFalse(_element->RemoveAttribute("attribA"));

			Assert::IsTrue(_element->SetAttributeString("attribA", "000"));
			Assert::AreEqual(1, _element->GetAttributeCount());
			Assert::IsTrue(_element->SetAttributeString("attribB", "001"));
			Assert::AreEqual(2, _element->GetAttributeCount());
			Assert::IsTrue(_element->SetAttributeString("attribC", "002"));
			Assert::AreEqual(3, _element->GetAttributeCount());

			IPluginXmlAttribute *attrib = nullptr;

			Assert::IsNotNull(attrib = _element->GetAttribute(0));
			//Assert::AreEqual("000", attrib1->GetStringValue());

			Assert::IsTrue(_element->RemoveAttribute("attribA"));
//*** IMPORTANT: having deleted an attribute of _element, you must now destroy any pointers to it 
//				 There isn't any obvious way to make _element responsible for this task without it maintaining 
//				 a list, but that in itself creates a host of other problems. Therefore best to let the developer
//				 decide how this will be done.
			Assert::AreEqual(0L, attrib->DestroyPtr());
			attrib = nullptr;
			Assert::AreEqual(2, _element->GetAttributeCount());
			Assert::IsNull(_element->GetAttributeString("attribA"));

			Assert::IsNotNull(attrib = _element->GetAttribute(0));
			//Assert::AreEqual("001", attrib1->GetStringValue());
			Assert::IsTrue(_element->RemoveAttribute("attribB"));
			Assert::AreEqual(0L, attrib->DestroyPtr());
			attrib = nullptr;
			Assert::AreEqual(1, _element->GetAttributeCount());
			Assert::IsNull(_element->GetAttributeString("attribB"));

			Assert::IsNotNull(attrib = _element->GetAttribute(0));
			//Assert::AreEqual("002", attrib1->GetStringValue());
			Assert::IsTrue(_element->RemoveAttribute("attribC"));
			Assert::AreEqual(0L, attrib->DestroyPtr());
			attrib = nullptr;
			Assert::AreEqual(0, _element->GetAttributeCount());
			Assert::IsNull(_element->GetAttributeString("attribC"));

			Assert::IsFalse(_element->RemoveAttribute("attribA"));
		}

		TEST_METHOD(SetGetAttributeStringTest)
		{
			Assert::IsTrue(_element->SetAttributeString("a000", "a999999"));
			Assert::AreEqual("a999999", _element->GetAttributeString("a000"));

			Assert::IsTrue(_element->SetAttributeString("a000", "999999"));		//integer
			Assert::AreEqual("999999",  _element->GetAttributeString("a000"));

			Assert::IsTrue(_element->SetAttributeString("a000", "999e3"));		//double
			Assert::AreEqual("999e3",  _element->GetAttributeString("a000"));

			Assert::IsTrue(_element->SetAttributeString("a000", "999.3"));		//double
			Assert::AreEqual("999.3",  _element->GetAttributeString("a000"));

			Assert::IsFalse(_element->SetAttributeString("a000", nullptr));		//null
			Assert::IsFalse(_element->SetAttributeString(nullptr, "a000"));		//null
			Assert::IsFalse(_element->SetAttributeString(nullptr, nullptr));			//null

			char *temp = (char *)malloc(IPluginXmlAttribute::MaxValueLength+1);
			Assert::IsNotNull(temp);
			if (temp != nullptr)
			{
				for (int x = 0; x < IPluginXmlAttribute::MaxValueLength+1; x++)
					*(temp+x) = 'a';
				Assert::IsFalse(_element->SetAttributeString("a000",temp));			//too long
				free(temp);
			}
		}

		TEST_METHOD(SetGetAttributeIntTest)
		{
			int value = 0;
			Assert::IsTrue(_element->SetAttributeInt("a000", -1));
			Assert::IsTrue(_element->GetAttributeInt("a000", &value));
			Assert::AreEqual(-1, value);
			Assert::IsTrue(_element->SetAttributeInt("a000", 0));
			Assert::IsTrue(_element->GetAttributeInt("a000", &value));
			Assert::AreEqual(0, value);
			Assert::IsTrue(_element->SetAttributeInt("a000", 1));
			Assert::IsTrue(_element->GetAttributeInt("a000", &value));
			Assert::AreEqual(1, value);

			Assert::IsTrue(_element->SetAttributeInt("a000", 999));
			Assert::IsTrue(_element->GetAttributeInt("a000", &value));
			Assert::AreEqual(999, value);

			Assert::IsTrue(_element->SetAttributeString("a000", "999"));
			Assert::IsTrue(_element->GetAttributeInt("a000", &value));
			Assert::AreEqual(999, value);

			Assert::IsTrue(_element->SetAttributeString("a000", "a999"));
			Assert::IsFalse(_element->GetAttributeInt("a000", &value));
		}

		TEST_METHOD(SetGetAttributeDoubleTest)
		{
			double value = 0.0;
			Assert::IsTrue(_element->SetAttributeDouble("a000", -0.1));
			Assert::IsTrue(_element->GetAttributeDouble("a000", &value));
			Assert::AreEqual(-0.1, value);
			Assert::IsTrue(_element->SetAttributeDouble("a000", 0.0));
			Assert::IsTrue(_element->GetAttributeDouble("a000", &value));
			Assert::AreEqual(0.0, value);
			Assert::IsTrue(_element->SetAttributeDouble("a000", 0.1));
			Assert::IsTrue(_element->GetAttributeDouble("a000", &value));
			Assert::AreEqual(0.1, value);

			Assert::IsTrue(_element->SetAttributeDouble("a000", 999.999));
			Assert::IsTrue(_element->GetAttributeDouble("a000", &value));
			Assert::AreEqual(999.999, value);

			Assert::IsTrue(_element->SetAttributeDouble("a000", 1.9E2));
			Assert::IsTrue(_element->GetAttributeDouble("a000", &value));
			Assert::AreEqual(190.0, value);
			Assert::IsTrue(_element->SetAttributeDouble("a000", 1.9E-2));
			Assert::IsTrue(_element->GetAttributeDouble("a000", &value));
			Assert::AreEqual(0.019, value);

			Assert::IsTrue(_element->SetAttributeString("a000", "9999.99"));
			Assert::IsTrue(_element->GetAttributeDouble("a000", &value));
			Assert::AreEqual(9999.99, value);

			Assert::IsTrue(_element->SetAttributeString("a000", "999999"));
			Assert::IsTrue(_element->GetAttributeDouble("a000", &value));
			Assert::AreEqual(999999.0, value);

			Assert::IsTrue(_element->SetAttributeString("a000", "a999"));
			Assert::IsFalse(_element->GetAttributeDouble("a000", &value));

			Assert::IsFalse(_element->SetAttributeDouble(nullptr, 0.0));		//null

		}

		TEST_METHOD(AttributeArraySimpleTest)
		{
			int testSize = 10;
			int *arrayInts = nullptr;
			arrayInts = (int *)malloc(testSize*sizeof(int));
			Assert::IsNotNull(arrayInts);
			if (arrayInts != nullptr)
			{
				for (int x = 0; x < testSize; x++)
					*(arrayInts+x) = x;

				Assert::AreEqual(0, _element->GetAttributeCount());
				Assert::IsFalse(_element->HasAttributeArray());
				Assert::IsTrue(_element->SetAttributeArray(&arrayInts, testSize));
				Assert::IsTrue(_element->HasAttributeArray());
				Assert::AreEqual(testSize, _element->GetAttributeArraySize());
				Assert::AreEqual(0, _element->GetAttributeCount());

				free(arrayInts);
			}
			int cnt = _element->GetAttributeArraySize();
			arrayInts = (int *)malloc((cnt)*sizeof(int));
			Assert::IsNotNull(arrayInts);
			if (arrayInts != nullptr)
			{
				for (int x = 0; x < cnt; x++)
					*(arrayInts+x) = 0;

				Assert::AreEqual(testSize, _element->GetAttributeArray(&arrayInts, cnt));

				free(arrayInts);
			}

			Assert::IsTrue(_element->RemoveAttributeArray());
			Assert::IsFalse(_element->HasAttributeArray());
		}

		TEST_METHOD(AttributeArrayInitTest)
		{
			int testSize = IPluginXmlElement::MaxArrayLength+1;
			int *arrayInts = nullptr;
			arrayInts = (int *)malloc(testSize*sizeof(int));
			Assert::IsNotNull(arrayInts);
			if (arrayInts != nullptr)
			{
				for (int x = 0; x < testSize; x++)
					*(arrayInts+x) = x;

				Assert::IsFalse(_element->HasAttributeArray());
				Assert::AreEqual(IPluginXmlElement::Invalid, _element->GetAttributeArraySize());
				Assert::IsFalse(_element->RemoveAttributeArray());
				Assert::AreEqual(IPluginXmlElement::Invalid, _element->GetAttributeArray(&arrayInts, testSize));

				Assert::IsFalse(_element->SetAttributeArray(&arrayInts, 0));
				Assert::IsFalse(_element->SetAttributeArray(nullptr, testSize));
				Assert::IsFalse(_element->SetAttributeArray(nullptr, 0));
				Assert::IsFalse(_element->SetAttributeArray(&arrayInts, testSize));

				Assert::IsTrue(_element->SetAttributeArray(&arrayInts, 100));		//it does work with testSize-1, but takes 3sec to run!
				Assert::IsFalse(_element->SetAttributeArray(&arrayInts, 100));		//can only set one array attribute for each element
				Assert::IsTrue(_element->RemoveAttributeArray());

				Assert::IsTrue(_element->SetAttributeDouble("a000", 999.999));
				Assert::IsFalse(_element->SetAttributeArray(&arrayInts, 100));		//must set array attribute before any other attribute types
				Assert::IsTrue(_element->RemoveAttribute("a000"));
				Assert::AreEqual(0, _element->GetAttributeCount());

				Assert::IsTrue(_element->SetAttributeArray(&arrayInts, 100));
				Assert::IsTrue(_element->SetAttributeDouble("a000", 999.999));
				Assert::AreEqual(100, _element->GetAttributeArray(&arrayInts, testSize));
				Assert::AreEqual(10, *(arrayInts+10));
				double value = 0.0;
				Assert::IsTrue(_element->GetAttributeDouble("a000", &value));
				Assert::AreEqual(999.999, value);

				if (arrayInts != nullptr)
					free(arrayInts);
			}
		}

		TEST_METHOD(AttributeArrayIterateTest)
		{
			int testSize = 20;
			int *arrayInts = nullptr;
			arrayInts = (int *)malloc(testSize*sizeof(int));
			Assert::IsNotNull(arrayInts);
			if (arrayInts != nullptr)
			{
				for (int x = 0; x < testSize; x++)
					*(arrayInts+x) = x;

				Assert::AreEqual(0, _element->GetAttributeCount());
				double value = 0.0;
				Assert::IsTrue(_element->SetAttributeArray(&arrayInts, testSize));
				Assert::IsNull(_element->GetAttribute(0));
				Assert::AreEqual(0, _element->GetAttributeCount());
				Assert::IsTrue(_element->SetAttributeDouble("a000", 999.999));
				Assert::AreEqual(1, _element->GetAttributeCount());
				Assert::IsTrue(_element->GetAttributeDouble("a000", &value));
				Assert::AreEqual(999.999, value);

				IPluginXmlAttribute *attrib1 = nullptr;
				Assert::IsNotNull(attrib1 = _element->GetAttribute(0));
				Assert::IsTrue(attrib1->GetDoubleValue( &value));
				Assert::AreEqual(999.999, value);

				Assert::IsTrue(_element->SetAttributeInt("b000", 999));
				Assert::AreEqual(2, _element->GetAttributeCount());

				int valueInt = 0;
				IPluginXmlAttribute *attrib2 = nullptr;
				Assert::IsNotNull(attrib2 = attrib1->GetNextAttribute());
				Assert::IsTrue(attrib2->GetIntValue( &valueInt));
				Assert::AreEqual(999, valueInt);

				Assert::AreEqual(testSize, _element->GetAttributeArray(&arrayInts, testSize));
				Assert::AreEqual(testSize-1, *(arrayInts+testSize-1));

				Assert::AreEqual(0L, attrib1->DestroyPtr());
				attrib1 = nullptr;
				Assert::AreEqual(0L, attrib2->DestroyPtr());
				attrib2 = nullptr;

				free(arrayInts);
			}
		}

		TEST_METHOD(IsValidAttribNameTest)
		{
			Assert::IsTrue(_element->IsValidAttribName("bye"));		//	for other tests see - PluginXmlAttributeTest::TEST_METHOD(CheckValidNameTest)
		}

		TEST_METHOD(IsValidAttribValueTest)
		{
			Assert::IsTrue(_element->IsValidAttribValue("bye"));	//	for other tests see - PluginXmlAttributeTest::TEST_METHOD(CheckValidValueTest)
		}

		TEST_METHOD(GetSiblingTest)
		{
			Assert::AreEqual(0, _element->GetChildCount());
			Assert::IsNull(_element->GetFirstChild());

			Assert::IsTrue(_element->LinkEndChild(_childA));
			Assert::IsTrue(_element->LinkEndChild(_childB));
			Assert::IsTrue(_element->LinkEndChild(_childC));
			Assert::IsTrue(_element->LinkEndChild(_childD));
			Assert::AreEqual(4, _element->GetChildCount());

			IPluginXmlElement *child1 = nullptr;
			IPluginXmlElement *child2 = nullptr;
			IPluginXmlElement *child3 = nullptr;

			Assert::IsNotNull(child1 = _element->GetFirstChild());
			Assert::AreEqual(4, child1->GetSiblingCount());
			Assert::IsNotNull(child2 = child1->GetSibling(1));
			Assert::AreEqual("childB", child2->GetValue());


			Assert::AreEqual(0L, child1->DestroyPtr());
			child1 = nullptr;
			Assert::AreEqual(0L, child2->DestroyPtr());
			child2 = nullptr;

		}

		TEST_METHOD(OwnedByParentTest)
		{
			IPluginXmlElement *child = static_cast<IPluginXmlElement *>(::CreateClassInstance(IPluginXmlElementIID::Guid()));
			Assert::IsNotNull(child);

			Assert::IsFalse(child->IsOwnedByParent());
			child->OwnedByParent(true);
			Assert::IsTrue(child->IsOwnedByParent());
			child->OwnedByParent(false);
			Assert::IsFalse(child->IsOwnedByParent());

			Assert::AreEqual(0L, child->DestroyPtr());
		}

		TEST_METHOD(LinkEndChildTest)
		{
			IPluginXmlElement *child = static_cast<IPluginXmlElement *>(::CreateClassInstance(IPluginXmlElementIID::Guid()));
			Assert::IsNotNull(child);

			Assert::IsTrue(_element->LinkEndChild(child));

			Assert::IsTrue(child->IsOwnedByParent());
			Assert::AreEqual(0L, child->DestroyPtr());		//Is owned by parent, so this doesn't delete internal data - done by _element->DestroyPtr() in DeinitPluginXmlTest
		}

		TEST_METHOD(RemoveChildTest)
		{
			Assert::AreEqual(0, _element->GetChildCount());
			Assert::IsNull(_element->GetChild(0));
			Assert::IsTrue(_element->LinkEndChild(_childA));
			Assert::IsTrue(_element->LinkEndChild(_childB));
			Assert::IsTrue(_element->LinkEndChild(_childC));
			Assert::AreEqual(3, _element->GetChildCount());

			Assert::IsTrue(_element->RemoveChild(_childB));
//*** IMPORTANT: having removed this node from _element, you must now destroy any pointers to it - see ClearTest
			Assert::AreEqual(0L, _childB->DestroyPtr());	
			_childB = nullptr;
			Assert::AreEqual(2, _element->GetChildCount());

			Assert::IsTrue(_element->RemoveChild(_childA));
			Assert::AreEqual(0L, _childA->DestroyPtr());	
			_childA = nullptr;
			Assert::AreEqual(1, _element->GetChildCount());

			Assert::IsTrue(_element->RemoveChild(_childC));
			Assert::AreEqual(0L, _childC->DestroyPtr());	
			_childC = nullptr;
			Assert::AreEqual(0, _element->GetChildCount());

		}

		TEST_METHOD(ClearTest)
		{
			Assert::AreEqual(0, _element->GetChildCount());
			Assert::IsNull(_element->GetChild(0));
			Assert::IsTrue(_element->LinkEndChild(_childA));
			Assert::IsTrue(_element->LinkEndChild(_childB));
			Assert::IsTrue(_element->LinkEndChild(_childC));

			Assert::IsTrue(_childC->LinkEndChild(_childD));

			Assert::AreEqual(3, _element->GetChildCount());
			Assert::AreEqual(1, _childC->GetChildCount());
			Assert::AreEqual(0, _childB->GetChildCount());

			Assert::IsTrue(_element->Clear());
			Assert::AreEqual(0, _element->GetChildCount());

//*** IMPORTANT: having cleared all the nodes of _element, you must now destroy any pointers to them 
//				 There isn't any obvious way to make _element responsible for this task without it maintaining 
//				 a list, but that in itself creates a host of other problems. Therefore best to let the developer
//				 decide how this will be done.

			Assert::AreEqual(0L, _childA->DestroyPtr());	
			_childA = nullptr;
			Assert::AreEqual(0L, _childB->DestroyPtr());
			_childB = nullptr;
			Assert::AreEqual(0L, _childC->DestroyPtr());
			_childC = nullptr;
			Assert::AreEqual(0L, _childD->DestroyPtr());
			_childD = nullptr;
		}

		TEST_METHOD(CloneTest)
		{
			Assert::AreEqual(0, _element->GetChildCount());
			Assert::IsNull(_element->GetChild(0));
			Assert::IsTrue(_element->LinkEndChild(_childA));
			Assert::IsTrue(_element->LinkEndChild(_childB));
			Assert::IsTrue(_element->LinkEndChild(_childC));
			Assert::IsTrue(_childC->LinkEndChild(_childD));
			Assert::AreEqual(3, _element->GetChildCount());
			Assert::AreEqual(1, _childC->GetChildCount());

			IPluginXmlElement *copy = nullptr;
			Assert::IsNotNull( copy = _element->Clone());

			Assert::IsTrue(_element->Clear());
			Assert::AreEqual(0, _element->GetChildCount());
			Assert::AreEqual(0L, _childA->DestroyPtr());	
			_childA = nullptr;
			Assert::AreEqual(0L, _childB->DestroyPtr());
			_childB = nullptr;
			Assert::AreEqual(0L, _childC->DestroyPtr());
			_childC = nullptr;
			Assert::AreEqual(0L, _childD->DestroyPtr());
			_childD = nullptr;

			IPluginXmlElement *child = nullptr; 
			IPluginXmlElement *childC1 = nullptr; 

			Assert::IsNotNull(child =  copy->GetChild(0));
			Assert::AreEqual("childA", child->GetValue());
			Assert::AreEqual("AAA", child->GetText());	
			Assert::AreEqual(0L, child->DestroyPtr());
			child = nullptr;
			Assert::IsNotNull(child = copy->GetChild(1));
			Assert::AreEqual("childB", child->GetValue());
			Assert::AreEqual("BBB", child->GetText());	
			Assert::AreEqual(0L, child->DestroyPtr());
			child = nullptr;
			Assert::IsNotNull(child =  copy->GetChild(2));
			Assert::AreEqual("childC", child->GetValue());
			Assert::AreEqual("CCC", child->GetText());

			Assert::IsNotNull(childC1 = child->GetChild(0));
			Assert::AreEqual("childB", childC1->GetValue());
			Assert::AreEqual("bbb", childC1->GetText());

			Assert::AreEqual(0L, childC1->DestroyPtr());
			childC1 = nullptr;
			Assert::AreEqual(0L, child->DestroyPtr());
			child = nullptr;

			Assert::AreEqual(0L, copy->DestroyPtr());
		}

		TEST_METHOD(GetParentTest)
		{
			Assert::IsTrue(_element->SetValue("qwerty"));
			Assert::IsNull(_childA->GetParent());
			Assert::IsTrue(_element->LinkEndChild(_childA));
			Assert::IsNotNull(_childA->GetParent());
			Assert::AreEqual("qwerty", _childA->GetParent()->GetValue());
		}

		TEST_METHOD(GetChildTest)
		{
			Assert::AreEqual(0, _element->GetChildCount());
			Assert::IsNull(_element->GetChild(0));
			Assert::IsTrue(_element->LinkEndChild(_childA));
			Assert::IsTrue(_element->LinkEndChild(_childB));
			Assert::IsTrue(_element->LinkEndChild(_childC));
			Assert::AreEqual(3, _element->GetChildCount());

			IPluginXmlElement *child = nullptr; 

			Assert::IsNotNull(child = _element->GetChild(0));
			Assert::AreEqual("childA", child->GetValue());
			Assert::AreEqual("AAA", child->GetText());	
			Assert::AreEqual(0L, child->DestroyPtr());
			child = nullptr;

			Assert::IsNotNull(child = _element->GetChild(2));
			Assert::AreEqual("childC", child->GetValue());
			Assert::AreEqual("CCC", child->GetText());	
			Assert::AreEqual(0L, child->DestroyPtr());
			child = nullptr;

			Assert::IsNotNull(child = _element->GetChild(1));
			Assert::AreEqual("childB", child->GetValue());
			Assert::AreEqual("BBB", child->GetText());	
			Assert::AreEqual(0L, child->DestroyPtr());
			child = nullptr;

			Assert::IsNull(_element->GetChild(-1));
			Assert::IsNull(_element->GetChild(3));
		}

		TEST_METHOD(GetFirstChildTest)
		{
			Assert::AreEqual(0, _element->GetChildCount());
			Assert::IsNull(_element->GetFirstChild());

			Assert::IsTrue(_element->LinkEndChild(_childA));
			Assert::IsTrue(_element->LinkEndChild(_childB));
			Assert::IsTrue(_element->LinkEndChild(_childC));
			Assert::AreEqual(3, _element->GetChildCount());

			IPluginXmlElement *child = nullptr;
			Assert::IsNotNull(child = _element->GetFirstChild());
			Assert::AreEqual("childA", child->GetValue());
			Assert::AreEqual("AAA", child->GetText());	
			Assert::AreEqual(0L, child->DestroyPtr());
			child = nullptr;

			Assert::IsNotNull(child = _element->GetFirstChild("childC"));
			Assert::AreEqual("childC", child->GetValue());
			Assert::AreEqual("CCC", child->GetText());	
			Assert::AreEqual(0L, child->DestroyPtr());
			child = nullptr;

			Assert::IsNull(child = _element->GetFirstChild(""));
		}

		TEST_METHOD(GetNextSiblingTest)
		{
			Assert::AreEqual(0, _element->GetChildCount());
			Assert::IsNull(_element->GetFirstChild());

			Assert::IsTrue(_element->LinkEndChild(_childA));
			Assert::IsTrue(_element->LinkEndChild(_childB));
			Assert::IsTrue(_element->LinkEndChild(_childC));
			Assert::AreEqual(3, _element->GetChildCount());

			IPluginXmlElement *child1 = nullptr;
			IPluginXmlElement *child2 = nullptr;

			Assert::IsNotNull(child1 = _element->GetFirstChild());
			Assert::IsNotNull(child2 = child1->GetNextSibling());
			Assert::AreEqual("childB", child2->GetValue());
			Assert::AreEqual("BBB", child2->GetText());
			Assert::AreEqual(0L, child2->DestroyPtr());
			child2 = nullptr;
			Assert::AreEqual(0L, child1->DestroyPtr());
			child1 = nullptr;

			Assert::IsNotNull(child1 = _element->GetFirstChild());
			Assert::IsNotNull(child2 = child1->GetNextSibling("childB"));
			Assert::AreEqual("childB", child2->GetValue());
			Assert::AreEqual("BBB", child2->GetText());
			Assert::AreEqual(0L, child2->DestroyPtr());
			child2 = nullptr;
			Assert::AreEqual(0L, child1->DestroyPtr());
			child1 = nullptr;

			Assert::IsNotNull(child1 = _element->GetFirstChild());
			Assert::IsNotNull(child2 = child1->GetNextSibling("childC"));
			Assert::AreEqual("childC", child2->GetValue());
			Assert::AreEqual("CCC", child2->GetText());
			Assert::AreEqual(0L, child2->DestroyPtr());
			child2 = nullptr;
			Assert::AreEqual(0L, child1->DestroyPtr());
			child1 = nullptr;

			Assert::IsNotNull(child1 = _element->GetChild(2));
			Assert::IsNull(child1->GetNextSibling());
			Assert::AreEqual(0L, child1->DestroyPtr());
			child1 = nullptr;

			Assert::IsNotNull(child1 = _element->GetFirstChild());
			Assert::IsNull(child1->GetNextSibling(""));
			Assert::AreEqual(0L, child1->DestroyPtr());
			child1 = nullptr;

			Assert::IsNotNull(child1 = _element->GetChild(2));
			Assert::IsNull(child1->GetNextSibling("childC"));
			Assert::AreEqual(0L, child1->DestroyPtr());
			child1 = nullptr;
		}

		TEST_METHOD(GetPreviousSiblingTest)
		{
			Assert::AreEqual(0, _element->GetChildCount());
			Assert::IsNull(_element->GetFirstChild());

			Assert::IsTrue(_element->LinkEndChild(_childA));
			Assert::IsTrue(_element->LinkEndChild(_childB));
			Assert::IsTrue(_element->LinkEndChild(_childC));
			Assert::IsTrue(_element->LinkEndChild(_childD));
			Assert::AreEqual(4, _element->GetChildCount());

			IPluginXmlElement *child1 = nullptr;
			IPluginXmlElement *child2 = nullptr;
			IPluginXmlElement *child3 = nullptr;

			Assert::IsNotNull(child1 = _element->GetFirstChild());
			Assert::IsNotNull(child2 = child1->GetNextSibling());
			Assert::AreEqual("childB", child2->GetValue());
			Assert::AreEqual("BBB", child2->GetText());
			Assert::IsNotNull(child3 = child2->GetPreviousSibling());
			Assert::AreEqual("childA", child3->GetValue());
			Assert::AreEqual("AAA", child3->GetText());

			Assert::AreEqual(0L, child3->DestroyPtr());
			child3 = nullptr;
			Assert::AreEqual(0L, child2->DestroyPtr());
			child2 = nullptr;
			Assert::AreEqual(0L, child1->DestroyPtr());
			child1 = nullptr;

			Assert::IsNotNull(child1 = _element->GetChild(2));
			Assert::IsNotNull(child2 = child1->GetPreviousSibling("childB"));
			Assert::AreEqual("childB", child2->GetValue());
			Assert::AreEqual("BBB", child2->GetText());
			Assert::AreEqual(0L, child2->DestroyPtr());
			child2 = nullptr;
			Assert::AreEqual(0L, child1->DestroyPtr());
			child1 = nullptr;

			Assert::IsNotNull(child1 = _element->GetChild(3));
			Assert::IsNotNull(child2 = child1->GetPreviousSibling("childB"));
			Assert::AreEqual("childB", child2->GetValue());
			Assert::AreEqual("BBB", child2->GetText());
			Assert::AreEqual(0L, child2->DestroyPtr());
			child2 = nullptr;
			Assert::AreEqual(0L, child1->DestroyPtr());
			child1 = nullptr;


			Assert::IsNotNull(child1 = _element->GetFirstChild());
			Assert::IsNull(child1->GetPreviousSibling());
			Assert::AreEqual(0L, child1->DestroyPtr());
			child1 = nullptr;

			Assert::IsNotNull(child1 = _element->GetChild(3));
			Assert::IsNull(child1->GetPreviousSibling(""));
			Assert::AreEqual(0L, child1->DestroyPtr());
			child1 = nullptr;

			Assert::IsNotNull(child1 = _element->GetChild(2));
			Assert::IsNull(child1->GetPreviousSibling("childC"));
			Assert::AreEqual(0L, child1->DestroyPtr());
			child1 = nullptr;

		}

		TEST_METHOD(CreateSaveOpenDocTest)
		{
			IPluginXmlElement *root = nullptr;

			Assert::IsTrue(_doc->New("Doc", "Unit Test", "1.1", true));
			Assert::IsNotNull(root = _doc->GetRootElement());

			Assert::IsTrue(_element->SetValue("hello"));
			Assert::IsTrue(root->LinkEndChild(_element));

			Assert::IsTrue(_doc->SaveAs("test.xml"));
			Assert::AreEqual(1L, root->DestroyPtr());
			Assert::IsTrue(_doc->Close());

			Assert::IsTrue(_doc->Open("test.xml"));
			Assert::IsNotNull(root = _doc->GetRootElement());

			Assert::AreEqual(1, root->GetChildCount());
			IPluginXmlElement *hello = nullptr;
			Assert::IsNotNull(hello = root->GetChild(0));
			Assert::AreEqual("hello", hello->GetValue());

			Assert::AreEqual(0L, hello->DestroyPtr());
			Assert::AreEqual(1L, root->DestroyPtr());
			Assert::IsTrue(_doc->Close());
		}

	};
}
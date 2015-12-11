#include "stdafx.h"
#include "CppUnitTest.h"
#include "MxProductID.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MxUtilsTest
{
	TEST_CLASS(MxProductIDTest)
	{
	public:

		TEST_METHOD(ConvertToStringTest)
		{
			std::string temp;
			Assert::AreEqual("{2A3BBF1E-BE54-4573-9407-828B6F7E7FDD}", MxGUID::ConvertToString(PRODID_MxUtils1, &temp));
		}

		TEST_METHOD(ConvertToGUIDTest)
		{
			std::string temp;
			Assert::IsTrue(MxGUID::IsEqual(PRODID_MxUtils1, MxGUID::ConvertFromString("{2A3BBF1E-BE54-4573-9407-828B6F7E7FDD}")));
		}

		TEST_METHOD(IsEqualGUIDTest)
		{
			Assert::IsTrue(MxGUID::IsEqual(PRODID_MxUtils1, PRODID_MxUtils1));
			Assert::IsFalse(MxGUID::IsEqual(PRODID_MxUtils1, PRODID_MxPluginManager1));
		}

		TEST_METHOD(IsValidGUIDTest)
		{
			Assert::IsTrue(MxGUID::IsValid(PRODID_MxUtils1));
			Assert::IsFalse(MxGUID::IsValid(GUID_Invalid));
		}


	};
}
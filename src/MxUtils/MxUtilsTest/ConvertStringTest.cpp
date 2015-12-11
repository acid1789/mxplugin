#include "stdafx.h"
#include "CppUnitTest.h"
#include "MxUtils\MxUtils1.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MxUtilsTest
{
	TEST_CLASS(ConvertStringTest)
	{
	public:

		TEST_METHOD_INITIALIZE(InitConvertStringTest)
		{
			MxError::Inst().Initialise(ProdIdUnitTestApp::Owner(), ProdIdUnitTestApp::ProdID(), ProdIdUnitTestApp::Name(), "v0.0.0.0", MxError::Development, MxError::VerboseReport);
		}

		TEST_METHOD_CLEANUP(DeinitConvertStringTest)
		{
			MxError::Inst().Reset();
		}
		
		TEST_METHOD(ConvertWString2String)
		{
			  std::string text;
	          Assert::IsNull(MxUtils::ConvertWString2String(nullptr,  nullptr));
	          Assert::IsNull(MxUtils::ConvertWString2String(&text, nullptr));
			  Assert::IsNull(MxUtils::ConvertWString2String(nullptr, L"Test"));

			  const char *result = "Test";
	          Assert::IsNotNull(MxUtils::ConvertWString2String(&text, L"Test"));
			  Assert::AreEqual(result, MxUtils::ConvertWString2String( &text, L"Test"));
			  Assert::AreEqual(result, text.c_str());

		}

		TEST_METHOD(ConvertString2WString)
		{
			  std::wstring text;
	          MxUtils::ConvertString2WString(&text, "Test");

	          Assert::IsNull(MxUtils::ConvertString2WString(nullptr,  nullptr));
	          Assert::IsNull(MxUtils::ConvertString2WString(&text, nullptr));
			  Assert::IsNull(MxUtils::ConvertString2WString(nullptr, "Test"));

			  const wchar_t *result = L"Test";
	          Assert::IsNotNull(MxUtils::ConvertString2WString(&text, "Test"));
			  Assert::AreEqual(result, MxUtils::ConvertString2WString(&text, "Test"));
			  Assert::AreEqual(result, text.c_str());
		}

	};
}
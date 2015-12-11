#include "stdafx.h"
#include "CppUnitTest.h"
#include "MxUtils\MxUtils1.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MxUtilsTest
{
	TEST_CLASS(RunConsoleAppTest)
	{
	public:
		
		TEST_METHOD_INITIALIZE(IniRegistryFnTest)
		{
			MxError::Inst().Initialise(ProdIdUnitTestApp::Owner(), ProdIdUnitTestApp::ProdID(), ProdIdUnitTestApp::Name(), "v0.0.0.0", MxError::Development, MxError::VerboseReport);
		}

		TEST_METHOD_CLEANUP(DeinitRegistryFnTest)
		{
			MxError::Inst().Reset();
		}

		TEST_METHOD(RunAppTest)
		{
			Assert::IsFalse( MxUtils::RunConsoleApp(nullptr, nullptr) );
			Assert::IsFalse( MxUtils::RunConsoleApp(L"?", nullptr) );
			Assert::IsTrue( MxUtils::RunConsoleApp(L"logman query MxUtilsTrace", nullptr) );
			std::wstring stdOut(L"");
			Assert::IsTrue( MxUtils::RunConsoleApp(L"logman query MxUtilsTrace", &stdOut) );
			Assert::IsTrue(stdOut.length() > 0 );
		}

		TEST_METHOD(GetLineCntTest)
		{
			Assert::AreEqual(MxUtils::Invalid, MxUtils::GetLineCnt(nullptr));

			Assert::AreEqual(0, MxUtils::GetLineCnt(L""));
			Assert::AreEqual(0, MxUtils::GetLineCnt(L"x"));
			Assert::AreEqual(0, MxUtils::GetLineCnt(L"xx"));
			Assert::AreEqual(0, MxUtils::GetLineCnt(L"xxx"));
			Assert::AreEqual(0, MxUtils::GetLineCnt(L"\r"));

			Assert::AreEqual(1, MxUtils::GetLineCnt(L"\r\n"));
			Assert::AreEqual(1, MxUtils::GetLineCnt(L"x\r\n"));
			Assert::AreEqual(1, MxUtils::GetLineCnt(L"\r\nx"));
			Assert::AreEqual(1, MxUtils::GetLineCnt(L"x\r\nx"));
			Assert::AreEqual(1, MxUtils::GetLineCnt(L"\r\n\r"));

			Assert::AreEqual(2, MxUtils::GetLineCnt(L"\r\n\r\n"));
			Assert::AreEqual(2, MxUtils::GetLineCnt(L"\r\nx\r\n"));
			Assert::AreEqual(2, MxUtils::GetLineCnt(L"\r\n\r\nx"));
			Assert::AreEqual(2, MxUtils::GetLineCnt(L"x\r\n\r\n"));

			Assert::AreEqual(3, MxUtils::GetLineCnt(L"\r\n\r\n\r\n"));
			Assert::AreEqual(3, MxUtils::GetLineCnt(L"xxx\r\nxxx\r\nxxx\r\nxxx"));

		}

		TEST_METHOD(GetLineTextTest)
		{
			Assert::IsFalse(MxUtils::GetLineText(nullptr, MxUtils::Invalid, nullptr));
			Assert::IsFalse(MxUtils::GetLineText(nullptr, -1, nullptr));
			Assert::IsFalse(MxUtils::GetLineText(nullptr, -2, nullptr));
			Assert::IsFalse(MxUtils::GetLineText(L"xxx\r\nyyy\r\nzzz\r\n", MxUtils::Invalid, nullptr));
			std::wstring text(L"");
			Assert::IsFalse(MxUtils::GetLineText(L"xxx\r\nyyy\r\nzzz\r\n", MxUtils::Invalid, &text));
			Assert::IsFalse(MxUtils::GetLineText(L"", MxUtils::Invalid, &text));

			Assert::IsTrue(MxUtils::GetLineText(L"xxx\r\nyyy\r\nzzz\r\n", 0, &text));
			Assert::AreEqual(L"xxx", text.c_str());
			Assert::IsTrue(MxUtils::GetLineText(L"xxx\r\nyyy\r\nzzz\r\n", 1, &text));
			Assert::AreEqual(L"yyy", text.c_str());
			Assert::IsTrue(MxUtils::GetLineText(L"xxx\r\nyyy\r\nzzz\r\n", 2, &text));
			Assert::AreEqual(L"zzz", text.c_str());
			Assert::IsTrue(MxUtils::GetLineText(L"xxx\r\nyyy\r\nzzz\r\n", 3, &text));
			Assert::AreEqual(L"", text.c_str());
			Assert::IsFalse(MxUtils::GetLineText(L"xxx\r\nyyy\r\nzzz\r\n", 4, &text));

			Assert::IsTrue(MxUtils::GetLineText(L"xxx\r\nyyy\r\nzzz", 2, &text));
			Assert::AreEqual(L"zzz", text.c_str());
			Assert::IsTrue(MxUtils::GetLineText(L"xxx", 0, &text));
			Assert::AreEqual(L"xxx", text.c_str());
		}

		TEST_METHOD(GetTextTest)
		{
			Assert::IsFalse(MxUtils::GetText(nullptr, nullptr, nullptr));
			Assert::IsFalse(MxUtils::GetText(L"axxx\r\nbyyy\r\nczzz\r\n", nullptr, nullptr));
			Assert::IsFalse(MxUtils::GetText(L"axxx\r\nbyyy\r\nczzz\r\n", L"", nullptr));

			std::wstring text(L"");
			Assert::IsTrue(MxUtils::GetText(L"axxx\r\nbyyy\r\nczzz\r\n", L"", &text));
			Assert::AreEqual(L"axxx", text.c_str());
			Assert::IsTrue(MxUtils::GetText(L"axxx\r\nbyyy\r\nczzz\r\n", L"a", &text));
			Assert::AreEqual(L"xxx", text.c_str());
			Assert::IsTrue(MxUtils::GetText(L"axxx\r\nbyyy\r\nczzz\r\n", L"b", &text));
			Assert::AreEqual(L"yyy", text.c_str());
			Assert::IsTrue(MxUtils::GetText(L"axxx\r\nbyyy\r\nczzz\r\n", L"c", &text));
			Assert::AreEqual(L"zzz", text.c_str());
			Assert::IsFalse(MxUtils::GetText(L"axxx\r\nbyyy\r\nczzz\r\n", L"d", &text));


			Assert::IsTrue(MxUtils::GetText(L"a:xxx\r\nb:yyy\r\nc:zzz\r\n", L"a:", &text));
			Assert::AreEqual(L"xxx", text.c_str());
			Assert::IsTrue(MxUtils::GetText(L"a:xxx\r\nb:yyy\r\nc:zzz\r\n", L"b:", &text));
			Assert::AreEqual(L"yyy", text.c_str());
			Assert::IsTrue(MxUtils::GetText(L"a:xxx\r\nb:yyy\r\nc:zzz\r\n", L"c:", &text));
			Assert::AreEqual(L"zzz", text.c_str());
			Assert::IsFalse(MxUtils::GetText(L"a:xxx\r\nb:yyy\r\nc:zzz\r\n", L"d:", &text));

			Assert::IsTrue(MxUtils::GetText(L"a:xxx\r\n", L"a:", &text));
			Assert::AreEqual(L"xxx", text.c_str());
			Assert::IsTrue(MxUtils::GetText(L"a:xxx", L"a:", &text));
			Assert::AreEqual(L"xxx", text.c_str());
			Assert::IsTrue(MxUtils::GetText(L"\r\nb:yyy\r\nc:zzz\r\n", L"b:", &text));
			Assert::AreEqual(L"yyy", text.c_str());
			Assert::IsTrue(MxUtils::GetText(L"a:xxx\r\nb:yyy\r\nc:zzz", L"c:", &text));
			Assert::AreEqual(L"zzz", text.c_str());
		}
	};
}
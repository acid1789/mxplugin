#include "stdafx.h"
#include "CppUnitTest.h"
#include "MxUtils\MxUtils1.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MxUtilsTest
{

	TEST_CLASS(EncryptTest)
	{
		static const char * const EncryptTest::Key;
		bool	 _SetupPassword;

	public:
		
		TEST_METHOD_INITIALIZE(InitEncryptTest)
		{
			MxError::Inst().Initialise(ProdIdUnitTestApp::Owner(), ProdIdUnitTestApp::ProdID(), ProdIdUnitTestApp::Name(), "v0.0.0.0", MxError::Development, MxError::VerboseReport);
			_SetupPassword = false;
		}

		TEST_METHOD_CLEANUP(DeinitEncryptTest)
		{
			MxError::Inst().Reset();
		}

		TEST_METHOD(EncryptDecryptTest)
		{
			LPCWSTR res = nullptr;
			std::wstring val(L"");
			std::wstring plaintext(L"");
			int len = 0;

			BYTE codetext[MxUtils::MaxDecryptLength];
											//invalid params
			len = MxUtils::EncryptString(nullptr, val.c_str(), EncryptTest::Key);
			Assert::AreEqual(MxUtils::Invalid, len);
			len = MxUtils::EncryptString(codetext, nullptr, EncryptTest::Key);
			Assert::AreEqual(MxUtils::Invalid, len);
			val = L"";
			len = MxUtils::EncryptString(codetext, val.c_str(), EncryptTest::Key);
			Assert::AreEqual(MxUtils::Invalid, len);
			val = L"123456789012345678901234567890123456789012345678901";			//val.length() == MxUtils::MaxEncryptLength+1
			len = MxUtils::EncryptString(codetext, val.c_str(), EncryptTest::Key);
			Assert::AreEqual(MxUtils::Invalid, len);

			len = 0;
			val = L"12345678901234567890123456789012345678901234567890123456789";	//val.length() == MxUtils::MaxDecryptLength+1 (add 8 bytes for mod 8)
			Assert::IsNull(MxUtils::DecryptString(&plaintext, codetext, len, EncryptTest::Key));
			Assert::AreEqual(0, (int)plaintext.length());
			val = L"";
			Assert::IsNull(MxUtils::DecryptString(&plaintext, codetext, len, EncryptTest::Key));
			Assert::AreEqual(0, (int)plaintext.length());
			Assert::IsNull(MxUtils::DecryptString(nullptr, codetext, len, EncryptTest::Key));
			Assert::AreEqual(0, (int)plaintext.length());
			Assert::IsNull(MxUtils::DecryptString(&plaintext, nullptr, len, EncryptTest::Key));
			Assert::AreEqual(0, (int)plaintext.length());

												//val.length() == 1
			val = L"1";
			len = MxUtils::EncryptString(codetext, val.c_str(), EncryptTest::Key);
			Assert::AreEqual(8, len);
			res = MxUtils::DecryptString(&plaintext, codetext, len, EncryptTest::Key);
			Assert::IsNotNull(res);
			Assert::AreEqual(1, (int)plaintext.length());
			Assert::AreEqual(1, (int)wcslen(res));
			Assert::AreEqual(val, plaintext);

											//val.length() == 7
			val = L"1234567";
			len = MxUtils::EncryptString(codetext, val.c_str(), EncryptTest::Key);
			Assert::AreEqual(8, len);
			res = MxUtils::DecryptString(&plaintext, codetext, len, EncryptTest::Key);
			Assert::IsNotNull(res);
			Assert::AreEqual(7, (int)plaintext.length());
			Assert::AreEqual(7, (int)wcslen(res));
			Assert::AreEqual(val, plaintext);

											//val.length() == 8
			val = L"12345678";
			len = MxUtils::EncryptString(codetext, val.c_str(), EncryptTest::Key);
			Assert::AreEqual(16, len);
			res = MxUtils::DecryptString(&plaintext, codetext, len, EncryptTest::Key);
			Assert::IsNotNull(res);
			Assert::AreEqual(8, (int)plaintext.length());
			Assert::AreEqual(8, (int)wcslen(res));
			Assert::AreEqual(val, plaintext);

											//val.length() == 9
			val = L"123456789";
			len = MxUtils::EncryptString(codetext, val.c_str(), EncryptTest::Key);
			Assert::AreEqual(16, len);
			res = MxUtils::DecryptString(&plaintext, codetext, len, EncryptTest::Key);
			Assert::IsNotNull(res);
			Assert::AreEqual(9, (int)plaintext.length());
			Assert::AreEqual(9, (int)wcslen(res));
			Assert::AreEqual(val, plaintext);

											//val.length() == MxUtils::MaxEncryptLength
			val = L"12345678901234567890123456789012345678901234567890";
			len = MxUtils::EncryptString(codetext, val.c_str(), EncryptTest::Key);
			res = MxUtils::DecryptString(&plaintext, codetext, len, EncryptTest::Key);
			Assert::IsNotNull(res);
			Assert::AreEqual(MxUtils::MaxEncryptLength, (int)plaintext.length());
			Assert::AreEqual(MxUtils::MaxEncryptLength, (int)wcslen(res));
			Assert::AreEqual(val, plaintext);

		}

		TEST_METHOD(PasswordFileTest)
		{
			int len = 0;
			std::wstring val = L"Hello World";
			BYTE codetext[MxUtils::MaxDecryptLength];

			if (_SetupPassword)
			{
				len = MxUtils::EncryptString(codetext, val.c_str(), EncryptTest::Key);
				Assert::IsTrue( len != MxUtils::Invalid);

				Assert::IsTrue(MxUtils::WritePasswordToFile(codetext, len, "UnitTestPwd.txt"));

				for (int x = 0; x < len; x++)
					codetext[x] = '\0';
			}

			len = MxUtils::ReadPasswordFromFile(codetext, MxUtils::MaxDecryptLength, "UnitTestPwd.txt");
			Assert::IsTrue(len > 0);

			LPCWSTR res = nullptr;
			std::wstring plaintext(L"");
			res = MxUtils::DecryptString(&plaintext, codetext, len, EncryptTest::Key);
			Assert::IsNotNull(res);

			if (_SetupPassword)
			{
				Assert::AreEqual(val, plaintext);
			}
		}
	};
	const char * const EncryptTest::Key = "Q56werPLY?";
}
#include "stdafx.h"
#include "CppUnitTest.h"
#include "MxUtils\MxUtils1.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MxUtilsTest
{
	TEST_CLASS(RegistryFnTest)
	{
	public:

		TEST_METHOD_INITIALIZE(InitRegistryFnTest)
		{
			MxError::Inst().Initialise(ProdIdUnitTestApp::Owner(), ProdIdUnitTestApp::ProdID(), ProdIdUnitTestApp::Name(), "v0.0.0.0", MxError::Development, MxError::VerboseReport);
		}

		TEST_METHOD_CLEANUP(DeinitRegistryFnTest)
		{
			MxError::Inst().Reset();
		}
		
		TEST_METHOD(CreateRegAppRootKeyTest)
		{
			 HKEY hAppRootKey = NULL;
			 Assert::IsFalse(MxUtils::CreateRegAppRootKey(NULL, nullptr, nullptr, nullptr));
			 Assert::IsFalse(MxUtils::CreateRegAppRootKey(NULL, nullptr, nullptr, &hAppRootKey));
			 Assert::IsFalse(MxUtils::CreateRegAppRootKey(NULL, nullptr,  L"UnitTest", &hAppRootKey));
			 Assert::IsFalse(MxUtils::CreateRegAppRootKey(NULL, L"Maximodex",  L"UnitTest", &hAppRootKey));
							//first time creates
			 Assert::IsTrue(MxUtils::CreateRegAppRootKey(HKEY_CURRENT_USER, L"Maximodex", L"UnitTest", &hAppRootKey)); //caller has responsibility for RegCloseKey(hAppRootKey);
			 Assert::IsTrue(RegCloseKey(hAppRootKey) ==  ERROR_SUCCESS);
							//subsequently it just opens
			 Assert::IsTrue(MxUtils::CreateRegAppRootKey(HKEY_CURRENT_USER, L"Maximodex", L"UnitTest", &hAppRootKey)); //caller has responsibility for RegCloseKey(hAppRootKey);
			 Assert::IsTrue(RegCloseKey(hAppRootKey) ==  ERROR_SUCCESS);

			 Assert::IsTrue(MxUtils::DelRegAppKey(HKEY_CURRENT_USER, L"Maximodex", L"UnitTest"));
		}

		TEST_METHOD(DelRegAppKey)
		{
			Assert::IsFalse(MxUtils::DelRegAppKey(NULL, nullptr, nullptr));
			Assert::IsFalse(MxUtils::DelRegAppKey(NULL, nullptr, L"UnitTest"));
			Assert::IsFalse(MxUtils::DelRegAppKey(NULL, L"Maximodex", L"UnitTest"));
							//delete a key that doesn't exist fails
			Assert::IsFalse(MxUtils::DelRegAppKey(HKEY_CURRENT_USER, L"Maximodex", L"UnitTest"));
							//create the key
			HKEY hAppRootKey = NULL;
			Assert::IsTrue(MxUtils::CreateRegAppRootKey(HKEY_CURRENT_USER, L"Maximodex", L"UnitTest", &hAppRootKey)); //caller has responsibility for RegCloseKey(hAppRootKey);
			Assert::IsTrue(RegCloseKey(hAppRootKey) ==  ERROR_SUCCESS);
							//now delete it
			Assert::IsTrue(MxUtils::DelRegAppKey(HKEY_CURRENT_USER, L"Maximodex", L"UnitTest"));
		}

		TEST_METHOD(CreateRegAppSubKeyTest)
		{
			 HKEY hAppRootKey = NULL;
			 Assert::IsTrue(MxUtils::CreateRegAppRootKey(HKEY_CURRENT_USER, L"Maximodex", L"UnitTest", &hAppRootKey)); //caller has responsibility for RegCloseKey(hAppRootKey);

			 HKEY hSubKey = NULL;
			 Assert::IsFalse(MxUtils::CreateRegAppSubKey(NULL, nullptr, nullptr));
			 Assert::IsFalse(MxUtils::CreateRegAppSubKey(NULL, nullptr, &hSubKey));
			 Assert::IsFalse(MxUtils::CreateRegAppSubKey(NULL, L"Test", &hSubKey));

			 Assert::IsTrue(MxUtils::CreateRegAppSubKey(hAppRootKey, L"Test", &hSubKey));	//caller has responsibility for RegCloseKey(hSubKey);

			 Assert::IsTrue(RegCloseKey(hSubKey) ==  ERROR_SUCCESS);			 
			 Assert::IsTrue(RegCloseKey(hAppRootKey) ==  ERROR_SUCCESS);
			 							//now delete it
			 Assert::IsTrue(MxUtils::DelRegAppKey(HKEY_CURRENT_USER, L"Maximodex", L"UnitTest"));
		}

		TEST_METHOD(IsPresentRegSubKey)
		{
			 Assert::IsFalse(MxUtils::IsPresentRegSubKey(NULL, nullptr));
			 Assert::IsFalse(MxUtils::IsPresentRegSubKey(NULL, L"Software\\Maximodex\\UnitTest"));
			 Assert::IsFalse(MxUtils::IsPresentRegSubKey(HKEY_CURRENT_USER, L"Software\\Maximodex\\UnitTest"));

			 HKEY hAppRootKey = NULL;
			 Assert::IsTrue(MxUtils::CreateRegAppRootKey(HKEY_CURRENT_USER, L"Maximodex", L"UnitTest", &hAppRootKey)); //caller has responsibility for RegCloseKey(hAppRootKey);
			 Assert::IsTrue(RegCloseKey(hAppRootKey) ==  ERROR_SUCCESS);

			 Assert::IsTrue(MxUtils::IsPresentRegSubKey(HKEY_CURRENT_USER, L"Software\\Maximodex\\UnitTest"));

			 Assert::IsTrue(MxUtils::DelRegAppKey(HKEY_CURRENT_USER, L"Maximodex", L"UnitTest"));
			 Assert::IsFalse(MxUtils::IsPresentRegSubKey(HKEY_CURRENT_USER, L"Software\\Maximodex\\UnitTest"));
		}

		TEST_METHOD(SetRegSubKeyIntTest)
		{
			 HKEY hAppRootKey = NULL;
			 Assert::IsTrue(MxUtils::CreateRegAppRootKey(HKEY_CURRENT_USER, L"Maximodex", L"UnitTest", &hAppRootKey)); //caller has responsibility for RegCloseKey(hAppRootKey);
			 HKEY hSubKey = NULL;
			 Assert::IsTrue(MxUtils::CreateRegAppSubKey(hAppRootKey, L"Test", &hSubKey));	//caller has responsibility for RegCloseKey(hSubKey);

			 Assert::IsFalse(MxUtils::SetRegSubKeyInt(NULL, L"666", 666));
			 Assert::IsTrue(MxUtils::SetRegSubKeyInt(hSubKey, nullptr, 555));	//set default value
			 Assert::IsTrue(MxUtils::SetRegSubKeyInt(hSubKey, L"666", 666));

			 Assert::IsTrue(RegCloseKey(hSubKey) ==  ERROR_SUCCESS);			 
			 Assert::IsTrue(RegCloseKey(hAppRootKey) ==  ERROR_SUCCESS);
			 							//now delete it
			 Assert::IsTrue(MxUtils::DelRegAppKey(HKEY_CURRENT_USER, L"Maximodex", L"UnitTest"));
		}

		TEST_METHOD(GetRegSubKeyIntTest)
		{
			 HKEY hAppRootKey = NULL;
			 Assert::IsTrue(MxUtils::CreateRegAppRootKey(HKEY_CURRENT_USER, L"Maximodex", L"UnitTest", &hAppRootKey)); //caller has responsibility for RegCloseKey(hAppRootKey);
			 HKEY hSubKey = NULL;
			 Assert::IsTrue(MxUtils::CreateRegAppSubKey(hAppRootKey, L"Test", &hSubKey));	//caller has responsibility for RegCloseKey(hSubKey);

			 Assert::IsFalse(MxUtils::SetRegSubKeyInt(NULL, L"666", 666));
			 Assert::IsTrue(MxUtils::SetRegSubKeyInt(hSubKey, nullptr, 555));	//set default value
			 Assert::IsTrue(MxUtils::SetRegSubKeyInt(hSubKey, L"666", 666));

			 int value = 0;
			 Assert::IsFalse(MxUtils::GetRegSubKeyInt(NULL,  nullptr, nullptr));
			 Assert::AreEqual(0, value);
			 Assert::IsFalse(MxUtils::GetRegSubKeyInt(NULL,  L"666",  &value));
			 Assert::AreEqual(0, value);
			 Assert::IsTrue(MxUtils::GetRegSubKeyInt(hSubKey,  nullptr,  &value));
			 Assert::AreEqual(555, value);
			 Assert::IsTrue(MxUtils::GetRegSubKeyInt(hSubKey,  L"666", &value));
			 Assert::AreEqual(666, value);

			 Assert::IsTrue(RegCloseKey(hSubKey) ==  ERROR_SUCCESS);			 
			 Assert::IsTrue(RegCloseKey(hAppRootKey) ==  ERROR_SUCCESS);
			 							//now delete it
			 Assert::IsTrue(MxUtils::DelRegAppKey(HKEY_CURRENT_USER, L"Maximodex", L"UnitTest"));
		}

		TEST_METHOD(SetRegSubKeyStrTest)
		{
			 HKEY hAppRootKey = NULL;
			 Assert::IsTrue(MxUtils::CreateRegAppRootKey(HKEY_CURRENT_USER, L"Maximodex", L"UnitTest", &hAppRootKey)); //caller has responsibility for RegCloseKey(hAppRootKey);
			 HKEY hSubKey = NULL;
			 Assert::IsTrue(MxUtils::CreateRegAppSubKey(hAppRootKey, L"Test", &hSubKey));	//caller has responsibility for RegCloseKey(hSubKey);

			 Assert::IsFalse(MxUtils::SetRegSubKeyStr(NULL, L"Name", L"Me"));
			 Assert::IsTrue(MxUtils::SetRegSubKeyStr(hSubKey, nullptr, L"xxx"));
			 Assert::IsTrue(MxUtils::SetRegSubKeyStr(hSubKey, L"Name", L"Me"));

			 Assert::IsTrue(RegCloseKey(hSubKey) ==  ERROR_SUCCESS);			 
			 Assert::IsTrue(RegCloseKey(hAppRootKey) ==  ERROR_SUCCESS);
			 							//now delete it
			 Assert::IsTrue(MxUtils::DelRegAppKey(HKEY_CURRENT_USER, L"Maximodex", L"UnitTest"));
		}

		TEST_METHOD(GetRegSubKeyStrTest)
		{
			 HKEY hAppRootKey = NULL;
			 Assert::IsTrue(MxUtils::CreateRegAppRootKey(HKEY_CURRENT_USER, L"Maximodex", L"UnitTest", &hAppRootKey)); //caller has responsibility for RegCloseKey(hAppRootKey);
			 HKEY hSubKey = NULL;
			 Assert::IsTrue(MxUtils::CreateRegAppSubKey(hAppRootKey, L"Test", &hSubKey));	//caller has responsibility for RegCloseKey(hSubKey);

			 Assert::IsFalse(MxUtils::SetRegSubKeyStr(NULL, L"Name", L"Me"));
			 Assert::IsTrue(MxUtils::SetRegSubKeyStr(hSubKey, nullptr, L"xxx"));
			 Assert::IsTrue(MxUtils::SetRegSubKeyStr(hSubKey, L"Name", L"Me"));

			 std::wstring value;
			 Assert::IsFalse(MxUtils::GetRegSubKeyStr(NULL, nullptr, nullptr));
			 Assert::IsFalse(MxUtils::GetRegSubKeyStr(hSubKey, nullptr, nullptr));
			 Assert::IsTrue(MxUtils::GetRegSubKeyStr(hSubKey, nullptr, &value));
			 Assert::AreEqual(L"xxx", value.c_str());
			 Assert::IsTrue(MxUtils::GetRegSubKeyStr(hSubKey, L"Name", &value));
			 Assert::AreEqual(L"Me", value.c_str());

			 Assert::IsTrue(RegCloseKey(hSubKey) ==  ERROR_SUCCESS);			 
			 Assert::IsTrue(RegCloseKey(hAppRootKey) ==  ERROR_SUCCESS);
			 							//now delete it
			 Assert::IsTrue(MxUtils::DelRegAppKey(HKEY_CURRENT_USER, L"Maximodex", L"UnitTest"));
		}

		TEST_METHOD(DeepDelRegAppKeyTest)
		{
			 HKEY hAppRootKey = NULL;
			 Assert::IsTrue(MxUtils::CreateRegAppRootKey(HKEY_CURRENT_USER, L"Maximodex", L"UnitTest", &hAppRootKey)); //caller has responsibility for RegCloseKey(hAppRootKey);
			 HKEY hSubKey = NULL;
			 Assert::IsTrue(MxUtils::CreateRegAppSubKey(hAppRootKey, L"Test", &hSubKey));	//caller has responsibility for RegCloseKey(hSubKey);
			 HKEY hSubChildKey = NULL;
			 Assert::IsTrue(MxUtils::CreateRegAppSubKey(hSubKey, L"Child", &hSubChildKey));	//caller has responsibility for RegCloseKey(hSubKey);

 			 Assert::IsTrue(RegCloseKey(hSubChildKey) ==  ERROR_SUCCESS);
			 Assert::IsTrue(RegCloseKey(hSubKey) ==  ERROR_SUCCESS);			 
			 Assert::IsTrue(RegCloseKey(hAppRootKey) ==  ERROR_SUCCESS);

			 Assert::IsTrue(MxUtils::DelRegAppKey(HKEY_CURRENT_USER, L"Maximodex", L"UnitTest", L"Test\\Child\\"));
			 							//now delete it
			 Assert::IsTrue(MxUtils::DelRegAppKey(HKEY_CURRENT_USER, L"Maximodex", L"UnitTest"));

		}
	};
}
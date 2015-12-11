#include "stdafx.h"
#include "CppUnitTest.h"

#include "MxUtils\MxUtils1.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MxUtilsTest
{		
	TEST_CLASS(MxUtilsTest)
	{
	private:
		TCHAR		   Buffer[MAX_PATH];
		std::string    _currentFolder;
		std::string    _targetPathFile;
		std::string	   _noexistPathFile;

	public:
		TEST_METHOD_INITIALIZE(InitPluginXmlTest)
		{
			MxError::Inst().Initialise(ProdIdUnitTestApp::Owner(), ProdIdUnitTestApp::ProdID(), ProdIdUnitTestApp::Name(), "v0.0.0.0", MxError::Development, MxError::VerboseReport);
			 ::GetCurrentDirectory(MAX_PATH,Buffer);
			 USES_CONVERSION; 
			 try
			 {
				_currentFolder = W2CA(Buffer);
			 }
			 catch(...)
			 {
				Assert::Fail(L"W2CA failed and threw exception");
			 }
			 _targetPathFile = _currentFolder;
			 _targetPathFile += "\\MxUtilsTest.dll";
			 _noexistPathFile = _currentFolder;
			 _noexistPathFile += "\\123456.txt";

		}

		TEST_METHOD_CLEANUP(DeinitPluginXmlTest)
		{
			MxError::Inst().Reset();
		}

		TEST_METHOD(IsExistingFolderNameTest)
		{
			Assert::IsFalse(MxUtils::IsExistingFolderName(""));
			Assert::IsTrue(MxUtils::IsExistingFolderName(_currentFolder.c_str()));
			
		}

		TEST_METHOD(IsExistingPathFilenameTest)
		{
			Assert::IsFalse(MxUtils::IsExistingPathFilename(""));
			Assert::IsTrue(MxUtils::IsExistingFolderName(_currentFolder.c_str()));
			Assert::IsFalse(MxUtils::IsExistingFolderName(_targetPathFile.c_str()));
		}

		TEST_METHOD(IsValidPathFilenameTest)
		{
			Assert::IsFalse(MxUtils::IsValidPathFilename(""));

			Assert::IsFalse(MxUtils::IsExistingFolderName(_noexistPathFile.c_str()));

			Assert::IsTrue(MxUtils::IsValidPathFilename(_noexistPathFile.c_str()));
			Assert::IsTrue(MxUtils::IsValidPathFilename(_targetPathFile.c_str()));
		}

		TEST_METHOD(GetEnvVarTest)
		{
			std::wstring value;

			Assert::IsNull(MxUtils::GetEnvironmentVar(&value, L"strangename", true));
			Assert::IsNotNull(MxUtils::GetEnvironmentVar(&value, L"windir", true));

			std::wstring upper = L"";
			int len = value.length();
			for (int x = 0; x < len; x++)
				upper += toupper(value[x]);

			Assert::AreEqual(L"C:\\WINDOWS", upper.c_str());
		}
	};
}
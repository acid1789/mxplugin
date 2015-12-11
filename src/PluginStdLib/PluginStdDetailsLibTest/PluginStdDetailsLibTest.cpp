#include "stdafx.h"
#include "CppUnitTest.h"
#include "PluginStdLib/PluginStdDetailsLib/PluginStdDetailsLib.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PluginStdDetailsLibTest
{		
	TEST_CLASS(PluginStdDetailsLibTest)
	{
		private:
		PluginStdDetailsLib	*_lib;
		public:
			TEST_METHOD_INITIALIZE(InitPluginStdDetailsLib)
			{
				_lib = new PluginStdDetailsLib();
			}

			TEST_METHOD_CLEANUP(DeinitPluginStdDetailsLib)
			{
				if (_lib != nullptr)
					delete _lib;
				_lib = nullptr;
			}

			TEST_METHOD(CreatePluginStdDetailsLibTest)
			{
				Assert::IsNotNull(_lib);
			}

		TEST_METHOD(PluginStdDetailsLibInfoTest)
		{

			std::string temp;
			Assert::IsTrue(MxGUID::IsEqual( PRODID_PluginStdLib, MxGUID::ConvertFromString( "{8CB84F11-8199-4508-A081-FDADB7015ABD}")));

	//IMxPluginBase
			Assert::IsTrue(MxGUID::IsEqual(IPluginStdLibIID::Guid(), _lib->GetInterfaceGUID()));
			Assert::AreEqual(IPluginStdLibIID::Name(), _lib->GetInterfaceName());
	//IMxPluginLibDetails
			Assert::AreEqual("PluginStdLib", _lib->GetLibName());
			Assert::AreEqual("Example implementation of the IMxPluginLibDetails interface", _lib->GetLibDescription());
			Assert::AreEqual("1.3.30.1", _lib->GetLibVersion());
			Assert::AreEqual("MxPluginTeam", _lib->GetLibOwnerName());
			Assert::AreEqual("Copyright (C) 2015 Will Stott",_lib->GetLibCopyrightNotice());
			Assert::AreEqual("{8CB84F11-8199-4508-A081-FDADB7015ABD}",_lib->GetLibID());
			Assert::AreEqual("http://www.mxplugin.org",_lib->GetLibSupportSite());
			Assert::AreEqual("New BSD Licence",_lib->GetLibLicenseName());
			Assert::AreEqual("http://mxplugin.codeplex.com/license",_lib->GetLibLicenseURL());
			std::wstring pathfilename(_lib->GetLibPathFilename());
			Assert::IsFalse(pathfilename.find(L"PluginStdLib.dll") != std::wstring::npos);
			Assert::IsTrue(MxGUID::IsEqual(SID_Universal,_lib->GetSocketID()));
	//IPluginStdDetailsLib
			Assert::AreEqual("hello from PluginStdLib",_lib->GetComment());
		}


	};
}
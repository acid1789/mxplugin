#include "stdafx.h"
#include "CppUnitTest.h"
#include "MxPluginManager\MxPluginManager1.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MxPluginManagerTest
{		
	TEST_CLASS(MxPluginManagerTest)
	{
	private:
		MxVersionInfo app;
		MxPluginManager	*_MxPluginManager;

	public:
		TEST_METHOD_INITIALIZE(InitMxPluginManager)
		{
			MxError::Inst().Initialise(app.GetOwner(), MxGUID::ConvertFromString(app.GetProductID()), app.GetProductName(), app.GetVersion(), MxError::Development, MxError::VerboseReport);		

			_MxPluginManager = new MxPluginManager();

		}

		TEST_METHOD_CLEANUP(DeinitMxPluginManager)
		{
			if (_MxPluginManager != nullptr)
				delete _MxPluginManager;
			_MxPluginManager = nullptr;

			MxError::Inst().Reset();
		}

		
		TEST_METHOD(CreateMxPluginManagerTest)
		{
			Assert::IsNotNull(_MxPluginManager);
		}

		TEST_METHOD(MxPluginManagerDetailsTest)
		{
			Assert::AreEqual("MxPluginManager", _MxPluginManager->GetLibName());
			Assert::AreEqual("MxPluginManager provides support for an application wanting to load MxPlugins (this library is not itself a MxPlugin)", _MxPluginManager->GetLibDescription());
			Assert::AreEqual("1.3.30.1", _MxPluginManager->GetLibVersion());
			Assert::AreEqual("MxPluginTeam", _MxPluginManager->GetLibOwnerName());
			Assert::AreEqual("Copyright (C) 2015 Will Stott", _MxPluginManager->GetLibCopyrightNotice());
			Assert::AreEqual("{7595969A-38FA-4FB6-A560-883E0C41728A}", _MxPluginManager->GetLibInternalName());
			Assert::AreEqual("http://www.mxplugin.org", _MxPluginManager->GetLibSupportSite());
			Assert::AreEqual("New BSD Licence", _MxPluginManager->GetLibLicenseName());
			Assert::AreEqual("http://mxplugin.codeplex.com/license", _MxPluginManager->GetLibLicenseURL());
			std::wstring pathfilename(_MxPluginManager->GetLibPathFilename());
			Assert::IsFalse(pathfilename.find(L"MxPluginManager.dll") != std::wstring::npos);
		}

		TEST_METHOD(MxPluginManagerRefreshPluginsInFolderTest)
		{
			_MxPluginManager->ClearAllMxPlugins();
			Assert::AreEqual(0, _MxPluginManager->GetPluginCount());
			Assert::IsNull(_MxPluginManager->GetPlugin(0));
			Assert::IsNull(_MxPluginManager->GetPlugin(GUID_Invalid));

			Assert::IsTrue(_MxPluginManager->RefreshPluginsInRootFolder( SID_Universal, true));

			int pluginCnt = 0;
			Assert::IsTrue((pluginCnt = _MxPluginManager->GetPluginCount()) > 0);
			Assert::IsNotNull(_MxPluginManager->GetPlugin(PRODID_PluginStdLib));	//PlugStdLib VS_VERSIONINFO - Internal Name
			Assert::IsNull(_MxPluginManager->GetPlugin(pluginCnt));
			Assert::IsNull(_MxPluginManager->GetPlugin(-1));
			Assert::IsNull(_MxPluginManager->GetPlugin(GUID_Invalid ));

			_MxPluginManager->ClearAllMxPlugins();
			Assert::AreEqual(0, _MxPluginManager->GetPluginCount());
			Assert::IsNull(_MxPluginManager->GetPlugin(0));
			Assert::IsNull(_MxPluginManager->GetPlugin(GUID_Invalid));
		}
	};
}
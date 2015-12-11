#include "stdafx.h"
#include "CppUnitTest.h"

#include "MxPluginManager\MxPluginManager1.h"
#include "MxPluginManager\MxPluginLib1.h"

#include "PluginStdLib\IPluginStdLib.h"
#include "MxProductID.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MxPluginManagerTest
{
	TEST_CLASS(MxPluginLibTest)
	{
	private:
		MxVersionInfo app;
		MxPluginLib	*_plugin;
		MxPluginManager	*_MxPluginManager;

	public:

		TEST_METHOD_INITIALIZE(InitMxPluginLib)
		{
			MxError::Inst().Initialise(app.GetOwner(), MxGUID::ConvertFromString(app.GetProductID()), app.GetProductName(), app.GetVersion(), MxError::Development, MxError::VerboseReport);		

			_plugin = new MxPluginLib();
			_MxPluginManager = new MxPluginManager();
		}

		TEST_METHOD_CLEANUP(DeinitMxPluginLib)
		{
			if (_plugin != nullptr)
				delete _plugin;
			_plugin = nullptr;
			if (_MxPluginManager != nullptr)
				delete _MxPluginManager;
			_MxPluginManager = nullptr;

			MxError::Inst().Reset();
		}

		
		TEST_METHOD(CreateMxPluginLibTest)
		{
			Assert::IsNotNull(_plugin);
		}

		TEST_METHOD(SelectionTest)
		{				//used just for making a selection when iterating through a list
			Assert::IsNotNull(_plugin);
			_plugin->SetSelection(false);
			Assert::IsFalse(_plugin->GetSelection());
			_plugin->SetSelection(true);
			Assert::IsTrue(_plugin->GetSelection());
		}

		TEST_METHOD(UnInitPluginTest)
		{
			Assert::IsNotNull(_plugin);
			Assert::IsFalse(_plugin->IsValid());
			Assert::IsNull(_plugin->CreateClassInstance(IMxPluginBaseIID::Guid()));
			Assert::AreEqual(MxPluginLib::InvalidStr, _plugin->GetLibName());
			Assert::AreEqual(MxPluginLib::InvalidStr, _plugin->GetLibVersion());
			Assert::AreEqual(MxPluginLib::InvalidStr, _plugin->GetLibDetails());
			Assert::AreEqual(MxPluginLib::InvalidStr, _plugin->GetLibSummary());
			Assert::AreEqual(MxPluginLib::InvalidStr, _plugin->GetPathFilename());
			Assert::AreEqual(MxPluginLib::InvalidwStr, _plugin->GetwPathFilename());
		}

		TEST_METHOD(RefreshPluginsInRootFolderTest)
		{
			_MxPluginManager->ClearAllMxPlugins();
			Assert::AreEqual(0, _MxPluginManager->GetPluginCount());
			Assert::IsTrue(_MxPluginManager->RefreshPluginsInRootFolder( SID_Universal, true) );

			Assert::IsTrue(_MxPluginManager->GetPluginCount() > 0);
			MxPluginLib *stdlib = _MxPluginManager->GetPlugin(0);
			Assert::IsNotNull(stdlib);

			Assert::AreEqual("PluginStdLib", stdlib->GetLibName());
			Assert::AreEqual("1.3.30.1", stdlib->GetLibVersion());
			std::string libdetails(stdlib->GetLibDetails());
			Assert::IsTrue(libdetails.find("PluginStdLib, 1.3.30.1") != std::string::npos);
			std::string libsummary(stdlib->GetLibSummary());
			Assert::IsTrue(libsummary.find("PluginStdLib, 1.3.30.1") != std::string::npos);
			std::string libpathfilename(stdlib->GetPathFilename());
			Assert::IsTrue(libpathfilename.find("PluginStdLib.dll") != std::string::npos);
		}

		TEST_METHOD(RefreshPluginsInFolderTest)
		{
			_MxPluginManager->ClearAllMxPlugins();
			Assert::AreEqual(0, _MxPluginManager->GetPluginCount());

			Assert::IsTrue(_MxPluginManager->RefreshPluginsInRootFolder(SID_Universal, true));
			Assert::IsTrue(_MxPluginManager->GetPluginCount() > 0);
			MxPluginLib *stdlib = _MxPluginManager->GetPlugin(PRODID_PluginStdLib);
			Assert::IsNotNull(stdlib);

			Assert::AreEqual("PluginStdLib", stdlib->GetLibName());
			Assert::AreEqual("1.3.30.1", stdlib->GetLibVersion());

			Assert::IsTrue( MxGUID::IsEqual(PRODID_PluginStdLib, stdlib->GetLibID())); 
			std::string libdetails(stdlib->GetLibDetails());
			Assert::IsTrue(libdetails.find("PluginStdLib, 1.3.30.1") != std::string::npos);
			std::string libsummary(stdlib->GetLibSummary());
			Assert::IsTrue(libsummary.find("PluginStdLib, 1.3.30.1") != std::string::npos);
			std::string libpathfilename(stdlib->GetPathFilename());
			Assert::IsTrue(libpathfilename.find("PluginStdLib.dll") != std::string::npos);
		}

		TEST_METHOD(PluginStdLibCreateClassInstanceTest)
		{
			_MxPluginManager->ClearAllMxPlugins();
			Assert::AreEqual(0, _MxPluginManager->GetPluginCount());

			Assert::IsTrue(_MxPluginManager->RefreshPluginsInRootFolder(SID_Universal, true));
			Assert::IsTrue(_MxPluginManager->GetPluginCount() > 0);
			MxPluginLib *plugin = _MxPluginManager->GetPlugin(PRODID_PluginStdLib);
			Assert::IsNotNull(plugin);

			IPluginStdLib *stdlib = nullptr;
			if ((stdlib =  static_cast<IPluginStdLib *>(plugin->CreateClassInstance(IPluginStdLibIID::Guid()))) != nullptr)
			{
				Assert::IsNotNull(stdlib);
				Assert::AreEqual("PluginStdLib", stdlib->GetLibName());
				Assert::AreEqual("1.3.30.1", stdlib->GetLibVersion());
				Assert::AreEqual("hello from PluginStdLib", stdlib->GetComment());

				Assert::AreEqual(0L, stdlib->DestroyPtr());
				stdlib = nullptr;
			}
		}
	};
}
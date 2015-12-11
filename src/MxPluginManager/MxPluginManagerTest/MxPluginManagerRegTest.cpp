#include "stdafx.h"
#include "CppUnitTest.h"
#include "MxPluginManager\MxPluginManager1.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MxPluginManagerTest
{
	TEST_CLASS(MxPluginManagerRegTest)
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
		
		TEST_METHOD(CreateMxPluginManagerRegTest)
		{
			Assert::IsNotNull(_MxPluginManager);
		}

		TEST_METHOD(ConvertNameTest)
		{
			std::wstring tmp;
			Assert::IsNull(MxPluginManager::ConvertName(nullptr, nullptr));
			Assert::IsNull(MxPluginManager::ConvertName("test", nullptr));
			Assert::IsNull(MxPluginManager::ConvertName(nullptr, &tmp));

			Assert::AreEqual(L"test", MxPluginManager::ConvertName("test", &tmp));
		}

		TEST_METHOD(ConvertAppKeyNameTest)
		{
			std::wstring tmp;
			Assert::IsNull(MxPluginManager::ConvertAppKeyName(-1, nullptr));
			Assert::IsNull(MxPluginManager::ConvertAppKeyName(-1,&tmp));
			Assert::IsNull(MxPluginManager::ConvertAppKeyName(MxPluginManager::AppCntMax+1, &tmp));
			Assert::IsNull(MxPluginManager::ConvertAppKeyName(0, nullptr));

			Assert::AreEqual(L"App0\\", MxPluginManager::ConvertAppKeyName(0, &tmp));
		}

		TEST_METHOD(ConvertGUIDTest)
		{
			std::wstring tmp;
			Assert::IsNull(MxPluginManager::ConvertGUID(GUID_Invalid, nullptr));
			Assert::AreEqual(L"{00000000-0000-0000-0000-000000000000}", MxPluginManager::ConvertGUID(GUID_Invalid, &tmp));
			Assert::AreEqual(L"{02742858-2CC8-4C92-BE69-5CC7BA387A7B}", MxPluginManager::ConvertGUID(SID_Universal, &tmp));
		}

		TEST_METHOD(SetFactoryDefaultsUnRegisterTest)
		{
			Assert::IsTrue(_MxPluginManager->SetFactoryDefaults());
			Assert::IsTrue(_MxPluginManager->UnRegister());
		}

		TEST_METHOD(GetSetRootFolderTest)
		{
			_MxPluginManager->UnRegister();

			Assert::IsTrue(_MxPluginManager->SetFactoryDefaults());
			Assert::AreEqual(MxPluginManager::DefaultPluginRootFolder, _MxPluginManager->GetRootFolder());
			Assert::IsTrue(_MxPluginManager->UnRegister());

		}

		TEST_METHOD(GetAppCntTest)
		{
			_MxPluginManager->UnRegister();

			Assert::AreEqual(MxPluginManager::InvalidVal, _MxPluginManager->GetAppCnt());
			Assert::IsTrue(_MxPluginManager->SetFactoryDefaults());
			Assert::AreEqual(1, _MxPluginManager->GetAppCnt());
			Assert::IsTrue(_MxPluginManager->UnRegister());
			Assert::AreEqual(MxPluginManager::InvalidVal, _MxPluginManager->GetAppCnt());
			Assert::IsTrue(_MxPluginManager->UnRegister());
		}

		TEST_METHOD(GetAppProdIDTest)
		{
			_MxPluginManager->UnRegister();

			std::string result;
			Assert::IsNull(_MxPluginManager->GetAppProdID(-1, nullptr));
			Assert::IsNull(_MxPluginManager->GetAppProdID(-1, &result));
			Assert::IsNull(_MxPluginManager->GetAppProdID(MxPluginManager::AppCntMax+1, &result));
			Assert::IsNull(_MxPluginManager->GetAppProdID(0, &result));

			Assert::IsTrue(_MxPluginManager->SetFactoryDefaults());
			Assert::AreEqual("{7595969A-38FA-4FB6-A560-883E0C41728A}", _MxPluginManager->GetAppProdID(0, &result));
			Assert::IsTrue(_MxPluginManager->UnRegister());
			Assert::IsNull(_MxPluginManager->GetAppProdID(0, &result));
			Assert::IsTrue(_MxPluginManager->UnRegister());
		}

		TEST_METHOD(GetAppIndexTest)
		{
			_MxPluginManager->UnRegister();

			Assert::IsTrue(_MxPluginManager->SetFactoryDefaults());
			Assert::AreEqual(1, _MxPluginManager->GetAppCnt());
			Assert::AreEqual(MxPluginManager::InvalidVal, _MxPluginManager->GetAppIndex(ProdIdPluginXmlApp::ProdID()));
			Assert::IsTrue(_MxPluginManager->RegAppInit(ProdIdPluginXmlApp::Name(), ProdIdPluginXmlApp::ProdID(), SID_Universal, MxError::Development, MxError::VerboseReport)); 
			Assert::AreEqual(2, _MxPluginManager->GetAppCnt());

			Assert::AreEqual(1, _MxPluginManager->GetAppIndex(ProdIdPluginXmlApp::ProdID()));
			Assert::AreEqual(1, _MxPluginManager->GetAppIndex("{C3F1D8CC-9965-4589-881A-EB561D748530}"));

			Assert::IsTrue(_MxPluginManager->UnRegister());
		}

		TEST_METHOD(RegAppInitTest)
		{
			_MxPluginManager->UnRegister();

			Assert::IsFalse(_MxPluginManager->RegAppInit(nullptr, GUID_Invalid,  GUID_Invalid, MxError::Development, MxError::VerboseReport)); 
			Assert::IsFalse(_MxPluginManager->RegAppInit(nullptr, GUID_Invalid,  SID_Universal, MxError::Development, MxError::VerboseReport)); 
			Assert::IsFalse(_MxPluginManager->RegAppInit(nullptr, PRODID_PluginXmlApp, SID_Universal, MxError::Development, MxError::VerboseReport)); 
			Assert::IsFalse(_MxPluginManager->RegAppInit(ProdIdPluginXmlApp::Name(), ProdIdPluginXmlApp::ProdID(), SID_Universal, MxError::Development, MxError::VerboseReport)); 

			Assert::AreEqual(MxPluginManager::ErrorSetFactoryDefaultNeeded, MxError::Inst().GetLastErrorCode());

			Assert::IsTrue(_MxPluginManager->SetFactoryDefaults());
			Assert::AreEqual(1, _MxPluginManager->GetAppCnt());
			Assert::IsTrue(_MxPluginManager->RegAppInit(ProdIdPluginXmlApp::Name(), ProdIdPluginXmlApp::ProdID(), SID_Universal, MxError::Development, MxError::VerboseReport)); 
			Assert::AreEqual(2, _MxPluginManager->GetAppCnt());
			std::string result;
			Assert::AreEqual("{C3F1D8CC-9965-4589-881A-EB561D748530}", _MxPluginManager->GetAppProdID(1, &result));

			Assert::IsTrue(_MxPluginManager->UnRegister());
		}

		TEST_METHOD(UnRegAppTest)
		{
			_MxPluginManager->UnRegister();

			const char *xmlProdID = "{C3F1D8CC-9965-4589-881A-EB561D748530}";
			Assert::IsFalse(_MxPluginManager->UnRegApp(xmlProdID));

			Assert::IsTrue(_MxPluginManager->SetFactoryDefaults());
			Assert::AreEqual(1, _MxPluginManager->GetAppCnt());
			Assert::IsTrue(_MxPluginManager->RegAppInit(ProdIdPluginXmlApp::Name(), ProdIdPluginXmlApp::ProdID(), SID_Universal, MxError::Development, MxError::VerboseReport)); 
			Assert::AreEqual(2, _MxPluginManager->GetAppCnt());
			std::string result;
			Assert::AreEqual("{C3F1D8CC-9965-4589-881A-EB561D748530}", _MxPluginManager->GetAppProdID(1, &result));

			Assert::IsTrue(_MxPluginManager->UnRegApp(xmlProdID));
			Assert::AreEqual(2, _MxPluginManager->GetAppCnt());			//doesn't decrement AppCnt, but does remove Apps\App1\ so GetAppProdID(1, &result) returns null
			Assert::IsNull(_MxPluginManager->GetAppProdID(1, &result));

			Assert::IsTrue(_MxPluginManager->UnRegister());
		}

		TEST_METHOD(GetAppNameTest)
		{
			_MxPluginManager->UnRegister();

			std::string tmp;
			Assert::AreEqual(MxPluginManager::InvalidName, _MxPluginManager->GetAppName("", nullptr));
			Assert::AreEqual(MxPluginManager::InvalidName, _MxPluginManager->GetAppName("", &tmp));

			Assert::IsTrue(_MxPluginManager->SetFactoryDefaults());
			Assert::IsTrue(_MxPluginManager->RegAppInit(ProdIdPluginXmlApp::Name(), ProdIdPluginXmlApp::ProdID(),  SID_Universal, MxError::Development, MxError::VerboseReport)); 
			Assert::AreEqual(2, _MxPluginManager->GetAppCnt());
			std::string result;
			const char *XmlProdID = _MxPluginManager->GetAppProdID(1, &result);
			Assert::AreEqual("{C3F1D8CC-9965-4589-881A-EB561D748530}", XmlProdID);

			Assert::AreEqual(ProdIdPluginXmlApp::Name(), _MxPluginManager->GetAppName(XmlProdID, &tmp));
	
			Assert::IsTrue(_MxPluginManager->UnRegister());
		}

		TEST_METHOD(GetAppSIDTest)
		{
			_MxPluginManager->UnRegister();

			std::string tmp;
			Assert::AreEqual(MxPluginManager::InvalidName, _MxPluginManager->GetAppSID("", nullptr));
			Assert::AreEqual(MxPluginManager::InvalidName, _MxPluginManager->GetAppSID("", &tmp));

			Assert::IsTrue(_MxPluginManager->SetFactoryDefaults());
			Assert::IsTrue(_MxPluginManager->RegAppInit(ProdIdPluginXmlApp::Name(), ProdIdPluginXmlApp::ProdID(),  SID_Universal, MxError::Development, MxError::VerboseReport)); 
			Assert::AreEqual(2, _MxPluginManager->GetAppCnt());
			std::string result;
			const char *XmlProdID = _MxPluginManager->GetAppProdID(1, &result);
			Assert::AreEqual("{C3F1D8CC-9965-4589-881A-EB561D748530}", XmlProdID);
			Assert::AreEqual("{02742858-2CC8-4C92-BE69-5CC7BA387A7B}", _MxPluginManager->GetAppSID(XmlProdID, &tmp));
	
			Assert::IsTrue(_MxPluginManager->UnRegister());
		}

		TEST_METHOD(GetAppKeyNameTest)
		{
			_MxPluginManager->UnRegister();

			std::string tmp;
			Assert::AreEqual(MxPluginManager::InvalidName, _MxPluginManager->GetAppName("", nullptr));
			Assert::AreEqual(MxPluginManager::InvalidName, _MxPluginManager->GetAppName("", &tmp));
			Assert::AreEqual(MxPluginManager::InvalidName, _MxPluginManager->GetAppName("{02742858-2CC8-4C92-BE69-5CC7BA387A7B}", &tmp));

			Assert::IsTrue(_MxPluginManager->SetFactoryDefaults());
			Assert::IsTrue(_MxPluginManager->RegAppInit(ProdIdPluginXmlApp::Name(), ProdIdPluginXmlApp::ProdID(),  SID_Universal, MxError::Development, MxError::VerboseReport)); 
			Assert::AreEqual(2, _MxPluginManager->GetAppCnt());
			std::string result;
			const char *XmlProdid = _MxPluginManager->GetAppProdID(1, &result);
			Assert::AreEqual("{C3F1D8CC-9965-4589-881A-EB561D748530}", XmlProdid);

			Assert::AreEqual("App1", _MxPluginManager->GetAppKeyName(XmlProdid, &tmp));
	
			Assert::IsTrue(_MxPluginManager->UnRegister());
		}

		TEST_METHOD(GetSetAppLogChannelTest)
		{
			_MxPluginManager->UnRegister();
			MxError::Inst().Reset();

			Assert::IsTrue(_MxPluginManager->GetAppLogChannel("") == MxError::NoLogging); 
			Assert::IsFalse(_MxPluginManager->SetAppLogChannel("", MxError::NoLogging)); 
			MxError::Inst().Reset();
			Assert::IsTrue(_MxPluginManager->GetAppLogChannel("{C3F1D8CC-9965-4589-881A-EB561D748530}") == MxError::NoLogging); 
			Assert::IsFalse(_MxPluginManager->SetAppLogChannel("{C3F1D8CC-9965-4589-881A-EB561D748530}", MxError::NoLogging)); 
			MxError::Inst().Reset();

			Assert::IsTrue(_MxPluginManager->SetFactoryDefaults());
			Assert::IsTrue(_MxPluginManager->RegAppInit(ProdIdPluginXmlApp::Name(), ProdIdPluginXmlApp::ProdID(),  SID_Universal, MxError::Development, MxError::VerboseReport)); 
			Assert::AreEqual(2, _MxPluginManager->GetAppCnt());
			std::string result;
			const char *XmlProdID = _MxPluginManager->GetAppProdID(1, &result);
			Assert::AreEqual("{C3F1D8CC-9965-4589-881A-EB561D748530}", XmlProdID);

			Assert::IsTrue(_MxPluginManager->GetAppLogChannel(XmlProdID) == MxError::Development); 
			Assert::IsTrue(_MxPluginManager->SetAppLogChannel(XmlProdID, MxError::Production)); 
			Assert::IsTrue(_MxPluginManager->GetAppLogChannel(XmlProdID) ==MxError::Production); 	

			Assert::IsTrue(_MxPluginManager->UnRegister());
		}

		TEST_METHOD(GetSetAppLogLevelTest)
		{
			_MxPluginManager->UnRegister();
			MxError::Inst().Reset();

			Assert::IsFalse(MxError::Inst().IsErrorSet());
			Assert::IsTrue(_MxPluginManager->GetAppLogLevel("") == MxError::QuietReport); 
			Assert::IsTrue(MxError::Inst().IsErrorSet());
			MxError::Inst().Reset();

			Assert::IsFalse(_MxPluginManager->SetAppLogLevel("", MxError::QuietReport)); 
			MxError::Inst().Reset();
			Assert::IsFalse(MxError::Inst().IsErrorSet());
			Assert::IsTrue(_MxPluginManager->GetAppLogLevel("{C3F1D8CC-9965-4589-881A-EB561D748530}") == MxError::QuietReport); 
			Assert::IsTrue(MxError::Inst().IsErrorSet());
			Assert::IsFalse(_MxPluginManager->SetAppLogLevel("{C3F1D8CC-9965-4589-881A-EB561D748530}", MxError::QuietReport)); 

			Assert::IsTrue(_MxPluginManager->SetFactoryDefaults());
			Assert::IsTrue(_MxPluginManager->RegAppInit(ProdIdPluginXmlApp::Name(), ProdIdPluginXmlApp::ProdID(), SID_Universal, MxError::Development, MxError::QuietReport)); 
			Assert::AreEqual(2, _MxPluginManager->GetAppCnt());
			std::string result;
			const char *XmlProdID = _MxPluginManager->GetAppProdID(1, &result);
			Assert::AreEqual("{C3F1D8CC-9965-4589-881A-EB561D748530}", XmlProdID);


			Assert::IsTrue(_MxPluginManager->GetAppLogLevel(XmlProdID) == MxError::QuietReport); 
			Assert::IsTrue(_MxPluginManager->SetAppLogLevel(XmlProdID, MxError::VerboseReport)); 
			Assert::IsTrue(_MxPluginManager->GetAppLogLevel(XmlProdID) == MxError::VerboseReport); 	

			Assert::IsTrue(_MxPluginManager->UnRegister());
		}
	};
}
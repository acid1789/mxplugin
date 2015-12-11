#include "stdafx.h"
#include "CppUnitTest.h"
#include "PluginXml/MxPluginXml/PluginXml.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PluginXmlTest
{		
	TEST_CLASS(PluginXmlTest) 
	{
	private:
		PluginXml *_pluginXml;

	public:
		TEST_METHOD_INITIALIZE(InitPluginXmlTest)
		{
			_pluginXml = new PluginXml();

		}

		TEST_METHOD_CLEANUP(DeinitPluginXmlTest)
		{
			if (_pluginXml != nullptr)
				delete _pluginXml;
			_pluginXml = nullptr;
		}
		
		TEST_METHOD(CreatePluginXmlTest)
		{
			Assert::IsNotNull(_pluginXml);
		}

		TEST_METHOD(IMxPluginBaseTest)
		{
			IMxPluginBase *base = static_cast<IMxPluginBase *>(::CreateClassInstance( GUID_Invalid ));
			Assert::IsNull(base);

			base = static_cast<IMxPluginBase *>(::CreateClassInstance(IMxPluginBaseIID::Guid()));

			Assert::IsNotNull(base);
			Assert::IsTrue(MxGUID::IsEqual(IPluginXmlLibIID::Guid(), base->GetInterfaceGUID()));
			Assert::AreEqual(IPluginXmlLibIID::Name(), base->GetInterfaceName());
	
			IMxPluginLibDetails *details = static_cast<IMxPluginLibDetails *>(base->Dynamic_cast(IMxPluginLibDetailsIID::Guid()));
			Assert::IsNotNull(details);
			Assert::AreEqual("{9E8E6865-D31E-40A2-B913-59BEBC11BC41}", details->GetLibID());
			Assert::AreEqual(1L, details->DestroyPtr());
			details = nullptr;

			IMxPluginBase *base2 = base;
			base->DuplicatePtr();
			Assert::AreEqual(1L, base2->DestroyPtr());
			base2 = nullptr;

			Assert::IsNotNull(base);
			Assert::IsTrue(MxGUID::IsEqual(IPluginXmlLibIID::Guid(), base->GetInterfaceGUID()));
			Assert::AreEqual(IPluginXmlLibIID::Name(), base->GetInterfaceName());
	
			Assert::AreEqual(0L, base->DestroyPtr());
			base = nullptr;
		}

		TEST_METHOD(IMxPluginLibDetailsTest)
		{
			IMxPluginLibDetails *details = static_cast<IMxPluginLibDetails *>(::CreateClassInstance(IMxPluginLibDetailsIID::Guid()));

			Assert::IsNotNull(details);

			Assert::AreEqual("{9E8E6865-D31E-40A2-B913-59BEBC11BC41}", details->GetLibID());
			Assert::AreEqual("PluginXml",details->GetLibName());
			Assert::AreEqual("1.3.30.1", details->GetLibVersion());
			Assert::AreEqual("Provides interface to the popular TinyXML open source library", details->GetLibDescription());
			Assert::AreEqual("http://www.mxplugin.org/plugins/support/MxPluginTeam/PluginXml", details->GetLibSupportSite());
			Assert::IsTrue(MxGUID::IsEqual(SID_Universal, details->GetSocketID()));

			Assert::AreEqual("MxPluginTeam",details->GetLibOwnerName());
			Assert::AreEqual("Copyright (C) 2015 Will Stott", details->GetLibCopyrightNotice());
	
			Assert::AreEqual("New BSD Licence", details->GetLibLicenseName());
			Assert::AreEqual("http://www.mxplugin.org/plugins/license/MxPluginTeam/PluginXml", details->GetLibLicenseURL());

			Assert::AreNotEqual(L"[invalidPathFilename]", details->GetLibPathFilename());

			Assert::AreEqual(0L, details->DestroyPtr());
			details = nullptr;
		}

		TEST_METHOD(IPluginXmlLibTest)
		{
			IPluginXmlLib *xml = static_cast<IPluginXmlLib *>(::CreateClassInstance(IPluginXmlLibIID::Guid()));

			Assert::IsNotNull(xml);

			Assert::AreEqual("{9E8E6865-D31E-40A2-B913-59BEBC11BC41}", xml->GetLibID());  //PRODID_PluginXml =	{9E8E6865-D31E-40A2-B913-59BEBC11BC41}

			Assert::AreEqual(0L, xml->DestroyPtr());
			xml = nullptr;
		}

	};
}
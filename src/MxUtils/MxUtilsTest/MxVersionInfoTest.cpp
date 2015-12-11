#include "stdafx.h"
#include "CppUnitTest.h"

#include "MxUtils\MxUtils1.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MxUtilsTest
{
	TEST_CLASS(MxVersionInfoTest)
	{
	public:

		TEST_METHOD_INITIALIZE(InitMxVersionInfoTest)
		{
			MxError::Inst().Initialise(ProdIdUnitTestApp::Owner(), ProdIdUnitTestApp::ProdID(), ProdIdUnitTestApp::Name(), "v0.0.0.0", MxError::Development, MxError::VerboseReport);
		}

		TEST_METHOD_CLEANUP(DeinitMxVersionInfoTest)
		{
			MxError::Inst().Reset();
		}
		
		TEST_METHOD(GetVersionInfoTest)
		{
			Assert::AreEqual("MxUtils", MxUtils::Inst().GetProductName());		
			Assert::AreEqual("Utilities for C++ Programs, including MxError", MxUtils::Inst().GetDescription());		
			Assert::AreEqual("1.3.30.1", MxUtils::Inst().GetVersion());	
			Assert::AreEqual("MxPluginTeam", MxUtils::Inst().GetOwner());
			Assert::AreEqual("Copyright (C) 2015 Will Stott", MxUtils::Inst().GetCopyrightNotice());
			Assert::AreEqual("{2A3BBF1E-BE54-4573-9407-828B6F7E7FDD}", MxUtils::Inst().GetProductID());

			std::wstring pathfilenameDLL = MxUtils::Inst().GetwPathFilename();
			Assert::IsTrue(pathfilenameDLL.find(L".dll") != std::string::npos);
		}

	};
}
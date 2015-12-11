#include "stdafx.h"
#include "CppUnitTest.h"
#include "MxUtils\MxUtils1.h"
#include "MxUtils\MxUtilsErrorCodes1.h"
#include "MxUtils\EventTracing.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MxUtilsTest
{
	TEST_CLASS(EventTracingTest)
	{
	public:

		TEST_METHOD_INITIALIZE(InitEventTracingTest)
		{
			MxError::Inst().Initialise(ProdIdUnitTestApp::Owner(), ProdIdUnitTestApp::ProdID(), ProdIdUnitTestApp::Name(), "v0.0.0.0", MxError::Development, MxError::VerboseReport);
		}

		TEST_METHOD_CLEANUP(DeinitEventTracingTest)
		{
			MxError::Inst().Reset();
		}
		
		TEST_METHOD(IsValidTest)
		{
			Assert::IsTrue(EventTracing::Inst().IsValid());
		}

		TEST_METHOD(IsEnabledTest)
		{
			Assert::IsFalse(EventTracing::Inst().IsEnabled());	//True if logman create  trace MxUtilsTrace -p {86563644-640D-417E-AB6F-0F543B0428D9}
		}

		TEST_METHOD(SetErrorEventTest)
		{
			Assert::IsTrue(EventTracing::Inst().SetErrorEvent( "MxPluginTeam", "MxUtils unit test", "product name", "0.0.0.0", 0, 0, MxError::CodeDefect, MxError::Abort, "test" ));
		}

		TEST_METHOD(SetLogEventTest)
		{
			Assert::IsTrue(EventTracing::Inst().SetLogEvent("MxPluginTeam", "MxUtils unit test", "product name", "0.0.0.0", 0, __FUNCTION__, __LINE__, "test" ));
		}

	};
}
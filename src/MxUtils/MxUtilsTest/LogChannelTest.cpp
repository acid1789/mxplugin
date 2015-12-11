#include "stdafx.h"
#include "CppUnitTest.h"
#include "MxUtils\MxUtils1.h"
#include "MxUtils\MxUtilsErrorCodes1.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MxUtilsTest
{
	TEST_CLASS(LogChannelTest)
	{
	private:
		std::string _msg;

	public:
		const char *RemovePreamble(const char *msg)
		{
			const char *rc = "";

			std::string temp(msg);
			size_t begin = std::string::npos;
			if ((begin = temp.find(")")) != std::string::npos)
			{
				_msg = temp.substr(begin+1, std::string::npos);
				rc = _msg.c_str();
			}
			return rc;
		}
		

		TEST_METHOD(LogChannelDevelopmentTest)
		{
			MxError::Inst().Initialise(ProdIdUnitTestApp::Owner(), ProdIdUnitTestApp::ProdID(), ProdIdUnitTestApp::Name(), "v0.0.0.0", MxError::Development, MxError::VerboseReport);

			MX_SETERROR(MX1002, MxError::CodeDefect, MxError::Abort, MxError::VerboseReport, "something is null");

			Assert::IsTrue(MxError::Inst().IsErrorSet());
			GUID guid;
			Assert::IsTrue(MxError::Inst().GetProductID(&guid));
			Assert::IsTrue(MxGUID::IsEqual(ProdIdUnitTestApp::ProdID(), guid));
			Assert::AreEqual("v0.0.0.0", MxError::Inst().GetProductVersion());
			Assert::AreEqual(0, (int)MxError::Inst().GetLogChannel());
			Assert::AreEqual(2, (int)MxError::Inst().GetReportLevel());

			Assert::AreEqual(MX1002, MxError::Inst().GetErrorCode());
			Assert::AreEqual(2, (int) MxError::Inst().GetErrorCategory());
			Assert::AreEqual(2, (int) MxError::Inst().GetErrorResetAction());
			Assert::AreEqual(2, (int) MxError::Inst().GetMsgLevel());

			Assert::AreEqual(" MxPluginTeam: MxUtilsTest::LogChannelTest::LogChannelDevelopmentTest, line=35, v0.0.0.0, Error=1002, CodeDefect, Abort: something is null\n", RemovePreamble(MxError::Inst().GetLastErrorStr()));

			MxError::Inst().Reset();
		}

		TEST_METHOD(LogChannelProductionTest)
		{
			MxError::Inst().Initialise(ProdIdUnitTestApp::Owner(), ProdIdUnitTestApp::ProdID(), ProdIdUnitTestApp::Name(), "v0.0.0.0", MxError::Production, MxError::VerboseReport);

			MX_SETERROR(MX1002, MxError::CodeDefect, MxError::Abort, MxError::VerboseReport, "something is null");

			Assert::IsTrue(MxError::Inst().IsErrorSet());
			GUID guid;
			Assert::IsTrue(MxError::Inst().GetProductID(&guid));
			Assert::IsTrue(MxGUID::IsEqual(ProdIdUnitTestApp::ProdID(), guid));
			Assert::AreEqual("v0.0.0.0", MxError::Inst().GetProductVersion());
			Assert::AreEqual(1, (int)MxError::Inst().GetLogChannel());
			Assert::AreEqual(2, (int)MxError::Inst().GetReportLevel());

			Assert::AreEqual(MX1002, MxError::Inst().GetErrorCode());
			Assert::AreEqual(2, (int) MxError::Inst().GetErrorCategory());
			Assert::AreEqual(2, (int) MxError::Inst().GetErrorResetAction());
			Assert::AreEqual(2, (int) MxError::Inst().GetMsgLevel());

			Assert::AreEqual(" MxPluginTeam: MxUtilsTest::LogChannelTest::LogChannelProductionTest, line=59, v0.0.0.0, Error=1002, CodeDefect, Abort: something is null\n", RemovePreamble(MxError::Inst().GetLastErrorStr()));

			MxError::Inst().Reset();
		}


		TEST_METHOD(LogChannelNoLoggingTest)
		{
			MxError::Inst().Initialise(ProdIdUnitTestApp::Owner(), ProdIdUnitTestApp::ProdID(), ProdIdUnitTestApp::Name(), "v0.0.0.0", MxError::NoLogging, MxError::VerboseReport);

			MX_SETERROR(MX1002, MxError::CodeDefect, MxError::Abort, MxError::VerboseReport, "something is null");

			Assert::IsTrue(MxError::Inst().IsErrorSet());
			GUID guid;
			Assert::IsTrue(MxError::Inst().GetProductID(&guid));
			Assert::IsTrue(MxGUID::IsEqual(ProdIdUnitTestApp::ProdID(), guid));
			Assert::AreEqual("v0.0.0.0", MxError::Inst().GetProductVersion());
			Assert::AreEqual(2, (int)MxError::Inst().GetLogChannel());
			Assert::AreEqual(2, (int)MxError::Inst().GetReportLevel());

			Assert::AreEqual(MX1002, MxError::Inst().GetErrorCode());
			Assert::AreEqual(2, (int) MxError::Inst().GetErrorCategory());
			Assert::AreEqual(2, (int) MxError::Inst().GetErrorResetAction());
			Assert::AreEqual(2, (int) MxError::Inst().GetMsgLevel());

			Assert::AreEqual("", RemovePreamble(MxError::Inst().GetLastErrorStr()));

			MxError::Inst().Reset();
		}

	};
}
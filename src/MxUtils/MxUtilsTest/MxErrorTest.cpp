#include "stdafx.h"
#include "CppUnitTest.h"
#include "MxUtils\MxUtils1.h"
#include "MxUtils\MxUtilsErrorCodes1.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MxUtilsTest
{
	TEST_CLASS(MxErrorTest)
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

		TEST_METHOD_INITIALIZE(InitMxErrorTest)
		{
			MxError::Inst().Initialise(ProdIdUnitTestApp::Owner(), ProdIdUnitTestApp::ProdID(), ProdIdUnitTestApp::Name(), "v0.0.0.0", MxError::Development, MxError::VerboseReport);
		}

		TEST_METHOD_CLEANUP(DeinitMxErrorTest)
		{
			MxError::Inst().Reset();
		}
		
		TEST_METHOD(SetErrorTest)
		{
			MX_SETERROR(MX1002, MxError::CodeDefect, MxError::Abort, MxError::VerboseReport, "something is null");

			Assert::IsTrue(MxError::Inst().IsErrorSet());
			Assert::AreEqual(" MxPluginTeam: MxUtilsTest::MxErrorTest::SetErrorTest, line=43, v0.0.0.0, Error=1002, CodeDefect, Abort: something is null\n", RemovePreamble(MxError::Inst().GetLastErrorStr()));
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

			Assert::AreEqual("", MxError::Inst().GetLastLogStr());
			Assert::AreEqual(L"", MxError::Inst().GetDatabaseErrorMsg());

			MxError::Inst().Reset();
			Assert::IsFalse(MxError::Inst().IsErrorSet());
		}

		TEST_METHOD(SetDbErrorTest)
		{
			MX_SETERROR(MX1002, MxError::Database, MxError::Abort, MxError::VerboseReport, "something is null.");

			Assert::IsTrue(MxError::Inst().IsErrorSet());
			Assert::AreEqual(" MxPluginTeam: MxUtilsTest::MxErrorTest::SetDbErrorTest, line=68, v0.0.0.0, Error=1002, Database, Abort: [database error: ] something is null.\n", RemovePreamble(MxError::Inst().GetLastErrorStr()));
			GUID guid;
			Assert::IsTrue(MxError::Inst().GetProductID(&guid));
			Assert::IsTrue(MxGUID::IsEqual(ProdIdUnitTestApp::ProdID(), guid));
			Assert::AreEqual(MX1002, MxError::Inst().GetErrorCode());
			Assert::AreEqual(8, (int)MxError::Inst().GetErrorCategory());
			Assert::AreEqual(2, (int)MxError::Inst().GetErrorResetAction());

			Assert::AreEqual("", MxError::Inst().GetLastLogStr() );
			Assert::AreEqual(L"", MxError::Inst().GetDatabaseErrorMsg());

		}

		TEST_METHOD(ErrorFrmtIntTest)
		{
			int x = 666;

			MX_SETERROR(MX1002, MxError::CodeDefect, MxError::Abort, MxError::VerboseReport, "int x = %d", x);
			Assert::AreEqual(" MxPluginTeam: MxUtilsTest::MxErrorTest::ErrorFrmtIntTest, line=88, v0.0.0.0, Error=1002, CodeDefect, Abort: int x = 666\n", RemovePreamble(MxError::Inst().GetLastErrorStr()) );
		}

		TEST_METHOD(ErrorFrmtStrTest)
		{
			const char *str = "test";

			MX_SETERROR(MX1002, MxError::CodeDefect, MxError::Abort, MxError::VerboseReport, "str = %s", str);
			Assert::AreEqual(" MxPluginTeam: MxUtilsTest::MxErrorTest::ErrorFrmtStrTest, line=96, v0.0.0.0, Error=1002, CodeDefect, Abort: str = test\n", RemovePreamble(MxError::Inst().GetLastErrorStr()) );
		}

		TEST_METHOD(ErrorFrmtDoubleTest)
		{
			double d = 9.99;

			MX_SETERROR(MX1002, MxError::CodeDefect, MxError::Abort, MxError::VerboseReport, "d = %1.2f", d);
			Assert::AreEqual(" MxPluginTeam: MxUtilsTest::MxErrorTest::ErrorFrmtDoubleTest, line=104, v0.0.0.0, Error=1002, CodeDefect, Abort: d = 9.99\n", RemovePreamble(MxError::Inst().GetLastErrorStr()) );
		}

		TEST_METHOD(GetWebsiteDefectReportUrlTest)
		{
			MX_SETERROR(MX1002, MxError::CodeDefect, MxError::Abort, MxError::VerboseReport, "something is null");
			std::string result;
			const char *url = MxError::Inst().GetWebsiteDefectReportUrl(&result, "http://www.unientry.org", IMxPluginBaseGUID );
			Assert::IsNotNull(url);
		}

	};
}
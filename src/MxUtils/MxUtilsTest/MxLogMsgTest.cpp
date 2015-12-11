#include "stdafx.h"
#include "CppUnitTest.h"
#include "MxUtils\MxUtils1.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MxUtilsTest
{
	TEST_CLASS(MxLogMsgTest)
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

	public:
		TEST_METHOD_INITIALIZE(InitMxLogMsgTest)
		{
			MxError::Inst().Initialise(ProdIdUnitTestApp::Owner(), ProdIdUnitTestApp::ProdID(), ProdIdUnitTestApp::Name(), "v0.0.0.0", MxError::Development, MxError::VerboseReport);
		}

		TEST_METHOD_CLEANUP(DeinitMxLogMsgTest)
		{
			MxError::Inst().Reset();
		}
		
		TEST_METHOD(LogFrmtIntTest)
		{
			int x = 666;

			MX_LOGMSG(MxError::VerboseReport, "int x = %d", x);

			Assert::AreEqual(" MxPluginTeam: MxUtilsTest::MxLogMsgTest::LogFrmtIntTest, line=44, v0.0.0.0, Log: int x = 666\n", RemovePreamble(MxError::Inst().GetLastLogStr()) );
		}

		TEST_METHOD(LogFrmtStrTest)
		{
			const char *str = "test";

			MX_LOGMSG(MxError::VerboseReport, "str = %s", str);

			Assert::AreEqual(" MxPluginTeam: MxUtilsTest::MxLogMsgTest::LogFrmtStrTest, line=53, v0.0.0.0, Log: str = test\n", RemovePreamble(MxError::Inst().GetLastLogStr()) );

		}

		TEST_METHOD(LogFrmtDoubleTest)
		{
			double d = 9.99;

			MX_LOGMSG(MxError::VerboseReport, "d = %1.2f", d);

			Assert::AreEqual(" MxPluginTeam: MxUtilsTest::MxLogMsgTest::LogFrmtDoubleTest, line=63, v0.0.0.0, Log: d = 9.99\n", RemovePreamble(MxError::Inst().GetLastLogStr()) );
		}

		TEST_METHOD(LogLevelVerboseTest)
		{
			MxError::Level level = MxError::VerboseReport;
			MX_LOGMSG(level, "MxError::Level=%d", (int)level);
			Assert::AreEqual(" MxPluginTeam: MxUtilsTest::MxLogMsgTest::LogLevelVerboseTest, line=71, v0.0.0.0, Log: MxError::Level=2\n", RemovePreamble(MxError::Inst().GetLastLogStr()) );

			level = MxError::AnalysisReport;
			MX_LOGMSG(level, "MxError::Level=%d", (int)level);
			Assert::AreEqual(" MxPluginTeam: MxUtilsTest::MxLogMsgTest::LogLevelVerboseTest, line=75, v0.0.0.0, Log: MxError::Level=1\n", RemovePreamble(MxError::Inst().GetLastLogStr()) );

			level = MxError::QuietReport;
			MX_LOGMSG(level, "MxError::Level=%d", (int)level);
			Assert::AreEqual(" MxPluginTeam: MxUtilsTest::MxLogMsgTest::LogLevelVerboseTest, line=79, v0.0.0.0, Log: MxError::Level=0\n", RemovePreamble(MxError::Inst().GetLastLogStr()) );
		}

		TEST_METHOD(LogLevelAnalysisTest)
		{
			MxError::Inst().Initialise(ProdIdUnitTestApp::Owner(), ProdIdUnitTestApp::ProdID(), ProdIdUnitTestApp::Name(), "v0.0.0.0", MxError::Development, MxError::AnalysisReport);
			
			MxError::Level level = MxError::VerboseReport;
			MX_LOGMSG(level, "MxError::Level=%d", (int)level);
			Assert::AreEqual("", MxError::Inst().GetLastLogStr() );

			level = MxError::AnalysisReport;
			MX_LOGMSG(level, "MxError::Level=%d", (int)level);
			Assert::AreEqual(" MxPluginTeam: MxUtilsTest::MxLogMsgTest::LogLevelAnalysisTest, line=92, v0.0.0.0, Log: MxError::Level=1\n", RemovePreamble(MxError::Inst().GetLastLogStr()) );

			level = MxError::QuietReport;
			MX_LOGMSG(level, "MxError::Level=%d", (int)level);
			Assert::AreEqual(" MxPluginTeam: MxUtilsTest::MxLogMsgTest::LogLevelAnalysisTest, line=96, v0.0.0.0, Log: MxError::Level=0\n", RemovePreamble(MxError::Inst().GetLastLogStr()) );
		}

		TEST_METHOD(LogLevelQuietTest)
		{
			MxError::Inst().Initialise(ProdIdUnitTestApp::Owner(), ProdIdUnitTestApp::ProdID(), ProdIdUnitTestApp::Name(), "v0.0.0.0", MxError::Development, MxError::QuietReport);

			MxError::Level level = MxError::VerboseReport;
			MX_LOGMSG(level, "MxError::Level=%d", (int)level);
			Assert::AreEqual("", MxError::Inst().GetLastLogStr() );

			level = MxError::AnalysisReport;
			MX_LOGMSG(level, "MxError::Level=%d", (int)level);
			Assert::AreEqual("", MxError::Inst().GetLastLogStr() );

			level = MxError::QuietReport;
			MX_LOGMSG(level, "MxError::Level=%d", (int)level);
			Assert::AreEqual(" MxPluginTeam: MxUtilsTest::MxLogMsgTest::LogLevelQuietTest, line=113, v0.0.0.0, Log: MxError::Level=0\n", RemovePreamble(MxError::Inst().GetLastLogStr()) );
		}

	};
}
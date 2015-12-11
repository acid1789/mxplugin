#include "stdafx.h"
#include "CppUnitTest.h"
#include "MxUtils\MxUtils1.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MxUtilsTest
{
	TEST_CLASS(DatabaseConnectionStrTest)
	{
	public:

		TEST_METHOD_INITIALIZE(InitDatabaseConnectionStrTest)
		{
			MxError::Inst().Initialise(ProdIdUnitTestApp::Owner(), ProdIdUnitTestApp::ProdID(), ProdIdUnitTestApp::Name(), "v0.0.0.0", MxError::Development, MxError::VerboseReport);
		}

		TEST_METHOD_CLEANUP(DeinitDatabaseConnectionStrTest)
		{
			MxError::Inst().Reset();
		}

		TEST_METHOD(SetUserIDPasswordTest)
		{
			std::wstring userid(L"");
			std::wstring pwd(L"");
			std::wstring connInputStr(L"");
			std::wstring connResultStr(L"");
			LPCTSTR res = L"";
											//null password
			res = MxUtils::SetUserIDPassword(&connInputStr, nullptr);
			Assert::IsNotNull(res);
			connResultStr = res;
			Assert::AreEqual(L"", connResultStr.c_str());		//failed

			pwd = L"";						//null conn
			res = MxUtils::SetUserIDPassword(nullptr, pwd.c_str());
			Assert::IsNotNull(res);
			connResultStr = res;
			Assert::AreEqual(L"", connResultStr.c_str());		//failed

			pwd = L"";						//empty password
			res = MxUtils::SetUserIDPassword(&connInputStr, pwd.c_str());
			Assert::IsNotNull(res);
			connResultStr = res;
			Assert::AreEqual(L"", connResultStr.c_str());		//failed

			pwd = L"hello";
			userid = L"";				    //empty UserId
			res = MxUtils::SetUserIDPassword(&connInputStr, pwd.c_str(), userid.c_str());
			Assert::IsNotNull(res);
			connResultStr = res;
			Assert::AreEqual(L"", connResultStr.c_str());		//failed

			pwd = L"h";						//min Password
			userid = L"m";				    //min UserId
			res = MxUtils::SetUserIDPassword(&connInputStr, pwd.c_str(), userid.c_str());
			Assert::IsNotNull(res);
			connResultStr = res;
			Assert::AreEqual(L"User ID=m;Password=h;", connResultStr.c_str());		//passed

			connInputStr = L"Password=x;";											//replace only Password
			pwd = L"h";						
			userid = L"m";				    
			res = MxUtils::SetUserIDPassword(&connInputStr, pwd.c_str());
			Assert::IsNotNull(res);
			connResultStr = res;
			Assert::AreEqual(L"Password=h;", connResultStr.c_str());

			connInputStr = L"Password=x";											//replace Password no terminating ;
			pwd = L"h";						
			userid = L"m";				    
			res = MxUtils::SetUserIDPassword(&connInputStr, pwd.c_str());
			Assert::IsNotNull(res);
			connResultStr = res;
			Assert::AreEqual(L"Password=h;", connResultStr.c_str());

			connInputStr = L"User ID=m;Password=x;";								//replace Password, ignore UserID
			pwd = L"h";						
			userid = L"m";				    
			res = MxUtils::SetUserIDPassword(&connInputStr, pwd.c_str());
			Assert::IsNotNull(res);
			connResultStr = res;
			Assert::AreEqual(L"User ID=m;Password=h;", connResultStr.c_str());		

			connInputStr = L"User ID=x;Password=x;";								//replace User ID and Password
			pwd = L"h";						
			userid = L"m";				    
			res = MxUtils::SetUserIDPassword(&connInputStr, pwd.c_str(), userid.c_str());
			Assert::IsNotNull(res);
			connResultStr = res;
			Assert::AreEqual(L"User ID=m;Password=h;", connResultStr.c_str());	

			connInputStr = L"Password=x;User ID=x;";								//switch User ID and Password
			pwd = L"h";						
			userid = L"m";				    
			res = MxUtils::SetUserIDPassword(&connInputStr, pwd.c_str(), userid.c_str());
			Assert::IsNotNull(res);
			connResultStr = res;
			Assert::AreEqual(L"Password=h;User ID=m;", connResultStr.c_str());	

			connInputStr = L"Password=x;User ID=x";									//no terminating ; for User ID 
			pwd = L"h";						
			userid = L"m";				    
			res = MxUtils::SetUserIDPassword(&connInputStr, pwd.c_str(), userid.c_str());
			Assert::IsNotNull(res);
			connResultStr = res;
			Assert::AreEqual(L"Password=h;User ID=m;", connResultStr.c_str());	

			connInputStr = L"Password=;User ID=;";									//empty User ID and Passwrd
			pwd = L"h";						
			userid = L"m";				    
			res = MxUtils::SetUserIDPassword(&connInputStr, pwd.c_str(), userid.c_str());
			Assert::IsNotNull(res);
			connResultStr = res;
			Assert::AreEqual(L"Password=h;User ID=m;", connResultStr.c_str());	

			connInputStr = L"Password=;User ID=;";									//longer User ID and Passwrd
			pwd = L"h1";						
			userid = L"m1";				    
			res = MxUtils::SetUserIDPassword(&connInputStr, pwd.c_str(), userid.c_str());
			Assert::IsNotNull(res);
			connResultStr = res;
			Assert::AreEqual(L"Password=h1;User ID=m1;", connResultStr.c_str());	

			connInputStr = L"Password=;User ID=;";									//3 char User ID and Passwrd
			pwd = L"h12";						
			userid = L"m12";				    
			res = MxUtils::SetUserIDPassword(&connInputStr, pwd.c_str(), userid.c_str());
			Assert::IsNotNull(res);
			connResultStr = res;
			Assert::AreEqual(L"Password=h12;User ID=m12;", connResultStr.c_str());	

			connInputStr = L"Password=12;User ID=12;";								//replace shorter User ID and Passwrd
			pwd = L"h";						
			userid = L"m";				    
			res = MxUtils::SetUserIDPassword(&connInputStr, pwd.c_str(), userid.c_str());
			Assert::IsNotNull(res);
			connResultStr = res;
			Assert::AreEqual(L"Password=h;User ID=m;", connResultStr.c_str());
																					//typical replace User ID and Passwrd
			connInputStr = L"Data Source=URADatabase;Initial Catalog=osImageIR1;Password=xxxxxx;User ID=Will;Persist Security Info=True;";								
			pwd = L"Hello";						
			userid = L"Dave";				    
			res = MxUtils::SetUserIDPassword(&connInputStr, pwd.c_str(), userid.c_str());
			Assert::IsNotNull(res);
			connResultStr = res;
			Assert::AreEqual(L"Data Source=URADatabase;Initial Catalog=osImageIR1;Password=Hello;User ID=Dave;Persist Security Info=True;", connResultStr.c_str());
																					//typical add User ID and Passwrd
			connInputStr = L"Data Source=URADatabase;Initial Catalog=osImageIR1;Persist Security Info=True;";								
			pwd = L"Hello";						
			userid = L"Dave";				    
			res = MxUtils::SetUserIDPassword(&connInputStr, pwd.c_str(), userid.c_str());
			Assert::IsNotNull(res);
			connResultStr = res;
			Assert::AreEqual(L"Data Source=URADatabase;Initial Catalog=osImageIR1;Persist Security Info=True;User ID=Dave;Password=Hello;", connResultStr.c_str());
																					//no terminating ; in connection
			connInputStr = L"Data Source=URADatabase;Initial Catalog=osImageIR1;Persist Security Info=True";								
			pwd = L"Hello";						
			userid = L"Dave";				    
			res = MxUtils::SetUserIDPassword(&connInputStr, pwd.c_str(), userid.c_str());
			Assert::IsNotNull(res);
			connResultStr = res;
			Assert::AreEqual(L"Data Source=URADatabase;Initial Catalog=osImageIR1;Persist Security Info=True;User ID=Dave;Password=Hello;", connResultStr.c_str());


		}

		TEST_METHOD(IsValidDbConnectionTest)
		{
			Assert::IsFalse(MxUtils::IsValidDbConnection(nullptr));

			std::wstring connStr(L"");
			Assert::IsFalse(MxUtils::IsValidDbConnection(connStr.c_str()));
																					//success
			connStr = L"Data Source=URADatabase;Initial Catalog=osImageIR1;Password=Hello;User ID=Dave;Persist Security Info=True;";
			Assert::IsTrue(MxUtils::IsValidDbConnection(connStr.c_str()));
																					//no UserID
			connStr = L"Data Source=URADatabase;Initial Catalog=osImageIR1;Password=Hello;Persist Security Info=True;";
			Assert::IsFalse(MxUtils::IsValidDbConnection(connStr.c_str()));
																					//empty UserID
			connStr = L"Data Source=URADatabase;Initial Catalog=osImageIR1;Password=Hello;User ID=;Persist Security Info=True;";
			Assert::IsFalse(MxUtils::IsValidDbConnection(connStr.c_str()));
																					//dup UserID
			connStr = L"Data Source=URADatabase;Initial Catalog=osImageIR1;Password=Hello;User ID=Dave;User ID=Wills;Persist Security Info=True;";
			Assert::IsFalse(MxUtils::IsValidDbConnection(connStr.c_str()));
																					//no Password
			connStr = L"Data Source=URADatabase;Initial Catalog=osImageIR1;User ID=Dave;Persist Security Info=True;";
			Assert::IsFalse(MxUtils::IsValidDbConnection(connStr.c_str()));
																					//empty Password
			connStr = L"Data Source=URADatabase;Initial Catalog=osImageIR1;Password=;User ID=Dave;Persist Security Info=True;";
			Assert::IsFalse(MxUtils::IsValidDbConnection(connStr.c_str()));
																					//dup Password
			connStr = L"Data Source=URADatabase;Initial Catalog=osImageIR1;Password=xxx;User ID=Dave;Password=yyy;Persist Security Info=True;";
			Assert::IsFalse(MxUtils::IsValidDbConnection(connStr.c_str()));
																					//no Data Source
			connStr = L"Initial Catalog=osImageIR1;Password=Hello;User ID=Dave;Persist Security Info=True;";
			Assert::IsFalse(MxUtils::IsValidDbConnection(connStr.c_str()));
																					//no Initial Catalog
			connStr = L"Data Source=URADatabase;Password=Hello;User ID=Dave;Persist Security Info=True;";
			Assert::IsFalse(MxUtils::IsValidDbConnection(connStr.c_str()));

		}

		TEST_METHOD(GetElementValueFromConnStrTest)
		{
			std::wstring connStr(L"Data Source=URADatabase;Password=Hello;User ID=Dave;Persist Security Info=True;");
			std::wstring value(L"");

			Assert::IsFalse(MxUtils::GetElementValueFromConnStr(nullptr,nullptr, nullptr));
			Assert::IsFalse(MxUtils::GetElementValueFromConnStr(&value,nullptr, nullptr));
			Assert::IsFalse(MxUtils::GetElementValueFromConnStr(&value, connStr.c_str(), nullptr));

			Assert::IsTrue(MxUtils::GetElementValueFromConnStr(&value, connStr.c_str(), L"Data Source"));
			Assert::AreEqual(L"URADatabase", value.c_str());
			Assert::IsTrue(MxUtils::GetElementValueFromConnStr(&value,connStr.c_str(), L"Password"));
			Assert::AreEqual(L"Hello", value.c_str());
			Assert::IsTrue(MxUtils::GetElementValueFromConnStr(&value,connStr.c_str(), L"User ID"));
			Assert::AreEqual(L"Dave", value.c_str());

		}

	};
}
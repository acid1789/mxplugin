// MxUtilsApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "MxUtils\MxUtils1.h"
#include "MxUtils\MxVersionInfo1.h"

int _tmain(int argc, _TCHAR* argv[])
{
	MxError::LogChannel channel = MxError::Development;
	MxVersionInfo app;

	std::wstring argProduction = L"/Log=Production";
	if ((argc == 2) && (argProduction == argv[1]))
		channel = MxError::Production;

			//initialise
	MX_SETUP (app.GetOwner(), PRODID_MxUtils1, app.GetProductName(),  app.GetVersion(), channel, MxError::VerboseReport, "MxUtilsApp starts");
	std::cout << MxError::Inst().GetProductName() << " v" << MxError::Inst().GetProductVersion() << ((channel == MxError::Development) ? " /Log=Development" : " /Log=Production") << std::endl;

	std::cout << MxUtils::Inst().GetProductName() << " v" << MxUtils::Inst().GetVersion() << std::endl;


	MX_SETERROR(MX1002, MxError::CodeDefect, MxError::Abort, MxError::VerboseReport, "***test***");
	MX_LOGMSG(MxError::VerboseReport, "test MX_SETERROR invoked");

			//terminating
	if (MxError::Inst().IsErrorSet())
	{
		std::cout << MxError::Inst().GetLastErrorStr();
		MxError::Inst().Reset();
	}

	std::cout <<app.GetProductName() << " ends" << std::endl;
	MX_END(MxError::VerboseReport, "MxUtilsApp ends");
	
	return 0;
}


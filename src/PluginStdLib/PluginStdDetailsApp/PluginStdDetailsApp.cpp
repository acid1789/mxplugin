// PluginStdDetailsApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdlib.h>
#include <iostream>
#include <string>

#include "PluginStdLib/PluginStdDetailsLib/PluginStdDetailsLib.h"


int main(int argc, char* argv[])
{
	MxVersionInfo app;
	MxError::LogChannel channel = MxError::Development;

	std::string argProduction = "/Log=Production";
	if ((argc == 2) && (argProduction == argv[1]))
		channel = MxError::Production;

	PluginStdDetailsLib lib;	

				//initialise
	MX_SETUP (app.GetOwner(), ProdIdPluginStdLibApp::ProdID(), app.GetProductName(), app.GetVersion(), channel, MxError::VerboseReport, "app starts");  //get Channel, ReportLevel from args (or registry); get v from VS_VERSION_INFO
	std::cout << MxError::Inst().GetProductName() << MxError::Inst().GetProductVersion() << ((channel == MxError::Development) ? " /Log=Development" : " /Log=Production") << std::endl << std::endl;

				//main body
	USES_CONVERSION; 
	#pragma warning(suppress: 6255)
	const char *pathfilename = W2CA(lib.GetLibPathFilename());
	std::cout << "PathFilename: " << ((pathfilename != nullptr) ? pathfilename : "(GetLibPathFilename is null)") << std::endl;
	std::cout << std::endl;

	std::cout << "Lib Name: " << lib.GetLibName() << std::endl;
	std::cout << "v" << lib.GetLibVersion() << std::endl;
	std::cout << "Description:" << lib.GetLibDescription() << std::endl;
	std::cout << "LibID=" << lib.GetLibID() << std::endl;
	std::cout << "Implements InterfaceName=" << lib.GetInterfaceName() << std::endl;
	std::string tmp;
	std::cout << "Implements InterfaceID=" << MxGUID::ConvertToString(lib.GetInterfaceGUID(), &tmp) << std::endl;
	std::cout << std::endl;
	std::cout << "Developer: " << lib.GetLibOwnerName() << std::endl;
	std::cout << "IPR: " << lib.GetLibCopyrightNotice() << std::endl;
	std::cout << "Supported at " << lib.GetLibSupportSite() << std::endl;
	std::cout << "Licence name: " << lib.GetLibLicenseName() << std::endl;
	std::cout << "Licence details: " << lib.GetLibLicenseURL() << std::endl;
	std::cout << std::endl;

	std::cout << "Developed to support App: " << MxGUID::ConvertToString(lib.GetSocketID(), &tmp) << std::endl;
	std::cout << std::endl;
	std::cout << "Comment: " << lib.GetComment() << std::endl;
	std::cout << std::endl;

			//terminating
	if (MxError::Inst().IsErrorSet())
	{
		std::cout << MxError::Inst().GetLastErrorStr() << MxError::Inst().GetLastLogStr();
		MxError::Inst().Reset();
	}
	MX_END(MxError::VerboseReport, "app ends");
	std::cout <<app.GetProductName() << " ends" << std::endl;

	return 0;
}


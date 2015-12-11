// MxPluginManagerApp.cpp : Defines the entry point for the console application.
//



#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
#include <string>

#include "PluginStdLib\IPluginStdLib.h"
#include "MxPluginManager\MxPluginManager1.h"
#include "MxProductID.h"


int main(int argc, char* argv[])
{
	MxVersionInfo app;
	MxError::LogChannel channel = MxError::Development;

	std::string argProduction = "/Log=Production";
	if ((argc == 2) && (argProduction == argv[1]))
		channel = MxError::Production;

//initialise
	MX_SETUP (app.GetOwner(), PRODID_MxPluginManagerApp, app.GetProductName(), app.GetVersion(), channel, MxError::VerboseReport, "app starts");  //get Channel, ReportLevel from args (or registry); get v from VS_VERSION_INFO
	std::cout << MxError::Inst().GetProductName() << MxError::Inst().GetProductVersion() << ((channel == MxError::Development) ? " /Log=Development" : " /Log=Production") << std::endl << std::endl;

	MxPluginManager lib(channel, MxError::VerboseReport);

//main body
	try
	{
		USES_CONVERSION; 

		std::cout << W2CA(lib.GetLibPathFilename()) << std::endl << std::endl;

		std::cout << "[" << lib.GetLibInternalName() << "] "  << std::endl;
		std::cout << lib.GetLibName() << " v" << lib.GetLibVersion() << " - " << lib.GetLibCopyrightNotice() << std::endl;
		std::cout << "support: " << lib.GetLibSupportSite() << std::endl;
		std::cout << "licence: " << lib.GetLibLicenseName() << " (" << lib.GetLibLicenseURL() << ")" << std::endl;
		std::cout << "description: " << lib.GetLibDescription() << std::endl << std::endl;

		std::wstring thirdpartLibs( L"C:\\Program Files (x86)\\Maximodex\\Common\\Bin" );
		thirdpartLibs += L"\\{8CB84F11-8199-4508-A081-FDADB7015ABD}";		//PRODID_PluginStdLib

		if (lib.RefreshPluginsInRootFolder(SID_Universal, true) == true)
		{
			std::cout << "-------------------------------------------------------------------------------" << std::endl;
			MxPluginLib *stdlib = nullptr;
			std::cout << "Plugins found=" << lib.GetPluginCount() << std::endl << std::endl;
	//start main loop
			for (int x = 0; x < lib.GetPluginCount(); x++)
			{
				if ((stdlib = lib.GetPlugin(x)) == nullptr)
					MX_SETERROR(MX1501, MxError::CodeDefect,  MxError::Abort,  MxError::QuietReport, "lib.GetPlugin(%d) returns nullptr", x); 
				else
				{
					std::cout << "..............................................................................." << std::endl;
					IPluginStdLib *plugin = nullptr;							//CREATE OBJ THAT IMPLEMENTS IPluginStdLibIID::GUID()
					if ((plugin =  static_cast<IPluginStdLib *>(stdlib->CreateClassInstance(IPluginStdLibIID::Guid()))) == nullptr)
						MX_SETERROR(MX1502, MxError::Install,  MxError::Abort,  MxError::VerboseReport, "CreateClassInstance for plugin %d returns nullptr", x); 
					else
					{															//USE OBJ                    
						std::cout << plugin->GetLibName() << " v" << plugin->GetLibVersion() <<  std::endl;				
						std::cout << plugin->GetLibDescription() << std::endl << std::endl;	
						plugin->DestroyPtr();									//TELL PLUGIN TO DELETE OBJ 
						plugin = nullptr;
					}
				}
			}
			std::cout << "-------------------------------------------------------------------------------" << std::endl;
		}
	}
	catch(std::exception const& e)
	{
		MX_SETERROR(MX1503, MxError::CodeDefect,  MxError::Abort,  MxError::QuietReport, "Unexpected exception: %s", e.what()); 
	}
//end main loop

//terminating
			//terminating
	if (MxError::Inst().IsErrorSet())
	{
		std::cout << MxError::Inst().GetLastErrorStr() << MxError::Inst().GetLastLogStr();
		MxError::Inst().Reset();
	}
	MX_END(MxError::VerboseReport, "ends");
	std::cout <<app.GetProductName() << " ends" << std::endl;

	return 0;
}


// MxPluginXmlApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <atlconv.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "MxProductID.h"
#include "MxPluginManager\MxPluginManager1.h"
#include "MxUtils\MxUtils1.h"

#include "PluginXml\IPluginXmlDoc.h"
#include "PluginXml\IPluginXmlElement.h"
#include "PluginXml\PluginXmlErrorCodes.h"

#include "PluginXml/MxPluginXml/PluginXml.h"

bool DoRuntimeLoad();

int main(int argc, char* argv[])
{
			//initialise
	MxVersionInfo app;

	MxError::LogChannel channel = MxError::Development;
	std::string argProduction = "/Log=Production";
	if ((argc > 1) && (argProduction == argv[1]))
		channel = MxError::Production;

	bool runtimeLoad = false;
	std::string argRuntimeLoad = "/Load=Runtime";	
	if ((argc > 2) && (argRuntimeLoad == argv[2]))
		runtimeLoad = true;

	MX_SETUP(app.GetOwner(), PRODID_PluginXmlApp, app.GetProductName(), app.GetVersion(), channel, MxError::VerboseReport, "app starts");  //get Channel, ReportLevel from args (or registry); get v from VS_VERSION_INFO
	std::cout << MxError::Inst().GetProductName() << " v" << MxError::Inst().GetProductVersion() << " ";
	std::cout << ((channel == MxError::Development) ? "/Log=Development" : argProduction) << " ";
	std::cout << ((runtimeLoad == false) ? "/Load=Loadtime" : argRuntimeLoad );
	std::cout << std::endl << std::endl;

			//PluginXML loaded at load time of PluginXmlApp as it is linked with stub PluginXml.lib and PluginXml.dll is located in same directory
	PluginXml lib(channel, MxError::VerboseReport);
	
	USES_CONVERSION; 
	#pragma warning(suppress: 6255)
	std::cout << W2CA(lib.GetLibPathFilename()) << std::endl << std::endl;

	std::cout << lib.GetLibName() << ", " << lib.GetLibVersion() << ", " << lib.GetLibDescription() << std::endl;
	std::cout << "Developed by " << lib.GetLibOwnerName() << ", " << lib.GetLibCopyrightNotice() << std::endl;
	std::cout << "Licence name: " << lib.GetLibLicenseName() << ", published at " << lib.GetLibLicenseURL() << std::endl;
	std::cout << "Support available from " << lib.GetLibSupportSite() << std::endl;
	std::string tmp;
	std::cout << "LibID=" << lib.GetLibID() << " SID=" << MxGUID::ConvertToString(lib.GetSocketID(), &tmp) << std::endl;
	std::cout << "Interface " << lib.GetInterfaceName() << " [" << MxGUID::ConvertToString(lib.GetInterfaceGUID(), &tmp) << "]" << std::endl  << std::endl;

	std::cout << lib.GetComment() << std::endl << std::endl;

			//PluginXML loaded at run time of PluginXmlApp - this requires that PluginXml.dll is installed in C:\Program Files (x86)\Maximodex\Common\Bin\{9E8E6865-D31E-40A2-B913-59BEBC11BC41} and Feature=Core are installed - see InstallNotes	
	if (runtimeLoad == false)
		std::cout << "INFO: runtimeLoad is false, not creating testApp.xml" << std::endl;
	else
	{
		if (DoRuntimeLoad() == false)
			std::cout << "FAIL: failed to create testApp.xml - have you copied PluginXml.dll into PRODID_DefaultPluginRootFolder?" << std::endl;
		else
			std::cout << "SUCCESS: created testApp.xml" << std::endl;
	}		

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

bool DoRuntimeLoad()
{
	bool rc = false;

	MxPluginManager pluginMngr;

	if(pluginMngr.RefreshPluginsInRootFolder( SID_Universal, true) == true)
	{
		MxPluginLib *xmlLib = pluginMngr.GetPlugin(PRODID_PluginXml);
		if (xmlLib != nullptr)
		{
			IPluginXmlDoc *doc = nullptr;
			if ((doc =  static_cast<IPluginXmlDoc *>(xmlLib->CreateClassInstance(IPluginXmlDocIID::Guid()))) != nullptr)
			{
				if (doc->New("Doc", "PluginXmlApp creates testApp.xml", "1.1", true) == true)
				{
					IPluginXmlElement *root = nullptr;
					if ((root = doc->GetRootElement()) != nullptr)
					{
						IPluginXmlElement *element = nullptr;
						if ((element =  static_cast<IPluginXmlElement *>(xmlLib->CreateClassInstance(IPluginXmlElementIID::Guid()))) != nullptr)
						{
							if (element->SetValue("intro") == true)
							{
								if (element->SetText("hello world!") == true)
								{
									if (element->SetAttributeString("comment", "so said ritchie") == true)
									{
										if (root->LinkEndChild(element) == true)
										{
											rc = true;
										}
									}
								}
							}
							element->DestroyPtr();
						}
						root->DestroyPtr();
					}
					doc->SaveAs("testApp.xml");
					if (doc->Close() == false)
						rc = false;
				}
				doc->DestroyPtr();
			}
		}
	}
	if (rc == true)
	{
		std::ifstream appXmlFile;
		appXmlFile.open ("testApp.xml");
		if (appXmlFile.bad() == false)
		{
			std::cout << std::endl << appXmlFile.rdbuf() << std::endl;
			appXmlFile.close();
		}
		std::cout << std::endl;
	}
	return rc;
}
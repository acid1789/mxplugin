// PluginImageProcXApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdlib.h>
#include <iostream>
#include <string>
#include <atlconv.h>

#include "PluginImageProcX/PluginImageProcXLib/PluginImageProcXLib.h"


int main(int argc, char* argv[])
{
	int rc = 0;

	std::cout << "PluginImageProcXApp v1.0.0.5" << std::endl << std::endl;

	PluginImageProcXLib lib;

	USES_CONVERSION; 

	std::cout << "PathFilename: " << W2CA(lib.GetLibPathFilename()) << std::endl;
	std::cout << std::endl;

	std::cout << std::endl;
	std::cout << "Lib Name: " << lib.GetLibName() << std::endl;
	std::cout << "v" << lib.GetLibVersion() << std::endl;
	std::cout << "Description:" << lib.GetLibDescription() << std::endl;
	std::cout << "LibID=" << lib.GetLibID() << std::endl;
	std::cout << "Implements Interface" << lib.GetInterfaceName() << std::endl;
	std::cout << "Implements InterfaceID=" << lib.GetInterfaceGUID() << std::endl;
	std::cout << std::endl;
	std::cout << "Developer: " << lib.GetLibOwnerName() << std::endl;
	std::cout << "IPR: " << lib.GetLibCopyrightNotice() << std::endl;
	std::cout << "Supported at " << lib.GetLibSupportSite() << std::endl;
	std::cout << "Licence name: " << lib.GetLibLicenseName() << std::endl;
	std::cout << "Licence details: " << lib.GetLibLicenseURL() << std::endl;
	std::cout << std::endl;
	std::cout << "Developed to support App: " << lib.GetSocketAppID() << std::endl;
	std::cout << std::endl;
	std::cout << "Api: " << lib.GetLibApiDetails() << std::endl;
	std::cout << std::endl;


	std::cout << "progam ends"  << std::endl;

	return rc;
}

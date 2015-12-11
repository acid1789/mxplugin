#include "stdafx.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

#include "PluginStdInterfaceLib.h"

int _tmain(int argc, _TCHAR* argv[])
{	
	std::cout << "***PluginStdInterfaceApp v1.0.0.7***" << std::endl;

	std::cout << "Instantiate and use an object of the class PluginStdInterfaceLib contained in a DLL loaded at startup as a dependency of this app because it is linked with PluginStdInterfaceLib.lib" << std::endl << std::endl;
	std::cout << "PluginStdInterface extends PluginSimpleObj by providing a COM-like interface" << std::endl << std::endl; 

	PluginStdInterfaceLib *obj = new PluginStdInterfaceLib();
	std::cout << "PluginStdInterfaceLib object created" << std::endl;

	std::cout << obj->GetComment() << std::endl;

	delete obj;
	std::cout << "PluginStdInterfaceLib object deleted" << std::endl << std::endl;

	std::cout << "SocketApp similarly instantiates the object, but loads PluginStdInterfaceLib.dll during its execution - no stub lib needed" << std::endl;

	std::cout << "program ends ok" << std::endl;
	return 0;
}




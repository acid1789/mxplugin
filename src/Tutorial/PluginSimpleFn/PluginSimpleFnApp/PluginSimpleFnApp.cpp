// PluginSimpleFnApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "..\..\..\PublicInterfaces\Tutorial\ISimpleFn.h"


int _tmain(int argc, _TCHAR* argv[])
{	
	std::cout << "***PluginSimpleFnApp v1.0.0.7***" << std::endl;

	std::cout << "Invoke a Simple Function contained in a DLL loaded at startup as a dependency of this app because it is linked with PluginSimpleFnLib.lib" << std::endl << std::endl;
	std::cout << SimpleFunction() << std::endl << std::endl;

	std::cout << "SocketApp invokes this same function, but loads PluginSimpleFnLib.dll during its execution - no stub lib needed" << std::endl;
	std::cout << "***PluginSimpleFnApp ends ok***" << std::endl;
	return 0;
}


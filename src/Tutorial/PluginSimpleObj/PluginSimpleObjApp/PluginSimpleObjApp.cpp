// PluginSimpleObjApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "SimpleObj.h"


int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "***PluginSimpleObjApp v1.0.0.7***" << std::endl;

	std::cout << "Instantiate and use an object of the class SimpleObj contained in a DLL loaded at startup as a dependency of this app because it is linked with PluginSimpleObjLib.lib" << std::endl << std::endl;

	ISimpleObj* Obj = Create();
	std::cout << "SimpleObj created" << std::endl;

	std::cout << Obj->GetMsg() << std::endl;

	Obj->Delete();
	std::cout << "SimpleObj deleted" << std::endl << std::endl;

	std::cout << "SocketApp similarly instantiates the object, but but loads PluginSimpleObjLib.dll during its execution - no stub lib needed" << std::endl;

	std::cout << "***PluginSimpleObjApp ends ok***" << std::endl;
	return 0;
}


// Socket.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <atlconv.h>

#include "..\..\..\PublicInterfaces\Tutorial\ISimpleFn.h"
#include "..\..\..\PublicInterfaces\Tutorial\ISimpleObj.h"
#include "..\..\..\PublicInterfaces\Tutorial\IPluginStdInterface.h"


HINSTANCE LoadDLL(std::string);

void CallSimpleObj(HINSTANCE);

void CallSimpleFn(HINSTANCE);

void CallStdInterface(HINSTANCE);


int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "***SocketApp v1.0.0.7***" << std::endl;

	std::string FileNameSimpleFn = "PluginSimpleFnLib.dll";
	std::string FileNameSimpleObj = "PluginSimpleObjLib.dll";
	std::string FileNameStdInterfaceLib = "PluginStdInterfaceLib.dll";

	std::cout << "Instantiate and use objects or functions contained in various DLLs loaded at runtime rather than startup - no stub libs needed" << std::endl << std::endl;


	/***Simple Function ***/
	HINSTANCE hInstance = NULL;
	if ((hInstance = LoadDLL(FileNameSimpleFn)) != NULL)
	{
		CallSimpleFn(hInstance);
		::FreeLibrary(hInstance);
		hInstance = NULL;
	}

	/***Simple Object ***/
	if ((hInstance = LoadDLL(FileNameSimpleObj)) != NULL)
	{
		CallSimpleObj(hInstance);
		::FreeLibrary(hInstance);
		hInstance = NULL;
	}

	/***PluginStdInterfaceLib***/
	if ((hInstance = LoadDLL(FileNameStdInterfaceLib)) != NULL)
	{
		CallStdInterface(hInstance);
		::FreeLibrary(hInstance);
		hInstance = NULL;
	}

	std::cout << std::endl << std::endl << "***SocketApp ends*** \n\n" << std::endl;

	return 0;
}


void CallSimpleFn(HINSTANCE hInstance)
{
	const char* (*pfn)(void) = 0;			//type of this pointer has to be the same as "SimpleFunction"
	try
	{
		*(FARPROC *)&pfn = ::GetProcAddress(hInstance, "SimpleFunction");
		if (pfn == nullptr)
			std::cout << "pfn not initialised: GetLastError() = " << ::GetLastError() << std::endl;
		else
		{
			std::cout << "call SimpleFn: ";
			std::cout << pfn() << std::endl;
		}
	}
	catch(std::exception const& e)
	{
		std::cout << "Error: unexpected exception" << e.what() << std::endl;
	}
}

void CallSimpleObj(HINSTANCE hInstance)
{	
	ISimpleObj *Obj = '\0';
	ISimpleObj * (*pfn)(void) = 0;			
	try 
	{
		*(FARPROC *)&pfn = ::GetProcAddress(hInstance, "Create");
		if (pfn == nullptr)
			std::cout << "pfn not initialised: GetLastError() = " << ::GetLastError() << std::endl;
		else
		{	
			Obj = pfn();
			std::cout << "SimpleObj instantiated ok" << std::endl;
			std::cout << Obj->GetMsg() << std::endl;	//same as (*Obj).GetMsg()
			Obj->Delete();
		}
	}
	catch(std::exception const& e)
	{
		std::cout << "Error: unexpected exception" << e.what() << std::endl;
	}
}

void CallStdInterface(HINSTANCE hInstance)
{
	void * (MXSTDMETHOD *pfn)(const GUID) = 0;

	*(FARPROC *)&pfn = ::GetProcAddress(hInstance, "CreateClassInstance");
	if (pfn == nullptr)
		std::cout << "pfn not initialised: GetLastError() = " << ::GetLastError() << std::endl;
	else
	{
		IPluginStdInterfaceLib * obj = static_cast<IPluginStdInterfaceLib * >(pfn(IPluginStdInterfaceLibIID::Guid()));
		if (obj == nullptr)
			std::cout << "failed to create object that implements IPluginStdInterfaceLib" << std::endl;
		else
		{
			std::cout << "Object that implements " << obj->GetInterfaceName() << " instantiated ok" << std::endl;
			std::cout << obj->GetComment() << std::endl;

			IMxPluginBase * baseObj = static_cast<IMxPluginBase * >(obj->Dynamic_cast(IMxPluginBaseIID::Guid()));
			if (baseObj == nullptr)
				std::cout << "failed to get object that implements IMxPluginBase" << std::endl;
			else
			{
				std::cout << baseObj->GetInterfaceName() << " also implements IMxPluginBase"  << std::endl;
				baseObj->DestroyPtr();
			}
			obj->DestroyPtr();
		}
	}
}


HINSTANCE LoadDLL(std::string FileName)
{	
	HINSTANCE rc = NULL;
	try
	{	
		USES_CONVERSION; 
		const wchar_t *dllFile = A2CW(FileName.c_str());

		if((dllFile == nullptr) || ((rc = ::LoadLibrary(dllFile)) == NULL))
			std::cout << std::endl << "Error: " << FileName.c_str() << "  Load failed" << std::endl;
		else
			std::cout << std::endl << "Success: " << FileName.c_str() << " Loaded ok." << std::endl;
	}
	catch(std::exception const& e)
	{
		std::cout << "Error: unexpected exception" << e.what() << std::endl;
	}
	
	return rc;
}

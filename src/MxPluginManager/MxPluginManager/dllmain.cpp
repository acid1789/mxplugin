// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "MxUtils\LibInfo.h"


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			LibInfo::Inst().SetVersionInfo(hModule);
		}
		break;
		case DLL_PROCESS_DETACH:
		{

		}
		break;
		case DLL_THREAD_ATTACH:
		{
		}
		break;
		case DLL_THREAD_DETACH:
		{
			
		}
		break;
	}
	return TRUE;
}
#include "stdafx.h"
#include "MxUtils\LibInfo.h"


LibInfo::LibInfo(void) : 
	_VersionInfo(nullptr)
{
	_VersionInfo = new MxVersionInfo();
}


LibInfo::~LibInfo(void)
{
	CleanupOnAppExitOnly();
#ifdef _DEBUG
	::OutputDebugString(L"****************LibInfo::~LibInfo() MxPluginManager all allocated memory now freed ********************\n" );
#endif
}

void LibInfo::CleanupOnAppExitOnly()
{
	if (_VersionInfo != nullptr)
		delete _VersionInfo;
	_VersionInfo = nullptr;
}
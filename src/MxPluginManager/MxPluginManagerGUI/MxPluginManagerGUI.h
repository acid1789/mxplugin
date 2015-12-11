
// MxPluginManagerGUI.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "MxUtils\MxUtils1.h"
#include "MxPluginManager\MxPluginManager1.h"

// CMxPluginManagerGUIApp:
// See MxPluginManagerGUI.cpp for the implementation of this class
//

class CMxPluginManagerGUIApp : public CWinAppEx
{
public:
	static const wchar_t * const CMxPluginManagerGUIApp::RegGeneralSectionName;

	static const wchar_t * const CMxPluginManagerGUIApp::RegGeneralTraceName;
	static const wchar_t * const CMxPluginManagerGUIApp::RegGeneralTraceDefault;
	static const wchar_t * const CMxPluginManagerGUIApp::RegGeneralETWLogName;
	static const wchar_t * const CMxPluginManagerGUIApp::RegGeneralETWLogDefault;
	static const wchar_t * const CMxPluginManagerGUIApp::RegGeneralDebugViewerAppName;
	static const wchar_t * const CMxPluginManagerGUIApp::RegGeneralDebugViewerAppDefault;
	static const wchar_t * const CMxPluginManagerGUIApp::RegGeneralEventViewerCmdName;
	static const wchar_t * const CMxPluginManagerGUIApp::RegGeneralEventViewerCmdParamsName;
	static const wchar_t * const CMxPluginManagerGUIApp::RegGeneralLogmanQueryCmdName;
	static const wchar_t * const CMxPluginManagerGUIApp::RegGeneralLogmanStartCmdName;
	static const wchar_t * const CMxPluginManagerGUIApp::RegGeneralLogmanStopCmdName;
	static const wchar_t * const CMxPluginManagerGUIApp::RegGeneralLogmanStdOutStatusLineName;
	static const wchar_t * const CMxPluginManagerGUIApp::RegGeneralLogmanStdOutStatusValueName;
	static const wchar_t * const CMxPluginManagerGUIApp::RegGeneralLogmanStdOutSuccessLineName;
	static const wchar_t * const CMxPluginManagerGUIApp::RegGeneralLogmanStdOutErrorRunAsAdminLineName;
	static const wchar_t * const CMxPluginManagerGUIApp::RegGeneralLogmanStdOutErrorInvalidTraceLineName;


public:
	CMxPluginManagerGUIApp();
	~CMxPluginManagerGUIApp();

// Overrides
public:
	virtual BOOL InitInstance();
	bool SetFactoryDefaults();

// Implementation

	MxPluginManager* m_PluginMngr;
	MxVersionInfo *m_verInfo;

	DECLARE_MESSAGE_MAP()
};

extern CMxPluginManagerGUIApp theApp;
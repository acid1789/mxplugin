
// SocketImageProcXGUI.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "MxPluginBaseEditableIID.h"

// CSocketImageProcXGUIApp:
// See SocketImageProcXGUI.cpp for the implementation of this class
//

class CSocketImageProcXGUIApp : public CWinAppEx
{
public:
	static const wchar_t * const CSocketImageProcXGUIApp::RegSectionGeneral;
	static const wchar_t * const CSocketImageProcXGUIApp::RegKeyRecordedIIDCnt;
	static const wchar_t * const CSocketImageProcXGUIApp::RegSectionRecordedIIDs;
	static const wchar_t * const CSocketImageProcXGUIApp::RegKeyRecordedIIDGUID;
	static const wchar_t * const CSocketImageProcXGUIApp::RegKeyRecordedIIDName;
	static const wchar_t * const CSocketImageProcXGUIApp::RegKeyPluginRootFolder;

	static const int CSocketImageProcXGUIApp::AddNewIID = -1;	
public:
	CSocketImageProcXGUIApp();

	CString GetPluginRootFolder();
	bool	SetPluginRootFolder(CString &folder);

	int		GetRecordedIIDCnt();
	bool	DeleteAllRecordedIIDs(LSTATUS *status);
	bool	DeleteRecordedIID(int index, LSTATUS *status);
	bool	GetRecordedIID(int index, MxPluginBaseEditableIID *result);
	bool	SetRecordedIID(int index, MxPluginBaseEditableIID *value);

private:
	BOOL CSocketImageProcXGUIApp::RegDelnodeRecurse (HKEY hKeyRoot, LPTSTR lpSubKey);
	BOOL CSocketImageProcXGUIApp::RegDelnode (HKEY hKeyRoot, LPCTSTR lpSubKey);

	CString m_PluginRootFolder;
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation


	DECLARE_MESSAGE_MAP()
};

extern CSocketImageProcXGUIApp theApp;
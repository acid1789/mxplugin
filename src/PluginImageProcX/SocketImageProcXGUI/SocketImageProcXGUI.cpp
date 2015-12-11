
// SocketImageProcXGUI.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "resource.h"

#include "SocketImageProcXGUI.h"
#include "SocketImageProcXGUIDlg.h"
#include "MxPluginBaseEditableIID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const wchar_t * const CSocketImageProcXGUIApp::RegSectionGeneral = L"General";
const wchar_t * const CSocketImageProcXGUIApp::RegKeyRecordedIIDCnt = L"RecordedIIDCnt";
const wchar_t * const CSocketImageProcXGUIApp::RegSectionRecordedIIDs = L"RecordedIIDs";
const wchar_t * const CSocketImageProcXGUIApp::RegKeyRecordedIIDGUID = L"IID-GUID-";
const wchar_t * const CSocketImageProcXGUIApp::RegKeyRecordedIIDName = L"IID-Name-";
const wchar_t * const CSocketImageProcXGUIApp::RegKeyPluginRootFolder = L"PluginRootFolder";




// CSocketImageProcXGUIApp

BEGIN_MESSAGE_MAP(CSocketImageProcXGUIApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinAppEx::OnHelp)
END_MESSAGE_MAP()


// CSocketImageProcXGUIApp construction

CSocketImageProcXGUIApp::CSocketImageProcXGUIApp() : 
	m_PluginRootFolder("")
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CSocketImageProcXGUIApp object

CSocketImageProcXGUIApp theApp;


// CSocketImageProcXGUIApp initialization

BOOL CSocketImageProcXGUIApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	InitContextMenuManager();
	InitShellManager();
	InitKeyboardManager();
	InitTooltipManager();

	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);
	
	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
//	CShellManager *pShellManager = new CShellManager;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(IDS_REG_OWNER_ROOT);

	GetPluginRootFolder();

	CSocketImageProcXGUIDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Delete the shell manager created above.
	//if (pShellManager != NULL)
	//{
	//	delete pShellManager;
	//}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

bool	CSocketImageProcXGUIApp::DeleteAllRecordedIIDs(LSTATUS *status)
{
	bool rc = false;

	if (status != '\0')
	{
		*status = 0;
		HKEY hAppKey = NULL;
		if ((hAppKey = GetAppRegistryKey()) == NULL)
			*status =  MxPluginLibs::InvalidRegKey;
		else
		{
			if (CWinApp::WriteProfileInt(CSocketImageProcXGUIApp::RegSectionGeneral, CSocketImageProcXGUIApp::RegKeyRecordedIIDCnt, 0) == TRUE)
			{
				CString SubKey(CSocketImageProcXGUIApp::RegSectionRecordedIIDs);
				*status = ::RegDeleteKey(hAppKey, SubKey);
				if ((*status == ERROR_SUCCESS) || (*status == 2))		//2 = not found, presume already deleted
					rc = true;
			}
		}
	}
	return rc;
}

int	CSocketImageProcXGUIApp::GetRecordedIIDCnt()
{
	int rc  = 0;

	rc = CWinApp::GetProfileInt(CSocketImageProcXGUIApp::RegSectionGeneral, CSocketImageProcXGUIApp::RegKeyRecordedIIDCnt, 0);

	return rc;
}

bool	CSocketImageProcXGUIApp::DeleteRecordedIID(int index, LSTATUS *status)
{
	bool rc = false;

	int cnt = GetRecordedIIDCnt();
	if ((index >= 0) && (cnt > 0) && (status != '\0'))
	{
		*status = 0;
		if (CWinApp::WriteProfileInt(CSocketImageProcXGUIApp::RegSectionGeneral, CSocketImageProcXGUIApp::RegKeyRecordedIIDCnt, cnt-1) == TRUE)
		{
			HKEY hAppKey = NULL;
			if ((hAppKey = GetAppRegistryKey()) == NULL)
				*status =  MxPluginLibs::InvalidRegKey;
			else
			{
				CString SubKey(CSocketImageProcXGUIApp::RegSectionRecordedIIDs);
				CString KeyValue(L"");
				KeyValue.Format(L"%s%d", CSocketImageProcXGUIApp::RegKeyRecordedIIDName, index);
				if ((*status = ::RegDeleteKeyValue(hAppKey, SubKey, KeyValue)) == ERROR_SUCCESS)
				{
					KeyValue.Format(L"%s%d", CSocketImageProcXGUIApp::RegKeyRecordedIIDGUID, index);
					if ((*status = ::RegDeleteKeyValue(hAppKey,SubKey, KeyValue)) == ERROR_SUCCESS)
						rc = true;
				}
				if ( rc == false)
					CWinApp::WriteProfileInt(CSocketImageProcXGUIApp::RegSectionGeneral, CSocketImageProcXGUIApp::RegKeyRecordedIIDCnt, cnt);
			}
		}
	}
	return rc;
}

bool	CSocketImageProcXGUIApp::GetRecordedIID(int index, MxPluginBaseEditableIID *result)
{
	bool rc = false;

	if (result != '\0')
	{
		int cnt = GetRecordedIIDCnt();
		if ((index >= 0) && (index < cnt))
		{
			CString key(L"");
			key.Format(L"%s%d", CSocketImageProcXGUIApp::RegKeyRecordedIIDName, index);
			CString Name = CWinApp::GetProfileString(CSocketImageProcXGUIApp::RegSectionRecordedIIDs, key);
			result->SetName(Name);

			key.Format(L"%s%d", CSocketImageProcXGUIApp::RegKeyRecordedIIDGUID, index);
			CString GUID = CWinApp::GetProfileString(CSocketImageProcXGUIApp::RegSectionRecordedIIDs, key);
			result->SetGUID(GUID);

			rc = true;
		}
	}
	return rc;
}

bool	CSocketImageProcXGUIApp::SetRecordedIID(int index, MxPluginBaseEditableIID *value)
{
	bool rc = false;

	if (value != '\0')
	{
		int cnt = GetRecordedIIDCnt();
		if (index == CSocketImageProcXGUIApp::AddNewIID)
			index = cnt++;
		if ((index >= 0) && (index < cnt))
		{
			CString key;
			key.Format(L"%s%d", CSocketImageProcXGUIApp::RegKeyRecordedIIDName, index);
			if ( CWinApp::WriteProfileString(CSocketImageProcXGUIApp::RegSectionRecordedIIDs, key, value->GetName()) )
			{
				key.Format(L"%s%d", CSocketImageProcXGUIApp::RegKeyRecordedIIDGUID, index);
				if ( CWinApp::WriteProfileString(CSocketImageProcXGUIApp::RegSectionRecordedIIDs, key, value->GetGUID()) )
					rc = (CWinApp::WriteProfileInt(CSocketImageProcXGUIApp::RegSectionGeneral, CSocketImageProcXGUIApp::RegKeyRecordedIIDCnt, cnt) == TRUE) ? true : false;
			}
			if ( rc == false)
			{
				LSTATUS status = NULL;
				DeleteRecordedIID(index, &status); 
			}
		}
	}
	return rc;
}

bool	CSocketImageProcXGUIApp::SetPluginRootFolder(CString &folder)
{
	bool rc = false;

	m_PluginRootFolder = folder; 

	if (CWinApp::WriteProfileString(CSocketImageProcXGUIApp::RegSectionGeneral, CSocketImageProcXGUIApp::RegKeyPluginRootFolder, folder) != 0)
		rc = true;

	return rc;
}

CString CSocketImageProcXGUIApp::GetPluginRootFolder()
{
	CString rc(L""); 

	if (m_PluginRootFolder.GetLength() != 0)
		rc = m_PluginRootFolder;
	else
	{
		rc = CWinApp::GetProfileString(CSocketImageProcXGUIApp::RegSectionGeneral, CSocketImageProcXGUIApp::RegKeyPluginRootFolder);
		m_PluginRootFolder = rc;
	}
	
	return rc;
}
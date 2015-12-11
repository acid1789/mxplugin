
// MxPluginManagerGUI.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

#include "MxPluginManagerGUI.h"
#include "MxPluginManagerGUIDlg.h"
#include "ShowErrorDlg.h"
#include "OptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const wchar_t * const CMxPluginManagerGUIApp::RegGeneralSectionName = L"General";
const wchar_t * const CMxPluginManagerGUIApp::RegGeneralTraceName = L"TraceName";
const wchar_t * const CMxPluginManagerGUIApp::RegGeneralTraceDefault = L"MxUtilsTrace";
const wchar_t * const CMxPluginManagerGUIApp::RegGeneralETWLogName = L"ETWLogPathFilename";
const wchar_t * const CMxPluginManagerGUIApp::RegGeneralETWLogDefault = L"c:\\EMxUtilsTrace_000001.evtx";
const wchar_t * const CMxPluginManagerGUIApp::RegGeneralDebugViewerAppName = L"OutputDebugStrViewerApp";
const wchar_t * const CMxPluginManagerGUIApp::RegGeneralDebugViewerAppDefault = L"C:\\Program Files (x86)\\DebugView\\Dbgview.exe";
const wchar_t * const CMxPluginManagerGUIApp::RegGeneralEventViewerCmdName  = L"EventViewerCmd";
const wchar_t * const CMxPluginManagerGUIApp::RegGeneralEventViewerCmdParamsName = L"EventViewerCmdParams";
const wchar_t * const CMxPluginManagerGUIApp::RegGeneralLogmanQueryCmdName = L"LogmanQueryCmd";
const wchar_t * const CMxPluginManagerGUIApp::RegGeneralLogmanStartCmdName = L"LogmanStartCmd";
const wchar_t * const CMxPluginManagerGUIApp::RegGeneralLogmanStopCmdName = L"LogmanStopCmd";
const wchar_t * const CMxPluginManagerGUIApp::RegGeneralLogmanStdOutStatusLineName = L"LogmanStdOutStatusLine";
const wchar_t * const CMxPluginManagerGUIApp::RegGeneralLogmanStdOutStatusValueName = L"LogmanStdOutStatusValue";
const wchar_t * const CMxPluginManagerGUIApp::RegGeneralLogmanStdOutSuccessLineName = L"LogmanStdOutSuccessLine";
const wchar_t * const CMxPluginManagerGUIApp::RegGeneralLogmanStdOutErrorRunAsAdminLineName = L"LogmanStdOutErrorRunAsAdminLine";
const wchar_t * const CMxPluginManagerGUIApp::RegGeneralLogmanStdOutErrorInvalidTraceLineName = L"LogmanStdOutErrorInvalidTraceLine";


// CMxPluginManagerGUIApp

BEGIN_MESSAGE_MAP(CMxPluginManagerGUIApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinAppEx::OnHelp)
END_MESSAGE_MAP()


// CMxPluginManagerGUIApp construction

CMxPluginManagerGUIApp::CMxPluginManagerGUIApp() : 
	m_PluginMngr(nullptr),
	m_verInfo(nullptr)
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CMxPluginManagerGUIApp::~CMxPluginManagerGUIApp()
{
	if (m_PluginMngr != nullptr)
		delete m_PluginMngr;
	m_PluginMngr = nullptr;
	if (m_verInfo != nullptr)
		delete m_verInfo;
	m_verInfo = nullptr;
}

// The one and only CMxPluginManagerGUIApp object

CMxPluginManagerGUIApp theApp;


// CMxPluginManagerGUIApp initialization

BOOL CMxPluginManagerGUIApp::InitInstance()
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


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	//CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization

	MxVersionInfo *m_verInfo = new MxVersionInfo();
	ASSERT( MxGUID::IsEqual(PRODID_MxPluginManagerGUI, MxGUID::ConvertFromString(m_verInfo->GetProductID()) ));
	MX_SETUP(m_verInfo->GetOwner(), MxGUID::ConvertFromString(m_verInfo->GetProductID()), m_verInfo->GetProductName(), m_verInfo->GetVersion(), MxError::Development, MxError::VerboseReport, "App starts");

	try
	{
		USES_CONVERSION;
		
		SetRegistryKey(A2CW(m_verInfo->GetOwner()));

		m_PluginMngr = new MxPluginManager(MxError::Development, MxError::VerboseReport);
		if (m_PluginMngr->RegAppInit(m_verInfo->GetProductName(), MxGUID::ConvertFromString(m_verInfo->GetProductID()), ProdIdMxPluginManagerGUI::SID(), MxError::Development, MxError::VerboseReport) == false)
		{
			if (MxError::Inst().GetLastErrorCode() != MxPluginManager::ErrorSetFactoryDefaultNeeded)
				MX_SETERROR ( MX1602, MxError::System, MxError::Abort, MxError::QuietReport, "RegAppInit failed %s, %s", m_verInfo->GetProductName(), m_verInfo->GetProductID());
			else
			{
				if (AfxMessageBox(IDS_SETFACTORYDEFAULTS_YESNO, MB_YESNO) == IDYES )
				{
					MxError::Inst().Reset();
					if ( m_PluginMngr->SetFactoryDefaults() == false)
						MX_SETERROR ( MX1601, MxError::System, MxError::Abort, MxError::QuietReport, "m_PluginMngr->SetFactoryDefaults() failed");
					else
					{
						if (SetFactoryDefaults() == false)
							MX_SETERROR ( MX1628, MxError::System, MxError::Abort, MxError::QuietReport, "SetFactoryDefaults() failed");
					}
				}
			}
		}
		MxError::Inst().SetLogging (m_PluginMngr->GetAppLogChannel(m_verInfo->GetProductID()), m_PluginMngr->GetAppLogLevel(m_verInfo->GetProductID()));

		CMxPluginManagerGUIDlg dlg;
		m_pMainWnd = &dlg;
		dlg.m_PluginMngr = m_PluginMngr;
		dlg.DoModal();		//all errors must be reported to the user before this dialog box closes

	}
	catch(std::exception const& e)
	{
		MX_SETERROR ( MX1604, MxError::System, MxError::TerminateApp, MxError::QuietReport, "unexpected exception - %s", e.what());
	}

	MX_END(MxError::VerboseReport, "App ends");

	//Bit of a cludge just to stop VS2012 reporting memory leaks before the following are called in their appropriate dtors
	//This memory is being freed, but not before VS2012 expects this to happen. Is this a problem with MFC Dialog Apps?
	//Even if the memory was not freed would it matter, because it is freed when the process ends? The reason I'm doing
	//this cludge is because lots of phantom errors may mask a real memory leak or other problem.

	if (m_PluginMngr != nullptr)
		delete m_PluginMngr;
	m_PluginMngr = nullptr;
	if (m_verInfo != nullptr)
		delete m_verInfo;
	m_verInfo = nullptr;

	MxPluginManager::CleanupOnAppExitOnly();
	MxUtils::Inst().CleanupOnAppExitOnly();
	MxError::Inst().CleanupOnAppExitOnly();

	//cludge ends

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}


bool CMxPluginManagerGUIApp::SetFactoryDefaults()
{
	bool rc = false;

	if (AfxGetApp()->WriteProfileString(CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralTraceName, CMxPluginManagerGUIApp::RegGeneralTraceDefault) == 0)
		MX_SETERROR (MX1631, MxError::System, MxError::Abort, MxError::QuietReport, "CMxPluginManagerGUIApp::RegGeneralTraceDefault failed");
	else
	{
		if (AfxGetApp()->WriteProfileString(CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralETWLogName, CMxPluginManagerGUIApp::RegGeneralETWLogDefault) == 0)
			MX_SETERROR (MX1648, MxError::System, MxError::Abort, MxError::QuietReport, "CMxPluginManagerGUIApp::RegGeneralETWLogName failed");
		else
		{
			if (AfxGetApp()->WriteProfileString(CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralDebugViewerAppName, CMxPluginManagerGUIApp::RegGeneralDebugViewerAppDefault) == 0)
				MX_SETERROR (MX1649, MxError::System, MxError::Abort, MxError::QuietReport, "CMxPluginManagerGUIApp::RegGeneralDebugViewerAppName failed");
			else
			{
				AfxGetApp()->WriteProfileString( CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralEventViewerCmdName, OptionsDlg::EventViewCmdDefault);
				AfxGetApp()->WriteProfileString( CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralEventViewerCmdParamsName, OptionsDlg::EventViewCmdParamsDefault);
				AfxGetApp()->WriteProfileString( CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralLogmanQueryCmdName, OptionsDlg::LogmanQueryCmdDefault);
				AfxGetApp()->WriteProfileString( CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralLogmanStartCmdName, OptionsDlg::LogmanStartCmdDefault);
				AfxGetApp()->WriteProfileString( CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralLogmanStopCmdName, OptionsDlg::LogmanStopCmdDefault);
				AfxGetApp()->WriteProfileString( CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralLogmanStdOutStatusLineName, OptionsDlg::LogmanStdOutStatusLineDefault);
				AfxGetApp()->WriteProfileString( CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralLogmanStdOutStatusValueName, OptionsDlg::LogmanStdOutStatusValueDefault);
				AfxGetApp()->WriteProfileString( CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralLogmanStdOutSuccessLineName, OptionsDlg::LogmanStdOutSuccessLineDefault);
				AfxGetApp()->WriteProfileString( CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralLogmanStdOutErrorRunAsAdminLineName, OptionsDlg::LogmanStdOutErrorRunAsAdminLineDefault);
				AfxGetApp()->WriteProfileString( CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralLogmanStdOutErrorInvalidTraceLineName, OptionsDlg::LogmanStdOutErrorInvalidTraceLineDefault);			

				rc = true;
			}
		}
	}
	return rc;
}
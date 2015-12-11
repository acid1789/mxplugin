// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include <fstream>
#include "MxPluginManagerGUI.h"
#include "OptionsDlg.h"
#include "afxdialogex.h"
#include "ShowErrorDlg.h"

#include "resource.h"


const wchar_t * const OptionsDlg::LogFileFilterFileDlg = L"Event Trace Log Files\0*.etl\0Event Tracing for Windows Files\0*.evtx\0(All Files)\0*.*\0\0";
const wchar_t * const OptionsDlg::ExeFileFilterFileDlg = L"Exe Files\0*.exe\0(All Files)\0*.*\0\0";

const wchar_t * const OptionsDlg::EventViewCmdDefault = L"eventvwr";
const wchar_t * const OptionsDlg::EventViewCmdParamsDefault = L"/l:";
const wchar_t * const OptionsDlg::LogmanQueryCmdDefault = L"Logman query ";
const wchar_t * const OptionsDlg::LogmanStartCmdDefault = L"Logman start ";
const wchar_t * const OptionsDlg::LogmanStopCmdDefault = L"Logman stop ";
const wchar_t * const OptionsDlg::LogmanStdOutStatusLineDefault = L"Status:               ";
const wchar_t * const OptionsDlg::LogmanStdOutStatusValueDefault = L"Stopped";
const wchar_t * const OptionsDlg::LogmanStdOutSuccessLineDefault = L"The command completed successfully.";
const wchar_t * const OptionsDlg::LogmanStdOutErrorRunAsAdminLineDefault = L"You're running with a restricted token, try running elevated.";
const wchar_t * const OptionsDlg::LogmanStdOutErrorInvalidTraceLineDefault =  L"Data Collector Set was not found.";

// OptionsDlg dialog

IMPLEMENT_DYNAMIC(OptionsDlg, CDialogEx)

OptionsDlg::OptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(OptionsDlg::IDD, pParent)
	, m_PluginRootFolder(_T(""))
	, m_ETWTraceName(_T(""))
	, m_OutputDebugStrViewerAppPathFilename(_T(""))
	, m_EventViewerCmd(_T(""))
	, m_EventViewerCmdParams(_T(""))
	, m_LogmanQueryCmd(_T(""))
	, m_LogmanStartCmd(_T(""))
	, m_LogmanStopCmd(_T(""))
	, m_LogmanStdOutStatusLine(_T(""))
	, m_LogmanStdOutStatusValue(_T(""))
	, m_LogmanStdOutSuccessLine(_T(""))
	, m_LogmanStdOutErrorRunAsAdminLine(_T(""))
	, m_LogmanStdOutErrorInvalidTraceLine(_T(""))
	, m_ETWLogPathFilename(_T(""))
	, m_ETWStatus(_T(""))
	, m_PluginMngr(nullptr)
	, m_AppSID(_T(""))
	, m_SelProdId(nullptr)
	, m_AppLogChanged (false)
{

}

OptionsDlg::~OptionsDlg()
{
	ClearProdIds();
}

void OptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PLUGINROOTFOLDER_EDIT, m_PluginRootFolder);
	DDX_Control(pDX, IDC_APPSCOMBO, m_AppsList);
	DDX_Control(pDX, IDC_LOGLEVELCOMBO, m_LogLevelList);
	DDX_Control(pDX, IDC_LOGTYPECOMBO, m_LogChannelList);
	DDX_Text(pDX, IDC_ETW_TRACENAME_EDIT, m_ETWTraceName);
	DDV_MaxChars(pDX, m_ETWTraceName, 250);
	DDX_Control(pDX, IDCANCEL, m_CancelButtonCtrl);
	DDX_Text(pDX, IDC_ETWSTATUS_STATIC, m_ETWStatus);
	DDX_Text(pDX, IDC_SID_EDIT, m_AppSID);
	DDX_Text(pDX, IDC_OUTPUTDEBUGSTRVIEWERAPP_EDIT, m_OutputDebugStrViewerAppPathFilename);
	DDX_Text(pDX, IDC_EVENTLOGPATHFILENAME_EDIT, m_ETWLogPathFilename);
	DDX_Control(pDX, IDC_ETWSTARTBUTTON, m_StartETWButton);
	DDX_Control(pDX, IDC_ETWSTOPBUTTON, m_StopETWButton);
	DDX_Control(pDX, IDC_BREAKONERROR, m_BreakOnLoadErrorCtrl);
}


BEGIN_MESSAGE_MAP(OptionsDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ETWSTARTBUTTON, &OptionsDlg::OnBnClickedETWStartButton)
	ON_BN_CLICKED(IDC_ETWSTOPBUTTON, &OptionsDlg::OnBnClickedETWStopButton)
	ON_BN_CLICKED(IDC_ETWCOPYLOG_BUTTON, &OptionsDlg::OnBnClickedETWCopyLogButton)
	ON_CBN_SELCHANGE  (IDC_APPSCOMBO, &OptionsDlg::OnCbnSelChangeAppsCombo)
	ON_BN_CLICKED(IDC_ETWVIEWLOG_BUTTON, &OptionsDlg::OnBnClickedETWOpenLogViewerButton)
	ON_EN_CHANGE(IDC_ETW_TRACENAME_EDIT, &OptionsDlg::OnEnChangeETWTraceNameEdit)
	ON_BN_CLICKED(IDC_ETWREFRESH_BUTTON, &OptionsDlg::OnBnClickedETWRefreshButton)
	ON_BN_CLICKED(IDC_RESETFACTORYDEFAULTS_BUTTON, &OptionsDlg::OnBnClickedResetFactoryDefaultsButton)
	ON_CBN_SELCHANGE(IDC_LOGLEVELCOMBO, &OptionsDlg::OnCbnSelChangeLogLevelCombo)
	ON_CBN_SELCHANGE (IDC_LOGTYPECOMBO, &OptionsDlg::OnCbnSelChangeLogChannelCombo)
	ON_BN_CLICKED(IDC_BROWSEFORVIEWER_BUTTON, &OptionsDlg::OnBnClickedBrowseDebugViewerAppButton)
	ON_BN_CLICKED(IDC_OPENVIEWER_BUTTON, &OptionsDlg::OnBnClickedOpenDebugViewerAppButton)
	ON_BN_CLICKED(IDC_ETWBROWSLOGPATHFILE_BUTTON, &OptionsDlg::OnBnClickedETWBrowseLogPathfileButton)
END_MESSAGE_MAP()


// OptionsDlg message handlers

BOOL OptionsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (m_PluginMngr == nullptr)
		MX_SETERROR (MX1605, MxError::CodeDefect, MxError::TerminateApp, MxError::QuietReport, "m_PluginMngr == nullptr");
	else
	{
		try
		{
			USES_CONVERSION;

			m_BreakOnLoadErrorCtrl.SetCheck((m_BreakOnDllLoadError == true) ? BST_CHECKED : BST_UNCHECKED);

			if ((PopulateLogChannelList() == true) && (PopulateLogLevelList() == true))
			{
				std::wstring rootFolder(L"");
				m_PluginRootFolder = m_PluginMngr->GetRootFolder();
				m_ETWTraceName = AfxGetApp()->GetProfileString( CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralTraceName, CMxPluginManagerGUIApp::RegGeneralTraceDefault);
				m_ETWLogPathFilename = AfxGetApp()->GetProfileString( CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralETWLogName, CMxPluginManagerGUIApp::RegGeneralETWLogDefault);
				m_OutputDebugStrViewerAppPathFilename = AfxGetApp()->GetProfileString( CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralDebugViewerAppName, CMxPluginManagerGUIApp::RegGeneralDebugViewerAppDefault);
				m_EventViewerCmd = AfxGetApp()->GetProfileString( CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralEventViewerCmdName, OptionsDlg::EventViewCmdDefault);
				m_EventViewerCmdParams = AfxGetApp()->GetProfileString( CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralEventViewerCmdParamsName, OptionsDlg::EventViewCmdParamsDefault);
				m_LogmanQueryCmd = AfxGetApp()->GetProfileString( CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralLogmanQueryCmdName, OptionsDlg::LogmanQueryCmdDefault);
				m_LogmanStartCmd = AfxGetApp()->GetProfileString( CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralLogmanStartCmdName, OptionsDlg::LogmanStartCmdDefault);
				m_LogmanStopCmd = AfxGetApp()->GetProfileString( CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralLogmanStopCmdName, OptionsDlg::LogmanStopCmdDefault);
				m_LogmanStdOutStatusLine = AfxGetApp()->GetProfileString( CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralLogmanStdOutStatusLineName, OptionsDlg::LogmanStdOutStatusLineDefault);
				m_LogmanStdOutStatusValue = AfxGetApp()->GetProfileString( CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralLogmanStdOutStatusValueName, OptionsDlg::LogmanStdOutStatusValueDefault);
				m_LogmanStdOutSuccessLine = AfxGetApp()->GetProfileString( CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralLogmanStdOutSuccessLineName, OptionsDlg::LogmanStdOutSuccessLineDefault);
				m_LogmanStdOutErrorRunAsAdminLine = AfxGetApp()->GetProfileString( CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralLogmanStdOutErrorRunAsAdminLineName, OptionsDlg::LogmanStdOutErrorRunAsAdminLineDefault);
				m_LogmanStdOutErrorInvalidTraceLine = AfxGetApp()->GetProfileString( CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralLogmanStdOutErrorInvalidTraceLineName, OptionsDlg::LogmanStdOutErrorInvalidTraceLineDefault);			

				CString AppDetails;
				int cnt = m_PluginMngr->GetAppCnt();
				int x = 0;
				for (x = 0; x < cnt; x++ )
				{
					std::string ProdID;
					if ( m_PluginMngr->GetAppProdID(x, &ProdID) != nullptr)
					{
						std::string ProdName;
						if ( m_PluginMngr->GetAppName(ProdID.c_str(), &ProdName) == nullptr)
							break;
						else
						{
							std::wstring wtmp;
							std::wstring pid(MxUtils::ConvertString2WString(&wtmp, ProdID.c_str()));
							AppDetails.Format(L"%s - ProdID=%s", MxUtils::ConvertString2WString(&wtmp, ProdName.c_str()), pid.c_str() );
							int index = 0;
							if ( (index = m_AppsList.AddString(AppDetails)) == CB_ERR)
								break;
							else
							{
								if ( m_AppsList.SetItemDataPtr(index, AddCopyProdId(&pid)) == CB_ERR)
									break;
							}
						}
					}
				}
				if ((x != cnt) || (cnt < 1))
					MX_SETERROR (MX1639, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "only %d of %d prodids found in registry added to combo box", x, cnt);
				else
					m_AppsList.SetCurSel(0);
			}
		}
		catch(...)
		{
		}
	}
	if ( MxError::Inst().IsErrorSet())
		ShowErrorDlg::DisplayUserError(IDS_BADACTION_OPTIONSINIT, this);
	else
	{
		OnCbnSelChangeAppsCombo();
		OnEnChangeETWTraceNameEdit();
	}

	return TRUE;  
}


void OptionsDlg::OnCbnSelChangeAppsCombo()
{
	int index = CB_ERR;

	DoUpdateAppLog();

	if((index = m_AppsList.GetCurSel()) == CB_ERR)
		MX_SETERROR (MX1640, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "invalid m_AppsList.GetCurSel()");
	else
	{
		if ( (m_SelProdId = static_cast<std::wstring *>(m_AppsList.GetItemDataPtr(index))) == nullptr)
			MX_SETERROR (MX1641, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "(m_AppsList.GetItemDataPtr(%d) returns nullptr", index);
		else
		{
			try
			{
				USES_CONVERSION;
				std::string ProdID(CW2A(m_SelProdId->c_str()));
				std::string SID;
				if ( m_PluginMngr->GetAppSID(ProdID.c_str(), &SID) == nullptr)
					MX_SETERROR (MX1642, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "m_PluginMngr->GetAppSID(%s) fails", ProdID.c_str());
				else
				{
					m_AppSID = A2CW(SID.c_str());
					SelectLogChannel(m_PluginMngr->GetAppLogChannel(ProdID.c_str()));
					SelectLogLevel(m_PluginMngr->GetAppLogLevel(ProdID.c_str()));
				}
			}
			catch(...)
			{
			}
		}
	}
	UpdateData(FALSE);

	if ( MxError::Inst().IsErrorSet())
		ShowErrorDlg::DisplayUserError(IDS_BADACTION_OPTIONSSELAPP);

}


void OptionsDlg::OnCbnSelChangeLogLevelCombo()
{
	m_AppLogChanged = true;
}


void OptionsDlg::OnCbnSelChangeLogChannelCombo()
{
	m_AppLogChanged = true;
}



void OptionsDlg::OnBnClickedETWBrowseLogPathfileButton()
{
	UpdateData(TRUE);

	CFileDialog dlgFile(TRUE);

	CString rc(m_ETWLogPathFilename);
	
	const int c_cMaxFiles = 1;
	const int c_cbBuffSize = (c_cMaxFiles * (MAX_PATH + 1)) + 1;
	dlgFile.GetOFN().lpstrFile = rc.GetBuffer(c_cbBuffSize);
	dlgFile.GetOFN().nMaxFile = c_cbBuffSize;
	dlgFile.GetOFN().lpstrFilter = OptionsDlg::LogFileFilterFileDlg;
	dlgFile.GetOFN().Flags = dlgFile.GetOFN().Flags | OFN_FILEMUSTEXIST;

	if ( dlgFile.DoModal() == IDOK )
	{
		m_ETWLogPathFilename = rc;
	}
	rc.ReleaseBuffer();

	UpdateData(FALSE);
}

void OptionsDlg::OnBnClickedETWCopyLogButton()
{
	UpdateData(TRUE);

	CFileDialog dlgFile(FALSE);

	CString rc(m_ETWLogPathFilename);
	
	const int c_cMaxFiles = 1;
	const int c_cbBuffSize = (c_cMaxFiles * (MAX_PATH + 1)) + 1;
	dlgFile.GetOFN().lpstrFile = rc.GetBuffer(c_cbBuffSize);
	dlgFile.GetOFN().nMaxFile = c_cbBuffSize;
	dlgFile.GetOFN().lpstrFilter = OptionsDlg::LogFileFilterFileDlg;
	dlgFile.GetOFN().Flags = dlgFile.GetOFN().Flags;

	if ( dlgFile.DoModal() == IDOK )
	{
		std::string SrcFile; 
		std::string DestFile; 
		try
		{
			USES_CONVERSION;

			SrcFile = CW2A(m_ETWLogPathFilename);
			DestFile = CW2A(rc);

			if (MxUtils::IsExistingPathFilename(SrcFile.c_str()) == false)
				MX_SETERROR (MX1658, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "source file not found - %s", SrcFile.c_str());
			else
			{
				if (MxUtils::IsValidPathFilename(DestFile.c_str(), MAX_PATH) == false)
					MX_SETERROR (MX1659, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "destination file cannot be created - %s", DestFile.c_str());
				else
				{
					std::ifstream  src(SrcFile.c_str());
					std::ofstream  dst(DestFile.c_str());
					dst << src.rdbuf();
				}
			}
		}
		catch(std::exception const& e)
		{
			MX_SETERROR (MX1657, MxError::System, MxError::Abort, MxError::QuietReport, "unexpected exception %s coping %s to %s", e.what(), SrcFile.c_str(), DestFile.c_str());
		}
	}
	rc.ReleaseBuffer();

	if ( MxError::Inst().IsErrorSet())
		ShowErrorDlg::DisplayUserError(IDS_BADACTION_COPYLOGFILE);
}


void OptionsDlg::OnBnClickedETWOpenLogViewerButton()
{
	UpdateData(TRUE);

	try
	{
		USES_CONVERSION;

		std::string LogFile; 
		LogFile = CW2A(m_ETWLogPathFilename);
		if (MxUtils::IsExistingPathFilename(LogFile.c_str()) == false)
			MX_SETERROR ( MX1655, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "Log file not found - %s", LogFile.c_str());
		else
		{
			CString params(m_EventViewerCmdParams);
			params += m_ETWLogPathFilename;
			int rc = (int)ShellExecute(NULL, NULL, m_EventViewerCmd, params, NULL, SW_SHOWNORMAL);
			if (rc < 32)
				MX_SETERROR ( MX1656, MxError::System, MxError::Abort, MxError::VerboseReport, "Log viewer failed - %s", LogFile.c_str());

		}
	}
	catch(...)
	{
	}
	if ( MxError::Inst().IsErrorSet())
		ShowErrorDlg::DisplayUserError(IDS_BADACTION_OPENETWLOGVIEWER);
}

void OptionsDlg::OnBnClickedETWRefreshButton()
{
	UpdateData(TRUE);

	m_StartETWButton.EnableWindow(FALSE);
	m_StopETWButton.EnableWindow(FALSE);

	m_ETWStatus = m_ETWTraceName;
	m_ETWStatus += L": ";

	CString StatusNotFound("");		
	if (StatusNotFound.LoadString(IDS_LOGMAN_ERR_STATUSNOTFOUND) == 0)
		StatusNotFound = L"Error, Status not found";	

	std::wstring StdOut(L"");
	std::wstring cmdLine(m_LogmanQueryCmd);
	if ( RunLogMan(cmdLine.c_str(), &StdOut) == true)
	{
		std::wstring status;
		if (MxUtils::GetText(StdOut.c_str(), m_LogmanStdOutStatusLine, &status) == false)
			m_ETWStatus += StatusNotFound;	
		else
			m_ETWStatus += status.c_str();

		if (m_LogmanStdOutStatusValue == status.c_str())
			m_StartETWButton.EnableWindow(TRUE);
		else
			m_StopETWButton.EnableWindow(TRUE);
	}
	UpdateData(FALSE);

	if ( MxError::Inst().IsErrorSet())
		ShowErrorDlg::DisplayUserError(IDS_BADACTION_ETWREFRESH);
}

void OptionsDlg::OnBnClickedETWStartButton()
{
	UpdateData(TRUE);

	CString Starting("");		
	if (Starting.LoadString(IDS_LOGMAN_STARTING) == 0)
		Starting = L"Starting, press 'Refresh' to get status";	

	m_StartETWButton.EnableWindow(FALSE);
	m_StopETWButton.EnableWindow(FALSE);

	std::wstring StdOut(L"");
	std::wstring cmdLine(m_LogmanStartCmd);
	if (RunLogMan(cmdLine.c_str(), &StdOut) == true)
	{
		m_ETWStatus = cmdLine.c_str();
		m_ETWStatus = Starting;
	}

	UpdateData(FALSE);

	if ( MxError::Inst().IsErrorSet())
		ShowErrorDlg::DisplayUserError(IDS_BADACTION_ETWSTART);
}


void OptionsDlg::OnBnClickedETWStopButton()
{
	UpdateData(TRUE);

	CString Stopping("");		
	if ( Stopping.LoadString(IDS_LOGMAN_STOPPING) == 0)
		 Stopping = L"Stopping, press 'Refresh' to get status";	

	m_StartETWButton.EnableWindow(FALSE);
	m_StopETWButton.EnableWindow(FALSE);

	std::wstring StdOut(L"");
	std::wstring cmdLine(m_LogmanStopCmd);
	if (RunLogMan(cmdLine.c_str(), &StdOut) == true)
	{
		m_ETWStatus = cmdLine.c_str();
		m_ETWStatus = Stopping;
	}

	UpdateData(FALSE);

	if ( MxError::Inst().IsErrorSet())
		ShowErrorDlg::DisplayUserError(IDS_BADACTION_ETWSTOP);
}


void OptionsDlg::OnEnChangeETWTraceNameEdit()
{
	UpdateData(TRUE);

	CString StatusUnknown("");		
	if (StatusUnknown.LoadString(IDS_LOGMAN_STATUSUNKNOWN) == 0)
		StatusUnknown = L"Status unknown, press 'Refresh' to get current status";

	m_StartETWButton.EnableWindow(FALSE);
	m_StopETWButton.EnableWindow(FALSE);

	m_ETWStatus = StatusUnknown;
	UpdateData(FALSE);
}



void OptionsDlg::OnBnClickedBrowseDebugViewerAppButton()
{
	UpdateData(TRUE);

	CFileDialog dlgFile(TRUE);

	CString rc(m_OutputDebugStrViewerAppPathFilename);
	
	const int c_cMaxFiles = 1;
	const int c_cbBuffSize = (c_cMaxFiles * (MAX_PATH + 1)) + 1;
	dlgFile.GetOFN().lpstrFile = rc.GetBuffer(c_cbBuffSize);
	dlgFile.GetOFN().nMaxFile = c_cbBuffSize;
	dlgFile.GetOFN().lpstrFilter = OptionsDlg::ExeFileFilterFileDlg;
	dlgFile.GetOFN().Flags = dlgFile.GetOFN().Flags | OFN_FILEMUSTEXIST;

	if ( dlgFile.DoModal() == IDOK )
	{
		m_OutputDebugStrViewerAppPathFilename = rc;
	}
	rc.ReleaseBuffer();

	UpdateData(FALSE);
}


void OptionsDlg::OnBnClickedOpenDebugViewerAppButton()
{
	UpdateData(TRUE);

	try
	{
		USES_CONVERSION;

		std::string AppFile; 
		AppFile = CW2A(m_OutputDebugStrViewerAppPathFilename);
		if (MxUtils::IsExistingPathFilename(AppFile.c_str()) == false)
			MX_SETERROR ( MX1653, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "App not found - %s", AppFile.c_str());
		else
		{
			int rc = (int)ShellExecute(NULL, L"open", m_OutputDebugStrViewerAppPathFilename, NULL, NULL, SW_SHOWNORMAL);
			if (rc < 32)
				MX_SETERROR ( MX1654, MxError::System, MxError::Abort, MxError::VerboseReport, "App failed - %s", AppFile.c_str());

		}
	}
	catch(...)
	{
	}

	if ( MxError::Inst().IsErrorSet())
		ShowErrorDlg::DisplayUserError(IDS_BADACTION_OPENDEBUGVIEWER);
}

void OptionsDlg::OnBnClickedResetFactoryDefaultsButton()
{
	if (AfxMessageBox(IDS_SETFACTORYDEFAULTS_YESNO, MB_YESNO) == IDYES )
	{
		m_CancelButtonCtrl.EnableWindow(FALSE);
		if ( m_PluginMngr->SetFactoryDefaults() == false)
		{
			MX_SETERROR ( MX1606, MxError::System, MxError::Abort, MxError::QuietReport, "m_PluginMngr->SetFactoryDefaults() failed");
			ShowErrorDlg::DisplayUserError(IDS_BADACTION_SETFACTORYDEFAULTS);
		}
		else
		{
			CMxPluginManagerGUIApp *app = dynamic_cast<CMxPluginManagerGUIApp *>(AfxGetApp());
			if (app == nullptr)
				MX_SETERROR ( MX1629, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "dynamic_cast<CMxPluginManagerGUIApp *>(AfxGetApp()) returns nullptr");
			else
			{
				if (app->SetFactoryDefaults() == false)
					MX_SETERROR ( MX1630, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "app->SetFactoryDefaults() fails");
			}
		}
		if ( MxError::Inst().IsErrorSet())
			ShowErrorDlg::DisplayUserError(IDS_BADACTION_RESETFACTORYDEFAULTSDOPTIONDLG);
	}
}


void OptionsDlg::OnOK()
{
	UpdateData(TRUE);

	if (AfxGetApp()->WriteProfileString( CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralTraceName, m_ETWTraceName) == 0)
		MX_SETERROR ( MX1632, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "set CMxPluginManagerGUIApp::RegGeneralTraceName fails");
	else
	{
		if (AfxGetApp()->WriteProfileString( CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralETWLogName, m_ETWLogPathFilename) == 0)
			MX_SETERROR ( MX1650, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "set CMxPluginManagerGUIApp::RegGeneralETWLogName fails");
		else
		{	
			if (AfxGetApp()->WriteProfileString( CMxPluginManagerGUIApp::RegGeneralSectionName, CMxPluginManagerGUIApp::RegGeneralDebugViewerAppName, m_OutputDebugStrViewerAppPathFilename) == 0)
				MX_SETERROR ( MX1651, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "set CMxPluginManagerGUIApp::RegGeneralDebugViewerAppName fails");
			else
			{
				m_BreakOnDllLoadError = (m_BreakOnLoadErrorCtrl.GetCheck() == BST_CHECKED) ? true : false;

				try
				{
					USES_CONVERSION;

					std::string traceName; 
					traceName = CW2A(m_ETWTraceName);
					std::string folder; 
					folder = CW2A(m_PluginRootFolder);

					MX_LOGMSG(MxError::VerboseReport, "SUCCESS: updated registry - TraceName=%s, PluginRootFolder=%s", traceName.c_str(), folder.c_str());
					DoUpdateAppLog();
				}
				catch(...)
				{
				}
			}
		}
	}
	ClearProdIds();
	if ( MxError::Inst().IsErrorSet())
		ShowErrorDlg::DisplayUserError(IDS_BADACTION_OKOPTIONDLG);
	else
		CDialogEx::OnOK();
}


void OptionsDlg::OnCancel()
{
	ClearProdIds();
	if ( MxError::Inst().IsErrorSet())
		ShowErrorDlg::DisplayUserError(IDS_BADACTION_CANCELOPTIONDLG);
	CDialogEx::OnCancel();
}

bool OptionsDlg::DoUpdateAppLog()
{
	bool rc = false;

	if ((m_SelProdId == nullptr) || (m_AppLogChanged == false))
		rc = true;
	else
	{
		m_CancelButtonCtrl.EnableWindow(FALSE);

		int index = CB_ERR;

		try
		{
			USES_CONVERSION;
			std::string ProdID(CW2A(m_SelProdId->c_str()));

			MxError::LogChannel channel = GetSelLogChannel();
				if ( m_PluginMngr->SetAppLogChannel(ProdID.c_str(), channel) == false)
				MX_SETERROR (MX1645, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "m_PluginMngr->SetAppLogChannel(%s, %d) fails", ProdID.c_str(), channel);
			else
			{
					MxError::Level level = GetSelLogLevel();
				if ( m_PluginMngr->SetAppLogLevel(ProdID.c_str(),level) == false)
					MX_SETERROR (MX1645, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "m_PluginMngr->SetAppLogLevel(%s, %d) fails", ProdID.c_str(), level);
				else
				{
					MX_LOGMSG(MxError::VerboseReport, "SUCCESS: Updated App ProdID=%s, channel=%d, level=%d", ProdID.c_str(), channel, level);
					m_AppLogChanged = false;
					rc = true;
				}
			}
		}
		catch(...)
		{
		}
	}
	return rc;
}


bool OptionsDlg::PopulateLogChannelList()
{
	bool rc = false;

	CString Development("");		//use OutputDebugString();
	if (Development.LoadString(IDS_LOG_DEVELOPMENT) == 0)
		Development = L"Development";
	CString Production("");			//use Event Tracing for Windows (ETW)
	if (Production.LoadString(IDS_LOG_PRODUCTION) == 0)
		Production = L"Production";
	CString NoLogging("");			//no logging
	if (NoLogging.LoadString(IDS_LOG_NONE) == 0)
		NoLogging = L"No Logging";

	int index = CB_ERR;
	if ((index = m_LogChannelList.AddString(Development)) != CB_ERR)
	{
		if ( m_LogChannelList.SetItemData(index, MxError::Development) != CB_ERR)
		{
			if ((index = m_LogChannelList.AddString(Production)) != CB_ERR)
			{
				if ( m_LogChannelList.SetItemData(index, MxError::Production) != CB_ERR)
				{
					if ((index = m_LogChannelList.AddString(NoLogging)) != CB_ERR)
					{
						if ( m_LogChannelList.SetItemData(index, MxError::NoLogging) != CB_ERR)
						{
							rc = true;
						}
					}
				}
			}
		}
	}
	if (rc == false)
		MX_SETERROR ( MX1634, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "m_LogChannelList was not initialised");
	return rc;
}

bool OptionsDlg::SelectLogChannel(MxError::LogChannel channel)
{
	bool rc = false;

	int cnt = m_LogChannelList.GetCount();
	for (int x = 0; x < cnt; x++)
	{
		if(m_LogChannelList.GetItemData(x) == channel)
		{
			if (m_LogChannelList.SetCurSel(x) != CB_ERR)
			{
				rc = true;
				break;
			}
		}
	}
	if (rc == false)
		MX_SETERROR ( MX1637, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "channel=%d cannot be selected", channel);
	return rc;
}

MxError::LogChannel  OptionsDlg::GetSelLogChannel()
{
	MxError::LogChannel rc = MxError::NoLogging;
	int index = CB_ERR;

	if ((index = m_LogChannelList.GetCurSel()) == CB_ERR)
		MX_SETERROR ( MX1647, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "m_LogChannelList bad selection");
	else
		rc = static_cast<MxError::LogChannel>(m_LogChannelList.GetItemData(index)); 

	return rc;
}

bool OptionsDlg::PopulateLogLevelList()
{
	bool rc = false;

	CString QuietReport("");		//set for Msgs that should always be reported
	if (QuietReport.LoadString(IDS_LOG_QUIETREPORT) == 0)
		QuietReport = L"Quiet Report";
	CString AnalysisReport("");		//set for Msgs that should be reported during problem analysis (bugs, performance issues, etc)
	if (AnalysisReport.LoadString(IDS_LOG_ANALYSISREPORT) == 0)
		AnalysisReport = L"Analysis Report";
	CString VerboseReport("");		//set for Msgs that should be reported only when Initialise(int ProductId, ProdName, Version, LogChannel channel, Level::VerboseReport) = 0;
	if ( VerboseReport.LoadString(IDS_LOG_VERBOSEREPORT) == 0)
		 VerboseReport = L"Verbose Report";

	int index = CB_ERR;
	if ((index = m_LogLevelList.AddString(QuietReport)) != CB_ERR)
	{
		if ( m_LogLevelList.SetItemData(index, MxError::QuietReport) != CB_ERR)
		{
			if ((index = m_LogLevelList.AddString( AnalysisReport)) != CB_ERR)
			{
				if ( m_LogLevelList.SetItemData(index, MxError::AnalysisReport) != CB_ERR)
				{
					if ((index = m_LogLevelList.AddString(VerboseReport)) != CB_ERR)
					{
						if ( m_LogLevelList.SetItemData(index, MxError::VerboseReport) != CB_ERR)
						{
							rc = true;
						}
					}
				}
			}
		}
	}
	if (rc == false)
		MX_SETERROR ( MX1635, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "m_LogChannelList was not initialised");

	return rc;
}

bool OptionsDlg::SelectLogLevel(MxError::Level level)
{
	bool rc = false;

	int cnt = m_LogLevelList.GetCount();
	for (int x = 0; x < cnt; x++)
	{
		if(m_LogLevelList.GetItemData(x) == level)
		{
			if (m_LogLevelList.SetCurSel(x) != CB_ERR)
			{
				rc = true;
				break;
			}
		}
	}
	if (rc == false)
		MX_SETERROR ( MX1636, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "Level=%d cannot be selected", level);
	return rc;
}

MxError::Level 	OptionsDlg::GetSelLogLevel()
{
	MxError::Level rc = MxError::VerboseReport;
	int index = CB_ERR;

	if ((index = m_LogLevelList.GetCurSel()) == CB_ERR)
		MX_SETERROR ( MX1646, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "m_LogLevelList bad selection");
	else
		rc = static_cast<MxError::Level>(m_LogLevelList.GetItemData(index)); 

	return rc;
}

std::wstring *OptionsDlg::AddCopyProdId( std::wstring *prodid)
{
	std::wstring *rc = nullptr;

	if (prodid != nullptr)
	{
		try
		{
			USES_CONVERSION;  
			std::string pid(CW2A(prodid->c_str()));

			bool found = false;
			size_t cnt = m_prodids.size();
			for (size_t x = 0; x < cnt; x++)
			{
				if ( *(m_prodids[x]) == *prodid)
				{
					found = true;
					break;
				}
			}
			if (found != false)
				MX_SETERROR ( MX1638, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "ProdID=%s already in list - does registry have duplicate ProdIds?", pid);
			else
			{
				rc = new std::wstring(*prodid);
				m_prodids.push_back(rc);
			}
		}
		catch(...)
		{
		}
	}
	return rc;
}

void  OptionsDlg::ClearProdIds()
{
	size_t cnt = m_prodids.size();
	for (size_t x = 0; x < cnt; x++)
	{
		delete  m_prodids[x];
	}
	m_prodids.clear();
}



bool OptionsDlg::RunLogMan(const wchar_t*CmdLine, std::wstring *StdOut)
{
	bool rc = false;

	CString ErrorUnknown("");		
	if (ErrorUnknown.LoadString(IDS_LOGMAN_ERR_UNKNOWN) == 0)
		ErrorUnknown = L"Error, unknown";
	CString ErrorRunFailed("");		
	if (ErrorRunFailed.LoadString(IDS_LOGMAN_ERR_RUNFAILED) == 0)
		ErrorRunFailed = L"Error, unable to run Logman";
	CString ErrorNoOutput("");		
	if (ErrorNoOutput.LoadString(IDS_LOGMAN_ERR_NOOUTPUT) == 0)
		ErrorNoOutput = L"Error, Logman produced no output";	
	CString ErrorInvalidOutput("");		
	if (ErrorInvalidOutput.LoadString(IDS_LOGMAN_ERR_INVALIDOUTPUT) == 0)
		ErrorInvalidOutput = L"Error, Logman produced invalid output";


	std::wstring result(ErrorUnknown);
	std::wstring cmdLine(CmdLine);
	cmdLine += m_ETWTraceName;

	if (MxUtils::RunConsoleApp(cmdLine.c_str(), StdOut, 65535, 3000) == false)
		m_ETWStatus = ErrorRunFailed;
	else
	{
		if (StdOut->find(m_LogmanStdOutSuccessLine) != std::string::npos)
			rc = true;
		else
		{
			std::string tmp;

			int StdOutLines = MxUtils::Invalid;
			if ( (StdOutLines = MxUtils::GetLineCnt(StdOut->c_str())) == MxUtils::Invalid)
				m_ETWStatus = ErrorNoOutput;
			else
			{

				if (MxUtils::GetLineText(StdOut->c_str(), ((StdOutLines-1 >= 0) ? StdOutLines-1 : StdOutLines), &result) == false)
					m_ETWStatus = ErrorInvalidOutput;
				else
				{
					if (m_LogmanStdOutErrorRunAsAdminLine == result.c_str())
						MX_SETERROR ( MX1660, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "Need to run MxPluginManager as Admin - %s", MxUtils::ConvertWString2String(&tmp, result.c_str()));
					else if (m_LogmanStdOutErrorInvalidTraceLine == result.c_str() )
						MX_SETERROR ( MX1662, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "error %s", MxUtils::ConvertWString2String(&tmp, result.c_str()));
					else
						MX_SETERROR ( MX1661, MxError::BadUserInput, MxError::Abort, MxError::VerboseReport, "error %s", MxUtils::ConvertWString2String(&tmp, result.c_str()));
					m_ETWStatus = result.c_str();
				}
			}
		}
	}
	return rc;
}






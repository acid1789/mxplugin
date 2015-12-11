
// SocketImageProcXGUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include <atlconv.h>

#include <iostream>
#include <fstream>

#include "SocketImageProcXGUI.h"
#include "SocketImageProcXGUIDlg.h"
#include "EditIIDListDlg.h"
#include "afxdialogex.h"

#include "MxPluginBaseEditableIID.h"

#include "IPluginImageProcXLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedResetFactoryDefaults();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RESETFACTORYDEFAULTS_BUTTON, &CAboutDlg::OnBnClickedResetFactoryDefaults)
END_MESSAGE_MAP()

void CAboutDlg::OnBnClickedResetFactoryDefaults()
{
	CString msg(L"Registry values for this application now set to factory defaults");

	CSocketImageProcXGUIApp *pApp = dynamic_cast<CSocketImageProcXGUIApp *>(AfxGetApp());
	if (pApp != '\0')
	{
		CString folder(L"");
		pApp->SetPluginRootFolder(folder);

		LSTATUS status;
		if (pApp->DeleteAllRecordedIIDs(&status) == false)
			msg.Format(L"Failed to reset registry values for this application - error code %d\n\nSocketImageProcXGUI error codes:\n%s\n\nOperating System error codes:\n%s\n\nFor other errors see MSDN System Error Codes", status, MxPluginLibs::ErrorCodeList, MxPluginLibs::ErrorCodeListMicrosoft);
	}
	AfxMessageBox(msg);
}

// CSocketImageProcXGUIDlg dialog




CSocketImageProcXGUIDlg::CSocketImageProcXGUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSocketImageProcXGUIDlg::IDD, pParent)
	, m_PluginRootFolder(_T(""))
	, m_SelPluginDetails(_T(""))
	, m_SelGUID(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pLibs = new MxPluginLibs();
}

CSocketImageProcXGUIDlg::~CSocketImageProcXGUIDlg()
{
	delete m_pLibs; 
}

void CSocketImageProcXGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PLUGINROOTFOLDER_EDIT, m_PluginRootFolder);
	DDV_MaxChars(pDX, m_PluginRootFolder, 500);
	DDX_Control(pDX, IDC_PLUGINLIST_COMBO, m_PluginList);
	DDX_Text(pDX, IDC_LIBDETAILS_EDIT, m_SelPluginDetails);
	DDX_Control(pDX, IDC_REFRESHBUTTON, m_RefreshButton);
	DDX_Control(pDX, IDC_IID_COMBO, m_IIDList);
	DDX_Control(pDX, IDC_EXPORTALLDETAILS_BUTTON, m_ExportAllDetailsButton);
	DDX_Control(pDX, IDC_EDITBUTTON, m_EditIIDsButton);
}

BEGIN_MESSAGE_MAP(CSocketImageProcXGUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BROWSE_BUTTON, &CSocketImageProcXGUIDlg::OnBnClickedBrowseButton)
	ON_BN_CLICKED(IDC_REFRESHBUTTON, &CSocketImageProcXGUIDlg::OnBnClickedRefreshButton)
	ON_CBN_SELCHANGE(IDC_PLUGINLIST_COMBO, &CSocketImageProcXGUIDlg::OnCbnSelchangePluginList)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_IID_COMBO, &CSocketImageProcXGUIDlg::OnCbnSelchangeIidCombo)
	ON_BN_CLICKED(IDC_EDITBUTTON, &CSocketImageProcXGUIDlg::OnBnClickedEditButton)
	ON_BN_CLICKED(IDC_EXPORTALLDETAILS_BUTTON, &CSocketImageProcXGUIDlg::OnBnClickedExportAllDetailsButton)
END_MESSAGE_MAP()


// CSocketImageProcXGUIDlg message handlers

void CSocketImageProcXGUIDlg::OnDestroy()
{
	DeinitIIDList();
	CDialogEx::OnDestroy();
}

BOOL CSocketImageProcXGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_IIDList.EnableWindow(FALSE);
	m_PluginList.EnableWindow(FALSE);

	m_EditIIDsButton.EnableWindow(FALSE);
	m_RefreshButton.EnableWindow(FALSE);
	m_ExportAllDetailsButton.EnableWindow(FALSE);

	m_SelPluginDetails = "No MxPlugs found";
	m_PluginRootFolder = ((CSocketImageProcXGUIApp *)AfxGetApp())->GetPluginRootFolder();

	OnBnClickedRefreshButton();

	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSocketImageProcXGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();

		m_PluginRootFolder = ((CSocketImageProcXGUIApp *)AfxGetApp())->GetPluginRootFolder();
		if (DeinitIIDList() == true)
			InitIIDList();
		OnBnClickedRefreshButton();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSocketImageProcXGUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSocketImageProcXGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSocketImageProcXGUIDlg::OnBnClickedBrowseButton()
{
	bool refresh = false;
	if (m_pLibs != '\0')
	{
		m_pLibs->ClearAllMxPlugins();

		CString libDir(m_PluginRootFolder);
		CWinAppEx *pApp = dynamic_cast<CWinAppEx *>(AfxGetApp());
		if (pApp)
		{
			if (pApp->GetShellManager()->BrowseForFolder(libDir, this, libDir, L"Select root folder for Plugin DLLs", BIF_RETURNONLYFSDIRS) )
			{
				if  (m_PluginRootFolder != libDir)
				{
					m_PluginRootFolder = libDir;
					((CSocketImageProcXGUIApp *)AfxGetApp())->SetPluginRootFolder(m_PluginRootFolder);
					refresh = true;
				}
			}
		}
	}
	UpdateData(FALSE);

	if (refresh)
		OnBnClickedRefreshButton();
}

void CSocketImageProcXGUIDlg::OnCbnSelchangeIidCombo()
{
	m_SelPluginDetails = "No MxPlugins Found";
	m_PluginList.EnableWindow(FALSE);
	m_PluginList.ResetContent();
	int index = CB_ERR;
	if ((index = m_IIDList.GetCurSel()) != CB_ERR)
	{
		MxPluginBaseEditableIID *pIId = '\0';
		if ( (pIId = (MxPluginBaseEditableIID *)m_IIDList.GetItemDataPtr(index)) != '\0')
		{
			DWORD errorCode = m_pLibs->RefreshPluginsInSubFolders(m_PluginRootFolder, pIId->GUID());
			if (errorCode != MxPluginLibs::NoError)
			{
				CString errMsg;
				errMsg.Format(L"Error: RefreshPlugins returned error code %d\n\nSocketImageProcXGUI error codes:\n%s\n\nOperating System error codes:\n%s\n\nFor other errors see MSDN System Error Codes", errorCode, MxPluginLibs::ErrorCodeList, MxPluginLibs::ErrorCodeListMicrosoft);
				AfxMessageBox(errMsg);
			}
			else
			{
				int cnt = m_pLibs->GetPluginCount();
				for (int x = 0; x < cnt; x++ )
				{
					CString libdetail (m_pLibs->GetPlugin(x)->GetLibSummary());
					int index = 0;
					if ((index = m_PluginList.AddString(libdetail)) != CB_ERR)
						m_PluginList.SetItemDataPtr(index, (void *)m_pLibs->GetPlugin(x));
				}
				if (cnt > 0 )
				{
					if (m_PluginList.SetCurSel(0) != CB_ERR)
					{
						OnCbnSelchangePluginList();
						m_PluginList.EnableWindow(TRUE);
						m_ExportAllDetailsButton.EnableWindow(TRUE);
					}
				}
			}
		}
	}
	UpdateData(FALSE);
}

void CSocketImageProcXGUIDlg::OnBnClickedRefreshButton()
{
	m_SelPluginDetails = "No MxPlugins Found";
	m_PluginList.ResetContent();
	m_PluginList.EnableWindow(FALSE);
	m_ExportAllDetailsButton.EnableWindow(FALSE);

	DeinitIIDList();
	if ( InitIIDList() == false )
		m_SelPluginDetails = "Initialisation of Interface IDs failed - suggest you reset to factory defaults (Sys|About)";
	else
	{
		if ((m_IIDList.GetCount() <= 0) || (m_PluginRootFolder.GetLength() <= 0))
			m_RefreshButton.EnableWindow(FALSE);
		else
		{
			m_RefreshButton.EnableWindow(TRUE);

			if (m_PluginRootFolder.GetLength() > 0 )
			{
				m_RefreshButton.EnableWindow(TRUE);
				OnCbnSelchangeIidCombo();
			}
		}
	}
	UpdateData(FALSE);
}


void CSocketImageProcXGUIDlg::OnCbnSelchangePluginList()
{
	m_SelPluginDetails = "No details found for selected MxPlugin";
	int index = 0;
	if ( (index = m_PluginList.GetCurSel()) != CB_ERR)
	{
		MxPluginLib *lib = '\0';
		if ((lib = (MxPluginLib *)m_PluginList.GetItemDataPtr(index)) != '\0')
			m_SelPluginDetails = lib->GetLibDetails();
	}
	UpdateData(FALSE);
}


void CSocketImageProcXGUIDlg::OnBnClickedEditButton()
{
	CEditIIDListDlg dlg;

	if (dlg.DoModal() == IDOK)
	{
		m_SelPluginDetails = "Initialisation of Interface IDs failed";

		DeinitIIDList();
		if ( InitIIDList() )
			OnCbnSelchangeIidCombo();
	}
	UpdateData(FALSE);
}

void CSocketImageProcXGUIDlg::OnBnClickedExportAllDetailsButton()
{
	OnBnClickedRefreshButton();

	CString comment(L"");
	comment.Format(L"CSocketImageProcXGUI - report on MxPlugins providing an interface derived from IMxPluginLibDetails found in:\r\n%s\r\n\r\n", m_PluginRootFolder);
	CString report(comment);

	MxPluginLib *lib = '\0';
	int libCnt = 0;
	int cnt = m_PluginList.GetCount();
	for (int index = 0; index < cnt; index++ )
	{
		if ((lib = (MxPluginLib *)m_PluginList.GetItemDataPtr(index)) == '\0')
			comment.Format(L"MxPlugin %d ERROR: unable to obtain details....................\r\n", libCnt);
		else
		{
			comment.Format(L"\r\nMxPlugin %d ...................................................\r\n", libCnt);
			report += comment;
			report += lib->GetLibDetails();
		}
		report += comment;
		libCnt++;
	}
    comment.Format(L"\r\n\r\nEnd of Report: %d MxPlugins in list, %d MxPlugins detailed.....\r\n", cnt, libCnt);
	report += comment;

	//select file

	CString PathFilename(m_PluginRootFolder);
	PathFilename += "\\MxPluginReport.txt";

	CFileDialog dlgFile(FALSE);

	const int c_cMaxFiles = 1;
	const int c_cbBuffSize = (c_cMaxFiles * (MAX_PATH + 1)) + 1;
	dlgFile.GetOFN().lpstrFile = PathFilename.GetBuffer(c_cbBuffSize);
	dlgFile.GetOFN().nMaxFile = c_cbBuffSize;
	dlgFile.GetOFN().lpstrFilter = L"Text Files\0*.txt\0(All Files)\0*.*\0\0";

	bool SaveToFile = false;
	if ( dlgFile.DoModal() == IDOK )
		SaveToFile = true;	
	PathFilename.ReleaseBuffer();

	if (SaveToFile)
	{
		try
		{
			USES_CONVERSION; 

			std::ofstream reportFile;
			reportFile.open (PathFilename);

			std::string reportStr;
			reportStr = (CW2A(report));
			reportFile << reportStr;

			reportFile.close();
		}
		catch(...)
		{
			CString errMsg("");
			errMsg.Format(L"Failed to create file, error code = %d - see MSDN system error codes for details", GetLastError());
			AfxMessageBox(errMsg); 
		}
	}
}

bool CSocketImageProcXGUIDlg::InitIIDList(void)
{
	bool rc = false;

	CString IIdDescription;

	MxPluginBaseEditableIID *pIID = '\0';

	if ((pIID = new MxPluginBaseEditableIID(IMxPluginLibDetailsIID::Name(), IMxPluginLibDetailsIID::GUID())) != '\0')
	{
		int index = 0;
		IIdDescription.Format(L"%s [%s]", pIID->GetName(),pIID->GetGUID()); 
		if ((index = m_IIDList.AddString(IIdDescription)) == CB_ERR)
			delete pIID;
		else
			m_IIDList.SetItemDataPtr(index, (void *)pIID);
	}

	CSocketImageProcXGUIApp *pApp = dynamic_cast<CSocketImageProcXGUIApp *>(AfxGetApp());
	if (pApp != '\0')
	{
		int IIDCnt = pApp->GetRecordedIIDCnt();
		for (int x = 0; x < IIDCnt; x++ )
		{
			pIID = new MxPluginBaseEditableIID();
			if (pApp->GetRecordedIID(x, pIID) == false)
			{
				CString errMsg;
				errMsg.Format(L"Error failed to Get RecordedIID %, error code = %d", x, GetLastError());
				AfxMessageBox (errMsg);
				break;
			}
			else
			{
				int index = CB_ERR;
				IIdDescription.Format(L"%s [%s]", pIID->GetName(),pIID->GetGUID()); 
				if ((index = m_IIDList.AddString(IIdDescription)) == CB_ERR)
					delete pIID;
				else
					m_IIDList.SetItemDataPtr(index, (void *)pIID);
			}
		}
		m_EditIIDsButton.EnableWindow(TRUE);
		if (m_IIDList.GetCount() <= 0)
			m_IIDList.EnableWindow(FALSE);
		else
		{
			m_IIDList.EnableWindow(TRUE);
			if (m_SelGUID.GetLength() != IMxPluginBaseIID::IIdSize )
				rc = (m_IIDList.SetCurSel(0) != CB_ERR) ? true : false;
			else
			{
				int cnt = m_IIDList.GetCount();
				for (int y = 0; y < cnt; y++)
				{
					if ( (pIID = (MxPluginBaseEditableIID *)m_IIDList.GetItemDataPtr(y)) != '\0')
					{
						if (m_SelGUID == pIID->GetGUID())
						{
							rc = (m_IIDList.SetCurSel(y) != CB_ERR) ? true : false;
							break;
						}
					}
				}
			}
		}
	}
	return rc;
}

bool CSocketImageProcXGUIDlg::DeinitIIDList(void)
{
	m_SelGUID = L"";
	MxPluginBaseEditableIID *pIId = '\0';
	int cnt = m_IIDList.GetCount();
	if (cnt > 0 )
	{
		int index = CB_ERR;
		if ( (index = m_IIDList.GetCurSel()) != CB_ERR)
		{
			MxPluginBaseEditableIID *pIId = '\0';
			if ( (pIId = (MxPluginBaseEditableIID *)m_IIDList.GetItemDataPtr(index)) != '\0')
				m_SelGUID = pIId->GetGUID();
		}
	}
	for (int x = 0; x < cnt; x++ )
	{
		if ((pIId = (MxPluginBaseEditableIID *)m_IIDList.GetItemDataPtr(x)) != '\0')
			delete pIId;
	}
	m_IIDList.ResetContent();

	return true;
}



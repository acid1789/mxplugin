
// MxPluginManagerGUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MxPluginManagerGUI.h"
#include "MxPluginManagerGUIDlg.h"
#include "OptionsDlg.h"
#include "InterfaceIDDlg.h"
#include "afxdialogex.h"

#include "ShowErrorDlg.h"

//#include "MxUtils\MxUtils1.h"

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
	CString m_ProductName;
	CString m_Version;
	CString m_Copyright;
	virtual BOOL OnInitDialog();

};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
	, m_ProductName(_T(""))
	, m_Version(_T(""))
	, m_Copyright(_T(""))
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PRODUCTNAME, m_ProductName);
	DDX_Text(pDX, IDC_VERSION, m_Version);
	DDX_Text(pDX, IDC_COPYRIGHT, m_Copyright);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	MxVersionInfo vers;
	m_ProductName = vers.GetProductName();
	m_Version = vers.GetVersion();
	m_Copyright = vers.GetCopyrightNotice();
	
	UpdateData(FALSE);

	return TRUE;
}

// CMxPluginManagerGUIDlg dialog



CMxPluginManagerGUIDlg::CMxPluginManagerGUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMxPluginManagerGUIDlg::IDD, pParent)
	, m_MxPluginRootFolder(_T(""))
	, m_LibDetails(_T(""))
	, m_LibID(_T(""))
	, m_PluginMngr(nullptr)
	, m_selSID(nullptr)
	, m_selPlugin(nullptr)
	, m_BreakOnDllLoadError(true)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMxPluginManagerGUIDlg::~CMxPluginManagerGUIDlg()
{
	ClearSids();
}

void CMxPluginManagerGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FOLDEREDIT, m_MxPluginRootFolder);
	DDX_Control(pDX, IDC_PLUGIN_LIST, m_PluginList);
	DDX_Text(pDX, IDC_PLUGINDETAILS_EDIT, m_LibDetails);
	DDX_Control(pDX, IDC_SELECTED_LIBID_EDIT, m_LibIDCtrl);
	DDX_Text(pDX, IDC_SELECTED_LIBID_EDIT, m_LibID);
	DDX_Control(pDX, IDC_SOCKETAPP_COMBO, m_SocketAppList);
	DDX_Control(pDX, IDC_INTERFACESBUTTON, m_InterfacesButton);
}

BEGIN_MESSAGE_MAP(CMxPluginManagerGUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INTERFACESBUTTON, &CMxPluginManagerGUIDlg::OnBnClickedInterfacesButton)
	ON_BN_CLICKED(IDC_COPY_BUTTON, &CMxPluginManagerGUIDlg::OnBnClickedCopyLibIDButton)
	ON_BN_CLICKED(IDC_OPTIONSBUTTON, &CMxPluginManagerGUIDlg::OnBnClickedOptionsbutton)
	ON_BN_CLICKED(IDC_REFRESHBUTTON, &CMxPluginManagerGUIDlg::OnBnClickedRefreshButton)
	ON_LBN_SELCHANGE(IDC_PLUGIN_LIST, &CMxPluginManagerGUIDlg::OnLbnSelchangePluginList)
	ON_CBN_SELCHANGE(IDC_SOCKETAPP_COMBO, &CMxPluginManagerGUIDlg::OnCbnSelChangeSocketAppCombo)
	ON_BN_CLICKED(IDC_WEBHELPBUTTON, &CMxPluginManagerGUIDlg::OnBnClickedWebHelpButton)
END_MESSAGE_MAP()


// CMxPluginManagerGUIDlg message handlers

BOOL CMxPluginManagerGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_InterfacesButton.EnableWindow(FALSE);

	if (m_PluginMngr == nullptr)
		MX_SETERROR (MX1603, MxError::CodeDefect, MxError::TerminateApp, MxError::QuietReport, "m_PluginMngr == nullptr");
	else
	{
		m_MxPluginRootFolder = m_PluginMngr->GetRootFolder();

		PopulateSocketAppList();
	}

	if (MxError::Inst().IsErrorSet() )
		ShowErrorDlg::DisplayUserError(IDS_BADACTION_APPINIT, this);
	else
	{
		ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);		// IDM_ABOUTBOX must be in the system command range.
		ASSERT(IDM_ABOUTBOX < 0xF000);
		ASSERT((IDM_OPTIONSDLG & 0xFFF0) == IDM_OPTIONSDLG);	// IDM_OPTIONSDLG must be in the system command range.
		ASSERT(IDM_OPTIONSDLG < 0xF000);

		CMenu* pSysMenu = GetSystemMenu(FALSE);
		if (pSysMenu != NULL)
		{
			BOOL bNameValid;
			CString strOptionsMenu;
			bNameValid = strOptionsMenu.LoadString(IDS_OPTIONS);
			ASSERT(bNameValid);
			if (!strOptionsMenu.IsEmpty())
			{
				pSysMenu->AppendMenu(MF_SEPARATOR);
				pSysMenu->AppendMenu(MF_STRING, IDM_OPTIONSDLG, strOptionsMenu);
			}
			CString strAboutMenu;
			bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
			ASSERT(bNameValid);
			if (!strAboutMenu.IsEmpty())
			{
				pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
			}
		}

		// Set the icon for this dialog.  The framework does this automatically
		//  when the application's main window is not a dialog
		SetIcon(m_hIcon, TRUE);			// Set big icon
		SetIcon(m_hIcon, FALSE);		// Set small icon

	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMxPluginManagerGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ((nID & 0xFFF0) == IDM_OPTIONSDLG)
	{
		OnBnClickedOptionsbutton();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMxPluginManagerGUIDlg::OnPaint()
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
HCURSOR CMxPluginManagerGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMxPluginManagerGUIDlg::OnBnClickedInterfacesButton()
{
	InterfaceIDDlg dlg;
	dlg.m_selPlugin = m_selPlugin;
	dlg.DoModal();
}


void CMxPluginManagerGUIDlg::OnBnClickedCopyLibIDButton()
{
	m_LibIDCtrl.SetSel(0, -1);
	m_LibIDCtrl.Copy();
}


void CMxPluginManagerGUIDlg::OnBnClickedOptionsbutton()
{
	OptionsDlg dlg;

	dlg.m_PluginMngr = m_PluginMngr;
	dlg.m_BreakOnDllLoadError = m_BreakOnDllLoadError;

	if (dlg.DoModal() == IDOK)
	{
		m_MxPluginRootFolder = dlg.m_PluginRootFolder;
		m_BreakOnDllLoadError = dlg.m_BreakOnDllLoadError;
		OnBnClickedRefreshButton();
	}

	if (MxError::Inst().IsErrorSet())
		ShowErrorDlg::DisplayUserError(IDS_BADACTION_OPTIONSET);
}


void CMxPluginManagerGUIDlg::OnBnClickedRefreshButton()
{
	UpdateData(FALSE);

	m_LibDetails = L"";
	m_LibID = L"";

	m_InterfacesButton.EnableWindow(FALSE);
	m_PluginMngr->ClearAllMxPlugins();
	m_PluginList.ResetContent();

	if ( m_selSID == nullptr)
		MX_SETERROR (MX1621, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "m_selSID == nullptr");
	else
	{
		if (m_PluginMngr->RefreshPluginsInRootFolder( MxGUID::ConvertFromString(m_selSID->c_str()), m_BreakOnDllLoadError) == true)
		{
			MxPluginLib *plugin = nullptr;
			int index = LB_ERR;
			int cnt =  m_PluginMngr->GetPluginCount();
			int x = 0;
			for (x = 0; x < cnt; x++ )
			{
				if ((plugin = m_PluginMngr->GetPlugin(x)) == nullptr)
					break;
				CString pluginDetails(plugin->GetLibSummary());
				if ( (index = m_PluginList.AddString(pluginDetails)) == LB_ERR)
					break;
				if ( m_PluginList.SetItemDataPtr(index, plugin) == LB_ERR)
					break;
			}
			if ( x < cnt)
				MX_SETERROR (MX1603, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "added only %d of %d plugins found; failed at %s", x, cnt, (plugin != nullptr) ? plugin->GetLibSummary() : "(plugin==nullptr)");
			else
			{
				m_PluginList.SetCurSel((index != LB_ERR) ? 0 : -1);
				if (index != LB_ERR)
					OnLbnSelchangePluginList();
			}
		}
	}
	if ( MxError::Inst().IsErrorSet())
		ShowErrorDlg::DisplayUserError(IDS_BADACTION_REFRESHPLUGINS	);
	UpdateData(FALSE);
}

void CMxPluginManagerGUIDlg::OnCancel()
{
	if ( MxError::Inst().IsErrorSet())
		ShowErrorDlg::DisplayUserError(IDS_BADACTION_CANCELAPP);
	ClearSids();
	CDialogEx::OnCancel();
}


void CMxPluginManagerGUIDlg::OnOK()
{
	if ( MxError::Inst().IsErrorSet())
		ShowErrorDlg::DisplayUserError(IDS_BADACTION_EXITAPP);
	ClearSids();
	CDialogEx::OnOK();
}


void CMxPluginManagerGUIDlg::OnLbnSelchangePluginList()
{
	int index = LB_ERR;
	
	m_selPlugin = nullptr;
	m_InterfacesButton.EnableWindow(FALSE);
	m_LibDetails = L"";
	m_LibID = L"";


	if ( (index = m_PluginList.GetCurSel()) == LB_ERR)
		MX_SETERROR (MX1608, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "no item selected");
	else
	{
		if ( (m_selPlugin = static_cast<MxPluginLib *>(m_PluginList.GetItemDataPtr(index))) == nullptr)
			MX_SETERROR (MX1609, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "item has no data item ptr");
		else
		{
			m_InterfacesButton.EnableWindow(TRUE);
			m_LibDetails = m_selPlugin->GetLibDetails();
			std::string libid;
			CString id(MxGUID::ConvertToString(m_selPlugin->GetLibID(), &libid));
			m_LibID = id;
		}
	}
	if ( MxError::Inst().IsErrorSet())
		ShowErrorDlg::DisplayUserError(IDS_BADACTION_PLUGINSEL);
	UpdateData(FALSE);
}

void CMxPluginManagerGUIDlg::OnCbnSelChangeSocketAppCombo()
{
	int index = CB_ERR;

	m_selSID = nullptr;
	if ((index = m_SocketAppList.GetCurSel()) != CB_ERR)
	{
		if ( (m_selSID = static_cast<std::string *>(m_SocketAppList.GetItemDataPtr(index))) == nullptr)
			MX_SETERROR (MX1620, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "unable to get pSid to combo box for index=%d", index);
	}
	if (MxError::Inst().IsErrorSet())
		ShowErrorDlg::DisplayUserError(IDS_BADACTION_APPCOMBOCHANGED);
	else
		OnBnClickedRefreshButton();
}

bool CMxPluginManagerGUIDlg::PopulateSocketAppList()
{
	bool rc = false;

	ClearSids();
	m_SocketAppList.ResetContent();

	try
	{
		USES_CONVERSION; 

		CString SocketAppAll("");
		if (SocketAppAll.LoadString(IDS_SOCKETAPP_ALL) == 0)
			SocketAppAll = L"Any Application - list all MxPlugins that implement IMxPluginLibDetailsIID";
		CString SocketAppUniversal("");
		if (SocketAppUniversal.LoadString(IDS_SOCKETAPP_UNIVERSAL) == 0)
			SocketAppUniversal = L"Univeral Socket Applications - list all MxPlugins for Socket ID={02742858-2CC8-4C92-BE69-5CC7BA387A7B}";

		int index = CB_ERR;
		std::string   sid;
		std::string*  pSid;

		MxGUID::ConvertToString(GUID_Invalid, &sid);
		if ( (pSid = AddCopySid (&sid)) == nullptr)
			MX_SETERROR (MX1611, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "unable to add SID_Invalid to Sids");
		else
		{
			if ((index = m_SocketAppList.AddString(SocketAppAll)) == CB_ERR)
				MX_SETERROR (MX1612, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "unable to add SID_Invalid to combo box");
			else
			{
				if ( m_SocketAppList.SetItemDataPtr(index, pSid) == CB_ERR)
					MX_SETERROR (MX1613, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "unable to add pSid to combo box for SID_INVALID");
				else
				{
					MxGUID::ConvertToString(SID_Universal, &sid);
					if ( (pSid = AddCopySid (&sid)) == nullptr)
						MX_SETERROR (MX1614, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "unable to add SID_Universal to Sids");
					else
					{
						if ((index = m_SocketAppList.AddString(SocketAppUniversal)) == CB_ERR)
							MX_SETERROR (MX1615, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "unable to add SID_Universal to combo box");
						{
							if ( m_SocketAppList.SetItemDataPtr(index, pSid) == CB_ERR)
								MX_SETERROR (MX1616, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "unable to add pSid to combo box for SID_INVALID");
							else
							{
								if ( PopulateSocketAppListFromRegistry() == false)
									MX_SETERROR (MX1617, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "PopulateSocketAppListFromRegistry() failed");
								else
									rc = true;
							}
						}
					}
				}
			}
		}
	}
	catch(...)
	{
	}
	if ( rc == true)
	{
		m_SocketAppList.SetCurSel(0);
		OnCbnSelChangeSocketAppCombo();
	}
	return rc;
}


bool CMxPluginManagerGUIDlg::PopulateSocketAppListFromRegistry()
{
	bool rc = false;

	CString SocketAppRegistry("");
	if (SocketAppRegistry.LoadString(IDS_SOCKETAPP_REGISTRY) == 0)
		SocketAppRegistry = L" - list all MxPlugins for Socket ID=";
	CString SocketAppRegDetails(L"");

	try
	{
		USES_CONVERSION; 

		int index = CB_ERR;
		std::string   sid;
		std::string*  pSid;

		int cnt = m_PluginMngr->GetAppCnt();
		int x = 0;
		for (x = 0; x < cnt; x++ )
		{
			std::string ProdID;
			if ( m_PluginMngr->GetAppProdID(x, &ProdID) != nullptr)
			{
				if ( m_PluginMngr->GetAppSID(ProdID.c_str(), &sid) == nullptr)
					break;
				else
				{
					if ( (pSid = AddCopySid (&sid)) != nullptr)	//only add SID if not found
					{
						std::string name;
						if ( m_PluginMngr->GetAppName(ProdID.c_str(), &name) == nullptr)
							break;
						else
						{
							std::wstring wtmp;
							CString pid(MxUtils::ConvertString2WString(&wtmp, ProdID.c_str()));
							CString prodname(MxUtils::ConvertString2WString(&wtmp, name.c_str())); 
							SocketAppRegDetails.Format(L"%s%s%s", (LPCWSTR)prodname, (LPCWSTR)SocketAppRegistry, (LPCWSTR)pid);
							if ((index = m_SocketAppList.AddString(SocketAppRegDetails)) != CB_ERR)
							{
								if ( m_SocketAppList.SetItemDataPtr(index, pSid) == CB_ERR)
								{
									MX_SETERROR (MX1619, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "unable to add pSid to combo box for %s", ProdID.c_str());
									break;
								}
							}
						}
					}
				}
			}
		}
		if ( (x != cnt) || (cnt == 0))
			MX_SETERROR (MX1618, MxError::System, MxError::Abort, MxError::QuietReport, "Not all Apps in Registry Apps have been processed - found %d, processed %d", cnt, x);
		else
			rc = true;
	}
	catch(...)
	{
	}
	return rc;
}

std::string *CMxPluginManagerGUIDlg::AddCopySid( std::string *sid)
{
	std::string *rc = nullptr;

	if (sid == nullptr)
		MX_SETERROR (MX1610, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "sid is nullptr");
	else
	{
		bool found = false;
		size_t cnt = m_sids.size();
		for (size_t x = 0; x < cnt; x++)
		{
			if ( *(m_sids[x]) == *sid)
			{
				found = true;
				break;
			}
		}
		if (found == false)
		{
			rc = new std::string(*sid);
			m_sids.push_back(rc);
		}
	}
	return rc;
}

void  CMxPluginManagerGUIDlg::ClearSids()
{
	size_t cnt = m_sids.size();
	for (size_t x = 0; x < cnt; x++)
	{
		delete  m_sids[x];
	}
	m_sids.clear();
}

void CMxPluginManagerGUIDlg::OnBnClickedWebHelpButton()
{
	ShellExecute(0, NULL, L"http://docs.mxplugin.org", NULL, NULL, SW_SHOWDEFAULT);
}

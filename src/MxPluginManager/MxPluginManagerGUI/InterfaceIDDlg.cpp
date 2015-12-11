// InterfaceIDDlg.cpp : implementation file
//

#include "stdafx.h"
#include <atlconv.h>

#include "PluginImageCurator\IImageCuratorPluginLib.h"	//copied from DiscountAsp\OCUDS-Production\Src\PublicInterfaces\PluginImageCurator
#include "PluginImageCurator\IImageCurator.h"			//copied from DiscountAsp\OCUDS-Production\Src\PublicInterfaces\PluginImageCurator

#include "MxPluginManagerGUI.h"
#include "InterfaceIDDlg.h"
#include "afxdialogex.h"
#include "ShowErrorDlg.h"


// InterfaceIDDlg dialog

IMPLEMENT_DYNAMIC(InterfaceIDDlg, CDialogEx)

InterfaceIDDlg::InterfaceIDDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(InterfaceIDDlg::IDD, pParent)
	, m_IIDDetails(_T(""))
	, m_LibDetails(_T(""))
	, m_IIDErrorMsg(_T(""))
	, m_selPlugin(nullptr)
{

}

InterfaceIDDlg::~InterfaceIDDlg()
{
	ClearLibDetails();
}

void InterfaceIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IDDCOMBO, m_IDDList);
	DDX_Text(pDX, IDC_INTERFACEDETAILSEDIT, m_IIDDetails);
	DDX_Text(pDX, IDC_LIBDETAILS_EDIT, m_LibDetails);
	DDX_Control(pDX, IDC_IPSEXPORT_BUTTON, m_IpsExortButton);
}


BEGIN_MESSAGE_MAP(InterfaceIDDlg, CDialogEx)
	ON_CBN_SELCHANGE  (IDC_IDDCOMBO, &InterfaceIDDlg::OnCbnSelChangeIddcombo)
	ON_BN_CLICKED(IDC_IPSEXPORT_BUTTON, &InterfaceIDDlg::OnBnClickedIpsExportButton)
END_MESSAGE_MAP()


// InterfaceIDDlg message handlers


BOOL InterfaceIDDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	IMxPluginLibDetails *iLibDetails = nullptr;
	if ((m_selPlugin == nullptr) || ((iLibDetails = m_selPlugin->GetIMxPluginLibDetails()) == nullptr))
		MX_SETERROR (MX1622, MxError::CodeDefect, MxError::TerminateApp, MxError::QuietReport, "m_selPlugin == nullptr, or m_selPlugin->GetIMxPluginLibDetails() is nullptr");
	else
	{
		CString InterfacesNotFound("");
		if (InterfacesNotFound.LoadString(IDS_NOINTERFACESFOUND) == 0)
			InterfacesNotFound = L"No interfaces found";
		CString InterfaceNotSupported("");
		if (InterfaceNotSupported.LoadString(IDS_INTERFACENOTSUPPORTED) == 0)
			InterfaceNotSupported = L"Interface not supported";

		m_IIDErrorMsg = L"";
		int nCnt = iLibDetails->GetInterfaceCount();
		if ( nCnt <= 0 )
			m_IIDErrorMsg =  InterfacesNotFound;
		else
		{
			try
			{
				USES_CONVERSION;

				m_LibDetails = A2CW(m_selPlugin->GetLibDetails());

				int index = CB_ERR;
				std::string tmp;
				CString details("");
				int x = 0;
				for (x = 0; x < nCnt; x++ )
				{
					GUID iid = iLibDetails->GetIID(x);
					if (iLibDetails->IsInterfaceSupported(iid) == false)
					{
						std::wstring wtmp;
						CString libid( MxUtils::ConvertString2WString(&wtmp, MxGUID::ConvertToString(iid, &tmp)));
						details.Format(L"%s - %s", (LPCWSTR)libid, (LPCWSTR)InterfaceNotSupported);
						m_IIDErrorMsg += details;
					}
					else
					{
						std::wstring wtmp1;
						std::wstring wtmp2;
						details.Format(L"%s - %s", MxUtils::ConvertString2WString(&wtmp1, MxGUID::ConvertToString(iid, &tmp)), MxUtils::ConvertString2WString(&wtmp2, iLibDetails->GetISummary(iid)));
						if ((index = m_IDDList.AddString(details)) == CB_ERR)
						{
							MX_SETERROR (MX1623, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "cannot add to combo box %s", iLibDetails->GetISummary(iid));
							break;
						}
						else
						{
							details.Format(L"%s - details:\r\n%s", MxUtils::ConvertString2WString(&wtmp1, MxGUID::ConvertToString(iid, &tmp)), MxUtils::ConvertString2WString(&wtmp2, iLibDetails->GetIDetails(iid)));
							std::wstring wdetails(details);
							if ( m_IDDList.SetItemDataPtr(index, AddCopyLibDetails(&wdetails)) == CB_ERR)
							{
								MX_SETERROR (MX1624, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "cannot add details to combo box for %s", iLibDetails->GetISummary(iid));
								break;
							}
						}
					}
				}
				if ( x != nCnt)
					MX_SETERROR (MX1626, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "only %d of %d interfaces processed for lib %s", x, nCnt, m_selPlugin->GetLibName());
			}
			catch(...)
			{
			}
		}
		iLibDetails->DestroyPtr();
	}
	if ( MxError::Inst().IsErrorSet())
		ShowErrorDlg::DisplayUserError(IDS_BADACTION_INTERFACEINIT, this);
	else
	{
		m_IDDList.SetCurSel(0);
		OnCbnSelChangeIddcombo();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void InterfaceIDDlg::OnCbnSelChangeIddcombo()
{
	USES_CONVERSION;    // declare locals used by the ATL macros
 
	m_IpsExortButton.ShowWindow(SW_HIDE);
	m_IIDDetails = L"";

	int index = CB_ERR;
	if ((index = m_IDDList.GetCurSel()) == CB_ERR)
		MX_SETERROR (MX1625, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "invalid selection in combo box");
	else
	{
		std::wstring *pInterfaceDetails = nullptr;
		if ( (pInterfaceDetails = static_cast<std::wstring *>(m_IDDList.GetItemDataPtr(index))) == nullptr)
			MX_SETERROR (MX1627, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "item %d has no data item ptr", index);
		else
		{
			if ( m_IIDErrorMsg.GetLength() > 0 )
			{
				m_IIDDetails = m_IIDErrorMsg;
				m_IIDDetails += L"\r\n\r\n";
			}
			m_IIDDetails += pInterfaceDetails->c_str();

			std::string  iid;
			if( MxGUID::ConvertToString(IImageCuratorPluginLibIID::Guid(), &iid) == nullptr)
				MX_SETERROR ( MX1663, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "item %d has no data item ptr", index);
			else
			{
#pragma warning(disable:6255)
				std::wstring wiid(A2CW(iid.c_str()));
#pragma warning(default:6255)
				if (pInterfaceDetails->find(wiid) != std::string::npos)
					m_IpsExortButton.ShowWindow(SW_SHOW);
			}
		}
	}
	if ( MxError::Inst().IsErrorSet())
		ShowErrorDlg::DisplayUserError(IDS_BADACTION_INTERFACESELCHANGE);

	UpdateData(FALSE);
}


void InterfaceIDDlg::OnCancel()
{
	ClearLibDetails();
	if ( MxError::Inst().IsErrorSet())
		ShowErrorDlg::DisplayUserError(IDS_BADACTION_EXITINTERFACEDLG);

	CDialogEx::OnCancel();
}


void InterfaceIDDlg::OnOK()
{
	ClearLibDetails();
	if ( MxError::Inst().IsErrorSet())
		ShowErrorDlg::DisplayUserError(IDS_BADACTION_EXITINTERFACEDLG);

	CDialogEx::OnOK();
}

std::wstring *InterfaceIDDlg::AddCopyLibDetails( std::wstring *details)
{
	std::wstring *rc = nullptr;

	if (details == nullptr)
		MX_SETERROR (MX1610, MxError::InvalidParams, MxError::Abort, MxError::QuietReport, "sid is nullptr");
	else
	{
		rc = new std::wstring(*details);
		 m_libdetails.push_back(rc);
	}
	return rc;
}

void  InterfaceIDDlg::ClearLibDetails()
{
	size_t cnt =  m_libdetails.size();
	for (size_t x = 0; x < cnt; x++)
	{
		delete   m_libdetails[x];
	}
	 m_libdetails.clear();
}

void InterfaceIDDlg::OnBnClickedIpsExportButton()
{
	USES_CONVERSION;    // declare locals used by the ATL macros

	if( m_selPlugin == nullptr)
		MX_SETERROR (MX1664, MxError::CodeDefect, MxError::Abort, MxError::QuietReport, "m_selPlugin is nullptr");
	else
	{
		CFileDialog dlgFile(FALSE, L"ips");
	
		CString pathfilenameIPS;
	
		const int c_cMaxFiles = 1;
		const int c_cbBuffSize = (c_cMaxFiles * (MAX_PATH + 1)) + 1;
		dlgFile.GetOFN().lpstrFile = pathfilenameIPS.GetBuffer(c_cbBuffSize);
		dlgFile.GetOFN().nMaxFile = c_cbBuffSize;
		dlgFile.GetOFN().lpstrFilter = L"IPS Files\0*.ips\0(All Files)\0*.*\0\0";
		dlgFile.GetOFN().Flags = dlgFile.GetOFN().Flags | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	
		if ( dlgFile.DoModal() != IDOK )
			pathfilenameIPS.ReleaseBuffer();
		else
		{
			pathfilenameIPS.ReleaseBuffer();
#pragma warning(disable:6255)
			std::string savePathFilename(W2CA(pathfilenameIPS));
#pragma warning(default:6255)
			IImageCuratorPluginLib	*lib = nullptr;
			if((lib = static_cast<IImageCuratorPluginLib *>(m_selPlugin->CreateClassInstance(IImageCuratorPluginLibIID::Guid()))) == nullptr)
				MX_SETERROR (MX1665, MxError::System, MxError::Abort, MxError::QuietReport, "Get IImageCuratorPluginLib failed");
			else
			{
				IImageCurator  *icp = nullptr;
				if ((icp = lib->GetIImageCurator()) == nullptr)
					MX_SETERROR (MX1666, MxError::System, MxError::Abort, MxError::QuietReport, "Get IImageCurator failed");
				else
				{
					if(icp->SaveImagePluginSpec(savePathFilename.c_str()) == false)
						MX_SETERROR (MX1667, MxError::System, MxError::Abort, MxError::QuietReport, "Unable to save IPS");
					else
					{
						ShellExecute(NULL, L"open", L"NOTEPAD", pathfilenameIPS, NULL, SW_SHOW);
					}
				}
			}
		}
	}
	if ( MxError::Inst().IsErrorSet())
		ShowErrorDlg::DisplayUserError(IDS_BADACTION_SAVEIPS);
}

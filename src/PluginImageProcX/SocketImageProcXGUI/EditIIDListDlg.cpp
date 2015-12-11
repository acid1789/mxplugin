// EditIIDListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IMxPluginLibDetails.h"
#include "SocketImageProcXGUI.h"
#include "EditIIDListDlg.h"
#include "MxPluginLibs.h"
#include "afxdialogex.h"


// CEditIIDListDlg dialog

IMPLEMENT_DYNAMIC(CEditIIDListDlg, CDialogEx)

CEditIIDListDlg::CEditIIDListDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditIIDListDlg::IDD, pParent)
	, m_BaseInterface(_T(""))
	, m_IDDName(_T(""))
	, m_IDDGUID(_T(""))
{

}

CEditIIDListDlg::~CEditIIDListDlg()
{
}

void CEditIIDListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MXPLUGINLIBDETAILS_IDD, m_BaseInterface);
	DDX_Text(pDX, IDC_IDDNAME_EDIT, m_IDDName);
	DDX_Text(pDX, IDC_IDDGUID_EDIT, m_IDDGUID);
	DDX_Control(pDX, IDC_IID_LIST, m_IIDList);
	DDX_Control(pDX, IDC_ADDBUTTON, m_AddButton);
	DDX_Control(pDX, IDC_DELETEBUTTON, m_DeleteButton);
	DDX_Control(pDX, IDOK, m_OKButton);
}


BEGIN_MESSAGE_MAP(CEditIIDListDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEditIIDListDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_ADDBUTTON, &CEditIIDListDlg::OnBnClickedAddButton)
	ON_BN_CLICKED(IDC_DELETEBUTTON, &CEditIIDListDlg::OnBnClickedDeleteButton)
	ON_EN_CHANGE(IDC_IDDNAME_EDIT, &CEditIIDListDlg::OnEnChangeIddEdit)
	ON_EN_CHANGE(IDC_IDDGUID_EDIT, &CEditIIDListDlg::OnEnChangeIddEdit)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CEditIIDListDlg message handlers

void CEditIIDListDlg::OnDestroy()
{
	MxPluginBaseEditableIID *pIId = '\0';
	int cnt = m_IIDList.GetCount();
	for (int x = 0; x < cnt; x++ )
	{
		if ((pIId = (MxPluginBaseEditableIID *)m_IIDList.GetItemDataPtr(x)) != '\0')
			delete pIId;
	}
	m_IIDList.ResetContent();

	CDialogEx::OnDestroy();
}


BOOL CEditIIDListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_OKButton.EnableWindow(FALSE);

	CString BaseGUID(IMxPluginLibDetailsIID::GUID());
	CString BaseName(IMxPluginLibDetailsIID::Name());

	m_BaseInterface.Format(L"%s [%s]", BaseName, BaseGUID);

	m_AddButton.EnableWindow(FALSE);

	CSocketImageProcXGUIApp *pApp = dynamic_cast<CSocketImageProcXGUIApp *>(AfxGetApp());
	if (pApp != '\0')
	{
		MxPluginBaseEditableIID *pIID = '\0';
		CString IIdDescription("");
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
				int index = LB_ERR;
				IIdDescription.Format(L"%s [%s]", pIID->GetName(),pIID->GetGUID()); 
				if ((index = m_IIDList.AddString(IIdDescription)) == LB_ERR)
					delete pIID;
				else
					m_IIDList.SetItemDataPtr(index, (void *)pIID);
			}
		}
		if (m_IIDList.GetCount() > 0)
			m_IIDList.SetCurSel(0);
	}

	if ((m_IIDList.GetCount() > 0 ) && (m_IIDList.GetCurSel() != LB_ERR ))
		m_DeleteButton.EnableWindow(TRUE);
	else
		m_DeleteButton.EnableWindow(FALSE);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEditIIDListDlg::OnEnChangeIddEdit()
{
	UpdateData(TRUE);

	if ((m_IDDName.GetLength() < 1) || (m_IDDGUID.GetLength() < IMxPluginBaseIID::IIdSize))
		m_AddButton.EnableWindow(FALSE);
	else
		m_AddButton.EnableWindow(TRUE);
}

void CEditIIDListDlg::OnBnClickedOk()
{
	CSocketImageProcXGUIApp *pApp = dynamic_cast<CSocketImageProcXGUIApp *>(AfxGetApp());
	if (pApp != '\0')
	{
		LSTATUS status = NULL;
		int IIDCnt = pApp->GetRecordedIIDCnt();
		int x = 0;
		for (x = 0; x < IIDCnt; x++ )
		{
			if (pApp->DeleteRecordedIID(x, &status) == false)
				break;
		}
		if (x != IIDCnt)
		{
			CString errMsg;
			errMsg.Format(L"Error failed to delete key for RecordIID %d, error code %d\n\nSocketImageProcXGUI error codes:\n%s\n\nOperating System error codes:\n%s\n\nFor other errors see MSDN System Error Codes", x, status, MxPluginLibs::ErrorCodeList, MxPluginLibs::ErrorCodeListMicrosoft);
			AfxMessageBox (errMsg);
		}
		else
		{
			int cnt = m_IIDList.GetCount();
			for (int index = 0; index < cnt; index++ )
			{
				MxPluginBaseEditableIID *pIID = (MxPluginBaseEditableIID *)m_IIDList.GetItemDataPtr(index);
				if (pApp->SetRecordedIID(CSocketImageProcXGUIApp::AddNewIID, pIID) == false)
				{
					CString errMsg;
					errMsg.Format(L"Error: Unable to save details of %s [%s], error code = %d", pIID->GetName(), pIID->GetGUID(), GetLastError() );
					AfxMessageBox(errMsg);
					break;
				}
			}
		}
	}

	CDialogEx::OnOK();
}


void CEditIIDListDlg::OnBnClickedAddButton()
{
	UpdateData(TRUE);

	CString BaseGUID(IMxPluginLibDetailsIID::GUID());
	CString errMsg("");
	if (BaseGUID ==  m_IDDGUID)
	{	
		errMsg.Format(L"Error: GUID cannot be the same as the Base Interface\r\n%s", m_BaseInterface);
		AfxMessageBox(errMsg);
	}
	else
	{
		bool error = false;
		MxPluginBaseEditableIID *pIId = '\0';
		int cnt = m_IIDList.GetCount();
		for (int x = 0; x < cnt; x++ )
		{
			if ((pIId = (MxPluginBaseEditableIID *)m_IIDList.GetItemDataPtr(x)) != '\0')
			{
				if (pIId->GetGUID() ==  m_IDDGUID)
				{
					errMsg.Format(L"Error: GUID already exists in item %d of the list \r\n%s [%s]", x, pIId->GetName(), pIId->GetGUID() );
					AfxMessageBox(errMsg);
					error = true;
					break;
				}
			}
		}
		if (error == false)
		{
			CString IIdDescription("");
			pIId = new MxPluginBaseEditableIID(m_IDDName, m_IDDGUID);
			if (pIId != '\0')
			{
				int index = -1;
				IIdDescription.Format(L"%s [%s]", pIId->GetName(),pIId->GetGUID()); 
				if ((index = m_IIDList.AddString(IIdDescription)) == LB_ERR)
					delete pIId;
				else
				{
					m_IIDList.SetItemDataPtr(index, (void *)pIId);
					m_IIDList.SetCurSel(index);
					m_DeleteButton.EnableWindow(TRUE);
					m_OKButton.EnableWindow(TRUE);
					m_IDDName = "";
					m_IDDGUID = "";
				}
			}
		}
	}
	UpdateData(FALSE);
}


void CEditIIDListDlg::OnBnClickedDeleteButton()
{
	int index = LB_ERR;
	if ((index = m_IIDList.GetCurSel()) != LB_ERR)
	{
		MxPluginBaseEditableIID *pIId = '\0';
		if ((pIId = (MxPluginBaseEditableIID *)m_IIDList.GetItemDataPtr(index)) != '\0')
		{
			if (m_IIDList.DeleteString(index) != LB_ERR)
			{
				delete pIId;
				if (m_IIDList.GetCount() == 0 )
					m_DeleteButton.EnableWindow(FALSE);
				else
				{
					if (index > 0)
						m_IIDList.SetCurSel(index-1);
					else
						m_IIDList.SetCurSel(0);
				}
				m_OKButton.EnableWindow(TRUE);
			}
		}
	}
}





// ShowErrorDlg.cpp : implementation file
//

#include "stdafx.h"

#include "MxUtils\MxError1.h"
#include "MxPluginManager\MxPluginManagerErrorCodes1.h"

#include "MxPluginManagerGUI.h"
#include "ShowErrorDlg.h"
#include "afxdialogex.h"


// ShowErrorDlg dialog

IMPLEMENT_DYNAMIC(ShowErrorDlg, CDialogEx)

ShowErrorDlg::ShowErrorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ShowErrorDlg::IDD, pParent)
	, m_ErrorCode(_T(""))
	, m_ErrorType(_T(""))
	, m_ErrorMsg(_T(""))
	, m_ActionMsg(_T(""))
{

}

ShowErrorDlg::~ShowErrorDlg()
{
}

int ShowErrorDlg::DisplayUserError(int ActionErrorCode, CWnd *pWndToClose)
{
	int rc = IDOK;

	if ( MxError::Inst().IsErrorSet())
	{
		ShowErrorDlg dlg;

		CString actionErrorStr("");
		if(dlg.m_ActionMsg.LoadString(ActionErrorCode) == 0)
		{
			if (dlg.m_ActionMsg.LoadString(ShowErrorDlg::BaseActionCode) == 0 )
				dlg.m_ActionMsg = L"no action error message found";
		}

		CString lastErrorStr("");
		int lastCode = MxError::Inst().GetLastErrorCode();
		if(lastErrorStr.LoadString(lastCode) == 0)
		{
			if (lastErrorStr.LoadString(MX1600) == 0 )	//program error; record this error number, terminate the app and then submit a defect report
				lastErrorStr = L"no error message found";
		}
		CString firstErrorStr("");
		int firstCode = MxError::Inst().GetErrorCode();
		if(firstErrorStr.LoadString(firstCode) == 0)
		{
			if (firstErrorStr.LoadString(MX1600) == 0 )	//program error; record this error number, terminate the app and then submit a defect report
				firstErrorStr = L"no error message found";
		}
		CString moreInfo("");
		if (moreInfo.LoadString(IDS_MOREINFO) == 0)
			moreInfo = L"More information follows...";

		if (lastErrorStr == firstErrorStr)
			dlg.m_ErrorMsg.Format(L"%s", (LPCTSTR)lastErrorStr);
		else
			dlg.m_ErrorMsg.Format(L"%s\r\n\r\n%s\r\n\r\n%s", (LPCTSTR)lastErrorStr, (LPCTSTR)moreInfo, (LPCTSTR)firstErrorStr);

		rc = dlg.DoModal();

		MxError::Inst().Reset();

		if (pWndToClose != nullptr)
			pWndToClose->PostMessage(WM_CLOSE,0,0);
	}
	return rc;
}

void ShowErrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ERRORCODE_STATIC, m_ErrorCode);
	DDX_Text(pDX, IDC_ERRORTYPE_STATIC, m_ErrorType);
	DDX_Text(pDX, IDC_ERRORMSG_EDIT, m_ErrorMsg);
}


BEGIN_MESSAGE_MAP(ShowErrorDlg, CDialogEx)
	ON_BN_CLICKED(IDC_DEFECTREPORT_BUTTON, &ShowErrorDlg::OnBnClickedDefectReportButton)
END_MESSAGE_MAP()


// ShowErrorDlg message handlers


BOOL ShowErrorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowText(m_ActionMsg);

	m_ErrorCode.Format(L"%d-%d", MxError::Inst().GetLastErrorCode(),  MxError::Inst().GetErrorCode());
	int errType = (int)MxError::Inst().GetErrorCategory();
	errType += ShowErrorDlg::BaseErrorCategoryCode;
	if (m_ErrorType.LoadString(errType) == 0 )	
		m_ErrorType = L"unknown error type";

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void ShowErrorDlg::OnBnClickedDefectReportButton()
{
	CString msg("");
	if (msg.LoadString(IDS_NOTSUPPORTEDYET) == 0)
		msg = L"Feature not supported yet, sorry!";
	AfxMessageBox(msg, MB_OK);
}

#pragma once

#include "resource.h"
// ShowErrorDlg dialog

class ShowErrorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ShowErrorDlg)

public:
	static const int	 ShowErrorDlg::BaseActionCode = IDS_BADACTION_BASE;
	static const int	 ShowErrorDlg::BaseErrorCategoryCode = IDS_ERRORCAT_BASE;

public:
	ShowErrorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ShowErrorDlg();

	static int	DisplayUserError(int ActionErrorCode, CWnd *pWndToClose=nullptr );

// Dialog Data
	enum { IDD = IDD_SHOWERROR_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_ActionMsg;
	CString m_ErrorCode;
	CString m_ErrorType;
	CString m_ErrorMsg;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedDefectReportButton();
};

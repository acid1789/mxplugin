#pragma once
#include "afxwin.h"


// CEditIIDListDlg dialog

class CEditIIDListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditIIDListDlg)

public:
	CEditIIDListDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditIIDListDlg();

// Dialog Data
	enum { IDD = IDD_EDITINTERFACELIST_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_BaseInterface;
	CString m_IDDName;
	CString m_IDDGUID;

	CListBox m_IIDList;
	CButton m_AddButton;
	CButton m_DeleteButton;

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedAddButton();
	afx_msg void OnBnClickedDeleteButton();
	afx_msg void OnEnChangeIddEdit();
	afx_msg void OnDestroy();
	CButton m_OKButton;
};

#pragma once
#include "afxwin.h"
#include "MxPluginManager\MxPluginManager1.h"

// InterfaceIDDlg dialog

class InterfaceIDDlg : public CDialogEx
{
	DECLARE_DYNAMIC(InterfaceIDDlg)

public:
	InterfaceIDDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~InterfaceIDDlg();

// Dialog Data
	enum { IDD = IDD_INTERFACEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	MxPluginLib *m_selPlugin;

	CString		m_IIDDetails;
	CString		m_IIDErrorMsg;
	CString		m_LibDetails;

	CComboBox	m_IDDList;

	afx_msg void OnCbnSelChangeIddcombo();
	afx_msg void OnBnClickedIpsExportButton();

	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();

private:
	void		 ClearLibDetails();
	std::wstring *AddCopyLibDetails( std::wstring *details);

	std::vector<std::wstring *> m_libdetails;
public:

	CButton m_IpsExortButton;
};

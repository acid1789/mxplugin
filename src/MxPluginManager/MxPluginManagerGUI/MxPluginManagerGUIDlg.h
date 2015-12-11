
// MxPluginManagerGUIDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "MxPluginManager\MxPluginManager1.h"


// CMxPluginManagerGUIDlg dialog
class CMxPluginManagerGUIDlg : public CDialogEx
{
// Construction
public:
	CMxPluginManagerGUIDlg(CWnd* pParent = NULL);	// standard constructor
	~CMxPluginManagerGUIDlg();
// Dialog Data
	enum { IDD = IDD_MXPLUGINMANAGERGUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedInterfacesButton();
	afx_msg void OnBnClickedCopyLibIDButton();
	afx_msg void OnBnClickedOptionsbutton();
	afx_msg void OnBnClickedRefreshButton();
	afx_msg void OnLbnSelchangePluginList();
	afx_msg void OnCbnSelChangeSocketAppCombo();

	virtual void OnCancel();
	virtual void OnOK();

	CString m_MxPluginRootFolder;
	CString m_LibID;
	CString m_LibDetails;

	CComboBox m_SocketAppList;
	CListBox m_PluginList;
	CEdit m_LibIDCtrl;
	CButton m_InterfacesButton;

	MxPluginManager *m_PluginMngr;

private:
	bool PopulateSocketAppList();
	bool PopulateSocketAppListFromRegistry();

	void ClearSids();
	std::string *AddCopySid( std::string *sid);

	MxPluginLib *m_selPlugin;
	std::string *m_selSID;
	std::vector<std::string *> m_sids;

	bool	m_BreakOnDllLoadError;


public:
	afx_msg void OnBnClickedWebHelpButton();
};

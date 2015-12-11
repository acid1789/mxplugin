
// SocketImageProcXGUIDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "MxPluginLibs.h"

// CSocketImageProcXGUIDlg dialog
class CSocketImageProcXGUIDlg : public CDialogEx
{
// Construction
public:
	CSocketImageProcXGUIDlg(CWnd* pParent = NULL);	// standard constructor
	~CSocketImageProcXGUIDlg();

// Dialog Data
	enum { IDD = IDD_SOCKETIMAGEPROCXGUI_DIALOG };

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
	CComboBox m_PluginList;
	CComboBox m_IIDList;

	CButton m_RefreshButton;
	CButton m_ExportAllDetailsButton;
	CButton m_EditIIDsButton;

	CString m_SelPluginDetails;
	CString m_PluginRootFolder;

	afx_msg void OnBnClickedBrowseButton();
	afx_msg void OnBnClickedRefreshButton();
	afx_msg void OnCbnSelchangePluginList();
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeIidCombo();
	afx_msg void OnBnClickedEditButton();
	afx_msg void OnBnClickedExportAllDetailsButton();

private:
	bool InitIIDList(void);
	bool DeinitIIDList(void);

private:
	MxPluginLibs *m_pLibs;
	CString       m_SelGUID;

};

#pragma once
#include "afxwin.h"
#include "MxPluginManager\MxPluginManager1.h"

// OptionsDlg dialog

class OptionsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(OptionsDlg)

public:
	OptionsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~OptionsDlg();

// Dialog Data
	enum { IDD = IDD_OPTIONSDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	static const wchar_t * const OptionsDlg::LogFileFilterFileDlg;
	static const wchar_t * const OptionsDlg::ExeFileFilterFileDlg;
	static const wchar_t * const OptionsDlg::EventViewCmdDefault;
	static const wchar_t * const OptionsDlg::EventViewCmdParamsDefault;
	static const wchar_t * const OptionsDlg::LogmanQueryCmdDefault;
	static const wchar_t * const OptionsDlg::LogmanStartCmdDefault;
	static const wchar_t * const OptionsDlg::LogmanStopCmdDefault;
	static const wchar_t * const OptionsDlg::LogmanStdOutStatusLineDefault;
	static const wchar_t * const OptionsDlg::LogmanStdOutStatusValueDefault;
	static const wchar_t * const OptionsDlg::LogmanStdOutSuccessLineDefault;
	static const wchar_t * const OptionsDlg::LogmanStdOutErrorRunAsAdminLineDefault;
	static const wchar_t * const OptionsDlg::LogmanStdOutErrorInvalidTraceLineDefault;

public:
	MxPluginManager *m_PluginMngr;

	CString m_PluginRootFolder;
	CString m_AppSID;

	CString m_ETWTraceName;
	CString m_ETWStatus;
	CString m_ETWLogPathFilename;
	
	CString	m_OutputDebugStrViewerAppPathFilename;

	CString m_EventViewerCmd;
	CString m_EventViewerCmdParams;
	CString m_LogmanQueryCmd;
	CString m_LogmanStartCmd;
	CString m_LogmanStopCmd;
	CString m_LogmanStdOutStatusLine;
	CString m_LogmanStdOutStatusValue;
	CString m_LogmanStdOutSuccessLine;
	CString m_LogmanStdOutErrorRunAsAdminLine;
	CString m_LogmanStdOutErrorInvalidTraceLine;


	CComboBox m_AppsList;
	CComboBox m_LogLevelList;
	CComboBox m_LogChannelList;

	CButton m_CancelButtonCtrl;
	CButton m_StartETWButton;
	CButton m_StopETWButton;

	CButton m_BreakOnLoadErrorCtrl;

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnCbnSelChangeAppsCombo();
	afx_msg void OnCbnSelChangeLogLevelCombo();
	afx_msg void OnCbnSelChangeLogChannelCombo();

	afx_msg void OnEnChangeETWTraceNameEdit();

	afx_msg void OnBnClickedETWStartButton();
	afx_msg void OnBnClickedETWStopButton();
	afx_msg void OnBnClickedETWRefreshButton();

	afx_msg void OnBnClickedETWCopyLogButton();
	afx_msg void OnBnClickedETWOpenLogViewerButton();

	afx_msg void OnBnClickedBrowseDebugViewerAppButton();
	afx_msg void OnBnClickedOpenDebugViewerAppButton();
	afx_msg void OnBnClickedETWBrowseLogPathfileButton();

	afx_msg void OnBnClickedResetFactoryDefaultsButton();

private:
	bool			DoUpdateAppLog();
	bool			PopulateLogChannelList();
	bool			SelectLogChannel(MxError::LogChannel channel);
	MxError::LogChannel GetSelLogChannel();
	bool			PopulateLogLevelList();
	bool			SelectLogLevel(MxError::Level level);
	MxError::Level 	GetSelLogLevel();
	void			ClearProdIds();
	std::wstring *	AddCopyProdId( std::wstring *prodid);
	bool			RunLogMan(const wchar_t*CmdLine, std::wstring *StdOut);

	std::vector<std::wstring *> m_prodids;
	std::wstring *  m_SelProdId;
	bool			m_AppLogChanged;


public:

	bool	m_BreakOnDllLoadError;
};

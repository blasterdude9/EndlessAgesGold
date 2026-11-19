// AXUninstallerDlg.h : header file
//

#pragma once


// CAXUninstallerDlg dialog
class CAXUninstallerDlg : public CDialog
{
// Construction
public:
	CAXUninstallerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_AXUNINSTALLER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CString ParseLine(ULONGLONG & pos,char * Buf, ULONGLONG length, int sqCount);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};

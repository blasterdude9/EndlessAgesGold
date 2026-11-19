// CCProcessSysDlg.h : header file
//

#if !defined(AFX_CCPROCESSSYSDLG_H__A5453EDA_A8F8_4A79_A24E_EAA989385DB5__INCLUDED_)
#define AFX_CCPROCESSSYSDLG_H__A5453EDA_A8F8_4A79_A24E_EAA989385DB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCCProcessSysDlg dialog

class CCCProcessSysDlg : public CDialog
{
// Construction
public:
	CCCProcessSysDlg(CWnd* pParent = NULL);	// standard constructor
  
	long timeStamp;
	long processDuration;
	CRITICAL_SECTION       m_criticalSection;
	
// Dialog Data
	//{{AFX_DATA(CCCProcessSysDlg)
	enum { IDD = IDD_CCPROCESSSYS_DIALOG };
	CString	m_status;
	CString	m_requestDirectory;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCCProcessSysDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ProcessNew();
	BOOL ProcessResults();
	CString AddCommand(CString exactNameOnCard,
								  CString orderNumber,
								  CString cardNumber,
								  CString expDate,
								  CString amount,
								  CString tax,
								  CString adress,
								  CString zip);
	CString ExtractInvoiceID(CString txtString);
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCCProcessSysDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBUTTONcycle();
	afx_msg void OnBUTTONforce();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBUTTONBrowse();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCPROCESSSYSDLG_H__A5453EDA_A8F8_4A79_A24E_EAA989385DB5__INCLUDED_)

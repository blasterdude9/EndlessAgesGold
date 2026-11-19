#if !defined(AFX_CONTROLADDOBJECTDLG_H__F8940740_0BA0_11D3_9FE7_B0A74CC10000__INCLUDED_)
#define AFX_CONTROLADDOBJECTDLG_H__F8940740_0BA0_11D3_9FE7_B0A74CC10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ControlAddObjectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CControlAddObjectDlg dialog

class AFX_EXT_CLASS   CControlAddObjectDlg : public CDialog
{
// Construction
public:
	CControlAddObjectDlg(CWnd* pParent = NULL);   // standard constructor
    int controlType;
	BOOL GL_invertMouse;
// Dialog Data
	//{{AFX_DATA(CControlAddObjectDlg)
	enum { IDD = IDD_DIALOG_ControlAddObject };
	CButton	m_invertMouse;
	CString	m_name;
	CString	m_comboBasis;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlAddObjectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CControlAddObjectDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCHECKInvertMouse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTROLADDOBJECTDLG_H__F8940740_0BA0_11D3_9FE7_B0A74CC10000__INCLUDED_)

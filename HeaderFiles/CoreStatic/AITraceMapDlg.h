#if !defined(AFX_AITRACEMAPDLG_H__CF0CFB41_1174_11D4_B1E8_0000B4BD56DD__INCLUDED_)
#define AFX_AITRACEMAPDLG_H__CF0CFB41_1174_11D4_B1E8_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AITraceMapDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAITraceMapDlg dialog

class AFX_EXT_CLASS   CAITraceMapDlg : public CDialog
{
// Construction
public:
	CAITraceMapDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAITraceMapDlg)
	enum { IDD = IDD_DIALOG_AITrace_MAP };
	CString	m_locationName;
	float	m_traceRaius;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAITraceMapDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAITraceMapDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AITRACEMAPDLG_H__CF0CFB41_1174_11D4_B1E8_0000B4BD56DD__INCLUDED_)

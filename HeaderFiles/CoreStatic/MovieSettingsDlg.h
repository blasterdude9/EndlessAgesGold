#if !defined(AFX_MOVIESETTINGSDLG_H__9B2B7001_08BE_11D3_BDB1_D82F9F3C1245__INCLUDED_)
#define AFX_MOVIESETTINGSDLG_H__9B2B7001_08BE_11D3_BDB1_D82F9F3C1245__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MovieSettingsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMovieSettingsDlg dialog

class AFX_EXT_CLASS   CMovieSettingsDlg : public CDialog
{
// Construction
public:
	CMovieSettingsDlg(CWnd* pParent = NULL);   // standard constructor
    BOOL movieLoops;
// Dialog Data
	//{{AFX_DATA(CMovieSettingsDlg)
	enum { IDD = IDD_DIALOG_MovieSettings };
	CButton	m_radioMovieLoops;
	CString	m_movieFile;
	int		m_percBottom;
	int		m_percLeft;
	int		m_percRight;
	int		m_percTop;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMovieSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMovieSettingsDlg)
	afx_msg void OnBUTTONBROWSEfile();
	afx_msg void OnRADIOMovieLoops();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOVIESETTINGSDLG_H__9B2B7001_08BE_11D3_BDB1_D82F9F3C1245__INCLUDED_)

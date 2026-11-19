#if !defined(AFX_HUDADDDISPLAYUNITDLG_H__01A22367_A2C0_11D3_B7BC_0000B4BD56DD__INCLUDED_)
#define AFX_HUDADDDISPLAYUNITDLG_H__01A22367_A2C0_11D3_B7BC_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HudAddDisplayUnitDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHudAddDisplayUnitDlg dialog

class AFX_EXT_CLASS   CHudAddDisplayUnitDlg : public CDialog
{
// Construction
public:
	CHudAddDisplayUnitDlg(CWnd* pParent = NULL);   // standard constructor
    int typeReturned;
    int redPen,greenPen,bluePen;
    int redText,greenText,blueText;

// Dialog Data
	//{{AFX_DATA(CHudAddDisplayUnitDlg)
	enum { IDD = IDD_DLG_HUD_AddDisplayUnit };
	int		m_posX;
	int		m_posY;
	float	m_uniFloat;
	CString	m_uniText;
	int		m_fontIndex;
	CString	m_type;
	int		m_uniInt;
	int		m_lineThickness;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHudAddDisplayUnitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString GetString(int indexLocal) ;
    int GetInt(CString nameString);
	// Generated message map functions
	//{{AFX_MSG(CHudAddDisplayUnitDlg)
	afx_msg void OnBUTTONDXFBrowse();
	afx_msg void OnBUTTONRGBText();
	afx_msg void OnBUTTONRGBVectors();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HUDADDDISPLAYUNITDLG_H__01A22367_A2C0_11D3_B7BC_0000B4BD56DD__INCLUDED_)

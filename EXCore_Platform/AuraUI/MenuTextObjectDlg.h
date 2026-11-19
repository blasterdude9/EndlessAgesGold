#if !defined(AFX_MENUTEXTOBJECTDLG_H__76A51AE1_AB67_11D4_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_MENUTEXTOBJECTDLG_H__76A51AE1_AB67_11D4_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MenuTextObjectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMenuTextObjectDlg dialog

class CMenuTextObjectDlg : public CDialog
{
// Construction
public:
	CMenuTextObjectDlg(CWnd* pParent = NULL);   // standard constructor
    int m_blendSelection;
	float m_colorLinkRed;
	float m_colorLinkGreen;
	float m_colorLinkBlue;
// Dialog Data
	//{{AFX_DATA(CMenuTextObjectDlg)
	enum { IDD = IDD_DIALOG_MenuTextObject };
	CComboBox	m_blendCombo;
	float	m_fontSizeX;
	float	m_fontSizeY;
	CString	m_textData;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuTextObjectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMenuTextObjectDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonFontcolor();
	float m_justify;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENUTEXTOBJECTDLG_H__76A51AE1_AB67_11D4_B1EE_0000B4BD56DD__INCLUDED_)

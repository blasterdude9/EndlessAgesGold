#if !defined(AFX_ADDLIGHTNINGSTRIKEDLG_H__1991CE21_00EE_11D5_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_ADDLIGHTNINGSTRIKEDLG_H__1991CE21_00EE_11D5_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddLightningStrikeDlg.h : header file
//
#include "CTextureDBClass.h"
/////////////////////////////////////////////////////////////////////////////
	
/////////////////////////////////////////////////////////////////////////////
// CAddLightningStrikeDlg dialog

class CAddLightningStrikeDlg : public CDialog
{
// Construction
public:
	CAddLightningStrikeDlg(CWnd* pParent = NULL);   // standard constructor
    CTextureDBObjList   * m_textureDataBaseRef;
	int                   m_textureIndexSelection;
	
// Dialog Data
	//{{AFX_DATA(CAddLightningStrikeDlg)
	enum { IDD = IDD_DIALOG_AddLightningStrike };
	CComboBox	m_textureDatabase;
	CString	m_soundFile;
	float	m_soundFactor;
	int		m_sndID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddLightningStrikeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddLightningStrikeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonBrowse();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDLIGHTNINGSTRIKEDLG_H__1991CE21_00EE_11D5_B1EE_0000B4BD56DD__INCLUDED_)

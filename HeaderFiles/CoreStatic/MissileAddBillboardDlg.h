#if !defined(AFX_MISSILEADDBILLBOARDDLG_H__61B486A2_7081_11D4_B1ED_0000B4BD56DD__INCLUDED_)
#define AFX_MISSILEADDBILLBOARDDLG_H__61B486A2_7081_11D4_B1ED_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MissileAddBillboardDlg.h : header file
//
#include "CTextureDBClass.h"
#include "CMissileClass.h"
/////////////////////////////////////////////////////////////////////////////
// CMissileAddBillboardDlg dialog

class AFX_EXT_CLASS   CMissileAddBillboardDlg : public CDialog
{
// Construction
public:
	CMissileAddBillboardDlg(CWnd* pParent = NULL);   // standard constructor
    CTextureDBObjList * textureDBref;
	int m_textureIndexRet;
	CMissileObj * m_missileRef;
// Dialog Data
	//{{AFX_DATA(CMissileAddBillboardDlg)
	enum { IDD = IDD_DIALOG_MissileAddBillboard };
	CButton	m_materialButton;
	CComboBox	m_textureList;
	float	m_billboardSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMissileAddBillboardDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMissileAddBillboardDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBUTTONMaterial();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MISSILEADDBILLBOARDDLG_H__61B486A2_7081_11D4_B1ED_0000B4BD56DD__INCLUDED_)

#if !defined(AFX_EDITLENSFLAREDLG_H__DAE1FF41_659A_11D4_A9C0_0000B4C06895__INCLUDED_)
#define AFX_EDITLENSFLAREDLG_H__DAE1FF41_659A_11D4_A9C0_0000B4C06895__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditLensFlareDLG.h : header file
//
#include "CHaloClass.h"

/////////////////////////////////////////////////////////////////////////////
// EditLensFlareDLG dialog

class AFX_EXT_CLASS   EditLensFlareDLG : public CDialog
{
// Construction
public:
	EditLensFlareDLG( CTextureDBObjList *texList, CWnd* pParent = NULL );
	
	
	LENSFLARETYPE *lensFlare;
	int lensFlareNum;

	void UpdateListBox();
// Dialog Data
	//{{AFX_DATA(EditLensFlareDLG)
	enum { IDD = IDD_DIALOG_EditLensFlare };
	CComboBox	m_textureDataBaseDropdown;
	CListBox	m_lensSelect;
	CString	m_meshName;
	float	m_distanceAlongAxis;
	float	m_focalPointX;
	float	m_focalPointY;
	float	m_focalPointZ;
	float	m_meshScaling;
	//}}AFX_DATA

	CTextureDBObjList *textureList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EditLensFlareDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(EditLensFlareDLG)
	afx_msg void OnBTNEditLensFlareBrowse();
	virtual BOOL OnInitDialog();
	afx_msg void OnBTNEditLensFlareAddLens();
	afx_msg void OnBTNEditLensFlareDeleteLens();
	afx_msg void OnBTNEditLensFlareUpdateLens();
	afx_msg void OnSelchangeLISTEditLensFlareSelectedLens();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITLENSFLAREDLG_H__DAE1FF41_659A_11D4_A9C0_0000B4C06895__INCLUDED_)

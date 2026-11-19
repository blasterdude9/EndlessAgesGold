#if !defined(AFX_VISUALMANAGERDLG_H__C4ECBAE0_88DF_452B_99F9_F1F4E52B1AC8__INCLUDED_)
#define AFX_VISUALMANAGERDLG_H__C4ECBAE0_88DF_452B_99F9_F1F4E52B1AC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VisualManagerDlg.h : header file
//
#include "CEXmeshClass.h"
#include "CTextureDBClass.h"
/////////////////////////////////////////////////////////////////////////////
// CVisualManagerDlg dialog

class  CVisualManagerDlg : public CDialog
{
// Construction
public:
	CVisualManagerDlg(CWnd* pParent = NULL);   // standard constructor

	CEXMeshObjList * m_exMeshListREF;
	LPDIRECT3DDEVICE9 g_pd3dDeviceRef;
	CTextureDBObjList * GL_textureDataBase;
// Dialog Data
	//{{AFX_DATA(CVisualManagerDlg)
	enum { IDD = IDD_DIALOG_visualsManagerDlg };
	CListBox	m_visualList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisualManagerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InList(int sel);
	// Generated message map functions
	//{{AFX_MSG(CVisualManagerDlg)
	afx_msg void OnBUTTONAddVisual();
	afx_msg void OnBUTTONDeleteVisual();
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONEditMaterial();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISUALMANAGERDLG_H__C4ECBAE0_88DF_452B_99F9_F1F4E52B1AC8__INCLUDED_)

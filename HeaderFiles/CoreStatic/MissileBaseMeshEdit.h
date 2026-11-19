#if !defined(AFX_MISSILEBASEMESHEDIT_H__61B486A1_7081_11D4_B1ED_0000B4BD56DD__INCLUDED_)
#define AFX_MISSILEBASEMESHEDIT_H__61B486A1_7081_11D4_B1ED_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MissileBaseMeshEdit.h : header file
//
#include "CMissileClass.h"
#include "CABFunctionLib.h"
#include "ctextureDBClass.h"
/////////////////////////////////////////////////////////////////////////////
// CMissileBaseMeshEdit dialog

class  AFX_EXT_CLASS CMissileBaseMeshEdit : public CDialog
{
// Construction
public:
	CMissileBaseMeshEdit(CWnd* pParent = NULL);   // standard constructor
    CMissileObj * m_missileRef;
	CABFunctionLib cbLib;
	CTextureDBObjList * GL_textureDataBase;
	LPDIRECT3DDEVICE9 g_pd3dDeviceRef;
// Dialog Data
	//{{AFX_DATA(CMissileBaseMeshEdit)
	enum { IDD = IDD_DIALOG_MissileBaseMeshEdit };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMissileBaseMeshEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMissileBaseMeshEdit)
	afx_msg void OnBUTTONEditMeshData();
	afx_msg void OnBUTTONEditMeshMaterial();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MISSILEBASEMESHEDIT_H__61B486A1_7081_11D4_B1ED_0000B4BD56DD__INCLUDED_)

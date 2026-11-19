#if !defined(AFX_OBJECTLIBRARYDLG_H__97E9E421_F378_11D4_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_OBJECTLIBRARYDLG_H__97E9E421_F378_11D4_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ObjectLibraryDlg.h : header file
//
#include "CObjectLibraryClass.h"
#include "CABFunctionLib.h"
#include "ctextureDBClass.h"
/////////////////////////////////////////////////////////////////////////////
// CObjectLibraryDlg dialog

class AFX_EXT_CLASS   CObjectLibraryDlg : public CDialog
{
// Construction
public:
	CObjectLibraryDlg(CWnd* pParent = NULL);   // standard constructor


	LPDIRECT3DDEVICE9 g_pd3dDeviceRef;
	CObjectCollectionList * m_objectLibraryRef;
	int m_finalIndex;

	CTextureDBObjList * GL_textureDataBase;
	CABFunctionLib m_cablib;
// Dialog Data
	//{{AFX_DATA(CObjectLibraryDlg)
	enum { IDD = IDD_DIALOG_ObjectLibrary };
	CButton	m_checkEnableBoxType;
	CListBox	m_lodList;
	CListBox	m_contentList;
	CListBox	m_libraryList;
	CString	m_description;
	float	m_lodRange;
	float	m_popoutBoxBasis;
	float	m_lodBoxMaxX;
	float	m_lodBoxMaxY;
	float	m_lodBoxMaxZ;
	float	m_lodBoxMinX;
	float	m_lodBoxMinY;
	float	m_lodBoxMinZ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectLibraryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListLibraryObjects(int sel);
	void InListContents(int sel);
	void InListLods(int sel);
	// Generated message map functions
	//{{AFX_MSG(CObjectLibraryDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONAddLibraryObject();
	afx_msg void OnBUTTONAddObject();
	afx_msg void OnBUTTONImport();
	virtual void OnOK();
	afx_msg void OnSelchangeLISTLibraryObjects();
	afx_msg void OnBUTTONSetDescription();
	afx_msg void OnBUTTONSetAltCollisionModel();
	afx_msg void OnBUTTONDeleteLibraryObject();
	afx_msg void OnBUTTONRemoveAltCollisionModel();
	afx_msg void OnBUTTONDeleteObject();
	afx_msg void OnBUTTONMaterialAccess();
	afx_msg void OnBUTTONSaveLibrary();
	afx_msg void OnBUTTONLoadLibrary();
	afx_msg void OnBUTTONAddLod();
	afx_msg void OnSelchangeLISTObjectContent();
	afx_msg void OnBUTTONSetLodRange();
	afx_msg void OnBUTTONDeleteLod();
	afx_msg void OnBUTTONSetPopoutBox();
	afx_msg void OnBUTTONMerge();
	afx_msg void OnBUTTONSetLodBoxValues();
	afx_msg void OnSelchangeLISTLodList();
	afx_msg void OnCHECKEnableBoxType();
	afx_msg void OnBUTTONsetGlobalFade();
	afx_msg void OnBUTTONsetGlobalColor();
	afx_msg void OnBUTTONsetGlobalBoundingBox();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJECTLIBRARYDLG_H__97E9E421_F378_11D4_B1EE_0000B4BD56DD__INCLUDED_)

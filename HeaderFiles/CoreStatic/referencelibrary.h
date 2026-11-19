#include "CReferenceLibrary.h"
#include "CTextureDBClass.h"
#include "CABFunctionLib.h"
#include "CWldObject.h"
#include "CElementsClass.h"
#pragma once
// ReferenceLibrary.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReferenceLibrary dialog

class  AFX_EXT_CLASS CReferenceLibrary : public CDialog
{
// Construction
public:
	CReferenceLibrary(CWnd* pParent = NULL);   // standard constructor

	CABFunctionLib      m_cabLib;
	CReferenceObjList * m_referenceDBREF;
	CTextureDBObjList * GL_textureDataBase;
	LPDIRECT3DDEVICE9 g_pd3dDeviceRef;
	CWldObjectList    * m_worldObjListRef;
	CString           * m_textureSearchPathsRef;
	int                 m_texturePathCountRef;

	CGlobalMaterialShaderList   * m_cgGlobalShaderDB;
	IDirectSound8				* m_dirSndRef;
	CElementObjList             * m_elementDBREF;

	

	CButton	            m_faceCameraChk;
	CStateManagementObj * m_renderStateObjectRef;
	void                * m_gStatusRef;
// Dialog Data
	//{{AFX_DATA(CReferenceLibrary)
	enum { IDD = IDD_DIALOG_ReferenceLibrary };
	CListBox	m_lodDB;
	CListBox	m_referenceLibraryDB;
	float	m_popoutBoxMaxX;
	float	m_popoutBoxMaxY;
	float	m_popoutBoxMaxZ;
	float	m_popoutBoxMinX;
	float	m_popoutBoxMinY;
	float	m_popoutBoxMinZ;
	CString	m_nameEdit;
	//}}AFX_DATA
	int     m_sequenceIndex;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReferenceLibrary)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void InListLods(int sel);
    BOOL InList(int sel);
	// Generated message map functions
	//{{AFX_MSG(CReferenceLibrary)
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	afx_msg void OnBUTTONRefMaterial();
	afx_msg void OnBUTTONReplace();
	afx_msg void OnBUTTONReplaceSkeletal();
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONRemoveCollisionModel();
	afx_msg void OnBUTTONAddCollisionModel();
	afx_msg void OnBUTTONAddLod();
	afx_msg void OnBUTTONDeleteLod();
	afx_msg void OnBUTTONEditLod();
	afx_msg void OnSelchangeLISTReferenceLibrary();
	afx_msg void OnBUTTONSetPopoutBox();
	afx_msg void OnBUTTONSaveLib();
	afx_msg void OnBUTTONSaveUnit();
	afx_msg void OnBUTTONImportUnit();
	afx_msg void OnBUTTONLoadLib();
	afx_msg void OnBUTTONSetName();
	afx_msg void OnBUTTONReinitClipBoxes();
	afx_msg void OnBUTTONCopyClipBoxToLods();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAnimationsettings();
	afx_msg void OnBnClickedButtonUnifynormals();
	afx_msg void OnBnClickedButtonRegeneratenormals();
	afx_msg void OnBnClickedCheckFacetocam();
	afx_msg void OnBnClickedButtonloadspt();
	afx_msg void OnBnClickedButtoneditlodmaterial();
	afx_msg void OnBnClickedButtonsquenceloadsetbtn();
public:
	afx_msg void OnBnClickedBtnSkeletaladdbasic();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedBtnSkeletaledit();
public:
	afx_msg void OnBnClickedBtnSkeletallodsys();
public:
	afx_msg void OnBnClickedButtonReplaceunit();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


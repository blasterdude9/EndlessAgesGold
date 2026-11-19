
#pragma once

//
#include "CMovementModel.h"
#include "CSkeletalClass.h"
#include "ctextureDBClass.h"
#include "CElementsClass.h"
#include "CStateManagementClass.h"
#include "CBoneCursorClass.h"
#include "CExplosionClass.h"
#include "CMenu3DClass.h"
#include "SkeletalAnimationDlg.h"
#include "globalstatusvariables.h"
/////////////////////////////////////////////////////////////////////////////
// CCursorScaleDlg dialog

class AFX_EXT_CLASS   CCursorScaleDlg : public CDialog
{
// Construction
public:
	CCursorScaleDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCursorScaleDlg)
	enum { IDD = IDD_DIALOG_CursorScale };
	float	m_scale;

	CBoneCursorObj * m_cursorRef;
	CMenu3DObj * m_menuRef;
	LPDIRECT3DDEVICE9 g_pd3dDeviceRef;

	CComboBox					m_cursorTypeDList;
	CComboBox					m_cursorClickEffectDList;
	CExplosionObjList *			m_explosionList;
	CComboBox					m_cursorHoverIDList;
	CListBox					m_additionalCursorList;
	CButton	                    m_checkUseExisting;
	GlobalStatusVariables     * m_gStatus;
	float                       m_limitX;
	float                       m_limitY;

	CSkillObjectList *			m_skillDatabaseREF;
	CStateManagementObj *		m_renderStateObjectRef;
	CGlobalMaterialShaderList * m_cgGlobalShaderDB;
	IDirectSound8*				m_dirSndRef;
    CTextureDBObjList *			m_textureDataBase;
	CElementObjList *			m_elementDBREF;
	//}}AFX_DATA

	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCursorScaleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListAdditionalCursors(int sel);
	// Generated message map functions
	//{{AFX_MSG(CCursorScaleDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedButtonmnaddcursor();
public:
	afx_msg void OnBnClickedButtonmndeletecursor();
public:
	afx_msg void OnBnClickedButtonmnreplacevalues();
public:
	afx_msg void OnBnClickedButtonmneditskeletal();
public:
	afx_msg void OnLbnSelchangeListcursorchain();
public:
	afx_msg void OnBnClickedButtonEditmaincursorsk();
};


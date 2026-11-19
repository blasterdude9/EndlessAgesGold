#if !defined(AFX_EXPLOSIONBASICDLG_H__B967A901_A47E_11D4_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_EXPLOSIONBASICDLG_H__B967A901_A47E_11D4_B1EE_0000B4BD56DD__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExplosionBasicDlg.h : header file
//
#include "resource.h"
#include "CExplosionClass.h"
#include "CABFunctionLib.h"
#include "CTextureDBClass.h"
#include "CElementsClass.h"
#include "CSkillClass.h"
#include "afxwin.h"
/////////////////////////////////////////////////////////////////////////////
// CExplosionBasicDlg dialog

class AFX_EXT_CLASS CExplosionBasicDlg : public CDialog
{
// Construction
public:
	CExplosionBasicDlg(CWnd* pParent = NULL);   // standard constructor
    CExplosionObj * m_explosionRef;
	CABFunctionLib CABLIB;
	CTextureDBObjList * GL_textureDataBase;
	LPDIRECT3DDEVICE9 g_pd3dDeviceRef;
	IDirectSound8 *   m_dirSndRef;
	CElementObjList * m_elementDBREF;
	CSkillObjectList * m_skillDatabaseRef;
// Dialog Data
	//{{AFX_DATA(CExplosionBasicDlg)
	enum { IDD = IDD_DIALOG_ExplosionBasicDlg };
	CComboBox	m_elementDatabase;
	CButton	m_radioOriNone;
	CButton	m_radioOriCamera;
	CButton	m_radioOriGround;
	CListBox	m_keyframeList;
	CString	m_fileName;
	float	m_rotationKeyX;
	float	m_rotationKeyY;
	float	m_rotationKeyZ;
	float	m_scaleKeyX;
	float	m_scaleKeyY;
	float	m_scaleKeyZ;
	int		m_keyDuration;
	int		m_animationDuration;
	int		m_setFadeAt;
	CString	m_explosionName;
	int		m_splashDamage;
	float	m_damageRadius;
	float	m_pushPower;
	int		m_specifiedDamageID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExplosionBasicDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListKeyframes(int sel);
	// Generated message map functions
	//{{AFX_MSG(CExplosionBasicDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONAddKeyframe();
	afx_msg void OnBUTTONSetMeshImage();
	afx_msg void OnBUTTONReplaceKeyframe();
	afx_msg void OnButtonBrowse();
	afx_msg void OnBUTTONDeleteKeyframe();
	afx_msg void OnSelchangeLISTKeyframes();
	virtual void OnOK();
	afx_msg void OnRADIOoToCamera();
	afx_msg void OnRADIOoToGround();
	afx_msg void OnRADIOoToNone();
	afx_msg void OnBUTTONMaterialAccess();
	afx_msg void OnBUTTONSound();
	afx_msg void OnBUTTONDeleteLightSys();
	afx_msg void OnBUTTONLightSys();
	afx_msg void OnBUTTONSkeletalSys();
	afx_msg void OnBUTTONRemoveSkeletalSys();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_skillCriteriaLoc;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPLOSIONBASICDLG_H__B967A901_A47E_11D4_B1EE_0000B4BD56DD__INCLUDED_)

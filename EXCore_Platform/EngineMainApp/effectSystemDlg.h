#pragma once
#include "afxwin.h"
#include "resource.h"
#include "CSkeletalClass.h"
#include "ctextureDBClass.h"
#include "CElementsClass.h"
#include "CStateManagementClass.h"
#include "CExplosionClass.h"
#include "SkeletalAnimationDlg.h"
#include "SelectionEffect.h"
#include "globalstatusvariables.h"

// CeffectSystemDlg dialog

class CeffectSystemDlg : public CDialog
{
	DECLARE_DYNAMIC(CeffectSystemDlg)

public:
	CeffectSystemDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CeffectSystemDlg();

	CSkillObjectList *			m_skillDatabaseREF;
	CStateManagementObj *		m_renderStateObjectRef;
	CGlobalMaterialShaderList * m_cgGlobalShaderDB;
	IDirectSound8*				m_dirSndRef;
    CTextureDBObjList *			m_textureDataBase;
	CElementObjList *			m_elementDBREF;
	LPDIRECT3DDEVICE9			m_pd3dDeviceRef;
	CSelectionEffectList      * m_selectionEffectDBRef;
	GlobalStatusVariables     * m_gStatus;


	void InListEffects(int sel);

// Dialog Data
	enum { IDD = IDD_DIALOG_SelectionEffectDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedButtonAddselectioneffect();
public:
	afx_msg void OnBnClickedButtonEditselectioneffect();
public:
	afx_msg void OnBnClickedButtonReplaceselectioneffect2();
public:
	afx_msg void OnBnClickedButtonDeleteselectioneffect3();
public:
	afx_msg void OnBnClickedButtonLoadselectioneffects();
public:
	afx_msg void OnBnClickedButtonSaveselectioneffects();
public:
	CListBox m_effectList;
public:
	CString m_effectName;
public:
	afx_msg void OnLbnSelchangeListSelectioneffectlist();
};

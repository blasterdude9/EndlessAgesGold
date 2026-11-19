#include "EnvParticleSystem.h"


#pragma once



 // CParticleInterfaceDlg dialog

class CParticleInterfaceDlg : public CDialog
{
	DECLARE_DYNAMIC(CParticleInterfaceDlg)

public:
	CParticleInterfaceDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CParticleInterfaceDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_ParticleInterface };

	EnvParticleSystem * m_particleSystemRef;
	void * m_gStatusMem;
//custom functions
	void RenderParticleControl();
	void UpdateParticleDisplay();
	void UpdateDialog();
	void EvalKeyframeState();
	void InListParticleSystems(int sel);
	void ClearSelectionFocus();
	void OnNewParticleSystem();
	void OnSaveParticleSystem();
    void OnLoadParticleSystem();

//live locks
	HprtKeyFrame * m_lockedKeyFrame;
	RECT           m_renderRect;

	CListBox	   m_systemsList;
	CEdit          m_sysName;
	CEdit          m_keySensitivity;



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
public:
	CStatic m_drawSurface;
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnEnChangeEditparticlebasemeshsize();
	CEdit m_baseSize;
	afx_msg void OnEnChangeEditparticlecount();
	CEdit m_particleCountEdit;
	CComboBox m_textureDivList;
	afx_msg void OnCbnSelchangeComboparticletexturediv();
	CEdit m_particleDurationEdit;
	CSliderCtrl m_influenceSlider;
	afx_msg void OnEnChangeEditparticlecycletime();
	afx_msg void OnNMReleasedcaptureSliderparticleinfluence(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_blendList;
	CComboBox m_textureList;
	afx_msg void OnCbnSelchangeComboparticleblend();
	afx_msg void OnBnClickedButtonBrowsetexture();
	afx_msg void OnCbnSelchangeComboparticletextureuse();
	CSliderCtrl m_keyGreen;
	CSliderCtrl m_keyBlue;
	afx_msg void OnNMReleasedcaptureSliderparticlekeyblue(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_keyRed;
	afx_msg void OnNMReleasedcaptureSliderparticlekeyred(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderparticlekeygreen(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_pSizeSlider;
	CSliderCtrl m_pPosWidthSlider;
	CSliderCtrl m_pPosHieghtSlider;
	afx_msg void OnNMReleasedcaptureSliderparticlesize(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderparticleposw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderparticleposh(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonParticledeletekey();
	afx_msg void OnSelchangeListParticlesystems();
	CButton m_deleteKeyBtn;
	void OnEnChangeEditparticleName();
	void OnEnChangeEditparticleSensitivity();
public:
	afx_msg void OnBnClickedButtonpsysdelete();
public:
	afx_msg void OnBnClickedButtonpsysimport();
public:
	afx_msg void OnBnClickedButtonpsysexport();
public:
	afx_msg void OnBnClickedButtonimportreplace();
};

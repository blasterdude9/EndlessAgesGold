#if !defined(AFX_MOTIONSETTINGSDLG_H__A7EC9241_D65C_11D2_BDB1_A62AA5311B6F__INCLUDED_)
#define AFX_MOTIONSETTINGSDLG_H__A7EC9241_D65C_11D2_BDB1_A62AA5311B6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MotionSettingsDlg.h : header file
//
#include <afxcmn.h>
#include "CPhysicsEmuClass.h"
#include "AIclass.h"
/////////////////////////////////////////////////////////////////////////////
// CMotionSettingsDlg dialog

class AFX_EXT_CLASS   CMotionSettingsDlg : public CDialog
{
// Construction
public:
	CMotionSettingsDlg(CWnd* pParent = NULL);   // standard constructor
    CMovementCaps * movementDB;
	CAIObjList    * m_aiListRef;
// Dialog Data
	//{{AFX_DATA(CMotionSettingsDlg)
	enum { IDD = IDD_DIALOG_MOTIONSETTINGS };

	CComboBox m_pkSys_firstPersonMode;
    CComboBox m_pkSys_thirdPersonMode;
	CComboBox m_pkSys_aiCfg;
	CButton m_pkSys_userConfigurable;

	CButton	m_checkFloatWhenDead;
	CSliderCtrl	m_fluidControl;
	CSliderCtrl	m_autoLevelXStrength;
	CSliderCtrl	m_autoLevelZStrength;
	CButton	m_autoLevelingX;
	CButton	m_autoLevelingZ;
	CSliderCtrl	m_sliderRotationDragOnGround;
	CSliderCtrl	m_sliderRotationDragConst;
	CSliderCtrl	m_sliderAirControl;
	CButton	m_antiGravity;
	CSliderCtrl	m_sliderAeroDynamics;
	CSliderCtrl	m_sliderGroundGrip;
	CButton	m_autoDecelForceZ;
	CButton	m_autoDecelForceY;
	CButton	m_checkJointMouseFocus;
	CButton	m_radioAutoDecel;
	float	m_accelerate;
	float	m_decellerate;
	float	m_maxPitch;
	float	m_maxSpeed;
	float	m_maxYaw;
	float	m_minSpeed;
	float	m_pitchAccel;
	float	m_pitchDecel;
	float	m_yawAccel;
	float	m_yawDecel;
	float	m_mouseRestriction;
	float	m_jumpPower;
	float	m_freeLookMaxPitch;
	float	m_freeLookMaxYaw;
	float	m_freeLookSpeed;
	float	m_mouseSensitivity;
	float	m_mass;
	float	m_fovSensitivity;
	float	m_maxFovSetting;
	float	m_minFovSetting;
	float	m_rollAccel;
	float	m_rollDecel;
	float	m_maxRoll;
	float	m_submergedHieght;
	float	m_stepTolerance;
	float	m_strafeDegrade;
	//}}AFX_DATA
    int     m_jumpRecoveryDur;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMotionSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void Refresh();
	// Generated message map functions
	//{{AFX_MSG(CMotionSettingsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLoadConfig();
	afx_msg void OnSaveConfig();
	virtual void OnOK();
	afx_msg void OnRadioAutodecel();
	afx_msg void OnCHECKJointMouseFocus();
	afx_msg void OnRADIOAutoDecelFcY();
	afx_msg void OnRADIOAutoDecelFcZ();
	afx_msg void OnCHECKAntiGravity();
	afx_msg void OnCHECKAutoLevelingX();
	afx_msg void OnCHECKAutoLevelingZ();
	afx_msg void OnCHECKEntityFloatsWhenDead();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	float m_groundGripEdit;
	float m_inertiaEdit;
	float m_crnStiffnessR;
	float m_crnStiffnessF;
	float m_maxGripEdit;
	float m_aeroDynamicsEdit;
	float m_autoLevelTol;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOTIONSETTINGSDLG_H__A7EC9241_D65C_11D2_BDB1_A62AA5311B6F__INCLUDED_)

#if !defined(AFX_LIGHTSETTINGSDLG_H__D421AB01_1A92_11D3_AFDD_DD06C23A1A25__INCLUDED_)
#define AFX_LIGHTSETTINGSDLG_H__D421AB01_1A92_11D3_AFDD_DD06C23A1A25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LightSettingsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLightSettingsDlg dialog

class AFX_EXT_CLASS   CLightSettingsDlg : public CDialog
{
// Construction
public:
	CLightSettingsDlg(CWnd* pParent = NULL);   // standard constructor
    BOOL  ocilating;
	float Lred;
	float Lgreen;
	float Lblue;
	BOOL  flickering;
	BOOL  linearAttenuation;
// Dialog Data
	//{{AFX_DATA(CLightSettingsDlg)
	enum { IDD = IDD_DIALOG_LightSettings };
	CButton	m_linearAttenuation;
	CButton	m_checkFlicker;
	CButton	m_checkOscilating;
	float	m_attenuation;
	float	m_attenuationAdjust;
	int		m_ocilatingSteps;
	int		m_dalay;
	int		m_flickerRange;
	float	m_range;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLightSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLightSettingsDlg)
	afx_msg void OnBUTTONLightColor();
	afx_msg void OnCheckOcilating();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckFlickering();
	afx_msg void OnCHECKLinearAttenuation();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIGHTSETTINGSDLG_H__D421AB01_1A92_11D3_AFDD_DD06C23A1A25__INCLUDED_)

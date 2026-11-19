#if !defined(AFX_ADDPARTICLESYSTEMDLG_H__BF7734E1_D75C_11D3_B7BE_0000B4BD56DD__INCLUDED_)
#define AFX_ADDPARTICLESYSTEMDLG_H__BF7734E1_D75C_11D3_B7BE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddParticleSystemDLG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddParticleSystemDLG dialog

class AFX_EXT_CLASS   CAddParticleSystemDLG : public CDialog
{
// Construction
public:
	CAddParticleSystemDLG(CWnd* pParent = NULL);   // standard constructor
    BOOL SortOn;
	BOOL SphereType;
	BOOL EndSpawn;
	BOOL BillboardOn;
	BOOL IsDirectional;
	BOOL FadeTime;
	BOOL Regenerate;
	BOOL VaryLifetime;

// Dialog Data
	//{{AFX_DATA(CAddParticleSystemDLG)
	enum { IDD = IDD_DLG_ADDParticleSys };
	CButton	m_VaryLifetime;
	CButton	m_Regenerate;
	CButton	m_fadeTime;
	CButton	m_isDirectional;
	CButton	m_billboardOn;
	CButton	m_sphereType;
	CButton	m_sortOn;
	CButton	m_endSpawn;
	CString	m_fileName;
	int		m_textureRef;
	float	m_startHieght;
	float	m_particleRed;
	int		m_particlelifeTime;
	float	m_particleGreen;
	int		m_particleCount;
	float	m_particleBlue;
	float	m_fieldRadius;
	float	m_initialSpeedX;
	float	m_initialSpeedY;
	float	m_initialSpeedZ;
	int     m_fadeStart;
	int  	m_fadeEnd;
	float	m_directpartSpeed;
	float	m_directpartSpread;
	float	m_BSpeed;
	float	m_BSpeedDRate;
	float	m_BSpread;
	float	m_BSpreadDRate;
	int		m_scaleEnd;
	int		m_scaleStart;
	float	m_meshScale;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddParticleSystemDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddParticleSystemDLG)
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONBrowseFileName();
	afx_msg void OnCHECKBillboardOn();
	afx_msg void OnCHECKEndSpwn();
	afx_msg void OnCHECKSortOn();
	afx_msg void OnCHECKSphereType();
	afx_msg void OnCHECKIsDirectional();
	afx_msg void OnCHECKFadeTime();
	afx_msg void OnCHECKRegenerate();
	virtual void OnOK();
	afx_msg void OnCHECKVaryLifetime();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDPARTICLESYSTEMDLG_H__BF7734E1_D75C_11D3_B7BE_0000B4BD56DD__INCLUDED_)

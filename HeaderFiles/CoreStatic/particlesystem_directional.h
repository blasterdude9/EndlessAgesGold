#if !defined(AFX_PARTICLESYSTEM_DIRECTIONAL_H__7B520921_8DC3_11D4_A9C0_0000B4C06895__INCLUDED_)
#define AFX_PARTICLESYSTEM_DIRECTIONAL_H__7B520921_8DC3_11D4_A9C0_0000B4C06895__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParticleSystem_Directional.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ParticleSystem_Directional dialog

class  AFX_EXT_CLASS ParticleSystem_Directional : public CPropertyPage
{
	DECLARE_DYNCREATE(ParticleSystem_Directional)

// Construction
public:
	ParticleSystem_Directional();
	~ParticleSystem_Directional();

	int OnQuerySiblings();

// Dialog Data
	//{{AFX_DATA(ParticleSystem_Directional)
	enum { IDD = IDD_DIALOG_ParticleSystem_Directional };
	float	m_burstSpeed;
	float	m_burstSpeedDeath;
	float	m_burstSpread;
	float	m_burstSpreadDeath;
	float	m_particleSpeed;
	float	m_particleSpread;
	int		m_scaleEnd;
	int		m_scaleStart;
	int		m_fadeEnd;
	int		m_fadeStart;
	int		m_fieldRadius;
	int		m_fieldHeight;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(ParticleSystem_Directional)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(ParticleSystem_Directional)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARTICLESYSTEM_DIRECTIONAL_H__7B520921_8DC3_11D4_A9C0_0000B4C06895__INCLUDED_)

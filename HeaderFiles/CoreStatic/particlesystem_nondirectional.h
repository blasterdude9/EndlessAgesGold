#if !defined(AFX_PARTICLESYSTEM_NONDIRECTIONAL_H__7B520922_8DC3_11D4_A9C0_0000B4C06895__INCLUDED_)
#define AFX_PARTICLESYSTEM_NONDIRECTIONAL_H__7B520922_8DC3_11D4_A9C0_0000B4C06895__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParticleSystem_NonDirectional.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ParticleSystem_NonDirectional dialog

class  AFX_EXT_CLASS ParticleSystem_NonDirectional : public CPropertyPage
{
	DECLARE_DYNCREATE(ParticleSystem_NonDirectional)

// Construction
public:
	ParticleSystem_NonDirectional();
	~ParticleSystem_NonDirectional();

// Dialog Data
	//{{AFX_DATA(ParticleSystem_NonDirectional)
	enum { IDD = IDD_DIALOG_ParticleSystem_NonDirectional };
	int		m_scaleEnd;
	int		m_scaleStart;
	int		m_fadeEnd;
	int		m_fadeStart;
	int		m_fieldRadius;
	float	m_xSpeed;
	float	m_ySpeed;
	float	m_zSpeed;
	int		m_startHeight;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(ParticleSystem_NonDirectional)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(ParticleSystem_NonDirectional)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARTICLESYSTEM_NONDIRECTIONAL_H__7B520922_8DC3_11D4_A9C0_0000B4C06895__INCLUDED_)

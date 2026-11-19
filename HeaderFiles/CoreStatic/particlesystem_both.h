
#pragma once
// ParticleSystem_Both.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ParticleSystem_Both dialog

class  AFX_EXT_CLASS ParticleSystem_Both : public CPropertyPage
{
	DECLARE_DYNCREATE(ParticleSystem_Both)

// Construction
public:
	
	ParticleSystem_Both()  : m_systemName(_T(""))
	{};
	ParticleSystem_Both(CTextureDBObjList *texList);
	~ParticleSystem_Both();
	
	CTextureDBObjList *m_textureList;
	CString m_textureName;
	int m_textureIndex;
	int m_isDirectionalChecked;
	int m_blendMethodLoc;

	RECT dimensions;

	CPropertySheet *m_sheet;
// Dialog Data
	//{{AFX_DATA(ParticleSystem_Both)
	enum { IDD = IDD_DIALOG_ParticleSystem_Both };
	CComboBox	m_textureSelect;
	CButton	m_isNormal;
	CButton	m_isDirectional;
	CStatic	m_visual;
	CSliderCtrl	m_redSlider;
	CSliderCtrl	m_greenSlider;
	CSliderCtrl	m_blueSlider;
	float	m_blue;
	float	m_green;
	float	m_red;
	BOOL	m_endSpawn;
	BOOL	m_fadeTime;
	BOOL	m_respawn;
	BOOL	m_sort;
	BOOL	m_sphereType;
	BOOL	m_varyLifetime;
	CString	m_fileName;
	int		m_particleCount;
	int		m_particleLifetime;
	float	m_meshScale;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(ParticleSystem_Both)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(ParticleSystem_Both)
	afx_msg void OnBUTTONParticleSystemBothBrowse();
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	afx_msg void OnSelchangeCOMBOtextureSelect();
	afx_msg void OnRADIOParticleSystemBothIsDirectional();
	afx_msg void OnRADIOParticleSystemBothIsNormal();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CComboBox m_blendMethod;
	CString m_systemName;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

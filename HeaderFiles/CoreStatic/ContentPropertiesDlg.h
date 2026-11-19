#if !defined(AFX_CONTENTPROPERTIESDLG_H__81D81C81_4058_11D3_AFDD_82AC5B0A2F24__INCLUDED_)
#define AFX_CONTENTPROPERTIESDLG_H__81D81C81_4058_11D3_AFDD_82AC5B0A2F24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ContentPropertiesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CContentPropertiesDlg dialog

class AFX_EXT_CLASS   CContentPropertiesDlg : public CDialog
{
// Construction
public:
	CContentPropertiesDlg(CWnd* pParent = NULL);   // standard constructor
    
// Dialog Data
	//{{AFX_DATA(CContentPropertiesDlg)
	enum { IDD = IDD_DIALOG_ContentProperties };
	int		m_gunType;
	float	m_hrdX;
	float	m_hrdY;
	float	m_hrdZ;
	int		m_fireRate;
	float	m_uMov;
	float	m_vMov;
	float	m_offsetX;
	float	m_offsetZ;
	float	m_offsetY;
	int		m_dependancyIndex;
	int		m_animTextureDBRef;
	int		m_animTexturelink;
	int		m_contentType;
	int		m_maxFuel;
	int		m_fuelUseRate;
	int		m_fuelRegenRate;
	int		m_particleLink;
	float	m_particleDirX;
	float	m_particleDirY;
	float	m_particleDirZ;
	int		m_modulationMethod;
	int		m_mirrorRenderable;
	int		m_haloType;
	int		m_lensflareType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContentPropertiesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CContentPropertiesDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTENTPROPERTIESDLG_H__81D81C81_4058_11D3_AFDD_82AC5B0A2F24__INCLUDED_)

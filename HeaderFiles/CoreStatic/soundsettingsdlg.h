#if !defined(AFX_SOUNDSETTINGSDLG_H__E4E5B901_34BD_11D3_AFDD_A9BC27470325__INCLUDED_)
#define AFX_SOUNDSETTINGSDLG_H__E4E5B901_34BD_11D3_AFDD_A9BC27470325__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SoundSettingsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSoundSettingsDlg dialog

class AFX_EXT_CLASS   CSoundSettingsDlg : public CDialog
{
// Construction
public:
	CSoundSettingsDlg(CWnd* pParent = NULL);   // standard constructor
    BOOL enabled;
	BOOL looping;
// Dialog Data
	//{{AFX_DATA(CSoundSettingsDlg)
	enum { IDD = IDD_DIALOG_SOUND };
	CButton	m_checkLooping;
	CButton	m_checkEnabled;
	float	m_distanceFactor;
	CString	m_fileName;
	int		m_sndID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSoundSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSoundSettingsDlg)
	afx_msg void OnBUTTONBrowseFileName();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckEnabled();
	afx_msg void OnCheckLooping();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOUNDSETTINGSDLG_H__E4E5B901_34BD_11D3_AFDD_A9BC27470325__INCLUDED_)

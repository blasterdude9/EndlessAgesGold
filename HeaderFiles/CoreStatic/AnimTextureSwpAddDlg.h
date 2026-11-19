#if !defined(AFX_ANIMTEXTURESWPADDDLG_H__AA9EAB01_52C0_11D3_AFDD_0000B45FDB0F__INCLUDED_)
#define AFX_ANIMTEXTURESWPADDDLG_H__AA9EAB01_52C0_11D3_AFDD_0000B45FDB0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnimTextureSwpAddDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAnimTextureSwpAddDlg dialog

class AFX_EXT_CLASS   CAnimTextureSwpAddDlg : public CDialog
{
// Construction
public:
	CAnimTextureSwpAddDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL useDecalTransparency;
	BOOL useAlphaTexture;


	float redSrc;
	float greenSrc;
	float blueSrc;

// Dialog Data
	//{{AFX_DATA(CAnimTextureSwpAddDlg)
	enum { IDD = IDD_DIALOG_AddAnimTextureDB };
	int		m_delay;
	int		m_alphaMapSize;
	int		m_howManyTextures;
	CString	m_textureName;
	int		m_alphaChannel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnimTextureSwpAddDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAnimTextureSwpAddDlg)
	afx_msg void OnCHECKdecalTransparency();
	afx_msg void OnCHECKusesAlphaTexture();
	afx_msg void OnBUTTONdecalSrcColor();
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONBrowseMap();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMTEXTURESWPADDDLG_H__AA9EAB01_52C0_11D3_AFDD_0000B45FDB0F__INCLUDED_)

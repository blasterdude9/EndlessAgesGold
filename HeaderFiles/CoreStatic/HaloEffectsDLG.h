#if !defined(AFX_HALOEFFECTSDLG_H__0B714F41_497A_11D4_A9BE_0000B4C06895__INCLUDED_)
#define AFX_HALOEFFECTSDLG_H__0B714F41_497A_11D4_A9BE_0000B4C06895__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HaloEffectsDLG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// HaloEffectsDLG dialog

class AFX_EXT_CLASS   HaloEffectsDLG : public CDialog
{
// Construction
public:
	HaloEffectsDLG(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(HaloEffectsDLG)
	enum { IDD = IDD_DIALOG_HaloEffects };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(HaloEffectsDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(HaloEffectsDLG)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HALOEFFECTSDLG_H__0B714F41_497A_11D4_A9BE_0000B4C06895__INCLUDED_)

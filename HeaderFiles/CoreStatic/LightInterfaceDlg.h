#include "CMaterialClass.h"
#include "CRTLightClass.h"
#include "CTextureDBClass.h"
#pragma once

// LightInterfaceDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLightInterfaceDlg dialog

class AFX_EXT_CLASS CLightInterfaceDlg : public CDialog
{
// Construction
public:
	CLightInterfaceDlg(CWnd* pParent = NULL);   // standard constructor

	CRTLightObj * m_lightREF;
	CTextureDBObjList * GL_textureDataBase;
// Dialog Data
	//{{AFX_DATA(CLightInterfaceDlg)
	enum { IDD = IDD_DIALOG_LightInterfaceDlg };
	float	m_attenuation0;
	float	m_attenuation1;
	float	m_attenuation2;
	float	m_range;
	CButton m_checkLightOn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLightInterfaceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLightInterfaceDlg)
	afx_msg void OnBUTTONMaterialAccess();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


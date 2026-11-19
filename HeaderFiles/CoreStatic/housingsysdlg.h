#if !defined(AFX_HOUSINGSYSDLG_H__1A7412E5_0C54_4A06_AB6C_C6C1DB9F5860__INCLUDED_)
#define AFX_HOUSINGSYSDLG_H__1A7412E5_0C54_4A06_AB6C_C6C1DB9F5860__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HousingSysDlg.h : header file
//
#include "CHousingClass.h"
#include "CTextureDBClass.h"
/////////////////////////////////////////////////////////////////////////////
// CHousingSysDlg dialog

class AFX_EXT_CLASS   CHousingSysDlg : public CDialog
{
// Construction
public:
	CHousingSysDlg(CWnd* pParent = NULL);   // standard constructor

 
	CTextureDBObjList * GL_textureDataBase;
	CHousingObjList * m_houseDBREF;
	LPDIRECT3DDEVICE9 g_pd3dDeviceRef;
	BOOL m_spwnHouseOnClose;
	int m_houseSelection;
// Dialog Data
	//{{AFX_DATA(CHousingSysDlg)
	enum { IDD = IDD_DIALOG_HouseSystem };
	CListBox	m_houseDBList;
	CString	m_houseName;
	float	m_xPosition;
	float	m_yPosition;
	float	m_zPosition;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHousingSysDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListHouses(int sel);
	// Generated message map functions
	//{{AFX_MSG(CHousingSysDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnBTNLoadHousingSystem();
	afx_msg void OnBTNSaveHousingSystem();
	afx_msg void OnBUTTONaddCloseStateCollision();
	afx_msg void OnBUTTONAddExternalVisualsL1();
	afx_msg void OnBUTTONAddOpenStateCollision();
	afx_msg void OnBUTTONCreateNew();
	afx_msg void OnBUTTONLoadInternalVisualSet();
	afx_msg void OnBUTTONLoadClipBox();
	afx_msg void OnBUTTONLoadInternalAreaBox();
	afx_msg void OnBUTTONAddExternalVisualsL2();
	afx_msg void OnBUTTONAddExternalVisualsL3();
	afx_msg void OnBUTTONLoadLodArea();
	afx_msg void OnBUTTONLoadLod2Area();
	afx_msg void OnBUTTONLoadLod3Area();
	afx_msg void OnBUTTONLoadDoorVisual();
	afx_msg void OnBUTTONAddLight();
	afx_msg void OnBUTTONDeleteLight();
	afx_msg void OnBUTTONReplaceSettings();
	afx_msg void OnSelchangeLISTHouseDatabase();
	afx_msg void OnCHECKspawnHouseOnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOUSINGSYSDLG_H__1A7412E5_0C54_4A06_AB6C_C6C1DB9F5860__INCLUDED_)

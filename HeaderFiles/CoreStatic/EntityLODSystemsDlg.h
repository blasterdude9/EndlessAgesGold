#pragma once

// EntityLODSystemsDlg.h : header file

#include "CSkeletalLODClass.h"

/////////////////////////////////////////////////////////////////////////////
// CEntityLODSystemsDlg dialog

class AFX_EXT_CLASS   CEntityLODSystemsDlg : public CDialog
{
// Construction
public:
	CEntityLODSystemsDlg(CWnd* pParent = NULL);   // standard constructor
	CSkeletalLODObjectList * m_lodRangeRefList;
// Dialog Data
	//{{AFX_DATA(CEntityLODSystemsDlg)
	enum { IDD = IDD_DIALOG_ENTITYLODSYS };
	CListBox	m_listLODSystems;
	float	m_distance;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEntityLODSystemsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListLODs(int sel);
	// Generated message map functions
	//{{AFX_MSG(CEntityLODSystemsDlg)
	afx_msg void OnButtonAddLodsys();
	afx_msg void OnBUTTONDeleteSystem();
	afx_msg void OnBUTTONReplaceValues();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListLodsystems();
	afx_msg void OnButtonEditLodsys();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


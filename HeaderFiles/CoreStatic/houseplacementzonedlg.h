#if !defined(AFX_HOUSEPLACEMENTZONEDLG_H__6A502B18_4C1F_42B1_BE70_C54B5822CCDC__INCLUDED_)
#define AFX_HOUSEPLACEMENTZONEDLG_H__6A502B18_4C1F_42B1_BE70_C54B5822CCDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HousePlacementZoneDlg.h : header file
//
#include "CHousePlacementClass.h"
#include "afxwin.h"
#include "CHousingClass.h"
/////////////////////////////////////////////////////////////////////////////
// CHousePlacementZoneDlg dialog

class AFX_EXT_CLASS   CHousePlacementZoneDlg : public CDialog
{
// Construction
public:
	CHousePlacementZoneDlg(CWnd* pParent = NULL);   // standard constructor

  
	CHousingZoneDB * m_housingZonesDB;
	LPDIRECT3DDEVICE9 g_pd3dDeviceRef;

	CHousingObjList             * m_houseObjListRef;


// Dialog Data
	//{{AFX_DATA(CHousePlacementZoneDlg)
	enum { IDD = IDD_DIALOG_PlacementZoneDlg };
	CListBox	m_zonesDB;
	CString	m_zoneName;
	int		m_worldID;
	float	m_subDivisionZ;
	float	m_subDivisionY;
	float	m_subDivisionX;
	float	m_density;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHousePlacementZoneDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InList(int sel);
	// Generated message map functions
	//{{AFX_MSG(CHousePlacementZoneDlg)
	afx_msg void OnBTNAddZone();
	afx_msg void OnBTNDeleteZone();
	afx_msg void OnBTNReplaceZone();
	virtual BOOL OnInitDialog();
	afx_msg void OnBTNZNSave();
	afx_msg void OnBTNZNLoad();
	afx_msg void OnBTNLoadVisibleBox();
	afx_msg void OnBUTTONLoadZoneBox();
	afx_msg void OnSelchangeLISTZonesDB();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_housePlacementsList;
	CListBox m_houseVaultInv;
	CListBox m_storeInv;
	afx_msg void OnLbnSelchangeListHouseplacements();
	afx_msg void OnBnClickedBtnExporttosqlhsd();
	afx_msg void OnBnClickedBtnImportfromsqlhsd();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOUSEPLACEMENTZONEDLG_H__6A502B18_4C1F_42B1_BE70_C54B5822CCDC__INCLUDED_)

#if !defined(AFX_AIPATHDLG_H__EE1C2201_1320_11D5_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_AIPATHDLG_H__EE1C2201_1320_11D5_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AIPathDlg.h : header file
//
#include <d3d9.h>
#include <d3dx9.h>
#include "CAIWebClass.h"
#include "CABFunctionLib.h"
/////////////////////////////////////////////////////////////////////////////
// CAIPathDlg dialog

class AFX_EXT_CLASS   CAIPathDlg : public CDialog
{
// Construction
public:
	CAIPathDlg(CWnd* pParent = NULL);   // standard constructor

	CAIWebObj      * m_aiObject;
	CABFunctionLib   m_cabLib;
	D3DVECTOR        m_charCurrentPosition;


// Dialog Data
	//{{AFX_DATA(CAIPathDlg)
	enum { IDD = IDD_DIALOG_AIPathDlg };
	CListBox	m_pathList;
	CListBox	m_locationList;
	int		m_locationIndex;
	int		m_nodeCount;
	CString	m_webName;
	long	m_duration;
	CString	m_locationName;
	float	m_xPos;
	float	m_yPos;
	float	m_zPos;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAIPathDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListRoute(int startLocation,int endLocation);
	void InListLocations(int sel);
	// Generated message map functions
	//{{AFX_MSG(CAIPathDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBTNQuerryPath();
	afx_msg void OnBUTTONRebuildPath();
	afx_msg void OnBUTTONCompileAll();
	afx_msg void OnBUTTONSetLocationName();
	afx_msg void OnSelchangeLISTLocationList();
	afx_msg void OnDblclkLISTLocationList();
	afx_msg void OnBUTTONIntegrityCheck();
	afx_msg void OnBUTTONaddNodeByPosition();
	afx_msg void OnBUTTONDeleteNode();
	afx_msg void OnBTNFinalizeManualTreeBuild();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AIPATHDLG_H__EE1C2201_1320_11D5_B1EE_0000B4BD56DD__INCLUDED_)

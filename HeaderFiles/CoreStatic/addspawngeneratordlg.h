#if !defined(AFX_ADDSPAWNGENERATORDLG_H__7C7704C1_DFF6_11D3_B1E8_0000B4BD56DD__INCLUDED_)
#define AFX_ADDSPAWNGENERATORDLG_H__7C7704C1_DFF6_11D3_B1E8_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddSpawnGeneratorDlg.h : header file
//
#include "CAiWebClass.h"
#include "AIclass.h"
#include "CSpawnClass.h"
/////////////////////////////////////////////////////////////////////////////
// CAddSpawnGeneratorDlg dialog

class AFX_EXT_CLASS   CAddSpawnGeneratorDlg : public CDialog
{
// Construction
public:
	CAddSpawnGeneratorDlg(CWnd* pParent = NULL);   // standard constructor
    BOOL useRuntimeEntityList;
	CAIWebObjList * treeList;
	CSpawnObj *     spawnObj;
	CAIObjList *    m_aiCfgDatabaseRef;
// Dialog Data
	//{{AFX_DATA(CAddSpawnGeneratorDlg)
	enum { IDD = IDD_DIALOG_AddSpawnGenerationPoint };
	CComboBox	m_aiCfgDB;
	CComboBox	m_trees;
	CComboBox	m_treeLocations;
	CButton	m_baseOnRuntimeEntity;
	CString	m_generatorName;
	int		m_maxOutputCapacity;
	int		m_possibilityOfSpawn;
	float	m_posX;
	float	m_posY;
	float	m_posZ;
	int		m_spawnIndex;
	float	m_spawnRadius;
	int		m_spawnType;
	int		m_aiCfg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddSpawnGeneratorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListTreeLocations(int sel);
	CString ConvertLongToString(int number);
	// Generated message map functions
	//{{AFX_MSG(CAddSpawnGeneratorDlg)
	afx_msg void OnCHECKBasedOnRuntime();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCOMBOTrees();
	virtual void OnOK();
	//}}AFX_MSG
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDSPAWNGENERATORDLG_H__7C7704C1_DFF6_11D3_B1E8_0000B4BD56DD__INCLUDED_)

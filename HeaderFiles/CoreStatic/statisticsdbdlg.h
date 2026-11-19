#if !defined(AFX_STATISTICSDBDLG_H__9A3FA9E3_B547_4C71_BBB5_C18A45258F30__INCLUDED_)
#define AFX_STATISTICSDBDLG_H__9A3FA9E3_B547_4C71_BBB5_C18A45258F30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticsDBDlg.h : header file
//
#include "CStatClass.h"

/////////////////////////////////////////////////////////////////////////////
// CStatisticsDBDlg dialog

class AFX_EXT_CLASS   CStatisticsDBDlg : public CDialog
{
// Construction
public:
	CStatisticsDBDlg(CWnd* pParent = NULL);   // standard constructor


	CStatObjList * statisticDBREF;

// Dialog Data
	//{{AFX_DATA(CStatisticsDBDlg)
	enum { IDD = IDD_DIALOG_StatisticsDB };
	CComboBox	m_benefitType;
	CListBox	m_statisticList;
	float	m_gainBasis;
	int		m_id;
	float	m_startValue;
	float	m_statisticCap;
	CString	m_statisticName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatisticsDBDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InList(int sel);
	// Generated message map functions
	//{{AFX_MSG(CStatisticsDBDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	afx_msg void OnBUTTONReplace();
	virtual void OnOK();
	afx_msg void OnSelchangeLISTStatistics();
	afx_msg void OnButtonLoad();
	afx_msg void OnButtonSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICSDBDLG_H__9A3FA9E3_B547_4C71_BBB5_C18A45258F30__INCLUDED_)

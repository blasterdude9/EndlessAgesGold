#if !defined(AFX_MERGEOPTIONSDLG_H__742E8332_0F29_4F2A_8FEC_F16DDBFBD5CF__INCLUDED_)
#define AFX_MERGEOPTIONSDLG_H__742E8332_0F29_4F2A_8FEC_F16DDBFBD5CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MergeOptionsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMergeOptionsDlg dialog

class CMergeOptionsDlg : public CDialog
{
// Construction
public:
	CMergeOptionsDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL m_aiMergeType;
// Dialog Data
	//{{AFX_DATA(CMergeOptionsDlg)
	enum { IDD = IDD_DIALOG_MergeOptionsDlg };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMergeOptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMergeOptionsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCHECKAiMergeMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_groupMode;
	afx_msg void OnBnClickedOk();
	int m_transitionGroupID;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MERGEOPTIONSDLG_H__742E8332_0F29_4F2A_8FEC_F16DDBFBD5CF__INCLUDED_)

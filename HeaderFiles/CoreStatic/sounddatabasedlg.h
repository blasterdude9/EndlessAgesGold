#include "CABFunctionLib.h"
#include "CSoundManagerClass.h"
#include "globalstatusvariables.h"

#pragma once
//

/////////////////////////////////////////////////////////////////////////////
// CSoundDatabaseDlg dialog

class AFX_EXT_CLASS   CSoundDatabaseDlg : public CDialog
{
// Construction
public:
	CSoundDatabaseDlg(CWnd* pParent = NULL);   // standard constructor
    CABFunctionLib   m_abLib;
    CSoundBankList * m_soundManagerRef;
	IDirectSound8* directSoundRef;

// Dialog Data
	//{{AFX_DATA(CSoundDatabaseDlg)
	enum { IDD = IDD_DIALOG_SoundDatabase };
	CListBox	m_soundBank;
	int			m_uniqueID;
	GlobalStatusVariables	*m_gstatusRef;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSoundDatabaseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListSoundFiles(int sel);
	// Generated message map functions
	//{{AFX_MSG(CSoundDatabaseDlg)
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONReplace();
	afx_msg void OnSelchangeLISTSoundBank();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonLoad();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTest();
};

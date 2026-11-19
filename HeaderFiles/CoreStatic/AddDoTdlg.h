#pragma once

#include "CDotClass.h"
#include "CElementsClass.h"
// AddDoTdlg dialog

class AFX_EXT_CLASS		AddDoTdlg : public CDialog
{
	DECLARE_DYNAMIC(AddDoTdlg)

public:
	AddDoTdlg(CWnd* pParent = NULL);   // standard constructor
	CDotClassList		*m_dotDBREF;
	CElementObjList     *m_currentElements;
	

// Dialog Data
	enum { IDD = IDD_DIALOG_DoTs };
	CComboBox	m_dotType;
	CComboBox	m_dotChannel;
	CListBox	m_dotList;
	CString		m_dotName;
	int			m_dotDuration;
	int			m_dotAmount;
	int			m_dotEffect;
	int			m_dotApplyPer;
	int			m_dotID;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void InListDots(int sel);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBUTTONAddDot();
	afx_msg void OnBUTTONRemoveDot();
	afx_msg void OnBUTTONReplaceDot();
	afx_msg void OnBUTTONSaveDot();
	afx_msg void OnBUTTONLoadDot();
	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnSelchangeDotList();
	

	DECLARE_MESSAGE_MAP()
};

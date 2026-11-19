#pragma once
#include "afxwin.h"


// CItemManagerDlg dialog

class CItemManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(CItemManagerDlg)

public:
	CItemManagerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CItemManagerDlg();

	int	   m_selectedEntity;
	void * globalStatusMem;
// Dialog Data
	enum { IDD = IDD_DIALOG_ItemInterface };

public:
	void InList(int sel);
	void InList(CString sel);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedButtonimadditem();
	afx_msg void OnBnClickedButtonimedititem();
	afx_msg void OnBnClickedButtonimdeleteitem();
	afx_msg void OnBnClickedButtonimarmitemtest();
public:
	CListBox m_imItemList;
public:
	afx_msg void OnBnClickedButtonimimportedb();
public:
	afx_msg void OnBnClickedButtonimimportapd();
public:
	afx_msg void OnBnClickedButtonimvisuals();
public:
	afx_msg void OnBnClickedButtonimsave();
public:
	afx_msg void OnBnClickedButtonimload();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedButtonexporttosql();
};

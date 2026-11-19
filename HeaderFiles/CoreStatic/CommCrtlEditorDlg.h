#pragma once

#include "CCommonCtrObj.h"
#include "afxwin.h"
// CCommCrtlEditorDlg dialog

class AFX_EXT_CLASS   CCommCrtlEditorDlg : public CDialog
{
	DECLARE_DYNAMIC(CCommCrtlEditorDlg)

public:
	CCommCrtlEditorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCommCrtlEditorDlg();

	CCommonCtrlList * m_controlDatabase;
	CTextureDBObjList * GL_textureDataBase;
	LPDIRECT3DDEVICE9 g_pd3dDeviceRef;
	IDirectSound8* g_directSoundRef;

// Dialog Data
	enum { IDD = IDD_DIALOG_CommCtrlEditor };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    void InListControls(int sel);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonidlestate();
	afx_msg void OnBnClickedButtondownstate();
	afx_msg void OnBnClickedButtonupstate();
	afx_msg void OnBnClickedButtonnewcontrol();
	afx_msg void OnBnClickedButtondeletecontrol();
	CString m_controlName;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CListBox m_controlList;
	afx_msg void OnBnClickedButtonreplacecntrlname();
	afx_msg void OnBnClickedButtondelidle();
	afx_msg void OnBnClickedButtondeleteupstate();
	afx_msg void OnBnClickedButtondeletedownstate();
	afx_msg void OnBnClickedButtonLoadlib();
	afx_msg void OnBnClickedButtonSavelib();
};

#pragma once

#include "CShaderLibClass.h"
#include "afxwin.h"
// CShaderLibDlg dialog

class CShaderLibDlg : public CDialog
{
	DECLARE_DYNAMIC(CShaderLibDlg)

public:
	CShaderLibDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CShaderLibDlg();

	void * m_gstatusMem;
	CShaderObjList * m_shaderLibRef;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	BOOL m_shaderTypeMode;//0=vertex 1=pixel
	CString m_shaderModeLabel;
	CButton m_fxCode;

// Dialog Data
	enum { IDD = IDD_DIALOG_ShaderLibEditor };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    void InList(int sel);

	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonSaveshaderlib();
	afx_msg void OnBnClickedBtnTestcompile();
	CString m_shaderCode;
	CString m_shaderName;
	CListBox m_shaderList;
	CListBox m_outputList;
	afx_msg void OnLbnSelchangeListshaderlist();
	afx_msg void OnBnClickedButtonloadshaderlib();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckFxcheck();
public:
//	afx_msg void OnEnVscrollEditShadercode();
public:
	afx_msg void OnEnChangeEditShadercode();
public:
	CEdit m_shaderNameEditCtrl;
public:
	CEdit m_shaderCodeCtrl;
public:
	CEdit m_debugOutput;
public:
	afx_msg void OnBnClickedButtonAddshader();
};

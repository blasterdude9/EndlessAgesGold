
#include "CGlobalMaterialShaderLib.h"
#include "CEXMeshClass.h"
#include "CTextureDBClass.h"
#include "afxwin.h"
#include "CShaderLibClass.h"
#include "CStateManagementClass.h"

#pragma once

class AFX_EXT_CLASS   CMaterialShaderLibDlg : public CDialog
{
	DECLARE_DYNAMIC(CMaterialShaderLibDlg)

public:
	CMaterialShaderLibDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMaterialShaderLibDlg();


	CTextureDBObjList * GL_textureDataBase;
	CGlobalMaterialShaderList * m_globalShaderRef;
	CABFunctionLib *cablib;

	CShaderObjList * m_pixelShaderLibRef;
	CShaderObjList * m_vertexShaderLibRef;
	CStateManagementObj * m_renderStateObjectRef;

// Dialog Data
	enum { IDD = IDD_DIALOG_MaterialShaderLib };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    void InListShaders(int sel);
	void InListShaderChain(int sel);
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	CComboBox m_level1TextureSelection;
	CComboBox m_level2TextureSelection;
	CComboBox m_level3TextureSelection;
	CComboBox m_level4TextureSelection;
	CComboBox m_level5TextureSelection;
	CListBox m_shaderList;
	afx_msg void OnBnClickedButtonAddnewshader();
	CComboBox m_level1Blend;
	CComboBox m_level2Blend;
	CComboBox m_level3Blend;
	CComboBox m_level4Blend;
	CComboBox m_level5Blend;
	int m_envUvSet;
	afx_msg void OnBnClickedButtonReplaceshader();
	afx_msg void OnBnClickedButtonDeleteshader();
	afx_msg void OnBnClickedCheckEnablematerialovverride();
	afx_msg void OnBnClickedCheckEnableenvmapping();
	CButton m_enableMaterialOvverride;
	CButton m_enableEnvMapping;
	afx_msg void OnBnClickedButtonAmbientaccess();
	afx_msg void OnLbnSelchangeListshaderlist();
	CString m_shaderName;
	afx_msg void OnBnClickedButtonDiffuseaccess();
	afx_msg void OnBnClickedButtonEmmisiveaccess();
	afx_msg void OnBnClickedButtonSpecularaccess();
	afx_msg void OnBnClickedButtonLoadshader();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonAddshadertochain();
	afx_msg void OnBnClickedButtonDelshaderfromchain();
	afx_msg void OnBnClickedButtonReplaceshaderonchain();
private:
	CComboBox m_pixelShaderDList;
	CComboBox m_vertexShaderDList;
	CComboBox m_vsReqDList;
	CComboBox m_psReqDList;
	CListBox m_shaderChainList;
	afx_msg void OnLbnSelchangeListshaderchainlist();
	int ConvertStVStoIndex(float vsfl);
	CString m_materialNotes;
};

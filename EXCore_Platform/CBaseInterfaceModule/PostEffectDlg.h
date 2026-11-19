#pragma once
#include "d3dx9.h"
#include "d3d9.h"
#include "postprocessmodule.h"
#include "afxwin.h"
// CPostEffectDlg dialog

class CPostEffectDlg : public CDialog
{
	DECLARE_DYNAMIC(CPostEffectDlg)

public:
	CPostEffectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPostEffectDlg();

	CTextureDBObjList         *  m_textureDataBaseRef;
	CPostProcessModuleManager *  m_processModuleRef;
    CPostProcessModuleManager ** m_processModuleMemoryRef;
	CShaderObjList            *  m_pxShaderSystemRef;

// Dialog Data
	enum { IDD = IDD_DIALOG_PostEffectDialog };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    void InListTextureData(int sel);
	void InListAvailShaders(int sel);
	void InListPostEffects(int sel);
	int GetSelHelperTextures(CComboBox * comboBox);
	void SetSelHelperTextures(CComboBox * comboBox, int sel);

	afx_msg void OnBnClickedButtonApplylive();
	afx_msg void OnBnClickedButtonAddnew();
	afx_msg void OnBnClickedButtonReplace();
	afx_msg void OnBnClickedButtonDelete();


	
	// Pixel Shader used to process the image after rendering, can be low level or FX
	CComboBox m_pixelShaderList;
	// Name of the post effect for reference.
	CString m_postEffectName;
	CComboBox m_textureSelection1;
	CComboBox m_textureSelection2;
	CComboBox m_textureSelection3;
	CComboBox m_textureSelection4;
	CComboBox m_textureSelection5;
	CComboBox m_textureSelection6;
	CComboBox m_textureSelection7;
	CListBox m_postEffectList;
	
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDisableposteffects();
	afx_msg void OnLbnSelchangeListposteffectlist();
	afx_msg void OnBnClickedButtonExport();
	afx_msg void OnBnClickedButtonImport();
};

#pragma once


// CVertexAnimSettingsDlg dialog


#include "CVertexAnimationModule.h"
#include "afxwin.h"

class AFX_EXT_CLASS   CVertexAnimSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CVertexAnimSettingsDlg)

public:
    CVertexAnimationModule * m_animModuleRef;
	LPDIRECT3DDEVICE9 g_pd3dDevice;


	CVertexAnimSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVertexAnimSettingsDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_VertexAnimSettings };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonAddfiles();
	afx_msg void OnBnClickedButtonClearfiles();
	CButton m_enableLooping;
	int m_timeBetweenFrames;
	CListBox m_fileList;
};

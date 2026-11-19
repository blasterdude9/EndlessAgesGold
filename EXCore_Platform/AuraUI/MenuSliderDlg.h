#if !defined(AFX_MENUSLIDERDLG_H__F8D20121_B07D_11D4_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_MENUSLIDERDLG_H__F8D20121_B07D_11D4_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MenuSliderDlg.h : header file
//
#include "CMenuSliderClass.h"
#include "afxwin.h"
/////////////////////////////////////////////////////////////////////////////
// CMenuSliderDlg dialog

class CMenuSliderDlg : public CDialog
{
// Construction
public:
	CMenuSliderDlg(CWnd* pParent = NULL);   // standard constructor
    CSliderBarObj * m_sliderBarRef;
	IDirectSound8* directSoundRef;
	CTextureDBObjList * GL_textureDataBase;
	LPDIRECT3DDEVICE9 g_pd3dDeviceRef;
// Dialog Data
	//{{AFX_DATA(CMenuSliderDlg)
	enum { IDD = IDD_DIALOG_MenuSliderDlg };
	float	m_boxClickSize;
	float	m_clickWidth;
	float	m_fontSizeX;
	float	m_fontSizeY;
	float	m_offsetX;
	float	m_positionBottomX;
	float	m_positionBottomY;
	float	m_positionMidX;
	float	m_positionMidY;
	float	m_positionTopX;
	float	m_positionTopY;
	int		m_visibleItems;
	float	m_visSpacing;
	int		m_sliderControlID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuSliderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMenuSliderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONCreateTopSystem();
	afx_msg void OnBUTTONCreateBottomSystem();
	afx_msg void OnBUTTONCreateMidSystem();
	afx_msg void OnBUTTONEditSystemBottom();
	afx_msg void OnBUTTONEditSystemMid();
	afx_msg void OnBUTTONEditSystemTop();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CButton m_selectionAbilityDisabled;
	float m_fontRed;
	float m_fontGreen;
	float m_fontBlue;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENUSLIDERDLG_H__F8D20121_B07D_11D4_B1EE_0000B4BD56DD__INCLUDED_)


#pragma once // _MSC_VER > 1000
// AppendageCustDlg.h : header file
//
#include "CMovementModel.h"
#include "CSkeletalClass.h"
#include "ctextureDBClass.h"
#include "CElementsClass.h"
#include "CStateManagementClass.h"
#include "afxwin.h"


/////////////////////////////////////////////////////////////////////////////
// CAppendageCustDlg dialog

class AFX_EXT_CLASS   CAppendageCustDlg : public CDialog
{
// Construction
public:
	CSkillObjectList * m_skillDatabaseREF;
	CStateManagementObj * m_renderStateObjectRef;
	CGlobalMaterialShaderList * m_cgGlobalShaderDB;
	CAppendageCustDlg(CWnd* pParent = NULL);   // standard constructor
    CMovementObj * m_focusMovPtr;
	IDirectSound8* m_dirSndRef;
    CTextureDBObjList * GL_textureDataBase;
	CElementObjList * m_elementDBREF;
	LPDIRECT3DDEVICE9 g_pd3dDeviceRef;
	CArmedInventoryList * m_armedInvenListRef;
	void                * m_gStatusRef;
	int (* AddTextureFunction)(CEXMeshObj * meshObj);
   
// Dialog Data
	//{{AFX_DATA(CAppendageCustDlg)
	enum { IDD = IDD_DIALOG_EntityAppendageDlg };
	CComboBox	m_cmboFunctionAttribute;
	CComboBox	m_bonesSel;
	CListBox	m_appendageList;
	int		m_miscInt;
	CString	m_name;
	int		m_boneAttached;
	int		m_functionalityAttrib;
	int		m_miscInt2;
	float	m_positionX;
	float	m_positionY;
	float	m_positionZ;
	int		m_locationOccupancyID;
	int		m_skillUse;
	int		m_abilityUseType;
	int		m_animAccessVersion;
	CString	m_miscIntText;
	CString	m_miscInt2Text;
	int		m_miscInt3;
	CString	m_miscInt3Text;
	int		m_type;
	int		m_dimCfgIndex;
	int		m_dimCfgChangeTo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppendageCustDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListAppendages(int selHighLight);
	void BonesInList();

	// Generated message map functions
	//{{AFX_MSG(CAppendageCustDlg)
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	afx_msg void OnBUTTONDeleteFirstPersonSkeleton();
	afx_msg void OnBUTTONDeleteThirdPersonSkeleton();
	afx_msg void OnBUTTONEditFirstPersonSkeleton();
	afx_msg void OnBUTTONEditThirdPersonSkeleton();
	afx_msg void OnBUTTONReplaceValues();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLISTAppendages();
	afx_msg void OnSelchangeCOMBOFunctionAttribute();
	afx_msg void OnBUTTONExport();
	afx_msg void OnBUTTONImport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTransform();
	afx_msg void OnBnClickedButtonImportkeepid();
public:
	afx_msg void OnBnClickedButtonImportreplace();
public:
	afx_msg void OnBnClickedButtonImportcompletefromdir();
public:
	afx_msg void OnBnClickedButtonImportreplacecompletefromdir();
public:
	afx_msg void OnBnClickedButtonImportreplacekeepcompletefromdir();
public:
	afx_msg void OnBnClickedButtonAddfirstpersonthroughdir();
public:
	afx_msg void OnBnClickedButtonAddthirdpersonthroughdir();
public:
	CButton m_alwaysShowFirstPersonCK;
public:
	afx_msg void OnBnClickedCheckalwaysshowfirstperson();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


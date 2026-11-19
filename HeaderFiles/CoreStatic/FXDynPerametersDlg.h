#pragma once

 #include "CABFunctionLib.h"
// CFXDynPerametersDlg dialog


#define DEF_ARBNONE 0
#define DEF_ARBCBUTTON 1
#define DEF_ARBCEDIT 2

#define DEF_ABFLOAT 0
#define DEF_ABINT 1
#define DEF_ABVECTOR 2
#define DEF_ABSTRING 3
#define DEF_4FLOATVAL 4
#define DEF_4X4FLOATVAL 5
#define DEF_UNKNOWN 6
#define DEF_4X3FLOATVAL 7

struct _DefArbConrol
{
	int dataType;//referenced type
    int controlType;
	int controlCommandRef;
	CButton cButtons;
    CEdit cEdits;
	int dataSize;
	byte * data;
};



class AFX_EXT_CLASS  CFXDynPerametersDlg : public CDialog
{
	DECLARE_DYNAMIC(CFXDynPerametersDlg)

public:
	CFXDynPerametersDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFXDynPerametersDlg();
//vars
public:
	CABFunctionLib m_cablib;
	LPD3DXEFFECT m_pEffectRef;
	int perameterCount;

	_DefArbConrol * m_peramControlsDefs;
	
	UINT uIDstart;
    UINT uIDcurrent;
	//fonts
	CFont m_smallArial;
	CFont m_mediumArial;
// Dialog Data
	enum { IDD = IDD_DIALOG_FXDynPerameters };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	virtual BOOL OnInitDialog();
	int GetParameterCount();
	BOOL AddControl(int type,
									 int curTop,
									 int curBottom, 
									 int curLeft, 
									 int curRight,
									 int commandRef,
									 int uid,
									 DWORD style,
									 D3DXPARAMETER_DESC descFX,
									 CFont * fontRef);
	void SetControlText(int uid,CString text);
	void SetControlText(int uid,float num);
    
	


protected:
//	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
//	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
protected:
//	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};

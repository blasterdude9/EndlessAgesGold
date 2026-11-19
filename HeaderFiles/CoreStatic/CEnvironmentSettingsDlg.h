#ifndef CEnvironmentSettingsDlg_H
#define CEnvironmentSettingsDlg_H

#define WINVER 0x0502


#include <afx.h>
#include "resource.h"
#include "direct.h"
#include "globalstatusvariables.h"
#include "colordlg.h"
#include "timesystemdlg.h"
#include "materialdlg.h"

///////////////////////////////////////////////////////////////////////////////
// Function:	PROTOTYPES
///////////////////////////////////////////////////////////////////////////////

int GetTreeChildIndex(HWND hTree, HTREEITEM hItem);
CString GetTreeItemText(UINT, CDialogBar*, HTREEITEM, int);

///////////////////////////////////////////////////////////////////////////////
// Function:	CEnvironmentSettingsDlg
// Description:	The environment settings dialog control.
///////////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CEnvironmentSettingsDlg : public CDialog
{
	friend int GetTreeChildIndex(HWND, HTREEITEM);
	friend CString GetTreeItemText(UINT, CDialogBar*, HTREEITEM, int);

public:
	enum { IDD = IDD_DIALOG_ENVIORMENT };

	CEnvironmentSettingsDlg(CWnd *pParent = 0);
	~CEnvironmentSettingsDlg();

	inline void		Status(void *pValue) { this->m_pStatus = static_cast<GlobalStatusVariables*>(pValue); }
	inline void		ObjectAddDialog(CDialogBar *pValue) { this->m_pObjectAddDialog = pValue; }

	BOOL			OnInitDialog();
	void			OnOK();
	void			EnvironmentUpdate(CFrameObj*);
	void			RefreshEnviorment();
	afx_msg void	OnBTNENVBckRgb();
	afx_msg void	OnBTNENVFogRgb();
	afx_msg void	OnBTNENVApplyChanges();
	afx_msg void	OnBTNENVMatch();
	afx_msg void	OnCHECKENVFogOn();
	afx_msg void	OnBTNENVBrowseSurObj();
	afx_msg void	OnCHECKEnableSurrounding();
	afx_msg void	OnBTNENVemmisive();
	afx_msg void	OnCHECKENVenableSun();
	afx_msg void	OnBTNENVsunColor();
	afx_msg void	OnBTNENVFogDay();
	afx_msg void	OnCHECKENVAmbientLightOn();
	afx_msg void	OnBTNENVambientDayLight();
	afx_msg void	OnCHECKENVTimeSystem();
	afx_msg void	OnCHECKENVWindRandomizer();
	afx_msg void	OnBTNENVTimeSystem();
	DECLARE_DYNAMIC(CEnvironmentSettingsDlg)

protected:
	DECLARE_MESSAGE_MAP()
	virtual void	DoDataExchange(CDataExchange* pDX);

private:
	GlobalStatusVariables	*m_pStatus;
	CDialogBar				*m_pObjectAddDialog;
};

#endif
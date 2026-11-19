#ifndef CSPAWNGENERATORDLG_H
#define CSPAWNGENERATORDLG_H

#define WINVER 0x0502

#include <afx.h>
#include "resource.h"
#include "direct.h"
#include "globalstatusvariables.h"
#include "cspawnclass.h"
#include "addspawngeneratordlg.h"
#include "CAISystemDlg.h"

///////////////////////////////////////////////////////////////////////////////
// Forward Declarations
///////////////////////////////////////////////////////////////////////////////

class CAISystemDlg;

///////////////////////////////////////////////////////////////////////////////
// Function:	PROTOTYPES
///////////////////////////////////////////////////////////////////////////////

void SpawnGenDatabaseInList(int sel, GlobalStatusVariables *pStatus, CDialog *pSpawnGeneratorDlg);

///////////////////////////////////////////////////////////////////////////////
// Function:	CSpawnGeneratorDlg
// Description:	The Spawn Generator dialog control.
///////////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CSpawnGeneratorDlg : public CDialog
{
	friend void SpawnGenDatabaseInList(int sel, GlobalStatusVariables *pStatus, CDialog *pSpawnGeneratorDlg);

public:
	enum { IDD = IDD_DIALOG_SpawnGeneratorUI };
	DECLARE_DYNAMIC(CSpawnGeneratorDlg)

	CSpawnGeneratorDlg(CWnd *pParent = 0);
	~CSpawnGeneratorDlg();

	inline void		Status(void *pValue) { this->m_pStatus = static_cast<GlobalStatusVariables*>(pValue); }
	inline void		EntityDlg(CDialogBar *pValue) { this->m_pEntityDlg = pValue; }
	inline void		AISystemDlg(CAISystemDlg *pValue) { this->m_pAISystemDlg = pValue; }

	afx_msg void	OnBTNSpawnAddGen();
	afx_msg void	OnBTNSpawnDeleteGen();
	afx_msg void	OnBTNSpawnEditGen();
	afx_msg void	OnBTNSPNExportSys();
	afx_msg void	OnBTNSPNImportSys();
	afx_msg void	OnCHECKSPNEnableSystem();
	afx_msg void	OnBTNSPNAppendSys();
	BOOL			OnInitDialog();
	void			OnOK();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void	DoDataExchange(CDataExchange* pDX);

private:
	GlobalStatusVariables	*m_pStatus;
	CDialogBar				*m_pEntityDlg;
	CAISystemDlg			*m_pAISystemDlg;
};

#endif
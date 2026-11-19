#ifndef CWEAPONSYSTEMDLG_H
#define CWEAPONSYSTEMDLG_H

#define WINVER 0x0502


#include <afx.h>
#include "common.h"
#include "resource.h"
#include "direct.h"
#include "globalstatusvariables.h"
#include "motionsettingsdlg.h"
#include "laserdlg.h"
#include "lightinterfacedlg.h"
#include "missileaddbillboarddlg.h"
#include "missilebasemeshedit.h"
#include "skeletalanimationdlg.h"
#include "soundsettingsdlg.h"

// Forward Declarations
class AFX_EXT_CLASS CWeaponSystemDlg;

///////////////////////////////////////////////////////////////////////////////
// Function:	PROTOTYPES
///////////////////////////////////////////////////////////////////////////////

static void
ConvertToX(CWeaponSystemDlg& dlg, CString *fileString);

///////////////////////////////////////////////////////////////////////////////
// Function:	CWeaponSystemDlg
// Description:	The weapon system dialog control.
///////////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CWeaponSystemDlg : public CDialog
{
	friend void	ConvertToX(CWeaponSystemDlg&, CString*);

public:
	enum { IDD = IDD_DIALOG_MISSILE_Interface };
	
	CWeaponSystemDlg(CWnd *pParent = NULL);
	virtual ~CWeaponSystemDlg();

	inline void		Status(void *value) { this->m_pStatus = static_cast<GlobalStatusVariables*>(value); }

	DECLARE_DYNAMIC(CWeaponSystemDlg)
	virtual BOOL	OnInitDialog();
	virtual void	OnOK();
	BOOL			InitDBMissileObject(CMissileObj* mislPtr);
	void			MissileDatabaseInList(int);
	void			InListMissileSkillCriteria(int);
	void			InListMisChannels(int);
	void			InListElementStatus(int);
	afx_msg void	OnSelchangeLISTMISmissileDataBase();
	afx_msg void	OnBTNMISAddMissile();
	afx_msg void	OnBTNMISBrowseFileName();
	afx_msg void	OnBTNMISDeleteMissile();
	afx_msg void	OnBTNMISsave();
	afx_msg void	OnBTNMISLoad();
	afx_msg void	OnBTNMISReplaceValues();
	afx_msg void	OnBTNMISEditVisual();
	afx_msg void	OnBTNMISAddBillboardMissile();
	afx_msg void	OnBTNMISEditDynamics();
	afx_msg void	OnBTNMISSound();
	afx_msg void	OnBTNMISAddSkeletalMissile();
	afx_msg void	OnBTNMISAddLaser();
	afx_msg void	OnBTNMISDeleteLight();
	afx_msg void	OnBTNMISLightSys();
	afx_msg void	OnBTNMISSaveUnit();
	afx_msg void	OnBTNMISLoadUnit();
	afx_msg void	OnBTNAddMissileChannel();
	afx_msg void	OnBTNRemoveMissileChannel();
	
protected:
	DECLARE_MESSAGE_MAP()
	virtual void	DoDataExchange(CDataExchange* pDX);

private:
	GlobalStatusVariables *m_pStatus;
};

#endif
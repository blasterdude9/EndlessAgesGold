#ifndef CEXPLOSIONSDATABASEDLG_H
#define CEXPLOSIONSDATABASEDLG_H

#define WINVER 0x0502

#include <afx.h>
#include "common.h"
#include "resource.h"
#include "direct.h"
#include "globalstatusvariables.h"
#include "explosionbasicdlg.h"

///////////////////////////////////////////////////////////////////////////////
// Function:	CExplosionsDatabaseDlg
// Description:	The explosion database dialog control.
///////////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CExplosionsDatabaseDlg : public CDialog
{
public:
	enum { IDD = IDD_DIALOG_EXPLOSIONS };

	CExplosionsDatabaseDlg(CWnd *pParent = 0);
	virtual ~CExplosionsDatabaseDlg();

	inline void		Status(void *pValue) { this->m_pStatus = static_cast<GlobalStatusVariables*>(pValue); }

	DECLARE_DYNAMIC(CExplosionsDatabaseDlg)
	virtual BOOL	OnInitDialog();
	virtual void	OnOK();
	afx_msg void	OnBTNEXPAddExplosion();
	afx_msg void	OnBTNEXPEditExplosionObj();
	afx_msg void	OnBTNEXPDeleteExplosions();
	afx_msg void	OnBTNEXPSaveUnit();
	afx_msg void	OnBTNEXPLoadUnit();
	afx_msg void	OnBTNEXPTest();
	afx_msg void	OnBTNEXPSaveExplosions();
	afx_msg void	OnBTNEXPLoadExplosions();
	void			InListExplosionDB(int sel);
	
protected:
	DECLARE_MESSAGE_MAP()
	virtual void	DoDataExchange(CDataExchange* pDX);

private:
	GlobalStatusVariables *m_pStatus;
};

#endif
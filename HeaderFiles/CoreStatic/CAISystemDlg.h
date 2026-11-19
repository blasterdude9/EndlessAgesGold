#ifndef CAISYSTEMDLG_H
#define CAISYSTEMDLG_H

#define WINVER 0x0502

#include <afx.h>
#include "resource.h"
#include "direct.h"
#include "globalstatusvariables.h"
#include "AiSettingsDlg.h"
#include "scripteventadd.h"
#include "scriptadd.h"
#include "SqlConnectionDlg.h"
#include "cmovementmodel.h"
#include "aiclass.h"
#include "ItemConflictDlg.h"
#include "AIPathDlg.h"
#include "caiwebclass.h"
#include "airaiddbclass.h"
#include "cspawngeneratordlg.h"

///////////////////////////////////////////////////////////////////////////////
// Forward declarations
///////////////////////////////////////////////////////////////////////////////

class CSpawnGeneratorDlg;

///////////////////////////////////////////////////////////////////////////////
// Function:	CAISystemDlg
// Description:	The AI System dialog control.
///////////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CAISystemDlg : public CDialog
{
public:
	enum { IDD = IDD_DIALOG_AI_BOTS };
	DECLARE_DYNAMIC(CAISystemDlg)

	CAISystemDlg(CWnd *pParent = 0);
	virtual ~CAISystemDlg();

	inline void					Status(void *pValue) { this->m_pStatus = static_cast<GlobalStatusVariables*>(pValue); }

	afx_msg void OnBTNAICreateNew();
	afx_msg void OnBTNAIDelete();
	afx_msg void OnBTNAIAddEvent();
	afx_msg void OnBTNAIAddScript();
	afx_msg void OnBTNAIEditScript();
	afx_msg void OnBTNAIDeleteScript();
	afx_msg void OnBTNAIEditEvent();
	afx_msg void OnBTNAIDeleteEvent();
	afx_msg void OnBTNAIAddTree();
	afx_msg void OnBTNAIDeleteTree();
	afx_msg void OnBTNAIEdit();
	afx_msg void OnBTNAISaveAiData();
	afx_msg void OnBTNAILoadAiData();
	afx_msg void OnSelchangeLISTAIScripts();
	afx_msg void OnBTNAIEditAITree();
	afx_msg void OnBTNAILoadAiUnit();
	afx_msg void OnBTNAISaveAiUnit();
	afx_msg void OnBTNAIImportCollisionSys();
	afx_msg void OnBTNAIDeleteSys();
	afx_msg void OnBTNAIExportCurrentCol();
	afx_msg void OnBTNAISaveColDB();
	afx_msg void OnBTNAILoadColDB();
	afx_msg void OnBTNAIIntegrityCheck();
	afx_msg void OnBTNAInewTree();
	afx_msg void OnBTNUIAIRainSys();
	afx_msg void OnBTNAIExportToSql();

	BOOL			OnInitDialog();
	void			OnOK();
	void AIdimIntegrityCheck();
	void AITreeInList(int sel);
	void AI_HandleScript(CMovementObj * ptrLoc);
	void OnSelchangeLISTAItree();
	void ScriptInList(int sel);
	void ScriptEventsInList(int sel);
	void InListCollisionDB(int sel);
	void AI_DBInList(int sel);
	BOOL AI_UseWeapon(CMovementObj * ptrLoc,CAIObj * AIPtr);
	BOOL AI_Roam(CMovementObj * ptrLoc,CAIObj * AIPtr);
	void AddAIObject(float     closeEnoughDist,
							 float     turnTolerance,
							 int       sightType,
		                     int       sightRandVariable,
		                     float     maxSightRadius,
		                     int       difficulty,
		                     BOOL      walkingType,
		                     int       startAttackZone,
		                     int       startAttackDist,
                             int       randChillTime,
		                     int       pathfindingMethod,
		                     int       randKamakazi);

protected:
	DECLARE_MESSAGE_MAP()
	virtual void	DoDataExchange(CDataExchange* pDX);

private:
	GlobalStatusVariables	*m_pStatus;
};

#endif
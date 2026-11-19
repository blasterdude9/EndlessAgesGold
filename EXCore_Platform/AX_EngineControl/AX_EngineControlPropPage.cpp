// AX_EngineControlPropPage.cpp : Implementation of the CAX_EngineControlPropPage property page class.

#include "stdafx.h"
#include "AX_EngineControl.h"
#include "AX_EngineControlPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CAX_EngineControlPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CAX_EngineControlPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CAX_EngineControlPropPage, "AX_ENGINECONTR.AX_EngineContrPropPage.1",
	0xe41e20b1, 0xc9d7, 0x4cfe, 0x9c, 0xd3, 0xba, 0xe3, 0x84, 0x72, 0xbe, 0x27)



// CAX_EngineControlPropPage::CAX_EngineControlPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CAX_EngineControlPropPage

BOOL CAX_EngineControlPropPage::CAX_EngineControlPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_AX_ENGINECONTROL_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CAX_EngineControlPropPage::CAX_EngineControlPropPage - Constructor

CAX_EngineControlPropPage::CAX_EngineControlPropPage() :
	COlePropertyPage(IDD, IDS_AX_ENGINECONTROL_PPG_CAPTION)
{
}



// CAX_EngineControlPropPage::DoDataExchange - Moves data between page and properties

void CAX_EngineControlPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CAX_EngineControlPropPage message handlers

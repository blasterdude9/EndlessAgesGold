// AbDeploymentCntrlPropPage.cpp : Implementation of the CAbDeploymentCntrlPropPage property page class.

#include "stdafx.h"
#include "AbDeploymentCntrl.h"
#include "AbDeploymentCntrlPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CAbDeploymentCntrlPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CAbDeploymentCntrlPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CAbDeploymentCntrlPropPage, "ABDEPLOYMENTCN.AbDeploymentCnPropPage.1",
	0x685d124a, 0xb448, 0x4647, 0x80, 0xdc, 0x68, 0xc1, 0x40, 0x69, 0x96, 0xea)



// CAbDeploymentCntrlPropPage::CAbDeploymentCntrlPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CAbDeploymentCntrlPropPage

BOOL CAbDeploymentCntrlPropPage::CAbDeploymentCntrlPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_ABDEPLOYMENTCNTRL_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CAbDeploymentCntrlPropPage::CAbDeploymentCntrlPropPage - Constructor

CAbDeploymentCntrlPropPage::CAbDeploymentCntrlPropPage() :
	COlePropertyPage(IDD, IDS_ABDEPLOYMENTCNTRL_PPG_CAPTION)
{
}



// CAbDeploymentCntrlPropPage::DoDataExchange - Moves data between page and properties

void CAbDeploymentCntrlPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CAbDeploymentCntrlPropPage message handlers

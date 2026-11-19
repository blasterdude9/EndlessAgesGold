//ActiveXPortalPropPage.cpp : Implementation of the CActiveXPortalPropPage property page class.

#include "stdafx.h"
#include "ActiveXPortal.h"
#include "ActiveXPortalPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CActiveXPortalPropPage, COlePropertyPage)



//Message map

BEGIN_MESSAGE_MAP(CActiveXPortalPropPage, COlePropertyPage)
END_MESSAGE_MAP()



//Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CActiveXPortalPropPage, "ACTIVEXPORTAL.ActiveXPortalPropPage.1",
	0x8a2bb602, 0x1703, 0x4e77, 0xab, 0xe1, 0x5f, 0xd3, 0xa6, 0x0, 0xb4, 0x98)



//CActiveXPortalPropPage::CActiveXPortalPropPageFactory::UpdateRegistry -
//Adds or removes system registry entries for CActiveXPortalPropPage

BOOL CActiveXPortalPropPage::CActiveXPortalPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	 if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_ACTIVEXPORTAL_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



//CActiveXPortalPropPage::CActiveXPortalPropPage - Constructor

CActiveXPortalPropPage::CActiveXPortalPropPage() :
	COlePropertyPage(IDD, IDS_ACTIVEXPORTAL_PPG_CAPTION)
{
}



//CActiveXPortalPropPage::DoDataExchange - Moves data between page and properties

void CActiveXPortalPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



//CActiveXPortalPropPage message handlers

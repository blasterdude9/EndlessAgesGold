#include "CEnvironmentSettingsDlg.h"

///////////////////////////////////////////////////////////////////////////////
// Function:	GetTreeChildIndex(HWND, HTREEITEM) 
// Description:	Returns the index of the selected item as the number in its list.
///////////////////////////////////////////////////////////////////////////////

int
GetTreeChildIndex(HWND hTree, HTREEITEM hItem) 
{
	int iLocation = 0;
	HTREEITEM hItemLoc = hItem;
	while(hItemLoc != NULL)
	{	   
		hItemLoc = TreeView_GetPrevSibling(hTree, hItemLoc);
		if(hItemLoc) ++iLocation;
	}
	return iLocation;
}

///////////////////////////////////////////////////////////////////////////////
// Function:	GetTreeItemText(UINT, CDialogBar*, HTREEITEM, int) 
// Description:	Retrieves text from a selected object from a tree.
///////////////////////////////////////////////////////////////////////////////

CString 
GetTreeItemText(UINT id, CDialogBar *barLoc, HTREEITEM treeItem, int maxLen)
{
	char buffer[100];
	TVITEM tvItem; 
	tvItem.mask = TVIF_TEXT;
	tvItem.hItem = treeItem;
	tvItem.cchTextMax = maxLen;
	tvItem.pszText = buffer;
	BOOL res = barLoc->SendDlgItemMessage(id,TVM_GETITEM,0,(LPARAM)(TVITEM*)&tvItem);
	if(res == FALSE) return "NONE";
	CString tempLocal = tvItem.pszText;
	return tempLocal;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CEnvironmentSettingsDlg
// Function:	MACROS
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CEnvironmentSettingsDlg, CDialog)

BEGIN_MESSAGE_MAP(CEnvironmentSettingsDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_ENV_BckRgb, OnBTNENVBckRgb)
	ON_BN_CLICKED(IDC_BTN_ENV_FogRgb, OnBTNENVFogRgb)
	ON_BN_CLICKED(IDC_BTN_ENV_ApplyChanges, OnBTNENVApplyChanges)
	ON_BN_CLICKED(IDC_BTN_ENV_Match, OnBTNENVMatch)
	ON_BN_CLICKED(IDC_CHECK_ENV_FogOn, OnCHECKENVFogOn)
	ON_BN_CLICKED(IDC_BTN_ENV_BrowseSurObj, OnBTNENVBrowseSurObj)
	ON_BN_CLICKED(IDC_BTN_ENV_emmisive, OnBTNENVemmisive)
	ON_BN_CLICKED(IDC_CHECK_ENV_enableSun, OnCHECKENVenableSun)
	ON_BN_CLICKED(IDC_BTN_ENV_sunColor, OnBTNENVsunColor)
	ON_BN_CLICKED(IDC_BTN_ENV_FogDay, OnBTNENVFogDay)
	ON_BN_CLICKED(IDC_CHECK_ENV_AmbientLightOn, OnCHECKENVAmbientLightOn)
	ON_BN_CLICKED(IDC_BTN_ENV_ambientDayLight, OnBTNENVambientDayLight)
	ON_BN_CLICKED(IDC_CHECK_ENV_TimeSystem, OnCHECKENVTimeSystem)
	ON_BN_CLICKED(IDC_CHECK_ENV_WindRandomizer, OnCHECKENVWindRandomizer)
	ON_BN_CLICKED(IDC_BTN_ENV_TimeSystem, OnBTNENVTimeSystem)
	ON_BN_CLICKED(IDC_CHECK_EnableSurrounding, OnCHECKEnableSurrounding)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// Class:		CEnvironmentSettingsDlg
// Function:	Ctor/Dtor
///////////////////////////////////////////////////////////////////////////////

CEnvironmentSettingsDlg::CEnvironmentSettingsDlg(CWnd* pParent) 
	: CDialog(CEnvironmentSettingsDlg::IDD, pParent)
{
	/*
	this->SetDlgItemText(IDC_EDIT_ENV_StartRange,"10.0");
	this->SetDlgItemText(IDC_EDIT_ENV_EndRange,"1000.0");
	this->SetDlgItemText(IDC_EDIT_ENV_Density,"1.0");
	this->SetDlgItemText(IDC_EDIT_ENV_FogRed,"0.0");
	this->SetDlgItemText(IDC_EDIT_ENV_FogGreen,"0.0");
	this->SetDlgItemText(IDC_EDIT_ENV_FogBlue,"0.0");
	this->SetDlgItemText(IDC_EDIT_ENV_BckRed,"0.0");
	this->SetDlgItemText(IDC_EDIT_ENV_BckGreen,"0.0");
	this->SetDlgItemText(IDC_EDIT_ENV_BckBlue,"0.0");
	this->SendDlgItemMessage(IDC_CHECK_ENV_enableSun, BM_SETCHECK, BST_CHECKED, 0);
	*/
}

CEnvironmentSettingsDlg::~CEnvironmentSettingsDlg()
{
	
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CEnvironmentSettingsDlg
// Function:	OnInitDialog()
// Description:	Inherited from CDialog.
///////////////////////////////////////////////////////////////////////////////

BOOL
CEnvironmentSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CEnvironmentSettingsDlg
// Function:	OnOK()
// Description:	Inherited from CDialog.
///////////////////////////////////////////////////////////////////////////////

void
CEnvironmentSettingsDlg::OnOK()
{
	CDialog::OnOK();
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CEnvironmentSettingsDlg
// Function:	DoDataExchange(CDataExchange*)
// Description:	Inherited from CDialog. 
//				No control data members at this time.
///////////////////////////////////////////////////////////////////////////////

void
CEnvironmentSettingsDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CEnvironmentSettingsDlg
// Function:	OnCHECKEnableSurrounding()
// Description:	Enables and disables surroundings.
///////////////////////////////////////////////////////////////////////////////

void 
CEnvironmentSettingsDlg::OnCHECKEnableSurrounding() 
{

}

///////////////////////////////////////////////////////////////////////////////
// Class:		CEnvironmentSettingsDlg
// Function:	OnBTNENVBckRgb()
// Description:	Present the color dialog for setting the background color.
///////////////////////////////////////////////////////////////////////////////

void 
CEnvironmentSettingsDlg::OnBTNENVBckRgb() 
{
#ifndef _ClientOutput 
	CColorDlg cdlg;
	cdlg.m_red = m_pStatus->m_environmentPtr->m_backRed;
	cdlg.m_green = m_pStatus->m_environmentPtr->m_backGreen;
	cdlg.m_blue = m_pStatus->m_environmentPtr->m_backBlue;
	cdlg.DoModal();

	this->SetDlgItemText(IDC_EDIT_ENV_BckRed,m_pStatus->m_cabLib.FloatToStingCnv(cdlg.m_red));
	this->SetDlgItemText(IDC_EDIT_ENV_BckGreen,m_pStatus->m_cabLib.FloatToStingCnv(cdlg.m_green));
	this->SetDlgItemText(IDC_EDIT_ENV_BckBlue,m_pStatus->m_cabLib.FloatToStingCnv(cdlg.m_blue));

	m_pStatus->m_environmentPtr->m_backRed = cdlg.m_red;
	m_pStatus->m_environmentPtr->m_backGreen = cdlg.m_green;
	m_pStatus->m_environmentPtr->m_backBlue = cdlg.m_blue;
	m_pStatus->InitializeEnvironment(TRUE);
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CEnvironmentSettingsDlg
// Function:	OnBTNENVFogRgb()
// Description:	Present the color dialog for setting fog color.
///////////////////////////////////////////////////////////////////////////////

void 
CEnvironmentSettingsDlg::OnBTNENVFogRgb() 
{
#ifndef _ClientOutput
	CColorDlg cdlg;
	cdlg.m_red = m_pStatus->m_environmentPtr->m_fogRed;
	cdlg.m_green = m_pStatus->m_environmentPtr->m_fogGreen;
	cdlg.m_blue = m_pStatus->m_environmentPtr->m_fogBlue;
	cdlg.DoModal();

	this->SetDlgItemText( IDC_EDIT_ENV_FogRed,m_pStatus->m_cabLib.FloatToStingCnv(cdlg.m_red));
	this->SetDlgItemText( IDC_EDIT_ENV_FogGreen,m_pStatus->m_cabLib.FloatToStingCnv(cdlg.m_green));
	this->SetDlgItemText( IDC_EDIT_ENV_FogBlue,m_pStatus->m_cabLib.FloatToStingCnv(cdlg.m_blue));	

	m_pStatus->m_environmentPtr->m_fogRed = cdlg.m_red;
	m_pStatus->m_environmentPtr->m_fogGreen = cdlg.m_green;
	m_pStatus->m_environmentPtr->m_fogBlue = cdlg.m_blue;
	m_pStatus->InitializeEnvironment(TRUE);
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CEnvironmentSettingsDlg
// Function:	OnBTNENVApplyChanges()
// Description:	Apply any changes to the environment.
///////////////////////////////////////////////////////////////////////////////

void CEnvironmentSettingsDlg::OnBTNENVApplyChanges() 
{
	if(this->SendDlgItemMessage(IDC_CHECK_ENV_FogOn,BM_GETSTATE,0,0)==BST_CHECKED)
		m_pStatus->m_environmentPtr->m_fogOn = TRUE;
	else
		m_pStatus->m_environmentPtr->m_fogOn = FALSE;

	if(this->SendDlgItemMessage(IDC_CHECK_lockBackColor,BM_GETSTATE,0,0)==BST_CHECKED)
		m_pStatus->m_environmentPtr->m_lockBackColor = TRUE;
	else
		m_pStatus->m_environmentPtr->m_lockBackColor = FALSE;

	CString fogStart; 
	this->GetDlgItemText( IDC_EDIT_ENV_StartRange,fogStart);
	m_pStatus->m_environmentPtr->m_fogStart = (float)atof((LPCTSTR)fogStart);

	CString fogEnd; 
	this->GetDlgItemText( IDC_EDIT_ENV_EndRange,fogEnd);
	m_pStatus->m_environmentPtr->m_fogRange = (float)atof((LPCTSTR)fogEnd);

	CString density; 
	this->GetDlgItemText( IDC_EDIT_ENV_Density,density);
	m_pStatus->m_environmentPtr->m_fogDensity = (float)atof((LPCTSTR)density);

	CString sunPosition; 
	this->GetDlgItemText( IDC_EDIT_ENV_sunPosX,sunPosition);
	m_pStatus->m_environmentPtr->m_sunPosition.x = (float)atof((LPCTSTR)sunPosition);

	this->GetDlgItemText( IDC_EDIT_ENVsunPosY,sunPosition);
	m_pStatus->m_environmentPtr->m_sunPosition.y = (float)atof((LPCTSTR)sunPosition);

	this->GetDlgItemText( IDC_EDIT_ENVsunPosZ,sunPosition);
	m_pStatus->m_environmentPtr->m_sunPosition.z = (float)atof((LPCTSTR)sunPosition);

	CString tempPosition;

	this->GetDlgItemText( IDC_EDIT_ENV_MaxWindStrength,tempPosition);
	m_pStatus->m_environmentPtr->m_maxWindStrength = (float)atof((LPCTSTR)tempPosition);

	this->GetDlgItemText( IDC_EDIT_ENV_WindDirX,tempPosition);
	m_pStatus->m_environmentPtr->m_windDirectionX = (float)atof((LPCTSTR)tempPosition);

	this->GetDlgItemText( IDC_EDIT_ENV_WindDirZ,tempPosition);
	m_pStatus->m_environmentPtr->m_windDirectionZ = (float)atof((LPCTSTR)tempPosition);

	this->GetDlgItemText( IDC_EDIT_ENV_Gravity,tempPosition);
	m_pStatus->m_environmentPtr->m_gravity = (float)atof((LPCTSTR)tempPosition);

	this->GetDlgItemText( IDC_EDIT_ENV_SeaLevel,tempPosition);
	m_pStatus->m_environmentPtr->m_seaLevel = (float)atof((LPCTSTR)tempPosition);

	this->GetDlgItemText( IDC_EDIT_ENV_SeaDrag,tempPosition);
	m_pStatus->m_environmentPtr->m_seaViscusDrag = (float)atof((LPCTSTR)tempPosition);

	m_pStatus->InitializeEnvironment(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CEnvironmentSettingsDlg
// Function:	OnBTNENVMatch()
// Description:	Matches background to fog.
///////////////////////////////////////////////////////////////////////////////

void 
CEnvironmentSettingsDlg::OnBTNENVMatch() 
{
	this->SetDlgItemText( IDC_EDIT_ENV_BckRed,m_pStatus->m_cabLib.FloatToStingCnv(m_pStatus->m_environmentPtr->m_fogRed));
	this->SetDlgItemText( IDC_EDIT_ENV_BckGreen,m_pStatus->m_cabLib.FloatToStingCnv(m_pStatus->m_environmentPtr->m_fogGreen));
	this->SetDlgItemText( IDC_EDIT_ENV_BckBlue,m_pStatus->m_cabLib.FloatToStingCnv(m_pStatus->m_environmentPtr->m_fogBlue));

	m_pStatus->m_environmentPtr->m_backRed = m_pStatus->m_environmentPtr->m_fogRed;
	m_pStatus->m_environmentPtr->m_backGreen = m_pStatus->m_environmentPtr->m_fogGreen;
	m_pStatus->m_environmentPtr->m_backBlue = m_pStatus->m_environmentPtr->m_fogBlue;
	m_pStatus->InitializeEnvironment(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CEnvironmentSettingsDlg
// Function:	OnCHECKENVFogOn()
// Description:	Updates on fog status.
///////////////////////////////////////////////////////////////////////////////

void 
CEnvironmentSettingsDlg::OnCHECKENVFogOn() 
{
	if(m_pStatus->m_environmentPtr->m_fogOn == TRUE)
		m_pStatus->m_environmentPtr->m_fogOn=FALSE;
	else
		m_pStatus->m_environmentPtr->m_fogOn=TRUE;
	m_pStatus->InitializeEnvironment(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CEnvironmentSettingsDlg
// Function:	OnBTNENVBrowseSurObj()
// Description:	Browse visual file for environment.
///////////////////////////////////////////////////////////////////////////////

void 
CEnvironmentSettingsDlg::OnBTNENVBrowseSurObj() 
{
   
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CEnvironmentSettingsDlg
// Function:	EnvironmentUpdate(CFrameObj*)
// Description:	-
///////////////////////////////////////////////////////////////////////////////

void 
CEnvironmentSettingsDlg::EnvironmentUpdate(CFrameObj* frame)
{
	D3DVECTOR position;
	frame->GetPosition(NULL,&position);		// Get frame position

	//if(m_pStatus->m_environmentPtr->List_content->List_mainFrame)
	//m_pStatus->m_environmentPtr->List_content->List_mainFrame->SetPosition(NULL,position.x,position.y,position.z);

	//if(m_pStatus->m_environmentPtr->List_flareFrame)
	//m_pStatus->m_environmentPtr->List_flareFrame->SetPosition(NULL,position.x,position.y,position.z);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CEnvironmentSettingsDlg
// Function:	OnBTNENVemmisive()
// Description:	-
///////////////////////////////////////////////////////////////////////////////

void 
CEnvironmentSettingsDlg::OnBTNENVemmisive() 
{
   
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CEnvironmentSettingsDlg
// Function:	OnCHECKENVenableSun()
// Description:	-
///////////////////////////////////////////////////////////////////////////////

void CEnvironmentSettingsDlg::OnCHECKENVenableSun() 
{
	if(m_pStatus->m_environmentPtr->m_sunLightOn == TRUE)
		m_pStatus->m_environmentPtr->m_sunLightOn = FALSE;
	else
		m_pStatus->m_environmentPtr->m_sunLightOn = TRUE;
	m_pStatus->InitializeEnvironment(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CEnvironmentSettingsDlg
// Function:	OnBTNENVsunColor()
// Description:	Changes sun color.
///////////////////////////////////////////////////////////////////////////////

void
CEnvironmentSettingsDlg::OnBTNENVsunColor() 
{
#ifndef _ClientOutput
	CColorDlg cdlg;
	cdlg.m_red = m_pStatus->m_environmentPtr->m_sunRed;
	cdlg.m_green = m_pStatus->m_environmentPtr->m_sunGreen;
	cdlg.m_blue = m_pStatus->m_environmentPtr->m_sunBlue;
	cdlg.DoModal();
	m_pStatus->m_environmentPtr->m_sunRed = cdlg.m_red;
	m_pStatus->m_environmentPtr->m_sunGreen = cdlg.m_green;
	m_pStatus->m_environmentPtr->m_sunBlue = cdlg.m_blue;
	m_pStatus->InitializeEnvironment(TRUE);
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CEnvironmentSettingsDlg
// Function:	OnBTNENVFogDay()
// Description:	-
///////////////////////////////////////////////////////////////////////////////

void 
CEnvironmentSettingsDlg::OnBTNENVFogDay() 
{
#ifndef _ClientOutput
	CColorDlg cdlg;
	cdlg.m_red = m_pStatus->m_environmentPtr->m_fogRed;
	cdlg.m_green = m_pStatus->m_environmentPtr->m_fogGreen;
	cdlg.m_blue = m_pStatus->m_environmentPtr->m_fogBlue;
	if(cdlg.DoModal()==IDOK)
	{
		this->SetDlgItemText(IDC_EDIT_ENV_FogRed,m_pStatus->m_cabLib.FloatToStingCnv(cdlg.m_red));
		this->SetDlgItemText(IDC_EDIT_ENV_FogGreen,m_pStatus->m_cabLib.FloatToStingCnv(cdlg.m_green));
		this->SetDlgItemText(IDC_EDIT_ENV_FogBlue,m_pStatus->m_cabLib.FloatToStingCnv(cdlg.m_blue));	
		m_pStatus->m_environmentPtr->m_fogRed = cdlg.m_red;
		m_pStatus->m_environmentPtr->m_fogGreen = cdlg.m_green;
		m_pStatus->m_environmentPtr->m_fogBlue = cdlg.m_blue;
		m_pStatus->InitializeEnvironment(TRUE);
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CEnvironmentSettingsDlg
// Function:	OnCHECKENVAmbientLightOn()
// Description:	Toggle ambient light.
///////////////////////////////////////////////////////////////////////////////

void 
CEnvironmentSettingsDlg::OnCHECKENVAmbientLightOn() 
{
	if(m_pStatus->m_environmentPtr->m_ambientLightOn == TRUE)
	{
		m_pStatus->m_environmentPtr->m_ambientLightOn = FALSE;   
		this->SendDlgItemMessage(IDC_CHECK_ENV_AmbientLightOn,BM_SETCHECK,BST_UNCHECKED,0);
	}
	else
	{
		m_pStatus->m_environmentPtr->m_ambientLightOn = TRUE;
		this->SendDlgItemMessage(IDC_CHECK_ENV_AmbientLightOn,BM_SETCHECK ,BST_CHECKED,0);
	}
	m_pStatus->InitializeEnvironment(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CEnvironmentSettingsDlg
// Function:	OnBTNENVambientDayLight()
// Description:	Present the color dialog to change ambient day light color.
///////////////////////////////////////////////////////////////////////////////

void 
CEnvironmentSettingsDlg::OnBTNENVambientDayLight() 
{
#ifndef _ClientOutput
	CColorDlg cdlg;
	cdlg.m_red = m_pStatus->m_environmentPtr->m_ambientLightRedDay;
	cdlg.m_green = m_pStatus->m_environmentPtr->m_ambientLightGreenDay;
	cdlg.m_blue = m_pStatus->m_environmentPtr->m_ambientLightBlueDay;
	if(cdlg.DoModal() == IDOK)
	{
		m_pStatus->m_environmentPtr->m_ambientLightRedDay = cdlg.m_red;
		m_pStatus->m_environmentPtr->m_ambientLightGreenDay = cdlg.m_green;
		m_pStatus->m_environmentPtr->m_ambientLightBlueDay = cdlg.m_blue;
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CEnvironmentSettingsDlg
// Function:	OnBTNENVambientDayLight()
// Description:	Toggle the time system checkbox control.
///////////////////////////////////////////////////////////////////////////////

void 
CEnvironmentSettingsDlg::OnCHECKENVTimeSystem() 
{
	if(m_pStatus->m_environmentPtr->m_enableTimeSystem == TRUE)
	{
		m_pStatus->m_environmentPtr->m_enableTimeSystem = FALSE;   
		this->SendDlgItemMessage(IDC_CHECK_ENV_TimeSystem,BM_SETCHECK,BST_UNCHECKED,0);
	}
	else
	{
		m_pStatus->m_environmentPtr->m_enableTimeSystem = TRUE;
		this->SendDlgItemMessage(IDC_CHECK_ENV_TimeSystem,BM_SETCHECK ,BST_CHECKED,0);
	}
	m_pStatus->InitializeEnvironment(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CEnvironmentSettingsDlg
// Function:	OnCHECKENVWindRandomizer()
// Description:	Toggle the wind randomizer checkbox control.
///////////////////////////////////////////////////////////////////////////////

void 
CEnvironmentSettingsDlg::OnCHECKENVWindRandomizer() 
{
	if(m_pStatus->m_environmentPtr->m_enableWindRandomizer == TRUE)
	{
		m_pStatus->m_environmentPtr->m_enableWindRandomizer = FALSE;   
		this->SendDlgItemMessage(IDC_CHECK_ENV_WindRandomizer,BM_SETCHECK,BST_UNCHECKED,0);
	}
	else
	{
		m_pStatus->m_environmentPtr->m_enableWindRandomizer = TRUE;
		this->SendDlgItemMessage(IDC_CHECK_ENV_WindRandomizer,BM_SETCHECK ,BST_CHECKED,0);
	}
	m_pStatus->InitializeEnvironment(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CEnvironmentSettingsDlg
// Function:	RefreshEnviorment()
// Description:	Refresh environment dialog's control values.
///////////////////////////////////////////////////////////////////////////////

void
CEnvironmentSettingsDlg::RefreshEnviorment()
{
#ifndef _ClientOutput 

		this->SetDlgItemText(IDC_EDIT_ENV_StartRange,m_pStatus->m_cabLib.FloatToStingCnv(m_pStatus->m_environmentPtr->m_fogStart));
		this->SetDlgItemText(IDC_EDIT_ENV_EndRange,m_pStatus->m_cabLib.FloatToStingCnv(m_pStatus->m_environmentPtr->m_fogRange));
		this->SetDlgItemText(IDC_EDIT_ENV_Density,m_pStatus->m_cabLib.FloatToStingCnv(m_pStatus->m_environmentPtr->m_fogDensity));
		this->SetDlgItemText(IDC_EDIT_ENV_FogRed,m_pStatus->m_cabLib.FloatToStingCnv(m_pStatus->m_environmentPtr->m_fogRed));
		this->SetDlgItemText(IDC_EDIT_ENV_FogGreen,m_pStatus->m_cabLib.FloatToStingCnv(m_pStatus->m_environmentPtr->m_fogGreen));
		this->SetDlgItemText(IDC_EDIT_ENV_FogBlue,m_pStatus->m_cabLib.FloatToStingCnv(m_pStatus->m_environmentPtr->m_fogBlue));
		this->SetDlgItemText(IDC_EDIT_ENV_BckRed,m_pStatus->m_cabLib.FloatToStingCnv(m_pStatus->m_environmentPtr->m_backRed));
		this->SetDlgItemText(IDC_EDIT_ENV_BckGreen,m_pStatus->m_cabLib.FloatToStingCnv(m_pStatus->m_environmentPtr->m_backGreen));
		this->SetDlgItemText(IDC_EDIT_ENV_BckBlue,m_pStatus->m_cabLib.FloatToStingCnv(m_pStatus->m_environmentPtr->m_backBlue));
		this->SetDlgItemText(IDC_EDIT_ENV_sunPosX,m_pStatus->m_cabLib.FloatToStingCnv(m_pStatus->m_environmentPtr->m_sunPosition.x));
		this->SetDlgItemText(IDC_EDIT_ENVsunPosY,m_pStatus->m_cabLib.FloatToStingCnv(m_pStatus->m_environmentPtr->m_sunPosition.y));
		this->SetDlgItemText(IDC_EDIT_ENVsunPosZ,m_pStatus->m_cabLib.FloatToStingCnv(m_pStatus->m_environmentPtr->m_sunPosition.z));
		this->SetDlgItemText(IDC_EDIT_ENV_MaxWindStrength,m_pStatus->m_cabLib.FloatToStingCnv(m_pStatus->m_environmentPtr->m_maxWindStrength));
		this->SetDlgItemText(IDC_EDIT_ENV_WindDirX,m_pStatus->m_cabLib.FloatToStingCnv(m_pStatus->m_environmentPtr->m_windDirectionX));
		this->SetDlgItemText(IDC_EDIT_ENV_WindDirZ,m_pStatus->m_cabLib.FloatToStingCnv(m_pStatus->m_environmentPtr->m_windDirectionZ));
		this->SetDlgItemText(IDC_EDIT_ENV_Gravity,m_pStatus->m_cabLib.FloatToStingCnv(m_pStatus->m_environmentPtr->m_gravity));
		this->SetDlgItemText(IDC_EDIT_ENV_SeaLevel,m_pStatus->m_cabLib.FloatToStingCnv(m_pStatus->m_environmentPtr->m_seaLevel));
		this->SetDlgItemText(IDC_EDIT_ENV_SeaDrag,m_pStatus->m_cabLib.FloatToStingCnv(m_pStatus->m_environmentPtr->m_seaViscusDrag));

		if(m_pStatus->m_environmentPtr->m_ambientLightOn==TRUE)
			this->SendDlgItemMessage(IDC_CHECK_ENV_AmbientLightOn,BM_SETCHECK ,BST_CHECKED,0);
		else
			this->SendDlgItemMessage(IDC_CHECK_ENV_AmbientLightOn,BM_SETCHECK,BST_UNCHECKED,0);

		if(m_pStatus->m_environmentPtr->m_enableTimeSystem==TRUE)
			this->SendDlgItemMessage(IDC_CHECK_ENV_TimeSystem,BM_SETCHECK ,BST_CHECKED,0);
		else
			this->SendDlgItemMessage(IDC_CHECK_ENV_TimeSystem,BM_SETCHECK,BST_UNCHECKED,0);

		if(m_pStatus->m_environmentPtr->m_enableWindRandomizer==TRUE)
			this->SendDlgItemMessage(IDC_CHECK_ENV_WindRandomizer,BM_SETCHECK ,BST_CHECKED,0);
		else
			this->SendDlgItemMessage(IDC_CHECK_ENV_WindRandomizer,BM_SETCHECK,BST_UNCHECKED,0);

		if(m_pStatus->m_environmentPtr->m_sunLightOn==TRUE)
			this->SendDlgItemMessage(IDC_CHECK_ENV_enableSun,BM_SETCHECK ,BST_CHECKED,0);
		else
			this->SendDlgItemMessage(IDC_CHECK_ENV_enableSun,BM_SETCHECK,BST_UNCHECKED,0);

		if(m_pStatus->m_environmentPtr->m_fogOn==TRUE)
			this->SendDlgItemMessage(IDC_CHECK_ENV_FogOn,BM_SETCHECK ,BST_CHECKED,0);
		else
			this->SendDlgItemMessage(IDC_CHECK_ENV_FogOn,BM_SETCHECK,BST_UNCHECKED,0);

#endif
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CEnvironmentSettingsDlg
// Function:	OnBTNENVTimeSystem()
// Description:	Present the Time System dialog.
///////////////////////////////////////////////////////////////////////////////

void 
CEnvironmentSettingsDlg::OnBTNENVTimeSystem() 
{
#ifndef _ClientOutput  
	CTimeSystemDlg dlg;
	dlg.g_pd3dDeviceRef = m_pStatus->m_dxRenderer->m_pd3dDevice;
	dlg.m_enviormentObjRef = m_pStatus->m_environmentPtr;
	dlg.m_textureDataBaseRef = m_pStatus->m_textureDataBase;
	dlg.m_directSoundRef = m_pStatus->m_dirSnd;

	if(!m_pStatus->m_environmentPtr->m_enviormentTimeSystem)
	{
		m_pStatus->m_environmentPtr->m_enviormentTimeSystem = new CTimeSystemObj();
	}

	if(dlg.DoModal() == IDOK)
	{
		if(m_pStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState)
		{
			m_pStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState->SafeDelete();
			delete m_pStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState;
			m_pStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState = 0;
		}

		if(m_pStatus->m_environmentPtr->m_enviormentTimeSystem->m_stormPtr)
		{
			m_pStatus->m_environmentPtr->m_enviormentTimeSystem->m_stormPtr->SafeDelete();
			delete m_pStatus->m_environmentPtr->m_enviormentTimeSystem->m_stormPtr;
			m_pStatus->m_environmentPtr->m_enviormentTimeSystem->m_stormPtr = 0;
		}
		//m_pStatus->m_environmentPtr->StartSystem(GetTickCount(),m_pStatus->m_dirSnd);
		//m_pStatus->m_environmentPtr->InitSystem(m_pStatus->m_dirSnd);
		if(m_pStatus->m_environmentPtr->m_enviormentTimeSystem)
			m_pStatus->m_environmentPtr->m_enviormentTimeSystem->StartSystem(GetTickCount(),0);
	}
#endif
}
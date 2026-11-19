// ParticleInterfaceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ParticleInterfaceDlg.h"
#include "globalstatusvariables.h"
#include "direct.h"

// CParticleInterfaceDlg dialog

IMPLEMENT_DYNAMIC(CParticleInterfaceDlg, CDialog)

CParticleInterfaceDlg::CParticleInterfaceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParticleInterfaceDlg::IDD, pParent)
{
    m_particleSystemRef = 0;
	
	m_lockedKeyFrame = NULL;
}

CParticleInterfaceDlg::~CParticleInterfaceDlg()
{
}

void CParticleInterfaceDlg::DoDataExchange(CDataExchange* pDX)
{
	 CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_reactiveKeyframeVis, m_drawSurface);
	DDX_Control(pDX, IDC_EDIT_particleBaseMeshSize, m_baseSize);
	DDX_Control(pDX, IDC_EDIT_particleCount, m_particleCountEdit);
	DDX_Control(pDX, IDC_COMBO_particleTextureDiv, m_textureDivList);
	DDX_Control(pDX, IDC_EDIT_particleCycleTime, m_particleDurationEdit);
	DDX_Control(pDX, IDC_SLIDER_particleInfluence, m_influenceSlider);
	DDX_Control(pDX, IDC_COMBO_particleBlend, m_blendList);
	DDX_Control(pDX, IDC_COMBO_particleTextureUse, m_textureList);
	DDX_Control(pDX, IDC_SLIDER_particleKeyGreen, m_keyGreen);
	DDX_Control(pDX, IDC_SLIDER_particleKeyBlue, m_keyBlue);
	DDX_Control(pDX, IDC_SLIDER_particleKeyRed, m_keyRed);
	DDX_Control(pDX, IDC_SLIDER_particleSize, m_pSizeSlider);
	DDX_Control(pDX, IDC_SLIDER_particlePosW, m_pPosWidthSlider);
	DDX_Control(pDX, IDC_SLIDER_particlePosH, m_pPosHieghtSlider);
	DDX_Control(pDX, IDC_BUTTON_ParticleDeleteKey, m_deleteKeyBtn);
    DDX_Control(pDX, IDC_LIST_Particle, m_systemsList);
    DDX_Control(pDX, IDC_EDIT_particleSysName, m_sysName);
    DDX_Control(pDX, IDC_EDIT_ParticleSensitivity, m_keySensitivity);
	
}


BEGIN_MESSAGE_MAP(CParticleInterfaceDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CParticleInterfaceDlg::OnBnClickedOk)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_EN_CHANGE(IDC_EDIT_particleBaseMeshSize, &CParticleInterfaceDlg::OnEnChangeEditparticlebasemeshsize)
	ON_EN_CHANGE(IDC_EDIT_particleCount, &CParticleInterfaceDlg::OnEnChangeEditparticlecount)
	ON_CBN_SELCHANGE(IDC_COMBO_particleTextureDiv, &CParticleInterfaceDlg::OnCbnSelchangeComboparticletexturediv)
	ON_EN_CHANGE(IDC_EDIT_particleCycleTime, &CParticleInterfaceDlg::OnEnChangeEditparticlecycletime)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_particleInfluence, &CParticleInterfaceDlg::OnNMReleasedcaptureSliderparticleinfluence)
	ON_CBN_SELCHANGE(IDC_COMBO_particleBlend, &CParticleInterfaceDlg::OnCbnSelchangeComboparticleblend)
	ON_BN_CLICKED(IDC_BUTTON_BrowseTexture, &CParticleInterfaceDlg::OnBnClickedButtonBrowsetexture)
	ON_CBN_SELCHANGE(IDC_COMBO_particleTextureUse, &CParticleInterfaceDlg::OnCbnSelchangeComboparticletextureuse)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_particleKeyBlue, &CParticleInterfaceDlg::OnNMReleasedcaptureSliderparticlekeyblue)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_particleKeyRed, &CParticleInterfaceDlg::OnNMReleasedcaptureSliderparticlekeyred)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_particleKeyGreen, &CParticleInterfaceDlg::OnNMReleasedcaptureSliderparticlekeygreen)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_particleSize, &CParticleInterfaceDlg::OnNMReleasedcaptureSliderparticlesize)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_particlePosW, &CParticleInterfaceDlg::OnNMReleasedcaptureSliderparticleposw)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_particlePosH, &CParticleInterfaceDlg::OnNMReleasedcaptureSliderparticleposh)
	ON_BN_CLICKED(IDC_BUTTON_ParticleDeleteKey, &CParticleInterfaceDlg::OnBnClickedButtonParticledeletekey)
	ON_LBN_SELCHANGE(IDC_LIST_Particle, &CParticleInterfaceDlg::OnSelchangeListParticlesystems)
	ON_EN_CHANGE(IDC_EDIT_particleSysName, &CParticleInterfaceDlg::OnEnChangeEditparticleName)
    ON_EN_CHANGE(IDC_EDIT_ParticleSensitivity, &CParticleInterfaceDlg::OnEnChangeEditparticleSensitivity)
    ON_BN_CLICKED(IDC_BUTTON_pSysNew, &CParticleInterfaceDlg::OnNewParticleSystem)
	ON_BN_CLICKED(IDC_BUTTON_pSysLoadDB, &CParticleInterfaceDlg::OnLoadParticleSystem)
	ON_BN_CLICKED(IDC_BUTTON_pSysSaveDB, &CParticleInterfaceDlg::OnSaveParticleSystem)
	ON_BN_CLICKED(IDC_BUTTON_pSysDelete, &CParticleInterfaceDlg::OnBnClickedButtonpsysdelete)
	ON_BN_CLICKED(IDC_BUTTON_pSysImport, &CParticleInterfaceDlg::OnBnClickedButtonpsysimport)
	ON_BN_CLICKED(IDC_BUTTON_pSysExport, &CParticleInterfaceDlg::OnBnClickedButtonpsysexport)
	ON_BN_CLICKED(IDC_BUTTON_importReplace, &CParticleInterfaceDlg::OnBnClickedButtonimportreplace)
END_MESSAGE_MAP()


// CParticleInterfaceDlg message handlers

void CParticleInterfaceDlg::OnBnClickedOk()
{
	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;
	if(gStatus->m_particleTest)
	{
          delete gStatus->m_particleTest;
          gStatus->m_particleTest = 0;
	}
	OnOK();
}

BOOL CParticleInterfaceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;

	for(POSITION posLoc = gStatus->m_particleDatabase->GetHeadPosition();posLoc != NULL;)
    {
       EnvParticleSystem * stsPtr = (EnvParticleSystem *)gStatus->m_particleDatabase->GetNext(posLoc);
       
	   int indexLoc = 0;
       if(gStatus->m_textureDataBase->TextureSearch(stsPtr->m_textureName,indexLoc,10,gStatus->m_dynamicTextureDataBase,0)==TRUE)
          stsPtr->m_textureIndexDyn = indexLoc;
    }
    gStatus->m_textureDataBase->CompileDirectAccess();

	
	m_influenceSlider.SetRange(0,100);
	m_keyRed.SetRange(0,100);
	m_keyGreen.SetRange(0,100);
	m_keyBlue.SetRange(0,100);

	m_pSizeSlider.SetRange(0,1000);
	m_keySensitivity.SetWindowText("4");
	m_pPosWidthSlider.SetRange(0,2000);
	m_pPosHieghtSlider.SetRange(0,4000);

	m_textureDivList.AddString("1 X 1");
	m_textureDivList.AddString("2 X 2");
	m_textureDivList.AddString("3 X 3");
	m_textureDivList.AddString("4 X 4");
	m_textureDivList.AddString("5 X 5");
	m_textureDivList.AddString("6 X 6");
	m_textureDivList.AddString("7 X 7");
	m_textureDivList.AddString("8 X 8");
	m_textureDivList.AddString("9 X 9");
	m_textureDivList.AddString("10 X 10");

	m_blendList.AddString("Modulate");
	m_blendList.AddString("Alpha");
	m_blendList.AddString("Amplified");
	m_blendList.AddString("Wash");
	m_blendList.AddString("Inverse");

	//in list textures
    ClearSelectionFocus();

	UpdateDialog();
	EvalKeyframeState();
	InListParticleSystems(-1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CParticleInterfaceDlg::EvalKeyframeState()
{
   if(m_lockedKeyFrame)
   {//enable
      m_keyRed.EnableWindow(1);
      m_keyRed.SetPos((int)(100.0f*m_lockedKeyFrame->m_startColorAlpha[0]));

	  m_keyGreen.EnableWindow(1);
      m_keyGreen.SetPos((int)(100.0f*m_lockedKeyFrame->m_startColorAlpha[1]));

	  m_keyBlue.EnableWindow(1);
      m_keyBlue.SetPos((int)(100.0f*m_lockedKeyFrame->m_startColorAlpha[2]));
    
	  m_pSizeSlider.EnableWindow(1);
	  m_pSizeSlider.SetPos((int)(100.0f*m_lockedKeyFrame->m_startPosScale[3]));

	  m_pPosWidthSlider.EnableWindow(1);
	  m_pPosWidthSlider.SetPos((int)(100.0f*m_lockedKeyFrame->m_startPosScale[0]));

	  m_pPosHieghtSlider.EnableWindow(1);
	  m_pPosHieghtSlider.SetPos((int)(100.0f*m_lockedKeyFrame->m_startPosScale[1]));

	  m_deleteKeyBtn.EnableWindow(1);

   }//end enable
   else
   {//disable
      m_keyRed.EnableWindow(0);
	  m_keyGreen.EnableWindow(0);
	  m_keyBlue.EnableWindow(0);

	  m_pSizeSlider.EnableWindow(0);
	  m_pPosWidthSlider.EnableWindow(0);
	  m_pPosHieghtSlider.EnableWindow(0);

	  m_deleteKeyBtn.EnableWindow(0);

   }//end disable

}

void CParticleInterfaceDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

    //dc.MoveTo(0,0);
	//dc.LineTo(120,120);

	
    RenderParticleControl();
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
}

void CParticleInterfaceDlg::RenderParticleControl()
{
	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;
	//define brushes
	CBrush * whiteBrush = new CBrush(RGB(255,255,255));
	//get render window
	
	m_drawSurface.GetClientRect(&m_renderRect);
	
	//get device 
    CDC * uiC = m_drawSurface.GetDC();
	uiC->FillRect(&m_renderRect,whiteBrush);//color background white
	//size inside
	if(!m_particleSystemRef)
		return;
	
	//int offsetX = renderSize.right/20;
	int offsetY = m_renderRect.bottom/20;
	m_renderRect.bottom -= offsetY;
	int curX = m_renderRect.right/2;
	int curY = m_renderRect.bottom;
	uiC->MoveTo(curX,curY);//move to anchor root
	//loop through key frames
	int rectoffset = m_renderRect.right / 50;
	RECT rectTemp;
	int curSub = 0;
	int sub = 0;
	for(POSITION posLoc = m_particleSystemRef->m_keyFrames.GetHeadPosition();posLoc != NULL;)
	{
        HprtKeyFrame * keyFrame = (HprtKeyFrame *)m_particleSystemRef->m_keyFrames.GetNext(posLoc);

		sub = (int)((float)(m_renderRect.bottom-(offsetY)) * keyFrame->m_header[0]);
        curY -= sub - curSub;
		curSub = sub;
        uiC->LineTo(curX,curY);
		rectTemp.left = curX-rectoffset;
		rectTemp.right = curX+rectoffset;
		rectTemp.top = curY+rectoffset;
		rectTemp.bottom = curY-rectoffset;
		if(m_lockedKeyFrame != keyFrame)
		   uiC->Rectangle(rectTemp.left-1,rectTemp.top+1,rectTemp.right+1,rectTemp.bottom-1);
		else
		{
           uiC->Rectangle(rectTemp.left-3,rectTemp.top+3,rectTemp.right+3,rectTemp.bottom-3);
		   uiC->Rectangle(rectTemp.left-1,rectTemp.top+1,rectTemp.right+1,rectTemp.bottom-1);
		}

		CBrush * curBrush = new CBrush(RGB(255.0f*keyFrame->m_startColorAlpha[0],255.0f*keyFrame->m_startColorAlpha[1],255.0f*keyFrame->m_startColorAlpha[2]));
        uiC->FillRect(&rectTemp,curBrush);
		
		delete curBrush;
		curBrush = 0;


		//keyFrame->m_header[0];//time pos
		//keyFrame->m_startPosScale[0];//X pos - width Axis
		//keyFrame->m_startPosScale[0];//Y pos - height pos

	}
	//cleanup
	delete whiteBrush;
	whiteBrush = 0;
}

void CParticleInterfaceDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnMouseMove(nFlags, point);
}

void CParticleInterfaceDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnLButtonUp(nFlags, point);
}

void CParticleInterfaceDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    //localize point
	 RECT rectWin;
	this->ClientToScreen(&point);
	m_drawSurface.ScreenToClient(&point);
	
    m_drawSurface.GetClientRect(&m_renderRect);
	int offsetY = m_renderRect.bottom/20;
	m_renderRect.bottom -= offsetY;
    m_lockedKeyFrame = NULL;//clear selection
	int curX = m_renderRect.right/2;
	int curY = m_renderRect.bottom;
	int rectoffset = m_renderRect.right / 50;
	int curSub = 0;
	int sub = 0;
	if(m_particleSystemRef)
	for(POSITION posLoc = m_particleSystemRef->m_keyFrames.GetHeadPosition();posLoc != NULL;)
	{
        HprtKeyFrame * keyFrame = (HprtKeyFrame *)m_particleSystemRef->m_keyFrames.GetNext(posLoc);

		sub = (int)((float)(m_renderRect.bottom-(offsetY)) * keyFrame->m_header[0]);
        curY -= sub - curSub;
		curSub = sub;

		if(point.x >= curX-rectoffset)
		   if(point.x <= curX+rectoffset)
		      if(point.y >= curY-rectoffset)
		         if(point.y <= curY+rectoffset)
				    m_lockedKeyFrame = keyFrame;
		//collision detection
	}

	RenderParticleControl();
	EvalKeyframeState();

	CDialog::OnLButtonDown(nFlags, point);
}



void CParticleInterfaceDlg::OnEnChangeEditparticlebasemeshsize()
{
     GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;
     CString textTemp;
	 m_baseSize.GetWindowTextA(textTemp);
	 m_particleSystemRef->m_baseMeshSize = atof(textTemp);
	 UpdateParticleDisplay();	 
}
//---------------------------------------------------------------------------------
//refresh dialog vars
void CParticleInterfaceDlg::UpdateDialog()
{ 
       

     GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;
     
     if(!m_particleSystemRef)
		 return;

	 //update textures
	 m_textureList.ResetContent();
	 for(POSITION posLoc = gStatus->m_textureDataBase->GetHeadPosition();posLoc != NULL;)
	 {
         CTextureDBObj * tex = (CTextureDBObj*)gStatus->m_textureDataBase->GetNext(posLoc);
		 m_textureList.AddString(tex->List_textureName);
	 }

     
     m_textureList.SetCurSel(m_particleSystemRef->m_textureIndexDyn);
	 
	 m_particleCountEdit.SetWindowTextA(gStatus->m_cabLib.ConvToString(m_particleSystemRef->m_NumParticles));
	 m_baseSize.SetWindowTextA(gStatus->m_cabLib.FloatToStingCnv(m_particleSystemRef->m_baseMeshSize));
	 m_textureDivList.SetCurSel(m_particleSystemRef->m_textureGrid-1);
	 m_particleDurationEdit.SetWindowTextA(gStatus->m_cabLib.ConvToString(m_particleSystemRef->m_lifetime));
	 m_influenceSlider.SetPos((int)(m_particleSystemRef->m_influence*100.0f));
	 m_blendList.SetCurSel(m_particleSystemRef->m_blendMode);
	 m_sysName.SetWindowTextA(m_particleSystemRef->m_systemName);
}
//---------------------------------------------------------------------------------
//refreshes particle test in engine for visual feedback
void CParticleInterfaceDlg::UpdateParticleDisplay()
{
	 GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;
     if(gStatus->m_particleTest)
	 {
          delete gStatus->m_particleTest;
          gStatus->m_particleTest = 0;
	 }
     if(!m_particleSystemRef)
	     return;

	 m_particleSystemRef->ConstructVSPMesh(m_particleSystemRef->m_baseMeshSize,m_particleSystemRef->m_influence,1.0f,m_particleSystemRef->m_textureGrid);
     m_particleSystemRef->m_vshaderMesh->InitBuffer(gStatus->m_dxRenderer->m_pd3dDevice,NULL);
	 m_particleSystemRef->Clone(&gStatus->m_particleTest,gStatus->m_dxRenderer->m_pd3dDevice);
}
//---------------------------------------------------------------------------------
void CParticleInterfaceDlg::OnEnChangeEditparticlecount()
{
	 GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;
     CString textTemp;
	 m_particleCountEdit.GetWindowTextA(textTemp);
	 m_particleSystemRef->m_NumParticles = atoi(textTemp);
	 if(m_particleSystemRef->m_NumParticles > 10000)
	 {
	    m_particleSystemRef->m_NumParticles = 10000;
		textTemp = "10000";
        m_particleCountEdit.SetWindowTextA(textTemp);
	 }
	 UpdateParticleDisplay();	 
}
//---------------------------------------------------------------------------------
void CParticleInterfaceDlg::OnCbnSelchangeComboparticletexturediv()
{
	 m_particleSystemRef->m_textureGrid = m_textureDivList.GetCurSel()+1;
	 if(m_particleSystemRef->m_textureGrid < 1)
	    m_particleSystemRef->m_textureGrid = 1;
     UpdateParticleDisplay();
}
//---------------------------------------------------------------------------------
void CParticleInterfaceDlg::OnEnChangeEditparticlecycletime()
{
	 GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;
     CString textTemp;
	 m_particleDurationEdit.GetWindowTextA(textTemp);
	 m_particleSystemRef->m_lifetime = atoi(textTemp);
	 if(m_particleSystemRef->m_lifetime < 1)
	    m_particleSystemRef->m_lifetime = 1;
	 UpdateParticleDisplay();	 
}
//---------------------------------------------------------------------------------
void CParticleInterfaceDlg::OnNMReleasedcaptureSliderparticleinfluence(NMHDR *pNMHDR, LRESULT *pResult)
{
	
     m_particleSystemRef->m_influence = (float)m_influenceSlider.GetPos()/100.0f;//((int)(m_particleSystemRef->m_influence*100.0f));
     UpdateParticleDisplay();

	*pResult = 0;
}
//---------------------------------------------------------------------------------
void CParticleInterfaceDlg::OnCbnSelchangeComboparticleblend()
{
	m_particleSystemRef->m_blendMode = m_blendList.GetCurSel();
	 if(m_particleSystemRef->m_blendMode < 1)
	    m_particleSystemRef->m_blendMode = 1;
     UpdateParticleDisplay();
}
//---------------------------------------------------------------------------------
void CParticleInterfaceDlg::OnBnClickedButtonBrowsetexture()
{
	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;
	static char BASED_CODE filter[] = "Texture Files|*.tga;*.bmp;*.png;*.dds;*.jpg;*.jpeg|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   m_particleSystemRef->m_textureName = opendialog.GetFileName();
	   int indexLoc = 0;
	   if(gStatus->m_textureDataBase->TextureSearch(m_particleSystemRef->m_textureName,indexLoc,10,gStatus->m_dynamicTextureDataBase,0)==TRUE)
	   {
		   m_particleSystemRef->m_textureIndexDyn = indexLoc;
		   gStatus->m_textureDataBase->CompileDirectAccess();
		   UpdateDialog();
	   }
	   
	   /*if(!gStatus->AddTextureToDatabase(m_particleSystemRef->m_vshaderMesh,10))
	       AfxMessageBox("Texture failed to load, make sure texture is in MapsModels Directory");
	   else
	   {
	       gStatus->m_textureDataBase->CompileDirectAccess();
           UpdateDialog();
	   }*/

	   UpdateParticleDisplay();
	 }
}

void CParticleInterfaceDlg::OnCbnSelchangeComboparticletextureuse()
{
     int texSelect = m_textureList.GetCurSel();
	 if(texSelect < 0)
	    return;

	 GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;
     POSITION posLoc = gStatus->m_textureDataBase->FindIndex(texSelect);
	 if(!posLoc)
		 return;
     CTextureDBObj * tex = (CTextureDBObj*)gStatus->m_textureDataBase->GetAt(posLoc);
     m_particleSystemRef->m_textureName				= tex->List_textureName;
	 m_particleSystemRef->m_textureIndexDyn			= texSelect;
	  
	 UpdateParticleDisplay();
}

void CParticleInterfaceDlg::OnNMReleasedcaptureSliderparticlekeyblue(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(!m_lockedKeyFrame)
		return;
	*pResult = 0;

	m_lockedKeyFrame->m_startColorAlpha[2] = (float)m_keyBlue.GetPos()*.01f;
    UpdateParticleDisplay();
	RenderParticleControl();
}

void CParticleInterfaceDlg::OnNMReleasedcaptureSliderparticlekeyred(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(!m_lockedKeyFrame)
		return;
	*pResult = 0;

	m_lockedKeyFrame->m_startColorAlpha[0] = (float)m_keyRed.GetPos()*.01f;
    UpdateParticleDisplay();
	RenderParticleControl();
}

void CParticleInterfaceDlg::OnNMReleasedcaptureSliderparticlekeygreen(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(!m_lockedKeyFrame)
		return;
	*pResult = 0;

	m_lockedKeyFrame->m_startColorAlpha[1] = (float)m_keyGreen.GetPos()*.01f;
    UpdateParticleDisplay();
	RenderParticleControl();
}

void CParticleInterfaceDlg::OnNMReleasedcaptureSliderparticlesize(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(!m_lockedKeyFrame)
		return;
	*pResult = 0;

	m_lockedKeyFrame->m_startPosScale[3] = (float)m_pSizeSlider.GetPos()*.01f;
    UpdateParticleDisplay();
	RenderParticleControl();
}

void CParticleInterfaceDlg::OnNMReleasedcaptureSliderparticleposw(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(!m_lockedKeyFrame)
		return;
	*pResult = 0;

	m_lockedKeyFrame->m_startPosScale[0] = (float)m_pPosWidthSlider.GetPos()*.01f;
    UpdateParticleDisplay();
	RenderParticleControl();
}

void CParticleInterfaceDlg::OnNMReleasedcaptureSliderparticleposh(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(!m_lockedKeyFrame)
		return;
	*pResult = 0;

	m_lockedKeyFrame->m_startPosScale[1] = (float)m_pPosHieghtSlider.GetPos()*.01f;
    UpdateParticleDisplay();
	RenderParticleControl();
}

void CParticleInterfaceDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(m_lockedKeyFrame)
	   return;

    this->ClientToScreen(&point);
	m_drawSurface.ScreenToClient(&point);

	m_drawSurface.GetClientRect(&m_renderRect);

	float time = 0.0f;
	time = 1.0f - (float)((float)point.y/(float)m_renderRect.bottom);

	m_particleSystemRef->m_keyFrames.AddKeyFrame(time);

	UpdateParticleDisplay();
	RenderParticleControl();

	CDialog::OnLButtonDblClk(nFlags, point);
}
void CParticleInterfaceDlg::OnBnClickedButtonParticledeletekey()
{
	if(!m_lockedKeyFrame)
	    return;

	POSITION posLast;
	if(m_particleSystemRef)
	for(POSITION posLoc = m_particleSystemRef->m_keyFrames.GetHeadPosition();(posLast = posLoc) != NULL;)
	{
        HprtKeyFrame * keyFrame = (HprtKeyFrame *)m_particleSystemRef->m_keyFrames.GetNext(posLoc);

		if(m_lockedKeyFrame == keyFrame)
		{
           delete keyFrame;
		   keyFrame = 0;
		   m_particleSystemRef->m_keyFrames.RemoveAt(posLast);
           break;
		}
	}

	UpdateParticleDisplay();
	RenderParticleControl();
}

void CParticleInterfaceDlg::InListParticleSystems(int sel)
{
    GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;

    m_systemsList.ResetContent();
    if(gStatus->m_particleDatabase)
	for(POSITION posLoc = gStatus->m_particleDatabase->GetHeadPosition();posLoc != NULL;)
	{
        EnvParticleSystem * stsPtr = (EnvParticleSystem *)gStatus->m_particleDatabase->GetNext(posLoc);

		m_systemsList.AddString(stsPtr->m_systemName);

	}
	if(sel > -1)
	   m_systemsList.SetCurSel(sel);

}

void CParticleInterfaceDlg::OnSelchangeListParticlesystems() 
{
	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;

    if(!gStatus->m_particleDatabase)
		return;

	int sel = m_systemsList.GetCurSel();
	if(sel < 0)
	   return;

    POSITION posLoc = gStatus->m_particleDatabase->FindIndex(sel);
	if(!posLoc)return;
        EnvParticleSystem * stsPtr = (EnvParticleSystem *)gStatus->m_particleDatabase->GetAt(posLoc);
        
		ClearSelectionFocus();

        m_particleSystemRef = stsPtr;

		UpdateDialog();
		UpdateParticleDisplay();
	    RenderParticleControl();

}

void CParticleInterfaceDlg::ClearSelectionFocus()
{
     m_particleSystemRef = NULL;
     m_lockedKeyFrame = NULL;
	 UpdateParticleDisplay();
	 RenderParticleControl();
}

//---------------------------------------------------------------------------------
void CParticleInterfaceDlg::OnEnChangeEditparticleName()
{
	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;
    int sel = m_systemsList.GetCurSel();
	if(sel < 0)
	   return;

    POSITION posLoc = gStatus->m_particleDatabase->FindIndex(sel);
	if(!posLoc)return;
        EnvParticleSystem * stsPtr = (EnvParticleSystem *)gStatus->m_particleDatabase->GetAt(posLoc);

		
	m_sysName.GetWindowTextA(stsPtr->m_systemName);

    InListParticleSystems(sel);
}
//---------------------------------------------------------------------------------
void CParticleInterfaceDlg::OnNewParticleSystem()
{
	   m_particleSystemRef = NULL;
	
	   GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;
	   m_particleSystemRef = new EnvParticleSystem();
	   m_particleSystemRef->m_cycleDuration = 2000;
	   m_particleSystemRef->m_NumParticles = 20;
	   m_particleSystemRef->m_blendMode = 2;
	
	   //add the first two default keys
	   float testTempPosScale[4];
	   float testTempColorAlpha[4];
	   ZeroMemory(testTempPosScale,sizeof(float)*4);
	   testTempColorAlpha[0] = 0.7f;
	   testTempColorAlpha[1] = 0.7f;
	   testTempColorAlpha[2] = 0.7f;
	   testTempColorAlpha[3] = 1.0f;
	   testTempPosScale[3] = 1.0f;
	   
	   m_particleSystemRef->m_keyFrames.AddKeyFrame(0.0f,testTempPosScale, testTempColorAlpha);//start anchor
	   ZeroMemory(testTempColorAlpha,sizeof(float)*4);
	   testTempPosScale[1] = 3.0f;
	   testTempPosScale[3] = 0.0f;
	   
	   m_particleSystemRef->m_keyFrames.AddKeyFrame(1.0f,testTempPosScale, testTempColorAlpha);//start anchor
       m_particleSystemRef->m_keyFrames.AddKeyFrame(.5f);
	   //update test particle
       m_particleSystemRef->m_baseMeshSize = 1.0f;
	   m_particleSystemRef->m_textureGrid = 2;

	   m_particleSystemRef->ConstructVSPMesh(m_particleSystemRef->m_baseMeshSize,m_particleSystemRef->m_influence,1.0f,m_particleSystemRef->m_textureGrid);
	   m_particleSystemRef->m_textureName = "particle_example.tga";
	   
	   int indexLoc = 0;
	    if(gStatus->m_textureDataBase->TextureSearch(m_particleSystemRef->m_textureName,indexLoc,10,gStatus->m_dynamicTextureDataBase,0)==TRUE)
	       m_particleSystemRef->m_textureIndexDyn = indexLoc;
	   
	   
	   gStatus->m_textureDataBase->CompileDirectAccess();//gets added texture working
	   
	   
	   gStatus->m_particleDatabase->AddTail(m_particleSystemRef);
	   InListParticleSystems(gStatus->m_particleDatabase->GetCount()-1);
	   
	   UpdateDialog();
	   UpdateParticleDisplay();
	   RenderParticleControl();
}
//---------------------------------------------------------------------------------
void CParticleInterfaceDlg::OnSaveParticleSystem()
{
	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;
    //use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
     //end use gamefiles folder for organization
     static char BASED_CODE filter[] = "Particle Files|*.PRT|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "PRT", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetPathName();
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
       if(res == FALSE){
		  SetCurrentDirectory(MainDir);
	      return;
	   }
       CArchive the_out_Archive(&fl,CArchive::store);
       the_out_Archive << gStatus->m_particleDatabase;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}
//---------------------------------------------------------------------------------
void CParticleInterfaceDlg::OnLoadParticleSystem()
{
	 GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;
     //use gamefiles folder for organization
     CString CurDir;
     CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization

    static char BASED_CODE filter[] = "Particle Files|*.PRT|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetPathName();
	   SetCurrentDirectory(MainDir);
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	   if(res == FALSE){
		  SetCurrentDirectory(MainDir);
		  return;
	   }
	   CArchive the_in_Archive(&fl,CArchive::load);
	   gStatus->m_particleDatabase->SafeDelete();
	   delete gStatus->m_particleDatabase;
	   gStatus->m_particleDatabase = 0;
	   the_in_Archive >> gStatus->m_particleDatabase;
	   the_in_Archive.Close();
       fl.Close();
	   //reassign textures
	   for(POSITION posLoc = gStatus->m_particleDatabase->GetHeadPosition();posLoc != NULL;)
	   {
           EnvParticleSystem * stsPtr = (EnvParticleSystem *)gStatus->m_particleDatabase->GetNext(posLoc);
           
		   int indexLoc = 0;
	       if(gStatus->m_textureDataBase->TextureSearch(stsPtr->m_textureName,indexLoc,10,gStatus->m_dynamicTextureDataBase,0)==TRUE)
	          stsPtr->m_textureIndexDyn = indexLoc;
	   }
	   gStatus->m_textureDataBase->CompileDirectAccess();
	 }
     SetCurrentDirectory(MainDir);
	 ClearSelectionFocus();
	 InListParticleSystems(-1);
}
void CParticleInterfaceDlg::OnBnClickedButtonpsysdelete()
{
	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;
    int sel = m_systemsList.GetCurSel();
	if(sel < 0)
	   return;

    POSITION posLoc = gStatus->m_particleDatabase->FindIndex(sel);
	if(!posLoc)return;
        EnvParticleSystem * stsPtr = (EnvParticleSystem *)gStatus->m_particleDatabase->GetAt(posLoc);

	delete stsPtr;
	stsPtr = 0;
	gStatus->m_particleDatabase->RemoveAt(posLoc);

	ClearSelectionFocus();


    InListParticleSystems(sel-1);

	UpdateDialog();
	UpdateParticleDisplay();
	RenderParticleControl();
}

void CParticleInterfaceDlg::OnBnClickedButtonpsysimport()
{
	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;
     //use gamefiles folder for organization
     CString CurDir;
     CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization

    static char BASED_CODE filter[] = "Particle Unit Files|*.PUI|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetPathName();
	   SetCurrentDirectory(MainDir);
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	   if(res == FALSE){
		  SetCurrentDirectory(MainDir);
		  return;
	   }
	   CArchive the_in_Archive(&fl,CArchive::load);
	   EnvParticleSystem * stsPtr = NULL;
	   the_in_Archive >> stsPtr;
	   the_in_Archive.Close();
       fl.Close();
	   //reassign textures
	   int indexLoc = 0;
	   if(gStatus->m_textureDataBase->TextureSearch(stsPtr->m_textureName,indexLoc,10,gStatus->m_dynamicTextureDataBase,0)==TRUE)
	      stsPtr->m_textureIndexDyn = indexLoc;
	   gStatus->m_textureDataBase->CompileDirectAccess();

	   gStatus->m_particleDatabase->AddTail(stsPtr);
	   ClearSelectionFocus();
	   m_particleSystemRef = stsPtr;
	 }
     SetCurrentDirectory(MainDir);
	 
	 InListParticleSystems(gStatus->m_particleDatabase->GetCount()-1);
	 UpdateDialog();
	 UpdateParticleDisplay();
	 RenderParticleControl();
}

void CParticleInterfaceDlg::OnBnClickedButtonpsysexport()
{
	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;
	int sel = m_systemsList.GetCurSel();
	if(sel < 0)
	{
	   AfxMessageBox("Must select a particle system");
	   return;

	}

    POSITION posLoc = gStatus->m_particleDatabase->FindIndex(sel);
	if(!posLoc)return;
        EnvParticleSystem * stsPtr = (EnvParticleSystem *)gStatus->m_particleDatabase->GetAt(posLoc);
    //use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
     //end use gamefiles folder for organization
     static char BASED_CODE filter[] = "Particle Unit Files|*.PUI|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "PUI", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetPathName();
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
       if(res == FALSE){
		  SetCurrentDirectory(MainDir);
	      return;
	   }
       CArchive the_out_Archive(&fl,CArchive::store);
       the_out_Archive << stsPtr;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}

void CParticleInterfaceDlg::OnEnChangeEditparticleSensitivity()
{
	 CString temp;
     m_keySensitivity.GetWindowText(temp);
	 int tempRange = atoi(temp) * 1000;
	 m_pPosWidthSlider.SetRange(0,(tempRange / 2));
	 m_pPosHieghtSlider.SetRange(0,tempRange);
	 m_pPosHieghtSlider.SetPos(0);
	 m_pPosWidthSlider.SetPos(0);
	 EvalKeyframeState();
	 /*UpdateDialog();
	 UpdateParticleDisplay();
	 EvalKeyframeState();
	 RenderParticleControl();*/

}

void CParticleInterfaceDlg::OnBnClickedButtonimportreplace()
{

	int sel = m_systemsList.GetCurSel();
	if(sel < 0)
	{
	   AfxMessageBox("Must Select a particle system to replace");
	   return;
	}
	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;
     //use gamefiles folder for organization
     CString CurDir;
     CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization

    static char BASED_CODE filter[] = "Particle Unit Files|*.PUI|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetPathName();
	   SetCurrentDirectory(MainDir);
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	   if(res == FALSE){
		  SetCurrentDirectory(MainDir);
		  return;
	   }
	   CArchive the_in_Archive(&fl,CArchive::load);
	   EnvParticleSystem * stsPtr = NULL;
	   the_in_Archive >> stsPtr;
	   the_in_Archive.Close();
       fl.Close();
	   //reassign textures
	   int trace = 0;
	   POSITION posLast;
	   for(POSITION posALoc = gStatus->m_particleDatabase->GetHeadPosition();(posLast = posALoc) != NULL;)
	   {
		   EnvParticleSystem * sys = (EnvParticleSystem *)gStatus->m_particleDatabase->GetNext(posALoc);
	       if(trace == sel)
		   {
		      gStatus->m_particleDatabase->InsertAfter(posLast,stsPtr);
              delete sys;
			  sys = 0;
              gStatus->m_particleDatabase->RemoveAt(posLast);
			  break;
		   }
		   trace++;
	   }

	   int indexLoc = 0;
	   if(gStatus->m_textureDataBase->TextureSearch(stsPtr->m_textureName,indexLoc,10,gStatus->m_dynamicTextureDataBase,0)==TRUE)
	      stsPtr->m_textureIndexDyn = indexLoc;
	   
	   gStatus->m_textureDataBase->CompileDirectAccess();

	   ClearSelectionFocus();
	   m_particleSystemRef = stsPtr;
	 }
     SetCurrentDirectory(MainDir);
	 
	 InListParticleSystems(sel);
	 UpdateDialog();
	 UpdateParticleDisplay();
	 RenderParticleControl();
}

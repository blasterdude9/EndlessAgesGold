// GlobalAdvancedDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "GlobalAdvancedDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGlobalAdvancedDlg dialog


CGlobalAdvancedDlg::CGlobalAdvancedDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGlobalAdvancedDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGlobalAdvancedDlg)
	m_targetFps = 0;
	m_interpolationGlobal = 0;
	m_resolutionHieght = 0;
	m_resolutionWidth = 0;
	//}}AFX_DATA_INIT
}


void CGlobalAdvancedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGlobalAdvancedDlg)
	DDX_Control(pDX, IDC_CHECK_CalcEntityCollision, m_calcEntityCollision);
	DDX_Control(pDX, IDC_CHECK_WBUFFER, m_wbufferPreffered);
	DDX_Control(pDX, IDC_CHECK_SELECTIONMethodOn, m_mouseSelectonMethod);
	DDX_Text(pDX, IDC_EDIT_TARGET_FPS, m_targetFps);
	DDX_Text(pDX, IDC_EDIT_INTERPOLATIONGLOBAL, m_interpolationGlobal);
	DDX_Text(pDX, IDC_EDIT_ResHieght, m_resolutionHieght);
	DDX_Text(pDX, IDC_EDIT_ResWidth, m_resolutionWidth);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_globalShader, m_globalShaderList);
}


BEGIN_MESSAGE_MAP(CGlobalAdvancedDlg, CDialog)
	//{{AFX_MSG_MAP(CGlobalAdvancedDlg)
	ON_BN_CLICKED(IDC_CHECK_SELECTIONMethodOn, OnCHECKSELECTIONMethodOn)
	ON_BN_CLICKED(IDC_CHECK_WBUFFER, OnCheckWbuffer)
	ON_BN_CLICKED(IDC_CHECK_CalcEntityCollision, OnCHECKCalcEntityCollision)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGlobalAdvancedDlg message handlers
void CGlobalAdvancedDlg::OnCHECKSELECTIONMethodOn() 
{
if(mouseSelectionMethodOn == TRUE)
   mouseSelectionMethodOn = FALSE;
else
   mouseSelectionMethodOn = TRUE;

}

BOOL CGlobalAdvancedDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(mouseSelectionMethodOn == TRUE)
       m_mouseSelectonMethod.SetCheck(1);
    else
       m_mouseSelectonMethod.SetCheck(0);

	if(w_bufferPreffered == TRUE)
       m_wbufferPreffered.SetCheck(1);
    else
       m_wbufferPreffered.SetCheck(0);
	
    if(calcEntityCollision == TRUE)
       m_calcEntityCollision.SetCheck(1);
    else
       m_calcEntityCollision.SetCheck(0);

    m_globalShaderList.AddString("NONE");
	for(POSITION posloc = m_shaderAvailable->GetHeadPosition();posloc != NULL;)
	{
        CShaderObj * shaderPtr = (CShaderObj *) m_shaderAvailable->GetNext(posloc);

        m_globalShaderList.AddString(shaderPtr->m_shaderName);
	}
	if(m_currentWorldShader > -1)
       m_globalShaderList.SetCurSel(m_currentWorldShader+1);
	else
	   m_globalShaderList.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGlobalAdvancedDlg::OnCheckWbuffer() 
{
if(w_bufferPreffered == TRUE)
   w_bufferPreffered = FALSE;
else
   w_bufferPreffered = TRUE;
}

void CGlobalAdvancedDlg::OnCHECKCalcEntityCollision() 
{
if(calcEntityCollision == TRUE)
   calcEntityCollision = FALSE;
else
   calcEntityCollision = TRUE;	
}

void CGlobalAdvancedDlg::OnBnClickedOk()
{
	m_currentWorldShader = m_globalShaderList.GetCurSel()-1;
	OnOK();
}

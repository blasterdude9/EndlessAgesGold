// ColorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "ColorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorDlg dialog


CColorDlg::CColorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorDlg)
	m_red = 0.0f;
	m_blue = 0.0f;
	m_green = 0.0f;
	color255 = FALSE;
	m_alpha = 0.0f;
	//}}AFX_DATA_INIT
}


void CColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorDlg)
	DDX_Control(pDX, IDC_SLIDER_ALPHA, m_sliderAlpha);
	DDX_Control(pDX, IDC_VISUAL, m_visual);
	DDX_Control(pDX, IDC_SLIDER_RED, m_sliderRed);
	DDX_Control(pDX, IDC_SLIDER_GREEN, m_sliderGreen);
	DDX_Control(pDX, IDC_SLIDER_BLUE, m_sliderBlue);
	DDX_Text(pDX, IDC_EDIT_RED, m_red);
	DDX_Text(pDX, IDC_EDIT_BLUE, m_blue);
	DDX_Text(pDX, IDC_EDIT_GREEN, m_green);
	DDX_Text(pDX, IDC_EDIT_ALPHA, m_alpha);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColorDlg, CDialog)
	//{{AFX_MSG_MAP(CColorDlg)
	ON_WM_HSCROLL()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorDlg message handlers

BOOL CColorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    
    if(color255 == TRUE){//if normal color system is in use
	 m_sliderBlue.SetRange(0,255,TRUE);
	 m_sliderRed.SetRange(0,255,TRUE);
	 m_sliderGreen.SetRange(0,255,TRUE);
	 m_sliderAlpha.SetRange(0,255,TRUE);
	 m_sliderRed.SetPos((int)(m_red));
	 m_sliderGreen.SetPos((int)(m_green));
     m_sliderBlue.SetPos((int)(m_blue));
	 m_sliderAlpha.SetPos((int)(m_alpha));
	}//if normal color system is in use
	else{//if d3d color system is to be used
	 m_sliderBlue.SetRange(0,100,TRUE);
	 m_sliderRed.SetRange(0,100,TRUE);
	 m_sliderGreen.SetRange(0,100,TRUE);
	 m_sliderAlpha.SetRange(0,100,TRUE);
	 m_sliderRed.SetPos((int)(m_red * 100.0f));
	 m_sliderGreen.SetPos((int)(m_green * 100.0f));
     m_sliderBlue.SetPos((int)(m_blue * 100.0f));
	 m_sliderAlpha.SetPos((int)(m_alpha * 100.0f));
	}//end if d3d color system is to be used
    //---------get rectangle of visual frame-------//
    m_visual.GetClientRect( &StaticRect ); 
    m_visual.ClientToScreen( &StaticRect );
    ScreenToClient( &StaticRect );
    Left = StaticRect.left;
    Top = StaticRect.top;
	Right = StaticRect.right;    
    Bottom = StaticRect.bottom; 

	Invalidate(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CColorDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	UpdateData(TRUE);
	if(color255 == FALSE){//if d3d color system is in use
	m_red = (float)m_sliderRed.GetPos() * .01f;
    m_green = (float)m_sliderGreen.GetPos() * .01f;
	m_blue = (float)m_sliderBlue.GetPos() * .01f;
	m_alpha = (float)m_sliderAlpha.GetPos() * .01f;
	}
	else{
    m_red = (float)m_sliderRed.GetPos();
    m_green = (float)m_sliderGreen.GetPos();
	m_blue = (float)m_sliderBlue.GetPos();
	m_alpha = (float)m_sliderAlpha.GetPos();
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	UpdateData(FALSE);
	Invalidate(FALSE);
}

void CColorDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	
	
}

void CColorDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
    CBrush* tbrush = new CBrush;
	if(color255 == FALSE)//if d3d color system is in use
	tbrush->CreateSolidBrush(RGB(m_red * 255.0f,m_green * 255.0f,m_blue * 255.0f));
	else//use normal color system
    tbrush->CreateSolidBrush(RGB(m_red,m_green,m_blue));

    dc.SelectObject(tbrush);
	dc.Rectangle( Left,Top,Right,Bottom);
	DeleteObject(tbrush);
	delete tbrush;
}

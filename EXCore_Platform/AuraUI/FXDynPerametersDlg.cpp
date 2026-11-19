// FXDynPerametersDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "d3dx9.h"
#include "FXDynPerametersDlg.h"
#include ".\fxdynperametersdlg.h"


// CFXDynPerametersDlg dialog

IMPLEMENT_DYNAMIC(CFXDynPerametersDlg, CDialog)
CFXDynPerametersDlg::CFXDynPerametersDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFXDynPerametersDlg::IDD, pParent)
{
}

CFXDynPerametersDlg::~CFXDynPerametersDlg()
{
}

void CFXDynPerametersDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFXDynPerametersDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


//overrides
BOOL CFXDynPerametersDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
    /*
	CRect(
    int l,
    int t,
    int r,
    int b 
    ) throw( );
    */
	m_peramControlsDefs = NULL;
	DWORD style = WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON;
	DWORD disabledStyle = WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_DISABLED;
	int vSpace = 2;
	int hSpace = 2;
	int buttonWidth = 200;
	int buttonHieght = 20;
	int curTop = vSpace;
	int curBottom = vSpace + buttonHieght;
	int curLeft = hSpace;
	int curRight = buttonWidth + hSpace;
	perameterCount = 0;
	uIDstart = 50000;
	uIDcurrent = uIDstart;
	int controlTypeTrace = 0;//0=Unknown 1=CButton 2=CEdit 
	//initialize custom fonts
    m_smallArial.CreateFont(
                            12,                        // nHeight
                            0,                         // nWidth
                            0,                         // nEscapement
                            0,                         // nOrientation
                            FW_NORMAL,                 // nWeight
                            FALSE,                     // bItalic
                            FALSE,                     // bUnderline
                            0,                         // cStrikeOut
                            ANSI_CHARSET,              // nCharSet
                            OUT_DEFAULT_PRECIS,        // nOutPrecision
                            CLIP_DEFAULT_PRECIS,       // nClipPrecision
                            DEFAULT_QUALITY,           // nQuality
                            DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
                            "Arial");                 // lpszFacename
	m_mediumArial.CreateFont(
                            15,                        // nHeight
                            0,                         // nWidth
                            0,                         // nEscapement
                            0,                         // nOrientation
                            FW_NORMAL,                 // nWeight
                            FALSE,                     // bItalic
                            FALSE,                     // bUnderline
                            0,                         // cStrikeOut
                            ANSI_CHARSET,              // nCharSet
                            OUT_DEFAULT_PRECIS,        // nOutPrecision
                            CLIP_DEFAULT_PRECIS,       // nClipPrecision
                            DEFAULT_QUALITY,           // nQuality
                            DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
                            "Arial");                 // lpszFacename

	int pCount = GetParameterCount();
    
    m_peramControlsDefs = new _DefArbConrol[pCount];


	//controlCommandRef 1=Browse for name 2=4X1 dialog options 4=3X1 dialog   //note:set value to perameter in IEffect also refresh control
	while(1)
	{
	    D3DXHANDLE floatingHandle = m_pEffectRef->GetParameter(NULL,perameterCount);
		if(floatingHandle)
		{
		   D3DXPARAMETER_DESC localDescStruct;
           if(m_pEffectRef->GetParameterDesc(floatingHandle,&localDescStruct)==S_OK)
		   {
			  float currentfloatData = 0.0f;
			  int currentIntData = 0;
		      int parsedType = DEF_ARBCBUTTON;
			  int parsedCommand = 0;
			  HRESULT getValue = S_OK;
			  D3DXVECTOR4 currentVectorData4;ZeroMemory(currentVectorData4,sizeof(D3DXVECTOR4));
			  CString typeDesc = "";
			  DWORD curStyle = style;
			  int dataType = DEF_ABFLOAT;

              if(localDescStruct.Type == D3DXPT_TEXTURE)
			  {//Identified texture
			     parsedType = DEF_ARBCBUTTON;
			     parsedCommand = 1;
				 dataType = DEF_ABSTRING;
			  }//end identified texture
			  else if(localDescStruct.Type == D3DXPT_FLOAT && localDescStruct.Columns == 1 && localDescStruct.Rows == 1)
			  {//base Float
			     getValue = m_pEffectRef->GetFloat(floatingHandle,&currentfloatData);
                 parsedType = DEF_ARBCEDIT;
			     parsedCommand = 0;
				 dataType = DEF_ABFLOAT;
			  }//end base Float
			  else if(localDescStruct.Type == D3DXPT_FLOAT && localDescStruct.Columns == 4 && localDescStruct.Rows == 1)
			  {//basic 4 float vector types
			     parsedType = DEF_ARBCBUTTON;
				 getValue = m_pEffectRef->GetVector(floatingHandle,&currentVectorData4);
                 typeDesc = " (Float4/Vector/Material)";
				 parsedCommand = 0;
				 dataType = DEF_4FLOATVAL;
			  }//end basic 4 float vector types
			  else if(localDescStruct.Type == D3DXPT_FLOAT && localDescStruct.Columns == 4 && localDescStruct.Rows == 4)
			  {//Matrix Type (generally locked)
			     parsedType = DEF_ARBCBUTTON;
				 typeDesc = " (Float(4X4))";
				 curStyle = disabledStyle;
				 parsedCommand = 0;
				 dataType = DEF_4X4FLOATVAL;
			  }//en Matrix Type (generally locked)
			  else if(localDescStruct.Type == D3DXPT_FLOAT && localDescStruct.Columns == 3 && localDescStruct.Rows == 4)
			  {//Matrix Type (generally locked)
			     typeDesc = " (Float(3X4))";
                 parsedType = DEF_ARBCBUTTON;
				 curStyle = disabledStyle;
				 parsedCommand = 0;
				 dataType = DEF_4X3FLOATVAL;
			  }//en Matrix Type (generally locked)
			  else if(localDescStruct.Type == D3DXPT_FLOAT && localDescStruct.Columns == 3 && localDescStruct.Rows == 1)
			  {//basic 3 float vector types
			     parsedType = DEF_ARBCBUTTON;
				 getValue = m_pEffectRef->GetVector(floatingHandle,&currentVectorData4);
                 typeDesc = " (Float3/Vector/Material)";
				 parsedCommand = 4;
				 dataType = DEF_ABVECTOR;
			  }//end basic 3 float vector types
			  else if(localDescStruct.Type == D3DXPT_INT && localDescStruct.Columns == 1 && localDescStruct.Rows == 1)
			  {//single int
			     parsedType = DEF_ARBCEDIT;
				 getValue = m_pEffectRef->GetInt(floatingHandle,&currentIntData);
				 parsedCommand = 0;
				 dataType = DEF_ABINT;
			  }//end single int
			  else
			  {//unknown data
			     parsedType = DEF_ARBCBUTTON;
                 m_peramControlsDefs[controlTypeTrace].controlType = 0;//CEdit
			     m_peramControlsDefs[controlTypeTrace].controlCommandRef = 0;//no state conditions
				 curStyle = disabledStyle;
			  }//end unknown data
			  

			  AddControl(parsedType,//int type,
							curTop,//int curTop,
							curBottom,//int curBottom, 
							curLeft,//int curLeft, 
							curRight,//int curRight,
							parsedCommand,//int commandRef,
							uIDcurrent,//int uid,
							curStyle,//DWORD style,
							localDescStruct,//D3DXPARAMETER_DESC descFX,
							&m_mediumArial);//CFont * fontRef);
              //set Values

			
			  //shift
			  controlTypeTrace++;
			  curTop += vSpace+buttonHieght;
	          curBottom += vSpace+buttonHieght; 
			  uIDcurrent++;
		   }
		}
		else//end of perameter list
		   break;

		perameterCount++;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CFXDynPerametersDlg::AddControl(int type,
									 int curTop,
									 int curBottom, 
									 int curLeft, 
									 int curRight,
									 int commandRef,
									 int uid,
									 DWORD style,
									 D3DXPARAMETER_DESC descFX,
									 CFont * fontRef)
{
	int arrayIndex = uid-uIDstart;
	switch(type)
	{
          case DEF_ARBNONE:
		       break;
          case DEF_ARBCBUTTON:
		       m_peramControlsDefs[arrayIndex].cButtons.Create(descFX.Name,style, CRect(curLeft,curTop,curRight,curBottom), this, uid);
			   m_peramControlsDefs[arrayIndex].cButtons.SetFont(fontRef,TRUE);
		       break;
		  case DEF_ARBCEDIT:
			   m_peramControlsDefs[arrayIndex].cEdits.Create(style, CRect(curLeft,curTop,curRight,curBottom), this, uid);
			   m_peramControlsDefs[arrayIndex].cEdits.SetFont(fontRef,TRUE);
		       break;
	}
    m_peramControlsDefs[arrayIndex].controlType = type;
    m_peramControlsDefs[arrayIndex].controlCommandRef = commandRef;
	return TRUE;
}

void CFXDynPerametersDlg::SetControlText(int uid,CString text)
{
	int arrayIndex = uid-uIDstart;
	switch(m_peramControlsDefs[arrayIndex].controlType)
	{
          case DEF_ARBNONE:
		       break;
          case DEF_ARBCBUTTON:
		       m_peramControlsDefs[arrayIndex].cButtons.SetWindowText(text);
		       break;
		  case DEF_ARBCEDIT:
			   m_peramControlsDefs[arrayIndex].cEdits.SetWindowText(text);
		       break;
	}
}

void CFXDynPerametersDlg::SetControlText(int uid,float num)
{
	SetControlText(uid,m_cablib.FloatToStingCnv(num));
}

int CFXDynPerametersDlg::GetParameterCount()
{
    int trace = 0;
	while(1)
	{
	    D3DXHANDLE floatingHandle = m_pEffectRef->GetParameter(NULL,trace);
		if(floatingHandle)
		{
		   D3DXPARAMETER_DESC localDescStruct;
           if(m_pEffectRef->GetParameterDesc(floatingHandle,&localDescStruct)==S_OK)
		   {
              trace++;
		   }
		}
		else 
		  break;
	}
	return trace;
}

// CFXDynPerametersDlg message handlers

void CFXDynPerametersDlg::OnBnClickedOk()
{
	
	OnOK();
}


BOOL CFXDynPerametersDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{//Handles Cdialog messages
	int indexCurrent = nID - uIDstart;
    if(indexCurrent > -1)
	{//handle action



	}//end handle action 
	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}//end handles Cdialog messages

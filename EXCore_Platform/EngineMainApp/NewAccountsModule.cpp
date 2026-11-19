// NewAccountsModule.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "NewAccountsModule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewAccountsModule dialog


CNewAccountsModule::CNewAccountsModule(CWnd* pParent /*=NULL*/)
	: CDialog(CNewAccountsModule::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewAccountsModule)
	m_password = _T("");
	m_username = _T("");
	//}}AFX_DATA_INIT
}


void CNewAccountsModule::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewAccountsModule)
	DDX_Control(pDX, IDC_CHECK_Registered, m_registered);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_password);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_username);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewAccountsModule, CDialog)
	//{{AFX_MSG_MAP(CNewAccountsModule)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewAccountsModule message handlers

void CNewAccountsModule::OnOK() 
{
	CDialog::OnOK();
}

void CNewAccountsModule::InListAccounts(int sel)
{ 
	
}


//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----converts a long into string-----------------------------//
CString CNewAccountsModule::ConvToString(int number)
{    
              if(number==0)
				 return "0";
	 
	          int decimal,sign;
              char *buffer;
              CString temp;	 
	          buffer = _fcvt(number,0,&decimal,&sign);
    	      temp = buffer;
			  if(sign != 0)
                 temp = operator+("-",temp);
		
			  return temp;
}

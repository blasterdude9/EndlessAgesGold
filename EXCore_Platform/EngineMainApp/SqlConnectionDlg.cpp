// SqlConnectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SqlConnectionDlg.h"


// CSqlConnectionDlg dialog

IMPLEMENT_DYNAMIC(CSqlConnectionDlg, CDialog)

CSqlConnectionDlg::CSqlConnectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSqlConnectionDlg::IDD, pParent)
	, m_username(_T(""))
	, m_password(_T(""))
	, m_connectionIpListText(_T(""))
{

}

CSqlConnectionDlg::~CSqlConnectionDlg()
{
}

void CSqlConnectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_connectionHistory, m_connectionDropList);
	DDX_Text(pDX, IDC_EDIT_cnUsername, m_username);
	DDX_Text(pDX, IDC_EDIT_cnPassword, m_password);
	DDX_Text(pDX, IDC_EDIT_cnIPAddress, m_database);
	DDX_CBString(pDX, IDC_COMBO_connectionHistory, m_connectionIpListText);
}


BEGIN_MESSAGE_MAP(CSqlConnectionDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSqlConnectionDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_connectionHistory, &CSqlConnectionDlg::OnCbnSelchangeComboconnectionhistory)
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_BTN_RemoveConnection, &CSqlConnectionDlg::OnBnClickedBtnRemoveconnection)
END_MESSAGE_MAP()


// CSqlConnectionDlg message handlers

void CSqlConnectionDlg::OnBnClickedOk()
{
	CAXEncryptionLib * encryptManager = new CAXEncryptionLib("3487583457swordOfcowtools27136782");
	UpdateData(TRUE);
	 m_ipAddress = m_connectionIpListText;
	 BOOL isNewEntry = TRUE;
     for(POSITION posLoc = m_connectionsList.GetHeadPosition();posLoc != NULL;)
	 {
        ACConnectionObject * newPtr = (ACConnectionObject *) m_connectionsList.GetNext(posLoc);
        if(newPtr->m_ipAddress.CompareNoCase(m_connectionIpListText)==0)
		{//found selection
			//overwrite just in case user changed data
			newPtr->m_ipAddress = m_connectionIpListText;
		    newPtr->m_userName = m_username;
	        newPtr->m_pass = m_password;
		    newPtr->m_databaseName = m_database;
		    isNewEntry = FALSE;
		}//end found selection
	 }
	 if(isNewEntry)
	 {
        ACConnectionObject * newPtr = new ACConnectionObject();
		newPtr->m_ipAddress = m_connectionIpListText;
		newPtr->m_userName = m_username;
	    newPtr->m_pass = m_password;
		newPtr->m_databaseName = m_database;
		m_connectionsList.AddTail(newPtr);
	 }
	
	 //write files
	 CFileException exc;
     CFile fl;
	 CString tempWrite;
     
     BOOL res = fl.Open("axcon_sql.cfg",CFile::modeCreate | CFile::modeReadWrite, &exc);
	 CArchive the_out_Archive(&fl,CArchive::store);
	 if(res)
	 {
		 for(POSITION posLoc = m_connectionsList.GetHeadPosition();posLoc != NULL;)
		 {
			ACConnectionObject * newPtr = (ACConnectionObject *) m_connectionsList.GetNext(posLoc);

			//tempWrite = encryptManager->EncryptString64(newPtr->m_ipAddress.GetBuffer());
			the_out_Archive.WriteString(newPtr->m_ipAddress);
			the_out_Archive.WriteString( "\r\n");
			tempWrite = encryptManager->EncryptString64(newPtr->m_userName.GetBuffer());
			the_out_Archive.WriteString(tempWrite);
			the_out_Archive.WriteString( "\r\n");
			tempWrite = encryptManager->EncryptString64(newPtr->m_pass.GetBuffer());
			the_out_Archive.WriteString(tempWrite);
			the_out_Archive.WriteString( "\r\n");
			tempWrite = encryptManager->EncryptString64(newPtr->m_databaseName.GetBuffer());
			the_out_Archive.WriteString(tempWrite);
			the_out_Archive.WriteString( "\r\n");
		 }
		 the_out_Archive.Close();
		 fl.Close();
	 }
	 UpdateData(FALSE);
	 //end write file
	 delete encryptManager;
	 encryptManager = 0;
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL CSqlConnectionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CAXEncryptionLib * encryptManager = new CAXEncryptionLib("3487583457swordOfcowtools27136782");
	CFileException excAuto;
     CString temp,temp1,temp1a,temp2,temp2a,temp3,temp3a,temp4,temp4a,assembly;
	 int lastsel = 0;
	 CFile flAuto;
	 CArchive the_ArchiveAuto(&flAuto,CArchive::load);
     BOOL resAuto = flAuto.Open("axcon_sql.cfg",CFile::modeRead, &excAuto);
	 if(resAuto)
	 {//existing data
	   
		while(the_ArchiveAuto.ReadString(temp1))
		{
			   if(the_ArchiveAuto.ReadString(temp2))
				   if(the_ArchiveAuto.ReadString(temp2a))
					   if(the_ArchiveAuto.ReadString(temp3))
						   if(the_ArchiveAuto.ReadString(temp3a))
						      if(the_ArchiveAuto.ReadString(temp4))
								 if(the_ArchiveAuto.ReadString(temp4a))
								 {
								    ACConnectionObject * newPtr = new ACConnectionObject();
									newPtr->m_ipAddress = temp1;//encryptManager->DecryptString64(assembly.GetBuffer());
									assembly = temp2 + "\r\n" + temp2a;
									newPtr->m_userName = encryptManager->DecryptString64(assembly.GetBuffer());
									assembly = temp3 + "\r\n" + temp3a;
									newPtr->m_pass = encryptManager->DecryptString64(assembly.GetBuffer());
									assembly = temp4 + "\r\n" + temp4a;
									newPtr->m_databaseName = encryptManager->DecryptString64(assembly.GetBuffer());
									m_connectionsList.AddTail(newPtr);
								 }
		}
        if(the_ArchiveAuto.ReadString(temp))
		   lastsel = atoi(temp);

		flAuto.Close();
	 }//existing data
     delete encryptManager;
	 encryptManager = 0;

	 InListConnections(lastsel);
     
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSqlConnectionDlg::InListConnections(int sel)
{
   m_connectionDropList.ResetContent();
   for(POSITION posLoc = m_connectionsList.GetHeadPosition();posLoc != NULL;)
   {
       ACConnectionObject * cnPtr = (ACConnectionObject *)m_connectionsList.GetNext(posLoc);
       m_connectionDropList.AddString(cnPtr->m_ipAddress);

   }
   if(sel > -1)
   {
      m_connectionDropList.SetCurSel(sel);
      UpdateData(TRUE);
      for(POSITION posLoc = m_connectionsList.GetHeadPosition();posLoc != NULL;)
	  {
        ACConnectionObject * newPtr = (ACConnectionObject *) m_connectionsList.GetNext(posLoc);
        if(newPtr->m_ipAddress.CompareNoCase(m_connectionIpListText)==0)
		{//found selection
			m_username = newPtr->m_userName;
            m_password = newPtr->m_pass;
            m_database = newPtr->m_databaseName;
			UpdateData(FALSE);
			return;
		}//end found selection
	  }
	  UpdateData(FALSE);
   }

}

void CSqlConnectionDlg::OnCbnSelchangeComboconnectionhistory()
{
	 UpdateData(TRUE);
	 CString selectedText;
	 m_connectionDropList.GetLBText(m_connectionDropList.GetCurSel(),selectedText);

     for(POSITION posLoc = m_connectionsList.GetHeadPosition();posLoc != NULL;)
	 {
        ACConnectionObject * newPtr = (ACConnectionObject *) m_connectionsList.GetNext(posLoc);
        if(newPtr->m_ipAddress.CompareNoCase(selectedText)==0)
		{//found selection
			m_username = newPtr->m_userName;
            m_password = newPtr->m_pass;
            m_database = newPtr->m_databaseName;
			m_connectionIpListText = newPtr->m_ipAddress;
			UpdateData(FALSE);
			return;
		}//end found selection
	 }
	 UpdateData(FALSE);
}

void CSqlConnectionDlg::OnDestroy()
{
	CDialog::OnDestroy();

	for(POSITION posLoc = m_connectionsList.GetHeadPosition();posLoc != NULL;)
	 {
        ACConnectionObject * newPtr = (ACConnectionObject *) m_connectionsList.GetNext(posLoc);
        delete newPtr;
		newPtr = 0;
	 }
	m_connectionsList.RemoveAll();
}

void CSqlConnectionDlg::OnBnClickedBtnRemoveconnection()
{
    int selection = m_connectionDropList.GetCurSel();
	if(selection < 0)
	{
       AfxMessageBox("Must Select a connection first.",0,0);
       return;
	}



	UpdateData(TRUE);
	 CString selectedText;
	 m_connectionDropList.GetLBText(m_connectionDropList.GetCurSel(),selectedText);

	 POSITION posLast = NULL;
     for(POSITION posLoc = m_connectionsList.GetHeadPosition();(posLast = posLoc) != NULL;)
	 {
        ACConnectionObject * newPtr = (ACConnectionObject *) m_connectionsList.GetNext(posLoc);
        if(newPtr->m_ipAddress.CompareNoCase(selectedText)==0)
		{//found selection
			delete newPtr;
			newPtr = 0;
            m_connectionsList.RemoveAt(posLast);
            m_connectionDropList.DeleteString(selection);
			return;
		}//end found selection
	 }
	 UpdateData(FALSE);
}

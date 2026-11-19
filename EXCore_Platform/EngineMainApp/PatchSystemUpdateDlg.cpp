// PatchSystemUpdateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PatchSystemUpdateDlg.h"
#include "SqlConnectionDlg.h"
#include "dbconnectionsystem.h"
#include "direct.h"


// CPatchSystemUpdateDlg dialog

IMPLEMENT_DYNAMIC(CPatchSystemUpdateDlg, CDialog)

CPatchSystemUpdateDlg::CPatchSystemUpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPatchSystemUpdateDlg::IDD, pParent)
	, m_crcOutput(_T(""))
	, m_filePathOutput(_T(""))
	, m_crccaboutput(_T(""))
{

}

CPatchSystemUpdateDlg::~CPatchSystemUpdateDlg()
{
}

void CPatchSystemUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_CRC, m_crcOutput);
	DDX_Text(pDX, IDC_STATIC_ptcFilePath, m_filePathOutput);
	DDX_Control(pDX, IDC_EDIT_ContentID, m_contentID);
	DDX_Control(pDX, IDC_EDIT_FILENAME, m_fileNameEdit);
	DDX_Control(pDX, IDC_EDIT_CABNAME, m_cabFileEdit);
	DDX_Text(pDX, IDC_STATIC_CRCCAB, m_crccaboutput);
}


BEGIN_MESSAGE_MAP(CPatchSystemUpdateDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_gmcConetntBrowseFile, &CPatchSystemUpdateDlg::OnBnClickedButtongmcconetntbrowsefile)
	ON_BN_CLICKED(IDC_BUTTON_gmcUpdateTable, &CPatchSystemUpdateDlg::OnBnClickedButtongmcupdatetable)
	ON_BN_CLICKED(IDOK, &CPatchSystemUpdateDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_BrowseFileName, &CPatchSystemUpdateDlg::OnBnClickedButtonBrowsefilename)
	ON_BN_CLICKED(IDC_BUTTON_BrowseCabName, &CPatchSystemUpdateDlg::OnBnClickedButtonBrowsecabname)
END_MESSAGE_MAP()


// CPatchSystemUpdateDlg message handlers

BOOL CPatchSystemUpdateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPatchSystemUpdateDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}



void CPatchSystemUpdateDlg::OnBnClickedButtongmcconetntbrowsefile()
{
	 CString CurDir;
     CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 
     //end use gamefiles folder for organization 	 
     static char BASED_CODE filter[] = "*.* Files|*.*|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   m_fileName = opendialog.GetFileName();
	   m_filePath = opendialog.GetPathName();
	   m_crc = m_crcGenerator.getcrc(m_filePath);

	   __int8 buffer[2000];
	   ZeroMemory(buffer,2000);
       //_ultoa_s(m_crc,buffer,10);
	   _i64toa_s(m_crc,buffer,2000,10);
	   m_crcString = buffer;

	   m_crcOutput = m_crcString;
	   m_filePathOutput = m_filePath;
	   UpdateData(FALSE);
	 }
	 SetCurrentDirectory(MainDir);
}

void CPatchSystemUpdateDlg::OnBnClickedButtongmcupdatetable()
{
	CSqlConnectionDlg dlg;
     if(dlg.DoModal() == IDOK)
	 {
        CDBConnectionSystem * dbConnection = new CDBConnectionSystem(dlg.m_ipAddress,
			                                                         dlg.m_database,
																     dlg.m_username,
																     dlg.m_password,
																     NULL);


		if(!dbConnection->m_connectionEstablished)
		{
			delete dbConnection;
		    dbConnection = 0;
			AfxMessageBox("Connection could not be established to the database-check your connection info");
			return;
		}
  
		//check if object exists first, otherwise update will do nothing
        CString checkEntry = "SELECT * FROM GMCONTENT WHERE FILENAME = '"+m_fileName+"'";
		_RecordsetPtr chkEntry = dbConnection->Query(checkEntry);
		if(dbConnection->GetCount(chkEntry) < 1)
	    {//its doesnt exists atm
			dbConnection->Close(chkEntry);
            CString addEntry = "INSERT INTO gmContent (CONTENTTYPE, FILENAME, CABNAME, FILECRC, CABCRC, OFFSETFOLDER, SECUREPATH) Values('$CONTENTTYPE','$FILENAME','$CABNAME','$FILECRC','$CABCRC','$OFFSETFOLDER','$SECUREPATH')";
			addEntry.Replace("$CONTENTTYPE",m_fileName);//this is the defaul here as well
			addEntry.Replace("$FILENAME",m_fileName);
			addEntry.Replace("$OFFSETFOLDER",m_offsetPath);
			addEntry.Replace("$CABNAME",m_cabName);
			addEntry.Replace("$CABCRC",m_cabCrcString);
			addEntry.Replace("$FILECRC",m_crcString);
			addEntry.Replace("$SECUREPATH",m_cabPath);
			
			dbConnection->ExecQuery(addEntry);
		}//end it doesnt exist
		else 
			dbConnection->Close(chkEntry);

		CString contentOption;
		m_contentID.GetWindowTextA(contentOption);
		if(contentOption.GetLength() > 1)
		{
			CString updateContentOpt = "UPDATE gmContent set contentType = '"+contentOption+"' WHERE FILENAME = '"+m_fileName+"'";
			dbConnection->ExecQuery(updateContentOpt);
		}

		CString updateAll = "Update gmContent set FILENAME = '"+m_fileName+"',OFFSETFOLDER = '"+m_offsetPath+"',CABNAME = '"+m_cabName+"',CABCRC='"+m_cabCrcString+"', FILECRC='"+m_crcString+"',SECUREPATH='"+m_cabPath+"' WHERE FILENAME = '"+m_fileName+"'";
        dbConnection->ExecQuery(updateAll);


		//handle possible cabinets
        /*CString updateQryCAB = "UPDATE gmContent set cabCRC = '$CRCOFCAB' WHERE CABNAME = '$CABINET'";
		updateQryCAB.Replace("$CRCOFCAB",m_cabCrcString);
		updateQryCAB.Replace("$CABINET",m_cabName);
        dbConnection->ExecQuery(updateQryCAB);
		//handle possible files
		CString updateQryFILE = "UPDATE gmContent set fileCRC = '$CRCOFFILE' WHERE FILENAME = '$NAMEOFFILE'";
		updateQryFILE.Replace("$CRCOFFILE",m_crcString);
		updateQryFILE.Replace("$NAMEOFFILE",m_fileName);
        dbConnection->ExecQuery(updateQryFILE);*/
		delete dbConnection;
		dbConnection = 0;
 
		AfxMessageBox("Completed",0,0);
	 }
}

void CPatchSystemUpdateDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CPatchSystemUpdateDlg::OnBnClickedButtonBrowsefilename()
{
	 CString CurDir;
     CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 
     //end use gamefiles folder for organization 	 
     static char BASED_CODE filter[] = "*.* Files|*.*|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   m_fileName = opendialog.GetFileName();
	   m_filePath = opendialog.GetPathName();
	   m_crc = m_crcGenerator.getcrc(m_filePath);

	   char * pBuff = m_filePath.GetBuffer();
	   int length = m_filePath.GetLength()-1;
	   m_offsetPath = "";

	   char switch1 = 0;
	   while(length > 0)
	   {
	     if(switch1 == 0)
		 {
	        if(pBuff[length]=='\\')
			{
               m_offsetPath = pBuff[length] + m_offsetPath;
			   switch1 = 1;
			}	
		 }
		 else if(switch1)
		 {
			 if(pBuff[length]=='\\')
                break;
			 m_offsetPath = pBuff[length] + m_offsetPath;
		 }
         length--;
	   }

	   m_crcString.Format("%u",m_crc);

	   m_filePathOutput = m_fileName +" "+m_crcString;

	   m_fileNameEdit.SetWindowTextA(m_fileName);
	   UpdateData(FALSE);
	 }
	 SetCurrentDirectory(MainDir);
}

void CPatchSystemUpdateDlg::OnBnClickedButtonBrowsecabname()
{
	CString CurDir;
     CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 
     //end use gamefiles folder for organization 	 
     static char BASED_CODE filter[] = "*.* Files|*.*|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   m_cabCrcString = "";
	   m_cabName = opendialog.GetFileName();
	   m_cabPath = opendialog.GetPathName();
	   m_cabFileEdit.SetWindowTextA(m_cabName);
	   m_cabCrc = m_crcGenerator.getcrc(m_cabPath);

	   m_cabCrcString.Format("%u",m_cabCrc);

	   m_crccaboutput = m_cabName + " " + m_cabCrcString;

	   UpdateData(FALSE);
	 }
	 SetCurrentDirectory(MainDir);
}

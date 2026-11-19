// CCProcessSysDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CCProcessSys.h"
#include "direct.h"
#import "msado15.dll" \
         no_namespace rename("EOF", "EndOfFile")
#include "CCProcessSysDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCCProcessSysDlg dialog

CCCProcessSysDlg::CCCProcessSysDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCCProcessSysDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCCProcessSysDlg)
	m_status = _T("");
	m_requestDirectory = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCCProcessSysDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCCProcessSysDlg)
	DDX_Text(pDX, IDC_STATIC_status, m_status);
	DDX_Text(pDX, IDC_EDIT_requestDirectory, m_requestDirectory);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCCProcessSysDlg, CDialog)
	//{{AFX_MSG_MAP(CCCProcessSysDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_cycle, OnBUTTONcycle)
	ON_BN_CLICKED(IDC_BUTTON_force, OnBUTTONforce)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_Browse, OnBUTTONBrowse)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCCProcessSysDlg message handlers

BOOL CCCProcessSysDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	InitializeCriticalSection(  &m_criticalSection );

	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCCProcessSysDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCCProcessSysDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCCProcessSysDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCCProcessSysDlg::OnBUTTONcycle() 
{
	SetTimer( 0, 10000, 0 );
	
}

void CCCProcessSysDlg::OnBUTTONforce() 
{
	EnterCriticalSection(  &m_criticalSection  );
   

	ProcessResults();
    ProcessNew();

	LeaveCriticalSection( &m_criticalSection);
}

void CCCProcessSysDlg::OnTimer(UINT nIDEvent) 
{
	EnterCriticalSection(  &m_criticalSection  );

    ProcessResults();
    ProcessNew();

	LeaveCriticalSection( &m_criticalSection);
	CDialog::OnTimer(nIDEvent);
}


BOOL CCCProcessSysDlg::ProcessResults()
{
	UpdateData(TRUE);
     CString resultsFile = operator+(m_requestDirectory,"icver001.ans");
	UpdateData(FALSE);
	UpdateData(TRUE);
     CString compareFile = operator+(m_requestDirectory,"icver001.cmp");
    UpdateData(FALSE);

	


    //Aquire Database
    if(FAILED(CoInitialize(NULL)))
    return FALSE;

	 _RecordsetPtr  pRst;
	 _bstr_t strCnn("Provider=sqloledb;Data Source=YOUR_SQL_SERVER;Initial Catalog=portal;User Id=YOUR_SQL_USER;Password=YOUR_SQL_PASSWORD;");
     pRst.CreateInstance(__uuidof(Recordset));
     BOOL enabled = TRUE;
     try
	 {
		pRst->Open(
            "select * from tblInvoices where status = 'pending payment'",//table
             strCnn,
             adOpenStatic,
             adLockOptimistic,
             adCmdText);
	 }
     catch(_com_error &e)
	 {
	        enabled = FALSE;
            _bstr_t bstrSource(e.Source());
            _bstr_t bstrDescription(e.Description());
			UpdateData(TRUE);	
		      m_status = (LPSTR)bstrDescription;
	        UpdateData(FALSE);
	 }
    //end aquire database
	 if(enabled)
	 {//database aquired successfully
        int bstrRecordCount = pRst->RecordCount;
	    CFileException exc,exc2;
        CFile fl,fl2;

	    CArchive the_Archive(&fl,CArchive::load);
        BOOL res = fl.Open(compareFile,CFile::modeRead, &exc);
	    CArchive the_Archive2(&fl2,CArchive::load);
        BOOL res2 = fl2.Open(resultsFile,CFile::modeRead, &exc2);
  
	    if(res == 0 || res2 == 0)
		{
		    CoUninitialize();
		    return FALSE;
		}

	    int compareArraySz = 0;
	    int processArray = 0;
	    CString * compareStringArray = NULL;
	    CString * processStringArray = NULL;
	 
	    if(res)
		{//read in compare array
	       CString tempBuffer;	   
           while(the_Archive.ReadString(tempBuffer))
		   {
             compareArraySz++;
		   }
	       the_Archive.Close();
	       fl.Close();
	       Sleep(200);
           CFile xFl;
	       CArchive the_ArchiveX(&xFl,CArchive::load);
           res = xFl.Open(compareFile,CFile::modeRead, &exc);
	   
	       if(compareArraySz > 0)
		   {
	          compareStringArray = new CString[compareArraySz];
	          int tracker = 0;
		      while(the_ArchiveX.ReadString(tempBuffer))
			  {
                  compareStringArray[tracker] = tempBuffer;
			      tracker++;
			  }
		   }
	       the_ArchiveX.Close();
	       xFl.Close();
	      //delete
	      DeleteFile(compareFile);
		}//end read in compare array

	 
	    if(res2)
		{
	       CString tempBuffer;
           while(the_Archive2.ReadString(tempBuffer))
		   {
               processArray++;
		   }
	       the_Archive2.Close();
	       fl2.Close();	   
	       Sleep(200);
           CFile xFl;
	       CArchive the_ArchiveX(&xFl,CArchive::load);
           res = xFl.Open(resultsFile,CFile::modeRead, &exc);
	       if(processArray > 0)
		   {
	          processStringArray = new CString[processArray];
	          int tracker = 0;
	          while(the_ArchiveX.ReadString(tempBuffer))
			  {
                 processStringArray[tracker] = tempBuffer;
		         tracker++;
			  }
		   }
	       the_ArchiveX.Close();
	       xFl.Close();
	      //delete
	      DeleteFile(resultsFile);
		}
       if(processArray > 0)
	   {//process results to sql server

		  
          for(int j=0;j<processArray;j++)
		  {
              CString InvoiceID = ExtractInvoiceID(compareStringArray[j]);
              CString result = processStringArray[j].GetAt(1);
			  
			  for(int iCnt = 0;iCnt<bstrRecordCount;iCnt++)
			  {
				_bstr_t temp = pRst->Fields->GetItem("InvoiceID")->Value;
			    CString invID = (LPSTR)temp;
			    if(invID == InvoiceID)
				{ 
			       if(result == "Y")
			       pRst->Fields->GetItem("Status")->Value = (_bstr_t)"Payment Received";
				   else
				   pRst->Fields->GetItem("Status")->Value = (_bstr_t)"Payment Declined";
				   pRst->Fields->GetItem("AuthorizationNumber")->Value = (_bstr_t)processStringArray[j];
				   
				   break;
				}
			    else
		           pRst->MoveNext();
			  }
             pRst->MoveFirst();
		  }
	   }//end process results to sql server

       if(compareStringArray){
         delete [] compareStringArray;
		compareStringArray = 0;
	   }
       if(processStringArray){
        delete [] processStringArray;
		processStringArray = 0;
	   }
	 }//database aquired successfully
	 else{
	   //CoUninitialize();
       return FALSE;
	 }
	
    CoUninitialize();
	return TRUE;
}


CString CCCProcessSysDlg::ExtractInvoiceID(CString txtString)
{
     
	CString temp = txtString.GetAt(0);
	int delTrack = 0;
	while(1)
	{
	  if(temp == '"')
		 delTrack++;

	  if(delTrack == 5){
		 txtString.Delete(0,1);
		 break;
	  }

      txtString.Delete(0,1);
	  temp = txtString.GetAt(0);
	}

    temp = "";

	while(1)
	{
       if(txtString.GetAt(0) == '"')
		  break;

       temp = operator+(temp,txtString.GetAt(0));

       txtString.Delete(0,1);
	}
	
 return temp;
}

void CCCProcessSysDlg::ProcessNew()
{

	//dont over write the files
	CFileException excCheck;
    CFile flCheck;
	CString fCheck = operator+(m_requestDirectory,"icver001.cmp");
    BOOL resCheck = flCheck.Open(fCheck,CFile::modeRead, &excCheck);
	if(resCheck != 0)
	{
	   flCheck.Close();
	   return;//skip for now already stuff there to be processed
	}
    //end dont over write the files
    
	if(FAILED(CoInitialize(NULL)))
    return;

	 _RecordsetPtr  pRst;
	 _bstr_t strCnn("Provider=sqloledb;Data Source=YOUR_SQL_SERVER;Initial Catalog=portal;User Id=YOUR_SQL_USER;Password=YOUR_SQL_PASSWORD;");
     pRst.CreateInstance(__uuidof(Recordset));
     BOOL enabled = TRUE;
     try
	 {
	    //execute connection method
        /*pRst->Open(
             tblInvoices,//table
             strCnn,
             adOpenStatic,
             adLockOptimistic,
             adCmdTable);*/

		pRst->Open(
            "select * from tblInvoices where status = 'pending payment'",//table
             strCnn,
             adOpenStatic,
             adLockOptimistic,
             adCmdText);
	 }
     catch(_com_error &e)
	 {
	        enabled = FALSE;
            _bstr_t bstrSource(e.Source());
            _bstr_t bstrDescription(e.Description());
			UpdateData(TRUE);	
		      m_status = (LPSTR)bstrDescription;
	        UpdateData(FALSE);
	 }
	 if(enabled)
	 {
	   int bstrRecordCount = pRst->RecordCount;
	   UpdateData(TRUE);	
	    m_status = "SQL initialized";
	   UpdateData(FALSE);

       CFileException exc;
       CFile fl;
   	   CArchive the_out_Archive(&fl,CArchive::store);

       UpdateData(TRUE);
       CString requestFile = operator+(m_requestDirectory,"icver001.req");
	   UpdateData(FALSE);

       BOOL res = fl.Open(requestFile,CFile::modeCreate | CFile::modeWrite, &exc);

       for(int j=0;j<bstrRecordCount;j++)
	   {
           CString exactNameOnCard;
		   CString orderNumber;
		   CString cardNumber;
		   CString expDate;
		   CString amount;
		   CString tax;
		   CString adress;
		   CString zip;

		   if(pRst->Fields->GetItem("Name")->ActualSize > 0)
		   {
              _bstr_t bufferSql = pRst->Fields->GetItem("Name")->Value;
		      exactNameOnCard = (LPSTR)bufferSql;
		   }

           if(pRst->Fields->GetItem("InvoiceID")->ActualSize > 0)
		   {
              _bstr_t bufferSql = pRst->Fields->GetItem("InvoiceID")->Value;
		      orderNumber = (LPSTR)bufferSql;
		   }

		   if(pRst->Fields->GetItem("CC")->ActualSize > 0)
		   {
              _bstr_t bufferSql = pRst->Fields->GetItem("CC")->Value;
		      cardNumber = (LPSTR)bufferSql;
		   }
		   
		   if(pRst->Fields->GetItem("CC")->ActualSize > 0)
		   {
              _bstr_t bufferSql = pRst->Fields->GetItem("CC")->Value;
		      cardNumber = (LPSTR)bufferSql;
		   }
		   
		   if(pRst->Fields->GetItem("Expiration")->ActualSize > 0)
		   {
              _bstr_t bufferSql = pRst->Fields->GetItem("Expiration")->Value;
		      expDate = (LPSTR)bufferSql;

			  CString three = expDate.GetAt(0);
			  CString four = expDate.GetAt(1);
			  CString one = expDate.GetAt(3);
			  CString two = expDate.GetAt(4);

			  expDate = operator+(operator+(one,two),operator+(three,four));
		   }

		   if(pRst->Fields->GetItem("Amount")->ActualSize > 0)
		   {
              _bstr_t bufferSql = pRst->Fields->GetItem("Amount")->Value;
		      amount = (LPSTR)bufferSql;
		   }

		   if(pRst->Fields->GetItem("State")->ActualSize > 0)
		   {
		      _bstr_t bufferSql = pRst->Fields->GetItem("State")->Value;
			  CString state = (LPSTR)bufferSql;
			  tax = "0.00";
			  if(state == "FL")
			  {
                 float taxFloat = (atof(amount)*.06f);
		         _bstr_t taxString = taxFloat;
		         tax = (LPSTR)taxString;
			  }
		   }


		   if(pRst->Fields->GetItem("Address1")->ActualSize > 0)
		   {
              _bstr_t bufferSql = pRst->Fields->GetItem("Address1")->Value;
		      adress = (LPSTR)bufferSql;
		   }

		   if(pRst->Fields->GetItem("Zip")->ActualSize > 0)
		   {
              _bstr_t bufferSql = pRst->Fields->GetItem("Zip")->Value;
		      zip = (LPSTR)bufferSql;
		   }
		   
		   CString compiledTxt = AddCommand(exactNameOnCard,
				                 orderNumber,
				                 cardNumber,
				                 expDate,
				                 amount,
				                 tax,
				                 adress,
				                 zip);

		   if(j<bstrRecordCount)
		   pRst->MoveNext();

		   the_out_Archive.WriteString(compiledTxt);
		   the_out_Archive.WriteString("\n");
	   }

	   
	   the_out_Archive.Close();
	   fl.Close();
	   //Copy File
	   UpdateData(TRUE);
       CString compareFile = operator+(m_requestDirectory,"icver001.cmp");
	   UpdateData(FALSE);
	   CopyFile(requestFile, 
                compareFile,   
                FALSE);

	   pRst->Close();
	   CoUninitialize();
	   return;
	 }
  CoUninitialize();
  UpdateData(TRUE);	
	    m_status = "SQL completed";
  UpdateData(FALSE);
}


//"<CMN>Landon Tyler Brown","986","4011802152570758","0310","46.53","32826","1816 Bonneville Dr","2.63","","","","","","","","","","","","",""
CString CCCProcessSysDlg::AddCommand(CString exactNameOnCard,
								  CString orderNumber,
								  CString cardNumber,
								  CString expDate,
								  CString amount,
								  CString tax,
								  CString adress,
								  CString zip)
{

    
    CString build = '"';
	build = operator+(build,"C1");
    build = operator+(build,'"');
	build = operator+(build,",");
	build = operator+(build,'"');
	build = operator+(build,"<CMN>");
    build = operator+(build,exactNameOnCard);
    build = operator+(build,'"');
	build = operator+(build,",");
	build = operator+(build,'"');
	build = operator+(build,orderNumber);
	build = operator+(build,'"');
	build = operator+(build,",");
	build = operator+(build,'"');
	build = operator+(build,cardNumber);
	build = operator+(build,'"');
	build = operator+(build,",");
	build = operator+(build,'"');
	build = operator+(build,expDate);
	build = operator+(build,'"');
	build = operator+(build,",");
	build = operator+(build,'"');
	build = operator+(build,amount);
	build = operator+(build,'"');
	build = operator+(build,",");
	build = operator+(build,'"');
	build = operator+(build,zip);
	build = operator+(build,'"');
	build = operator+(build,",");
	build = operator+(build,'"');
	build = operator+(build,adress);
	build = operator+(build,'"');
	build = operator+(build,",");
	build = operator+(build,'"');
	build = operator+(build,tax);
	build = operator+(build,'"');
	build = operator+(build,",");
	build = operator+(build,'"');
	build = operator+(build,'"');
	build = operator+(build,",");
	build = operator+(build,'"');
	build = operator+(build,'"');
	build = operator+(build,",");
	build = operator+(build,'"');
	build = operator+(build,'"');
	build = operator+(build,",");
	build = operator+(build,'"');
	build = operator+(build,'"');
	build = operator+(build,",");
	build = operator+(build,'"');
	build = operator+(build,'"');
	build = operator+(build,",");
	build = operator+(build,'"');
	build = operator+(build,'"');
	build = operator+(build,",");
	build = operator+(build,'"');
	build = operator+(build,'"');
	build = operator+(build,",");
	build = operator+(build,'"');
	build = operator+(build,'"');
	build = operator+(build,",");
	build = operator+(build,'"');
	build = operator+(build,'"');
	build = operator+(build,",");
	build = operator+(build,'"');
	build = operator+(build,'"');
	build = operator+(build,",");
	build = operator+(build,'"');
	build = operator+(build,'"');
	build = operator+(build,",");
	build = operator+(build,'"');
	build = operator+(build,'"');
	build = operator+(build,",");
	build = operator+(build,'"');
	build = operator+(build,'"');

    return build;
}



void CCCProcessSysDlg::OnBUTTONBrowse() 
{
	static char BASED_CODE filter[] = "*.req|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   m_requestDirectory = opendialog.GetPathName();
	   UpdateData(FALSE);
	 }
}

void CCCProcessSysDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	KillTimer(0);
	
}

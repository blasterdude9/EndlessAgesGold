#include "CTextureDatabaseDlg.h"

///////////////////////////////////////////////////////////////////////////////
// Class:		CTextureDatabaseDlg
// Function:	MACROS
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CTextureDatabaseDlg, CDialog)

BEGIN_MESSAGE_MAP(CTextureDatabaseDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_TXTDB_ADD, OnBtnTxtdbAdd)
	ON_BN_CLICKED(IDC_BTN_TXTDB_DELETE, OnBtnTxtdbDelete)
	ON_BN_CLICKED(IDC_BTN_TXTDB_EDIT, OnBtnTxtdbEdit)
	ON_BN_CLICKED(IDC_BTN_TXTDB_REINIT_ALL, OnBtnTxtdbReinitAll)
	ON_BN_CLICKED(IDC_BTN_TXTDB_Browse, OnBTNTXTDBBrowse)
	ON_BN_CLICKED(IDC_BTN_TXTDB_TextureReport, OnBTNTXTDBTextureReport)
	ON_BN_CLICKED(IDC_BTN_TXTDB_AddUrl, OnBtnTxtdbAddUrl)
	ON_LBN_SELCHANGE(IDC_LIST_TXTDB_Database, OnSelchangeLISTTXTDBDatabase)
	ON_BN_CLICKED(IDC_BTN_TX_SaveTextureBank, OnBTNTXSaveTextureBank)
	ON_BN_CLICKED(IDC_BTN_TX_LoadTextureBank, OnBTNTXLoadTextureBank)
	ON_BN_CLICKED(IDC_BTN_TXB_SetMipLevels, OnBTNTXBSetMipLevels)
	ON_BN_CLICKED(IDC_BTN_TXB_RemoveDuplicates, OnBTNTXBRemoveDuplicates)
	ON_BN_CLICKED(IDC_BUTTON_UpdateAllTextures, OnBtnUpdateAllTextures)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// Class:		CTextureDatabaseDlg
// Function:	Ctor/Dtor
///////////////////////////////////////////////////////////////////////////////

CTextureDatabaseDlg::CTextureDatabaseDlg(CWnd* pParent) 
	: CDialog(CTextureDatabaseDlg::IDD, pParent)
{
	
}

CTextureDatabaseDlg::~CTextureDatabaseDlg()
{
	
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CTextureDatabaseDlg
// Function:	OnInitDialog()
// Description:	Inherited from CDialog.
///////////////////////////////////////////////////////////////////////////////

BOOL
CTextureDatabaseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Texture compression should be ENABLED by default
	this->SendDlgItemMessage(IDC_CHECK_TEX_BltOn, BM_SETCHECK, BST_CHECKED, 0);
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CTextureDatabaseDlg
// Function:	OnOK()
// Description:	Inherited from CDialog.
///////////////////////////////////////////////////////////////////////////////

void
CTextureDatabaseDlg::OnOK()
{
	CDialog::OnOK();
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CTextureDatabaseDlg
// Function:	DoDataExchange(CDataExchange*)
// Description:	Inherited from CDialog. 
//				No control data members at this time.
///////////////////////////////////////////////////////////////////////////////

void
CTextureDatabaseDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CTextureDatabaseDlg::OnBtnTxtdbReinitAll() 
{
    
    if(this->SendDlgItemMessage(IDC_CHECK_TEX_BltOn,BM_GETSTATE ,0,0) == BST_CHECKED)
	{
	   m_pStatus->m_dynamicTextureDataBase->m_compressRef = 1;
	   m_pStatus->m_textureDataBase->m_compressRef = 1;
	   m_pStatus->m_compressTextures = TRUE;
	}
	else
	{
       m_pStatus->m_dynamicTextureDataBase->m_compressRef = 0;
	   m_pStatus->m_textureDataBase->m_compressRef = 0;
	   m_pStatus->m_compressTextures = FALSE;
	}

	m_pStatus->ReinitAllTexturesFromDatabase();
	m_pStatus->InListTextureDB(0, this);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------texture browse-----------------------------------------------------------//
void CTextureDatabaseDlg::OnBTNTXTDBBrowse() 
{
     m_pStatus->ReinitAllTexturesFromDatabaseRemoveUnused();
	 m_pStatus->InListTextureDB(0, this);
	 AfxMessageBox("Completed");
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------texture report-----------------------------------------------------------//
void CTextureDatabaseDlg::OnBTNTXTDBTextureReport() 
{
	// Get Current Location
	CString CurDir;
	char buffer[_MAX_PATH];
	_getcwd(buffer, _MAX_PATH);
	CurDir = buffer;

	// Create Command Buffer and Create TextureReport Directory
	CString command;
	command = "mkdir " + CurDir + "\\TextureReport";
	system(command.GetBuffer());

	// Delete any existing files in TextureReport Directory
	command = "del " + CurDir + "\\TextureReport\\* /Q";
	system(command.GetBuffer());

	// Clear Existing Error Log
	command = "echo Starting Texture Copying... > " + CurDir + "\\textureReportErrors.txt";
	system(command.GetBuffer());

	// Create NewLine Character Reference
	char newline = '\n';

	// Set the fileName and Open File for Writing
	CString fileName = CurDir + "\\textureReport.txt";
	FILE *m_file = fopen(fileName.GetBuffer(), "w");

	// Loop through texture DB
	for(POSITION texPositionLoc = m_pStatus->m_textureDataBase->GetHeadPosition();texPositionLoc != NULL;)
	{
		CTextureDBObj * txtPtrLoc = (CTextureDBObj *)m_pStatus->m_textureDataBase->GetNext(texPositionLoc);

		// Write Texture Name to File
		fwrite(txtPtrLoc->List_textureName, strlen(txtPtrLoc->List_textureName), 1, m_file);
		fwrite(&newline, 1, 1, m_file);

		// Copy File to Clean Directory
		command = "copy \"" + CurDir + "\\MapsModels\\" + txtPtrLoc->List_textureName + "\" \"" + CurDir + "\\TextureReport\\" + txtPtrLoc->List_textureName + "\"";
		system(command.GetBuffer());

		// Verify File Was Written
		command = "IF NOT EXIST \"" + CurDir + "\\TextureReport\\" + txtPtrLoc->List_textureName + "\" echo " + txtPtrLoc->List_textureName + " Not Copied. >> " + CurDir + "\\textureReportErrors.txt";
		system(command.GetBuffer());
	}	

	// Close the File
	fclose(m_file);

	// Tell User the report is generated.
	AfxMessageBox("Report Generated.");
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//

void CTextureDatabaseDlg::OnSelchangeLISTTXTDBDatabase() 
{
		int runtimeIndex = this->SendDlgItemMessage(IDC_LIST_TXTDB_Database,LB_GETCURSEL ,0,0);
	       if(runtimeIndex == -1){
              return;
		   }
	       POSITION posLoc = m_pStatus->m_textureDataBase->FindIndex(runtimeIndex);
	       if(!posLoc)
		       return;
	           CTextureDBObj * ptrLoc = ( CTextureDBObj* ) m_pStatus->m_textureDataBase->GetAt(posLoc);
			   
			   this->SetDlgItemInt(  IDC_BTN_TXB_mipLevels,ptrLoc->m_textureObject->m_dwMipMapCount,TRUE);
               this->SetDlgItemInt(  IDC_EDIT_TXTDB_TextureName,ptrLoc->m_bump,TRUE);
			   this->SetDlgItemInt(  IDC_EDIT_TXTDB_TextureBumpType,ptrLoc->m_bumpType,TRUE);
			   this->SetDlgItemText(  IDC_EDIT_TXTDBPerameter,m_pStatus->m_cabLib.FloatToStingCnv(ptrLoc->m_peramameter1));
               this->SetDlgItemText(  IDC_EDIT_txtCompressedOrd,m_pStatus->m_cabLib.ConvToString((int)ptrLoc->m_textureObject->m_compressedOvrd));
			   if(ptrLoc->m_textureObject->m_usage == 0)
			      this->SendDlgItemMessage(  IDC_CHECK_TXB_BitmapLoadOverride,BM_SETCHECK,0,0);
			   else
			      this->SendDlgItemMessage(  IDC_CHECK_TXB_BitmapLoadOverride,BM_SETCHECK,1,0);
}

void CTextureDatabaseDlg::OnBtnTxtdbAddUrl()
{
    #ifndef _ClientOutput
	//attemp multiselect
    //working
	      int mipLevels = 10;
		  CString loadContruction = "http://www.endlessagesportal.com/images/EndlessTitleRes.jpg";
          char charbuf[256];
	      lstrcpy(charbuf,loadContruction.GetBuffer());
		  CTextureDBObj * newPtr = new CTextureDBObj(charbuf);
          newPtr->m_textureObject = new CTextureEX();
	      if(newPtr->m_textureObject->LoadItFromURL(loadContruction,
													mipLevels,
													m_pStatus->m_dxRenderer->m_pd3dDevice,
													0,
													true,
													m_pStatus->m_advUrlBasic,
													m_pStatus->m_advUrlVending)==TRUE)
		  {//success
             newPtr->m_textureObject->Restore(m_pStatus->m_dxRenderer->m_pd3dDevice,
											  0,
											  newPtr->m_bump,
											  newPtr->m_bumpType,
											  m_pStatus->m_dxRenderer->m_maxTextureSizeSquared,
											  m_pStatus->m_renderStateObject->m_pCaps,
											  m_pStatus->m_compressTextures,
											  m_pStatus->m_advUrlBasic,
										      m_pStatus->m_advUrlVending);   
	         m_pStatus->m_textureDataBase->AddTail(newPtr);
             m_pStatus->InListTextureDB(m_pStatus->m_textureDataBase->GetCount()-1, this);
		  }//end success
	      else{
			  MessageBox("failed to load:"+loadContruction);
			  delete newPtr;
			  newPtr = 0;
		  }
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------add texture--------------------------------------------------------------//
void CTextureDatabaseDlg::OnBtnTxtdbAdd() 
{
#ifndef _ClientOutput
	//attemp multiselect
    //working
    CAddTextureDlg dlg;
	dlg.m_mipLevels = 1;
	dlg.fileCounter = 0;
	if(dlg.DoModal()==IDOK){
	   
       
	   for(int loop=0;loop<dlg.fileCounter;loop++){//load all files
          if(dlg.fileCounter != 1 && loop == 0)
			 loop++;

		  CString loadContruction;
		  
             loadContruction = dlg.FileNames[0]+"\\"+dlg.FileNames[loop];

          char charbuf[256];
	      lstrcpy(charbuf,dlg.FileNames[loop]);
		  CTextureDBObj * newPtr = new CTextureDBObj(charbuf);
          newPtr->m_textureObject = new CTextureEX();
	      if(newPtr->m_textureObject->LoadIt(loadContruction,
			                                 dlg.m_mipLevels,
											 m_pStatus->m_dxRenderer->m_pd3dDevice,
											 FALSE)==TRUE){//success
	         newPtr->m_textureObject->Restore(m_pStatus->m_dxRenderer->m_pd3dDevice,
											  0,
											  newPtr->m_bump,
											  newPtr->m_bumpType,
											  m_pStatus->m_dxRenderer->m_maxTextureSizeSquared,
											  m_pStatus->m_renderStateObject->m_pCaps,
											  m_pStatus->m_compressTextures,
											  m_pStatus->m_advUrlBasic,
										      m_pStatus->m_advUrlVending);
	         m_pStatus->m_textureDataBase->AddTail(newPtr);
             m_pStatus->InListTextureDB(m_pStatus->m_textureDataBase->GetCount()-1, this);
		  }//end success
	      else{
          MessageBox("failed");
		  }
	   }

	   m_pStatus->m_textureDataBase->CompileDirectAccess();
	}
#endif
}

void CTextureDatabaseDlg::OnBtnTxtdbDelete() 
{
	int runtimeIndex = this->SendDlgItemMessage(IDC_LIST_TXTDB_Database,LB_GETCURSEL ,0,0);
	if(runtimeIndex == -1)
	{
		MessageBox("Must select an texture object.");
		return;
	}
	POSITION posLoc = m_pStatus->m_textureDataBase->FindIndex(runtimeIndex);
	if(!posLoc)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;

	CTextureDBObj * ptrLoc = ( CTextureDBObj* ) m_pStatus->m_textureDataBase->GetAt(posLoc);	
	m_pStatus->m_textureDataBase->RemoveAt(posLoc);
	m_pStatus->InListTextureDB(runtimeIndex-1, this);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------Edit Texture-------------------------------------------------------------//
void CTextureDatabaseDlg::OnBtnTxtdbEdit() 
{
#ifndef _ClientOutput
int runtimeIndex = this->SendDlgItemMessage(IDC_LIST_TXTDB_Database,LB_GETCURSEL ,0,0);
	       if(runtimeIndex == -1){
              MessageBox("Must select an texture object.");
		      return;
		   }
	       POSITION posLoc = m_pStatus->m_textureDataBase->FindIndex(runtimeIndex);
	       if(!posLoc)
		       return;
	           CTextureDBObj * ptrLoc = ( CTextureDBObj* ) m_pStatus->m_textureDataBase->GetAt(posLoc);		   

    //working
    CAddTextureDlg dlg;
	dlg.m_mipLevels = 1;
	if(dlg.DoModal()==IDOK){
		if(dlg.fileCounter > 2){
           MessageBox("Error: Must select only 1 texture when replacing.");
		   return;
		}
       
       

	   for(int loop=0;loop<dlg.fileCounter;loop++){//load all files
          if(dlg.fileCounter != 1 && loop == 0)
			 loop++;

		  CString loadContruction;
		  
             loadContruction = dlg.FileNames[0]+"\\"+dlg.FileNames[loop];

          char charbuf[256];
	      lstrcpy(charbuf,dlg.FileNames[loop]);
		  ptrLoc->m_textureObject->SafeDelete();
		  delete ptrLoc->m_textureObject;
		  ptrLoc->m_textureObject=0;
		  lstrcpy(ptrLoc->List_textureName,charbuf);
          ptrLoc->m_textureObject = new CTextureEX();
	      if(ptrLoc->m_textureObject->LoadIt(loadContruction,
			                                 dlg.m_mipLevels,
			                                 m_pStatus->m_dxRenderer->m_pd3dDevice,
											 FALSE)==TRUE){//success
             m_pStatus->InListTextureDB(runtimeIndex, this);
			 m_pStatus->ReinitAllTexturesFromDatabase();
		  }//end success
	      else{
          MessageBox("failed");
		  }
	   }
	}				
				
#endif
}

void CTextureDatabaseDlg::OnBTNTXLoadTextureBank() 
{
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization 	
	static char BASED_CODE filter[] = "TXB Files|*.TXB|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   SetCurrentDirectory(MainDir);
	   m_pStatus->LoadTextureBank(fileName);
	   m_pStatus->InListTextureDB(0, this);
	   if(m_pStatus->m_libraryReferenceDB)
	      m_pStatus->m_libraryReferenceDB->ClearInstances();
	  
	 }
	SetCurrentDirectory(MainDir);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CTextureDatabaseDlg::TextureRebuildWithSearch(int mipLevels)
{
  //use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir,"\\MapsModels\\";
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization  
  for(POSITION texPositionLoc = m_pStatus->m_textureDataBase->GetHeadPosition();texPositionLoc != NULL;)
  {
    CTextureDBObj * txtPtrLoc = (CTextureDBObj *)m_pStatus->m_textureDataBase->GetNext(texPositionLoc);
    CString conv = txtPtrLoc->List_textureName;
	CTextureEX * newPtr = new CTextureEX();
	if(newPtr->LoadIt(conv,
		              mipLevels,
					  m_pStatus->m_dxRenderer->m_pd3dDevice,
					  FALSE)==TRUE){//success
	   newPtr->SafeDelete();
	   delete newPtr;
	   newPtr=0;
	   txtPtrLoc->m_textureObject->SafeDelete();
	   delete txtPtrLoc->m_textureObject;
	   txtPtrLoc->m_textureObject = 0;
	   txtPtrLoc->m_textureObject = new CTextureEX();
	   if(txtPtrLoc->m_textureObject->LoadIt(conv,
		                                     mipLevels,
											 m_pStatus->m_dxRenderer->m_pd3dDevice,
											 FALSE)==TRUE){
	 
	   }
	}//end success
  }
  m_pStatus->ReinitAllTexturesFromDatabase();
  SetCurrentDirectory(MainDir);
}

void CTextureDatabaseDlg::OnBTNTXBSetMipLevels() 
{
	int runtimeIndex = this->SendDlgItemMessage(IDC_LIST_TXTDB_Database,LB_GETCURSEL ,0,0);
	       if(runtimeIndex == -1){
              return;
		   }
	       POSITION posLoc = m_pStatus->m_textureDataBase->FindIndex(runtimeIndex);
	       if(!posLoc)
		       return;
	           CTextureDBObj * ptrLoc = ( CTextureDBObj* ) m_pStatus->m_textureDataBase->GetAt(posLoc);
			   
               ptrLoc->m_textureObject->m_dwMipMapCount		= this->GetDlgItemInt(IDC_BTN_TXB_mipLevels,NULL,TRUE);
			   ptrLoc->m_bump								= this->GetDlgItemInt(IDC_EDIT_TXTDB_TextureName,NULL,TRUE);
			   ptrLoc->m_bumpType							= this->GetDlgItemInt(IDC_EDIT_TXTDB_TextureBumpType,NULL,TRUE);
			   
			   ptrLoc->m_textureObject->m_usage = this->SendDlgItemMessage(IDC_CHECK_TXB_BitmapLoadOverride,BM_GETSTATE,0,0);

               CString Peram1; 
               this->GetDlgItemText( IDC_EDIT_TXTDBPerameter,Peram1);
               ptrLoc->m_peramameter1 = (float)atof((LPCTSTR)Peram1);

			   ptrLoc->m_textureObject->m_compressedOvrd = this->GetDlgItemInt(IDC_EDIT_txtCompressedOrd,NULL,TRUE);

			   BOOL processImage = TRUE;
               /*ptrLoc->m_textureObject->Restore(m_pStatus->m_dxRenderer->m_pd3dDevice,
				                                0,
												ptrLoc->m_bump,
												ptrLoc->m_bumpType,
												m_pStatus->m_dxRenderer->m_maxTextureSizeSquared,
												m_pStatus->m_renderStateObject->m_pCaps,
												m_pStatus->m_compressTextures,
												processImage,
												m_pStatus->m_advUrlBasic,
												m_pStatus->m_advUrlVending);*/
			  if(ptrLoc->m_textureObject->Restore(	m_pStatus->m_dxRenderer->m_pd3dDevice,
													0,
													ptrLoc->m_bump,
													ptrLoc->m_bumpType,
													m_pStatus->m_dxRenderer->m_maxTextureSizeSquared,
													m_pStatus->m_renderStateObject->m_pCaps,
													m_pStatus->m_compressTextures,
													processImage,
													m_pStatus->m_advUrlBasic,
													m_pStatus->m_advUrlVending) != S_OK)
			   {
				   CString build = ptrLoc->List_textureName;
			       AfxMessageBox(build + " could not initialize",0,0);
			   }

			   

               m_pStatus->InListTextureDB(runtimeIndex, this);
}

void CTextureDatabaseDlg::OnBTNTXBRemoveDuplicates() 
{
	if(AfxMessageBox("Are you sure you want to apply these changes to this texture permenantly?",MB_OKCANCEL,0)!=IDOK)
	   return;
	//RemoveDuplicatesThatExistFromFile();
	int runtimeIndex = this->SendDlgItemMessage(IDC_LIST_TXTDB_Database,LB_GETCURSEL ,0,0);
	       if(runtimeIndex == -1){
              return;
		   }
	       POSITION posLoc = m_pStatus->m_textureDataBase->FindIndex(runtimeIndex);
	       if(!posLoc)
		       return;
	           CTextureDBObj * ptrLoc = ( CTextureDBObj* ) m_pStatus->m_textureDataBase->GetAt(posLoc);
			   
               ptrLoc->m_textureObject->m_dwMipMapCount		= this->GetDlgItemInt(IDC_BTN_TXB_mipLevels,NULL,TRUE);
			   ptrLoc->m_bump								= this->GetDlgItemInt(IDC_EDIT_TXTDB_TextureName,NULL,TRUE);
			   ptrLoc->m_bumpType							= this->GetDlgItemInt(IDC_EDIT_TXTDB_TextureBumpType,NULL,TRUE);
			   
			   ptrLoc->m_textureObject->m_usage = this->SendDlgItemMessage(IDC_CHECK_TXB_BitmapLoadOverride,BM_GETSTATE,0,0);

               

               CString Peram1; 
               this->GetDlgItemText( IDC_EDIT_TXTDBPerameter,Peram1);
               ptrLoc->m_peramameter1 = (float)atof((LPCTSTR)Peram1);
			   ptrLoc->m_textureObject->m_compressedOvrd = this->GetDlgItemInt(IDC_EDIT_txtCompressedOrd,NULL,TRUE);
			   
			   BOOL processImage = TRUE;
               if(ptrLoc->m_textureObject->Restore(	m_pStatus->m_dxRenderer->m_pd3dDevice,
													0,
													ptrLoc->m_bump,
													ptrLoc->m_bumpType,
													m_pStatus->m_dxRenderer->m_maxTextureSizeSquared,
													m_pStatus->m_renderStateObject->m_pCaps,
													m_pStatus->m_compressTextures,
													processImage,
													m_pStatus->m_advUrlBasic,
													m_pStatus->m_advUrlVending) != S_OK)
			   {
					CString build = ptrLoc->List_textureName;
			        AfxMessageBox(build + " could not initialize",0,0);
			   }
			   else
			        ptrLoc->m_textureObject->CopyDataLiveDataToBaseData(0);

               m_pStatus->InListTextureDB(runtimeIndex, this);
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CTextureDatabaseDlg::OnBTNTXSaveTextureBank() 
{
 //use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization  
	static char BASED_CODE filter[] = "TXB ENGIN|*.TXB|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "EXG", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   m_pStatus->SaveTextureBank(fileName,0,2048);
	 }
    SetCurrentDirectory(MainDir);
}

void CTextureDatabaseDlg::OnBtnUpdateAllTextures()
{
	//m_gStatus->m_dynamicTextureDataBase->DirectoryUpdateAllTextures();
	this->m_pStatus->m_dynamicTextureDataBase->DirectoryUpdateAllTextures();		
	AfxMessageBox("Completed");
}
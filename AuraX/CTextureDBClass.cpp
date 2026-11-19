#include "stdafx.h"

#include <d3d9.h>
#include <afxtempl.h>
#include "direct.h"

#include "CTextureDBClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CTextureDBObj::CTextureDBObj(char L_textureName[256])
{
	m_textureObject = NULL;
	List_tempIndex=-1;
	List_isInUseInScene = FALSE;
	m_formatOvrd = 0;
	m_bump = 0;
	m_bumpType = 0;
	m_peramameter1 = 0.0f;
	for(int loop=0;loop<256;loop++)
		List_textureName[loop] = L_textureName[loop];
	m_tempVar = 0;
	m_allocatedType = 0;	
}

void CTextureDBObj::SafeDelete()
{
	if(m_textureObject){
       m_textureObject->SafeDelete();
       delete m_textureObject;
	   m_textureObject=0;
	}
}

void CTextureDBObj::Clone(CTextureDBObj **copy)
{
     CTextureDBObj * clone = new CTextureDBObj();

	 CopyMemory(clone->List_textureName,List_textureName,256); 
	 clone->List_isInUseInScene = List_isInUseInScene;
	 clone->m_formatOvrd        = m_formatOvrd;
	 clone->m_bump              = m_bump;
	 clone->m_bumpType          = m_bumpType;
	 clone->m_peramameter1      = m_peramameter1;
     clone->List_tempIndex      = List_tempIndex;
	 clone->m_tempVar           = m_tempVar;
	 clone->m_allocatedType     = m_allocatedType;

	 if(m_textureObject)
	    m_textureObject->Clone(&clone->m_textureObject);

	 *copy = clone;

}
//keeps settings and updates texture
BOOL CTextureDBObj::UpdateTextureFromSearchDir(CString & mapsModelsDir,
											   CString * textureSearchPathRef,
											   int & searchPathCountRef,
											   LPDIRECT3DDEVICE9 d3dDeviceRef,
											   int maxTextureSizeSquareRef,
											   D3DCAPS9 & pCapsRef,
											   int & compressRef,
											   CString & advUrl,
											   CString & advUrlVending)
{
       CTextureEX * updatedTexture = new CTextureEX();
	   CString CurDir = mapsModelsDir;
	   
	   
       CString convString = this->List_textureName;//charbuf;
	   CString final = operator+(CurDir,convString);
       
	   BOOL resultX = updatedTexture->LoadIt(final,this->m_textureObject->m_dwMipMapCount,d3dDeviceRef,FALSE);

	   if(!resultX)
		 if(textureSearchPathRef)
	       for(int xP=0;xP<searchPathCountRef;xP++)
		   {
			   CurDir = textureSearchPathRef[xP];
	           final = operator+(CurDir,convString);
	           resultX = updatedTexture->LoadIt(final,this->m_textureObject->m_dwMipMapCount,d3dDeviceRef,FALSE);
               if(resultX)
			      break;
		   }
	   BOOL processImage = TRUE;
	   if(resultX)//success
		 if(updatedTexture->Restore(d3dDeviceRef,
									0,
									m_bump,
									m_bumpType,
									maxTextureSizeSquareRef,
									pCapsRef,
									compressRef,
									processImage,
									advUrl,
									advUrlVending)==S_OK)
		 {//2
	        updatedTexture->CopyDataLiveDataToBaseData(compressRef);
			if(m_textureObject)
			{
				m_textureObject->SafeDelete();
				delete m_textureObject;
				m_textureObject = 0;
			}
            m_textureObject = updatedTexture;
			return TRUE;
		 }

		 if(updatedTexture)
		 {
		    updatedTexture->SafeDelete();
		    delete updatedTexture;
			updatedTexture = 0;
		 }

	return FALSE;
}

void CTextureDBObjList::Clone(CTextureDBObjList ** copy)
{
     CTextureDBObjList * clone = new CTextureDBObjList();

	 for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
         CTextureDBObj * nPtr = (CTextureDBObj *) GetNext(posLoc);
		 CTextureDBObj * cPtr = NULL;
		 nPtr->Clone(&cPtr);
		 clone->AddTail(cPtr);

	 }

     *copy = clone;
}

void CTextureDBObjList::ExportCompressedTXB(CString outputPath,
											LPDIRECT3DDEVICE9 pd3dDevice,
											int compressed,
											int maxTextureSize,
											int fileCompressType)
{
     if(compressed == 0)
	 {
        CFileException exc;
		CFile fl;
		BOOL res = fl.Open(outputPath,CFile::modeCreate | CFile::modeWrite, &exc);
		if(res == FALSE){
			return;
		}
		CArchive the_out_Archive(&fl,CArchive::store);
		the_out_Archive << this;
		the_out_Archive.Close();
		fl.Close();

        return;
	 }



     CTextureDBObjList * clonedList = NULL;
     this->Clone(&clonedList);
	 clonedList->m_compressRef = compressed;
	 D3DCAPS9           pCaps;
     pd3dDevice->GetDeviceCaps(&pCaps);
     clonedList->RestoreAllTextures( pd3dDevice, 
						             maxTextureSize,
						             pCaps);
	 //refresh base data
	 for(POSITION posLoc = clonedList->GetHeadPosition();posLoc != NULL;)
	 {
         CTextureDBObj * nPtr = (CTextureDBObj *) clonedList->GetNext(posLoc);
		 if(nPtr->m_textureObject)
		    nPtr->m_textureObject->CopyDataLiveDataToBaseData(fileCompressType);
	 }

	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(outputPath,CFile::modeCreate | CFile::modeWrite, &exc);
     if(res == FALSE){
	      return;
	 }
     CArchive the_out_Archive(&fl,CArchive::store);
     the_out_Archive << clonedList;
     the_out_Archive.Close();
     fl.Close();

	 clonedList->SafeDelete();
	 delete clonedList;
	 clonedList = 0;

}
/*void CTextureDBObjList::ExportCompressedTXB(CString outputPath,
											LPDIRECT3DDEVICE9 pd3dDevice,
											int compressed,
											int maxTextureSize,
											int fileCompressType)
{
     if(compressed == 0)
	 {
        CFileException exc;
		CFile fl;
		BOOL res = fl.Open(outputPath,CFile::modeCreate | CFile::modeWrite, &exc);
		if(res == FALSE){
			return;
		}
		CArchive the_out_Archive(&fl,CArchive::store);
		the_out_Archive << this;
		the_out_Archive.Close();
		fl.Close();

        return;
	 }



     CTextureDBObjList * clonedList = NULL;
     this->Clone(&clonedList);
	 clonedList->m_compressRef = compressed;
	 D3DCAPS9           pCaps;
     pd3dDevice->GetDeviceCaps(&pCaps);
     clonedList->RestoreAllTextures( pd3dDevice, 
						             maxTextureSize,
						             pCaps);
	 //refresh base data
	 for(POSITION posLoc = clonedList->GetHeadPosition();posLoc != NULL;)
	 {
         CTextureDBObj * nPtr = (CTextureDBObj *) clonedList->GetNext(posLoc);
		 if(nPtr->m_textureObject)
		    nPtr->m_textureObject->CopyDataLiveDataToBaseData(fileCompressType);
	 }

	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(outputPath,CFile::modeCreate | CFile::modeWrite, &exc);
     if(res == FALSE){
	      return;
	 }
     CArchive the_out_Archive(&fl,CArchive::store);
     the_out_Archive << clonedList;
     the_out_Archive.Close();
     fl.Close();

	 clonedList->SafeDelete();
	 delete clonedList;
	 clonedList = 0;

}*/


void CTextureDBObjList::RestoreAllTextures( LPDIRECT3DDEVICE9 pd3dDevice, 
										   int & maxTextureSize,
										   D3DCAPS9 & pCaps)
{
    HRESULT err = 0;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CTextureDBObj * txObj = (CTextureDBObj *)GetNext(posLoc);	
		err = txObj->m_textureObject->Restore(pd3dDevice,
										0,
										txObj->m_bump,
										txObj->m_bumpType,
										maxTextureSize,
										pCaps,
										m_compressRef,
										GL_advURL,
										GL_advURLVending);
        if(err == 1)
		{
			//AfxMessageBox("Error");
			//AfxMessageBox(txObj->List_textureName);
		}
	}
	
}

void CTextureDBObjList::CacheRawData()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CTextureDBObj * txObj = (CTextureDBObj *)GetNext(posLoc);
        txObj->m_textureObject->CacheRawData();
	}
}

void CTextureDBObjList::InvalidateAllTextures( )
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CTextureDBObj * txObj = (CTextureDBObj *)GetNext(posLoc);
        txObj->m_textureObject->Invalidate();
	}
}

CString CTextureDBObjList::GetNameByIndex(int index)
{
	CString temp = "NA";
    if(index < 0)
	   return temp;


	POSITION posLoc = FindIndex(index);
	if(!posLoc)
		return temp;
    CTextureDBObj * txObj = (CTextureDBObj *)GetAt(posLoc);
	temp = txObj->List_textureName;
    return temp;
}


CTextureDBObjList::CTextureDBObjList()
{
	directAccess = NULL;

	GL_pathMapModelsRef        = "";
    d3dDeviceRef               = NULL;
    m_maxTextureSizeSquareRef  = 2048;
    m_editorModeOn             = FALSE;
    m_textureSearchPathRef     = NULL;
    m_searchPathCountRef       = 0;
	m_compressRef              = 1;
}

void CTextureDBObjList::InitReferences(CString & mapsModelsPath,
								       LPDIRECT3DDEVICE9 dedDeviceRef,
									   int & maxTextureSizeSquareRef,
									   BOOL & editorModeOn,
									   CString * textureSearchPath,
									   int & textureSearchPathCount,
									   int & compressIt,
									   CString & advUrl,
									   CString & advUrlVending
									   )
{
	GL_advURL                  = advUrl;
	GL_advURLVending           = advUrlVending;
	GL_pathMapModelsRef        = mapsModelsPath;
    d3dDeviceRef               = dedDeviceRef;
    m_maxTextureSizeSquareRef  = maxTextureSizeSquareRef;
	d3dDeviceRef->GetDeviceCaps(&m_pCapsRef);
    m_editorModeOn             = editorModeOn;
    m_textureSearchPathRef     = textureSearchPath;
    m_searchPathCountRef       = textureSearchPathCount;
	m_compressRef              = compressIt;
}

void CTextureDBObjList::CompileDirectAccess()
{
    int textureCount = (int)GetCount();
	if(textureCount == 0)
	   return;

	if(directAccess)
	{
       delete [] directAccess;
       directAccess = 0;
	}
    directAccess = new DirectAccessStruct[textureCount];
	int tracer = 0;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        directAccess[tracer].object = (CTextureDBObj *)GetNext(posLoc);
		tracer++;  
	}
	directAccessObjectCount = tracer;
}

CTextureDBObjList::~CTextureDBObjList()
{
	 for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
			CTextureDBObj * txObj = (CTextureDBObj *)GetNext(posLoc);
			txObj->SafeDelete();
			delete txObj;
			txObj=0;
	 }
     RemoveAll();

	 if(directAccess)
	 {
			delete [] directAccess;
			directAccess = 0;
			directAccessObjectCount = 0;
	 }
}


void CTextureDBObjList::SafeDelete()
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CTextureDBObj * txObj = (CTextureDBObj *)GetNext(posLoc);
        txObj->SafeDelete();
		delete txObj;
		txObj=0;
	 }
     RemoveAll();

	 if(directAccess)
	 {
       delete [] directAccess;
       directAccess = 0;
	   directAccessObjectCount = 0;
	 }
}

void CTextureDBObjList::FlushAllByAllocID(int alID)
{
	POSITION posLast = NULL;
	for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
        CTextureDBObj * txObj = (CTextureDBObj *)GetNext(posLoc);
        if(alID == txObj->m_allocatedType)
		{
		//txObj->SafeDelete();
		//delete txObj;
		//txObj=0;
		RemoveAt(posLast);
		}
	 }
}

void CTextureDBObjList::DeleteAllByAllocID(int alID)
{
	POSITION posLast = NULL;
	for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
        CTextureDBObj * txObj = (CTextureDBObj *)GetNext(posLoc);
        if(alID == txObj->m_allocatedType)
		{
		txObj->SafeDelete();
		delete txObj;
		txObj=0;
		RemoveAt(posLast);
		}
	 }
}

void CTextureDBObjList::RemoveAllTempTexturesByChannel(int tempSurfaceID)
{
	POSITION posLast = NULL;
     for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
        CTextureDBObj * txObj = (CTextureDBObj *)GetNext(posLoc);
        if(txObj->m_textureObject)
           if(txObj->m_textureObject->m_tempSurface == tempSurfaceID)
		   {
              txObj->SafeDelete();
			  delete txObj;
			  txObj = 0;
              
			  RemoveAt(posLast);
		   }
	 }
     CompileDirectAccess();
}

void CTextureDBObjList::RemoveAllByTempVal()
{
	POSITION posLast = NULL;
     for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
        CTextureDBObj * txObj = (CTextureDBObj *)GetNext(posLoc);
           if(txObj->m_tempVar == 0)
		   {
		      txObj->SafeDelete();
			  delete txObj;
			  txObj = 0;
			  RemoveAt(posLast);
		   }
	 }
     CompileDirectAccess();
}
void CTextureDBObjList::RemoveAllTempTextures()
{
	POSITION posLast = NULL;

	for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;)
	{
		CTextureDBObj *txObj = (CTextureDBObj*)GetNext(posLoc);
		if(txObj->m_textureObject->m_tempSurface > 0)
		{
			txObj->SafeDelete();
			delete txObj;
			txObj = 0;
			RemoveAt(posLast);
		}
	}
	CompileDirectAccess();
}

int CTextureDBObjList::AddAssignTempTexture(LPDIRECT3DDEVICE9 pd3dDevice,
											CString &textureName,
											int tempChannel)
{

	int trace = 0;
	if(!textureName || textureName == "NONE" || textureName.GetLength() == 0)
		return -1;
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {//see if already loaded
        CTextureDBObj * txObj = (CTextureDBObj *)GetNext(posLoc);
		   if(txObj->m_textureObject)
           if(textureName.CompareNoCase(txObj->List_textureName)==0)
		   {
		      if(txObj->m_textureObject->m_tempSurface == 0 || txObj->m_textureObject->m_tempSurface == tempChannel)
              return trace;
		   }

	    trace++;
	 }//end see if already loaded

	 char charbuf[256];
     lstrcpy(charbuf, textureName);
	 CTextureDBObj * txObj = new CTextureDBObj(charbuf);
     txObj->m_textureObject = new CTextureEX();

	 //txObj->m_textureObject->m_dwMipMapCount = 1;//FIX
	 CString final = GL_pathMapModelsRef + textureName;
	 bool url = false;
	   if(strncmp(charbuf,"http",3)==0 || strncmp(charbuf,"www.",3)==0)
	      url = true;
	 BOOL resultLoad = FALSE;
		   
	   if(url == false)
	   {
	   BOOL compressionOvrd = 0;
	   resultLoad = txObj->m_textureObject->LoadIt(final,
													1,
													pd3dDevice,
													tempChannel);		
	   if(tempChannel == 0)
	   if(resultLoad)
           if(txObj->m_textureObject->Restore(pd3dDevice,
											   0,
											   txObj->m_bump,
											   txObj->m_bumpType,
											   m_maxTextureSizeSquareRef,
											   m_pCapsRef,
											   compressionOvrd,
											   GL_advURL,
											   GL_advURLVending)!=S_OK)
		   {
              resultLoad = FALSE;						 
		   }
	   }
	   else
	   {
	       resultLoad = txObj->m_textureObject->LoadItFromURL( charbuf,
															    1,
															    pd3dDevice,
															    tempChannel,
															    false,
																GL_advURL,
																GL_advURLVending);
	   }

     if(resultLoad)
	 {//success
        AddTail(txObj);
	    CompileDirectAccess();
		return (int)(GetCount()-1);
	 }
	 else
	 {
        txObj->SafeDelete();
		delete txObj;
		txObj = 0;
	 }

     return -1;
}







void CTextureDBObj::Serialize( CArchive& ar )
{
int reserved = 0;

CObject::Serialize( ar );
	if(ar.IsStoring() ){
       ar.SetObjectSchema(2);
       ASSERT(ar.GetObjectSchema() == 2);

	   for(int loop=0;loop<256;loop++)
	   ar << List_textureName[loop];

	   ar << m_formatOvrd;

	   ar << m_textureObject;

	   ar << m_bump;
	   ar << m_bumpType;
	   ar << m_peramameter1;
	   ar << reserved;

	}
	else
	{
       int version = ar.GetObjectSchema();

	   for(int loop=0;loop<256;loop++)
       ar >> List_textureName[loop];

	   ar >> m_formatOvrd;
	   
       ar >> m_textureObject;//new var

	   m_bump=0;
       if(version > 1)
	   {
	     ar >> m_bump;
	     ar >> m_bumpType;
	     ar >> m_peramameter1;
	     ar >> reserved;
	   }
	   
	   List_isInUseInScene = FALSE;
	   List_tempIndex = -1;

	   m_tempVar = 0;
	   m_allocatedType = 0;
	}	
}
//---------------------------------------------------------------------//
void CTextureDBObjList::DirectoryUpdateAllTextures()
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
           CTextureDBObj * txObj = (CTextureDBObj *)GetNext(posLoc);
           BOOL result = txObj->UpdateTextureFromSearchDir( GL_pathMapModelsRef,
															m_textureSearchPathRef,
															m_searchPathCountRef,
															d3dDeviceRef,
															m_maxTextureSizeSquareRef,
															m_pCapsRef,
															m_compressRef,
															GL_advURL,
															GL_advURLVending);
	 }
}
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
BOOL CTextureDBObjList::TextureSearch(CString textureName, 
									  int & index,
									  int maxMipLevels,
									  CTextureDBObjList * m_dynamicTextureDataBase,
									  int tempChannel
									  )
{
char charbuf[256];

CString temp = textureName;
int chCount = temp.GetLength();
if(chCount == 0)
return FALSE;

lstrcpy(charbuf, textureName);
//make sure it doesnt already exist
int indexTraceLoc = 0;
for(POSITION posLoc = this->GetHeadPosition();posLoc != NULL;)
{
    CTextureDBObj * ptrLoc = ( CTextureDBObj* ) this->GetNext(posLoc);
    int result = strncmp(charbuf,ptrLoc->List_textureName,256);
	
    if(result==0){//exists
	   ptrLoc->m_tempVar = 1;
	   index = indexTraceLoc;
	   return TRUE;//dont add
	   }
indexTraceLoc++;
}
//if(!m_editorModeOn)
    //return FALSE;
//end make sure it doesnt already exist
       CString cmp = charbuf;
       if(cmp == "NONE" || cmp == "none" )
		  return FALSE;


	   bool url = false;
	   if(strncmp(charbuf,"http",3)==0 || strncmp(charbuf,"www.",3)==0)
	      url = true;

       CTextureDBObj * newPtr = new CTextureDBObj(charbuf);
	   newPtr->m_tempVar = 1;
       newPtr->m_textureObject = new CTextureEX();
	   CString CurDir = GL_pathMapModelsRef;
	   
       CString convString = charbuf;
	   CString final = operator+(CurDir,convString);
	   BOOL resultLoad = FALSE;
		   
	   if(url == false)
	   {
	   resultLoad = newPtr->m_textureObject->LoadIt(final,
													maxMipLevels,
													d3dDeviceRef,
													tempChannel);
	       if(resultLoad)
           if(newPtr->m_textureObject->Restore(d3dDeviceRef,
												0,
												newPtr->m_bump,
												newPtr->m_bumpType,
												m_maxTextureSizeSquareRef,
												m_pCapsRef,
												m_compressRef,
												GL_advURL,
												GL_advURLVending)!=S_OK)
		   {
              resultLoad = FALSE;						 
		   }
	   }
	   else
	   {
	       resultLoad = newPtr->m_textureObject->LoadItFromURL( charbuf,
															    maxMipLevels,
															    d3dDeviceRef,
															    tempChannel,
															    false,
																GL_advURL,
																GL_advURLVending);
	   }
	   
	   if(resultLoad){//success
	      this->AddTail(newPtr);
		  if(m_dynamicTextureDataBase)
		     m_dynamicTextureDataBase->AddTail(newPtr);
	      index = indexTraceLoc;
		  
       }//end success
	   else{
         newPtr->m_textureObject->SafeDelete();
		 delete newPtr->m_textureObject;
		 newPtr->m_textureObject=0;
		 delete newPtr;
		 newPtr=0;
	    index = -1;
	   }
	   //InListTextureDB(-1);
return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------add texture--------------------------------------------------------------//
BOOL CTextureDBObjList::AddTextureToDatabaseTemp(CString * storedTextures,
											     int * assignmentArray,
											     int tempChannel)
{
   for(int textureNum=0;textureNum<7;textureNum++)
   {//
       
   

       assignmentArray[textureNum] = AddAssignTempTexture(d3dDeviceRef,
											              storedTextures[textureNum],
														  tempChannel);
   }
return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------add texture--------------------------------------------------------------//
void CTextureDBObjList::UpdateLiveDataToBaseData()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
         CTextureDBObj * nPtr = (CTextureDBObj *) GetNext(posLoc);
		 if(nPtr->m_textureObject)
		    nPtr->m_textureObject->CopyDataLiveDataToBaseData(m_compressRef);
	 }
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------add texture--------------------------------------------------------------//
BOOL CTextureDBObjList::AddTextureToDatabase(CString * storedTextures,
											 int * assignmentArray,
											 CTextureDBObjList * m_dynamicTextureDataBase
											 )
{
for(int textureNum=0;textureNum<7;textureNum++){//
char charbuf[256];

CString temp = storedTextures[textureNum];

int chCount = temp.GetLength();
if(chCount == 0)
continue;

lstrcpy(charbuf, storedTextures[textureNum]);
//make sure it doesnt already exist
int indexTraceLoc = 0;
BOOL continueIt = FALSE;
for(POSITION posLoc = this->GetHeadPosition();posLoc != NULL;)
{
    CTextureDBObj * ptrLoc = ( CTextureDBObj* ) this->GetNext(posLoc);
    int result = strncmp(charbuf,ptrLoc->List_textureName,256);
    if(result==0){//exists
	   ptrLoc->m_tempVar = 1;
	   assignmentArray[textureNum] = indexTraceLoc;
	   continueIt = TRUE;
	   break;//dont add
	   }
indexTraceLoc++;
}
if(!m_editorModeOn)//CHANGE
   continue;

if(continueIt == TRUE)
   continue;

//end make sure it doesnt already exist
       CString cmp = charbuf;
       if(cmp == "NONE" || cmp == "none" )
		  return FALSE;
       CTextureDBObj * newPtr = new CTextureDBObj(charbuf);
       newPtr->m_textureObject = new CTextureEX();
	   CString CurDir = GL_pathMapModelsRef;
	   
       CString convString = charbuf;
	   CString final = operator+(CurDir,convString);

	   BOOL resultX = newPtr->m_textureObject->LoadIt(final,10,d3dDeviceRef,FALSE);

	   if(!resultX)
		 if(m_textureSearchPathRef)
	       for(int xP=0;xP<m_searchPathCountRef;xP++)
		   {
			   CurDir = m_textureSearchPathRef[xP];
	           final = operator+(CurDir,convString);
	           resultX = newPtr->m_textureObject->LoadIt(final,10,d3dDeviceRef,FALSE);
               if(resultX)
			      break;
		   }
	   
	   if(resultX){//success
		 if(newPtr->m_textureObject->Restore(d3dDeviceRef,
											 0,
											 newPtr->m_bump,
											 newPtr->m_bumpType,
											 m_maxTextureSizeSquareRef,
											 m_pCapsRef,
											 m_compressRef,
											 GL_advURL,
											 GL_advURLVending)==S_OK){//2
		    newPtr->m_tempVar = 1;
	        this->AddTail(newPtr);
			m_dynamicTextureDataBase->AddTail(newPtr);
	        storedTextures[textureNum] = convString;
	        assignmentArray[textureNum] = indexTraceLoc;
		 }//end 2
		 else{
	     //GL_systemReadout->AddLine(operator+("Error loading::",final),g_pd3dDevice,m_symbolMapDB,m_speechMode);
		 //MessageBox(operator+("Error loading::",final));
		 newPtr->m_textureObject->SafeDelete();
		 delete newPtr->m_textureObject;
		 newPtr->m_textureObject=0;
		 delete newPtr;
		 newPtr=0;
		 }
       }//end success
	   else{
         newPtr->m_textureObject->SafeDelete();
		 delete newPtr->m_textureObject;
		 newPtr->m_textureObject=0;
		 delete newPtr;
		 newPtr=0;
	     assignmentArray[textureNum] = -1;
	    //meshObj->List_storedTextureFromFile[textureNum] = "none";  
	   }
	   //InListTextureDB(TextureDataBase->GetCount()-1);
}
return TRUE;
}


IMPLEMENT_SERIAL(CTextureDBObj, CObject, VERSIONABLE_SCHEMA|2)
IMPLEMENT_SERIAL(CTextureDBObjList, CObList, 0)


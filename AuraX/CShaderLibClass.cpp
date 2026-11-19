#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <d3dx9.h>
#include <afxtempl.h>

#include "CShaderLibClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CShaderObj::CShaderObj()
{
    m_shaderName;
    m_shaderCode;
	m_shader = NULL;
	m_pShader = NULL;
	m_pEffect = NULL;
	m_shaderValidHandle = NULL;
	m_shaderType = 0;//shader type 0=vertex 1=pixel
	m_compiler = 0;
	m_effectPeramCount = 0;
	m_shaderUsedOnce = 0;
}

void CShaderObj::SafeDelete()
{
    m_shaderUsedOnce = 0;

    if(m_shader)
	{
       m_shader->Release();
	   m_shader = 0;
	}

	if(m_pShader)
	{
       m_pShader->Release();
	   m_pShader = 0;
	}

	if(m_pEffect)
	{//effect delete
       //delete possible dynamic procedurals
       int trace = 0;
	   while(1)
	   {//peram loop
	     D3DXHANDLE floatingHandle = m_pEffect->GetParameter(NULL,trace);
		 if(!floatingHandle)
		 break;
		 trace++;
		   
		 D3DXPARAMETER_DESC localDescStruct;
         if(m_pEffect->GetParameterDesc(floatingHandle,&localDescStruct)!=S_OK)
		 continue;
			  
         if(localDescStruct.Type != D3DXPT_TEXTURE)
		 continue;
		   
		 if(localDescStruct.Annotations < 1)
	     continue;
		     
		 int Depth = 0;
		 D3DXPARAMETER_DESC AnnotDesc;
		 LPCSTR textureType,pstrFunction,pstrTarget,pstrName;
		 textureType = pstrFunction = pstrTarget = pstrName = NULL;
		 for( UINT iAnnot = 0; iAnnot < localDescStruct.Annotations; iAnnot++ )
         {
                  D3DXHANDLE hAnnot = m_pEffect->GetAnnotation ( floatingHandle, iAnnot );
                  if(m_pEffect->GetParameterDesc( hAnnot, &AnnotDesc )==S_OK)
				  {
                     if( _strcmpi( AnnotDesc.Name, "name" ) == 0 )
                         m_pEffect->GetString( hAnnot, &pstrName );
                     else if ( _strcmpi( AnnotDesc.Name, "function" ) == 0 )
                         m_pEffect->GetString( hAnnot, &pstrFunction );
                     else if ( _strcmpi( AnnotDesc.Name, "depth" ) == 0 )
                         m_pEffect->GetInt( hAnnot, &Depth );
				  }       	 
		 }
	     if( Depth > 0 )
		 {//3D texture detected
		     if(pstrName == NULL && pstrFunction != NULL)
			 {//delete
			   LPDIRECT3DBASETEXTURE9 ppTexture = NULL;
               HRESULT errHr = m_pEffect->GetTexture(floatingHandle,&ppTexture);
               if(errHr == S_OK)
			   if(ppTexture)
			   {
                  m_pEffect->SetTexture(floatingHandle,NULL);
				  ppTexture->Release();
				  ppTexture = 0;
			   }
			 }//end delete
		 }//end 3D texture detected	 		  		  
	   }//end delete possible dynamic procedurals
       m_pEffect->Release();
	   m_pEffect = 0;
	}//end effect delete
}

BOOL CShaderObj::Compile(LPDIRECT3DDEVICE9 pd3dDevice,CString * error)
{
    if(m_shader)
	{
       m_shader->Release();
	   m_shader = 0;
	}

	if(m_pShader)
	{
       m_pShader->Release();
	   m_pShader = 0;
	}

	if(m_pEffect)
	{
       m_pEffect->Release();
	   m_pEffect = 0;
	}

    HRESULT hr;
 
	int bufSize = m_shaderCode.GetLength();
	
	if(m_compiler == 0)
	{//compile low level
	LPD3DXBUFFER pCode=NULL;
	LPD3DXBUFFER errorCodes;
    hr = D3DXAssembleShader((LPCTSTR) m_shaderCode,
                                      bufSize,//UINT SrcDataLen,
                                      NULL,//CONST D3DXMACRO* pDefines,
                                      NULL,//LPD3DXINCLUDE pInclude,
                                      0,//DWORD Flags,
                                      &pCode,//LPD3DXBUFFER* ppShader,
                                      &errorCodes);

	if(pCode)
	{
	   if(m_shaderType == 0)
	   {
	      hr = pd3dDevice->CreateVertexShader( (DWORD*)pCode->GetBufferPointer(), &m_shader );
	   }
	   else if(m_shaderType == 1)
	   {
	      hr = pd3dDevice->CreatePixelShader( (DWORD*)pCode->GetBufferPointer(), &m_pShader );
	   }
   
	   if(errorCodes)
	   if(errorCodes->GetBufferSize() > 0)
	   {
	    char * newMsg = new char[errorCodes->GetBufferSize()];
		CopyMemory(newMsg,errorCodes->GetBufferPointer(),errorCodes->GetBufferSize());
		if(error)
		   *error = newMsg;
		delete [] newMsg;
		newMsg = 0;
	   }



       pCode->Release();
	   pCode = 0;
	}
	else
	{
        //get error codes
	  if(errorCodes)
	  if(errorCodes->GetBufferSize() > 0)
	  {
	    char * newMsg = new char[errorCodes->GetBufferSize()];
		CopyMemory(newMsg,errorCodes->GetBufferPointer(),errorCodes->GetBufferSize());
		if(error)
		   *error = newMsg;
		delete [] newMsg;
		newMsg = 0;
	  }
	    return FALSE;
	}
	}//end compile low level
	else if(m_compiler == 1)
	{//compile fx style
 
	   LPD3DXBUFFER parseErrorsBuffer;
       DWORD dwFlags = D3DXSHADER_DEBUG;

	   HRESULT complErr = D3DXCreateEffect( pd3dDevice, 
		                      m_shaderCode, 
							  bufSize, 
							  NULL, 
                              NULL,//&includeManager, 
							  dwFlags, 
							  NULL, 
							  &m_pEffect, 
							  &parseErrorsBuffer );


       if(complErr == 0)
	   {
			 m_effectPeramCount = 0;
	         while(1)
	         {
	             D3DXHANDLE floatingHandle = m_pEffect->GetParameter(NULL,m_effectPeramCount);
		         if(floatingHandle)
		         {
		            D3DXPARAMETER_DESC localDescStruct;
                    if(m_pEffect->GetParameterDesc(floatingHandle,&localDescStruct)==S_OK)
		            {
                       m_effectPeramCount++;
		            }
		         }
		         else 
		           break;
	         }
	   }
	   else
	      return FALSE;
	}//end compile fx style

	
	return TRUE;
}

void CShaderObjList::Compile(LPDIRECT3DDEVICE9 pd3dDevice)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CShaderObj * spnPtr = (CShaderObj *)GetNext(posLoc);
		spnPtr->Compile(pd3dDevice,NULL);
	}
}

void CShaderObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CShaderObj * spnPtr = (CShaderObj *)GetNext(posLoc);
		spnPtr->SafeDelete();
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}

void CShaderObjList::AddShader(CString shaderName, CString code, int shaderType)
{
     CShaderObj * shader = new CShaderObj();
	 shader->m_shaderCode = code;
     shader->m_shaderName = shaderName;
	 shader->m_shaderType = shaderType;
     AddTail(shader);
}

BOOL CShaderObjList::DeleteByIndex(int index)
{
     if(index < 0)
	    return FALSE;

     POSITION posLoc = FindIndex(index);
     if(!posLoc)
	     return FALSE;

     CShaderObj * spnPtr = (CShaderObj *)GetAt(posLoc);
     delete spnPtr;
	 spnPtr = 0;
	 RemoveAt(posLoc);
	 return TRUE;
}

CShaderObj* CShaderObjList::GetByIndex(int index)
{
     if(index < 0)
	    return NULL;

     POSITION posLoc = FindIndex(index);
     if(!posLoc)
	     return NULL;

     CShaderObj * spnPtr = (CShaderObj *)GetAt(posLoc);
	 return spnPtr;
}

CShaderObj* CShaderObjList::GetByName(CString shaderName,int & index)
{

	 index=0;
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
         CShaderObj * spnPtr = (CShaderObj *)GetNext(posLoc);
         if(spnPtr->m_shaderName.CompareNoCase(shaderName)==0)
		    return spnPtr;

		 index++;
	 }
	 return NULL;
}

BOOL CShaderObjList::SetShaderByIndex(LPDIRECT3DDEVICE9 pd3dDevice,
									  int index,
									  int mode,
									  LPD3DXEFFECT * effectRet,
									  int & correctPeramCount,
									  CString * retCode,
									  CShaderObj ** retPtr)
{

     
	    

     if(index < 0)
	 {
	       if(mode==0)
	          pd3dDevice->SetVertexShader(NULL);
		   else if(mode==1)
		   {
		      pd3dDevice->SetPixelShader(NULL);
			  *effectRet = NULL;
		   }
        return FALSE;
	 }

     POSITION posLoc = FindIndex(index);
     if(!posLoc)
	 {
	       if(mode==0)
	          pd3dDevice->SetVertexShader(NULL);
		   else if(mode==1)
		   {
		      pd3dDevice->SetPixelShader(NULL);
			  *effectRet = NULL;
		   }
         
		 return FALSE;
	 }

     CShaderObj * spnPtr = (CShaderObj *)GetAt(posLoc);
	 if(!spnPtr)
		 return FALSE;

	 if(retPtr)
	    *retPtr = spnPtr;

	 if(retCode)
	    *retCode = spnPtr->m_shaderCode;

	 if(spnPtr->m_compiler == 1)
	 if(spnPtr->m_pEffect)
	 {//found complete effect set it
		if(!spnPtr->m_shaderValidHandle)
		   if(spnPtr->m_pEffect->FindNextValidTechnique(NULL,&spnPtr->m_shaderValidHandle)==S_OK)
		      if(spnPtr->m_pEffect->SetTechnique(spnPtr->m_shaderValidHandle)==S_OK)
		      {//initialized
                 //set fail flag
                 

		      }//end initialized
	   
		if(spnPtr->m_shaderValidHandle)
		{
           *effectRet = spnPtr->m_pEffect;
		   correctPeramCount = spnPtr->m_effectPeramCount;
	       return TRUE;
		}
		else
		   return FALSE;
	 }//end found complete effect set it

     if(spnPtr->m_shader)
	    if( pd3dDevice->SetVertexShader(spnPtr->m_shader)!=0)
		{
			spnPtr->m_shaderType = 0;
		    return FALSE;
		}

     if(spnPtr->m_pShader)
	    if( pd3dDevice->SetPixelShader(spnPtr->m_pShader)!=0)
		{
			spnPtr->m_shaderType = 1;
		    return FALSE;
		}

	 return TRUE;
}

int CShaderObjList::LoadShaderByStringData(CString dataStr,LPDIRECT3DDEVICE9 pd3dDevice, CString & fxNameRet)
{//let the parse begin
   
   CString shaderName = "";
   CString shaderCode = "";
   BOOL fxType = TRUE;
   int dataLength = dataStr.GetLength();
   if(dataLength < 2)
      return -1;

   for(int loop=0;loop<dataLength;loop++)
   {
	   char curC = dataStr.GetAt(0);
       
 
       if(curC == '*')
	   {
          dataStr.Delete(0,1);
	      break;
	   }
	   shaderName += curC;

	   dataStr.Delete(0,1);
   }

   shaderCode = dataStr;


   //if(!GetByName(shaderName))
   //   return FALSE;

   //compare then add
   CShaderObj * shdrPtr = new CShaderObj();
   shdrPtr->m_shaderCode = shaderCode;
   shdrPtr->m_shaderName = shaderName;
   shdrPtr->m_compiler = fxType;//flag fx
   shdrPtr->m_shaderType = 1;

   

   shdrPtr->Compile(pd3dDevice,NULL);

   int index = 0;
   CShaderObj * shdrPtrDup = GetByName(shaderName,index);
   fxNameRet = shaderName;
   if(shdrPtrDup)
      if(shdrPtrDup->m_effectPeramCount != shdrPtr->m_effectPeramCount)
	  {
		  shdrPtr->m_shaderName + " ParamCount:"+ ConvToString(shdrPtr->m_effectPeramCount);
		  AddTail(shdrPtr);
	     return (int)(GetCount()-1);
	  }
	  else
	     return index;//found valid shader
	     
   
   if(!shdrPtrDup)
       AddTail(shdrPtr);

   return (int)(GetCount()-1);//v-fmt
}//end let the parse begin

//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----converts a long into string-----------------------------//
CString CShaderObjList::ConvToString(int number)
{    
	          CString temp;
			  temp.Format("%i",number);
              /*if(number==0)
				 return "0";
	 
	          int decimal,sign;
              char *buffer;
              CString temp;	 
	          buffer = _fcvt(number,0,&decimal,&sign);
    	      temp = buffer;
			  if(sign != 0)
                 temp = operator+("-",temp);*/
		
			  return temp;
}

void CShaderObj::Serialize( CArchive& ar )
{
BOOL reserved = 0;
CObject::Serialize( ar );
	if(ar.IsStoring() ){
       ar.SetObjectSchema(2);
	   ASSERT(ar.GetObjectSchema() == 2);

	   ar << m_shaderName
          << m_shaderCode;

	   ar << m_shaderType
		  << m_compiler
		  << reserved
		  << reserved
		  << reserved;
	}
	else
	{
       int version = ar.GetObjectSchema();

       ar >> m_shaderName
          >> m_shaderCode;

	   if(version == 2)
	   {
          ar >> m_shaderType
		     >> m_compiler
		     >> reserved
		     >> reserved
		     >> reserved;
	   }


	   m_shader = NULL;
	   m_pShader = NULL;
	   m_pEffect = NULL;
	   m_shaderUsedOnce = 0;
	}
}     


IMPLEMENT_SERIAL(CShaderObj, CObject, VERSIONABLE_SCHEMA|2)
IMPLEMENT_SERIAL(CShaderObjList, CObList, 0)


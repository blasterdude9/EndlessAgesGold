//#include "stdafx.h"
#include "afxwin.h"
#include "afxext.h"
#define D3D_OVERLOADS
#include "resource.h"
#include "d3d9.h"
//#include <afxtempl.h>
#include "chiarclemeshclass.h"

CHiarcleVisObj::CHiarcleVisObj()
{
   m_mesh = NULL;
}

void CHiarcleVisObj::Invalidate()
{
   if(m_mesh)
      m_mesh->m_abVertexArray.Invalidate();
}

void CHiarcleVisObj::Clone(CHiarcleVisObj ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
    CHiarcleVisObj * copyLocal = new CHiarcleVisObj();

	if(m_mesh)
	m_mesh->Clone(&copyLocal->m_mesh,g_pd3dDevice);

	*clone = copyLocal;
}

void CHiarcleVisObj::SafeDelete()
{
	if(m_mesh){
	   m_mesh->SafeDelete();
	   delete m_mesh;
	   m_mesh = 0;
	}
}

void CHiarcleVisObj::Optimize()
{
    if(m_mesh){
	   m_mesh->OptimizeMesh(NULL);
	   m_mesh->GenerateSmoothNormals();
	}
}

void CHiarcleVisObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_mesh;
	}
	else
	{
       ar >> m_mesh;
	}
}     

void CHiarcleVisList::Optimize()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CHiarcleVisObj * vObj = (CHiarcleVisObj *)GetNext(posLoc);
		vObj->Optimize();
	}
}

void CHiarcleVisList::Invalidate()
{
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CHiarcleVisObj * vObj = (CHiarcleVisObj *)GetNext(posLoc);
		vObj->Invalidate();
	}
}

void CHiarcleVisList::Clone(CHiarcleVisList ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     CHiarcleVisList * hVisList = new CHiarcleVisList();
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
        CHiarcleVisObj * vObj = (CHiarcleVisObj *)GetNext(posLoc);
		CHiarcleVisObj * cloneObj = NULL;
        vObj->Clone(&cloneObj,g_pd3dDevice);
		hVisList->AddTail(cloneObj);
	 }
    *clone = hVisList;
}

void CHiarcleVisList::SafeDelete()
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
        CHiarcleVisObj * vObj = (CHiarcleVisObj *)GetNext(posLoc);
        vObj->SafeDelete();
        delete vObj;
		vObj=0;
	 }
	 RemoveAll();
}

CHiarchyMesh::CHiarchyMesh()
{
  m_lodChain = NULL;
}

void CHiarchyMesh::Clone(CHiarchyMesh ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     CHiarchyMesh * copyLocal = new CHiarchyMesh();
	 m_lodChain->Clone(&copyLocal->m_lodChain,g_pd3dDevice);

	 *clone = copyLocal;
}

void CHiarchyMesh::SafeDelete()
{
	if(m_lodChain){
	 m_lodChain->SafeDelete();
     delete m_lodChain;
	 m_lodChain = 0;
	}

}

void CHiarchyMesh::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_lodChain;
	}
	else
	{
       ar >> m_lodChain;
	}
}  

void CHiarchyMesh::SetFadeOut(long duration)
{
    if(!m_lodChain)
		return;
        for(POSITION posLoc = m_lodChain->GetHeadPosition();posLoc != NULL;)
		{
	        CHiarcleVisObj * vObj = (CHiarcleVisObj *) m_lodChain->GetNext(posLoc);
			vObj->m_mesh->List_materialObject->SetFadeOut(duration);   
		}
}

void CHiarchyMesh::SetFadeIn(long duration)
{
    if(!m_lodChain)
		return;
        for(POSITION posLoc = m_lodChain->GetHeadPosition();posLoc != NULL;)
		{
	        CHiarcleVisObj * vObj = (CHiarcleVisObj *) m_lodChain->GetNext(posLoc);
			vObj->m_mesh->List_materialObject->SetFadeIn(duration,TRUE);   
		}
}

void CHiarchyMesh::Load(CString filename)
{
   CFileException exc;
     CFile fl;
     BOOL res = fl.Open(filename,CFile::modeRead, &exc);
	 if(res == FALSE)
		 return;
	     m_lodChain->SafeDelete();
		 delete m_lodChain;
		 m_lodChain=0;
	     CArchive the_in_Archive(&fl,CArchive::load);
	     the_in_Archive >> m_lodChain;
	     the_in_Archive.Close();
         fl.Close();
}

void CHiarchyMesh::Save(CString filename)
{
   CFileException exc;
   CFile fl;
   BOOL res = fl.Open(filename,CFile::modeCreate | CFile::modeWrite, &exc);
   CArchive the_out_Archive(&fl,CArchive::store);
		
   the_out_Archive << m_lodChain;

   the_out_Archive.Close();
   fl.Close(); 
}


BOOL CHiarchyMesh::SetLevel(int index,CString filename)
{
     CFileException exc;
     CFile fl;
 
     POSITION lodPos = m_lodChain->FindIndex(0);
    
	 CMaterialObject * primaryMaterial = NULL;
	 if(lodPos){
	    CHiarcleVisObj * delPtr = (CHiarcleVisObj *) m_lodChain->GetAt(lodPos);
        delPtr->m_mesh->List_materialObject->Clone(&primaryMaterial);
	 }
	 



     BOOL res = fl.Open(filename,CFile::modeRead, &exc);
	 if(res == FALSE)
		 return FALSE;

         CHiarcleVisObj * vObj = NULL;

	     CArchive the_in_Archive(&fl,CArchive::load);
	     the_in_Archive >> vObj;
	     the_in_Archive.Close();
         fl.Close();

		 if(primaryMaterial){
            delete vObj->m_mesh->List_materialObject;
			vObj->m_mesh->List_materialObject = NULL;
            primaryMaterial->Clone(&vObj->m_mesh->List_materialObject);
		 }

    POSITION posLoc = m_lodChain->FindIndex(index);
    if(!posLoc){
	    m_lodChain->AddTail(vObj);
	}
	else{//replace
        m_lodChain->InsertAfter(posLoc,vObj);
		CHiarcleVisObj * delPtr = (CHiarcleVisObj *) m_lodChain->GetAt(posLoc);
		delPtr->SafeDelete();
		delete delPtr;
		delPtr = 0;
		m_lodChain->RemoveAt(posLoc);
	}//end replace
   return TRUE;
}

CHiarcleVisObj * CHiarchyMesh::GetByIndex(int index)
{
    POSITION posLoc = m_lodChain->FindIndex(index);
	if(!posLoc)
		return NULL;
        CHiarcleVisObj * delPtr = (CHiarcleVisObj *)m_lodChain->GetAt(posLoc);
		return delPtr;
}

void CHiarchyMesh::Invalidate()
{

    if(m_lodChain)
	   m_lodChain->Invalidate();
}

BOOL CHiarchyMesh::SaveLevel(int index,CString filename)
{

if(!m_lodChain)
	   return FALSE;

   POSITION posLoc = m_lodChain->FindIndex(index);
   if(!posLoc)
	  return FALSE;

      CHiarcleVisObj * vObj = (CHiarcleVisObj *) m_lodChain->GetAt(posLoc);


   CFileException exc;
   CFile fl;
   BOOL res = fl.Open(filename,CFile::modeCreate | CFile::modeWrite, &exc);
   CArchive the_out_Archive(&fl,CArchive::store);
		
   the_out_Archive << vObj;

   the_out_Archive.Close();
   fl.Close(); 
   return TRUE;
}

IMPLEMENT_SERIAL(CHiarchyMesh, CObject, 0)
IMPLEMENT_SERIAL(CHiarcleVisObj, CObject, 0)
IMPLEMENT_SERIAL(CHiarcleVisList, CObject, 0)
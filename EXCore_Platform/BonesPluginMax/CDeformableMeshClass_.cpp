#define D3D_OVERLOADS
#include "resource.h"
#include <afxtempl.h>
#include "cdeformablemeshclass.h"

CDeformableVisObj::CDeformableVisObj()
{
   m_mesh                 = NULL;
   m_originalArray        = NULL;
   m_vertexAssignments    = NULL;
   m_assignmentCount      = 0;
}


BOOL CDeformableVisObj::VerticiePartialCompare(ABVERTEX v1,ABVERTEX v2)
{
     if(v1.x      == v2.x &&
	    v1.y      == v2.y &&
	    v1.z      == v2.z &&
	    v1.tx0.tu == v2.tx0.tu &&
	    v1.tx0.tv == v2.tx0.tv &&
	    v1.tx1.tu == v2.tx1.tu &&
		v1.tx1.tv == v2.tx1.tv &&
		v1.tx2.tu == v2.tx2.tu &&
		v1.tx2.tv == v2.tx2.tv &&
		v1.tx3.tu == v2.tx3.tu &&
		v1.tx3.tv == v2.tx3.tv &&
		v1.tx4.tu == v2.tx4.tu &&
		v1.tx4.tv == v2.tx4.tv &&
		v1.tx5.tu == v2.tx5.tu &&
		v1.tx5.tv == v2.tx5.tv &&
		v1.tx6.tu == v2.tx6.tu &&
		v1.tx6.tv == v2.tx6.tv &&
		v1.tx7.tu == v2.tx7.tu &&
		v1.tx7.tv == v2.tx7.tv)
        return  TRUE;//matches criteria

	 return FALSE;
}

void CDeformableVisObj::Optimize()
{
    if(!m_mesh)
		return;

        int newVertexArraySize = m_mesh->m_abVertexArray.m_vertexCount;
        //lets do this
	    for(int i=0;i<m_mesh->m_abVertexArray.m_indecieCount;i++)
		{//primary loop
			int currentIndecie = m_mesh->m_abVertexArray.m_indecieArray[i];
	        m_mesh->m_abVertexArray.m_origVertexArray[currentIndecie];//current vertex
            for(int c=0;c<m_mesh->m_abVertexArray.m_indecieCount;c++)
			{//check others loop
               if(c == i)//dont check same indecie
				  continue;
                
			   int alterIndecie = m_mesh->m_abVertexArray.m_indecieArray[c];

			   if(currentIndecie == alterIndecie)
                  continue;//already optimal
			      
			      if(VerticiePartialCompare(m_mesh->m_abVertexArray.m_origVertexArray[currentIndecie],m_mesh->m_abVertexArray.m_origVertexArray[alterIndecie]))
				  {//vertex is exact and can share base indecie value
                     m_mesh->m_abVertexArray.m_indecieArray[c] = currentIndecie;
                     newVertexArraySize--;
				  }//end vertex is exact and can share base indecie value

			}//end check others loop
		}//end primary loop
		//rebuild vertex array
		ABVERTEX * vArrayNew = new ABVERTEX[newVertexArraySize];
		//build non duplicate indecie array//
        int  nonDupIndecieCount = 0;
        WORD * newIndecieArray = new WORD[m_mesh->m_abVertexArray.m_indecieCount];
		CopyMemory(newIndecieArray,m_mesh->m_abVertexArray.m_indecieArray,(sizeof(WORD)*m_mesh->m_abVertexArray.m_indecieCount));
		
		//---------------------------------//
        //-Rebuild vertexArray and indecie-//
        //---------------------------------//
		VertexAssignment * newVertexAssignments  = NULL;
		int                vertexAssignmentCount = 0;
        int dupTrace = 0;
        for(int indecieLoop=0;indecieLoop<m_mesh->m_abVertexArray.m_indecieCount;indecieLoop++)
		{//1
            int currentValue = m_mesh->m_abVertexArray.m_indecieArray[indecieLoop];
            BOOL logIt = TRUE;
	        for(int count=0;count<indecieLoop;count++)
			{//check to see if this has been added
               if(m_mesh->m_abVertexArray.m_indecieArray[count] == currentValue){//if already has been used dont log
                  logIt = FALSE;
			      break;
			   }//end if already has been used dont log
			}//end check to see if this has been added
	         if(logIt == TRUE){
		        //set vertex data
				CopyMemory(&vArrayNew[dupTrace],&m_mesh->m_abVertexArray.m_origVertexArray[currentValue],sizeof(ABVERTEX));
                //SetVertexAssignmentvalueByVertexIndex(currentValue,dupTrace);

                //set vertex assignment addition
                for(int loop=0;loop<m_assignmentCount;loop++)
				{
                   if(m_vertexAssignments[loop].vertexIndex == currentValue)
				   {
                      AddVertexAssignmentIndi(m_vertexAssignments[loop].boneIndex,
									          dupTrace,
											  &newVertexAssignments,
											  &vertexAssignmentCount);
				   }
				}
                //end set vertex assignment addistion
				
				//adjust indecies
                for(int indLoop=0;indLoop<m_mesh->m_abVertexArray.m_indecieCount;indLoop++)
				{//check to see if this has been added
                 if(m_mesh->m_abVertexArray.m_indecieArray[indLoop] == currentValue){//if already has been used dont log
                    newIndecieArray[indLoop] = dupTrace; 
				 }//end if already has been used dont log
				}//end check to see if this has been added

               dupTrace++;
			 }
  }//end 1
   delete [] m_mesh->m_abVertexArray.m_indecieArray;
   m_mesh->m_abVertexArray.m_indecieArray=0;

   delete [] m_mesh->m_abVertexArray.m_origVertexArray;
   m_mesh->m_abVertexArray.m_origVertexArray = 0;

   m_mesh->m_abVertexArray.m_indecieArray = newIndecieArray;
   m_mesh->m_abVertexArray.m_origVertexArray   = vArrayNew;
   m_mesh->m_abVertexArray.m_vertexCount  = newVertexArraySize; 

   
   delete [] m_vertexAssignments;
   m_vertexAssignments = 0;
   m_vertexAssignments = newVertexAssignments;
   m_assignmentCount   = vertexAssignmentCount;

   CopyMemory(m_originalArray,m_mesh->m_abVertexArray.m_origVertexArray,sizeof(ABVERTEX)*m_mesh->m_abVertexArray.m_vertexCount);
   OptimizeVertexAssignments();
   m_mesh->GenerateSmoothNormals();
}


void CDeformableVisObj::OptimizeVertexAssignments()
{
   int newVertexCount     = 0;
   BOOL done = FALSE;

   while(done == FALSE)
   {
     done = TRUE;

     for(int v=0;v<m_assignmentCount;v++)
	 { 
       m_vertexAssignments[v].boneIndex;//current
       for(int c=0;c<m_assignmentCount;c++)
	   {
           if(c == v)
           continue;

           if(m_vertexAssignments[c].boneIndex != m_vertexAssignments[v].boneIndex)
		   continue;

           if(m_mesh->m_abVertexArray.m_origVertexArray[m_vertexAssignments[c].vertexIndex].x == m_mesh->m_abVertexArray.m_origVertexArray[m_vertexAssignments[v].vertexIndex].x &&
			  m_mesh->m_abVertexArray.m_origVertexArray[m_vertexAssignments[c].vertexIndex].y == m_mesh->m_abVertexArray.m_origVertexArray[m_vertexAssignments[v].vertexIndex].y &&
			  m_mesh->m_abVertexArray.m_origVertexArray[m_vertexAssignments[c].vertexIndex].z == m_mesh->m_abVertexArray.m_origVertexArray[m_vertexAssignments[v].vertexIndex].z)
		   {//same position optimize here
              AddAssignmentSub(&m_vertexAssignments[v],m_vertexAssignments[c].vertexIndex);
		   
			  for(int res=c;res<(m_assignmentCount-1);res++)
			  {//reindex array
                 CopyMemory(&m_vertexAssignments[res],&m_vertexAssignments[res+1],sizeof(VertexAssignment));
                
			  }//end reindex array
		   
			  m_assignmentCount--;
			  done = FALSE;
			  break;
		   }//end same position optimize here
	   }
	  if(!done)
		  break;//start over
	 }
   }
}

void CDeformableVisObj::SafeDelete()
{
	if(m_mesh){
       m_mesh->SafeDelete();
	   delete m_mesh;
       m_mesh = 0;
	}

	if(m_originalArray){
       delete [] m_originalArray;
       m_originalArray = 0;
	}

	if(m_vertexAssignments){
		for(int loop=0;loop<m_assignmentCount;loop++){
            if(m_vertexAssignments[loop].vertexesCarried){
               delete [] m_vertexAssignments[loop].vertexesCarried;
               m_vertexAssignments[loop].vertexesCarried = 0;
			}
		}
       delete m_vertexAssignments;
	   m_vertexAssignments=0;
	}
}

void CDeformableVisObj::Clone(CDeformableVisObj ** clone)
{
     CDeformableVisObj * copyLocal = new CDeformableVisObj();

	 copyLocal->m_assignmentCount = m_assignmentCount;

	 if(m_mesh){
	    m_mesh->Clone(&copyLocal->m_mesh);
        copyLocal->m_originalArray = new ABVERTEX[m_mesh->m_abVertexArray.m_vertexCount];
        CopyMemory(copyLocal->m_originalArray,m_originalArray,(sizeof(ABVERTEX)*m_mesh->m_abVertexArray.m_vertexCount));
	 }

     if(m_vertexAssignments){
		 if(m_assignmentCount > 0)
            copyLocal->m_vertexAssignments = new VertexAssignment[m_assignmentCount];
		    for(int loop=0;loop<m_assignmentCount;loop++){
                copyLocal->m_vertexAssignments[loop].boneIndex        = m_vertexAssignments[loop].boneIndex;
				copyLocal->m_vertexAssignments[loop].carriedCount     = m_vertexAssignments[loop].carriedCount;
				copyLocal->m_vertexAssignments[loop].vertexIndex      = m_vertexAssignments[loop].vertexIndex;
				copyLocal->m_vertexAssignments[loop].vertexesCarried  = NULL;
				if(m_vertexAssignments[loop].carriedCount > 0){
				   copyLocal->m_vertexAssignments[loop].vertexesCarried = new int[m_vertexAssignments[loop].carriedCount];
				   CopyMemory(copyLocal->m_vertexAssignments[loop].vertexesCarried,m_vertexAssignments[loop].vertexesCarried,(sizeof(int)*m_vertexAssignments[loop].carriedCount));
				}
			}
	 }

	 *clone = copyLocal;
}

void CDeformableVisObj::AddAssignmentSub(VertexAssignment * assignment,
										 int appendVertexIndex)
{
   int * vertexCarriedLocal = new int[assignment->carriedCount+1];

   for(int loop=0;loop<assignment->carriedCount;loop++)
   {
       vertexCarriedLocal[loop] = assignment->vertexesCarried[loop];
   }

   vertexCarriedLocal[assignment->carriedCount] = appendVertexIndex;

   assignment->carriedCount++;

   delete [] assignment->vertexesCarried;
   assignment->vertexesCarried = 0;

   assignment->vertexesCarried = new int[assignment->carriedCount];
   CopyMemory(assignment->vertexesCarried,vertexCarriedLocal,(sizeof(int)*assignment->carriedCount));
   delete [] vertexCarriedLocal;
   vertexCarriedLocal=0;
}


void CDeformableVisObj::AddVertexAssignmentIndi(              int boneIndex,
											                  int vertexIndex,
															  VertexAssignment ** vReturn,
															  int *vCountReturn)
{
  //allocate new list
  VertexAssignment * vertexObj = new VertexAssignment[(*vCountReturn+1)];
  VertexAssignment * oldArray = *vReturn;
   //transfer
   for(int i=0;i<*vCountReturn;i++)
   {
     vertexObj[i].boneIndex          = oldArray[i].boneIndex;
     vertexObj[i].vertexIndex        = oldArray[i].vertexIndex;
     vertexObj[i].carriedCount       = 0;
     vertexObj[i].vertexesCarried    = NULL;
   }
   //add new
     vertexObj[*vCountReturn].boneIndex   = boneIndex;
     vertexObj[*vCountReturn].vertexIndex = vertexIndex;
	 vertexObj[*vCountReturn].carriedCount       = 0;
     vertexObj[*vCountReturn].vertexesCarried    = NULL;

	 if(*vReturn)
	 delete [] *vReturn;
	 *vReturn = 0;

     *vReturn = new VertexAssignment[(*vCountReturn+1)];

	 CopyMemory(*vReturn,vertexObj,(sizeof(VertexAssignment)*(*vCountReturn+1)));

	 if(vertexObj)
		delete [] vertexObj;
	    vertexObj = 0;
	 *vCountReturn = (*vCountReturn+1);
}

void CDeformableVisObj::AddVertexAssignment(int boneIndex,
											int vertexIndex)
{
//allocate new list
VertexAssignment * vertexObj = new VertexAssignment[(m_assignmentCount+1)];

//transfer
for(int i=0;i<m_assignmentCount;i++)
{
   vertexObj[i].boneIndex          = m_vertexAssignments[i].boneIndex;
   vertexObj[i].vertexIndex        = m_vertexAssignments[i].vertexIndex;
   vertexObj[i].carriedCount       = 0;
   vertexObj[i].vertexesCarried    = NULL;
}
//add new
vertexObj[m_assignmentCount].boneIndex   = boneIndex;
vertexObj[m_assignmentCount].vertexIndex = vertexIndex;

if(m_vertexAssignments)
{
   delete [] m_vertexAssignments;
   m_vertexAssignments = 0;
}

m_assignmentCount++;
m_vertexAssignments = new VertexAssignment[m_assignmentCount];
CopyMemory(m_vertexAssignments,vertexObj,(sizeof(VertexAssignment)*m_assignmentCount));
if(vertexObj)
   delete [] vertexObj;
   vertexObj = 0;

}


void CDeformableVisObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_mesh << m_assignmentCount;

       for(int i=0;i<m_mesh->m_abVertexArray.m_vertexCount;i++)
	   ar << m_originalArray[i].x
		  << m_originalArray[i].y
		  << m_originalArray[i].z;

       for(int loop=0;loop<m_assignmentCount;loop++){
           ar << m_vertexAssignments[loop].boneIndex
			  << m_vertexAssignments[loop].carriedCount
			  << m_vertexAssignments[loop].vertexIndex;
 
		   for(int i=0;i<m_vertexAssignments[loop].carriedCount;i++)
		   ar << m_vertexAssignments[loop].vertexesCarried[i];
	   }
	} 
	else
	{
       ar >> m_mesh >> m_assignmentCount;

	   if(m_mesh->m_abVertexArray.m_vertexCount > 0){
	      m_originalArray = new ABVERTEX[m_mesh->m_abVertexArray.m_vertexCount];
          for(int i=0;i<m_mesh->m_abVertexArray.m_vertexCount;i++)
	      ar >> m_originalArray[i].x
		     >> m_originalArray[i].y
		     >> m_originalArray[i].z;
	   }

       m_vertexAssignments = NULL;
       if(m_assignmentCount > 0)
          m_vertexAssignments = new VertexAssignment[m_assignmentCount];

	   for(int loop=0;loop<m_assignmentCount;loop++){
           ar >> m_vertexAssignments[loop].boneIndex
			  >> m_vertexAssignments[loop].carriedCount
			  >> m_vertexAssignments[loop].vertexIndex;
 
		   m_vertexAssignments[loop].vertexesCarried = NULL;
           if(m_vertexAssignments[loop].carriedCount > 0)
			  m_vertexAssignments[loop].vertexesCarried = new int[m_vertexAssignments[loop].carriedCount];

		      for(int i=0;i<m_vertexAssignments[loop].carriedCount;i++)
		      ar >> m_vertexAssignments[loop].vertexesCarried[i];
	   }


	}
}   

CDeformableVisObj * CDeformableVisList::GetByIndex(int index)
{
    POSITION posLoc = FindIndex(index);
	if(!posLoc)
		return NULL;
        return (CDeformableVisObj *)GetAt(posLoc);
}

void CDeformableVisList::Clone(CDeformableVisList ** clone)
{
   CDeformableVisList * copyLocal = new CDeformableVisList();
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CDeformableVisObj * dPtr = (CDeformableVisObj *)GetNext(posLoc);
		CDeformableVisObj * cloned = NULL;
		dPtr->Clone(&cloned);
		copyLocal->AddTail(cloned);
	}
   *clone = copyLocal;
}

void CDeformableVisList::Optimize()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CDeformableVisObj * dPtr = (CDeformableVisObj *)GetNext(posLoc);
	    dPtr->Optimize();
	}
}

void CDeformableVisList::Normalize()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CDeformableVisObj * dPtr = (CDeformableVisObj *)GetNext(posLoc);
	    dPtr->m_mesh->GenerateSmoothNormals();
	}
}

void CDeformableVisList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CDeformableVisObj * dPtr = (CDeformableVisObj *)GetNext(posLoc);
		dPtr->SafeDelete();
		delete dPtr;
        dPtr = 0;
	}
	RemoveAll();
}


CDeformableMesh::CDeformableMesh()
{
  m_lodChain = NULL;
}

void CDeformableMesh::SafeDelete()
{
	if(m_lodChain){
       m_lodChain->SafeDelete();
	   delete m_lodChain;
       m_lodChain=0;
	}
}

void CDeformableMesh::Clone(CDeformableMesh ** clone)
{
     CDeformableMesh * copyLocal = new CDeformableMesh();
	 if(m_lodChain)
	 m_lodChain->Clone(&copyLocal->m_lodChain);

     *clone = copyLocal;
}

void CDeformableMesh::Save(CString filename)
{
   CFileException exc;
   CFile fl;
   BOOL res = fl.Open(filename,CFile::modeCreate | CFile::modeWrite, &exc);
   CArchive the_out_Archive(&fl,CArchive::store);
		
   the_out_Archive << m_lodChain;

   the_out_Archive.Close();
   fl.Close(); 
}

BOOL CDeformableMesh::SaveLevel(int index,CString filename)
{
   if(!m_lodChain)
	   return FALSE;

   POSITION posLoc = m_lodChain->FindIndex(index);
   if(!posLoc)
	  return FALSE;

      CDeformableVisObj * vObj = (CDeformableVisObj *) m_lodChain->GetAt(posLoc);


   CFileException exc;
   CFile fl;
   BOOL res = fl.Open(filename,CFile::modeCreate | CFile::modeWrite, &exc);
   CArchive the_out_Archive(&fl,CArchive::store);
		
   the_out_Archive << vObj;

   the_out_Archive.Close();
   fl.Close(); 
   return TRUE;
}

BOOL CDeformableMesh::SetLevel(int index,CString filename)
{
   CFileException exc;
     CFile fl;
     BOOL res = fl.Open(filename,CFile::modeRead, &exc);
	 if(res == FALSE)
		 return FALSE;

	 POSITION lodPos = m_lodChain->FindIndex(0);
     CMaterialObject * primaryMaterial = NULL;
	 if(lodPos){
	    CDeformableVisObj * delPtr = (CDeformableVisObj *) m_lodChain->GetAt(lodPos);
        delPtr->m_mesh->List_materialObject->Clone(&primaryMaterial);
	 }


         CDeformableVisObj * vObj = NULL;

	     CArchive the_in_Archive(&fl,CArchive::load);
	     the_in_Archive >> vObj;
	     the_in_Archive.Close();
         fl.Close();

		 if(primaryMaterial){
            delete vObj->m_mesh->List_materialObject;
			vObj->m_mesh->List_materialObject = NULL;
            primaryMaterial->Clone(&vObj->m_mesh->List_materialObject);
			delete primaryMaterial;
			primaryMaterial=0;
		 }

    POSITION posLoc = m_lodChain->FindIndex(index);
    if(!posLoc){
	    m_lodChain->AddTail(vObj);
	}
	else{//replace
        m_lodChain->InsertAfter(posLoc,vObj);
		CDeformableVisObj * delPtr = (CDeformableVisObj *) m_lodChain->GetAt(posLoc);
		delPtr->SafeDelete();
		delete delPtr;
		delPtr = 0;
		m_lodChain->RemoveAt(posLoc);
	}//end replace

   return TRUE;
}



void CDeformableMesh::Load(CString filename)
{
     CFileException exc;
     CFile fl;
     BOOL res = fl.Open(filename,CFile::modeRead, &exc);
	 if(res == FALSE)
		 return;
	     if(m_lodChain)
	     m_lodChain->SafeDelete();
		 delete m_lodChain;
		 m_lodChain=0;
	     CArchive the_in_Archive(&fl,CArchive::load);
	     the_in_Archive >> m_lodChain;
	     the_in_Archive.Close();
         fl.Close();
}

void CDeformableMesh::Serialize( CArchive& ar )
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

void CDeformableMeshList::SafeDelete()
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
        CDeformableMesh * dPtr = (CDeformableMesh *)GetNext(posLoc);
		dPtr->SafeDelete();
		delete dPtr;
        dPtr = 0;
	 }
	 RemoveAll();
}

 

IMPLEMENT_SERIAL(CDeformableVisObj, CObject, 0)
IMPLEMENT_SERIAL(CDeformableVisList, CObList, 0)
IMPLEMENT_SERIAL(CDeformableMesh, CObList, 0)
IMPLEMENT_SERIAL(CDeformableMeshList, CObList, 0)
#include "stdafx.h"
#define D3D_OVERLOADS

#include <afxtempl.h>
#include "d3d9.h"
#include "cdeformablemeshclass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDeformableVisObj::CDeformableVisObj()
{
   m_mesh                 = NULL;
   m_originalArrayVector  = NULL;
   m_vertexAssignments    = NULL;
   m_blendedVertexAssignments = NULL;
   m_blendedVertexAssignmentsWt = NULL;
   m_assignmentCount      = 0;
   m_referencedMemory     = FALSE;
   m_blendedType          = FALSE;
}

void CDeformableVisObj::Invalidate()
{
    if(m_mesh)
	   m_mesh->m_abVertexArray.Invalidate();

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

	ABVERTEX * vArray = NULL;
	int count = 0;

	m_mesh->m_abVertexArray.GetABVERTEXStyle(&vArray,&count);


        int newVertexArraySize = m_mesh->m_abVertexArray.m_vertexCount;
        //lets do this
	    for(int i=0;i<m_mesh->m_abVertexArray.m_indecieCount;i++)
		{//primary loop
			int currentIndecie = m_mesh->m_abVertexArray.m_indecieArray[i];
	        vArray[currentIndecie];//current vertex
            for(int c=0;c<m_mesh->m_abVertexArray.m_indecieCount;c++)
			{//check others loop
               if(c == i)//dont check same indecie
				  continue;
                
			   int alterIndecie = m_mesh->m_abVertexArray.m_indecieArray[c];

			   if(currentIndecie == alterIndecie)
                  continue;//already optimal
			      
			      if(VerticiePartialCompare(vArray[currentIndecie],vArray[alterIndecie]))
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
				CopyMemory(&vArrayNew[dupTrace],&vArray[currentValue],sizeof(ABVERTEX));
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

   m_mesh->m_abVertexArray.m_indecieArray = newIndecieArray;
   //m_mesh->m_abVertexArray.m_origVertexArray   = vArrayNew;
   m_mesh->m_abVertexArray.m_vertexCount  = newVertexArraySize; 
   m_mesh->m_abVertexArray.SetABVERTEXStyle(NULL,vArrayNew,m_mesh->m_abVertexArray.m_vertexCount);

   
   delete [] m_vertexAssignments;
   m_vertexAssignments = 0;
   m_vertexAssignments = newVertexAssignments;
   m_assignmentCount   = vertexAssignmentCount;

   
   for(int vLoop = 0;vLoop<m_mesh->m_abVertexArray.m_vertexCount;vLoop++)
   {
       CopyMemory(&m_originalArrayVector[vLoop],&vArrayNew[vLoop],sizeof(D3DVECTOR));
   
   }
   //CopyMemory(m_originalArray,vArrayNew,sizeof(ABVERTEX)*m_mesh->m_abVertexArray.m_vertexCount);

   delete [] vArray;
   vArray = 0;

   OptimizeVertexAssignments();
   m_mesh->GenerateSmoothNormals();
}


void CDeformableVisObj::OptimizeVertexAssignments()
{

   ABVERTEX * vArray = NULL;
	int count = 0;

	m_mesh->m_abVertexArray.GetABVERTEXStyle(&vArray,&count);

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

           if(vArray[m_vertexAssignments[c].vertexIndex].x == vArray[m_vertexAssignments[v].vertexIndex].x &&
			  vArray[m_vertexAssignments[c].vertexIndex].y == vArray[m_vertexAssignments[v].vertexIndex].y &&
			  vArray[m_vertexAssignments[c].vertexIndex].z == vArray[m_vertexAssignments[v].vertexIndex].z)
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
 
   if(vArray)
	  delete [] vArray;
      vArray = 0;

}

void CDeformableVisObj::SafeDelete()
{
	if(m_mesh){
       m_mesh->SafeDelete();
	   delete m_mesh;
       m_mesh = 0;
	}

	if(!m_referencedMemory)
	if(m_originalArrayVector){
       delete [] m_originalArrayVector;
       m_originalArrayVector = 0;
	}

	if(!m_referencedMemory)
	{
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
	  if(m_blendedVertexAssignments)
	  {
		for(int loop=0;loop<m_assignmentCount;loop++){
			if(m_blendedVertexAssignments[loop].boneIndexes){
               delete [] m_blendedVertexAssignments[loop].boneIndexes;
               m_blendedVertexAssignments[loop].boneIndexes = 0;
			}
			if(m_blendedVertexAssignments[loop].weightsToBone){
               delete [] m_blendedVertexAssignments[loop].weightsToBone;
               m_blendedVertexAssignments[loop].weightsToBone = 0;
			}
			if(m_blendedVertexAssignments[loop].offsetVects){
               delete [] m_blendedVertexAssignments[loop].offsetVects;
               m_blendedVertexAssignments[loop].offsetVects = 0;
			}
            if(m_blendedVertexAssignments[loop].vertexesCarried){
               delete [] m_blendedVertexAssignments[loop].vertexesCarried;
               m_blendedVertexAssignments[loop].vertexesCarried = 0;
			}
		}
       delete [] m_blendedVertexAssignments;
	   m_blendedVertexAssignments=0;
 
	  }
      if(m_blendedVertexAssignmentsWt)
	  {
		for(int loop=0;loop<m_assignmentCount;loop++){
			if(m_blendedVertexAssignmentsWt[loop].boneIndexes){
               delete [] m_blendedVertexAssignmentsWt[loop].boneIndexes;
               m_blendedVertexAssignmentsWt[loop].boneIndexes = 0;
			}
			if(m_blendedVertexAssignmentsWt[loop].weightsToBone){
               delete [] m_blendedVertexAssignmentsWt[loop].weightsToBone;
               m_blendedVertexAssignmentsWt[loop].weightsToBone = 0;
			}
			if(m_blendedVertexAssignmentsWt[loop].offsetVects){
               delete [] m_blendedVertexAssignmentsWt[loop].offsetVects;
               m_blendedVertexAssignmentsWt[loop].offsetVects = 0;
			}
            if(m_blendedVertexAssignmentsWt[loop].vertexesCarried){
               delete [] m_blendedVertexAssignmentsWt[loop].vertexesCarried;
               m_blendedVertexAssignmentsWt[loop].vertexesCarried = 0;
			}
		}
       delete [] m_blendedVertexAssignmentsWt;
	   m_blendedVertexAssignmentsWt=0;
 
	  }

	    
	}
}

void CDeformableVisObj::Clone(CDeformableVisObj ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     CDeformableVisObj * copyLocal = new CDeformableVisObj();

	 copyLocal->m_assignmentCount = m_assignmentCount;
	 copyLocal->m_blendedType = m_blendedType;
	 copyLocal->m_referencedMemory = TRUE;

	 if(m_mesh){
		if(m_mesh->m_abVertexArray.m_vertexCount < 1)
		{
			*clone = NULL;
			return;
		}

	    m_mesh->Clone(&copyLocal->m_mesh,g_pd3dDevice);
		copyLocal->m_originalArrayVector = m_originalArrayVector;
        //copyLocal->m_originalArrayVector = new D3DVECTOR[m_mesh->m_abVertexArray.m_vertexCount];
        //CopyMemory(copyLocal->m_originalArrayVector,m_originalArrayVector,(sizeof(D3DVECTOR)*m_mesh->m_abVertexArray.m_vertexCount));
	 }
	 else 
	 {
		 *clone = NULL;
		 return;
	 }

     if(m_vertexAssignments)
	    copyLocal->m_vertexAssignments = m_vertexAssignments;
	 
	 if(m_blendedVertexAssignments)
        copyLocal->m_blendedVertexAssignments = m_blendedVertexAssignments;

     if(m_blendedVertexAssignmentsWt)
        copyLocal->m_blendedVertexAssignmentsWt = m_blendedVertexAssignmentsWt;

	 *clone = copyLocal;
}

void CDeformableVisObj::CloneAllRaw(CDeformableVisObj ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     CDeformableVisObj * copyLocal = new CDeformableVisObj();

	 copyLocal->m_assignmentCount = m_assignmentCount;
	 copyLocal->m_blendedType = m_blendedType;
	 copyLocal->m_referencedMemory = FALSE;

	 if(m_mesh){
	    m_mesh->Clone(&copyLocal->m_mesh,g_pd3dDevice);
		if(m_originalArrayVector && m_mesh->m_abVertexArray.m_vertexCount > 0)
		{
			copyLocal->m_originalArrayVector = new ABVERTEX0L[m_mesh->m_abVertexArray.m_vertexCount];
			CopyMemory(copyLocal->m_originalArrayVector,m_originalArrayVector,(sizeof(ABVERTEX0L)*m_mesh->m_abVertexArray.m_vertexCount));
		}
		else if(m_mesh->m_abVertexArray.m_vertexCount > 0)
		{
			copyLocal->m_originalArrayVector = new ABVERTEX0L[m_mesh->m_abVertexArray.m_vertexCount];
			m_mesh->m_abVertexArray.Fill0UVArrayData(copyLocal->m_originalArrayVector);
		}
	 }

     if(m_vertexAssignments)
	 {
	    copyLocal->m_vertexAssignments = new VertexAssignment[m_assignmentCount];
		ZeroMemory(copyLocal->m_vertexAssignments,sizeof(VertexAssignment) * m_assignmentCount);
        for(int i=0;i<m_assignmentCount;i++)
		{
            copyLocal->m_vertexAssignments[i].boneIndex = m_vertexAssignments[i].boneIndex;
			copyLocal->m_vertexAssignments[i].vertexIndex = m_vertexAssignments[i].vertexIndex;
			copyLocal->m_vertexAssignments[i].carriedCount = m_vertexAssignments[i].carriedCount;

			if(m_vertexAssignments[i].carriedCount > 0)
			{
			copyLocal->m_vertexAssignments[i].vertexesCarried = new short[m_vertexAssignments[i].carriedCount];
	        CopyMemory(copyLocal->m_vertexAssignments[i].vertexesCarried,m_vertexAssignments[i].vertexesCarried,sizeof(short)*copyLocal->m_vertexAssignments[i].carriedCount);
		    }
		}
	 }
	 
	 if(m_blendedVertexAssignments)
	 {
        copyLocal->m_blendedVertexAssignments = new VertexAssignmentBlended[m_assignmentCount];
        ZeroMemory(copyLocal->m_blendedVertexAssignments,sizeof(VertexAssignmentBlended) * m_assignmentCount);
        for(int i=0;i<m_assignmentCount;i++)
		{
            copyLocal->m_blendedVertexAssignments[i].boneIndexesCount = m_blendedVertexAssignments[i].boneIndexesCount;
			copyLocal->m_blendedVertexAssignments[i].vertexIndex      = m_blendedVertexAssignments[i].vertexIndex;
			copyLocal->m_blendedVertexAssignments[i].carriedCount     = m_blendedVertexAssignments[i].carriedCount;
	  
			if(m_blendedVertexAssignments[i].boneIndexesCount > 0)
			{
			   copyLocal->m_blendedVertexAssignments[i].boneIndexes = new byte[m_blendedVertexAssignments[i].boneIndexesCount];
               copyLocal->m_blendedVertexAssignments[i].offsetVects = new ABVERTEX0L[m_blendedVertexAssignments[i].boneIndexesCount];
			   copyLocal->m_blendedVertexAssignments[i].weightsToBone = new float[m_blendedVertexAssignments[i].boneIndexesCount];
 
			   CopyMemory(copyLocal->m_blendedVertexAssignments[i].boneIndexes,m_blendedVertexAssignments[i].boneIndexes,sizeof(byte)*m_blendedVertexAssignments[i].boneIndexesCount);
               CopyMemory(copyLocal->m_blendedVertexAssignments[i].offsetVects,m_blendedVertexAssignments[i].offsetVects,sizeof(ABVERTEX0L)*m_blendedVertexAssignments[i].boneIndexesCount);
			   CopyMemory(copyLocal->m_blendedVertexAssignments[i].weightsToBone,m_blendedVertexAssignments[i].weightsToBone,sizeof(float)*m_blendedVertexAssignments[i].boneIndexesCount);
			}
			
			
			if(m_blendedVertexAssignments[i].carriedCount > 0)
			{
			copyLocal->m_blendedVertexAssignments[i].vertexesCarried = new short[m_blendedVertexAssignments[i].carriedCount];
	        CopyMemory(copyLocal->m_blendedVertexAssignments[i].vertexesCarried,m_blendedVertexAssignments[i].vertexesCarried,sizeof(short)*copyLocal->m_blendedVertexAssignments[i].carriedCount);
		    }
		}
	 }

     if(m_blendedVertexAssignmentsWt)
	 {
        copyLocal->m_blendedVertexAssignmentsWt = new VertexAssignmentBldTgt[m_assignmentCount];
        ZeroMemory(copyLocal->m_blendedVertexAssignmentsWt,sizeof(VertexAssignmentBldTgt) * m_assignmentCount);
        for(int i=0;i<m_assignmentCount;i++)
		{
            copyLocal->m_blendedVertexAssignmentsWt[i].boneIndexesCount = m_blendedVertexAssignmentsWt[i].boneIndexesCount;
			copyLocal->m_blendedVertexAssignmentsWt[i].vertexIndex      = m_blendedVertexAssignmentsWt[i].vertexIndex;
			copyLocal->m_blendedVertexAssignmentsWt[i].carriedCount     = m_blendedVertexAssignmentsWt[i].carriedCount;
	  
			if(m_blendedVertexAssignmentsWt[i].boneIndexesCount > 0)
			{
			   copyLocal->m_blendedVertexAssignmentsWt[i].boneIndexes = new byte[m_blendedVertexAssignmentsWt[i].boneIndexesCount];
               copyLocal->m_blendedVertexAssignmentsWt[i].offsetVects = new ABVERTEX0Lb[m_blendedVertexAssignmentsWt[i].boneIndexesCount];
			   copyLocal->m_blendedVertexAssignmentsWt[i].weightsToBone = new float[m_blendedVertexAssignmentsWt[i].boneIndexesCount];
 
			   CopyMemory(copyLocal->m_blendedVertexAssignmentsWt[i].boneIndexes,m_blendedVertexAssignmentsWt[i].boneIndexes,sizeof(byte)*m_blendedVertexAssignmentsWt[i].boneIndexesCount);
               CopyMemory(copyLocal->m_blendedVertexAssignmentsWt[i].offsetVects,m_blendedVertexAssignmentsWt[i].offsetVects,sizeof(ABVERTEX0Lb)*m_blendedVertexAssignmentsWt[i].boneIndexesCount);
			   CopyMemory(copyLocal->m_blendedVertexAssignmentsWt[i].weightsToBone,m_blendedVertexAssignmentsWt[i].weightsToBone,sizeof(float)*m_blendedVertexAssignmentsWt[i].boneIndexesCount);
			}
			
			
			if(m_blendedVertexAssignmentsWt[i].carriedCount > 0)
			{
			copyLocal->m_blendedVertexAssignmentsWt[i].vertexesCarried = new short[m_blendedVertexAssignmentsWt[i].carriedCount];
	        CopyMemory(copyLocal->m_blendedVertexAssignmentsWt[i].vertexesCarried,m_blendedVertexAssignmentsWt[i].vertexesCarried,sizeof(short)*copyLocal->m_blendedVertexAssignmentsWt[i].carriedCount);
		    }
		}
	 }

	 *clone = copyLocal;
}

void CDeformableVisObj::AddAssignmentSub(VertexAssignment * assignment,
										 int appendVertexIndex)
{
   short * vertexCarriedLocal = new short[assignment->carriedCount+1];

   for(int loop=0;loop<assignment->carriedCount;loop++)
   {
       vertexCarriedLocal[loop] = assignment->vertexesCarried[loop];
   }

   vertexCarriedLocal[assignment->carriedCount] = appendVertexIndex;

   assignment->carriedCount++;

   delete [] assignment->vertexesCarried;
   assignment->vertexesCarried = 0;

   assignment->vertexesCarried = new short[assignment->carriedCount];
   CopyMemory(assignment->vertexesCarried,vertexCarriedLocal,(sizeof(short)*assignment->carriedCount));
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

void CDeformableVisObj::AddVertexAssignmentBlended(byte * boneIndexes,
												   float * wieghtsToBone,
												   int boneIndexesCount,
											       int vertexIndex)
{
//allocate new list
m_blendedType = TRUE;
VertexAssignmentBlended * vertexObj = new VertexAssignmentBlended[(m_assignmentCount+1)];

  //transfer
  for(int i=0;i<m_assignmentCount;i++)
  {
   vertexObj[i].boneIndexesCount = m_blendedVertexAssignments[i].boneIndexesCount;
   CopyMemory(vertexObj[i].boneIndexes,m_blendedVertexAssignments[i].boneIndexes,sizeof(byte)*m_blendedVertexAssignments[i].boneIndexesCount);
   CopyMemory(vertexObj[i].weightsToBone,m_blendedVertexAssignments[i].weightsToBone,sizeof(float)*m_blendedVertexAssignments[i].boneIndexesCount);
   vertexObj[i].vertexIndex        = m_blendedVertexAssignments[i].vertexIndex;
   vertexObj[i].carriedCount       = 0;
   vertexObj[i].vertexesCarried    = NULL;
  }
  //add new
  CopyMemory(vertexObj[m_assignmentCount].boneIndexes,boneIndexes,sizeof(byte)*boneIndexesCount);
  CopyMemory(vertexObj[m_assignmentCount].weightsToBone,wieghtsToBone,sizeof(float)*boneIndexesCount);
  //vertexObj[m_assignmentCount].   = boneIndex;
  vertexObj[m_assignmentCount].vertexIndex = vertexIndex;

  if(m_blendedVertexAssignments)
  {
   delete [] m_blendedVertexAssignments;
   m_blendedVertexAssignments = 0;
  }

  m_assignmentCount++;
  m_blendedVertexAssignments = new VertexAssignmentBlended[m_assignmentCount];
  CopyMemory(m_blendedVertexAssignments,vertexObj,(sizeof(VertexAssignmentBlended)*m_assignmentCount));
  if(vertexObj)
     delete [] vertexObj;
     vertexObj = 0;

}


void CDeformableVisObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
       ar.SetObjectSchema(3);
       ASSERT(ar.GetObjectSchema() == 3);

	   ar << m_mesh << m_assignmentCount << m_blendedType;

       if(!m_blendedType)
	   {//Rigid system
         for(int loop=0;loop<m_assignmentCount;loop++)
		 {//assignment loop
           ar << m_vertexAssignments[loop].boneIndex
			  << m_vertexAssignments[loop].carriedCount
			  << m_vertexAssignments[loop].vertexIndex;
		   
		   for(int i=0;i<m_vertexAssignments[loop].carriedCount;i++)
		   ar << m_vertexAssignments[loop].vertexesCarried[i];
	     }//end assignment loop
	   }//end Rigid system
	   else if(m_blendedType == 1)
	   {//blended system
	      int i=0;
          for(int loop=0;loop<m_assignmentCount;loop++)
		  {
              ar << m_blendedVertexAssignments[loop].boneIndexesCount
			     << m_blendedVertexAssignments[loop].carriedCount
			     << m_blendedVertexAssignments[loop].vertexIndex;

              for(i=0;i<m_blendedVertexAssignments[loop].boneIndexesCount;i++)
			  {
		          ar << m_blendedVertexAssignments[loop].boneIndexes[i] 
				     << m_blendedVertexAssignments[loop].weightsToBone[i]
					 << m_blendedVertexAssignments[loop].offsetVects[i].x
				     << m_blendedVertexAssignments[loop].offsetVects[i].y
					 << m_blendedVertexAssignments[loop].offsetVects[i].z
					 << m_blendedVertexAssignments[loop].offsetVects[i].nx
				     << m_blendedVertexAssignments[loop].offsetVects[i].ny
					 << m_blendedVertexAssignments[loop].offsetVects[i].nz;
			  }

			  for(i=0;i<m_blendedVertexAssignments[loop].carriedCount;i++)
		          ar << m_blendedVertexAssignments[loop].vertexesCarried[i];
		  }
	   }//end blended system
	   else if(m_blendedType == 2)
	   {//blended system w tangent
		  int i=0;
          for(int loop=0;loop<m_assignmentCount;loop++)
		  {
              ar << m_blendedVertexAssignmentsWt[loop].boneIndexesCount
			     << m_blendedVertexAssignmentsWt[loop].carriedCount
			     << m_blendedVertexAssignmentsWt[loop].vertexIndex;

              for(i=0;i<m_blendedVertexAssignmentsWt[loop].boneIndexesCount;i++)
			  {
		          ar << m_blendedVertexAssignmentsWt[loop].boneIndexes[i] 
				     << m_blendedVertexAssignmentsWt[loop].weightsToBone[i]
					 << m_blendedVertexAssignmentsWt[loop].offsetVects[i].x
				     << m_blendedVertexAssignmentsWt[loop].offsetVects[i].y
					 << m_blendedVertexAssignmentsWt[loop].offsetVects[i].z
					 << m_blendedVertexAssignmentsWt[loop].offsetVects[i].nx
				     << m_blendedVertexAssignmentsWt[loop].offsetVects[i].ny
					 << m_blendedVertexAssignmentsWt[loop].offsetVects[i].nz
					 << m_blendedVertexAssignmentsWt[loop].offsetVects[i].tX
				     << m_blendedVertexAssignmentsWt[loop].offsetVects[i].tY
					 << m_blendedVertexAssignmentsWt[loop].offsetVects[i].tZ;
			  }

			  for(i=0;i<m_blendedVertexAssignmentsWt[loop].carriedCount;i++)
		          ar << m_blendedVertexAssignmentsWt[loop].vertexesCarried[i];
		  }
	   }//end blended system w tangent
	} 
	else
	{
	    int version = ar.GetObjectSchema();
      
       m_vertexAssignments = NULL;
	   m_blendedVertexAssignments = NULL;
       
	   if(version < 2)
	   {//VERSION 1
	    ar >> m_mesh >> m_assignmentCount;
        m_blendedType = FALSE;
	    if(m_assignmentCount > 0)
           m_vertexAssignments = new VertexAssignment[m_assignmentCount];

	    if(m_mesh->m_abVertexArray.m_vertexCount > 0)
		{
		  m_originalArrayVector = new ABVERTEX0L[m_mesh->m_abVertexArray.m_vertexCount];
          m_mesh->m_abVertexArray.Fill0UVArrayData(m_originalArrayVector);//Correct
		  for(int i=0;i<m_mesh->m_abVertexArray.m_vertexCount;i++)
		  {
	          ar >> m_originalArrayVector[i].x
		         >> m_originalArrayVector[i].y
		         >> m_originalArrayVector[i].z;
		  }
	    }


	    int convTemp = 0;
	    for(int loop=0;loop<m_assignmentCount;loop++){
           ar >> convTemp;m_vertexAssignments[loop].boneIndex = convTemp;
		   ar >> convTemp;m_vertexAssignments[loop].carriedCount = convTemp;
		   ar >> convTemp;m_vertexAssignments[loop].vertexIndex = convTemp;
 
		   m_vertexAssignments[loop].vertexesCarried = NULL;
           if(m_vertexAssignments[loop].carriedCount > 0)
			  m_vertexAssignments[loop].vertexesCarried = new short[m_vertexAssignments[loop].carriedCount];
 
		        for(int i=0;i<m_vertexAssignments[loop].carriedCount;i++)
			    {
		          ar >> convTemp;m_vertexAssignments[loop].vertexesCarried[i] = convTemp;
			    }
	    }
	   }//END VERSION 1
	   else if(version == 2)
	   {//VERSION 2
        ar >> m_mesh >> m_assignmentCount;
        m_blendedType = FALSE;
	    if(m_assignmentCount > 0)
           m_vertexAssignments = new VertexAssignment[m_assignmentCount];

        if(m_mesh->m_abVertexArray.m_vertexCount > 0)
		{
	      //ABVERTEX * dummy = new ABVERTEX[m_mesh->m_abVertexArray.m_vertexCount];
		  m_originalArrayVector = new ABVERTEX0L[m_mesh->m_abVertexArray.m_vertexCount];
          m_mesh->m_abVertexArray.Fill0UVArrayData(m_originalArrayVector);//Correct
		  for(int i=0;i<m_mesh->m_abVertexArray.m_vertexCount;i++)
		  {
	          ar >> m_originalArrayVector[i].x
		         >> m_originalArrayVector[i].y
		         >> m_originalArrayVector[i].z;
		  }
		  //m_mesh->m_abVertexArray.Fill0UVArrayData(m_originalArrayVector);//Correct 
		}


	    for(int loop=0;loop<m_assignmentCount;loop++){
           ar >> m_vertexAssignments[loop].boneIndex;
		   ar >> m_vertexAssignments[loop].carriedCount;
		   ar >> m_vertexAssignments[loop].vertexIndex;
 
		   m_vertexAssignments[loop].vertexesCarried = NULL;
           if(m_vertexAssignments[loop].carriedCount > 0)
			  m_vertexAssignments[loop].vertexesCarried = new short[m_vertexAssignments[loop].carriedCount];
 
		        for(int i=0;i<m_vertexAssignments[loop].carriedCount;i++)
			    {
		          ar >> m_vertexAssignments[loop].vertexesCarried[i];
			    }
	    }
	   }//END VERSION 2
	   else if(version == 3)
	   {//VERSION 3
         ar >> m_mesh >> m_assignmentCount >> m_blendedType;
	     if(m_assignmentCount > 0)
		 {
		    if(m_blendedType == 0)
              m_vertexAssignments = new VertexAssignment[m_assignmentCount];
			else if(m_blendedType == 1)
			  m_blendedVertexAssignments = new VertexAssignmentBlended[m_assignmentCount];
			else if(m_blendedType == 2)
			  m_blendedVertexAssignmentsWt = new VertexAssignmentBldTgt[m_assignmentCount];
		 }

		 if(!m_blendedType)
         if(m_mesh->m_abVertexArray.m_vertexCount > 0)
		 {//build orig vector array
		  m_originalArrayVector = new ABVERTEX0L[m_mesh->m_abVertexArray.m_vertexCount];
		  m_mesh->m_abVertexArray.Fill0UVArrayData(m_originalArrayVector);//Correct 
		 }//end build orig vector array

		 if(!m_blendedType)
		 {//rigid type
	      for(int loop=0;loop<m_assignmentCount;loop++){
           ar >> m_vertexAssignments[loop].boneIndex;
		   ar >> m_vertexAssignments[loop].carriedCount;
		   ar >> m_vertexAssignments[loop].vertexIndex;
 
		   m_vertexAssignments[loop].vertexesCarried = NULL;
           if(m_vertexAssignments[loop].carriedCount > 0)
			  m_vertexAssignments[loop].vertexesCarried = new short[m_vertexAssignments[loop].carriedCount];
 
		        for(int i=0;i<m_vertexAssignments[loop].carriedCount;i++)
			    {
		          ar >> m_vertexAssignments[loop].vertexesCarried[i];
			    }
	      }
		 }//end rigid type
		 else if(m_blendedType == 1)
		 {//blended type
          for(int loop=0;loop<m_assignmentCount;loop++)
		  {
              ar >> m_blendedVertexAssignments[loop].boneIndexesCount
			     >> m_blendedVertexAssignments[loop].carriedCount
			     >> m_blendedVertexAssignments[loop].vertexIndex;

			  m_blendedVertexAssignments[loop].boneIndexes = NULL;
			  m_blendedVertexAssignments[loop].vertexesCarried = NULL;
			  m_blendedVertexAssignments[loop].weightsToBone = NULL;
			  m_blendedVertexAssignments[loop].offsetVects = NULL;
              if(m_blendedVertexAssignments[loop].boneIndexesCount > 0)
			  {
                 m_blendedVertexAssignments[loop].boneIndexes = new byte[m_blendedVertexAssignments[loop].boneIndexesCount];
				 m_blendedVertexAssignments[loop].weightsToBone = new float[m_blendedVertexAssignments[loop].boneIndexesCount];
				 m_blendedVertexAssignments[loop].offsetVects = new ABVERTEX0L[m_blendedVertexAssignments[loop].boneIndexesCount];
			  }

			  int i=0;
              for(i=0;i<m_blendedVertexAssignments[loop].boneIndexesCount;i++)
			  {
		          ar >> m_blendedVertexAssignments[loop].boneIndexes[i] 
				     >> m_blendedVertexAssignments[loop].weightsToBone[i]
					 >> m_blendedVertexAssignments[loop].offsetVects[i].x
				     >> m_blendedVertexAssignments[loop].offsetVects[i].y
					 >> m_blendedVertexAssignments[loop].offsetVects[i].z
					 >> m_blendedVertexAssignments[loop].offsetVects[i].nx
				     >> m_blendedVertexAssignments[loop].offsetVects[i].ny
					 >> m_blendedVertexAssignments[loop].offsetVects[i].nz;
			  }

			  
			  if(m_blendedVertexAssignments[loop].carriedCount > 0)
			     m_blendedVertexAssignments[loop].vertexesCarried = new short[m_blendedVertexAssignments[loop].carriedCount];

			  for(i=0;i<m_blendedVertexAssignments[loop].carriedCount;i++)
		          ar >> m_blendedVertexAssignments[loop].vertexesCarried[i];
		  }
		 }//end blended type
		 else if(m_blendedType == 2)
		 {//blended type w tangent
          for(int loop=0;loop<m_assignmentCount;loop++)
		  {
              ar >> m_blendedVertexAssignmentsWt[loop].boneIndexesCount
			     >> m_blendedVertexAssignmentsWt[loop].carriedCount
			     >> m_blendedVertexAssignmentsWt[loop].vertexIndex;

			  m_blendedVertexAssignmentsWt[loop].boneIndexes = NULL;
			  m_blendedVertexAssignmentsWt[loop].vertexesCarried = NULL;
			  m_blendedVertexAssignmentsWt[loop].weightsToBone = NULL;
			  m_blendedVertexAssignmentsWt[loop].offsetVects = NULL;
              if(m_blendedVertexAssignmentsWt[loop].boneIndexesCount > 0)
			  {
                 m_blendedVertexAssignmentsWt[loop].boneIndexes = new byte[m_blendedVertexAssignmentsWt[loop].boneIndexesCount];
				 m_blendedVertexAssignmentsWt[loop].weightsToBone = new float[m_blendedVertexAssignmentsWt[loop].boneIndexesCount];
				 m_blendedVertexAssignmentsWt[loop].offsetVects = new ABVERTEX0Lb[m_blendedVertexAssignmentsWt[loop].boneIndexesCount];
			  }
              int i=0;
              for(i=0;i<m_blendedVertexAssignmentsWt[loop].boneIndexesCount;i++)
			  {
		          ar >> m_blendedVertexAssignmentsWt[loop].boneIndexes[i] 
				     >> m_blendedVertexAssignmentsWt[loop].weightsToBone[i]
					 >> m_blendedVertexAssignmentsWt[loop].offsetVects[i].x
				     >> m_blendedVertexAssignmentsWt[loop].offsetVects[i].y
					 >> m_blendedVertexAssignmentsWt[loop].offsetVects[i].z
					 >> m_blendedVertexAssignmentsWt[loop].offsetVects[i].nx
				     >> m_blendedVertexAssignmentsWt[loop].offsetVects[i].ny
					 >> m_blendedVertexAssignmentsWt[loop].offsetVects[i].nz
					 >> m_blendedVertexAssignmentsWt[loop].offsetVects[i].tX
				     >> m_blendedVertexAssignmentsWt[loop].offsetVects[i].tY
					 >> m_blendedVertexAssignmentsWt[loop].offsetVects[i].tZ;
			  }

			  
			  if(m_blendedVertexAssignmentsWt[loop].carriedCount > 0)
			     m_blendedVertexAssignmentsWt[loop].vertexesCarried = new short[m_blendedVertexAssignmentsWt[loop].carriedCount];

			  for(i=0;i<m_blendedVertexAssignmentsWt[loop].carriedCount;i++)
		          ar >> m_blendedVertexAssignmentsWt[loop].vertexesCarried[i];
		  }
		 }//end blended type w tangent
	   }//END VERSION 3

	   //m_blendedVertexAssignments = NULL;
	   //m_blendedType              = FALSE;
       m_referencedMemory         = FALSE;

	}
}   

CDeformableVisObj * CDeformableVisList::GetByIndex(int index)
{
    POSITION posLoc = FindIndex(index);
	if(!posLoc)
		return NULL;
        return (CDeformableVisObj *)GetAt(posLoc);
}

void CDeformableVisList::Clone(CDeformableVisList ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
   CDeformableVisList * copyLocal = new CDeformableVisList();
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CDeformableVisObj * dPtr = (CDeformableVisObj *)GetNext(posLoc);
		CDeformableVisObj * cloned = NULL;
		dPtr->Clone(&cloned,g_pd3dDevice);
		if(cloned)
			copyLocal->AddTail(cloned);
	}
   *clone = copyLocal;
}

void CDeformableVisList::CloneAllRaw(CDeformableVisList ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
   CDeformableVisList * copyLocal = new CDeformableVisList();
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CDeformableVisObj * dPtr = (CDeformableVisObj *)GetNext(posLoc);
		CDeformableVisObj * cloned = NULL;
		dPtr->CloneAllRaw(&cloned,g_pd3dDevice);
		if(cloned)
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

void CDeformableVisList::Invalidate()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CDeformableVisObj * dPtr = (CDeformableVisObj *)GetNext(posLoc);
	    dPtr->Invalidate();
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
  m_publicDim = FALSE;
  m_dimName = "";
  m_collisionFiltered = true;
}

void CDeformableMesh::SafeDelete()
{
	if(m_lodChain){
       m_lodChain->SafeDelete();
	   delete m_lodChain;
       m_lodChain=0;
	}
}

void CDeformableMesh::Clone(CDeformableMesh ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     CDeformableMesh * copyLocal = new CDeformableMesh();
	 if(m_lodChain)
	 m_lodChain->Clone(&copyLocal->m_lodChain,g_pd3dDevice);

	 copyLocal->m_publicDim					= m_publicDim;
	 copyLocal->m_dimName					= m_dimName;
	 copyLocal->m_collisionFiltered			= m_collisionFiltered;

     *clone = copyLocal;
}

void CDeformableMesh::CloneAllRaw(CDeformableMesh ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     CDeformableMesh * copyLocal = new CDeformableMesh();
	 if(m_lodChain)
	 m_lodChain->CloneAllRaw(&copyLocal->m_lodChain,g_pd3dDevice);

     copyLocal->m_publicDim = m_publicDim;
	 copyLocal->m_dimName = m_dimName;
	 copyLocal->m_collisionFiltered = m_collisionFiltered;

     *clone = copyLocal;
}

void CDeformableMesh::Invalidate()
{
    if(m_lodChain)
	   m_lodChain->Invalidate();
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

CDeformableVisObj * CDeformableMesh::GetLOD(int & lod, int & overrideLod)
{
	if (!m_lodChain) return NULL;
	 if(overrideLod > -1)
        return m_lodChain->GetByIndex(overrideLod);
	 if(lod > -1)
        return m_lodChain->GetByIndex(lod);
     return NULL;
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

///////////////////////////////////////////////////////////////////////////////
// Function:	PadAltIds(CString*)
// Description:	Pad ALT IDs for deformable mesh alternate configurations.
//				For example, "ALT1_Hair" becomes "ALT01_Hair". Note, this
//				solution only pads to the tens place and only for the
//				"ALT#_xxxx" naming pattern.
///////////////////////////////////////////////////////////////////////////////

CString
PadAltIds(CString* strID)
{
	if(	(strID->Find("ALT", 0) == 0)
		&& ((*strID)[4] == '_'))
	{
		strID->Insert(3, '0');
	}
	return *strID;
}

void CDeformableMesh::Serialize( CArchive& ar )
{
	BOOL reserved = 0;
	CObject::Serialize( ar );
	if(ar.IsStoring())
	{	
		ar.SetObjectSchema(3);
		ASSERT(ar.GetObjectSchema() == 3);

		ar	<< m_lodChain;

		ar	<< m_publicDim 
			<< m_collisionFiltered
			<< reserved;

		ar	<< m_dimName;
	} 
	else
	{
		int version = ar.GetObjectSchema();
		m_publicDim = FALSE;

		ar >> m_lodChain;
		if(version > 1)
		{
			ar	>> m_publicDim 
				>> m_collisionFiltered
				>> reserved;
		}

		if(version > 2)
		{
			ar >> m_dimName;
		}
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

CDeformableMesh * CDeformableMeshList::GetByName(CString altName)
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
        CDeformableMesh * dPtr = (CDeformableMesh *)GetNext(posLoc);
		//if(altName.CompareNoCase(dPtr->m_dimName)==0)
		if(altName.CompareNoCase(dPtr->DimName())==0)
		   return dPtr;
	 }
	 return NULL;
}
 

IMPLEMENT_SERIAL(CDeformableVisObj, CObject, VERSIONABLE_SCHEMA|3)
IMPLEMENT_SERIAL(CDeformableVisList, CObList, 0)
IMPLEMENT_SERIAL(CDeformableMesh, CObList, VERSIONABLE_SCHEMA|3)
IMPLEMENT_SERIAL(CDeformableMeshList, CObList, 0)
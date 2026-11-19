#include "stdafx.h"
#include "afxwin.h"
#include "afxext.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "CReferenceImporterExporter.h"



CReferenceImpExpObj::CReferenceImpExpObj(CString setName, D3DMATRIX matrix)
{
    m_idName = setName;
	m_matrix = matrix;
	m_type = 0;
	//DPD Spotlight addition
	m_lightType = D3DLIGHT_POINT; //default to Omni/point
	m_theta = m_phi = m_falloff = 0.0f;
	m_direction.x = 0;
	m_direction.y = 0;
	m_direction.z = 1;
	m_atten0 = m_atten1 = m_atten2 = 0.0f;

	m_optionalBox = FALSE;
	m_minX = m_minY = m_minZ = 0.0f;
	m_maxX = m_maxY = m_maxZ = 0.0f;
}

CReferenceImpExpObj::CReferenceImpExpObj(CString setName, int type, D3DMATRIX matrix
										 ,float redValue, float greenValue, float blueValue
										 ,float startAttenuation, float endAttenuation, float intensity
										,int lightType, D3DVECTOR direction, float theta
										,float phi, float falloff, float sightRange)
{
	m_idName = setName;
	m_type   = type;
	m_matrix = matrix;
	m_redValue   = redValue;
	m_greenValue = greenValue;
	m_blueValue	 = blueValue;
	m_intensity  = intensity;
	m_sightRange = sightRange;
	m_startAttenuation	= startAttenuation;
	m_endAttenuation	= endAttenuation;
	//We convert from Max to D3D types
	//D3D {Point = 1, Spot = 2, Dir = 3}
	//Max {Omni = 0, Spot = 1, Dir = 2)
	m_lightType = lightType + 1; 

	memcpy(&m_direction, &direction, sizeof(D3DVECTOR));
	m_theta		= theta;
	m_phi		= phi;
	m_falloff	= falloff;

	//directX attenuation values
	//this formula is for nearly smooth fade
	m_atten0 = 1.0f;
	m_atten1 = 1.0f/endAttenuation;
	m_atten2 = 1.0f/endAttenuation/endAttenuation;

	m_optionalBox = FALSE;
	m_minX = m_minY = m_minZ = 0.0f;
	m_maxX = m_maxY = m_maxZ = 0.0f;
}


CReferenceImpExpObj::CReferenceImpExpObj(CString setName, int type, D3DMATRIX matrix
										 ,float redValue, float greenValue, float blueValue
										 ,float startAttenuation, float endAttenuation, float intensity
										,int lightType, D3DVECTOR direction, float theta
										,float phi, float falloff, float sightRange,
										BOOL optionalBox, float minX, float minY, float minZ,
										float maxX, float maxY, float maxZ)
{

    m_optionalBox = optionalBox;
	m_minX = minX;
	m_minY = minY;
	m_minZ = minZ;
	m_maxX = maxX;
	m_maxY = maxY;
	m_maxZ = maxZ;


	m_idName = setName;
	m_type   = type;
	m_matrix = matrix;
	m_redValue   = redValue;
	m_greenValue = greenValue;
	m_blueValue	 = blueValue;
	m_intensity  = intensity;
	m_sightRange = sightRange;
	m_startAttenuation	= startAttenuation;
	m_endAttenuation	= endAttenuation;
	//We convert from Max to D3D types
	//D3D {Point = 1, Spot = 2, Dir = 3}
	//Max {Omni = 0, Spot = 1, Dir = 2)
	m_lightType = lightType + 1; 

	memcpy(&m_direction, &direction, sizeof(D3DVECTOR));
	m_theta		= theta;
	m_phi		= phi;
	m_falloff	= falloff;

	//directX attenuation values
	//this formula is for nearly smooth fade
	m_atten0 = 1.0f;
	m_atten1 = 1.0f/endAttenuation;
	m_atten2 = 1.0f/endAttenuation/endAttenuation;
}

void CReferenceImpExpList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CReferenceImpExpObj * spnPtr = (CReferenceImpExpObj *)GetNext(posLoc);
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}


void CReferenceImpExpObj::Serialize( CArchive& ar )
{
int reserved = 0;

CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   
		ar.SetObjectSchema(5);
		ASSERT(ar.GetObjectSchema() == 5);

		ar  << m_idName;
			
		for(int loop=0;loop<4;loop++)
           for(int loop2=0;loop2<4;loop2++)
               ar << m_matrix.m[loop][loop2];

	   
		ar  << m_type
			<< m_redValue
			<< m_greenValue
		    << m_blueValue
		    << m_intensity
		    << m_startAttenuation
		    << m_endAttenuation;

		ar  << m_lightType
			<< m_direction.x
			<< m_direction.y
			<< m_direction.z
			<< m_theta
			<< m_phi
			<< m_falloff
			<< m_atten0
			<< m_atten1
			<< m_atten2;

		ar  << m_sightRange
			<< reserved
			<< reserved
			<< reserved;

		ar  << m_optionalBox
			<< m_minX
			<< m_minY
			<< m_minZ
		    << m_maxX
			<< m_maxY
			<< m_maxZ;
	}
	else
	{
	   int version = ar.GetObjectSchema();
	   
       ar  >> m_idName;
	   
	   for(int loop=0;loop<4;loop++)
           for(int loop2=0;loop2<4;loop2++)
               ar >> m_matrix.m[loop][loop2];

	   m_endAttenuation = m_startAttenuation = m_intensity = m_redValue = m_greenValue = m_blueValue = 1.0;
	   m_lightType = m_type = 0;
	   m_direction.x = m_direction.y = m_direction.z = m_theta = m_phi = m_falloff = m_atten0 = m_atten1 = m_atten2 = 0.0;
	   m_sightRange = m_endAttenuation;

	   if(version > 1)
	   {
           ar  >> m_type
			   >> m_redValue
			   >> m_greenValue
			   >> m_blueValue
		       >> m_intensity
			   >> m_startAttenuation
			   >> m_endAttenuation;
		   m_sightRange = m_endAttenuation;
	   }
	   if(version > 2)
	   {
		   ar  >> m_lightType
			   >> m_direction.x
			   >> m_direction.y
			   >> m_direction.z
			   >> m_theta
			   >> m_phi
			   >> m_falloff
			   >> m_atten0
			   >> m_atten1
			   >> m_atten2;
		   m_sightRange = m_endAttenuation;
	   }
	   if(version > 3)
	   {
		   ar  >> m_sightRange
			   >> reserved
			   >> reserved
			   >> reserved;

		   m_optionalBox = FALSE;
	       m_minX = m_minY = m_minZ = 0.0f;
	       m_maxX = m_maxY = m_maxZ = 0.0f;
	   }
	   if(version > 4)
	   {
		   ar  >> m_optionalBox
			   >> m_minX
			   >> m_minY
			   >> m_minZ
		       >> m_maxX
			   >> m_maxY
			   >> m_maxZ;
	   }
	}
}     

BOOL CReferenceImpExpList::AddNewReference(CString name, D3DMATRIX matrix)
{
    CReferenceImpExpObj *newPtr = new CReferenceImpExpObj(name,matrix);
	
	AddTail(newPtr);

    return TRUE;
}

BOOL CReferenceImpExpList::AddNewReference(CString name, int type, D3DMATRIX matrix
											,float redValue, float greenValue, float blueValue
											,float startAttenuation, float endAttenuation, float intensity
											,int lightType, D3DVECTOR direction, float theta
											,float phi, float falloff, float sightRange)
{
    CReferenceImpExpObj *newPtr = new CReferenceImpExpObj(name,type,matrix,redValue,greenValue,blueValue
															,startAttenuation,endAttenuation,intensity
															,lightType, direction, theta, phi, falloff,sightRange);
	
	AddTail(newPtr);
    
	return TRUE;
}

BOOL CReferenceImpExpList::AddNewReference(CString name, int type, D3DMATRIX matrix
											,float redValue, float greenValue, float blueValue
											,float startAttenuation, float endAttenuation, float intensity
											,int lightType, D3DVECTOR direction, float theta
											,float phi, float falloff, float sightRange,
											BOOL optionalBox, float boxMaxX, float boxMaxY, float boxMaxZ, float boxMinX, float boxMinY, float boxMinZ)
{
    CReferenceImpExpObj *newPtr = new CReferenceImpExpObj(name,type,matrix,redValue,greenValue,blueValue
															,startAttenuation,endAttenuation,intensity
															,lightType, direction, theta, phi, falloff,sightRange,optionalBox,boxMinX,boxMinY,boxMinZ,boxMaxX,boxMaxY,boxMaxZ);
	
	AddTail(newPtr);
    
	return TRUE;
}

BOOL CReferenceImpExpList::SaveToFile(CString fileName)
{
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
		
	  if(res == FALSE){
		  AfxMessageBox("Cannot create " + fileName);
	      return FALSE;
	   }
       CArchive the_out_Archive(&fl,CArchive::store);
       the_out_Archive << this;
       the_out_Archive.Close();
       fl.Close();
	   return TRUE;
}

BOOL CReferenceImpExpList::InitializeFromFile(CString fileName)
{
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
	    AfxMessageBox("Cannot open " + fileName);
		return FALSE;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);

	 CReferenceImpExpList * newImportedList = NULL;
     
	 the_in_Archive >> newImportedList;
	 the_in_Archive.Close();
     fl.Close();

	 if(!newImportedList)
	 {
        AfxMessageBox("Invaid Format " + fileName);
		return FALSE;
	 }

     for(POSITION posLoc = newImportedList->GetHeadPosition();posLoc != NULL;)
	 {
         CReferenceImpExpObj * oPtr = (CReferenceImpExpObj *) newImportedList->GetNext(posLoc);
         this->AddTail(oPtr);
	 }
	 newImportedList->RemoveAll();
	 delete newImportedList;
	 newImportedList = 0;

  return TRUE;
}



IMPLEMENT_SERIAL(CReferenceImpExpObj, CObject, VERSIONABLE_SCHEMA|5)
IMPLEMENT_SERIAL(CReferenceImpExpList, CObList, 0)


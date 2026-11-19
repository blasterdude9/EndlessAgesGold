#include "stdafx.h"
#define D3D_OVERLOADS
#include "d3d9.h"
#include <D3DX9.h>
#include "CTextureDBClass.h"
#include "CShadowClass.h"
#include "math.h"
#include "CCollisionClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

D3DVERTEX ToVertex(D3DXVECTOR3 pt);
BOOL DuplicateTri(CPolyRetStruct *Polys, int cPolys, CPolyRetStruct TestPoly);

CShadowClass::CShadowClass(float radius,
						   float collRadius,
						   float extentUp,
						   float extentDown,
						   BOOL scale,
						   int textureIndex,
						   CString textureSearchName)
{
    m_type          = 0;
	m_lastNormal.x  = 0.0f;
	m_lastNormal.y  = 1.0f;
	m_lastNormal.z  = 0.0f;
	ZeroMemory(&m_wldPosition,sizeof(D3DXVECTOR3));
    m_valid         = FALSE;

	m_color.r = 1.0f;
	m_color.g = 1.0f;
	m_color.b = 1.0f;

	m_pd3dDevice8 = NULL;	

	m_normals = NULL;
	
	m_vertices = NULL;
	m_numvertices = 0;
				
	m_collisionVects = NULL;
	m_numcollisionVects = 0;

	m_collisionScaleVects = NULL;
	m_numcollisionScaleVects = 0;

	m_firstTime = TRUE;

	m_scaleTexture = scale;
	m_beginScaling = FALSE;

	m_originalRadius       = radius;
	m_collisionRadius      = collRadius;
	m_extentUp             = (float)fabs(extentUp);   //just to be safe...
	m_extentDown           = (float)fabs(extentDown);
	m_textureSearchName    = textureSearchName;
    m_textureIndex         = textureIndex;

	//initialize shadow mesh object
	m_shadowMesh = NULL;
	m_shadowMesh = new CEXMeshObj();
	m_shadowMesh->List_blendType = 5;
	m_shadowMesh->m_validUvCount = 1;
	m_shadowMesh->List_textureSelect[0] = textureIndex;
	m_shadowMesh->List_materialObject = new CMaterialObject();
	m_shadowMesh->List_materialObject->m_textureTileType = 2;
	m_shadowMesh->List_materialObject->m_baseMaterial.Emissive.r = 1.0f;
    m_shadowMesh->List_materialObject->m_baseMaterial.Emissive.g = 1.0f;
    m_shadowMesh->List_materialObject->m_baseMaterial.Emissive.b = 1.0f;
    m_shadowMesh->List_materialObject->m_baseMaterial.Emissive.a = 1.0f;
}

void CShadowClass::SafeDelete()
{
	if(m_normals) 
		{ 
		delete [] m_normals;
		m_normals = NULL;
		}

	if(m_vertices) 
		{
		delete [] m_vertices;
		m_vertices = NULL;
		}

	if(m_collisionVects) 
		{
		delete [] m_collisionVects;
		m_collisionVects = NULL;
		}

	if(m_collisionScaleVects) 
		{
		delete [] m_collisionScaleVects;
		m_collisionScaleVects = NULL;
		}

	if(m_shadowMesh)
		{
		m_shadowMesh->m_abVertexArray.m_vertexArray1 = NULL;
		m_shadowMesh->SafeDelete();
		delete m_shadowMesh;
		m_shadowMesh = NULL;
		}
}


CShadowClass::~CShadowClass()
{

}

//FUNCTIONS

void CShadowClass::EditParameters(float radius,
								  float collRadius,
						          float extentUp,
						          float extentDown,
								  BOOL scale)
{
	m_originalRadius       = radius;
	m_collisionRadius      = collRadius;
	m_extentUp             = (float)fabs(extentUp);   //just to be safe...
	m_extentDown           = (float)fabs(extentDown);
	m_scaleTexture         = scale;
}

void CShadowClass::Clone(CShadowClass ** clone)
{
	CShadowClass * returnClone = new CShadowClass(m_originalRadius,
		                                          m_collisionRadius,
												  m_extentUp,
                                                  m_extentDown,
												  m_scaleTexture,
												  m_textureIndex,
												  m_textureSearchName);
	
	//returnClone->
	*clone = returnClone;
}

void CShadowClass::GetCEXMeshObject(CEXMeshObj **shadowObject)
{

	//create a mesh object from the shadow polys
	if(!m_shadowMesh->List_materialObject){
        m_shadowMesh->List_materialObject = new CMaterialObject();
        m_shadowMesh->List_materialObject->m_baseMaterial.Diffuse.r = 0.0f;
        m_shadowMesh->List_materialObject->m_baseMaterial.Diffuse.g = 0.0f;
        m_shadowMesh->List_materialObject->m_baseMaterial.Diffuse.b = 0.0f;
        m_shadowMesh->List_materialObject->m_baseMaterial.Diffuse.a = 0.0f;
        m_shadowMesh->List_materialObject->m_baseMaterial.Ambient.r = 0.0f;
        m_shadowMesh->List_materialObject->m_baseMaterial.Ambient.g = 0.0f;
        m_shadowMesh->List_materialObject->m_baseMaterial.Ambient.b = 0.0f;
        m_shadowMesh->List_materialObject->m_baseMaterial.Ambient.a = 0.0f;
        m_shadowMesh->List_materialObject->m_baseMaterial.Emissive.r = 1.0f;
        m_shadowMesh->List_materialObject->m_baseMaterial.Emissive.g = 1.0f;
        m_shadowMesh->List_materialObject->m_baseMaterial.Emissive.b = 1.0f;
        m_shadowMesh->List_materialObject->m_baseMaterial.Emissive.a = 1.0f;
		m_shadowMesh->List_materialObject->m_blendMode = 5;
		m_shadowMesh->List_materialObject->m_textureTileType = 2;
	}

	if(!m_vertices)
	    return;
	

	m_shadowMesh->m_abVertexArray.m_vertexArray1 = m_vertices;//Reference Memory
	m_shadowMesh->m_abVertexArray.m_vertexCount = m_numvertices;
	m_shadowMesh->m_abVertexArray.m_indecieCount = m_numvertices;
	m_shadowMesh->m_abVertexArray.m_uvCount = 1;
	if(m_shadowMesh->m_abVertexArray.m_indicieBuffer)
	{
       m_shadowMesh->m_abVertexArray.m_indicieBuffer->Release();
       m_shadowMesh->m_abVertexArray.m_indicieBuffer = 0;
	}
	if(m_shadowMesh->m_abVertexArray.m_vertexBuffer)
	{
       m_shadowMesh->m_abVertexArray.m_vertexBuffer->Release();
       m_shadowMesh->m_abVertexArray.m_vertexBuffer = NULL;
	}
	if(m_shadowMesh->m_abVertexArray.m_indecieArray) 
 	   delete [] m_shadowMesh->m_abVertexArray.m_indecieArray;
	m_shadowMesh->m_abVertexArray.m_indecieArray = new WORD[m_numvertices];
	for(int vert=0; vert < m_numvertices; vert++)
		m_shadowMesh->m_abVertexArray.m_indecieArray[vert] = WORD(vert);

	m_shadowMesh->List_textureSelect[0] = m_textureIndex;
	
	//CopyMemory(m_shadowMesh->m_abVertexArray.m_origVertexArray, m_vertices, sizeof(ABVERTEX)*m_numvertices);
	//m_shadowMesh->m_abVertexArray.m_vertexCount = m_numvertices;
    
	//create indicies
	//if(m_shadowMesh->m_abVertexArray.m_indecieArray) 
	//	delete [] m_shadowMesh->m_abVertexArray.m_indecieArray;

	
	
	//m_shadowMesh->m_abVertexArray.m_indecieArray = new WORD[m_numvertices];
	//for(int vert=0; vert < m_numvertices; vert++)
	//	m_shadowMesh->m_abVertexArray.m_indecieArray[vert] = WORD(vert);

	//return
	*shadowObject = m_shadowMesh;

}


void CShadowClass::Calculate( D3DXVECTOR3 objectPos,
				              void *collisionRef
			                )

{
   CCollisionObj *collisionObject = (CCollisionObj *)collisionRef;
   if(m_type == 1)
   {//
       int actualCollisions = 0;
	   int consideredCollsiions = 0;
	   int objectHit = 0;

	   D3DXVECTOR3 startSeg,segEnd;
	   startSeg.x = objectPos.x;
	   startSeg.y = objectPos.y + m_extentUp;
	   startSeg.z = objectPos.z;
	   segEnd.x   = objectPos.x;
	   segEnd.y   = objectPos.y - m_extentDown;
	   segEnd.z   = objectPos.z;

	   if(collisionObject->CollisionSegmentSI(
					  startSeg,
					  segEnd,
					  &m_wldPosition.x,
					  &m_wldPosition.y,
					  &m_wldPosition.z,
					  &actualCollisions,
					  &consideredCollsiions,
					  &m_lastNormal,
					  &objectHit,
					  100.0f,
					  FALSE
					 ))
					 m_valid = TRUE;
	                 else
				     m_valid = FALSE;
   }//
   else if(m_type == 0)
   {//matt texture shadow
	//////////////////////////
	//save variables to class
	//////////////////////////
	m_objectPos    = objectPos;
	m_radius       = m_originalRadius;

	/////////////
	//body
	/////////////

	//if(m_firstTime || !(m_oldPos == m_objectPos)) {

		if(m_collisionVects) { delete [] m_collisionVects; m_collisionVects = NULL; }
		MakeCollisionPoly();

		if(m_scaleTexture) {
			if(m_collisionScaleVects) {	delete [] m_collisionScaleVects; m_collisionScaleVects = NULL;}
			MakeCollisionScalePoly();
			GetCollidedScalePolys(collisionObject);
			if(m_beginScaling) ScaleTexture();
		}

		if(m_normals)  { delete [] m_normals;  m_normals  = NULL; }
		if(m_vertices) { delete [] m_vertices; m_vertices = NULL; }

		GetCollidedPolys(collisionObject);

		AddTextureCoordinates();
	//}
	
	/////////////
	//clean-up
	/////////////
	if(m_firstTime)	m_firstTime = FALSE;
	m_oldPos = m_objectPos;
   }//end matt texture shadow
}



 void CShadowClass::GetCollidedScalePolys( void *collisionRef )
{
    CCollisionObj *collisionObject = (CCollisionObj *)collisionRef;
	m_intersectY = 0;
	int CPolyCount = m_numcollisionScaleVects/3;

	int totalPolys=0;

	//get all polygons that collide with the collision shape
	for(int CPolyIndex=0; CPolyIndex < CPolyCount; CPolyIndex++)
	{
		int numberPolys=0;
		
		CPolyRetStruct dummyPolys[20];
		D3DXVECTOR3      dummyNormals[20];
		float		   dummyDistances[20];
		int			   dummyCollisions=0,
					   dummyConsidered=0;
		D3DXVECTOR3	   dummyLatestNormal, 
			           dummyVirtualNormal, 
					   dummyOrigin;
		float		   dummyX=0,
					   dummyY=0,
					   dummyZ=0,
					   dummyYMax=0;
		
		collisionObject->CollisionPolyRetrieval
						(
							&m_collisionScaleVects[CPolyIndex*3],
							&dummyX,
							&dummyY,
							&dummyZ,
							&dummyCollisions,
							&dummyConsidered,
							dummyNormals,
							dummyDistances,
        					numberPolys,
							&dummyYMax,
							TRUE, //RecordAllInfo,
							&dummyLatestNormal,
							dummyVirtualNormal,
							dummyOrigin,
							FALSE, //UseAngle,
							dummyPolys
						); //cNormals returns the number of polys if RecordAllInfo = TRUE

		totalPolys += numberPolys;
		m_intersectY += dummyY;	
	}

	if(totalPolys > 0) 
	   m_beginScaling = FALSE;
	else m_beginScaling = TRUE;

	m_intersectY /= CPolyCount;

}

void CShadowClass::GetCollidedPolys( void *collisionRef )
{
    CCollisionObj *collisionObject = (CCollisionObj *)collisionRef;
	int cCollPolys = m_numcollisionVects/3; //number of polys in CollisionVert structure

	CPolyRetStruct *PolyList = NULL;
	PolyList = new CPolyRetStruct[cCollPolys*20]; //maximum number of polygons that can be returned
	int cPolyList=0;

	D3DXVECTOR3 *Normals = NULL;
	Normals = new D3DXVECTOR3[cCollPolys*20]; //maximum number of normals that can be returned
	
	//get all polygons that collide with the collision shape
	for(int CPolyIndex=0; CPolyIndex < cCollPolys; CPolyIndex++)
	{
		int cNormals=0;
		CPolyRetStruct CollisionPolys[20];
		D3DXVECTOR3      CollisionNormals[20];

		//dummy variables for CollisionPolyRetrieval()
		int dummyCollisions=0, dummyConsidered=0;
		D3DXVECTOR3 dummyLatestNormal, dummyVirtualNormal, dummyOrigin;
		float dummyX=0, dummyY=0, dummyZ=0, dummyYMax=0;
		float dummyDistances[20];
		
		collisionObject->CollisionPolyRetrieval
						(
							&m_collisionVects[CPolyIndex*3],
							&dummyX,
							&dummyY,
							&dummyZ,
							&dummyCollisions,
							&dummyConsidered,
							CollisionNormals,
							dummyDistances,
        					cNormals,
							&dummyYMax,
							TRUE, //RecordAllInfo,
							&dummyLatestNormal,
							dummyVirtualNormal,
							dummyOrigin,
							FALSE, //UseAngle,
							CollisionPolys
						); //cNormals returns the number of polys if RecordAllInfo = TRUE

		//add all polygons that are not duplicates
		for(int addpoly=0; addpoly < cNormals; addpoly++) 
		{

			if(CollisionNormals[addpoly].y < 0.5f && CollisionNormals[addpoly].y > -0.5f)
				continue; //ignore polys that are mostly vertical

			if(!DuplicateTri(PolyList, cPolyList, CollisionPolys[addpoly]))
			{
				PolyList[cPolyList] = CollisionPolys[addpoly];
				Normals[cPolyList]  = CollisionNormals[addpoly];
				cPolyList++;
			}

		}
		
	}


	if(cPolyList <= 0)
	   return;
	////////////////////////
	////////////////////////
	m_vertices = new ABVERTEX1L[3*cPolyList];
	m_normals  = new D3DXVECTOR3[cPolyList];

	//copy polys and normals to class variables
	for(int poly=0; poly < cPolyList; poly++) {
		
		//if(Normals[poly].y > -.01f && Normals[poly].y < .01f) continue; //ignore vertical polys
		m_vertices[(poly*3)+0] = ToVertex(PolyList[poly].polygon[0]);
		m_vertices[(poly*3)+1] = ToVertex(PolyList[poly].polygon[1]);
		m_vertices[(poly*3)+2] = ToVertex(PolyList[poly].polygon[2]);
 
	    m_vertices[(poly*3)+0].ny = 1.0f;
		m_vertices[(poly*3)+1].ny = 1.0f;
		m_vertices[(poly*3)+2].ny = 1.0f;

		m_normals[poly] = Normals[poly];
	}

	//de-allocate memory
	if(PolyList)  delete [] PolyList;
	if(Normals) delete [] Normals;
	
	//send out number of polygons
	m_numvertices = cPolyList*3;
}


void CShadowClass::GetCollidedPolys2( void *collisionRef, 
									  ABVERTEX1L * buffer, 
									  int & returnVCount,
									  D3DXVECTOR3 objectPos,
									  float radius,
									  float extentUp,
									  float extentDown)
{
	CCollisionObj *collisionObject = (CCollisionObj *)collisionRef;
	int cCollPolys = m_numcollisionVects/3; //number of polys in CollisionVert structure
    returnVCount = 0;
	CPolyRetStruct *PolyList = NULL;
	PolyList = new CPolyRetStruct[cCollPolys*20]; //maximum number of polygons that can be returned
	int cPolyList=0;

	D3DXVECTOR3 *Normals = NULL;
	Normals = new D3DXVECTOR3[cCollPolys*20]; //maximum number of normals that can be returned
	
    //make collision vects
	//Creates a square pyramid
	float zmax = objectPos.z+radius;
	float zmin = objectPos.z-radius;
	float xmin = objectPos.x-radius;
	float xmax = objectPos.x+radius;
	float ymin = objectPos.y-extentDown;
	float ymax = objectPos.y+extentUp;
		
	D3DXVECTOR3 collisionVects[12];
	int collisionVectCount = 12;

 	collisionVects[0] = D3DXVECTOR3(objectPos.x, ymax, objectPos.z);
	collisionVects[1] = D3DXVECTOR3(xmin , ymin, zmax);
	collisionVects[2] = D3DXVECTOR3(xmax , ymin, zmin);

	collisionVects[3] = D3DXVECTOR3(objectPos.x, ymax, objectPos.z);
	collisionVects[4] = D3DXVECTOR3(xmin , ymin, zmin);
	collisionVects[5] = D3DXVECTOR3(xmax , ymin, zmax);

	collisionVects[6] = D3DXVECTOR3(objectPos.x, ymax, objectPos.z);
	collisionVects[7] = D3DXVECTOR3(xmin , ymin, objectPos.z);
	collisionVects[8] = D3DXVECTOR3(xmax , ymin, objectPos.z);

	collisionVects[ 9] = D3DXVECTOR3(objectPos.x, ymax, objectPos.z);
	collisionVects[10] = D3DXVECTOR3(objectPos.x, ymin, zmax);
	collisionVects[11] = D3DXVECTOR3(objectPos.x, ymin,  zmin);



	//get all polygons that collide with the collision shape
	for(int CPolyIndex=0; CPolyIndex < cCollPolys; CPolyIndex++)
	{
		int cNormals=0;
		CPolyRetStruct CollisionPolys[20];
		D3DXVECTOR3      CollisionNormals[20];

		//dummy variables for CollisionPolyRetrieval()
		int dummyCollisions=0, dummyConsidered=0;
		D3DXVECTOR3 dummyLatestNormal, dummyVirtualNormal, dummyOrigin;
		float dummyX=0, dummyY=0, dummyZ=0, dummyYMax=0;
		float dummyDistances[20];
		
		collisionObject->CollisionPolyRetrieval
						(
							&collisionVects[CPolyIndex*3],
							&dummyX,
							&dummyY,
							&dummyZ,
							&dummyCollisions,
							&dummyConsidered,
							CollisionNormals,
							dummyDistances,
        					cNormals,
							&dummyYMax,
							TRUE, //RecordAllInfo,
							&dummyLatestNormal,
							dummyVirtualNormal,
							dummyOrigin,
							FALSE, //UseAngle,
							CollisionPolys
						); //cNormals returns the number of polys if RecordAllInfo = TRUE

		//add all polygons that are not duplicates
		for(int addpoly=0; addpoly < cNormals; addpoly++) 
		{

			if(CollisionNormals[addpoly].y < 0.5f && CollisionNormals[addpoly].y > -0.5f)
				continue; //ignore polys that are mostly vertical

			if(!DuplicateTri(PolyList, cPolyList, CollisionPolys[addpoly]))
			{
				PolyList[cPolyList] = CollisionPolys[addpoly];
				Normals[cPolyList]  = CollisionNormals[addpoly];
				cPolyList++;
				//returnVCount = returnVCount + 3;
				
			}

		}
		
	}


	if(cPolyList <= 0)
	   return;
	////////////////////////
	////////////////////////
	
	
	returnVCount = 3*cPolyList;
	//m_vertices = new ABVERTEX1L[3*cPolyList];
	//m_normals  = new D3DXVECTOR3[cPolyList];

	//copy polys and normals to class variables
	ABVERTEX1L *vtx = NULL;
	for(int poly=0; poly < cPolyList; poly++) {
		
		//if(Normals[poly].y > -.01f && Normals[poly].y < .01f) continue; //ignore vertical polys
		//m_vertices[(poly*3)+0] = ToVertex(PolyList[poly].polygon[0]);
		//m_vertices[(poly*3)+1] = ToVertex(PolyList[poly].polygon[1]);
		//m_vertices[(poly*3)+2] = ToVertex(PolyList[poly].polygon[2]);
		buffer[(poly*3)+0].x = PolyList[poly].polygon[0].x;
		buffer[(poly*3)+0].y = PolyList[poly].polygon[0].y;
		buffer[(poly*3)+0].z = PolyList[poly].polygon[0].z;
		buffer[(poly*3)+1].x = PolyList[poly].polygon[1].x;
		buffer[(poly*3)+1].y = PolyList[poly].polygon[1].y;
		buffer[(poly*3)+1].z = PolyList[poly].polygon[1].z;
		buffer[(poly*3)+2].x = PolyList[poly].polygon[2].x;
		buffer[(poly*3)+2].y = PolyList[poly].polygon[2].y;
		buffer[(poly*3)+2].z = PolyList[poly].polygon[2].z;
 
		buffer[(poly*3)+0].nx = 0.0f;
	    buffer[(poly*3)+0].ny = 1.0f;
		buffer[(poly*3)+0].nz = 0.0f;
		buffer[(poly*3)+1].nx = 0.0f;
		buffer[(poly*3)+1].ny = 1.0f;
		buffer[(poly*3)+1].nz = 0.0f;
		buffer[(poly*3)+2].nx = 0.0f;
		buffer[(poly*3)+2].ny = 1.0f;
		buffer[(poly*3)+2].nz = 0.0f;

		//m_normals[poly] = Normals[poly];

        for(int line=0; line < 3; line++) {
			
			vtx = &buffer[(poly*3)+line];

			//y offset to help correct z-errors
			vtx->y += .2f;

			float d = radius*2;
			//calculate texture coordinates

			if(m_objectPos.x > vtx->x)
				vtx->tx0.tu = (-objectPos.x+radius+vtx->x)/d;
			else
				vtx->tx0.tu = (-objectPos.x-radius+vtx->x)/d+1.0f;

			if(m_objectPos.z > vtx->z)
				vtx->tx0.tv = (-objectPos.z+radius+vtx->z)/d;
			else
				vtx->tx0.tv = (-objectPos.z-radius+vtx->z)/d+1.0f;
		}			

	}

	//de-allocate memory
	if(PolyList)  delete [] PolyList;
	if(Normals) delete [] Normals;
	
	//send out number of polygons
	//m_numvertices = cPolyList*3;
}


void CShadowClass::MakeCollisionScalePoly()
{
	D3DXVECTOR3 pos = m_objectPos;

	//Creates a square pyramid
	float zmax = pos.z+2.0f;
	float zmin = pos.z-2.0f;
	float xmin = pos.x-2.0f;
	float xmax = pos.x+2.0f;
	float ymin = pos.y-2.0f;
	float ymax = pos.y;

	m_collisionScaleVects = new D3DXVECTOR3[6];

 	m_collisionScaleVects[0] = D3DXVECTOR3(pos.x, ymax, pos.z);
	m_collisionScaleVects[1] = D3DXVECTOR3(xmin , ymin, zmax);
	m_collisionScaleVects[2] = D3DXVECTOR3(xmax , ymin, zmin);

	m_collisionScaleVects[3] = D3DXVECTOR3(pos.x, ymax, pos.z);
	m_collisionScaleVects[4] = D3DXVECTOR3(xmin , ymin, zmin);
	m_collisionScaleVects[5] = D3DXVECTOR3(xmax , ymin, zmax);

	m_numcollisionScaleVects = 6;

}

void CShadowClass::MakeCollisionPoly()
{
	D3DXVECTOR3 pos = m_objectPos;

	//Creates a square pyramid
	float zmax = pos.z+m_collisionRadius;
	float zmin = pos.z-m_collisionRadius;
	float xmin = pos.x-m_collisionRadius;
	float xmax = pos.x+m_collisionRadius;
	float ymin = pos.y-m_extentDown;
	float ymax = pos.y+m_extentUp;
		
	m_collisionVects = new D3DXVECTOR3[12];

 	m_collisionVects[0] = D3DXVECTOR3(pos.x, ymax, pos.z);
	m_collisionVects[1] = D3DXVECTOR3(xmin , ymin, zmax);
	m_collisionVects[2] = D3DXVECTOR3(xmax , ymin, zmin);

	m_collisionVects[3] = D3DXVECTOR3(pos.x, ymax, pos.z);
	m_collisionVects[4] = D3DXVECTOR3(xmin , ymin, zmin);
	m_collisionVects[5] = D3DXVECTOR3(xmax , ymin, zmax);

	m_collisionVects[6] = D3DXVECTOR3(pos.x, ymax, pos.z);
	m_collisionVects[7] = D3DXVECTOR3(xmin , ymin, pos.z);
	m_collisionVects[8] = D3DXVECTOR3(xmax , ymin, pos.z);

	m_collisionVects[ 9] = D3DXVECTOR3(pos.x, ymax, pos.z);
	m_collisionVects[10] = D3DXVECTOR3(pos.x, ymin, zmax);
	m_collisionVects[11] = D3DXVECTOR3(pos.x, ymin,  zmin);

	m_numcollisionVects = 12;
}

void CShadowClass::ScaleTexture()
{
	if(m_numvertices <= 0) return;
	return; ////////////////////////////////////////////////////////////////////////////////
	float Y = m_intersectY;

	if(m_extentDown != 0) { 
		float yDist = (float)fabs(m_objectPos.y-Y);
		
		if(yDist > m_extentDown) 
			m_radius = 0.0f;
		else
			m_radius = m_originalRadius*(m_extentDown-yDist)/m_extentDown;
		
	}
	//else m_radius = m_radius;
	
	if(m_radius > m_originalRadius) m_radius = m_originalRadius;

}

void CShadowClass::AddTextureCoordinates()
{

	int numpolys = m_numvertices/3;
	ABVERTEX1L *vtx;

	for(int poly=0; poly < numpolys; poly++) {
		
		for(int line=0; line < 3; line++) {
			
			vtx = &m_vertices[(poly*3)+line];

			//y offset to help correct z-errors
			vtx->y += .2f;

			float d = m_radius*2;
			//calculate texture coordinates

			if(m_objectPos.x > vtx->x)
				vtx->tx0.tu = (-m_objectPos.x+m_radius+vtx->x)/d;
			else
				vtx->tx0.tu = (-m_objectPos.x-m_radius+vtx->x)/d+1.0f;

			if(m_objectPos.z > vtx->z)
				vtx->tx0.tv = (-m_objectPos.z+m_radius+vtx->z)/d;
			else
				vtx->tx0.tv = (-m_objectPos.z-m_radius+vtx->z)/d+1.0f;
				
			/*if(Pos.x > vtx->x)//same code, but in a form that makes sense
				vtx->tu = -((m_objectPos.x-m_radius)-vtx->x)/(m_radius*2);
			else
				vtx->tu = (vtx->x-(m_objectPos.x+m_radius))/(m_radius*2)+1.0f;

			if(Pos.z > vtx->z)
				vtx->tv = -((m_objectPos.z-m_radius)-vtx->z)/(m_radius*2);
			else
				vtx->tv = (vtx->z-(m_objectPos.z+m_radius))/(m_radius*2)+1.0f;*/
		}			
			
	}

}

void CShadowClass::DrawShadow(LPDIRECT3DDEVICE9 pd3dDevice,D3DXVECTOR3 offset)
{
/*
	m_pd3dDevice7  = pd3dDevice7;
	m_offset       = offset;
	//DRAW COLLIDED POLYGONS
	
	D3DMATERIAL7 panelMat, savedMaterial;
	ZeroMemory(&panelMat, sizeof(D3DMATERIAL7));
	ZeroMemory(&savedMaterial, sizeof(D3DMATERIAL7));

	panelMat.Emissive   = m_color;
	panelMat.Emissive.a = 1;

	pd3dDevice7->GetMaterial(&savedMaterial);
	pd3dDevice7->SetMaterial(&panelMat);
	
	D3DMATRIX init, savedWorldMat;
	ZeroMemory(&savedWorldMat, sizeof(D3DMATRIX));
	ZeroMemory(&init, sizeof(D3DMATRIX));

	init._11 = init._22 = init._33 = init._44 = 1.0f;

	init._41 -= m_offset.x;
	init._42 -= m_offset.y;
	init._43 -= m_offset.z;

	pd3dDevice7->GetTransform( D3DTRANSFORMSTATE_WORLD, &savedWorldMat );
	pd3dDevice7->SetTransform( D3DTRANSFORMSTATE_WORLD, &init );

	pd3dDevice7->SetTextureStageState( 0, D3DTSS_ADDRESS, D3DTADDRESS_CLAMP);
	pd3dDevice7->SetRenderState(D3DRENDERSTATE_ZBIAS, 16);
	pd3dDevice7->SetRenderState(D3DRENDERSTATE_ZFUNC, D3DCMP_LESSEQUAL);
    
	//get blend settings
	BOOL oldBlendEnable = FALSE;
	D3DBLEND oldSrcBlend, oldDestBlend;
	ZeroMemory(&oldSrcBlend, sizeof(D3DBLEND));
	ZeroMemory(&oldDestBlend, sizeof(D3DBLEND));
	
	pd3dDevice7->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, (LPDWORD)&oldBlendEnable );
	pd3dDevice7->GetRenderState( D3DRENDERSTATE_SRCBLEND,  (LPDWORD)&oldSrcBlend);
	pd3dDevice7->GetRenderState( D3DRENDERSTATE_DESTBLEND, (LPDWORD)&oldDestBlend );

	pd3dDevice7->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
	pd3dDevice7->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_ZERO );
    pd3dDevice7->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCCOLOR );

	pd3dDevice7->DrawPrimitive
		(
		D3DPT_TRIANGLELIST,
		D3DFVF_VERTEX,
		m_vertices, 
		m_numvertices,
		NULL
		);

	pd3dDevice7->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, oldBlendEnable );
    pd3dDevice7->SetRenderState( D3DRENDERSTATE_SRCBLEND,  oldSrcBlend );
	pd3dDevice7->SetRenderState( D3DRENDERSTATE_DESTBLEND, oldDestBlend );

	pd3dDevice7->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP );
	pd3dDevice7->SetRenderState(D3DRENDERSTATE_ZBIAS, 0);
	pd3dDevice7->SetTransform( D3DTRANSFORMSTATE_WORLD, &savedWorldMat );
	pd3dDevice7->SetMaterial(&savedMaterial);*/

}

//Helper functions
ABVERTEX1L CShadowClass::ToVertex(D3DXVECTOR3 pt)
{
	ABVERTEX1L NewPt;
	ZeroMemory(&NewPt, sizeof(ABVERTEX1L));

	NewPt.x = pt.x;
	NewPt.y = pt.y;
	NewPt.z = pt.z;

	return NewPt;
}

BOOL CShadowClass::DuplicateTri(CPolyRetStruct *Polys, int cPolys, CPolyRetStruct TestPoly)
{
	BOOL ret=FALSE;

	for(int p=0; p < cPolys; p++) 
	{
		if(	Polys[p].polygon[0] == TestPoly.polygon[0] &&
			Polys[p].polygon[1] == TestPoly.polygon[1] &&
			Polys[p].polygon[2] == TestPoly.polygon[2] )
		{
			ret = TRUE;
			break;
		}
		
	}

	return ret;
}

void CShadowClass::DrawLine( D3DXVECTOR3 max, D3DXVECTOR3 min)
{
	/*D3DMATRIX newMatrix;
	ZeroMemory(&newMatrix, sizeof(D3DMATRIX));
	newMatrix._11 = newMatrix._22 = newMatrix._33 = newMatrix._44 = 1.0f;
	
	D3DMATERIAL7 OldMaterial, NewMaterial;

	m_pd3dDevice7->GetMaterial(&OldMaterial);
	
	newMatrix._41 = newMatrix._41 - m_offset.x;
    newMatrix._42 = newMatrix._42 - m_offset.y;
    newMatrix._43 = newMatrix._43 - m_offset.z;
    
	m_pd3dDevice7->SetTransform( D3DTRANSFORMSTATE_WORLD, &newMatrix ); 

    ZeroMemory(&NewMaterial, sizeof(D3DMATERIAL7));
	D3DCOLORVALUE EmmisiveColor;EmmisiveColor.r = 1.0f;EmmisiveColor.g = 1.0f;EmmisiveColor.b = 1.0f;EmmisiveColor.a = 1.0f;
	NewMaterial.emissive = EmmisiveColor;
	
	D3DVERTEX BoxVerts[5];
	D3DXVECTOR3 v = D3DXVECTOR3(0,0,-1);
 
	//Draw top of box
	BoxVerts[0] = D3DVERTEX(D3DXVECTOR3(max.x,max.y,max.z),v,0,0);
	BoxVerts[1] = D3DVERTEX(D3DXVECTOR3(min.x,min.y,min.z),v,0,0);
	
	//draw box  
	m_pd3dDevice7->SetTexture(0, NULL);
	m_pd3dDevice7->SetTexture(1, NULL);

	//get blend settings
	BOOL oldBlendEnable = FALSE;

	m_pd3dDevice7->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, (LPDWORD)&oldBlendEnable );
	
	m_pd3dDevice7->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice7->SetMaterial( &NewMaterial );
    
	m_pd3dDevice7->DrawPrimitive(D3DPT_LINELIST, D3DFVF_VERTEX, BoxVerts, 2, NULL);
	
	m_pd3dDevice7->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, oldBlendEnable );
	m_pd3dDevice7->SetMaterial( &OldMaterial );*/
}


void CShadowClass::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_originalRadius
	      << m_collisionRadius
	      << m_extentUp
	      << m_extentDown
		  << m_color.r
		  << m_color.g
		  << m_color.b
		  << m_scaleTexture
		  << m_textureSearchName
          << m_textureIndex;
	}
	else
	{
       ar >> m_originalRadius
	      >> m_collisionRadius
	      >> m_extentUp
	      >> m_extentDown
		  >> m_color.r
		  >> m_color.g
		  >> m_color.b
		  >> m_scaleTexture
		  >> m_textureSearchName
          >> m_textureIndex;


	   m_type          = 0;
	   m_lastNormal.x  = 0.0f;
	   m_lastNormal.y  = 1.0f;
	   m_lastNormal.z  = 0.0f;
       m_valid         = FALSE;
     
	   m_pd3dDevice8 = NULL;	
       m_normals = NULL;

	   m_vertices = NULL;
	   m_numvertices = 0;

	   m_collisionVects = NULL;
	   m_numcollisionVects = 0;

   	   m_firstTime = TRUE;

	   m_beginScaling = FALSE;
	   m_collisionScaleVects = NULL;

	   m_shadowMesh = NULL;
	   m_shadowMesh = new CEXMeshObj;

	   /*m_shadowMesh->List_blendType = 5;
	   m_shadowMesh->List_materialObject = new CMaterialObject();
 	   m_shadowMesh->List_materialObject->m_textureTileType = 2;
	   m_shadowMesh->List_materialObject->m_baseMaterial.Emissive.r = 1.0f;
       m_shadowMesh->List_materialObject->m_baseMaterial.Emissive.g = 1.0f;
       m_shadowMesh->List_materialObject->m_baseMaterial.Emissive.b = 1.0f;
       m_shadowMesh->List_materialObject->m_baseMaterial.Emissive.a = 1.0f;*/
	
	}
}





IMPLEMENT_SERIAL(CShadowClass, CObject, 0)
//#include "CCollisionClass.h"
//#include "CEXMeshClass.h"
//#include "CStructuresMisl.h"


#pragma once
 
//PROTOTYPES
class CShadowClass : public CObject 
{
    CShadowClass(){}
    
     
	DECLARE_SERIAL( CShadowClass );
	
    

    public:

    int          m_type;//0=cut model shadows    1=mat texture shadow
	D3DXVECTOR3    m_lastNormal;
	BOOL         m_valid;
	D3DXVECTOR3    m_wldPosition;

	public://FUNCTIONS
		
		~CShadowClass();

		CShadowClass(float radius,
						   float collRadius,
						   float extentUp,
						   float extentDown,
						   BOOL scale,
						   int textureIndex,
						   CString textureSearchName);
        void Serialize( CArchive& ar );
		void Clone(CShadowClass ** clone);
		void EditParameters(float radius,
							float collRadius,
						    float extentUp,
						    float extentDown,
							BOOL scale);
		/*void Calculate( D3DXVECTOR3 objectPos,
						CCollisionObj *collisionObject
					   );

		void GetCEXMeshObject(CEXMeshObj **shadowObject);*/

	//private://FUNCTIONS
        void SafeDelete();
		/*void MakeCollisionPoly();
		void GetCollidedPolys(CCollisionObj *collisionObject);

		
		void MakeCollisionScalePoly();
		void GetCollidedScalePolys(CCollisionObj *collisionObject);

		void ScaleTexture();

		void AddTextureCoordinates();
		
		void DrawShadow(LPDIRECT3DDEVICE9 pd3dDevice,D3DXVECTOR3 offset);

		//helpers
		
		void DrawLine(D3DXVECTOR3 max, D3DXVECTOR3 min);
		ABVERTEX ToVertex(D3DXVECTOR3 pt);
		BOOL DuplicateTri(CPolyRetStruct *Polys, int cPolys, CPolyRetStruct TestPoly);*/

	//private://VARIABLES
        CString   m_textureSearchName;
		int       m_textureIndex;


		//LPDIRECT3DDEVICE9 m_pd3dDevice8; //Direct 3D Device 8.0
		D3DXVECTOR3 m_offset; //render offset

		D3DXVECTOR3 m_objectPos; //caster position
		D3DXVECTOR3 m_oldPos;    //caster's old position
		float m_radius; //shadow radius
		float m_collisionRadius; //collision radius
		float m_originalRadius; //shadow radius
		float m_extentUp;   //collision extention upwards
		float m_extentDown; //collision extention  downwards

		BOOL m_scaleTexture;
		BOOL m_beginScaling;
		float m_intersectY;
		
		D3DCOLORVALUE m_color;
				
		D3DXVECTOR3 *m_normals;  //normals for shadow polys
		
		ABVERTEX *m_vertices; //vertices for shadow
		int m_numvertices;	   //number of shadow vertices
				
		D3DXVECTOR3 *m_collisionVects; //"vertices" for collision object
		int m_numcollisionVects;     // number of collision "vertices"

		D3DXVECTOR3 *m_collisionScaleVects; //"vertices" for collision object
		int m_numcollisionScaleVects;     // number of collision "vertices"

		CEXMeshObj *m_shadowMesh;


		BOOL m_firstTime;
	
};

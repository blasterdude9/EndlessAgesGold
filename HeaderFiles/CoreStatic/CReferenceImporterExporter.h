#ifndef CREFERENCEIMPEXP_H
#define CREFERENCEIMPEXP_H

#define OBJECT_REF_TYPE 0
#define LIGHT_REF_TYPE  1

#ifdef PLUGIN 
class CReferenceImpExpObj : public CObject
#endif
#ifndef PLUGIN 
class  CReferenceImpExpObj : public CObject
#endif
{
	DECLARE_SERIAL( CReferenceImpExpObj );
        
public:
	CReferenceImpExpObj(){};
	CReferenceImpExpObj(CString setName, D3DMATRIX matrix);
	CReferenceImpExpObj(CString test, int type, D3DMATRIX matrix
					,float redValue, float greenValue, float blueValue
					,float startAttenuation, float endAttenuation, float intensity
					,int lightType, D3DVECTOR direction, float theta
                    ,float phi, float falloff, float sightRange);
	CReferenceImpExpObj(CString setName, int type, D3DMATRIX matrix
										 ,float redValue, float greenValue, float blueValue
										 ,float startAttenuation, float endAttenuation, float intensity
										,int lightType, D3DVECTOR direction, float theta
										,float phi, float falloff, float sightRange,
										BOOL optionalBox, float minX, float minY, float minZ,
										float maxX, float maxY, float maxZ);

    CString m_idName;
	int		m_type;
	D3DMATRIX m_matrix;
	float	m_redValue;
	float	m_blueValue;
	float	m_greenValue;
	float	m_intensity;
	float	m_startAttenuation;
	float   m_endAttenuation;

	//DPD Spotlight
	int       m_lightType;
	D3DVECTOR m_direction;
	float	  m_theta;
	float	  m_phi;
	float	  m_falloff;
	
	float   m_atten0;
	float   m_atten1;
	float   m_atten2;

	float   m_sightRange;

	BOOL m_optionalBox;
	float m_minX,m_minY,m_minZ;
	float m_maxX,m_maxY,m_maxZ;

	void Serialize( CArchive& ar );
	
};

#ifdef PLUGIN 
class  CReferenceImpExpList : public CObList
#endif
#ifndef PLUGIN 
class   CReferenceImpExpList : public CObList
#endif
{
public:
   CReferenceImpExpList(){};
   void SafeDelete();
   BOOL SaveToFile(CString fileName);
   BOOL AddNewReference(CString name, D3DMATRIX matrix);
   BOOL AddNewReference(CString name, int type, D3DMATRIX matrix
					,float redValue, float greenValue, float blueValue
					,float startAttenuation, float endAttenuation, float intensity
					,int lightType, D3DVECTOR direction, float theta 
					,float phi, float falloff, float sightRange);
   BOOL AddNewReference(CString name, int type, D3DMATRIX matrix
											,float redValue, float greenValue, float blueValue
											,float startAttenuation, float endAttenuation, float intensity
											,int lightType, D3DVECTOR direction, float theta
											,float phi, float falloff, float sightRange,
											BOOL optionalBox, float boxMaxX, float boxMaxY, float boxMaxZ, float boxMinX, float boxMinY, float boxMinZ);

   BOOL InitializeFromFile(CString fileName);
    
     
   DECLARE_SERIAL( CReferenceImpExpList );
};

#endif
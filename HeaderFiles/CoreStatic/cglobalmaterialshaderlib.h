#ifndef CGlobalMaterialShaderLib_H
#define CGlobalMaterialShaderLib_H

class CShaderDefObj : public CObject
{
	DECLARE_SERIAL( CShaderDefObj );
	public:
	CShaderDefObj()
	{
	   m_pixelShader = -1;
	   m_vertexShader = -1;
       m_minimumPixSupport = 0.0f;
	   m_minimumVSSupport = 0.0f;
	};

	int m_pixelShader;//-1=none
	int m_vertexShader;
    float m_minimumPixSupport;
	float m_minimumVSSupport;
	void Serialize( CArchive& ar )
	{
		CObject::Serialize( ar );
		if(ar.IsStoring() )
		{	
		   ar << m_pixelShader << m_vertexShader << m_minimumPixSupport << m_minimumVSSupport;
		}
		else
	    {
           ar >> m_pixelShader >> m_vertexShader >> m_minimumPixSupport >> m_minimumVSSupport; 
		}
	}
};

class CShaderChain : public CObList
{
   public:
   DECLARE_SERIAL( CShaderChain );
   CShaderChain(){};
   ~CShaderChain()
   {
       for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	   {
           CShaderDefObj * csdPtr = (CShaderDefObj *) GetNext(posLoc);
           delete csdPtr;
		   csdPtr = 0;
	   }
	   RemoveAll();
   };

   BOOL GetCompatible(int & vsShaderRef, int & pxShaderRef, float currentVSSup, float currentPXSup)
   {
       for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	   {
           CShaderDefObj * csdPtr = (CShaderDefObj *) GetNext(posLoc);
           if(currentPXSup >= csdPtr->m_minimumPixSupport && currentVSSup >= csdPtr->m_minimumVSSupport)
		   {
              vsShaderRef = csdPtr->m_vertexShader;
              pxShaderRef = csdPtr->m_pixelShader;
			  return TRUE;
		   }
	   }
	   vsShaderRef = -1;
       pxShaderRef = -1;

	   return FALSE;
   }
};

class CGlobalMaterialShader : public CObject
{
	
	DECLARE_SERIAL( CGlobalMaterialShader );
	
public:
    CGlobalMaterialShader();
	~CGlobalMaterialShader()
	{
       if(m_shaderChain)
	   {
          delete m_shaderChain;
		  m_shaderChain = 0;
	   }
	}
    
	CString         m_shaderName;
    int             m_requiredUvCount;

	int             m_chromeUvSet;
	int             m_enableChroming;

	int             m_textureIndexArray[10];//-1 = NO Change
    CString         m_textureStringArray[10];//pull textures and reassigns indexes
	int             m_blendArray[10];

    BOOL            m_useOverrideMaterial;
	D3DMATERIAL9    m_overrideMaterial;

	//shader chain
    CShaderChain   *m_shaderChain;//if valid we have custome pipline to use
	int             m_curPixShader;//initializes one time and sets this value based on chain -1 = none
	int             m_curVSShader;//initializes one time and sets this value based on chain -1 = none
	CString         m_shaderNotes;
  
	void ScaleForRuntimeCompatibility(BOOL & bumpSupport, float & supportedPx, float & supportedVs);
	void Serialize( CArchive& ar );
	
};

class CGlobalMaterialShaderList : public CObList
{
public:
   CGlobalMaterialShaderList(){};
   void SafeDelete();
    
	DECLARE_SERIAL( CGlobalMaterialShaderList );
	

	void ScaleForRuntimeCompatibility(BOOL & bumpSupport, float & supportedPx, float & supportedVs);
	CGlobalMaterialShader * GetByIndex(int index);
};

#endif
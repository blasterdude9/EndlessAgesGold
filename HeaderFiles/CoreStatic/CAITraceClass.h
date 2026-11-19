#ifndef CAITRACECLASS_H
#define CAITRACECLASS_H


//TextureSetList
class CAITraceObj : public CObject
{
	
     
	DECLARE_SERIAL( CAITraceObj );
	
    
public:
	CAITraceObj();
    CString        m_NameOfLocation;
	D3DVECTOR      m_locationCenter;
	float          m_toleranceOfLocation;

	BOOL SetData(CString      locationName,
				 D3DVECTOR    position,
				 float        Tolerance);
	void Serialize( CArchive& ar );

};

class CAITraceObjList : public CObList
{
public:
   CAITraceObjList(){};
   
     
	DECLARE_SERIAL( CAITraceObjList );
	
    
};

#endif
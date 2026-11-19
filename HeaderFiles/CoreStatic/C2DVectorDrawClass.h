#ifndef CEX2DVECTCLASS_H
#define CEX2DVECTCLASS_H

struct LINEEXG
{
    float startX,startY,endX,endY,circV;
};

class  CEX2DVectObj : public CObject
{
	
     
    DECLARE_SERIAL( CEX2DVectObj )
    
    
public:
    CEX2DVectObj();
                
    LINEEXG *                  List_xyPointArray;
    WORD                       List_xyPointCount; 
	float                      List_centerPointX;
	float                      List_centerPointY;
    float                      List_radius;
	
         
    void SafeDelete();
	BOOL GetCenterAndRadius();
	BOOL Clone(CEX2DVectObj **copy);
	BOOL LoadDxfFile(CString filename);
	BOOL Scale(float size);
	float MagnitudeSquared(float Mx1,float My1,
									  float Mz1,float Mx2,
									  float My2,float Mz2);
	void Serialize( CArchive& ar );
	
};

class  CEX2DVectObjList : public CObList
{
public:
   CEX2DVectObjList(){};
    
     
    DECLARE_SERIAL( CEX2DVectObjList )
    
    
};

#endif
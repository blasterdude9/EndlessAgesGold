#ifndef CHUDCLASS_H
#define CHUDCLASS_H

#include "C2DVectorDrawClass.h"
#include "CTextClass.h"
#include "CMovementModel.h"

//Unit Diplay Class
class  CHudDisplayObj : public CObject
{
	
     
	DECLARE_SERIAL( CHudDisplayObj );
	
    
public:
//variables
CString            List_uniText;
int                List_unitType;
CEX2DVectObj*      List_vectorObj;
int                List_posX;
int                List_posY;
int                List_fontIndex;
float              List_uniFloat;
int                List_uniInt;
int                List_penRed;
int                List_penGreen;
int                List_penBlue;
int                List_textRed;
int                List_textGreen;
int                List_textBlue;
int                List_brushRed;
int                List_brushGreen;
int                List_brushBlue;
int                List_lineThickness;
//functions
    CHudDisplayObj();
	void SafeDelete();
	void Serialize( CArchive& ar );
};
class  CHudDisplayList : public CObList
{
public:
   CHudDisplayList(){};
    
     
	DECLARE_SERIAL( CHudDisplayList );
	
    
};
//Main Hud Class
class  CHudObj : public CObject
{
	
     
	DECLARE_SERIAL( CHudObj );
	
    
	CHudObj(){}
public:
    CHudObj(CString hudName);               
    CString              List_HudCollectionName;
	CHudDisplayList *    List_displayUnitList;
         
    void SafeDelete();
	CString ConvToString(int number);
	CString FloatToStingCnv(float number);
	BOOL GetScreenPos(int *ScrnPosHorz, int *ScrnPosVert, int HieghtRes,
                      int WidthRes,double ClpPlaneLeft,double ClpPlaneBottom,
                      double ClpPlaneRight,double ClpPlaneTop,
                      double ClpPlaneDist,D3DVECTOR My3DPnt,float Aspect);
	BOOL AddUnit(int uniInt,
		         int type,
				 int posX,
				 int posY,
				 int fontIndex,
				 float uniFloat,
				 CString uniString,
				 BOOL LoadDxfFile,
			     int penRed,
		         int penGreen,
		         int penBlue,
		         int textRed,
		         int textGreen,
		         int textBlue,
		         int brushRed,
		         int brushGreen,
		         int brushBlue,
				 int LineThickness
				 );
	float MagnitudeSquared(float Mx1,float My1,
									  float Mz1,float Mx2,
									  float My2,float Mz2);
	BOOL BoundingSphereCheck(float radius1,float radius2,float x1,float y1,float z1,
										float x2,float y2,float z2);
	void Serialize( CArchive& ar );	
};

class  CHudObjList : public CObList
{
public:
   CHudObjList(){};
   
   void SafeDelete();
	DECLARE_SERIAL( CHudObjList );
	
    
};
#endif
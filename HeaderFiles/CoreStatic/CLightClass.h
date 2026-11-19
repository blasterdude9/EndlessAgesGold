#include "CFrameObj.h"
#include "CLightTrackerClass.h"

#ifndef CLIGHTCLASS_H
#define CLIGHTCLASS_H

class  CLightObj : public CObject
{
	CLightObj(){}
	
     
	DECLARE_SERIAL( CLightObj );
	
    
public:
	
	CLightObj(
		          BOOL  L_linearTypeAtten,
		          BOOL  L_ficker,
				  int L_flickerOnRange,
		          float L_atenuationAdjustVarial,
		          float L_attenuationVarial,
		          int   L_ocilatingVarial,
		          float L_colorRed,
				  float L_colorGreen,
				  float L_colorBlue,
				  float L_attenuation,
				  float L_atenuationAdjust,
                  BOOL  L_ocilating,
				  int   L_ocilatingSteps,
                  int   L_delay,
				  int   L_delayVarial,
				  BOOL  L_on,
				  //LPDIRECT3DRMLIGHT L_light,
				  CFrameObj* L_lightFrame,
				  float L_range
		          );

                  D3DLIGHT8 List_imStruct;
				  int List_imLight;
	              float List_range;
                  BOOL  List_linearTypeAtten;
	              int   List_flickerOnRange;
	              BOOL  List_ficker;
	              float List_atenuationAdjustVarial;
	              float List_attenuationVarial;
	              int   List_ocilatingVarial;
	              CFrameObj* List_lightFrame;
	              //LPDIRECT3DRMLIGHT List_light;
	              float List_colorRed;
				  float List_colorGreen;
				  float List_colorBlue;
				  float List_attenuation;
				  float List_atenuationAdjust;
                  BOOL  List_ocilating;
				  int   List_ocilatingSteps;
                  int   List_delay;
				  int   List_delayVarial;
				  BOOL  List_on;
	

	    void SetLightPositionForRender(LPDIRECT3DDEVICE8 device,D3DVECTOR offset);
	    void Serialize( CArchive& ar );
		void SafeDelete(LPDIRECT3DDEVICE8 device,
						CLightTrackerObjList * lightTrackList);

};

class  CLightObjList : public CObList
{
public:
   CLightObjList(){};
   
    
   DECLARE_SERIAL( CLightObjList )
   
   
};


#endif
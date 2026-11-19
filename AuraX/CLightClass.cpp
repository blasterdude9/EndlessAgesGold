#include "stdafx.h"
#include "resource.h"
#include <afxtempl.h>
#include <d3d8.h>
#include "CLightClass.h"


//DELETION
void CLightObj::SafeDelete(LPDIRECT3DDEVICE7 device,
						   CLightTrackerObjList * lightTrackList)
{
if(!List_lightFrame)
   return;
List_lightFrame->SafeDelete();
delete List_lightFrame;
List_lightFrame=0;
lightTrackList->LightHasBeenRemoved(List_imLight);
device->LightEnable(List_imLight, FALSE );
}
//set light poistion
void CLightObj::SetLightPositionForRender(LPDIRECT3DDEVICE7 device,D3DVECTOR offset)
{
    if(!List_on)
	    return;

    D3DVECTOR currentPos;
	List_lightFrame->GetPosition(NULL,&currentPos);  
	currentPos.x -= offset.x;
	currentPos.y -= offset.y;
	currentPos.z -= offset.z;
    List_imStruct.dvPosition = currentPos;             
    device->SetLight( List_imLight, &List_imStruct);
}


//CONSTRUCTION
CLightObj::CLightObj(
				  BOOL  L_linearTypeAtten,
				  BOOL  L_ficker,
				  int   L_flickerOnRange,
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
				  CFrameObj* L_lightFrame,
				  float L_range
		          )
{
List_range = L_range;
List_linearTypeAtten = L_linearTypeAtten;
List_flickerOnRange = L_flickerOnRange;
List_ficker = L_ficker;
List_flickerOnRange = L_flickerOnRange;
List_atenuationAdjustVarial = L_atenuationAdjustVarial;
List_attenuationVarial = L_attenuationVarial;
List_ocilatingVarial = L_ocilatingVarial;
List_lightFrame = L_lightFrame;
//List_light = L_light;
List_colorRed = L_colorRed;
List_colorGreen = L_colorGreen;
List_colorBlue = L_colorBlue;
List_attenuation = L_attenuation;
List_atenuationAdjust = L_atenuationAdjust;
List_ocilating = L_ocilating;
List_ocilatingSteps = L_ocilatingSteps;
List_delay = L_delay;
List_delayVarial = L_delayVarial;
List_on = L_on;
List_imLight=-1;
ZeroMemory( &List_imStruct, sizeof(D3DLIGHT7) ); 
}

void CLightObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
	   ar << List_colorRed
          << List_colorGreen
          << List_colorBlue
          << List_attenuation
          << List_atenuationAdjust
          << List_ocilating
          << List_ocilatingSteps
          << List_delay
          << 0//List_delayVarial
          << List_on
		  << 0//List_ocilatingVarial;
		  << List_ficker
		  << List_flickerOnRange
		  << List_linearTypeAtten
		  << List_range;
	}
	else
	{
       ar >> List_colorRed
          >> List_colorGreen
          >> List_colorBlue
          >> List_attenuation
          >> List_atenuationAdjust
          >> List_ocilating
          >> List_ocilatingSteps
          >> List_delay
          >> List_delayVarial
          >> List_on
		  >> List_ocilatingVarial
		  >> List_ficker
		  >> List_flickerOnRange
		  >> List_linearTypeAtten
		  >> List_range;

	      ZeroMemory( &List_imStruct, sizeof(D3DLIGHT7) ); 
	      List_atenuationAdjustVarial=0;
	      List_delayVarial=0;
	      List_attenuationVarial= 0;
	      List_attenuationVarial = 0;
	      List_lightFrame=0;
		  List_ocilatingVarial=0;
		  //List_light=0;
		  List_imLight=-1;

	}
}

IMPLEMENT_SERIAL(CLightObj, CObject, 0)
IMPLEMENT_SERIAL(CLightObjList, CObList, 0)
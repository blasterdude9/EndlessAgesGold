#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>

#include "CNetworkRecordClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CNetworkKeyObj::CNetworkKeyObj()
{
List_structType           = -1;//default illegal msg
CString temp              ="EMPTY";
List_uniBool              = TRUE;
List_uniInt               = 0;
List_uniInt2              = 0;
List_messageType          = 0;
List_Xpos                 = 0.0f;
List_Ypos                 = 0.0f;
List_Zpos                 = 0.0f;
List_OrientateDirX        = 0.0f;
List_OrientateDirY        = 0.0f;
List_OrientateDirZ        = 0.0f;
List_OrientateUpX         = 0.0f;
List_OrientateUpY         = 0.0f;
List_OrientateUpZ         = 0.0f;
List_FreeLookPitch        = 0.0f;
List_FreeLookYaw          = 0.0f;
List_idRefrence           = NULL;
    
lstrcpy(List_uniText,temp);
for(int i=0;i<4;i++)
 for(int j=0;j<4;j++)
  List_matrixX[i][j]=0.0f;

List_attribute            = 0;
		
List_index                = 0;
List_networkID            = 0;
	
List_waveOptionIndex      = 0;
List_speachRadius         = 0.0f;	
lstrcpy(List_chatMsg,temp);
lstrcpy(List_username,temp);

}





void CNetworkKeyObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
		  
       ar << List_structType
          << List_uniBool 
          << List_uniInt 
          << List_uniInt2 
          << List_messageType
          << List_Xpos
          << List_Ypos 
          << List_Zpos
          << List_OrientateDirX
          << List_OrientateDirY
          << List_OrientateDirZ
          << List_OrientateUpX  
          << List_OrientateUpY  
          << List_OrientateUpZ 
          << List_FreeLookPitch 
          << List_FreeLookYaw  
          << List_idRefrence 
	      << List_attribute
          << List_index
          << List_networkID
          << List_waveOptionIndex 
          << List_speachRadius;
          
          int loop=0;
	      for(loop=0;loop<25;loop++)
	          ar << List_uniText[loop];
		  for(loop=0;loop<50;loop++){
	          ar << List_chatMsg[loop]
                 << List_username[loop];
		  }

       for(int i=0;i<4;i++)
         for(int j=0;j<4;j++)
         ar << List_matrixX[i][j];
	}
	else
	{
       ar >> List_structType
          >> List_uniBool 
          >> List_uniInt 
          >> List_uniInt2 
          >> List_messageType
          >> List_Xpos
          >> List_Ypos 
          >> List_Zpos
          >> List_OrientateDirX
          >> List_OrientateDirY
          >> List_OrientateDirZ
          >> List_OrientateUpX  
          >> List_OrientateUpY  
          >> List_OrientateUpZ 
          >> List_FreeLookPitch 
          >> List_FreeLookYaw  
          >> List_idRefrence 
	      >> List_attribute
          >> List_index
          >> List_networkID
          >> List_waveOptionIndex 
          >> List_speachRadius;

	      int loop=0;
          for(loop=0;loop<25;loop++)
	          ar >> List_uniText[loop];
		  for(loop=0;loop<50;loop++){
	          ar >> List_chatMsg[loop]
                 >> List_username[loop];
		  }

       for(int i=0;i<4;i++)
         for(int j=0;j<4;j++)
         ar >> List_matrixX[i][j];
	}
}

IMPLEMENT_SERIAL(CNetworkKeyObj, CObject, 0)
IMPLEMENT_SERIAL(CNetworkKeyObjList, CObList, 0)
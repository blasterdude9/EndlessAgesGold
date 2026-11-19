#ifndef CNETWORKKEYCLASS_H
#define CNETWORKKEYCLASS_H

#include "dplay.h"

//object attributes for animation managment
class   CNetworkKeyObj : public CObject
{
	DECLARE_SERIAL( CNetworkKeyObj );
public:
	CNetworkKeyObj();
    //----variables----//
	int        List_structType;//0=movment message 1=attribute 2=firemsg 3=item 4=itemlocation 5=text comm  6=world change

	
	BOOL       List_uniBool;
	int        List_uniInt;
	int        List_uniInt2;
	int        List_messageType;
	float      List_Xpos;
	float      List_Ypos;
	float      List_Zpos;
	float      List_OrientateDirX;
	float      List_OrientateDirY;
	float      List_OrientateDirZ;
	float      List_OrientateUpX;
	float      List_OrientateUpY;
	float      List_OrientateUpZ;
	float      List_FreeLookPitch;
	float      List_FreeLookYaw;
	DPID       List_idRefrence;
    
	char       List_uniText[25]; 
	float      List_matrixX[4][4];

    int        List_attribute;
		
	int        List_index;
	int        List_networkID;
	
	int        List_waveOptionIndex;
    float      List_speachRadius;	
    char       List_chatMsg[50];

	char       List_username[50];
    
	void Serialize( CArchive& ar );

};

class   CNetworkKeyObjList : public CObList
{
public:
   CNetworkKeyObjList(){};
   DECLARE_SERIAL( CNetworkKeyObjList )
};

#endif
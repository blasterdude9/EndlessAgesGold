#ifndef CSTRUCTURES_H
#define CSTRUCTURES_H

#include "dplay.h"
#include "dplobby.h"
#include "d3d9.h"
#include <D3DX9.h>
#include <map>
#include "Common.h"
using namespace std;

//includes for some variables in structs

//#include "viewportclass.h"

//--------------
//Misc Structures
//--------------
typedef struct _A3DMATRIX {
     float        _11, _12, _13, _14;
     float        _21, _22, _23, _24;
     float        _31, _32, _33, _34;
     float        _41, _42, _43, _44;

     float m[4][4];
    _A3DMATRIX() { }
    _A3DMATRIX( float _m00, float _m01, float _m02, float _m03,
                float _m10, float _m11, float _m12, float _m13,
                float _m20, float _m21, float _m22, float _m23,
                float _m30, float _m31, float _m32, float _m33
        )
        {
                m[0][0] = _m00; m[0][1] = _m01; m[0][2] = _m02; m[0][3] = _m03;
                m[1][0] = _m10; m[1][1] = _m11; m[1][2] = _m12; m[1][3] = _m13;
                m[2][0] = _m20; m[2][1] = _m21; m[2][2] = _m22; m[2][3] = _m23;
                m[3][0] = _m30; m[3][1] = _m31; m[3][2] = _m32; m[3][3] = _m33;
        }
} A3DMATRIX, *LPA3DMATRIX;


struct TXTUV
{
    float tu,tv;
};

struct CMISCDATASTRUCT
{
    signed char m_catagorie;
	signed char m_miscByte1;
    signed char m_miscByte2;
	signed char m_miscByte3;
	short m_datasize;
    short m_miscShort;
};

struct ABDVECTOR
{
    double x;
	double y;
	double z;
};

struct SHADOWVERTEX
{
    D3DXVECTOR4 p;
    D3DCOLOR    color;

    //static const DWORD FVF;
};
//const DWORD SHADOWVERTEX::FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

struct ABVERTEX
{
    float x, y, z;
	float nx, ny, nz;
    TXTUV tx0;
	TXTUV tx1;
	TXTUV tx2;
	TXTUV tx3;
	TXTUV tx4;
	TXTUV tx5;
	TXTUV tx6;
	TXTUV tx7;
	float tx, ty, tz;
};

struct D3DVERTEX
{
    float x, y, z;
	float nx, ny, nz;
    float tu,tv;
};


struct ABVERTEX0L
{
    float x, y, z;
	float nx, ny, nz;
};

struct ABVERTEX0La
{
    float x, y, z;
	float nx, ny, nz;
	D3DCOLOR diffuse;
};

struct ABVERTEX0Lb
{
    float x, y, z;
	float nx, ny, nz;
	float tX, tY, tZ;
};

struct ABVERTEX1L
{
    float x, y, z;
	float nx, ny, nz;
    TXTUV tx0;
};

struct ABVERTEX1LNT
{
    float x, y, z;
    TXTUV tx0;
};

struct ABVERTEX1La
{
    float x, y, z;
	float nx, ny, nz;
	D3DCOLOR diffuse;
    TXTUV tx0;	
};

struct ABVERTEX1Lb
{
    float x, y, z;
	float nx, ny, nz;
    TXTUV tx0;
	float tX, tY, tZ;	
};

struct ABVERTEX2L
{
    float x, y, z;
	float nx, ny, nz;
    TXTUV tx0;
	TXTUV tx1;
};

struct ABVERTEX2La
{
    float x, y, z;
	float nx, ny, nz;
	D3DCOLOR diffuse;
    TXTUV tx0;
	TXTUV tx1;	
};

struct ABVERTEX1Lx
{
    float x, y, z;
	float ox,oy,oz;
	D3DCOLOR diffuse;
    TXTUV tx0;
	TXTUV tx1;	
};

struct ABVERTEX2Lb
{
    float x, y, z;
	float nx, ny, nz;
    TXTUV tx0;
	TXTUV tx1;	
	float tX, tY, tZ;	
};

struct ABVERTEX3L
{
    float x, y, z;
	float nx, ny, nz;
    TXTUV tx0;
	TXTUV tx1;
	TXTUV tx2;
};

struct ABVERTEX3La
{
    float x, y, z;
	float nx, ny, nz;
	D3DCOLOR diffuse;
    TXTUV tx0;
	TXTUV tx1;
	TXTUV tx2;
	
};

struct ABVERTEX3Lb
{
    float x, y, z;
	float nx, ny, nz;
    TXTUV tx0;
	TXTUV tx1;
	TXTUV tx2;
	float tX, tY, tZ;	
};

struct ABVERTEX4L
{
    float x, y, z;
	float nx, ny, nz;
    TXTUV tx0;
	TXTUV tx1;
	TXTUV tx2;
	TXTUV tx3;
};

struct ABVERTEX4La
{
    float x, y, z;
	float nx, ny, nz;
	D3DCOLOR diffuse;
    TXTUV tx0;
	TXTUV tx1;
	TXTUV tx2;
	TXTUV tx3;
	
};

struct ABVERTEX4Lb
{
    float x, y, z;
	float nx, ny, nz;
    TXTUV tx0;
	TXTUV tx1;
	TXTUV tx2;
	TXTUV tx3;
	float tX, tY, tZ;	
};

struct ABVERTEX5L
{
    float x, y, z;
	float nx, ny, nz;
    TXTUV tx0;
	TXTUV tx1;
	TXTUV tx2;
	TXTUV tx3;
	TXTUV tx4;
};

struct ABVERTEX5Lb
{
    float x, y, z;
	float nx, ny, nz;
    TXTUV tx0;
	TXTUV tx1;
	TXTUV tx2;
	TXTUV tx3;
	TXTUV tx4;
	float tX, tY, tZ;
};

struct ABVERTEX6L
{
    float x, y, z;
	float nx, ny, nz;
    TXTUV tx0;
	TXTUV tx1;
	TXTUV tx2;
	TXTUV tx3;
	TXTUV tx4;
	TXTUV tx5;
};

struct ABVERTEX6Lb
{
    float x, y, z;
	float nx, ny, nz;
    TXTUV tx0;
	TXTUV tx1;
	TXTUV tx2;
	TXTUV tx3;
	TXTUV tx4;
	TXTUV tx5;
	float tX, tY, tZ;
};

struct ABVERTEX7L
{
    float x, y, z;
	float nx, ny, nz;
    TXTUV tx0;
	TXTUV tx1;
	TXTUV tx2;
	TXTUV tx3;
	TXTUV tx4;
	TXTUV tx5;
	TXTUV tx6;
};

struct ABVERTEX8L
{
    float x, y, z;
	float nx, ny, nz;
    TXTUV tx0;
	TXTUV tx1;
	TXTUV tx2;
	TXTUV tx3;
	TXTUV tx4;
	TXTUV tx5;
	TXTUV tx6;
	TXTUV tx7;
};

struct ABVERTEXONE
{
    float x, y, z;
	float nx, ny, nz;
    TXTUV tx0;
};

struct ABBOX
{
    float minX;
	float maxX;
    float minY;
	float maxY;
	float minZ;
	float maxZ;
};

struct ABSPECFACE
{
    float x;
	float y;
    float z;
	char sw;
};

struct INVECTOR
{
 int x, y, z;
};

typedef struct _BNDBOX {
        D3DVECTOR min, max;
}BNDBOX;
typedef BNDBOX *LPBNDBOX;

struct CPolyRetStruct
{
   D3DXVECTOR3 polygon[3];
};


struct CFaceStruct
{
    D3DVECTOR polygon[3];
	D3DVECTOR normal;
	//BNDBOX box;

	int m_objectIndexRef;
};



//--------------
//Network Structures
//--------------
typedef struct { //bytes alot 51
	signed char m_catagorie;
	char logInName[25];
	char logInPass[25]; 
} LOG_STRUCT, *LPLOG_STRUCT;

//OPTIMIZED FOR FIREING CALLS
typedef struct { //10 bytes
  signed char m_catagorie;
  short lookatPosX;  
  short lookatPosY; 
  short lookatPosZ;
  short fireQueIndex;
  short idRefrence;
  short indexer;
} FIRE_STRUCT, *LPFIRE_STRUCT;


//OPTIMIZED FOR ATTRIBUTE CALLS
typedef struct { //20 bytes
	signed char m_catagorie;
    short attribute;
	int   uniInt;
	int   uniInt2;
	short uniShort3;
	short CRCcheck;
	short idRefrence;
} ATTRIB_STRUCT, *LPATTRIB_STRUCT;

  
typedef struct { //
  signed char m_catagorie;
  short data; 
} PING_STRUCT, *LPPING_STRUCT;

//OPTIMIZED FOR ARM CALLS
typedef struct { //12 bytes
	signed char m_catagorie;
	signed char dupCount;//this will say how many in a container you must index before use of this type (mad for customizable stuff that ids can be the same)
    short index;
	short CRCcheck;
	short idRefrence;
} EQUIP_STRUCT, *LPEQUIP_STRUCT;

//Entity cfg pak
typedef struct { //12 bytes
	signed char m_catagorie;
	signed char m_isItem;
	signed char anim;
	signed char m_clanMember;
	signed char m_groupMember;
	int networkDefinedID;
    short dbCfg;
    short nameDataSize;
	short invenDataSize;
	short defCfgDataSize;
	short Xpos;
	short Ypos;
	short Zpos;
	short mount;
	int clanID;
	bool isAI;
} ENTITYCFG_HEADER, *LPENTITYCFG_HEADER;

typedef struct {
	signed char			m_catagorie;
	char				m_itemName[23];
	char				m_locationID;
	char				m_animationAccess;//anim pref
	char				m_defaultAnimation;
	char				m_shotsPerFire;
	char				m_chargeType;
	char				m_useType;
	char				m_attachmentCount;
	char				m_damageElementOvrd;
	unsigned short		m_fireQueOvrd;
	unsigned short		m_damageOvrd;
	unsigned short		m_visualRef;
	unsigned short		m_glid;
	unsigned short		m_maxCharges;
	unsigned short		m_optNetID;
} DYNITEMCREATE, *LPDYNITEMCREATE;

typedef struct {
	short				m_visualID;
	short				m_fireQueOvrd;
	short				m_raceBind;
	char				m_dimIndex;
	char				m_dimSwitchTo;
}ITEMVISUAL, *LPITEMVISUAL;

//Entity position pak
typedef struct { //8 bytes
	signed char m_catagorie;
    short       m_entPosCount;//large for ai block updates
	short       m_staticCount;
	short       m_houseMsgCount;
	signed char m_fireMsgCount;
	signed char m_entitCfgMsgCount;
	signed char m_itemCfgMsgCount;
	signed char m_damageInfoMsgCount;
	signed char m_generalMsgCount;
	signed char m_detailedMsgCount;
	//signed char m_houseMsgCount;
} ENTITYPOS_HEADER, *LPENTITYPOS_HEADER;

typedef struct { //8 bytes
	signed char m_catagorie;
    int         m_entPosCount;//large for ai block updates
	int         m_entitCfgMsgCount;
    int         m_worldChannel;
	int         m_damageMsgCount;
	int         m_detailedMsgCount;
} AIPOS_HEADER, *LPAIPOS_HEADER;

//Client to server guarenteed
typedef struct { //8 bytes
	signed char m_catagorie;
	signed char m_attributeMsgCount;
    signed char m_dmgMsgCount;
    signed char m_txtMsgCount;
    signed char m_equipMsgCount;
    signed char m_updateStatMsgCount;
	signed char m_grpTxtMsgCount;
	signed char m_miscMsgCount;
} ENTITYGUPDATE_HEADER, *LPENTITYGUPDATE_HEADER;

typedef struct { //8 bytes
	int m_catagorie;
	int m_attributeMsgCount;
    int m_dmgMsgCount;
    int m_txtMsgCount;
    int m_equipMsgCount;
    int m_updateStatMsgCount;
	int m_grpTxtMsgCount;
	int m_miscMsgCount;
	int m_fireMsgCount;
} AI_ENTITYGUPDATE_HEADER, *AI_LPENTITYGUPDATE_HEADER;

//OPTIMIZED FOR MOVING  56 bytes //packet is never sent
typedef struct { //
	signed char m_catagorie;
	float Xpos;
	float Ypos;
	float Zpos;
	float OrientateDirX;
	float OrientateDirY;
	float OrientateDirZ;
	float OrientateUpX;
	float OrientateUpY;
	float OrientateUpZ;
	float FreeLookPitch;
	float FreeLookYaw;
	//short dbIndex;
	short curAnim;
	short secondaryAnim;
	short idRefrence;
	
} MOVE_STRUCT, *LPMOVE_STRUCT;

typedef struct {           //24 bytes currently
	signed char m_catagorie;
	signed char curAnim;
	signed char secondaryAnim;
	signed char OrientateDirX;   
	signed char OrientateDirY;
	signed char OrientateDirZ;
	signed char OrientateUpX;   
	signed char OrientateUpY;
	signed char OrientateUpZ;
	signed char FreeLookPitch;         //pitch
	//signed char FreeLookYaw;           //yaw	
	//short dbIndex;
    //short CRCcheck;
	short idReference;
	float Xpos;  
	float Ypos;
	float Zpos;
} SHRUNK_MOVE_STRUCT, *SHRUNK_LPMOVE_STRUCT; //LP variable 

//to server beta struct
typedef struct {           //28 bytes currently
	signed char m_catagorie;
	signed char curAnim;
	signed char secondaryAnim;
	signed char OrientateDirX;   
	signed char OrientateDirY;
	signed char OrientateDirZ;
	signed char OrientateUpX;   
	signed char OrientateUpY;
	signed char OrientateUpZ;
	signed char FreeLookPitch;         //pitch
	//signed char FreeLookYaw;           //yaw	
	//short dbIndex;
    //short CRCcheck;
	float Xpos;  
	float Zpos;
	short idReference;
	long  tmel;
	//short indexer;
	float Ypos;
	float CRCCHECK;
	
}SHRUNK_TOSVRMOV_STRUCT, *LPSHRUNK_TOSVRMOV_STRUCT; //LP variable 

//AI block Update
typedef struct {   
	signed char m_catagorie;
} AI_BLOCKPOSUPDATE_STRUCT, *AI_LPBLOCKPOSUPDATE_STRUCT; 
//GLID item block Update
typedef struct {   
	signed char m_catagorie;
	short       m_miscInt;
} BLOCKITEMLIST_STRUCT, *LPBLOCKITEMLIST_STRUCT; 

typedef struct {   
	int         m_acID;
	signed char m_open;
} GENERICCONFIG_STRUCT, *LPGENERICCONFIG_STRUCT; 

struct ITEMDATA_STRUCT 
{   
public:
	ITEMDATA_STRUCT()
	{
		this->m_GLID = -1;
		this->m_quantity = 0;
		this->m_dMod = 0;
		this->m_armed = 0;
	}

	bool ITEMDATA_STRUCT::operator==(const ITEMDATA_STRUCT &rhs) const
	{
		if ((this->m_GLID == rhs.m_GLID)
		&& (this->m_quantity == rhs.m_quantity)
		&& (this->m_dMod == rhs.m_dMod)
		&& (this->m_armed == rhs.m_armed))
			return true;
		return false;
	}

	ITEMDATA_STRUCT& operator=(ITEMDATA_STRUCT &temp)
	{
       CopyMemory(this, &temp, sizeof(ITEMDATA_STRUCT));
	   return *this;
	}

	short       m_GLID;
	short       m_quantity;
	signed char m_dMod;
	char		m_armed;
}; 

struct ITEMDATA_STRUCT_EX : public ITEMDATA_STRUCT
{   
public:
	ITEMDATA_STRUCT_EX()
	{
		__super::ITEMDATA_STRUCT();
		this->m_index = 0;
		this->m_received = false;
	}

	ITEMDATA_STRUCT_EX(ITEMDATA_STRUCT item)
	{
		this->m_index = 0;
		this->m_GLID = item.m_GLID;
		this->m_quantity = item.m_quantity;
		this->m_dMod = item.m_dMod;
		this->m_armed = item.m_armed;
		this->m_received = false;
	}

	bool ITEMDATA_STRUCT_EX::operator==(const ITEMDATA_STRUCT_EX &rhs) const
	{
		if ((this->m_index == rhs.m_index)
		&& (this->m_GLID == rhs.m_GLID)
		&& (this->m_quantity == rhs.m_quantity)
		&& (this->m_dMod == rhs.m_dMod)
		&& (this->m_armed == rhs.m_armed)
		&& (this->m_received == rhs.m_received))
			return true;
		return false;
	}

	ITEMDATA_STRUCT_EX& operator=(ITEMDATA_STRUCT_EX &rhs)
	{
       CopyMemory(this, &rhs, sizeof(ITEMDATA_STRUCT_EX));
	   return *this;
	}

	short m_index;
	bool m_received;
};

typedef struct {   
	short       m_GLID;
	short       m_quanity;
	int         m_price;
	signed char m_dMod;
} ITEMDATA_STRUCTEXT, *LPITEMDATA_STRUCTEXT; 

//ITEM CHECK PACKET //4 bytes
typedef struct {//from network to client only no crc needed
  signed char m_catagorie;
  short networkID;
} ITEM_CHECKTOCLIENT, *LPITEM_CHECKTOCLIENT;

//ITEM LOCATION PACKET //about 36 bytes 
typedef struct {
	signed char m_catagorie;
	signed char m_nameLength;
	signed char m_cfgLength;
	signed char m_itemArmedLength;
	signed char m_rotation;
	signed char m_autoTrigger;
	short dbIndex;
	short networkID;
	float Xpos;
	float Ypos;
	float Zpos;
	short curAnim;
    short mount;
	//entent this configuration/positional packet for complete transforms
	bool useExactOrientation; 
	signed char OrientateDirX;   
	signed char OrientateDirY;
	signed char OrientateDirZ;
	signed char OrientateUpX;   
	signed char OrientateUpY;
	signed char OrientateUpZ;
} ITEM_LOCATION, *LPITEM_LOCATION;

typedef struct {
	signed char m_catagorie;
	signed char m_nameLength;
	signed char m_cfgLength;
	signed char m_itemArmedLength;
	signed char m_rotation;
	signed char m_autoTrigger;
	short dbIndex;
	short networkID;
	short Xpos;
	float Ypos;
	short Zpos;
	short curAnim;
    short mount;
} ITEM_LOCATION_LEGACY, *LPITEM_LOCATION_LEGACY;

//TEXT MSG  //54 bytes
typedef struct {
    signed char m_catagorie;
    short       referenceID;
} TXT_MSG, *LPTXT_MSG;
//data Build text //31 bytes
typedef struct { 
	signed char m_catagorie;
	short CRCcheck;
	short dbIndex;
	char  worldName[25];
	int   currentTrace;
	short uniInt1;
	short uniInt2;
	short uniInt3;
} SPAWN_STRUCT, *LPSPAWN_STRUCT;
//data Build text //31 bytes
typedef struct { 
	signed char m_catagorie;
	short CRCcheck;
	short miscint;
	short messageType;
	char  uniText[25]; 
} MENUBUILD_STRUCT, *LPMENUBUILD_STRUCT;

typedef struct { 
	signed char m_catagorie;
	short messageType;
	short networkAssignedID;   
	char  uniText[25]; 
} NAME_STRUCT, *LPNAME_STRUCT;

//LOGON context packet 
typedef struct {
	signed char m_catagorie;
	char  username[25];
	char  password[25];
	short versionInfo;
	void * acMem;
} CONTEXT_VALIDATION, *LPCONTEXT_VALIDATION;

//Cast 
typedef struct {
	signed char m_catagorie;
} EXMSG_CATAGORIE, *LPEXMSG_CATAGORIE;

typedef struct {
	short m_data;
} SHORTTYPE_DATA, *LPSHORTTYPE_DATA;

typedef struct {
	signed char m_catagorie;
	short m_data;
} DATAGRAM4_DATA, *LPDATAGRAM4_DATA;

typedef struct {
	signed char m_catagorie;
	signed char m_data;
} MSGHANDLER, *LPMSGHANDLER;

typedef struct {
	signed char m_catagorie;
	int m_menuBind;
	int m_bankCash;
	int m_bankItemCount;
	int m_onCharCash;
} BANK_BLOCKHEADER, *LPBANK_BLOCKHEADER;

typedef struct {
	signed char m_catagorie;
} SVR_REJECTIONREPLY, *LPSVR_REJECTIONREPLY;

typedef struct {
	signed char m_catagorie;
	int m_menuBind;
	int m_playerCash;
	int m_storeItemCount;
	int m_storeCash;
} STORE_BLOCKHEADER, *LPSTORE_BLOCKHEADER;

typedef struct {
	signed char m_catagorie;
	short       m_menuLink;
} LOOTBLOCK_DATA, *LPLOOTBLOCK_DATA;

typedef struct {
	signed char m_catagorie;
	short       m_data;
	short       m_networkID;
	int         m_miscInt;
} COMMAND_DATA, *LPCOMMAND_DATA;

typedef struct {
	signed char m_catagorie;
	short       m_entityType;
	short       m_spawnCfg;
	short       m_nameLength;
	short       m_attachmentCount;
	short       m_defCfgCount;
} CREATECHARHEADER_DATA, *LPCREATECHARHEADER_DATA;

typedef struct {
	signed char m_catagorie;
	signed char m_type;//0=missile index 1=mele type 2=explosionIndex
	short       m_playerThatGotHitID;
	short       m_attackerNetowkID;
	short       m_miscValue;
	short       m_securityIndexer;
	short       m_targetX;
	short       m_targetY;
	short       m_targetZ;
	float       CRCCHECK;
} HITPLAYER_DATA, *LPHITPLAYER_DATA;

typedef struct {
	short       m_networAssignedID;
} STATICENTITY_DATA, *LPSTATICENTITY_DATA;

typedef struct {
	signed char m_catagorie;
	DWORD       m_timePassed;
	short       m_freqState;
} TIMESYNC_DATA, *LPTIMESYNC_DATA;

typedef struct {
	signed char m_catagorie;
    signed char m_stasis;
	signed char m_skillType;
	signed char m_level;
	signed char m_xpPerc;
	BOOL		m_facBonus1;
	BOOL		m_facBonus2;
	BOOL		m_facBonus3;
	short       m_hp;
	short       m_maxHp;
	short       m_str;
	short       m_dex;
	short       m_wis;
	int			m_curXP;
	int			m_maxXP;
	int			m_curFaction;
	int			m_rechargeTimer;
} STATBLKHEADER_DATA, *LPSTATBLKHEADER_DATA;

typedef struct {
	signed char m_catagorie;
	short       m_targetPlayerID;
	short       m_damageAmount;
	bool		m_death;
} DAMAGEINFO_DATA, *LPDAMAGEINFO_DATA;

typedef struct {
	signed char m_catagorie;
	short       m_itemAssignedID;
} BLOCKQUEST_DATA, *LPBLOCKQUEST_DATA;

typedef struct {
	signed char m_catagorie;
	short       m_menuID;
    short       m_glid;
} SKILLABILITIES_HEADER, *LPSKILLABILITIES_HEADER;

typedef struct {
	signed char m_catagorie;
	short       m_skillCount;
} SKILLS_HEADER, *LPSKILLS_HEADER;

typedef struct {
	signed char m_catagorie;
	short       m_data;
	short       m_miscInt;
	short       m_miscInt2;
	short       m_miscInt3;
} DETAILEDMSG_DATA, *LPDETAILEDMSG_DATA;

typedef struct {
	short       m_posX;
	short       m_posY;
	short       m_posZ;
	int         m_acID;
	short       m_houseType;
} HOUSE_DATA, *LPHOUSE_DATA;

typedef struct {
	signed char m_catagorie;
	signed char m_type;
	char  username[30];
	char  password[30];
	char  newUsername[30];
	char  newPassword[30];
} ADD_ACCOUNT, *LPADD_ACCOUNT;

typedef struct {
	float m_xPos;
	float m_yPos;
	short m_iconDBIndex;
} ICON_POS, *LPICON_POS;

typedef struct {
	signed char m_catagorie;
	int m_cashAmount;
} TRADE_HEADER, *LPTRADE_HEADER;

typedef struct { //10 bytes
  signed char catagorie;
  char ipaddress[20];
} RECONNECT_STRUCT, *LPRECONNECT_STRUCT;

typedef struct {
	signed char m_catagorie;
	short m_xPos;
	short m_yPos;
	short m_zPos;
	int   m_dynaID;
	short m_aiCfg;
	short m_dynaIDMaxSpawn;
	long  m_lifeLine;
	int   m_worldChannel;
	int   m_clanID;
	short m_spawnArrayCount;
	short m_spawnArrayOffset;
	char  worldName[25];
	int	  m_spawnRotationRange;
} AIDYNSPAWN_POS, *LPAIDYNSPAWN_POS;

typedef struct SQUAD_MESSAGE
{
public:
	SQUAD_MESSAGE()
	{
		this->NetID = 0;
		this->Health = 0;
		this->Position = 0;
		this->PlayerNameLength = 0;
		this->SquadNameLength = 0;
		this->PlayerName = NULL;
		this->SquadName = NULL;
	}

	~SQUAD_MESSAGE()
	{
		SafeDeleteArray(this->PlayerName);
		SafeDeleteArray(this->SquadName);
	}
	/*
	unsigned short m_playerNetID;
	unsigned char m_healthPercentage;
	unsigned char m_manaPercentage;
	*/
	unsigned short NetID;
	unsigned char Health;
	unsigned char Position;
	unsigned char PlayerNameLength;
	char *PlayerName;
	unsigned char SquadNameLength;
	char *SquadName;
} *LPSQUAD_MESSAGE;

///////////////////////////////////////////////////////////////////////////////
// Description:	These defines are used to identify arena- and quest-related response 
//	packets, since they all use the BLOCKQUEST_DATA response channel. 
//	These IDs are also used to assign the menu toggle group IDs for arena
//	registration, particularly the arena MODE and TYPE packet IDs.
///////////////////////////////////////////////////////////////////////////////

enum QUEST_PACKETS: unsigned char
{
	ARENA_UNSPECIFIED_ID = 0,
	ARENA_MODE_ID,
	ARENA_TYPE_ID,
	ARENA_INFO_ID,
	ARENA_JOIN_ID,
	ARENA_LEAVE_ID,
	QUEST_JOURNAL_ID
};

///////////////////////////////////////////////////////////////////////////////
// Enum: ARENA_MODES / ARENA_TYPES
// Description:	Describe avaialable MODEs and TYPEs of arenas battles.
///////////////////////////////////////////////////////////////////////////////

enum ARENA_MODES: unsigned char
{
	ARENAMODE_UNSPECIFIED = 0,
	ARENAMODE_SOLO,
	ARENAMODE_TEAM
};

enum ARENA_TYPES: unsigned char
{
	ARENATYPE_UNSPECIFIED = 0,
	ARENATYPE_PLAYERELIMINATION,
	ARENATYPE_MOBELIMINATION,
	ARENATYPE_DEATHMATCH,
	ARENATYPE_DOMINATION,
	ARENATYPE_MAX
};

enum ARENA_STATUS: unsigned char
{
	ARENASTATUS_NONE = 0,
	ARENASTATUS_COMMITTED,
	ARENASTATUS_ENGAGED
};

enum ARENA_SEVERITY: unsigned char
{
	AS_SUCCESS = 0,
	AS_WARNING = 1,
	AS_FAIL = 2
};

enum ARENA_RESULTS: unsigned char
{
	AR_SUCCESS = 0,
	AR_JOIN_FAILED = 1,
	AR_LEAVE_FAILED = 2,
	AR_COULD_NOT_FIND_ARENA_BOT = 4,
	AR_INVALID_PLAYER_OR_ACCOUNT = 8,
	AR_INSUFFICIENT_FUNDS_FOR_BETTING = 16
};

struct ArenaResult
{
	ARENA_SEVERITY Severity;
	ARENA_RESULTS Result;

	ArenaResult()
	{
		this->Result = AR_SUCCESS;
		this->Severity = AS_SUCCESS;
	}
};

// NOTE: These values can be found in CWorldChannelRules.h
const static int SOLO_ARENATYPES_TO_WORLDRULE_MAP[] =
{
	-1,	// UNSPECIFIED
	8,	// WORLDRULE_ELIMINATION
	12,	// WORLDRULE_ELIMINATEMOBS_ARENA
	14,	// WORLDRULE_DEATHMATCH
	16, // WORLDRULE_DOMINATION
	-1,	// UNSPECIFIED
};

// NOTE: These values can be found in CWorldChannelRules.h
const static int TEAM_ARENATYPES_TO_WORLDRULE_MAP[] =
{
	-1,	// UNSPECIFIED
	9,	// WORLDRULE_TEAM_ELIMINATION
	-1,	// WORLDRULE_ELIMINATEMOBS_ARENA
	15,	// WORLDRULE_TEAM_DEATHMATCH
	17, // WORLDRULE_TEAM_DOMINATION
	-1,	// UNSPECIFIED
};

// NOTE: These values can be found in Common.h
const static int ARENATYPES_SPLASH_IMAGES_ID_LOOKUP[] =
{
	-1,		// UNSPECIFIED
	327,	// ELIMINATION
	-1,		// ELIMINATEMOBS
	321,	// DEATHMATCH
	349,	// DOMINATION
	-1,		// UNSPECIFIED
};

///////////////////////////////////////////////////////////////////////////////
// Global Var:	ARENA_TYPES_MAP
// Description:	Used to store relationships between arena TYPE enum values and
//	their textual name, which are used for display and output to UI
//	menus.
///////////////////////////////////////////////////////////////////////////////

const static LPTSTR ARENA_TYPES_MAP[] =
{
	"No arenas.",
	"Player Elimination",
	"Mob Elimination",
	"Deathmatch",
	"Domination",
	"No arenas."
};

///////////////////////////////////////////////////////////////////////////////
// Global Var:	ARENA_TYPES_DESCRIPTIONS_MAP
// Description:	Used to store relationships between arena TYPE enum values and
//	their long description, which are used for display and output to UI
//	menus.
///////////////////////////////////////////////////////////////////////////////

const static LPTSTR ARENA_TYPES_DESCRIPTIONS_MAP[] =
{
	"No arena description available.",
	"Be the last smoking gun attached to a still breathing organism, and YOU will TRIUMPH! Team Elimination calls for the very LAST team standing upright. Nothing else will do. Lock and Load. Enjoy.",
	"This is a description of Mob Elimination arenas.",
	"Take names and rack up bodies, all in a blistering ten minutes of mayhem. Blow away unlucky players, within this short timeframe, for the honor of Single Mode Master! Shoot down the most targets in 10 minutes to rank as the Team Deathmatch Champions!",
	"If one was to cheat death and arrive at 10 direct kills, before all others, that one would truly be DOMINANT! Team Domination requires 10 fresh heads to claim the prize. Shorten your xmas list, with this easy to use process. Remember, Ammo spent is Time well spent.",
	"No arena description available."
};

///////////////////////////////////////////////////////////////////////////////
// Global Var:	ARENA_TYPES_DESCRIPTIONS_MAP
// Description:	Used to store relationships between arena TYPE enum values and
//	their long description, which are used for display and output to UI
//	menus.
///////////////////////////////////////////////////////////////////////////////

typedef std::pair<CString, int> ARENA_IMAGE_MAP_PAIR;

const static int NUM_ARENA_MAPS = 11;
const ARENA_IMAGE_MAP_PAIR ArenaImageMapPairs[NUM_ARENA_MAPS] =
{
	ARENA_IMAGE_MAP_PAIR("thegauntlet.exg", ID_ARENA_MAP_THE_GAUNTLET_IMAGE),
	ARENA_IMAGE_MAP_PAIR("theunderpond.exg", ID_ARENA_MAP_UNDERPOND_IMAGE),
	ARENA_IMAGE_MAP_PAIR("comstatarena.exg", ID_ARENA_MAP_COMSTAT_IMAGE),
	ARENA_IMAGE_MAP_PAIR("thecombine.exg", ID_ARENA_MAP_THE_COMBINE_IMAGE),
	ARENA_IMAGE_MAP_PAIR("tendrilcove.exg", ID_ARENA_MAP_TENDRIL_COVE_IMAGE),
	ARENA_IMAGE_MAP_PAIR("commandcentral.exg", ID_ARENA_MAP_COMMAND_CENTRAL_IMAGE),
	ARENA_IMAGE_MAP_PAIR("the_bar.exg", ID_ARENA_MAP_THE_BAR_IMAGE),
	ARENA_IMAGE_MAP_PAIR("thevista.exg", ID_ARENA_MAP_THE_VISTA_IMAGE),
	ARENA_IMAGE_MAP_PAIR("themitscher.exg", ID_ARENA_MAP_MITCHER_IMAGE),
	ARENA_IMAGE_MAP_PAIR("theskirmisher.exg", ID_ARENA_MAP_THE_SKIRMISHER_IMAGE),
	ARENA_IMAGE_MAP_PAIR("training.exg", ID_ARENA_MAP_COMSTAT_IMAGE)
};

const std::map<CString, int> 
ArenaImageMap(ArenaImageMapPairs, 
		  ArenaImageMapPairs + sizeof(ArenaImageMapPairs) / sizeof(ArenaImageMapPairs[0]));

///////////////////////////////////////////////////////////////////////////////
// Enum:		ARENA_MESSAGE_ID
// Description:	Types of arena events.
///////////////////////////////////////////////////////////////////////////////

enum ARENA_MESSAGE_ID : unsigned char
{
	ARENA_BEGIN = 0,
	ARENA_VICTORY,
	ARENA_DEFEAT,
	ARENA_KILLING_BLOW,
	ARENA_SET_MOST_WANTED,
	ARENA_REMOVE_MOST_WANTED,
	ARENA_SOLE_SURVIVOR,
	ARENA_MAX
};

///////////////////////////////////////////////////////////////////////////////
// Class:		ARENA_MESSAGE
// Description:	Used to communicate arena event messages to a player. This
//	struct is also used to relay arena stats to a player at the arena's close,
//	as a result of the player's loss or victory.
///////////////////////////////////////////////////////////////////////////////

typedef struct ARENA_MESSAGE
{
public:
	ARENA_MESSAGE()
	{
		ArenaMessage = ARENA_MAX; 
		ArenaType = 0;
		Score = 0;
		Kills = 0;
		Deaths = 0;
		Damage = 0;
		Reward = 0;
	}

	ARENA_MESSAGE_ID ArenaMessage; 
	unsigned int ArenaType;
	unsigned int Score;
	unsigned int Kills;
	unsigned int Deaths;
	unsigned int Damage;
	int Reward;
} *LPARENA_MESSAGE;

///////////////////////////////////////////////////////////////////////////////
// Class:		ArenaInfo / ArenaInfoEX
// Description:	Basic and extended arena info. Used during the arena registration
//	process.
///////////////////////////////////////////////////////////////////////////////

class ArenaInfo
{
public:
	ARENA_MODES Mode;
	ARENA_TYPES Type;

	ArenaInfo()
	{
		Mode = ARENAMODE_UNSPECIFIED;
		Type = ARENATYPE_UNSPECIFIED;
	}
};

class ArenaInfoEX : public ArenaInfo
{
public:
	CString Name;
	CString WorldName;
	unsigned short Index;
	unsigned int TimeReceived;
	unsigned int TimeRemainingForAutostart;
	unsigned short NumMinPlayers;
	unsigned short NumMaxPlayers;
	unsigned short NumCurrentPlayers;
	unsigned char NumTeams;
	unsigned short *NumPlayersOnTeam;
	unsigned int TimeLimit;
	unsigned int BetPool;

	ArenaInfoEX()
	{
		Index = 0;
		this->TimeReceived = ::GetTickCount();
		this->TimeRemainingForAutostart = 0;
		this->NumMinPlayers = 0;
		this->NumMaxPlayers = 0;
		this->NumCurrentPlayers = 0;
		this->NumTeams = 0;
		this->NumPlayersOnTeam = NULL;
		this->TimeLimit = 0; 
		this->BetPool = 0;
	}

	~ArenaInfoEX()
	{
		if(this->NumPlayersOnTeam)
		{
			delete[] this->NumPlayersOnTeam;
			this->NumPlayersOnTeam = 0;
		}
	}
};

//Global ID bound items misc use (no crc server use only)
/*typedef struct {
	short   gl_ID;
	short   quanity;
} MISC_GLIDUSE, *LPMISC_GLIDUSE;*/
////////////////////////////////////
//Nathan's
////////////////////////////////////
struct s_trigger : public CObject
{
 CString name;
 CString description;
 int type;
 //type:
 //0 - exit game
 //1 - start game
 //2 - save game
 //3 - load game
 //4 - exit menu
 //5 - change menu
 //6 - change cursor
 int trigger;
 int mouseButton;
 //trigger:
 //0 - mouse over
 //1 - mouse down
 //2 - mouse up
 //3 - mouse clicked
 //4 - mouse double clicked
 //5 - mouse wheel up
 //6 - mouse wheel down
 //mouseButton:
 //0 - left mouse button
 //1 - right mouse button
 //2 - middle mouse button
};





#endif

#pragma once
#include "sharednet.h"

class CDamageSystems
{
public:
	CDamageSystems(CSharedNet * sharedInterface);
	~CDamageSystems(void);

	//Vars
    CSharedNet * m_sharedInterfaceRef;

	//Functions
	BOOL HandleDamageMsgServerSide(LPHITPLAYER_DATA formatedMessage,int & msgSize,DPID & idFromLoc,int & curSimutanius,CPlayerObject * playerObjectData);
	BOOL HandleDamageMsgServerSide_safe(LPVOID lpMessage,int & msgSize,DPID & idFromLoc,int & curSimutanius);
	int AIXVerifyWeaponProjectile(CPlayerObject * playerObjectData, int fireQueIndexx, int fireQueOvrd);
	int AIXVerifyWeaponExplosion(CPlayerObject * playerObjectData, int explosionQueIndex);
	int AIXVerifyAttachment(CPlayerObject * playerObjectData, int attachment);
	float GetFinalDamageTweened(float baseDamage,int perfective,int profficiency,int damageRangeBaseMinPerc,int damageRangeBaseMaxPerc);
	int AIXVerifyDotAttachmentMissile(CPlayerObject *playerObjectData, int dotIndex);
	int AIXVerifyDotAttachmentMelee(CPlayerObject *playerObjectData, int dotIndex);
	int AddInPowerupResist(CPlayerObject *playerObjectData, int element, int armourRating);

	int WoundChannelCalculation(CPlayerObject *playerObjectData, int damageAmount, int damageChannel);

	BOOL EADamageCalculation(float & damageAmount,LPHITPLAYER_DATA formatedMessage, int msgSize, DPID idFromLoc, int curSimutanius, CPlayerObject *playerObjectData,int & scIncre);
	float ChronicleDamageCalculation(LPHITPLAYER_DATA formatedMessage, int msgSize, DPID idFromLoc, int curSimutanius, CPlayerObject *playerObjectData);
	float EATwoDamageCalculation(LPHITPLAYER_DATA formatedMessage, int msgSize, DPID idFromLoc, int curSimutanius, CPlayerObject *playerObjectData);

};

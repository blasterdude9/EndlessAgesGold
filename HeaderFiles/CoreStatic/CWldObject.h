#ifndef WLDOBJECT_H
#define WLDOBJECT_H
#include "CPolyObj.h"
//#include "CLightClass.h"
#include "CAttributeClass.h"
#include "CStructuresMisl.h"
#include <mmsystem.h>
#include "dsound.h"
//#include "CLightTrackerClass.h"
#include "CTriggerClass.h"
#include "CContainmentClass.h"
#include "CMeshLodClass.h"
#include "EnvParticleSystem.h"
#include "CSSoundRange.h"
#include "CNodeObject.h"
#include "CRTLightClass.h"
#include "CVertexAnimationModule.h"

class   CWldObject : public CObject
{
	CWldObject(){}
	
     
    DECLARE_SERIAL( CWldObject )
    
    
public:
	
	CWldObject(int GlobalTraceNumber);

	BNDBOX                  List_popoutBox;
	CPolyObj               *List_collisionInfoBuffer;
    BOOL                    List_removeEnviorment;
	int                     List_usedAsSpawnPoint;
	int                     List_miscInt;
	float                   List_miscFloat;
	int                     List_environmentAttached;
	IDirectSoundBuffer     *List_baseSoundBuffer;
	IDirectSound3DBuffer   *List_buffer3DControl;
	CString                 List_soundFileName;
	BOOL                    List_useSound;
	float                   List_soundFactor;
	BOOL                    List_isPlaying;
	float                   List_validRange;
	CAttributeObj          *List_attribute;
    int                     List_collisionType;
	//CLightObj *           List_lightObject;
	float                   List_popoutRadius;
	BOOL                    List_poppedOut;
    int                     List_identity;
	int                     List_groupNumber;
	BOOL                    List_showBox;
	CTriggerObjectList *    List_triggerList;
	CContainmentObject*     m_containmentObject;
	CMeshLODObjectList*     m_lodVisualList;
	CEXMeshObj*             m_meshObject;
	BOOL                    m_renderInMirrorTag;
	D3DVECTOR               m_particleDirection;
	EnvParticleSystem      *m_particleEmmiter;
	int                     m_haloLink;
	int                     m_lensFlareType;
	int                     m_particleLink;
	CString                 m_fileNameRef;
	BOOL                    m_collisionInfoFilter;
	BOOL                    m_renderFilter;
	CSoundRangeObject      *m_soundRange;
	int                     m_filterGroup;//-1 = none
	CNodeObj               *m_node;
	CRTLightObj            *m_lightObject;
	CVertexAnimationModule *m_animModule;
	int                     m_envReactionAttrib;
	int                     m_surfaceProperty;
	CCharonaObj            *m_charona;


	BOOL UpdateLods();
	void Serialize( CArchive& ar );
	BOOL DeleteTrigger(int index);
	void SafeDelete();

	void AddTrigger(int action,
				    int miscInt,
					float activationRadius,
					CString miscTxt,
					D3DVECTOR vectMisc,
					BOOL oneTimeOnly);

	BOOL EditTrigger(int index,
							 int action,
							 int miscInt,
							 float activationRadius,
							 CString miscTxt,
							 D3DVECTOR miscVect,
							 BOOL oneTimeOnly);

	BOOL BoundingSphereCheck(float radius1,float radius2,
		                     float x1,float y1,float z1,
						     float x2,float y2,float z2);
	void Move(D3DVECTOR translation,D3DVECTOR meshCenter);
	void Scale(D3DVECTOR scale,D3DVECTOR meshCenter);
	void Rotate(D3DVECTOR rotation,D3DVECTOR meshCenter);
	void CalibrateCenteredBoundingBox(int sightRange);
};

struct DirectAccessStructWLD
{
    CWldObject * object;
};


class   CWldObjectList : public CObList
{
public:
   CWldObjectList(){directAccess=NULL;directAccessObjectCount=0;};

   void CacheUnimediateData();
   void SafeDelete();
   DirectAccessStructWLD * directAccess;
   int                     directAccessObjectCount;
   
    
   void IntegrityCheck();
   void CompileDirectAccess();
   CEXMeshObj * GetMeshByName(CString meshname);
   CWldObject * GetByUnid(int id);
   DECLARE_SERIAL( CWldObjectList )
   

   
};

#endif




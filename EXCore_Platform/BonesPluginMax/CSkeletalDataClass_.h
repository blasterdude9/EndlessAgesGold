#ifndef CSKELETALDATACLASS_H
#define CSKELETALDATACLASS_H

#include "CBoneClass.h"
#include "cdeformablemeshclass.h"
//this is the base data export for the bones system without animations
struct DeformableMeshStructV2
{
    CDeformableMesh      * m_defMeshVisualUnit;//has internal LODs
};

class CSkeletalDataObj : public CObject
{
	DECLARE_SERIAL( CSkeletalDataObj );
public:
	
	CSkeletalDataObj();
	
	CString                m_SkeletalName;
	int                    m_frameSpeed;
	int                    m_ticksPerFrame;
    CBoneList            * m_bonesList;

    //temp data
	int                    m_physiqueModCount;//defines how many def meshes
	int                    m_firstFrame;
	int                    m_lastFrame;
	int                    m_frameCount;


	DeformableMeshStructV2*m_deformableMeshArrayV2;
	int                    m_deformableMeshArrayCount;
    
	void SafeDelete();
	void Serialize( CArchive& ar );
};

   
#endif
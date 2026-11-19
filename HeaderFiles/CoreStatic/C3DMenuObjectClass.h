#ifndef C3DMENUOBJECTCLASS_H
#define C3DMENUOBJECTCLASS_H
#include "CEXMeshClass.h"
#include "CSkeletalClass.h"
#include "CStructuresMisl.h"

class  C3DMenuObjectClass : public CObject
{
 public:

 C3DMenuObjectClass();
 ~C3DMenuObjectClass();
 
 CString m_name;
 CString m_fileName;

 BOOL m_hasText;
 CString m_text;

 BOOL m_hasMesh;
 CEXMeshObj *m_mesh;

 BOOL m_hasSkeleton;
 CSkeletonObject *m_skeleton;

 BOOL m_isDraggable;
 BOOL m_isSelectable;
 
 D3DVECTOR m_degRotation;
 D3DVECTOR m_position;
 D3DVECTOR m_rotationIncrement;

 DWORD m_lastTickCount;
 
 float m_scale;
 int m_blendMode;
 int m_actionID;
 int m_mouseTriggerID;
  
 void SafeDelete();
 void InitSkeleton(CString fileName);
 void InitMesh(CString fileName, LPDIRECT3D7 pD3D7);
 void InitText(CString text);
};

#endif
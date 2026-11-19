#include "StdAfx.h"
#include "servermovementsystems.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CServerMovementSystems::CServerMovementSystems(CSharedNet * sharedInterface)
{
	m_sharedInterfaceRef = sharedInterface;

}

CServerMovementSystems::~CServerMovementSystems(void)
{
}

//--------------Manage smooth movement of network entities :))))----------------------//
void CServerMovementSystems::InterpolateNetworkEntiesServerSide(CPlayerObject * pData,DWORD & CurrentTickCount)
{
	
	  DWORD networkInterp        = pData->m_interpolationVar;
	  DWORD interpolationCurStep = (CurrentTickCount - pData->m_timeLastUpdated);
	   
      
	  if(interpolationCurStep > networkInterp)
	     interpolationCurStep = networkInterp;
      
	  pData->m_3dPosition  = m_sharedInterfaceRef->m_cabLib.GL_matrixFunctions.InterpolatePoints(pData->m_Last3dPosition, 
						                                 pData->m_Target3dPosition,
					                                     networkInterp, 
					                                     interpolationCurStep);
     
      pData->m_dirVector   = pData->m_TargetDirVector;/*m_sharedInterfaceRef->m_cabLib.GL_matrixFunctions.InterpolatePoints(pData->m_LastDirVector, 
						                                  pData->m_TargetDirVector,
					                                      networkInterp, 
					                                      interpolationCurStep);*/
      
	  pData->m_upVector    = pData->m_TargetUpVector;/*m_sharedInterfaceRef->m_cabLib.GL_matrixFunctions.InterpolatePoints(
		                                                  pData->m_LastUpVector, 
						                                  pData->m_TargetUpVector,
					                                      networkInterp, 
					                                      interpolationCurStep);*/
	  

	  pData->m_upVector = m_sharedInterfaceRef->m_cabLib.GL_matrixFunctions.Normalize(pData->m_upVector);
	  pData->m_dirVector = m_sharedInterfaceRef->m_cabLib.GL_matrixFunctions.Normalize(pData->m_dirVector);
}
#pragma once
#include "menumanagement.h"
#include "globalstatusvariables.h"
#include "CInput.h"

class UserController
{
public:
	UserController(	HWND hwndLoc, 
					GlobalStatusVariables * gStatus,
					CMenuManagement * menuManagement,
					bool initDirectInput);
	~UserController(void);

	//Vars
	BOOL	m_restrictMovement;
	BOOL	m_lockControlSys;

	CInput	*	m_input;
	BOOL		m_ssScur;
    BOOL		m_lastMode;
	DWORD		m_lastQuickbarStamp;

	//Reference objects
	CMenuManagement			  * m_menuManagement;
	GlobalStatusVariables     * m_gStatus;

	DWORD						m_aquireTimeStamp;
	DWORD						m_npcQuertTimeStamp;

    //Functions
	void UserControlUpdate(CMovementObj * entityPtr, int controlConfigNum);
	void ClearInputStates();
	void MouseKeyboardUpdate();
	void ZoomIn(CMovementObj * ptrLoc,CCameraObj * camPtrRef);
	void ZoomOut(CMovementObj * ptrLoc,CCameraObj * camPtrRef);
	void ZoomManageMent(CMovementObj * ptrLoc,CCameraObj * camPtrRef);
	CString GetPossibleSymbols(int startIndex,CString curSection,int * totalCount);
	void MouseTurnLeft(CMovementCaps *movDB,float mouseControlXLoc);
    void MouseTurnRight(CMovementCaps *movDB,float mouseControlXLoc);	
    void MouseTurnPullUp(CMovementCaps *movDB,float mouseControlYLoc);	
    void MouseTurnDive(CMovementCaps *movDB,float mouseControlYLoc);
	//bool SetQuickItem(int BarID, int SlotID);
	void Accellerate(CMovementCaps *movDB,float overRide,float & timeAdjust);
	void Deccellerate(CMovementCaps *movDB);
	void SideStepRight(CMovementCaps *movDB);
	void SideStepLeft(CMovementCaps *movDB);
	void JumpMovementClassObj(CMovementObj * ptrLoc);
	void FreeLookRight(CMovementObj * ptrLoc);
	void FreeLookLeft(CMovementObj * ptrLoc);
	void FreeLookDown(CMovementObj * ptrLoc);
	void FreeLookUp(CMovementObj * ptrLoc);
	BOOL BailOutEntity(CMovementObj * currentEntity);
	BOOL ArmPrevWeapon(CMovementObj * movObj,BOOL override);
	BOOL ArmNextWeapon(CMovementObj * movObj,BOOL override);
    void ZeroForces(CMovementCaps *movDB);
    void TurnLeft(CMovementCaps *movDB);
    void TurnRight(CMovementCaps *movDB);
    void Dive(CMovementCaps *movDB);
    void PullUp(CMovementCaps *movDB);
    void RollLeft(CMovementCaps *movDB);
    void RollRight(CMovementCaps *movDB);
	BOOL GetJet(CMovementObj * ptrLoc,int fuelUse,D3DVECTOR & force);
	//BOOL ArmEntityItemSys(CMovementObj * mainPtr,int itemID);
	//BOOL DisarmEntityItemSys(CMovementObj * mainPtr,int itemID);
	void DecelTurn(CMovementCaps *movDB);
	void DecelPitch(CMovementCaps *movDB);
	void DecelRoll(CMovementCaps *movDB);
	void DecelSpeed(CMovementCaps *movDB);
	void DecelSideStep(CMovementCaps *movDB);
	BOOL RegenJet(CMovementObj * ptrLoc,int fuelGain);
	void AutoLevelEntity(CMovementObj * entPtr, float strengthX, float strengthZ, float tolerance);
	void GlobalControlsInLoop();
	BOOL AcquireInputDevices();
	void UnacquireInputDevices();
	void MouseManagerCallback();
	//void ProcessKeyboardBufferedData();
	CString ParseData(int msg);
	void ShowWinCursor(BOOL state);
	void AutoSteeringBasedOnAIObj(CMovementObj * ptrLoc,D3DVECTOR locationLoc,float & adjust);
	void PointClickController(CMovementObj * entityPtr);
	BOOL USER_AI_Transform(CMovementObj * ptrLoc,CWaterZoneObjList * m_waterZoneDB,double timeAdjust,double lastTimeAdjust);
	void ProcessKeyboardMouseButtons(CMovementObj * ptrLoc, 
				                     CMovementObj *entityPtr,
									 CMovementObj *lookfirePtr,
									 DWORD & currentTime,
									 BOOL & jetsWereUsed,
									 float & AdditionalForceX,
									 float & AdditionalForceY,
									 float & AdditionalForceZ,
									 BOOL restrictChat,
									 CCameraObj * camPtrRef,
									 ControlDBObj * conPtr,
									 BOOL filterMouseButtons);
	void ProcessChat(CMovementObj * ptrLoc,CMovementObj *entityPtr);
	void USER_AIFollowAttackTargetState(CMovementObj * ptrLoc,
					                    float & timeAdjust,
					                    CMovementObjList * entObjRuntime,
							            DWORD & currentTime,
										int & targetTrace);
	void USER_AIFireAtEnemy(CMovementObj * ptrLoc,
					        float & timeAdjust,
					        CMovementObjList * entObjRuntime,
					        DWORD & currentTime);
	void TargetingSystemCallback();
	void CoordinateRemapper( float & orthoX,
							 float & orthoY,
							 int & orthoScreenWidth,
							 int & orthoScreenHeight,
							 float & virtualWidth,
							 float & virtualHeight,
							 float * newX,
							 float * newY);
	void CoordinateCornerToCenter( float & Width,
							       float & Height,
							       float & posX,
							       float & posY);

private:
		bool	m_bIsJumping,
				m_bIsFlying,
				m_bToggleAutoRun,
				m_bIsDisplayed,
				m_bShowCursor;	
};

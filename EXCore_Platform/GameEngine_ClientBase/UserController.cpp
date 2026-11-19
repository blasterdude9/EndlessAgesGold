#include "stdafx.h"
#include "usercontroller.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UserController::UserController(HWND hwndLoc,
							   GlobalStatusVariables * gStatus,
							   CMenuManagement * menuManagement,
							   bool initDirectInput)
{
	m_lastQuickbarStamp = GetTickCount();
	m_lastMode          = -1;
    m_restrictMovement	= FALSE;
	m_gStatus			= gStatus;
	m_menuManagement	= menuManagement;
	m_lockControlSys	= FALSE;
	m_aquireTimeStamp	= GetTickCount();
	m_npcQuertTimeStamp	= GetTickCount();
	m_ssScur			= TRUE;
	m_input				= new CInput(&gStatus->m_dxRenderer->m_currentWidth,
									 &gStatus->m_dxRenderer->m_currentHeight);

	m_bIsJumping		= false;
	m_bIsFlying			= false;
	m_bToggleAutoRun	= false;
	m_bIsDisplayed		= false;
	m_bShowCursor		= false;

	ClearInputStates();

	if(!initDirectInput)
		return;

    HRESULT err;
	err = DirectInput8Create(AfxGetInstanceHandle(), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**) &m_input->m_directInput,NULL);
     //-------Keyboard....--------//
	if(err != S_OK)
	{
	   m_input->m_directInput = NULL;
	   return;
	}
	gStatus->m_inputReference = m_input;
	
}

UserController::~UserController(void)
{
	if(m_input)
		delete m_input;
	m_input = 0;
}

void UserController::UnacquireInputDevices()
{
	EnterCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
	if(m_input->m_directKeyboard)
		m_input->m_directKeyboard->Unacquire();

	if(m_input->m_directMouse)
		m_input->m_directMouse->Unacquire();

	 m_gStatus->m_dxRenderer->m_showMouse = TRUE;

    LeaveCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
}

BOOL UserController::AcquireInputDevices()
{
    if((GetTickCount()-m_aquireTimeStamp) < 500)
        return FALSE;

    m_gStatus->m_dxRenderer->m_showMouse = TRUE;

    m_aquireTimeStamp = GetTickCount();

	if(!m_input->m_directInput)
	    return FALSE;

    EnterCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);

	if(!m_input->m_directKeyboard)
	{
        ClearInputStates();
        if(m_input->m_directInput->CreateDevice(GUID_SysKeyboard,&m_input->m_directKeyboard, NULL) !=  S_OK)
		{
		   LeaveCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
		   return FALSE;
		}

		if(m_input->m_directKeyboard->SetCooperativeLevel(m_gStatus->m_dxRenderer->m_topWindowHandle, DISCL_FOREGROUND | DISCL_EXCLUSIVE | DISCL_NOWINKEY ) != S_OK)
		{
           if(m_input->m_directKeyboard)
		      m_input->m_directKeyboard->Release();
		   m_input->m_directKeyboard = 0;
		   LeaveCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
		   return FALSE;
		}

		if(m_input->m_directKeyboard->SetDataFormat(&c_dfDIKeyboard)==S_OK)
	    {
				DIPROPDWORD  dipdw; 
				dipdw.diph.dwSize = sizeof(DIPROPDWORD); 
				dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
				dipdw.diph.dwObj = 0; 
				dipdw.diph.dwHow = DIPH_DEVICE; 
				dipdw.dwData = 10; 
				if(m_input->m_directKeyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)!=S_OK)
				{
                   if(m_input->m_directKeyboard)
		              m_input->m_directKeyboard->Release();
		           m_input->m_directKeyboard = 0;
				   LeaveCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
                   return FALSE;
				}
		}
		else
		{
            if(m_input->m_directKeyboard)
		       m_input->m_directKeyboard->Release();
		   m_input->m_directKeyboard = 0;
		   LeaveCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
		   return FALSE;
		}
	}
		
    if(m_input->m_directKeyboard->Acquire()!= S_OK)
	{
		LeaveCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
		return FALSE;
	}
	//--------mouse---------------//
	if(!m_input->m_directMouse)
	{
	    if(m_input->m_directInput->CreateDevice(GUID_SysMouse, &m_input->m_directMouse, NULL) != S_OK)
		{
			LeaveCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
		   return FALSE;
		}
        
        #ifndef _FORCEMOUSE
		if(m_input->m_directMouse->SetCooperativeLevel(m_gStatus->m_dxRenderer->m_topWindowHandle, DISCL_FOREGROUND | DISCL_EXCLUSIVE | DISCL_NOWINKEY )!= S_OK)
        #endif
        #ifdef _FORCEMOUSE
		if(m_input->m_directMouse->SetCooperativeLevel(m_gStatus->m_dxRenderer->m_topWindowHandle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY )!= S_OK)
        #endif
		{
           if(m_input->m_directMouse)
		      m_input->m_directMouse->Release();
		   m_input->m_directMouse = 0;
		   LeaveCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
		   return FALSE;
		}
		
		if(m_input->m_directMouse->SetDataFormat(&c_dfDIMouse) != S_OK)
		{
           if(m_input->m_directMouse)
		      m_input->m_directMouse->Release();
		   m_input->m_directMouse = 0;
		   LeaveCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
		   return FALSE;
		}
	}


	if(m_input->m_directMouse->Acquire() != S_OK)
    {
	   LeaveCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
       return FALSE;
    }

	m_gStatus->m_dxRenderer->m_showMouse = FALSE;

	LeaveCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);

	return TRUE;
}
//------------------------------------------------------------------------------------/*/
/*void UserController::ProcessKeyboardBufferedData()
{
    if(!m_directKeyboard)
		return;

	DWORD curTime = m_gStatus->GetTimeStamp_DW();

	//Update the shift state
	m_shiftOn = m_keyboardKeys[DIK_LSHIFT].IsDown() || m_keyboardKeys[DIK_RSHIFT].IsDown();

	//Update capslock
	if(m_keyboardKeys[DIK_CAPSLOCK].JustPressed())
		m_capsOn = !m_capsOn;

	CString addition;
	for(int kLoop = 0; kLoop < KEYBOARD_MAX_KEYS; kLoop++)
	{
		if(m_keyboardKeys[kLoop].JustPressed() || m_keyboardKeys[kLoop].ShouldRepeat(curTime))
		{//key was pressed
			addition = ParseData(kLoop);
  
			if(m_gStatus->m_acceptConsoleTxt)
			{
				m_gStatus->m_consolTXT = m_gStatus->m_consolTXT + addition;//operator+(m_gStatus->m_chatBuild,TranslateCharValue(Msg.wParam));
				if(kLoop == DIK_BACKSPACE )
					m_gStatus->m_backspaceQued = TRUE;
			}
    

			if(GetTickCount()-m_gStatus->m_lastChatCommandStamp > 50)
			if(m_gStatus->m_multiplayerObject->m_chatEnabled == TRUE ||
			   m_gStatus->m_multiplayerObject->m_groupChatEnabled == TRUE ||
			   m_gStatus->m_multiplayerObject->m_clanChatEnabled == TRUE)
			{//retrieve text dont miss
				if(kLoop != DIK_BACKSPACE )
				     m_gStatus->m_chatBuild = m_gStatus->m_chatBuild + addition;//operator+(m_gStatus->m_chatBuild,TranslateCharValue(Msg.wParam));
				else
				     m_gStatus->m_chatDeleteString = m_gStatus->m_chatDeleteString + "x";
			}//end retrieve text dont miss
		}//end key was pressed
	}
}*/
//-----------------------------------------------------------------//
CString UserController::ParseData(int msg)
{
	BOOL capitalMode = m_input->m_shiftOn ^ m_input->m_capsOn;

	switch(msg)
	{
	   case DIK_A:
		 if(capitalMode)
            return "A";
		 else
            return "a";

	   case DIK_B:
	     if(capitalMode)
            return "B";
		 else
            return "b";

	   case DIK_C:
	     if(capitalMode)
            return "C";
		 else
            return "c";

	   case DIK_D:
	     if(capitalMode)
            return "D";
		 else
            return "d";

	   case DIK_E:
	     if(capitalMode)
            return "E";
		 else
            return "e";
  
       case DIK_F:
	     if(capitalMode)
            return "F";
		 else
            return "f";
 
       case DIK_G:
	     if(capitalMode)
            return "G";
		 else
            return "g";

	   case DIK_H:
	     if(capitalMode)
            return "H";
		 else
            return "h";

	   case DIK_I:
	     if(capitalMode)
            return "I";
		 else
            return "i";
	
	   case DIK_J:
	     if(capitalMode)
            return "J";
		 else
            return "j";

	   case DIK_K:
	     if(capitalMode)
            return "K";
		 else
            return "k";

	   case DIK_L:
	     if(capitalMode)
            return "L";
		 else
            return "l";
	
	   case DIK_M:
	     if(capitalMode)
            return "M";
		 else
            return "m";

	   case DIK_N:
	     if(capitalMode)
            return "N";
		 else
            return "n";

	   case DIK_O:
	     if(capitalMode)
            return "O";
		 else
            return "o";
	
	   case DIK_P:
	     if(capitalMode)
            return "P";
		 else
            return "p";

	   case DIK_Q:
	     if(capitalMode)
            return "Q";
		 else
            return "q";

	   case DIK_R:
	     if(capitalMode)
            return "R";
		 else
            return "r";

	   case DIK_S:
	     if(capitalMode)
            return "S";
		 else
            return "s";
	
	   case DIK_T:
	     if(capitalMode)
            return "T";
		 else
            return "t";

	   case DIK_U:
	     if(capitalMode)
            return "U";
		 else
            return "u";

	   case DIK_V:
	     if(capitalMode)
            return "V";
		 else
            return "v";
	
	   case DIK_W:
	     if(capitalMode)
            return "W";
		 else
            return "w";

	   case DIK_X:
	     if(capitalMode)
            return "X";
		 else
            return "x";

	   case DIK_Y:
	     if(capitalMode)
            return "Y";
		 else
            return "y";

	   case DIK_Z:
	     if(capitalMode)
            return "Z";
		 else
            return "z";

       case DIK_1:
	     if(m_input->m_shiftOn)
            return "!";
		 else
            return "1";

	   case DIK_2:
	     if(m_input->m_shiftOn)
            return "@";
		 else
            return "2";

	   case DIK_3:
	     if(m_input->m_shiftOn)
            return "#";
		 else
            return "3";

	   case DIK_4:
	     if(m_input->m_shiftOn)
            return "$";
		 else
            return "4";

	   case DIK_5:
	     if(m_input->m_shiftOn)
            return "%";
		 else
            return "5";

	   case DIK_6:
	     if(m_input->m_shiftOn)
            return "^";
		 else
            return "6";

	   case DIK_7:
	     if(m_input->m_shiftOn)
            return "&";
		 else
            return "7";

	   case DIK_8:
	     if(m_input->m_shiftOn)
            return "*";
		 else
            return "8";

	   case DIK_9:
	     if(m_input->m_shiftOn)
            return "(";
		 else
            return "9";

	   case DIK_0:
	     if(m_input->m_shiftOn)
            return ")";
		 else
            return "0";

	   case DIK_NUMPAD1:
		   return "1";

	   case DIK_NUMPAD2:
		   return "2";

	   case DIK_NUMPAD3:
		   return "3";

	   case DIK_NUMPAD4:
		   return "4";

	   case DIK_NUMPAD5:
		   return "5";

	   case DIK_NUMPAD6:
		   return "6";

	   case DIK_NUMPAD7:
		   return "7";

	   case DIK_NUMPAD8:
		   return "8";

	   case DIK_NUMPAD9:
		   return "9";

	   case DIK_NUMPAD0:
		   return "0";

	   case DIK_MINUS:
		   if(m_input->m_shiftOn)
            return "_";
		 else
            return "-";

	   case DIK_EQUALS:
	     if(m_input->m_shiftOn)
            return "+";
		 else
            return "=";


	   case DIK_NUMPADPLUS:
          return "+";

       case DIK_NUMPADMINUS:
          return "_";

	   
	   case DIK_NUMPADSLASH:
          return "/";


	   case DIK_NUMPADSTAR:
          return "*";
		  
	   
	   
	   case DIK_LBRACKET:
		   if(m_input->m_shiftOn)
            return "{";
		 else
            return "[";


	   case DIK_RBRACKET:
	     if(m_input->m_shiftOn)
            return "}";
		 else
            return "]";

	   case DIK_BACKSLASH:
		   if(m_input->m_shiftOn)
            return "|";
		 else
            return "\\";


	   case DIK_SEMICOLON:
	     if(m_input->m_shiftOn)
            return ":";
		 else
            return ";";

	   case DIK_APOSTROPHE:
		if(m_input->m_shiftOn)
		   return (CString)'"';
		else
           return "'";

	   case DIK_COMMA:
		if(m_input->m_shiftOn)
		   return "<";
		else
           return ",";
		 
	   case DIK_PERIOD:
		if(m_input->m_shiftOn)
		   return ">";
		else
           return ".";

	   case DIK_SLASH:
		if(m_input->m_shiftOn)
		   return "?";
		else
           return "/";

	   case DIK_GRAVE:
		if(m_input->m_shiftOn)
		   return "~";
		else
           return "`";

	   case DIK_SPACE:
		return " ";

   }

   return "";

}
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void UserController::UserControlUpdate(CMovementObj * entityPtr,
									   int controlConfigNum) 
{
    

	m_lastMode = 0;
	if(!m_gStatus->m_editorModeOn && m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
	{
		// connected: the in-game login window (if it was shown) is done
		if(m_gStatus->m_runtimeMenuDatabase && m_gStatus->m_runtimeMenuDatabase->GetVisualByID(EA_LOGIN_MENU_ID, false))
			m_gStatus->m_runtimeMenuDatabase->CloseByID(EA_LOGIN_MENU_ID, &m_gStatus->m_showMenuCursor);
	}
	if(!m_gStatus->m_editorModeOn && !m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
	{
		if(m_gStatus->m_multiplayerObject->m_attempConnectionEnabled == TRUE)
		{
			if(GetTickCount()-m_gStatus->m_multiplayerObject->m_attemptConnectionStamp > 20000)
			{
				if(m_gStatus->m_systemReadout)
				   m_gStatus->m_systemReadout->AddLine("Unable to connect to server.  Try again later.");
				m_gStatus->m_multiplayerObject->m_attempConnectionEnabled = FALSE;  
				m_gStatus->m_multiplayerObject->m_attemptConnectionStamp = GetTickCount();
			}
		}
		else
		{
/*			if(GetTickCount() - m_gStatus->m_multiplayerObject->m_attemptConnectionStamp > 3000)
			{
				static int holder = 0;
				CString defaultMessage = "Retrying Connection";
				if(m_gStatus->m_systemReadout)
				{
					if(!holder)
					{
						m_gStatus->m_systemReadout->AddLine("\n\n\n\n\n\n\n\n\n   ");
						m_gStatus->m_systemReadout->AddLine("\n\n\n\n\n\n\n\n\n   ");
						m_gStatus->m_systemReadout->AddLine("\n\n\n\n\n\n\n\n\n   ");
						m_gStatus->m_systemReadout->AddLine(defaultMessage);
						holder++;
					}
					else
					{
						m_gStatus->m_systemReadout->AddLine("\n\n\n\n\n\n\n\n\n   ");
						m_gStatus->m_systemReadout->AddLine("\n\n\n\n\n\n\n\n\n   ");
						m_gStatus->m_systemReadout->AddLine("\n\n\n\n\n\n\n\n\n   ");
						for(int i = 0; i < holder; i++)
							defaultMessage.Append(".");
						m_gStatus->m_systemReadout->AddLine(defaultMessage);
						holder++;

						if(holder == 10)
							holder = 0;
					}
				}
				m_gStatus->m_multiplayerObject->m_attemptConnectionStamp = GetTickCount();
			}
*/		}
	}
	else
	{
		static BOOL yay = FALSE;
		if(!yay)
		{
			if(m_gStatus->m_systemReadout)
			{
				m_gStatus->m_systemReadout->AddLine("\n\n\n\n\n\n\n\n\n   ");
				m_gStatus->m_systemReadout->AddLine("\n\n\n\n\n\n\n\n\n   ");
				m_gStatus->m_systemReadout->AddLine("\n\n\n\n\n\n\n\n\n   ");
				m_gStatus->m_systemReadout->AddLine("Connection Established.");
			}
			yay = TRUE;
		}
	}

	

	if(entityPtr == NULL)
		return;

	if(m_lockControlSys)
		return;

	CMovementObj * lookfirePtr	= NULL;
	DWORD currentTime			= GetTickCount();

	CMovementObj * ptrLoc = NULL;
	if(entityPtr->m_posses)
	{//possesing somthing
		ptrLoc = entityPtr->m_posses;//entityPtr->m_posses;
		lookfirePtr = entityPtr;
		//ptrLoc = entityPtr->m_posses;
		//lookfirePtr = entityPtr->m_posses;
	}//end possesing somthing
	else
	{
		ptrLoc = entityPtr;
		lookfirePtr = entityPtr;
	}

	//Reset the cursor, because it may have been changed in the Point-Click system
	m_gStatus->m_currentCursor = CURSOR_NORMAL;	
	
	m_restrictMovement = FALSE;
	//BOOL restrictChat = FALSE;

	if(entityPtr->List_movementDB->m_currentSurfaceProp == 1)
		entityPtr->List_movementDB->List_doGravity = FALSE;
	 
	bool menusHandledInput = m_menuManagement->HandleInput(m_input);
	

	if(!m_gStatus->m_editorModeOn && m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
	{//client app activated
		if(entityPtr->List_networkDefinedID > 0)
		{
			if((GetTickCount()-m_gStatus->m_multiplayerObject->GetLastCommunicationTime()) > m_gStatus->m_multiplayerObject->m_lagMSTolerance)
			{//lagged
				if(!m_gStatus->m_lagged)
				if(m_gStatus->m_systemReadout)
					m_gStatus->m_systemReadout->AddLine("Connection Interuption...");
				m_gStatus->m_lagged = TRUE;
				return;
			}//end lagged
		}
		else
			return;
	}//end client app activated
 
	if(m_gStatus->m_lagged)
	{
		if(m_gStatus->m_systemReadout)
			m_gStatus->m_systemReadout->AddLine("Connection Resumed...");
		m_gStatus->m_lagged = FALSE;
	}

	float doYaw = 0.0f;
	float doPitch = 0.0f;
	//ADDITIONAL ENTITY FORCES
	float AdditionalForceX = 0.0f;
	float AdditionalForceY = 0.0f;
	float AdditionalForceZ = 0.0f;
	//EXTERNAL FORCES
	float ExternalForceX = ptrLoc->List_externalVectorTemp.x;
	float ExternalForceY = ptrLoc->List_externalVectorTemp.y;
	float ExternalForceZ = ptrLoc->List_externalVectorTemp.z;
	ptrLoc->List_externalVectorTemp.x = 0.0f;
	ptrLoc->List_externalVectorTemp.y = 0.0f;
	ptrLoc->List_externalVectorTemp.z = 0.0f;

	if(entityPtr->List_energy > 0 && m_gStatus->m_globalAvVar == TRUE)
	{//alive
		m_gStatus->m_respawning       = FALSE;
		m_gStatus->m_respawnTimeStamp = currentTime;

		BOOL jetsWereUsed = FALSE;
		ptrLoc->List_movementDB->m_jetPackInUse = FALSE;

		float exponentBasis = 1.0f;
		//get device state
		CCameraObj * camPtrRef = m_gStatus->m_runtimeViewportDB->GetCameraObjectFromEntity(entityPtr);
		float mouseControlXLoc =(m_input->m_mouseHiResX * 3.0f) / m_gStatus->m_timeAdjustFloat;//(float)GL_MS.lX;
		float mouseControlYLoc =(m_input->m_mouseHiResY * 3.0f) / m_gStatus->m_timeAdjustFloat;//(float)GL_MS.lY;
  
        if(!m_gStatus->m_controlsList)
			return;

		POSITION posLoc = m_gStatus->m_controlsList->FindIndex(controlConfigNum);
		if(!posLoc)
		{
			m_gStatus->LoadControlCfgList("");
			posLoc = m_gStatus->m_controlsList->FindIndex(controlConfigNum);
		}
		if(!posLoc)
			return;
		ControlDBObj * conPtr = ( ControlDBObj* ) m_gStatus->m_controlsList->GetAt(posLoc);	

		if(!conPtr->m_invertMouse)//non inverted
			mouseControlYLoc = -mouseControlYLoc;

		ptrLoc->List_movementDB->SetUseStatesToFalse();

		BOOL altDown = (m_input->m_keyboardKeys[DIK_LALT].IsDown() || m_input->m_keyboardKeys[DIK_RALT].IsDown());
		BOOL midMouseDown = m_input->m_mouseButtons[MOUSE_MIDDLEBUTTON].IsDown();
		BOOL cursorRightDrag = (m_gStatus->m_showMenuCursor && m_input->m_mouseButtons[MOUSE_RIGHTBUTTON].IsDown());
		BOOL freeOrbitActive = ((m_input->m_shiftOn || altDown || midMouseDown || cursorRightDrag) &&
		                        m_gStatus->m_cameraModule->m_rightClickControl == TRUE &&
		                        ptrLoc->m_firstPersonMode == FALSE);

		// Press Home to snap 3rd-person orbit camera back behind character
		if(ptrLoc->m_firstPersonMode == FALSE && m_input->m_keyboardKeys[DIK_HOME].IsDown())
		{
			m_gStatus->m_cameraModule->m_curYaw = 0.0f;
			m_gStatus->m_cameraModule->m_orbitPitchOffset = 0.0f;
		}

		// Always allow mouse wheel to smoothly zoom 3rd-person camera in/out when menus aren't scrolling
		if(!freeOrbitActive && !m_gStatus->m_showMenuCursor && ptrLoc->m_firstPersonMode == FALSE && m_input->m_mouseWheelZ != 0)
		{
			m_gStatus->m_cameraModule->ApplyMouseRotation(0.0f, 0.0f, (float)m_input->m_mouseWheelZ * -0.008f);
		}

		if(freeOrbitActive)
		{
		   float orbitX = mouseControlXLoc * 0.15f;
		   float orbitY = mouseControlYLoc * 0.12f;
		   if(m_input->m_keyboardKeys[DIK_LEFT].IsDown())  orbitX -= 2.5f * m_gStatus->m_timeAdjustFloat;
		   if(m_input->m_keyboardKeys[DIK_RIGHT].IsDown()) orbitX += 2.5f * m_gStatus->m_timeAdjustFloat;
		   if(m_input->m_keyboardKeys[DIK_UP].IsDown())    orbitY += 2.0f * m_gStatus->m_timeAdjustFloat;
		   if(m_input->m_keyboardKeys[DIK_DOWN].IsDown())  orbitY -= 2.0f * m_gStatus->m_timeAdjustFloat;

           m_gStatus->m_cameraModule->ApplyMouseRotation(orbitX, orbitY, (float)m_input->m_mouseWheelZ*-.008f);
           mouseControlXLoc = 0.0f;
		   mouseControlYLoc = 0.0f;
		   if(m_input->m_keyboardKeys[DIK_TAB].IsDown() || m_input->m_keyboardKeys[DIK_1].IsDown())
		   if(GetTickCount()-m_gStatus->m_cameraModule->m_switchStamp > 500)
		   {
              if(m_gStatus->m_cameraModule->m_reCorrectY > 0.0f)
			     m_gStatus->m_cameraModule->m_reCorrectY = 0.0f;
			  else
			     m_gStatus->m_cameraModule->m_reCorrectY = 10.0f;

			  m_gStatus->m_cameraModule->m_switchStamp = GetTickCount();
		   }
		}
        else if(!m_gStatus->m_showMenuCursor)//If the menu system isnt handling mouse movements
		{
			//adjust for sensitivity alter due to zoom 
			float sensitivityLoc = lookfirePtr->List_movementDB->List_mouseSensitivity;
			//mouse directional called once if used an basis
			if(conPtr->List_controlBasis == 1)
			{//if turning based on mouse
				if(mouseControlYLoc != 0.0f)
					ptrLoc->List_movementDB->m_pitchIsUsed = TRUE;
				if(mouseControlXLoc != 0.0f)
					ptrLoc->List_movementDB->m_yawIsUsed = TRUE;
				if(lookfirePtr->List_movementDB->List_jointMouseFocus == FALSE)
				{//not mech type
					MouseTurnLeft(ptrLoc->List_movementDB,mouseControlXLoc);
					MouseTurnRight(ptrLoc->List_movementDB,mouseControlXLoc);
					MouseTurnPullUp(ptrLoc->List_movementDB,mouseControlYLoc);
					MouseTurnDive(ptrLoc->List_movementDB,mouseControlYLoc);
					ptrLoc->List_movementDB->m_yawIsUsed = TRUE;
				}//end not mech type
			}//end if turning based on mouse
			
			float additionalDrag = 1.0f - ptrLoc->List_movementDB->m_rotationDragConstant;
			float drag = ptrLoc->List_movementDB->m_rotationalGroundDrag - additionalDrag;

			if(ptrLoc->List_movementDB->List_doGravity == TRUE)
				drag = ptrLoc->List_movementDB->m_rotationDragConstant;

			if(lookfirePtr->List_movementDB->List_jointMouseFocus == TRUE)
			{//mech type
				//PITCH FREELOOK
				float newYLocal = mouseControlYLoc;

				lookfirePtr->List_movementDB->m_freeLookPichForce = (float)lookfirePtr->List_movementDB->CalculateForceValue(	(double)(newYLocal*conPtr->m_mouseSensitivity),
																																(double)drag,
																																(double)lookfirePtr->List_movementDB->m_freeLookPichForce,
																																m_gStatus->m_timeAdjust,
																																m_gStatus->m_lastTimeAdjust);
				  
				  
				  
				//if(lookfirePtr->m_firstPersonMode)
				lookfirePtr->List_movementDB->List_freeLookCurPitch += lookfirePtr->List_movementDB->m_freeLookPichForce;//ARB1
				//LIMITS
				if(lookfirePtr->List_movementDB->List_freeLookCurPitch > lookfirePtr->List_movementDB->List_freeLookCurMaxPitch)
					lookfirePtr->List_movementDB->List_freeLookCurPitch = lookfirePtr->List_movementDB->List_freeLookCurMaxPitch;
				if(lookfirePtr->List_movementDB->List_freeLookCurPitch < -lookfirePtr->List_movementDB->List_freeLookCurMaxPitch)
					lookfirePtr->List_movementDB->List_freeLookCurPitch = -lookfirePtr->List_movementDB->List_freeLookCurMaxPitch;
			   
				ptrLoc->List_movementDB->List_freeLookCurPitch = lookfirePtr->List_movementDB->List_freeLookCurPitch;	  
				//END PITCH FREELOOK
				//YAW FREELOOK
				if(ptrLoc->List_movementDB->List_freeLookCurMaxYaw == 0)
				{//if no ability to turn torso turn body
					MouseTurnLeft(ptrLoc->List_movementDB,mouseControlXLoc);
					MouseTurnRight(ptrLoc->List_movementDB,mouseControlXLoc);
				}//if no ability to turn torso turn body
				else
				{//ability to turn torso  
					ptrLoc->List_movementDB->m_freeLookYawForce = (float)lookfirePtr->List_movementDB->CalculateForceValue(	(double)(mouseControlXLoc*conPtr->m_mouseSensitivity),
																															(double)drag,
																															(double)ptrLoc->List_movementDB->m_freeLookYawForce,
																															m_gStatus->m_timeAdjust,
																															m_gStatus->m_lastTimeAdjust);
					ptrLoc->List_movementDB->List_freeLookCurYaw += ptrLoc->List_movementDB->m_freeLookYawForce;
					//LIMITS
					if(ptrLoc->List_movementDB->List_freeLookCurYaw > ptrLoc->List_movementDB->List_freeLookCurMaxYaw)
						ptrLoc->List_movementDB->List_freeLookCurYaw = ptrLoc->List_movementDB->List_freeLookCurMaxYaw;
					if(ptrLoc->List_movementDB->List_freeLookCurYaw < -ptrLoc->List_movementDB->List_freeLookCurMaxYaw)
						ptrLoc->List_movementDB->List_freeLookCurYaw = -ptrLoc->List_movementDB->List_freeLookCurMaxYaw;
				}//ability to turn torso
				//END YAW FREELOOK

				if(entityPtr->List_skeletalObject)
					entityPtr->List_skeletalObject->ApplyFreeLookRotation(entityPtr->List_movementDB->List_freeLookCurPitch,entityPtr->List_movementDB->List_freeLookCurYaw);
			}//end mech type

			//adjust camera
			/*if(camPtrRef)
			{
				camPtrRef->List_cameraCurPitch = entityPtr->List_movementDB->List_freeLookCurPitch;
				camPtrRef->List_cameraCurYaw = entityPtr->List_movementDB->List_freeLookCurYaw;
			}*/
		}
		//Resets
		lookfirePtr->List_currentlyFireing = FALSE;

		if(!menusHandledInput)
			ProcessKeyboardMouseButtons(ptrLoc, 
										entityPtr,
										lookfirePtr,
										currentTime,
										jetsWereUsed,
										AdditionalForceX,
										AdditionalForceY,
										AdditionalForceZ,
										FALSE,
										camPtrRef,
										conPtr,
										FALSE);

		if(ptrLoc->List_movementDB->m_yawIsUsed == FALSE)
			DecelTurn(ptrLoc->List_movementDB);
		if(ptrLoc->List_movementDB->m_pitchIsUsed == FALSE)
			DecelPitch(ptrLoc->List_movementDB);
		if(ptrLoc->List_movementDB->m_rollIsUsed == FALSE)
			DecelRoll(ptrLoc->List_movementDB);
		if(ptrLoc->List_movementDB->m_speedControlInUse == FALSE)
			DecelSpeed(ptrLoc->List_movementDB);
		if(ptrLoc->List_movementDB->m_sidestepsInUse == FALSE)
			DecelSideStep(ptrLoc->List_movementDB);

		RegenJet(ptrLoc,10);

		//If the menu system isnt handling mouse movements
		if(!m_gStatus->m_showMenuCursor)
		{
			if(ptrLoc->List_movementDB->List_maxYaw != 0.0f)
			{//valid yaw variable
				if(conPtr->List_controlBasis == 1 || ptrLoc->List_firstPersonModel == TRUE)
					doYaw = -(mouseControlXLoc*(conPtr->m_mouseSensitivity));//correction for walk gameplay
				else//do it the normal way
					doYaw =(float)ptrLoc->List_movementDB->List_curRotY;
			}//end valid yaw variable
	
			if(ptrLoc->List_movementDB->List_maxPitch != 0.0f)
			{//valid pitch variable
				if(conPtr->List_controlBasis == 1 || ptrLoc->List_firstPersonModel == TRUE)
					doPitch = -(mouseControlYLoc*(conPtr->m_mouseSensitivity));//correction for walk gameplay
				else//do it the normal way
					doPitch =(float)ptrLoc->List_movementDB->List_curRotX;
			}//end valid pitch variable
		}

		//adjustments
		if(ptrLoc->List_movementDB->List_curTransZ < ptrLoc->List_movementDB->List_minSpeed)
			ptrLoc->List_movementDB->List_curTransZ = ptrLoc->List_movementDB->List_minSpeed; 
	}//end alive
	else
	{//dead
		if(entityPtr->m_posses)
			entityPtr->m_posses->List_energy = 0;

		if(ptrLoc->m_collisionType == 4)
			ptrLoc->List_movementDB->List_curTransX = 100.0f;

		if(ptrLoc->List_skeletalObject)
			ptrLoc->List_skeletalObject->ApplyFreeLookRotation(0.0f,0.0f);

		m_gStatus->ChangeToThirdPersonView(entityPtr);//make sure he sees hes toast

		ZeroForces(entityPtr->List_movementDB);
		//if on ground and dead in network game respawn
		if(entityPtr->List_movementDB->List_doGravity == FALSE || entityPtr->List_movementDB->m_inLiquid == TRUE)
			if(m_gStatus->m_respawning == FALSE)
			{//on ground tell server to respawn
				if((currentTime-m_gStatus->m_respawnTimeStamp) > m_gStatus->m_respawnWait)
				{
					if(m_gStatus->m_multiplayerObject->m_currentCharacterInUse < 0)
						m_gStatus->m_multiplayerObject->m_currentCharacterInUse = 0;

					m_gStatus->m_globalAvVar = TRUE;
					m_gStatus->m_axLogger.WriteDebugFile("Play char command sent",0,1);
					m_gStatus->m_netGameLogicObject->PlayCharacterOnAccount(m_gStatus->m_multiplayerObject->m_currentCharacterInUse,m_gStatus->m_multiplayerObject,m_gStatus->m_localContentClientVersion);
			  
					m_gStatus->m_respawning = TRUE;
				}
			}//end on ground tell server to respawn
	}//end dead

	//New additional
	m_gStatus->m_scndRec = 0;//clear cap

	D3DVECTOR orig;
	float submergeValue = 0.0f;
	ptrLoc->List_baseFrame->GetPosition(NULL,&orig);
	BOOL inWater = m_gStatus->m_waterZoneDB->IsPositionInWater(orig,&submergeValue);

	ptrLoc->List_movementDB->DoPhysics(	m_gStatus->m_timeAdjust,
										ptrLoc->List_baseFrame,
										ptrLoc->List_movementDB,
										AdditionalForceX,
										AdditionalForceY,
										AdditionalForceZ,
										ExternalForceX,
										ExternalForceY,
										ExternalForceZ,
										doYaw,
										doPitch,
										ptrLoc->List_movementDB->m_rollIsUsed,
										ptrLoc->List_movementDB->m_pitchIsUsed,
										ptrLoc->List_movementDB->m_yawIsUsed,
										submergeValue,
										m_gStatus->m_environmentPtr->m_seaViscusDrag,
										m_gStatus->m_environmentPtr->m_gravity,
										m_gStatus->m_lastTimeAdjust,
										ptrLoc->m_collisionRaySys,
										m_gStatus->m_systemReadout);

	AutoLevelEntity(ptrLoc,
					ptrLoc->List_movementDB->m_autoLevelXStrength * ptrLoc->List_movementDB->m_autoLevelX,
					ptrLoc->List_movementDB->m_autoLevelZStrength * ptrLoc->List_movementDB->m_autoLevelZ,
					ptrLoc->List_movementDB->m_autoLevelTolerance);

	if((GetTickCount()-m_gStatus->m_speedTimerStamp) >= 500)
	{
		D3DVECTOR thisPos;
		ptrLoc->List_baseFrame->GetPosition(NULL,&thisPos);
		thisPos.y = 0.0f;
		m_gStatus->m_speedPositionStamp.y = 0.0f;
		
		/*ptrLoc->List_movementDB->m_curSpeed = m_gStatus->m_matLib.MagnitudeSquared(thisPos.x,thisPos.y,thisPos.z,m_gStatus->m_speedPositionStamp.x,m_gStatus->m_speedPositionStamp.y,m_gStatus->m_speedPositionStamp.z);*/
		ptrLoc->List_movementDB->m_curSpeed = (int)(m_gStatus->m_matLib.MagnitudeSquared(thisPos.x,thisPos.y,thisPos.z,m_gStatus->m_speedPositionStamp.x,m_gStatus->m_speedPositionStamp.y,m_gStatus->m_speedPositionStamp.z));
		//WARNING: C4244 --> float to int
		
		ptrLoc->List_baseFrame->GetPosition(NULL,&m_gStatus->m_speedPositionStamp);
		m_gStatus->m_speedTimerStamp = GetTickCount();
	}

	if(m_gStatus->m_speedOmeterOn)
	 if(m_gStatus->m_systemReadout)
	 {
	    float mph = 0.0f;
        if(ptrLoc->List_movementDB->m_curSpeed > 0.0f)
		{
           mph = ceil(((ptrLoc->List_movementDB->m_curSpeed * 120.0)*60.0f)/5280.0f);
		}
		m_gStatus->m_systemReadout->AddLine("CurSpeed: " + m_gStatus->m_cabLib.ConvToString(ptrLoc->List_movementDB->m_curSpeed) + " Mph");
	 }
 
	//if((currentTime - m_gStatus->m_menuClosedStamp) < 150)
	//	m_restrictMovement = TRUE;

	/*if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
	{//client app activated
		if(entityPtr->List_networkDefinedID > 0)
		{
			if((currentTime-m_gStatus->m_multiplayerObject->m_lastCommunicationTime) > m_gStatus->m_multiplayerObject->m_lagMSTolerance)
			{//lagged
				if(!m_gStatus->m_lagged)
					m_gStatus->m_systemReadout->AddLine("Connection Interuption...");
				m_gStatus->m_lagged = TRUE;
				return;
			}//end lagged
		}
		else
			return;
	}//end client app activated
 
	if(m_gStatus->m_lagged)
		m_gStatus->m_systemReadout->AddLine("Connection Resumed...");
 
	m_gStatus->m_lagged = FALSE;*/

	/*menu lock option
	for(POSITION mnuPos = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();mnuPos != NULL;)
	{//loop through menus
		CMenu3DObj* menu = (CMenu3DObj*)m_gStatus->m_runtimeMenuDataBase->GetNext(mnuPos);
		if(menu->m_lockControls)
		{
			m_restrictMovement = TRUE;
			m_gStatus->m_menuClosedStamp = currentTime;
			this->ZeroForces(ptrLoc->List_movementDB);
		}
		if(menu->m_restrictChat)
			restrictChat = TRUE;
	}*/

	//ptrLoc = entityPtr;
/*	BOOL jetsWereUsed = FALSE;
	ptrLoc->List_movementDB->m_jetPackInUse = FALSE;
 
	//ADDITIONAL ENTITY FORCES
	float AdditionalForceX=0.0f;
	float AdditionalForceY=0.0f;
	float AdditionalForceZ=0.0f;
	//EXTERNAL FORCES
	float ExternalForceX=ptrLoc->List_externalVectorTemp.x;
	float ExternalForceY=ptrLoc->List_externalVectorTemp.y;
	float ExternalForceZ=ptrLoc->List_externalVectorTemp.z;
	ptrLoc->List_externalVectorTemp.x = 0.0f;
	ptrLoc->List_externalVectorTemp.y = 0.0f;
	ptrLoc->List_externalVectorTemp.z = 0.0f;
	float exponentBasis = 1.0f;
	//get device state
	CCameraObj * camPtrRef = m_gStatus->m_runtimeViewportDB->GetCameraObjectFromEntity(entityPtr);
	float mouseControlXLoc=(m_mouseHiResX*3.0f)/m_gStatus->m_timeAdjustFloat;//(float)GL_MS.lX;
	float mouseControlYLoc=(m_mouseHiResY*3.0f)/m_gStatus->m_timeAdjustFloat;//(float)GL_MS.lY;

	POSITION posLoc = m_gStatus->m_controlsList->FindIndex(controlConfigNum);
    if(!posLoc)
		return;
	ControlDBObj * conPtr = ( ControlDBObj* ) m_gStatus->m_controlsList->GetAt(posLoc);	

	if(!conPtr->m_invertMouse)//non inverted
		mouseControlYLoc = -mouseControlYLoc;

	//MISC
	if(m_mousepoint.lZ != 0)
	{//MOUSE wheel
		if(m_mousepoint.lZ>0)
			ZoomIn(entityPtr,camPtrRef);
		else
			ZoomOut(entityPtr,camPtrRef);
	}//end MOUSE wheel
 
	//adjust for sensitivity alter due to zoom 
	float sensitivityLoc = lookfirePtr->List_movementDB->List_mouseSensitivity;

	//Zoom Management 
	ZoomManageMent(lookfirePtr,camPtrRef);
	if(camPtrRef)
	{
		if(lookfirePtr->m_firstPersonMode == TRUE)
			camPtrRef->List_locked = FALSE;
		else
			camPtrRef->List_locked = TRUE;

		if(lookfirePtr->List_movementDB->List_capabilityFovMax != camPtrRef->List_fovView)
		{//adjust
			if(camPtrRef->List_fovView != 0.0f)
			{
				float adFactor = camPtrRef->List_fovView/lookfirePtr->List_movementDB->List_capabilityFovMax;
				sensitivityLoc = sensitivityLoc * (adFactor * .5f);
			}
		}//end adjust
	}

	//ProcessChat(ptrLoc,entityPtr);

    ptrLoc->List_movementDB->SetUseStatesToFalse();
    
	//mouse directional called once if used an basis
	if(!m_restrictMovement)
	{//allow movement
		if(conPtr->List_controlBasis == 1)
		{//if turning based on mouse
			if(mouseControlYLoc != 0.0f)
				ptrLoc->List_movementDB->m_pitchIsUsed = TRUE;
			if(mouseControlXLoc != 0.0f)
				ptrLoc->List_movementDB->m_yawIsUsed = TRUE;
			if(lookfirePtr->List_movementDB->List_jointMouseFocus == FALSE)
			{//not mech type
				MouseTurnLeft(ptrLoc->List_movementDB,mouseControlXLoc);
				MouseTurnRight(ptrLoc->List_movementDB,mouseControlXLoc);
				MouseTurnPullUp(ptrLoc->List_movementDB,mouseControlYLoc);
				MouseTurnDive(ptrLoc->List_movementDB,mouseControlYLoc);
				ptrLoc->List_movementDB->m_yawIsUsed = TRUE;
			}//end not mech type
		}//end if turning based on mouse
		
		float additionalDrag = 1.0f - ptrLoc->List_movementDB->m_rotationDragConstant;
		float drag = ptrLoc->List_movementDB->m_rotationalGroundDrag - additionalDrag;

		if(ptrLoc->List_movementDB->List_doGravity == TRUE)
			drag = ptrLoc->List_movementDB->m_rotationDragConstant;
		if(lookfirePtr->List_movementDB->List_jointMouseFocus == TRUE)
		{//mech type
			//PITCH FREELOOK
			float newYLocal = mouseControlYLoc;

			lookfirePtr->List_movementDB->m_freeLookPichForce = (float)lookfirePtr->List_movementDB->CalculateForceValue(	(double)(newYLocal*conPtr->m_mouseSensitivity),
																															(double)drag,
																															(double)lookfirePtr->List_movementDB->m_freeLookPichForce,
																															m_gStatus->m_timeAdjust,
																															m_gStatus->m_lastTimeAdjust);
			  
			  
			  
			//if(lookfirePtr->m_firstPersonMode)
			lookfirePtr->List_movementDB->List_freeLookCurPitch += lookfirePtr->List_movementDB->m_freeLookPichForce;//ARB1
			//LIMITS
			if(lookfirePtr->List_movementDB->List_freeLookCurPitch > lookfirePtr->List_movementDB->List_freeLookCurMaxPitch)
				lookfirePtr->List_movementDB->List_freeLookCurPitch = lookfirePtr->List_movementDB->List_freeLookCurMaxPitch;
			if(lookfirePtr->List_movementDB->List_freeLookCurPitch < -lookfirePtr->List_movementDB->List_freeLookCurMaxPitch)
				lookfirePtr->List_movementDB->List_freeLookCurPitch = -lookfirePtr->List_movementDB->List_freeLookCurMaxPitch;
		   
			ptrLoc->List_movementDB->List_freeLookCurPitch = lookfirePtr->List_movementDB->List_freeLookCurPitch;	  
			//END PITCH FREELOOK
			//YAW FREELOOK
			if(ptrLoc->List_movementDB->List_freeLookCurMaxYaw == 0)
			{//if no ability to turn torso turn body
				MouseTurnLeft(ptrLoc->List_movementDB,mouseControlXLoc);
				MouseTurnRight(ptrLoc->List_movementDB,mouseControlXLoc);
			}//if no ability to turn torso turn body
			else
			{//ability to turn torso  
				ptrLoc->List_movementDB->m_freeLookYawForce = (float)lookfirePtr->List_movementDB->CalculateForceValue(	(double)(mouseControlXLoc*conPtr->m_mouseSensitivity),
																														(double)drag,
																														(double)ptrLoc->List_movementDB->m_freeLookYawForce,
																														m_gStatus->m_timeAdjust,
																														m_gStatus->m_lastTimeAdjust);
				ptrLoc->List_movementDB->List_freeLookCurYaw += ptrLoc->List_movementDB->m_freeLookYawForce;
				//LIMITS
				if(ptrLoc->List_movementDB->List_freeLookCurYaw > ptrLoc->List_movementDB->List_freeLookCurMaxYaw)
					ptrLoc->List_movementDB->List_freeLookCurYaw = ptrLoc->List_movementDB->List_freeLookCurMaxYaw;
				if(ptrLoc->List_movementDB->List_freeLookCurYaw < -ptrLoc->List_movementDB->List_freeLookCurMaxYaw)
					ptrLoc->List_movementDB->List_freeLookCurYaw = -ptrLoc->List_movementDB->List_freeLookCurMaxYaw;
			}//ability to turn torso
			//END YAW FREELOOK

			if(entityPtr->List_skeletalObject)
				entityPtr->List_skeletalObject->ApplyFreeLookRotation(entityPtr->List_movementDB->List_freeLookCurPitch,entityPtr->List_movementDB->List_freeLookCurYaw);
		}//end mech type

		//adjust camera
		if(camPtrRef)
		{
			camPtrRef->List_cameraCurPitch = entityPtr->List_movementDB->List_freeLookCurPitch;
			camPtrRef->List_cameraCurYaw = entityPtr->List_movementDB->List_freeLookCurYaw;
		}
		//Resets
		lookfirePtr->List_currentlyFireing = FALSE;
	}//allow movement
*/
/*	ProcessKeyboardMouseButtons(ptrLoc, 
								entityPtr,
								lookfirePtr,
							    currentTime,
								jetsWereUsed,
								AdditionalForceX,
								AdditionalForceY,
								AdditionalForceZ,
								restrictChat,
								camPtrRef,
								conPtr,
								FALSE);

*/	//after control decel if a motion is not used
/*	if(ptrLoc->List_movementDB->m_yawIsUsed==FALSE)
		DecelTurn(ptrLoc->List_movementDB);
	if(ptrLoc->List_movementDB->m_pitchIsUsed==FALSE)
		DecelPitch(ptrLoc->List_movementDB);
	if(ptrLoc->List_movementDB->m_rollIsUsed==FALSE)
		DecelRoll(ptrLoc->List_movementDB);
	if(ptrLoc->List_movementDB->m_speedControlInUse==FALSE)
		DecelSpeed(ptrLoc->List_movementDB);
	if(ptrLoc->List_movementDB->m_sidestepsInUse==FALSE)
		DecelSideStep(ptrLoc->List_movementDB);
*/
/*	float doYaw= 0.0f;
	float doPitch= 0.0f;
	if(!m_restrictMovement)
	{//allow turn pitch regen
		//if(!jetsWereUsed)
				RegenJet(ptrLoc,15);

		if(ptrLoc->List_movementDB->List_maxYaw != 0.0f)
		{//valid yaw variable
			if(conPtr->List_controlBasis == 1 || ptrLoc->List_firstPersonModel == TRUE)
				doYaw = -(mouseControlXLoc*(conPtr->m_mouseSensitivity));//correction for walk gameplay
			else//do it the normal way
				doYaw =(float)ptrLoc->List_movementDB->List_curRotY;
		}//end valid yaw variable

		if(ptrLoc->List_movementDB->List_maxPitch != 0.0f)
		{//valid pitch variable
			if(conPtr->List_controlBasis == 1 || ptrLoc->List_firstPersonModel == TRUE)
				doPitch = -(mouseControlYLoc*(conPtr->m_mouseSensitivity));//correction for walk gameplay
			else//do it the normal way
				doPitch =(float)ptrLoc->List_movementDB->List_curRotX;
		}//end valid pitch variable
	}//end allow turn pitch regen
*/
/*	if(ptrLoc->List_energy <= 0)
	{//dead so dont use user input
		if(ptrLoc->List_skeletalObject)
			ptrLoc->List_skeletalObject->ApplyFreeLookRotation(0.0f,0.0f);

		m_gStatus->ChangeToThirdPersonView(entityPtr);//make sure he sees hes toast
	}//end dead so dont use user input

	//adjustments
	if(ptrLoc->List_movementDB->List_curTransZ < ptrLoc->List_movementDB->List_minSpeed)
		ptrLoc->List_movementDB->List_curTransZ = ptrLoc->List_movementDB->List_minSpeed; 
	//New additional
	m_gStatus->m_scndRec = 0;//clear cap

	D3DVECTOR orig;
	float submergeValue = 0.0f;
	ptrLoc->List_baseFrame->GetPosition(NULL,&orig);
	BOOL inWater = m_gStatus->m_waterZoneDB->IsPositionInWater(orig,&submergeValue);

	if(m_restrictMovement)
	if(ptrLoc->m_collisionType == 4)
		ptrLoc->List_movementDB->List_curTransX = 100.0f;

	ptrLoc->List_movementDB->DoPhysics(	m_gStatus->m_timeAdjust,
										ptrLoc->List_baseFrame,
										ptrLoc->List_movementDB,
										AdditionalForceX,
										AdditionalForceY,
										AdditionalForceZ,
										ExternalForceX,
										ExternalForceY,
										ExternalForceZ,
										doYaw,
										doPitch,
										ptrLoc->List_movementDB->m_rollIsUsed,
										ptrLoc->List_movementDB->m_pitchIsUsed,
										ptrLoc->List_movementDB->m_yawIsUsed,
										submergeValue,
										m_gStatus->m_environmentPtr->m_seaViscusDrag,
										m_gStatus->m_environmentPtr->m_gravity,
										m_gStatus->m_lastTimeAdjust,
										ptrLoc->m_collisionRaySys,
										m_gStatus->m_systemReadout);

	if(ptrLoc->List_movementDB->m_autoLevel == TRUE)
		AutoLevelEntity(ptrLoc,ptrLoc->List_movementDB->m_autoLevelStrength,ptrLoc->List_movementDB->m_autoLevelTolerance);

    if((GetTickCount()-m_gStatus->m_speedTimerStamp) >= 500)
	{
		D3DVECTOR thisPos;
		ptrLoc->List_baseFrame->GetPosition(NULL,&thisPos);
		thisPos.y = 0.0f;
		m_gStatus->m_speedPositionStamp.y = 0.0f;
		ptrLoc->List_movementDB->m_curSpeed = m_gStatus->m_matLib.MagnitudeSquared(thisPos.x,thisPos.y,thisPos.z,m_gStatus->m_speedPositionStamp.x,m_gStatus->m_speedPositionStamp.y,m_gStatus->m_speedPositionStamp.z);
		ptrLoc->List_baseFrame->GetPosition(NULL,&m_gStatus->m_speedPositionStamp);
		m_gStatus->m_speedTimerStamp = GetTickCount();
	}

	if(m_gStatus->m_speedOmeterOn)
		m_gStatus->m_systemReadout->AddLine(ptrLoc->List_movementDB->m_curSpeed);*/
}
//-----------------------------------------------------------------------------------
//handle chat model
//-----------------------------------------------------------------------------------
void UserController::ProcessChat(CMovementObj * ptrLoc,CMovementObj *entityPtr)
{
//Talking chat stuffola 
	/*BOOL handleKeyStrokes = TRUE;
	   if(m_gStatus->m_multiplayerObject)
	if(	m_gStatus->m_multiplayerObject->m_chatEnabled == TRUE ||
		m_gStatus->m_multiplayerObject->m_groupChatEnabled == TRUE ||
		m_gStatus->m_multiplayerObject->m_clanChatEnabled == TRUE)
	{//chat model*/
		DWORD curTime = GetTickCount();
		CString addition = "";
		int size = ptrLoc->List_networkTextString.GetLength();
		int nameSize = m_gStatus->m_multiplayerObject->m_userName.GetLength();

		for(int kLoop = 0; kLoop < KEYBOARD_MAX_KEYS; kLoop++)
			if(m_input->m_keyboardKeys[kLoop].JustPressed() || m_input->m_keyboardKeys[kLoop].ShouldRepeat(curTime))
				if(kLoop != DIK_BACKSPACE)
					addition += ParseData(kLoop);
				else
					ptrLoc->List_networkTextString.Delete(ptrLoc->List_networkTextString.GetLength() - 1, 1);

		if(addition.GetLength() > 0)
			ptrLoc->List_networkTextString += addition;

		/*int deleteStringCount = m_gStatus->m_chatDeleteString.GetLength();
		  if(deleteStringCount > 0)
		  {
            int chatCurrentLength = ptrLoc->List_networkTextString.GetLength();
            if(chatCurrentLength > 0)
			if(chatCurrentLength >= deleteStringCount)
               ptrLoc->List_networkTextString.Delete((size-deleteStringCount),deleteStringCount);
			m_gStatus->m_chatDeleteString.Empty();
		  }

		  if(m_gStatus->m_speechMode == 1)
		if(m_keyboardKeys[DIK_SPACE].JustPressed())
		    {
		       int len = entityPtr->List_networkTextString.GetLength();
	           if(len > 0)
                  if(entityPtr->List_networkTextString.GetAt(len-1) == '~')
	                       entityPtr->List_networkTextString.Delete(len-1,1);
				
			   m_gStatus->m_chatBuild = " ~";
		    }
 
		  if(m_gStatus->m_chatBuild.GetLength() > 0)
		  {//valid additive
		     if(m_gStatus->m_tempTalk)
			 if(m_gStatus->m_speechMode == 1)
             if(m_gStatus->m_tempTalk->m_unicodeTempSection.GetLength() > 0)
			 {
				//if they pressed a number key
			    char xtemp2 = m_gStatus->m_chatBuild.GetAt(0);
				if(((int)xtemp2) < 58 && ((int)xtemp2) > 47)
			    {
                    m_gStatus->m_chatBuild = m_gStatus->m_cabLib.ConvToString(m_gStatus->m_unicodeStartIndex+atoi(m_gStatus->m_chatBuild));
                    CSymbolMapperObj * symbolMapPtr = m_gStatus->m_symbolMapDB->GetByID((m_gStatus->m_tempTalk->m_unicodeTempSection + m_gStatus->m_chatBuild));
					if(symbolMapPtr)
					{
                       //delete everything back to the tilde
					   int lDelLength = ptrLoc->List_networkTextString.GetLength();
					   while(lDelLength > 0)
					   {
                          if(entityPtr->List_networkTextString.GetAt(lDelLength-1) == '~')
						  {
                             entityPtr->List_networkTextString.Delete(lDelLength-1,1);
                             break;
						  }
						  entityPtr->List_networkTextString.Delete(lDelLength-1,1);
                          lDelLength--;
					   }
                       if(symbolMapPtr->m_extendedOption.GetLength() > 0)
                       m_gStatus->m_chatBuild = symbolMapPtr->m_extendedOption;
					   else
                       m_gStatus->m_chatBuild = symbolMapPtr->m_uniqueID;
					}
					m_gStatus->m_chatBuild = m_gStatus->m_chatBuild + "~";
				}
			 }

             ptrLoc->List_networkTextString = operator+(ptrLoc->List_networkTextString,m_gStatus->m_chatBuild);
			//m_gStatus->m_chatBuild.Empty();
		  }//end valid additive

		  if(m_gStatus->m_speechMode == 1)
		  if(ptrLoc->List_networkTextString.GetLength() == 0)
		     ptrLoc->List_networkTextString += "~";
*/
		if(m_input->m_keyboardKeys[DIK_RETURN].IsDown())
		{
			//ptrLoc->List_networkTextString = ptrLoc->List_networkTextString;
             if(size > 0)
			 if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
			 {
			    BOOL commandFound = FALSE;
				//ptrLoc->List_networkTextString.MakeUpper();
				CString preFix = (CString)ptrLoc->List_networkTextString.GetAt(0);
				if(preFix == ":")
				{//check to see if this is a command
				   CString parse = ptrLoc->List_networkTextString;
				   int length = parse.GetLength();
				   
				   if(length > 7)
				   {//valid length for invite command lets check
	                  int length = parse.GetLength();
	                  int trim = length - 7;
                      CString name = parse;
                      parse.Delete(7,trim);
                      if(parse == ":INVITE")
					  {//INVITE COMMAND 
					     name.Delete(0,8);//parse out name
                         CMovementObj * entityToInvite = m_gStatus->m_entObjRuntime->GetObjByName(name);//search name in runtime
                         if(entityToInvite)
						 {//found it
							 m_gStatus->m_netGameLogicObject->SendCommandMsg(3,
									                                         entityToInvite->List_networkDefinedID,
									                                         entityPtr->List_networkDefinedID);//ask to join our group
						 }//end found it
						 commandFound = TRUE;
					  }//END INVITE COMMAND
				   }//end valid length for invite command lets check
				   if(length == 10)
				   {//valid length for invite command lets check
					  parse = ptrLoc->List_networkTextString;
	                  int length = parse.GetLength();
                      if(parse == ":GROUPMENU")
					  {//SQUAD MENU COMMAND 
						 m_gStatus->m_netGameLogicObject->SendCommandMsg(4,
									                                     0,
									                                     entityPtr->List_networkDefinedID);//request group list
						 commandFound = TRUE;
					  }//END GROUP MENU COMMAND
				   }//end valid length for invite command lets check
				   if(length > 8)
				   {//possible squelch
                      CString possibleCommand = ptrLoc->List_networkTextString;
					  possibleCommand.Delete(8,possibleCommand.GetLength()-8);
                      if(possibleCommand == ":SQUELCH")
					  {
                         possibleCommand = ptrLoc->List_networkTextString;
						 possibleCommand.Delete(0,9);
                         m_gStatus->m_filterSpeechDB->AddName(possibleCommand);
						 if(m_gStatus->m_systemReadout)
							m_gStatus->m_systemReadout->AddLine(possibleCommand + " has been squelched...");
					  }
				   }//end possible squelch
				   if(length > 10)
				   {//possible squelch
                      CString possibleCommand = ptrLoc->List_networkTextString;
					  possibleCommand.Delete(10,possibleCommand.GetLength()-10);
                      if(possibleCommand == ":DESQUELCH")
					  {
                         possibleCommand = ptrLoc->List_networkTextString;
						 possibleCommand.Delete(0,11);
                         m_gStatus->m_filterSpeechDB->UnBlockName(possibleCommand);
						 if(m_gStatus->m_systemReadout)
							m_gStatus->m_systemReadout->AddLine(possibleCommand + " has been desquelched...");
					  }
				   }//end possible squelch
				}//end check to see if this is a command
				if(!commandFound)
				{//else normal chat
                  if(m_gStatus->m_speechMode != 0)
                  {
                     int len = entityPtr->List_networkTextString.GetLength();
	                 if(len > 0)
                        if(entityPtr->List_networkTextString.GetAt(len-1) == '~')
	                       entityPtr->List_networkTextString.Delete(len-1,1);
	                 }


				  if(m_gStatus->m_multiplayerObject->m_chatEnabled)
					 m_gStatus->SendChatString(ptrLoc->List_networkTextString);
				  else if(m_gStatus->m_multiplayerObject->m_groupChatEnabled)
                     m_gStatus->m_multiplayerObject->SendTextMsgToGroup(DPNSEND_GUARANTEED,
								                        DPID_SERVERPLAYER,//can be NULL if client
											            ptrLoc->List_networkTextString,
											            entityPtr->List_networkDefinedID);
				  else if(m_gStatus->m_multiplayerObject->m_clanChatEnabled)
					 m_gStatus->m_multiplayerObject->SendTextMsgToGroup(DPNSEND_GUARANTEED,
								                                DPID_SERVERPLAYER,//can be NULL if client
											                    ptrLoc->List_networkTextString,
											                    entityPtr->List_networkDefinedID);
 
				}//end normal chat
			 }
			 m_gStatus->m_multiplayerObject->m_chatEnabled = FALSE;
			 m_gStatus->m_multiplayerObject->m_groupChatEnabled = FALSE;
			 m_gStatus->m_multiplayerObject->m_clanChatEnabled = FALSE;
			 ptrLoc->List_networkTextString = "";
			 size=0;
		  }

		    if(m_gStatus->m_multiplayerObject->m_chatEnabled == TRUE ||
			   m_gStatus->m_multiplayerObject->m_groupChatEnabled == TRUE ||
			   m_gStatus->m_multiplayerObject->m_clanChatEnabled == TRUE)
			{
		     CString tempString;
			 if(m_gStatus->m_multiplayerObject->m_chatEnabled)
			    tempString = operator+("YOU SAY: ",ptrLoc->List_networkTextString);
             else if(m_gStatus->m_multiplayerObject->m_groupChatEnabled)
			    tempString = operator+("SAY TO GROUP: ",ptrLoc->List_networkTextString);
			 else if(m_gStatus->m_multiplayerObject->m_clanChatEnabled)
			    tempString = operator+("SAY TO CLAN: ",ptrLoc->List_networkTextString);

             if(!m_gStatus->m_tempTalk)
	             m_gStatus->m_tempTalk = new CTextureRenderObj(tempString,
				                                       2,
			                                           m_gStatus->m_hudYouSayPosX,
				                                       m_gStatus->m_hudYouSayPosY,
				                                       m_gStatus->m_hudYouSaySizeX,
				                                       m_gStatus->m_hudYouSaySizeY,
										               TRUE,
													   m_gStatus->m_hudYouSayRed,
													   m_gStatus->m_hudYouSayGreen,
													   m_gStatus->m_hudYouSayBlue);

             m_gStatus->m_tempTalk->Initialize(m_gStatus->m_3dDevice,tempString,FALSE,m_gStatus->m_symbolMapDB,m_gStatus->m_speechMode,0.0f); 
			 if(m_gStatus->m_tempTalk->m_unicodeTempSection.GetLength() > 0)
			 {
			    m_gStatus->m_maxUnicodeOptions = 0;
				m_gStatus->m_globalUnicodeSection = GetPossibleSymbols(m_gStatus->m_unicodeStartIndex,m_gStatus->m_tempTalk->m_unicodeTempSection,&m_gStatus->m_maxUnicodeOptions);
                if(m_gStatus->m_lastGlobalUnicodeSection != m_gStatus->m_tempTalk->m_unicodeTempSection)
				   m_gStatus->m_unicodeStartIndex = 0;
				m_gStatus->m_lastGlobalUnicodeSection = m_gStatus->m_tempTalk->m_unicodeTempSection;
			 }
			 else
			    m_gStatus->m_globalUnicodeSection = "";

             m_gStatus->m_textRenderList->AddTail(m_gStatus->m_tempTalk);
			}
		//handleKeyStrokes = FALSE;
	//}//end chat model
}
//-----------------------------------------------------------------------------------
//Function will process keyboard and mouse functions and apply it to the entity ptr
//-----------------------------------------------------------------------------------

void UserController::ProcessKeyboardMouseButtons(CMovementObj * ptrLoc, 
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
												 BOOL filterMouseButtons)
{

	//static bool m_bToggleAutoRun = false;
	bool		justPressed = false, 
				isHeld = false;
	BOOL processIt;

	for(POSITION posLoc2 = conPtr->List_controlInfoList->GetHeadPosition();posLoc2 != NULL;)
	{//info loop
		ControlInfoObj * infoPtr = ( ControlInfoObj* ) conPtr->List_controlInfoList->GetNext(posLoc2);
 
		//---------------------------//
		//Mouse and keyboard section
		//---------------------------//
		justPressed = false;
		processIt = FALSE;
		if(infoPtr->List_controlType == 0)
		/*if(m_input->m_keyboardKeys[infoPtr->List_buttonDown].IsDown() &&
		   m_input->m_shiftOn == infoPtr->m_shiftDown &&
		   m_input->m_altOn   == infoPtr->m_altDown &&
		   m_input->m_ctrlOn  == infoPtr->m_ctrlDown)*/
	    if((int)m_input->m_keyboardKeys[infoPtr->List_buttonDown].IsDown() &&
		   (int)m_input->m_shiftOn == (int)infoPtr->m_shiftDown &&
		   (int)m_input->m_altOn   == (int)infoPtr->m_altDown &&
		   (int)m_input->m_ctrlOn  == (int)infoPtr->m_ctrlDown)
		   //WARNING: C4805 --> unsafe mix of type BOOL and type bool in operation
		{
			processIt = TRUE;
			if(m_input->m_keyboardKeys[infoPtr->List_buttonDown].JustPressed())
				justPressed = true;
		}
 
		if(!filterMouseButtons)
		if(infoPtr->List_controlType == 1)
		/*if(m_input->m_mouseButtons[infoPtr->List_buttonDown].IsDown() &&
		   m_input->m_shiftOn == infoPtr->m_shiftDown &&
		   m_input->m_altOn   == infoPtr->m_altDown &&
		   m_input->m_ctrlOn  == infoPtr->m_ctrlDown)*/
		   
		   if((int)m_input->m_mouseButtons[infoPtr->List_buttonDown].IsDown() &&
		   (int)m_input->m_shiftOn == (int)infoPtr->m_shiftDown &&
		   (int)m_input->m_altOn   == (int)infoPtr->m_altDown &&
		   (int)m_input->m_ctrlOn  == (int)infoPtr->m_ctrlDown)
		   //WARNING: C4805 --> unsafe mix of type BOOL and type bool in operation
		{
			processIt = TRUE;
			if(m_input->m_mouseButtons[infoPtr->List_buttonDown].JustPressed())
				justPressed = true;
			else
				justPressed = false;

			if(m_input->m_mouseButtons[infoPtr->List_buttonDown].IsHeld())
 				isHeld = true;
			else
				isHeld = false;
		}

	   if(m_input->m_mouseButtons[MOUSE_RIGHTBUTTON].IsDown() && m_gStatus->m_showMenuCursor)
		   processIt = FALSE;

		
 
		if(processIt)
		{//if a mouse or keyboard button is down
			switch(infoPtr->List_action)  
			{
			    case 0:
				{
					// If any menus are showing, prevent player from firing
					if(m_gStatus->m_showMenuCursor) break;					

					if (m_gStatus->m_multiplayerObject->m_itemGlobalMaxCharges > 0
					&& m_gStatus->m_multiplayerObject->m_itemGlobalCurCharges < 1)
					{
						m_bIsDisplayed = true;
					//	break;
					}
					else
						m_bIsDisplayed = false;

					//Hacked up attack for now...
					//CHRIS HACK
					//entityPtr->m_energyCurrent -= 5;
					//commented above line to update the EA Server
					if(entityPtr->m_energyCurrent < 0)
						entityPtr->m_energyCurrent = 0;
					else
						if(m_gStatus->m_scndRec == 0)
							if(!m_gStatus->m_multiplayerObject->m_rechargeTimer)
								lookfirePtr->List_currentlyFireing = TRUE;

					break;
				}
                case 2:
                	TurnRight(ptrLoc->List_movementDB);break;
				case 3:
					TurnLeft(ptrLoc->List_movementDB); break;
				case 4:
					PullUp(ptrLoc->List_movementDB); break;
				case 5:
					Dive(ptrLoc->List_movementDB); break;
				case 6:
					SideStepRight(ptrLoc->List_movementDB); break;
				case 7:
					SideStepLeft(ptrLoc->List_movementDB); break;
				case 8:
                    Accellerate(ptrLoc->List_movementDB,0.0f,m_gStatus->m_timeAdjustFloat);
					m_bToggleAutoRun = false;
					break;
				case 9:
                    Deccellerate(ptrLoc->List_movementDB);
					m_bToggleAutoRun = false;
					break;
				case 11:
					RollLeft(ptrLoc->List_movementDB); break;
				case 12:
					RollRight(ptrLoc->List_movementDB); break;
				case 16:
					FreeLookRight(ptrLoc); break;
				case 17:
					FreeLookLeft(ptrLoc); break;
				case 18:
					FreeLookUp(ptrLoc); break;
				case 19:
					FreeLookDown(ptrLoc); break;
				case 20:
					BailOutEntity(ptrLoc); break;
				case 22:
					m_gStatus->ChangeToFirstPersonView(entityPtr); break;
				case 23:
					m_gStatus->ChangeToThirdPersonView(entityPtr); break;
				case 24:
					ArmNextWeapon(lookfirePtr,FALSE); break;
				case 25:
					ArmPrevWeapon(lookfirePtr,FALSE); break;
				case 26:
					/*if(m_gStatus->m_multiplayerObject->m_groupChatEnabled == FALSE && m_gStatus->m_multiplayerObject->m_clanChatEnabled == FALSE)
					{
						m_gStatus->m_multiplayerObject->m_chatEnabled = TRUE;
						m_gStatus->m_lastChatCommandStamp = GetTickCount();
					}*/

					if(justPressed)
					{
						POSITION posLast;
						for(POSITION posLoc = m_gStatus->m_runtimeMenuDatabase->GetHeadPosition();(posLast = posLoc) != NULL;)
						{
							CMenuWindow *menu = (CMenuWindow*)m_gStatus->m_runtimeMenuDatabase->GetNext(posLoc);
							if(menu)
							{
								if(menu->m_visualID == m_gStatus->m_menuChat->m_visualID)
								{
									//Move it to the front of the menu list
									m_gStatus->m_runtimeMenuDatabase->RemoveAt(posLast);
									m_gStatus->m_runtimeMenuDatabase->AddHead(menu);

									//Set it to focused
									menu->m_isFocused = true;
								}
								else
									menu->m_isFocused = false;
							}
						}
						
						if(m_gStatus->m_menuChatInput)
						{
							//Set focus onto the chat input text box
							m_gStatus->m_menuChatInput->m_isFocused = true;
						}

						//Show the mouse while we're typing
						m_gStatus->m_showMenuCursor++;
					}
					break;
				case 27:
					//ZoomIn(ptrLoc,camPtrRef); 
					break;
				case 28:
					//ZoomOut(ptrLoc,camPtrRef); 
					break;
				case 29:
					m_gStatus->OpenMenu((int)infoPtr->List_miscFloat1, false); break;
				case 15:
				{
					//*
					bool m_bJetPackIsArmed = false;
					CArmedInventoryObj *armedPtr = NULL;
					POSITION pos = NULL;

					// Search to see if a jet pack is currently armed
					if(ptrLoc->List_armedInventoryDB)
					{
						for(pos = ptrLoc->List_armedInventoryDB->GetHeadPosition(); pos != NULL;)
						{
							armedPtr = (CArmedInventoryObj*)ptrLoc->List_armedInventoryDB->GetNext(pos);
							if(armedPtr->m_functionalityAttribute == 3) //jet
								m_bJetPackIsArmed = true;
						}
					}

					// KLUDGE: HELD state is not working for some reason. This is a workaround
					//	for when the control key is held to enable continuous jumping.
					if (!ptrLoc->List_movementDB->List_currentlyJumping)
						justPressed = true;

					if(justPressed || isHeld)
					{
						if(!m_bJetPackIsArmed ||
						(m_bJetPackIsArmed && !ptrLoc->List_movementDB->m_jetPackInUse))
						{
							this->m_bIsFlying = false;
						}

						/*if(!m_bJetPackIsArmed || !ptrLoc->List_movementDB->List_currentlyJumping)
						{
							JumpMovementClassObj(ptrLoc);
							justPressed = false;
							break;
						}
						else if(ptrLoc->List_movementDB->List_currentlyJumping || this->m_bIsFlying)
						{ 
							justPressed = false;
							this->m_bIsFlying = true; 
							//* Fall through to jet pack...* /
						}
						else 
						{
							justPressed = false;
							break;
						}*/

						if (this->m_bIsFlying)
							break;
						else
							JumpMovementClassObj(ptrLoc);
					}
					
					//if(!this->m_bIsFlying) break;
					//else { //* Fall through to jet pack...*/ }
					//* /

					/*static bool bIsArmed = false;
					static CArmedInventoryObj *armedPtr = NULL;
					static POSITION pos = NULL;

					// Reset static variables
					bIsArmed = false;
					armedPtr = NULL;
					pos = NULL;

					// Search to see if a jet pack is currently armed
					if(ptrLoc->List_armedInventoryDB)
					{
						for(pos = ptrLoc->List_armedInventoryDB->GetHeadPosition(); pos != NULL;)
						{
							armedPtr = (CArmedInventoryObj*)ptrLoc->List_armedInventoryDB->GetNext(pos);
							if(armedPtr->m_functionalityAttribute == 3) //jet
								bIsArmed = true;
						}
					}

					if(ptrLoc->List_movementDB->List_jumpPower > 0) 
					{
						// If the jet pack isn't armed 
						// OR <Space> was just pressed...
						if(!bIsArmed || justPressed)
						{
							JumpMovementClassObj(ptrLoc); 
							justPressed = false; // Reset for continuous jumping on <Space> hold
							break;
						}
					}
					// Fallthrough to jet pack logic IF 
					// the jet pack is armed AND <Space> is held.
					// else {}*/
					break;
				}
				case 30:
				{
					D3DVECTOR forceJet;
					if(GetJet(ptrLoc,20,forceJet))
					{
						if(!m_gStatus->gl_svrJt)
						{
							AdditionalForceX += forceJet.x;
							AdditionalForceY += forceJet.y;
							AdditionalForceZ += forceJet.z;
						}
						else
						{
							AdditionalForceX += forceJet.x;
							AdditionalForceY += m_gStatus->gl_frceY;
							AdditionalForceZ += m_gStatus->gl_frceZ;
						}
						jetsWereUsed = TRUE;
						ptrLoc->List_movementDB->m_jetPackInUse = TRUE;
					}
					break;
				}
				case 31:
					ZeroForces(ptrLoc->List_movementDB); break;
				case 32:
					/* DEPRECATED /////////////////////////////////////////////
					if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
					if((currentTime - m_gStatus->m_controlTimeStamp) > 1000)
					{//delay
						m_gStatus->SendChatString(infoPtr->m_miscString);
						m_gStatus->m_controlTimeStamp = currentTime;
					}//end delay
					/////////////////////////////////////////////////////////*/

					if (m_gStatus->m_multiplayerObject->GetEnabledClient_safe()
					&& ((currentTime - m_gStatus->m_controlTimeStamp) > 1000)) //delay
					{
						// Get the client's entity
						POSITION pos = m_gStatus->m_entObjRuntime->GetHeadPosition();
						if (!pos) break;
						CMovementObj *pEntity = (CMovementObj*)m_gStatus->m_entObjRuntime->GetNext(pos);
						
						// Request available arena types
						// NOTE: This is an asynchronous call, so we're done after the request
						/*
						m_gStatus->m_netGameLogicObject->SendTextMsg(DPNSEND_GUARANTEED,
							DPID_SERVERPLAYER,
							SCMD_INTERACTION,
							NULL,
							pEntity->List_networkDefinedID,
							NULL,
							TMC_COMMAND);
							*/
						m_gStatus->m_netGameLogicObject->SendTextMsg(DPNSEND_GUARANTEED,
							DPID_SERVERPLAYER,
							pEntity->List_networkDefinedID,
							NULL,
							TMC_COMMAND,
							NULL,
							SCMD_INTERACTION);
						m_gStatus->m_controlTimeStamp = currentTime;
					}
					break;
				case 33:
					m_gStatus->m_netGameLogicObject->UseItemByGLID(((int)infoPtr->List_miscFloat1),m_gStatus->m_entObjRuntime); break;
				case 34:
					if(justPressed)
					{
						POSITION posLast;
						for(POSITION posLoc = m_gStatus->m_runtimeMenuDatabase->GetHeadPosition();(posLast = posLoc) != NULL;)
						{
							CMenuWindow *menu = (CMenuWindow*)m_gStatus->m_runtimeMenuDatabase->GetNext(posLoc);
							if(menu->m_visualID == m_gStatus->m_menuChat->m_visualID)
							{
								//Move it to the front of the menu list
								m_gStatus->m_runtimeMenuDatabase->RemoveAt(posLast);
								m_gStatus->m_runtimeMenuDatabase->AddHead(menu);

								//Set it to focused
								menu->m_isFocused = true;
							}
							else
								menu->m_isFocused = false;
						}
						
						if(m_gStatus->m_menuChatInput)
						{
							//Set focus onto the chat input text box
							m_gStatus->m_menuChatInput->m_isFocused = true;
							m_gStatus->m_menuChatInput->m_fullText	= "/C ";
							m_gStatus->m_menuChatInput->WrapLine();
						}

						//Show the mouse while we're typing
						m_gStatus->m_showMenuCursor++;
					}
					break;
				case 35:
					ptrLoc->m_ovverrideAnimation  = 18;
					ptrLoc->m_ovverrideDuration   = 0;
					ptrLoc->m_ovverrideStamp      = GetTickCount();
					break;
				case 36:
					ptrLoc->m_ovverrideAnimation  = 19;
					ptrLoc->m_ovverrideDuration   = 0;
					ptrLoc->m_ovverrideStamp      = GetTickCount();
					break;
				case 37:
					ptrLoc->m_ovverrideAnimation  = 20;
					ptrLoc->m_ovverrideDuration   = 0;
					ptrLoc->m_ovverrideStamp      = GetTickCount();
					break;
				case 38:
					ptrLoc->m_ovverrideAnimation  = 21;
					ptrLoc->m_ovverrideDuration   = 0;
					ptrLoc->m_ovverrideStamp      = GetTickCount();
					break;
				case 39:
					ptrLoc->m_ovverrideAnimation  = 22;
					ptrLoc->m_ovverrideDuration   = 0;
					ptrLoc->m_ovverrideStamp      = GetTickCount();
					break;
				case 40:
					ptrLoc->m_ovverrideAnimation  = 23;
					ptrLoc->m_ovverrideDuration   = 0;
					ptrLoc->m_ovverrideStamp      = GetTickCount();
					break;
				case 41:
					ptrLoc->m_ovverrideAnimation  = 24;
					ptrLoc->m_ovverrideDuration   = 0;
					ptrLoc->m_ovverrideStamp      = GetTickCount();
					break;
				case 42:
					ptrLoc->m_ovverrideAnimation  = 25;
					ptrLoc->m_ovverrideDuration   = 0;
					ptrLoc->m_ovverrideStamp      = GetTickCount();
					break;
				case 43:
					ptrLoc->m_ovverrideAnimation  = 26;
					ptrLoc->m_ovverrideDuration   = 0;
					ptrLoc->m_ovverrideStamp      = GetTickCount();
					break;
				case 44:
					ptrLoc->m_ovverrideAnimation  = 27;
					ptrLoc->m_ovverrideDuration   = 0;
					ptrLoc->m_ovverrideStamp      = GetTickCount();
					break;
				case 45:
					m_gStatus->ArmEntityItemSys(entityPtr,((int)infoPtr->List_miscFloat1),0); break;
				case 46:
					if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
					if((currentTime-m_gStatus->m_reloadTimerStamp) > 1000)
					{
						m_gStatus->m_netGameLogicObject->SendCommandMsg(6,0,entityPtr->List_networkDefinedID);
						m_gStatus->m_reloadTimerStamp = currentTime;
					}
					break;
				case 47: //Hide Skill Use
				   if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
				   if((currentTime-m_gStatus->m_netGameLogicObject->m_durationToServerStamp) > m_gStatus->m_netGameLogicObject->m_durationToServer)
				   {
						m_gStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED,
																		DPID_SERVERPLAYER,
																		33,
																		-1,
																		entityPtr->List_networkDefinedID,
																		-1,
																		3,
																		NULL);
						m_gStatus->m_netGameLogicObject->m_durationToServerStamp = currentTime;
					}
					break; //End Hide Skill Use
				case 48: //Reveal Skill Use
					if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
					if((currentTime-m_gStatus->m_netGameLogicObject->m_durationToServerStamp) > m_gStatus->m_netGameLogicObject->m_durationToServer)
					{
						m_gStatus->m_netGameLogicObject->SendAttribMsg( DPNSEND_GUARANTEED,
																		DPID_SERVERPLAYER,
																		33,
																		-1,
																		entityPtr->List_networkDefinedID,
																		-1,
																		11,
																		NULL);
						m_gStatus->m_netGameLogicObject->m_durationToServerStamp = currentTime;
					}
					break; //End Reveal Skill Use
				case 49:
					/*if(m_gStatus->m_multiplayerObject->m_groupChatEnabled == FALSE && m_gStatus->m_multiplayerObject->m_chatEnabled == FALSE)
					{
						m_gStatus->m_multiplayerObject->m_clanChatEnabled = TRUE;
						m_gStatus->m_lastChatCommandStamp = GetTickCount();
					}*/

					if(justPressed)
					{
						POSITION posLast;
						for(POSITION posLoc = m_gStatus->m_runtimeMenuDatabase->GetHeadPosition();(posLast = posLoc) != NULL;)
						{
							CMenuWindow *menu = (CMenuWindow*)m_gStatus->m_runtimeMenuDatabase->GetNext(posLoc);
							if(menu->m_visualID == m_gStatus->m_menuChat->m_visualID)
							{
								//Move it to the front of the menu list
								m_gStatus->m_runtimeMenuDatabase->RemoveAt(posLast);
								m_gStatus->m_runtimeMenuDatabase->AddHead(menu);

								//Set it to focused
								menu->m_isFocused = true;
							}
							else
								menu->m_isFocused = false;
						}

						if(m_gStatus->m_menuChatInput)
						{
							//Set focus onto the chat input text box
							m_gStatus->m_menuChatInput->m_isFocused = true;
							m_gStatus->m_menuChatInput->m_fullText	= "/GROUP ";
							m_gStatus->m_menuChatInput->WrapLine();
						}

						//Show the mouse while we're typing
						m_gStatus->m_showMenuCursor++;
					}
					break;
				case 50:
					if((currentTime - m_gStatus->m_netGameLogicObject->m_durationToServerStamp) > m_gStatus->m_netGameLogicObject->m_durationToServer)
					{//1
						POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
						if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
						if(entPosition)
						{//valid Alpha  
							CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
							if(ptrLoc->List_controlType == 0)
								m_gStatus->m_netGameLogicObject->SendAttribMsg( DPNSEND_GUARANTEED,
																				DPID_SERVERPLAYER,
																				33,
																				(int)infoPtr->List_miscFloat1,//ability selection
																				ptrLoc->List_networkDefinedID,
																				(int)infoPtr->List_miscFloat2,//invObj->m_itemData->m_requiredSkillLevel,
																				(short)infoPtr->List_miscFloat3,//
																				NULL);//invObj->m_itemData->m_requiredSkillType);
						}//end valid Alpha  
						m_gStatus->m_netGameLogicObject->m_durationToServerStamp = currentTime;
					}
					break;
				case 51:
					ptrLoc->m_ovverrideAnimation  = 28;
					ptrLoc->m_ovverrideDuration   = 0;
					ptrLoc->m_ovverrideStamp      = GetTickCount();
					break;
				case 52:
					ptrLoc->m_ovverrideAnimation  = 29;
					ptrLoc->m_ovverrideDuration   = 0;
					ptrLoc->m_ovverrideStamp      = GetTickCount();
					break;
				case 53: //Open Inventory
					if(justPressed)
					{
						//Attempt to Close Menu
						bool bClosed = m_gStatus->m_runtimeMenuDatabase->CloseByID(ID_PLAYER_INVENTORY_MENU, &m_gStatus->m_showMenuCursor);
						bool bClose2 = m_gStatus->m_runtimeMenuDatabase->CloseByID(ID_RESISTANCE_MENU, &m_gStatus->m_showMenuCursor);
						
						//Open Menu if Wasn't Just Closed
						if(!bClosed)
							m_gStatus->OpenMenu(ID_PLAYER_INVENTORY_MENU, false);
						if(!bClose2)
							m_gStatus->OpenMenu(ID_RESISTANCE_MENU, false);
					}
					break;
				case 54: //Toggle Resistances
					if(justPressed)
					{
						if(CMenu3DObj *menu = m_gStatus->m_runtimeMenuDataBase->GetByID(79))
							menu->CloseMenu(NULL);
						else
							m_gStatus->OpenMenu(79, false); //TODO: Switch this to use a value from the "important menu" array
					}
					break;
				case 55://Toggle Journal
					if(justPressed)
					{
						if(CMenu3DObj *menu = m_gStatus->m_runtimeMenuDataBase->GetByID(200))
							menu->CloseMenu(NULL);
						else
							m_gStatus->OpenMenu(200, false); //TODO: Switch this to use a value from the "important menu" array
					}
					break;
				case 56://Toggle View Mode
					if(justPressed)
					{
						if(entityPtr->m_firstPersonMode == TRUE)
							m_gStatus->ChangeToThirdPersonView(entityPtr);
						else
							m_gStatus->ChangeToFirstPersonView(entityPtr);
					}
					break;
				case 57://Show Options
					if(justPressed)
					{
						if(CMenu3DObj *menu = m_gStatus->m_runtimeMenuDataBase->GetByID(72))
							menu->CloseMenu(NULL);
						else
							m_gStatus->OpenMenu(72, false); //TODO: Switch this to use a value from the "important menu" array
					}
					break;
				case 58://Show Functions
					if(justPressed)
					{
						if(CMenu3DObj *menu = m_gStatus->m_runtimeMenuDataBase->GetByID(206))
							menu->CloseMenu(NULL);
						else
							m_gStatus->OpenMenu(206, false); //TODO: Switch this to use a value from the "important menu" array
					}
					break;
				case 59://Show Character Screen
					if(justPressed)
					{
						//Attempt to Close Menu
					/*	bool bClosed = m_gStatus->m_runtimeMenuDatabase->CloseByID(CHARACTER_MENU, &m_gStatus->m_showMenuCursor);
						
						//Open Menu if Wasn't Just Closed
						if(!bClosed)
							m_gStatus->OpenMenu(309, false);*/
					}
					break;
				case 60://Autorun
					if(justPressed)
						m_bToggleAutoRun = !m_bToggleAutoRun;	
					break;
				/*
				case 61://Use Quick Item 1
					if(m_gStatus->m_multiplayerObject->Quickbar(0,0) > -1)
						m_gStatus->ActivateItem(m_gStatus->m_multiplayerObject->Quickbar(0,0));
					break;
				case 62://Use Quick Item 2
					if(m_gStatus->m_multiplayerObject->Quickbar(0,1) > -1)
						m_gStatus->ActivateItem(m_gStatus->m_multiplayerObject->Quickbar(0,1));
					break;
				case 63://Use Quick Item 3
					if(m_gStatus->m_multiplayerObject->Quickbar(0,2) > -1)
						m_gStatus->ActivateItem(m_gStatus->m_multiplayerObject->Quickbar(0,2));
					break;
				case 64://Use Quick Item 4
					if(m_gStatus->m_multiplayerObject->Quickbar(0,3) > -1)
						m_gStatus->ActivateItem(m_gStatus->m_multiplayerObject->Quickbar(0,3));
					break;
				case 65://Use Quick Item 5
					if(m_gStatus->m_multiplayerObject->Quickbar(0,4) > -1)
						m_gStatus->ActivateItem(m_gStatus->m_multiplayerObject->Quickbar(0,4));
					break;
				*/
				// Use Quick Item 1-10
				case 61:
				case 62:
				case 63:
				case 64:
				case 65:
				case 66:
				case 67:
				case 68:
				case 69:
//				case 73:
//				case 74:
				case 70:
				{
					if(GetTickCount()-m_lastQuickbarStamp < 50)
						break;//avoid spam

					
					// IF we're not in the game client, then BREAK
					if (!this->m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
						break;

					m_lastQuickbarStamp = GetTickCount();

					// IF any of these menus are open, DISABLE access to the Quickbars
					CMenuWindow *pBankMenu = dynamic_cast<CMenuWindow*>(this->m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ID_BANK_INVENTORY_MENU, true));
					CMenuWindow *pStoreMenu = dynamic_cast<CMenuWindow*>(this->m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ID_STORE_MENU, true));
					CMenuWindow *pTradeMenu = dynamic_cast<CMenuWindow*>(this->m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_MENU, true));
					if (pBankMenu || pStoreMenu || pTradeMenu)
						break;

					CItemObj *pItem = NULL;
					int iGLID = -1, iSlotID = 0;

					// GET the slot ID
					switch(infoPtr->List_action)
					{
						case 61: this->m_gStatus->m_multiplayerObject->m_iQuickbarLastActivatedIndex = iSlotID = 0; break;
						case 62: this->m_gStatus->m_multiplayerObject->m_iQuickbarLastActivatedIndex = iSlotID = 1; break;
						case 63: this->m_gStatus->m_multiplayerObject->m_iQuickbarLastActivatedIndex = iSlotID = 2; break;
						case 64: this->m_gStatus->m_multiplayerObject->m_iQuickbarLastActivatedIndex = iSlotID = 3; break;
						case 65: this->m_gStatus->m_multiplayerObject->m_iQuickbarLastActivatedIndex = iSlotID = 4; break;
						case 66: this->m_gStatus->m_multiplayerObject->m_iQuickbarLastActivatedIndex = iSlotID = 5; break;
						case 67: this->m_gStatus->m_multiplayerObject->m_iQuickbarLastActivatedIndex = iSlotID = 6; break;
						case 68: this->m_gStatus->m_multiplayerObject->m_iQuickbarLastActivatedIndex = iSlotID = 7; break;
						case 69: this->m_gStatus->m_multiplayerObject->m_iQuickbarLastActivatedIndex = iSlotID = 8; break;
						case 70: this->m_gStatus->m_multiplayerObject->m_iQuickbarLastActivatedIndex = iSlotID = 9; break;
	//					case 73: this->m_gStatus->m_multiplayerObject->m_iQuickbarLastActivatedIndex = iSlotID = 10; break;
	//					case 74: this->m_gStatus->m_multiplayerObject->m_iQuickbarLastActivatedIndex = iSlotID = 11; break;
						default: break;
					}

					// GET the item in the current Quick Slot
					CMenuInventoryItem **pInventoryItem = this->m_gStatus->m_multiplayerObject->Quickbar(0, iSlotID);
					if (!(*pInventoryItem)) break;
					iGLID = (*pInventoryItem)->m_data.m_GLID;
					pItem = this->m_gStatus->m_itemManager->GetItemByID(iGLID);

					// GET reference to menu visuals (if available)
					CMenuInventory *pInventory = dynamic_cast<CMenuInventory*>(this->m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY, true));

					// IF the item is weapon OR armor, arm it...
					if (pItem)
					{
						// EQUIP weapons/armor quickly...
						//	ERROR checking: We have to check for required skill levels!
					/*	if ((pItem->m_disarmable)
						&& (entityPtr->m_level >= pItem->m_requiredSkillLevel))
						{							
							// SEND equip message to the server
							this->m_gStatus->m_netGameLogicObject->SendEquipMsg(iGLID,
								entityPtr->List_networkDefinedID, NULL, 5, 0);

							// ARM the item locally
							CMovementObj *DBPtr = this->m_gStatus->m_entObjList->GetByID(entityPtr->List_refModel);
							if (DBPtr)
								entityPtr->ArmItemByGLID(iGLID, DBPtr, this->m_gStatus->m_itemManager, NULL);
						}
						// USE non-weapon/armor items with anti-spam delay...
						else
						{
							m_gStatus->ActivateItem(*pInventoryItem);
						}
						*/  
						m_gStatus->ActivateItem(*pInventoryItem);

						// IF the item is equipped, SET the equip selection indicator
						if (pInventory)
							pInventory->SetSelectionIndicator(*pInventoryItem, CMenuInventory::IST_EQUIPPED, true);
					}
					break;
				}
				case 71: // Auto-arm all quick items
				{
					//if(GetTickCount()-m_lastQuickbarStamp < 50)
						//break;//avoid spam
					// IF we're not in the game client, then BREAK
					if (!this->m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
						break;

					CItemObj *pItem = NULL;
					int iGLID = -1, iDupePassed = 0;

					// LOOP through all Quick Slots
					for (int i=0; i<NUM_QUICKBARS; i++)
					{
						for (int j=0; j<NUM_QUICKBAR_SLOTS; j++)
						{
							// GET the item in the current Quick Slot
							CMenuInventoryItem **pInventoryItem = this->m_gStatus->m_multiplayerObject->Quickbar(i, j);
							if (!(*pInventoryItem)) continue;
							int iGLID = (*pInventoryItem)->m_data.m_GLID;
							pItem = this->m_gStatus->m_itemManager->GetItemByID(iGLID);
							if (!pItem) continue; // item no longer known to the client

							/*/ IF the item is weapon OR armor, arm it...
							if (pItem && (pItem->m_useType == 0))
							{
								// SEND equip message to the server
								this->m_gStatus->m_netGameLogicObject->SendEquipMsg(iGLID,
									entityPtr->List_networkDefinedID, NULL, 5, 0);

								// ARM the item locally
								CMovementObj *DBPtr = this->m_gStatus->m_entObjList->GetByID(entityPtr->List_refModel);
								if (DBPtr)
									entityPtr->ArmItemByGLID(iGLID, DBPtr, this->m_gStatus->m_itemManager, NULL);
							}*/
							// EQUIP weapons/armor quickly...
							//	ERROR checking: We have to check for required skill levels!
							if ((pItem->m_disarmable)
							&& (entityPtr->m_level >= pItem->m_requiredSkillLevel))
							{							
								// SEND equip message to the server
								this->m_gStatus->m_netGameLogicObject->SendEquipMsg(iGLID,
									entityPtr->List_networkDefinedID, NULL, 5, 0);

								// ARM the item locally
								/*CMovementObj *DBPtr = this->m_gStatus->m_entObjList->GetByID(entityPtr->List_refModel);
								if (DBPtr)
									entityPtr->ArmItemByGLID(iGLID, DBPtr, this->m_gStatus->m_itemManager, NULL);*/
							}
							// USE non-weapon/armor items with anti-spam delay...
							{
								m_gStatus->ActivateItem(*pInventoryItem);
							}
						}
					}
					break;
				}
				case 72: //Open Quest Journal
				{
					if(justPressed)//Attempt to Close Menu
					{
						bool bClosed = m_gStatus->m_runtimeMenuDatabase->CloseByID(QUEST_JOURNAL_MENU, &m_gStatus->m_showMenuCursor);
					
						//Open Menu if Wasn't Just Closed
						if(!bClosed)
							m_gStatus->OpenMenu(QUEST_JOURNAL_MENU, false);
					}
							
					break;
				}
			/*	case 73: //Open Badges
				{
					if(justPressed)//Attempt to Close Menu
					{
						bool bClosed = m_gStatus->m_runtimeMenuDatabase->CloseByID(EMBLEMS_MENU, &m_gStatus->m_showMenuCursor);
					
						//Open Menu if Wasn't Just Closed
						if(!bClosed)
							m_gStatus->OpenMenu(EMBLEMS_MENU, false);
						else
						{
							m_gStatus->ShowHoverText(ID_HOVER_TEXT_MENU, "");
							m_gStatus->m_runtimeMenuDatabase->CloseByID(ID_HOVER_TEXT_MENU, &m_gStatus->m_showMenuCursor);
						}
					}
							
					break;
				} */
			}
		}//end if a mouse or keyboard button is down
	}//end info loop

	//Run forward if auto run is currently on
	if(m_bToggleAutoRun)
		Accellerate(ptrLoc->List_movementDB, 0.0f, m_gStatus->m_timeAdjustFloat);

	//QUICK BAR USER CONTROLS
	//THESE KEYS CANT BE CHANGED SO THEY ARE BEING HARD CODED
	//ALT + NUMBERS 1 - 0
	/*
	if(m_input->m_altOn && m_input->m_keyboardKeys[DIK_1].JustPressed())
		SetQuickItem(1,0);
	else if(m_input->m_altOn && m_input->m_keyboardKeys[DIK_2].JustPressed())
		SetQuickItem(1,1);
	else if(m_input->m_altOn && m_input->m_keyboardKeys[DIK_3].JustPressed())
		SetQuickItem(1,2);
	else if(m_input->m_altOn && m_input->m_keyboardKeys[DIK_4].JustPressed())
		SetQuickItem(1,3);
	else if(m_input->m_altOn && m_input->m_keyboardKeys[DIK_5].JustPressed())
		SetQuickItem(1,4);
	else if(m_input->m_altOn && m_input->m_keyboardKeys[DIK_6].JustPressed())
		SetQuickItem(1,5);
	else if(m_input->m_altOn && m_input->m_keyboardKeys[DIK_7].JustPressed())
		SetQuickItem(1,6);
	else if(m_input->m_altOn && m_input->m_keyboardKeys[DIK_8].JustPressed())
		SetQuickItem(1,7);
	else if(m_input->m_altOn && m_input->m_keyboardKeys[DIK_9].JustPressed())
		SetQuickItem(1,8);
	else if(m_input->m_altOn && m_input->m_keyboardKeys[DIK_0].JustPressed())
		SetQuickItem(1,9);

	//SHIFT + NUMBERS 1 - 0
	else if(m_input->m_shiftOn && m_input->m_keyboardKeys[DIK_1].JustPressed())
		SetQuickItem(2,0);
	else if(m_input->m_shiftOn && m_input->m_keyboardKeys[DIK_2].JustPressed())
		SetQuickItem(2,1);
	else if(m_input->m_shiftOn && m_input->m_keyboardKeys[DIK_3].JustPressed())
		SetQuickItem(2,2);
	else if(m_input->m_shiftOn && m_input->m_keyboardKeys[DIK_4].JustPressed())
		SetQuickItem(2,3);
	else if(m_input->m_shiftOn && m_input->m_keyboardKeys[DIK_5].JustPressed())
		SetQuickItem(2,4);
	else if(m_input->m_shiftOn && m_input->m_keyboardKeys[DIK_6].JustPressed())
		SetQuickItem(2,5);
	else if(m_input->m_shiftOn && m_input->m_keyboardKeys[DIK_7].JustPressed())
		SetQuickItem(2,6);
	else if(m_input->m_shiftOn && m_input->m_keyboardKeys[DIK_8].JustPressed())
		SetQuickItem(2,7);
	else if(m_input->m_shiftOn && m_input->m_keyboardKeys[DIK_9].JustPressed())
		SetQuickItem(2,8);
	else if(m_input->m_shiftOn && m_input->m_keyboardKeys[DIK_0].JustPressed())
		SetQuickItem(2,9);

	//NUMBERS 1 - 0
	else if(m_input->m_keyboardKeys[DIK_1].JustPressed())
		SetQuickItem(0,0);
	else if(m_input->m_keyboardKeys[DIK_2].JustPressed())
		SetQuickItem(0,1);
	else if(m_input->m_keyboardKeys[DIK_3].JustPressed())
		SetQuickItem(0,2);
	else if(m_input->m_keyboardKeys[DIK_4].JustPressed())
		SetQuickItem(0,3);
	else if(m_input->m_keyboardKeys[DIK_5].JustPressed())
		SetQuickItem(0,4);
	else if(m_input->m_keyboardKeys[DIK_6].JustPressed())
		SetQuickItem(0,5);
	else if(m_input->m_keyboardKeys[DIK_7].JustPressed())
		SetQuickItem(0,6);
	else if(m_input->m_keyboardKeys[DIK_8].JustPressed())
		SetQuickItem(0,7);
	else if(m_input->m_keyboardKeys[DIK_9].JustPressed())
		SetQuickItem(0,8);
	else if(m_input->m_keyboardKeys[DIK_0].JustPressed())
		SetQuickItem(0,9);
	*/
	//END QUICK BAR USER CONTROLS
}

//--------------Function causes a freelook to pitch down--------------------------------//
void UserController::ZoomOut(CMovementObj * ptrLoc,CCameraObj * camPtrRef)
{
if(!camPtrRef)
return;

	if(ptrLoc->List_movementDB->List_targetFov < ptrLoc->List_movementDB->List_capabilityFovMax)
	{
	ptrLoc->List_movementDB->List_lastFov = camPtrRef->List_fovView;
	ptrLoc->List_movementDB->List_fovCurrentInterp = 0;
	ptrLoc->List_movementDB->List_targetFov += ptrLoc->List_movementDB->List_capFovChangeSensitivity*m_gStatus->m_timeAdjustFloat;	
  }
}
//--------------Function causes a freelook to pitch down--------------------------------//
void UserController::ZoomIn(CMovementObj * ptrLoc,CCameraObj * camPtrRef)
{
if(!camPtrRef)
return;

	if(ptrLoc->List_movementDB->List_targetFov > ptrLoc->List_movementDB->List_capabilityFovMin)
	{
	 ptrLoc->List_movementDB->List_lastFov = camPtrRef->List_fovView;
	 ptrLoc->List_movementDB->List_fovCurrentInterp = 0;
	 ptrLoc->List_movementDB->List_targetFov -= ptrLoc->List_movementDB->List_capFovChangeSensitivity*m_gStatus->m_timeAdjustFloat;	  
  }
}
//----------------------------------------------------------//
void UserController::MouseKeyboardUpdate()
{
	EnterCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);

	if(m_gStatus->m_dxRenderer->m_showMouse)
	{
		ClearInputStates();
		LeaveCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
		return;
	}

	if(!m_input->m_directInput)
	{
		LeaveCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
		return;
	}
	if(!m_input->m_directMouse)
	{
		LeaveCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
		AcquireInputDevices();
		return;
	}
	if(!m_input->m_directKeyboard)
	{
		LeaveCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
		AcquireInputDevices();
		return;
	}

	if(!m_input->UpdateInput(m_gStatus->GetTimeStamp_DW()))
	{
		LeaveCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
		AcquireInputDevices();
		return;
	}

	LeaveCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
}
//----------------------------------------------------------//
void UserController::ClearInputStates()
{
	ZeroMemory(&m_input->m_mousepointTarget,sizeof(DIMOUSESTATE));
	m_input->m_mouseHiResX = 0.0f;
	m_input->m_mouseHiResY = 0.0f;

	DWORD curTime = m_gStatus->GetTimeStamp_DW();
	for(int i = 0; i < KEYBOARD_MAX_KEYS; i++)
		m_input->m_keyboardKeys[i].UpdateState(false, curTime);
	
	for(int i = 0; i < MOUSE_MAX_BUTTONS; i++)
		m_input->m_mouseButtons[i].UpdateState(false, curTime);
}
//----------------------------------------------------------//
void UserController::MouseManagerCallback()
{
	EnterCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
	if(m_gStatus->m_dxRenderer->m_fullScreenMode == TRUE)
	{
		if(m_gStatus->m_dxRenderer->m_showMouse == TRUE)
		{
		   //ShowWinCursor(FALSE);
		   //::ShowCursor(FALSE);
           LeaveCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
		   if(AcquireInputDevices()==TRUE)
		   {
		      EnterCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
              m_gStatus->m_dxRenderer->m_showMouse = FALSE;
		      m_gStatus->m_dxRenderer->m_mousePendingCommand = 0;
			  LeaveCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
		   }
		   EnterCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
		}
		LeaveCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
		return;
	}

	if(!m_gStatus->m_dxRenderer->m_mouseManagerEnabled)
	{
		LeaveCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
	    return;
	}

	

    if(m_gStatus->m_dxRenderer->m_mousePendingCommand != 0)
	{
       if(m_gStatus->m_dxRenderer->m_mousePendingCommand == 2)
	   {//disable if need be
         if(m_gStatus->m_dxRenderer->m_showMouse == TRUE)
		 {
            LeaveCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
		    if(AcquireInputDevices()==TRUE)
			{
			   EnterCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
		       m_gStatus->m_dxRenderer->m_showMouse = FALSE;
			   m_gStatus->m_dxRenderer->m_mousePendingCommand = 0;
			   LeaveCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
			}
			EnterCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
		 }
	   }
	   else if(m_gStatus->m_dxRenderer->m_mousePendingCommand == 1)
	   {
         if(m_gStatus->m_dxRenderer->m_showMouse == FALSE)
		 {
            
		    m_gStatus->m_dxRenderer->m_showMouse = TRUE;
			LeaveCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
			UnacquireInputDevices();
			EnterCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
			m_gStatus->m_dxRenderer->m_mousePendingCommand = 0;
		 }
	   }
	}

	ShowWinCursor(m_gStatus->m_dxRenderer->m_showMouse);

    /*if(m_gStatus->m_dxRenderer->m_showMouse == FALSE)
	{//handle unfocus if not then lock curson to center of window handle
		if(::GetForegroundWindow()!=m_gStatus->m_dxRenderer->m_topWindowHandle && m_gStatus->m_dxRenderer->m_fullScreenMode == FALSE)
		{
           //::ShowCursor(TRUE);
		   m_gStatus->m_dxRenderer->m_mousePendingCommand = 2;
		   //m_gStatus->m_dxRenderer->m_showMouse = TRUE;
		   //ReleaseExcDevices();
		   //LeaveCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
		   //return;
		}
	}//end handle unfocus if not then lock curson to center of window handle*/
	LeaveCriticalSection(&m_gStatus->m_dxRenderer->m_mouseManagerSection);
}

//--------------Function causes a freelook to pitch down--------------------------------//
void UserController::ZoomManageMent(CMovementObj * ptrLoc,CCameraObj * camPtrRef)
{
 if(!camPtrRef){  
	 return;	
 }
int hardcodeInterpolation = 20;

   if(ptrLoc->List_movementDB->List_targetFov != camPtrRef->List_fovView){
      if(ptrLoc->List_movementDB->List_fovCurrentInterp > hardcodeInterpolation)
		 ptrLoc->List_movementDB->List_fovCurrentInterp = hardcodeInterpolation;
	     if(ptrLoc->List_movementDB->List_fovCurrentInterp!=hardcodeInterpolation)
            camPtrRef->List_fovView = m_gStatus->m_matLib.InterpolateFloats(
						                                    ptrLoc->List_movementDB->List_lastFov, 
						                                    ptrLoc->List_movementDB->List_targetFov,
					                                        hardcodeInterpolation, 
					                                        ptrLoc->List_movementDB->List_fovCurrentInterp);
         if(camPtrRef->List_fovView > ptrLoc->List_movementDB->List_capabilityFovMax)
		    camPtrRef->List_fovView = ptrLoc->List_movementDB->List_capabilityFovMax;
		 if(camPtrRef->List_fovView < ptrLoc->List_movementDB->List_capabilityFovMin)
		    camPtrRef->List_fovView = ptrLoc->List_movementDB->List_capabilityFovMin;
		 
		 
		 ptrLoc->List_movementDB->List_fovCurrentInterp++;
   }
}
//-------------------------------------------
CString UserController::GetPossibleSymbols(int startIndex,CString curSection,int * totalCount)
{
     CString tempBuild = "";

     if(!m_gStatus->m_symbolMapDB)
		 return "";
	 int temp = 0;
     for(POSITION posLoc = m_gStatus->m_symbolMapDB->GetHeadPosition();posLoc != NULL;)
	 {
         CSymbolMapperObj * symMapObj = (CSymbolMapperObj *)m_gStatus->m_symbolMapDB->GetNext(posLoc);
		 if(symMapObj->m_group == curSection)
            temp++;
	 }

	 *totalCount = temp;


	 int tracer = 0;
	 int tracer2 = 0;
	 for(POSITION posLoc = m_gStatus->m_symbolMapDB->GetHeadPosition();posLoc != NULL;)
	 {
         CSymbolMapperObj * symMapObj = (CSymbolMapperObj *)m_gStatus->m_symbolMapDB->GetNext(posLoc);
		 if(symMapObj->m_group == curSection)
		 {
			if(tracer2 < startIndex)
			{
               tracer2++;
               continue;
			}

            CString numericRef = m_gStatus->m_cabLib.ConvToString(tracer);
            if(symMapObj->m_extendedOption.GetLength() > 0)
			tempBuild += numericRef+")" + symMapObj->m_extendedOption + " ";
			else	
			tempBuild += numericRef+")" + symMapObj->m_uniqueID + " ";
			
			tracer++;
			if(tracer == 10)
			   break;
		 }
	 }
	 return tempBuild;
}
//--------------mouse turn left-------------------------------------------------------//
void UserController::MouseTurnLeft(CMovementCaps *movDB,float mouseControlXLoc)
{
if(movDB->List_maxYaw==0)
   return;
        if(mouseControlXLoc < 0.0f){//begin left
           //--------------------actual turn mech--------------------
	       movDB->List_curRotY = (float)(movDB->List_curRotY + fabs(mouseControlXLoc));
	       if(movDB->List_maxYaw < movDB->List_curRotY)
	          movDB->List_curRotY = movDB->List_maxYaw;
		}//end begin turn left
}
//--------------mouse turn right------------------------------------------------------//
void UserController::MouseTurnRight(CMovementCaps *movDB,float mouseControlXLoc)	
{		
if(movDB->List_maxYaw==0)
   return;
		if(mouseControlXLoc > 0.0f){//begin right
	    //--------------------actual turn mech--------------------
        //movDB->List_curRotY -= movDB->List_yawAccel;
	    movDB->List_curRotY = (float)(movDB->List_curRotY - fabs(mouseControlXLoc));
	    if(-movDB->List_maxYaw > movDB->List_curRotY)
	       movDB->List_curRotY = -movDB->List_maxYaw;
		}//end turn right
}
//--------------mouse turn pullup-----------------------------------------------------//
void UserController::MouseTurnPullUp(CMovementCaps *movDB,float mouseControlYLoc)	
{
	if(movDB->List_maxPitch==0)
	   return;

        if(mouseControlYLoc < 0.0f){//begin pitch1
	       //movDB->List_curRotX += movDB->List_pitchAccel;
	       movDB->List_curRotX = (float)(movDB->List_curRotX + fabs(mouseControlYLoc));
	       if(movDB->List_maxPitch < movDB->List_curRotX)
	          movDB->List_curRotX = movDB->List_maxPitch;	
		}//end pitch1
}
//--------------mouse turn dive-------------------------------------------------------//
void UserController::MouseTurnDive(CMovementCaps *movDB,float mouseControlYLoc)	
{
if(movDB->List_maxPitch==0)
	   return;

        if(mouseControlYLoc > 0.0f){//begin pitch2
	       //movDB->List_curRotX -= movDB->List_pitchAccel;
	       movDB->List_curRotX = (float)(movDB->List_curRotX - fabs(mouseControlYLoc));
	    if(-movDB->List_maxPitch > movDB->List_curRotX)
	       movDB->List_curRotX = -movDB->List_maxPitch;
		}//end pitch 2
}
//--------------Function accellerate--------------------------------------------------//
void UserController::Accellerate(CMovementCaps *movDB,float overRide,float & timeAdjust)
{

   if(overRide == 0.0f){
    movDB->List_curTransZ += movDB->List_accellerate*timeAdjust;
    if(movDB->List_curTransZ > movDB->List_maxSpeed)
       movDB->List_curTransZ = movDB->List_maxSpeed; 
   }
   else{
    if(movDB->List_curTransZ > overRide)
       movDB->List_curTransZ -= (movDB->List_accellerate*.5f)*timeAdjust;
	else
	   movDB->List_curTransZ += movDB->List_accellerate*timeAdjust;
	
	if(movDB->List_curTransZ > movDB->List_maxSpeed)
       movDB->List_curTransZ = movDB->List_maxSpeed; 
   }
   movDB->m_speedControlInUse = TRUE;
}
//--------------Function decellerate--------------------------------------------------//
void UserController::Deccellerate(CMovementCaps *movDB)
{
   if(movDB->List_maxSpeed > 0.0f && movDB->List_minSpeed > -movDB->List_maxSpeed * 0.85f)
      movDB->List_minSpeed = -movDB->List_maxSpeed * 0.85f;

   movDB->List_curTransZ -= movDB->List_accellerate*m_gStatus->m_timeAdjustFloat;
   if(movDB->List_curTransZ < movDB->List_minSpeed)
	  movDB->List_curTransZ = movDB->List_minSpeed;
      
      movDB->m_speedControlInUse = TRUE;
}
//--------------sidestep right--------------------------------------------------------//
void UserController::SideStepRight(CMovementCaps *movDB)
{
   if(movDB->m_strafeReduction > 1.0)
	  movDB->m_strafeReduction = 1.0f;

   movDB->List_curTransX += (movDB->List_accellerate*movDB->m_strafeReduction)*m_gStatus->m_timeAdjustFloat;
   if(movDB->List_curTransX > (movDB->List_maxSpeed*movDB->m_strafeReduction))
      movDB->List_curTransX = (movDB->List_maxSpeed*movDB->m_strafeReduction);

      movDB->m_sidestepsInUse = TRUE;
}
//--------------sidestep right--------------------------------------------------------//
void UserController::SideStepLeft(CMovementCaps *movDB)
{
   
   if(movDB->m_strafeReduction > 1.0)
	  movDB->m_strafeReduction = 1.0f;
	
	
   movDB->List_curTransX -= (movDB->List_accellerate*movDB->m_strafeReduction)*m_gStatus->m_timeAdjustFloat;
   if(movDB->List_curTransX < -(movDB->List_maxSpeed*movDB->m_strafeReduction))
      movDB->List_curTransX = -(movDB->List_maxSpeed*movDB->m_strafeReduction); 
   
      movDB->m_sidestepsInUse = TRUE;
}
//--------------Function causes a character to jump baby------------------------------//
void UserController::JumpMovementClassObj(CMovementObj * ptrLoc)
{
    //return;
    long curTime = (long)GetTickCount();
    if((curTime - ptrLoc->List_movementDB->m_grvtyChngTimeStmp) < 100)
		return;

	if((curTime - ptrLoc->List_movementDB->m_jumpRecoveryStamp) < ((long)ptrLoc->List_movementDB->m_jumpRecoverTime))
		return;

	if(ptrLoc->List_movementDB->List_currentlyJumping == FALSE && ptrLoc->List_movementDB->List_doGravity == FALSE){
	  ptrLoc->List_movementDB->m_burstForceY = ptrLoc->List_movementDB->List_jumpPower;
      ptrLoc->List_movementDB->List_currentlyJumping = TRUE;
	  ptrLoc->List_movementDB->m_jumpRecoveryStamp = GetTickCount();
	}
}
//--------------Function causes a camera to pitch up----------------------------------//
void UserController::FreeLookUp(CMovementObj * ptrLoc)
{
if(!ptrLoc->List_movementDB->List_freeLookRefFrame)
   return;

             ptrLoc->List_movementDB->List_freeLookCurPitch += (ptrLoc->List_movementDB->List_freeLookSpeed);
			 if(ptrLoc->List_movementDB->List_freeLookCurPitch <= ptrLoc->List_movementDB->List_freeLookCurMaxPitch){
	           D3DMATRIX matrixLoc;
	           m_gStatus->m_matLib.Rotate (((ptrLoc->List_movementDB->List_freeLookSpeed)),0.0f,0.0f,matrixLoc.m);
	           ptrLoc->List_movementDB->List_freeLookRefFrame->AddTransformBefore(matrixLoc); 
			 }
			 else
			   ptrLoc->List_movementDB->List_freeLookCurPitch -= (ptrLoc->List_movementDB->List_freeLookSpeed);
}
//--------------Function causes a freelook to pitch down--------------------------------//
void UserController::FreeLookDown(CMovementObj * ptrLoc)
{
if(!ptrLoc->List_movementDB->List_freeLookRefFrame)
   return;

	           ptrLoc->List_movementDB->List_freeLookCurPitch -= (ptrLoc->List_movementDB->List_freeLookSpeed); 
			   if(ptrLoc->List_movementDB->List_freeLookCurPitch >= -ptrLoc->List_movementDB->List_freeLookCurMaxPitch){
	            D3DMATRIX matrixLoc;
	            m_gStatus->m_matLib.Rotate (((-ptrLoc->List_movementDB->List_freeLookSpeed)),0.0f,0.0f,matrixLoc.m);
	            ptrLoc->List_movementDB->List_freeLookRefFrame->AddTransformBefore(matrixLoc); 
			   }
			   else
			   ptrLoc->List_movementDB->List_freeLookCurPitch += (ptrLoc->List_movementDB->List_freeLookSpeed); 
}
//--------------Function causes a freelook to pitch down--------------------------------//
void UserController::FreeLookLeft(CMovementObj * ptrLoc)
{
if(!ptrLoc->List_movementDB->List_freeLookRefFrame)
   return;

	           ptrLoc->List_movementDB->List_freeLookCurYaw -= (ptrLoc->List_movementDB->List_freeLookSpeed); 
			   if(ptrLoc->List_movementDB->List_freeLookCurYaw >= -ptrLoc->List_movementDB->List_freeLookCurMaxYaw){
	            D3DMATRIX matrixLoc;
	            m_gStatus->m_matLib.Rotate (0.0f,((-ptrLoc->List_movementDB->List_freeLookSpeed)),0.0f,matrixLoc.m);
	            ptrLoc->List_movementDB->List_freeLookRefFrame->AddTransformAfter(matrixLoc); 
			   }
			   else
			   ptrLoc->List_movementDB->List_freeLookCurYaw += (ptrLoc->List_movementDB->List_freeLookSpeed); 
}
//--------------Function causes a freelook to pitch down--------------------------------//
void UserController::FreeLookRight(CMovementObj * ptrLoc)
{
if(!ptrLoc->List_movementDB->List_freeLookRefFrame)
   return;

	         ptrLoc->List_movementDB->List_freeLookCurYaw += (ptrLoc->List_movementDB->List_freeLookSpeed);
			 if(ptrLoc->List_movementDB->List_freeLookCurYaw <= ptrLoc->List_movementDB->List_freeLookCurMaxYaw){
	           D3DMATRIX matrixLoc;
	           m_gStatus->m_matLib.Rotate (0.0f,((ptrLoc->List_movementDB->List_freeLookSpeed)),0.0f,matrixLoc.m);
	           ptrLoc->List_movementDB->List_freeLookRefFrame->AddTransformAfter(matrixLoc); 
			 }
			 else
			   ptrLoc->List_movementDB->List_freeLookCurYaw -= (ptrLoc->List_movementDB->List_freeLookSpeed);
}
//-----------bail from (vehicle)entity if utilizing-----------------------------------//
BOOL UserController::BailOutEntity(CMovementObj * currentEntity)
{
if(currentEntity->List_controlType != 0 || currentEntity->List_possesedBy == -1)
return FALSE;

currentEntity->List_movementDB->List_curSpeed = 0.0f;
currentEntity->List_movementDB->List_curRotX=0.0f;
currentEntity->List_movementDB->List_curRotY=0.0f;
BOOL networkEntity = currentEntity->List_networkEntity;
currentEntity->List_networkEntity = FALSE;
int origionalEntityRefModel = currentEntity->List_refModel;
int controlAssignMent = currentEntity->List_controlType;
int targetEntityRefModel = currentEntity->List_possesedBy;
D3DMATRIX targetMatrix;
currentEntity->List_baseFrame->GetTransform(NULL,&targetMatrix);
DPID networkID = currentEntity->List_idFromLoc;
//spawn in to replace vehicle ;)
m_gStatus->ActivateMovementEntity(  NULL,
					                0,
					                targetMatrix,
					                0,//int cameraIndex,
					                0,//int vportIndex,
					                targetEntityRefModel,//int indexLoc,
									controlAssignMent,//int controlAssignment,
									0,//int entityType,
									networkID,//DPNID idFromLoc,
									networkEntity,//BOOL networkEntity,
									TRUE,
									FALSE,//BOOL addHead)
									-1,
									-1,
									NULL,
									NULL,
									-1,
									-1,
									m_gStatus->m_collisionBase,
									m_gStatus->m_collisionDatabase,
									m_gStatus->m_entObjRuntime,
									m_gStatus->m_currentWorkingFile);
//basically have the other entity become static / empty again
currentEntity->List_controlType = 10;

CMovementObj * focusLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetHead();
m_gStatus->m_matLib.Translate (40.0f,
		                      10.0f,
		                      40.0f,
		                      targetMatrix.m);
focusLoc->List_baseFrame->AddTransformAfter(targetMatrix);
return TRUE;
}

//------------------------------------------------------------------------------------//
BOOL UserController::ArmNextWeapon(CMovementObj * movObj,BOOL override)
{

	if(movObj->List_energy <= 0)
	   return FALSE;
    

	if(override == FALSE){
     if(((long)GetTickCount() - m_gStatus->m_actionDelayStamp) < m_gStatus->m_actionDelay)
	  	 return FALSE;
	     m_gStatus->m_actionDelayStamp = GetTickCount(); 
	}
    
    if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe()){
		m_gStatus->m_netGameLogicObject->SendAttribMsg( DPNSEND_GUARANTEED,
														DPID_SERVERPLAYER,
														17,
														0,//next flag
														movObj->List_networkDefinedID,
														0,
														0,
														NULL);
       return TRUE;
	}

	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL UserController::ArmPrevWeapon(CMovementObj * movObj,BOOL override)
{
    if(movObj->List_energy <= 0)
	   return FALSE;
    

	if(override == FALSE){
     if(((long)GetTickCount() - m_gStatus->m_actionDelayStamp) < m_gStatus->m_actionDelay)
	  	 return FALSE;
	     m_gStatus->m_actionDelayStamp = GetTickCount(); 
	}

    if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe()){
		m_gStatus->m_netGameLogicObject->SendAttribMsg( DPNSEND_GUARANTEED,
														DPID_SERVERPLAYER,
														17,
														1,//prev flag
														movObj->List_networkDefinedID,
														0,
														0,
														NULL);
       return TRUE;
	}
	return TRUE;
}
//--------------Function basis turn Right---------------------------------------------//
void UserController::ZeroForces(CMovementCaps *movDB)
{
    movDB->List_curTransX = 0.0f;
	movDB->List_curTransY = 0.0f;
	movDB->List_curTransZ = 0.0f;
	movDB->List_curRotX = 0.0f;
	movDB->List_curRotY = 0.0f;
	movDB->List_curRotZ = 0.0f;
}
//--------------Function basis turn left----------------------------------------------//
void UserController::TurnLeft(CMovementCaps *movDB)
{
	  movDB->List_curRotY += movDB->List_yawAccel;
	  if(movDB->List_maxYaw < movDB->List_curRotY)
	     movDB->List_curRotY = movDB->List_maxYaw;
	  movDB->m_yawIsUsed = TRUE;
}
//--------------Function basis turn Right---------------------------------------------//
void UserController::TurnRight(CMovementCaps *movDB)
{
    movDB->List_curRotY -= movDB->List_yawAccel;
	  if(-movDB->List_maxYaw > movDB->List_curRotY)
	      movDB->List_curRotY = -movDB->List_maxYaw;
	  movDB->m_yawIsUsed = TRUE;
}
//--------------Function basis PullUp-------------------------------------------------//
void UserController::Dive(CMovementCaps *movDB)
{
    if(movDB->List_maxPitch==0.0f)
	   return;
	movDB->List_curRotX += movDB->List_pitchAccel;
	  if(movDB->List_maxPitch < movDB->List_curRotX)
	     movDB->List_curRotX = movDB->List_maxPitch;	

	  movDB->m_pitchIsUsed = TRUE;
}
//--------------Function basis pulls up-----------------------------------------------//
void UserController::PullUp(CMovementCaps *movDB)
{
	if(movDB->List_maxPitch==0.0f)
	   return;
	movDB->List_curRotX -= movDB->List_pitchAccel;
	if(-movDB->List_maxPitch > movDB->List_curRotX)
	    movDB->List_curRotX = -movDB->List_maxPitch;

	movDB->m_pitchIsUsed = TRUE;
}
//--------------Function basis rolls--------------------------------------------------//
void UserController::RollLeft(CMovementCaps *movDB)
{
if(movDB->m_maxRoll==0.0f)
	   return;

movDB->List_curRotZ += movDB->m_rollAccel*m_gStatus->m_timeAdjustFloat;
	  if(movDB->m_maxRoll < movDB->List_curRotZ)
	     movDB->List_curRotZ = movDB->m_maxRoll;

	     movDB->m_rollIsUsed = TRUE;
}
//--------------Function basis rolls--------------------------------------------------//
void UserController::RollRight(CMovementCaps *movDB)
{
	if(movDB->m_maxRoll==0.0f)
	   return;
	movDB->List_curRotZ -= movDB->m_rollAccel*m_gStatus->m_timeAdjustFloat;
	  if(-movDB->m_maxRoll > movDB->List_curRotZ)
	     movDB->List_curRotZ = -movDB->m_maxRoll;

	  movDB->m_rollIsUsed = TRUE;
}
//----------------------------------------------------------//
BOOL UserController::GetJet(CMovementObj * ptrLoc,int fuelUse,D3DVECTOR & force)
{
    force.x = 0.0f;
	force.y = 0.0f;
    force.z = 0.0f;
    if(ptrLoc->List_armedInventoryDB)
        for(POSITION posLoc = ptrLoc->List_armedInventoryDB->GetHeadPosition();posLoc != NULL;)
		{
            CArmedInventoryObj * armedPtr = (CArmedInventoryObj *)ptrLoc->List_armedInventoryDB->GetNext(posLoc);
            if(armedPtr->m_functionalityAttribute == 3){//jet
               
			   int * tempAssist = new int;
			   *tempAssist = 0;
				if(armedPtr->m_obfusionMiscInt)
				{
                   *tempAssist = *armedPtr->m_obfusionMiscInt;
                   delete armedPtr->m_obfusionMiscInt;
				   armedPtr->m_obfusionMiscInt = 0;
				}



               *tempAssist = *tempAssist - ((int)(m_gStatus->m_timeAdjust * ((float)fuelUse)));
			   if(*tempAssist > 0 || *tempAssist == 0){
			     force.x = armedPtr->m_miscPosition.x;
	             force.y = armedPtr->m_miscPosition.y;
                 force.z = armedPtr->m_miscPosition.z;
				 
                 armedPtr->m_obfusionMiscInt = tempAssist;
				 return TRUE;
			   }
			   armedPtr->m_obfusionMiscInt = tempAssist;
               
			}//end jet
		}
	return FALSE;
}
//--------------Function basis decel turn---------------------------------------------//
void UserController::DecelTurn(CMovementCaps *movDB)
{
            if(movDB->List_curRotY > 0)
               movDB->List_curRotY -= movDB->List_yawDecel*m_gStatus->m_timeAdjustFloat;
			else if(movDB->List_curRotY < 0)
               movDB->List_curRotY += movDB->List_yawDecel*m_gStatus->m_timeAdjustFloat;
			if(movDB->List_curRotY > 0 && movDB->List_curRotY < ((movDB->List_yawDecel*m_gStatus->m_timeAdjustFloat)+.01f))
		    movDB->List_curRotY = 0;
		    if(movDB->List_curRotY < 0 && movDB->List_curRotY > -((movDB->List_yawDecel*m_gStatus->m_timeAdjustFloat)+.01f))
		    movDB->List_curRotY = 0;
		   
}
//--------------Function basis decel pitch--------------------------------------------//
void UserController::DecelPitch(CMovementCaps *movDB)
{
		if(movDB->List_curRotX > 0)
                movDB->List_curRotX -= movDB->List_pitchDecel*m_gStatus->m_timeAdjustFloat;
		     else if(movDB->List_curRotX < 0)
                movDB->List_curRotX += movDB->List_pitchDecel*m_gStatus->m_timeAdjustFloat;
		     if(movDB->List_curRotX > 0 && movDB->List_curRotX < ((movDB->List_pitchDecel*m_gStatus->m_timeAdjustFloat)+.01f))
		        movDB->List_curRotX = 0;
		     if(movDB->List_curRotX < 0 && movDB->List_curRotX > -((movDB->List_pitchDecel*m_gStatus->m_timeAdjustFloat)+.01f))
		        movDB->List_curRotX = 0;
}
//--------------Function basis decel roll---------------------------------------------//
void UserController::DecelRoll(CMovementCaps *movDB)
{
		  if(movDB->List_curRotZ > 0)
             movDB->List_curRotZ -= movDB->List_pitchDecel*m_gStatus->m_timeAdjustFloat;
		  else if(movDB->List_curRotZ < 0)
             movDB->List_curRotZ += movDB->List_pitchDecel*m_gStatus->m_timeAdjustFloat;
		  if(movDB->List_curRotZ > 0 && movDB->List_curRotZ < ((movDB->List_pitchDecel*m_gStatus->m_timeAdjustFloat) +.01f))
			 movDB->List_curRotZ = 0;
		  if(movDB->List_curRotZ < 0 && movDB->List_curRotZ > -((movDB->List_pitchDecel*m_gStatus->m_timeAdjustFloat) +.01f))
			 movDB->List_curRotZ = 0;
}
//--------------Function basis decel speed--------------------------------------------//
void UserController::DecelSpeed(CMovementCaps *movDB)
{
 if(movDB->List_autoDecelZ == TRUE && movDB->List_currentlyJumping == FALSE){
   if(movDB->List_curTransZ > (movDB->List_decellerate*m_gStatus->m_timeAdjustFloat))
	  movDB->List_curTransZ -= (movDB->List_decellerate*m_gStatus->m_timeAdjustFloat);
   else if(movDB->List_curTransZ < -(movDB->List_decellerate*m_gStatus->m_timeAdjustFloat))
	  movDB->List_curTransZ += (movDB->List_decellerate*m_gStatus->m_timeAdjustFloat);
   else
	  movDB->List_curTransZ=0.0f;
 }
}
//--------------Function basis decel speed--------------------------------------------//
void UserController::DecelSideStep(CMovementCaps *movDB)
{
 if(movDB->List_autoDecel == TRUE){
   if(movDB->List_curTransX > (movDB->List_decellerate*m_gStatus->m_timeAdjustFloat))
	  movDB->List_curTransX -= (movDB->List_decellerate*m_gStatus->m_timeAdjustFloat);
   else if(movDB->List_curTransX < -(movDB->List_decellerate*m_gStatus->m_timeAdjustFloat))
	  movDB->List_curTransX += (movDB->List_decellerate*m_gStatus->m_timeAdjustFloat);
   else
	  movDB->List_curTransX=0.0f;
 }
}
//----------------------------------------------------------//
BOOL UserController::RegenJet(CMovementObj * ptrLoc,int fuelGain)
{
		//CHRIS HACK
		//CHRONICLE DEMO ENERGY SAMPLE
		float m_energyIncrease = (((ptrLoc->m_energyRate - ((ptrLoc->m_energyCurrent + 0.01)/ptrLoc->m_energyMax)) 
									- (m_gStatus->m_multiplayerObject->m_currentStat1*0.0001)
									+ ((m_gStatus->m_multiplayerObject->m_currentStat2*0.0001) * 2)
									- (m_gStatus->m_multiplayerObject->m_currentStat3*0.0001)) * .1)
									* m_gStatus->m_timeAdjust;

		if (m_energyIncrease < 0)
			m_energyIncrease = 0;
		
		ptrLoc->m_energyCurrent += m_energyIncrease;

		/*ptrLoc->m_energyMax = m_gStatus->m_multiplayerObject->m_currentStat2 + m_gStatus->m_multiplayerObject->m_currentStat3;*/
		ptrLoc->m_energyMax = (float)m_gStatus->m_multiplayerObject->m_currentStat2 + m_gStatus->m_multiplayerObject->m_currentStat3;
		//WARNING: C4244 --> int to float

		//Das Formula
		//(((r - ((c + .01)/m)) - (f*.0001) + ((a*.0001) * 2) - (i*.0001)) * .1) * timewait

		if(ptrLoc->m_energyCurrent > ptrLoc->m_energyMax)
			ptrLoc->m_energyCurrent = ptrLoc->m_energyMax;
		

		//END CHRON HACK

        if(!ptrLoc->List_armedInventoryDB)
        return FALSE;
        for(POSITION posLoc = ptrLoc->List_armedInventoryDB->GetHeadPosition();posLoc != NULL;)
		{
            CArmedInventoryObj * armedPtr = (CArmedInventoryObj *)ptrLoc->List_armedInventoryDB->GetNext(posLoc);
            if(armedPtr->m_functionalityAttribute == 3){//jet
               int * tempAssist = new int;
			   *tempAssist = 0;
				if(armedPtr->m_obfusionMiscInt)
				{
                   *tempAssist = *armedPtr->m_obfusionMiscInt;
                   delete armedPtr->m_obfusionMiscInt;
				   armedPtr->m_obfusionMiscInt = 0;
				}

               *tempAssist = *tempAssist + ((int)(m_gStatus->m_timeAdjust * ((float)fuelGain)));
			   if(*tempAssist > armedPtr->m_miscInt){
			      *tempAssist = armedPtr->m_miscInt;
			   }
               armedPtr->m_obfusionMiscInt = tempAssist;
			   

               return TRUE;
			}//end jet
		}
        return TRUE;
}
//------------------------------------------------------------------------------------//
void UserController::AutoLevelEntity(CMovementObj *entPtr, float strengthX, float strengthZ, float tolerance)
{
     D3DVECTOR xyzLocation,upVect,dirVect,axis,lastLocation;
	 D3DMATRIX position;
	 float autolevelPower = strengthX*m_gStatus->m_timeAdjustFloat;

     entPtr->List_baseFrame->GetTransform(NULL,&position);
	 entPtr->List_baseFrame->GetPosition(NULL,&lastLocation);
	 
	 position._41 = 0.0f;
	 position._42 = 0.0f;
	 position._43 = 0.0f;
	 
	 xyzLocation.x = position._41;
	 xyzLocation.y = position._42;
	 xyzLocation.z = position._43;

     entPtr->List_baseFrame->GetOrientation(&dirVect,&upVect);
	 if(upVect.x == 0.0f && upVect.z == 0.0f)
		return;

	 if(fabs(upVect.y) > (1.0f - tolerance))//if(fabs(upVect.y) == 1.0f)
		return;

	 axis.x = xyzLocation.x - upVect.x;
     axis.z = xyzLocation.z - upVect.z;
	 axis.y = 0.0f;
	 float initOne  = 1.0f;
     float initTemp = 0.0f;
	 float distance1 = m_gStatus->m_matLib.MagnitudeSquared(upVect.x,upVect.y,upVect.z,
		                                                   initTemp,initOne,initTemp);
     if(distance1 == 0.0f)
		return;

	 entPtr->List_baseFrame->SetPosition2(NULL,position._41,position._42,position._43);

     if(fabs(upVect.y) == 0.0f)
		upVect.y = .001f;
	 
	 float turnReq = atan(fabs(distance1)/fabs(upVect.y));


	 D3DMATRIX roationalMod;
	 m_gStatus->m_matLib.Rotate(0.0,90.0,0.0f,roationalMod.m);
	 axis = m_gStatus->m_matLib.UpdatePolygonVertex(roationalMod.m,axis);
	 axis = m_gStatus->m_matLib.Normalize(axis);

     if(turnReq < autolevelPower)
		autolevelPower = turnReq;

	 D3DMATRIX apply = m_gStatus->m_matLib.AxisAngleRotationV2( axis,autolevelPower);
     entPtr->List_baseFrame->AddTransformAfter(apply);
	 entPtr->List_baseFrame->SetPosition(NULL,lastLocation);
}
//-------------------------------------------------------------------------------------//
void UserController::ShowWinCursor(BOOL state)
{
    if(m_ssScur == state)
	   return;
    m_ssScur = state;
    #ifndef _FORCEMOUSE
	::ShowCursor(m_ssScur);
    #endif

}

//system pivots off of global click data

/*bool UserController::SetQuickItem(int BarID, int SlotID)
{
	if(m_gStatus->m_multiplayerObject->Quickbar(BarID,SlotID) > -1)
	{
		//Get Inv Object based off stored GLID
		CItemObj * invObj = m_gStatus->m_itemManager->GetItemByID(m_gStatus->m_multiplayerObject->Quickbar[BarID][SlotID]);
		if(invObj)
		{
			//Get Visuals Based on invObj and stored Visual ID
			CMenuImage *quickItem = (CMenuImage*)m_gStatus->m_runtimeMenuDatabase->GetVisualByID(m_gStatus->m_multiplayerObject->m_ActiveItem[0], true);
			CMenuImage *itemIcon = (CMenuImage*)m_gStatus->m_menuIconDatabase->GetVisualByID(invObj->m_visualRef, true);

			if(!itemIcon)
			{
				//Set a Default Icon (First one in DB)
				POSITION posCurVisual = m_gStatus->m_menuIconDatabase->GetHeadPosition();
				if(posCurVisual)
					itemIcon = (CMenuImage*)m_gStatus->m_menuIconDatabase->GetNext(posCurVisual);
			}

			//Make sure we have a quick Item and Established Quick Slot
			if(quickItem && itemIcon)
			{
				//Update the Image
				quickItem->m_texturePosX		= itemIcon->m_texturePosX;
				quickItem->m_texturePosY		= itemIcon->m_texturePosY;
				quickItem->m_fullTextureWidth	= itemIcon->m_fullTextureWidth;
				quickItem->m_fullTextureHeight	= itemIcon->m_fullTextureHeight;

				//quickItem->UpdateTexture(itemIcon->m_textureSelect);
				quickItem->UpdateTextureCoords();

				m_gStatus->m_multiplayerObject->m_ActiveItem[1] = m_gStatus->m_multiplayerObject->Quickbar[BarID][SlotID];

				return true;
			}
			else
			{
				m_gStatus->m_systemReadout->AddLine("*** INVALID: UNABLE TO SET TO QUICK ITEM ***");
			}
		}
	}
	else
	{
		if(m_gStatus->m_systemReadout)
			m_gStatus->m_systemReadout->AddLine("*** INVALID: SLOT IS EMPTY ***");
	}

	return false;
}*/
//-----------------------------------------------------------------
//auto steering
//-----------------------------------------------------------------
//------------------------------------------------------------------------------------//
void UserController::AutoSteeringBasedOnAIObj(CMovementObj * ptrLoc,D3DVECTOR locationLoc,float & adjust)
{

	     if(ptrLoc->m_aiObj->m_exponentTurning == TRUE)
		    ptrLoc->List_movementDB->m_exponentTurning = true;

         if(ptrLoc->m_aiObj->m_exponentTurning == TRUE)
		 {
            ptrLoc->List_movementDB->List_curRotY=0.0f;
            ptrLoc->List_movementDB->List_curRotX=0.0f;
		 }
		
		 //---------------------------------//
		 //YAW
		 //---------------------------------//
	     if(locationLoc.z > 0.0f){//behind you
			if(locationLoc.x > 0.0f){
			    if(ptrLoc->m_aiObj->m_exponentTurning == FALSE)
				   TurnLeft(ptrLoc->List_movementDB);
				else{
                   ptrLoc->List_movementDB->List_curRotY = ptrLoc->List_movementDB->List_maxYaw*adjust;
				   ptrLoc->List_movementDB->m_yawIsUsed = TRUE;
				}
			}
			else{
			    if(ptrLoc->m_aiObj->m_exponentTurning == FALSE)
				   TurnRight(ptrLoc->List_movementDB);
				else{
                   ptrLoc->List_movementDB->List_curRotY = -ptrLoc->List_movementDB->List_maxYaw*adjust;
				   ptrLoc->List_movementDB->m_yawIsUsed = TRUE;
				}
			}
		
		 }//end behind you
         else{//steer
         //yaw
		   if(locationLoc.x < 0.0f){//-AIPtr->List_turnTolerance){
			if(ptrLoc->m_aiObj->m_exponentTurning == FALSE)
			   TurnRight(ptrLoc->List_movementDB);
			else{
			  if(locationLoc.z != 0.0f){
                  float turnReq = atan(fabs(locationLoc.x)/fabs(locationLoc.z))* 57.3f;
                  
				  if(turnReq > ptrLoc->List_movementDB->List_maxYaw * adjust)
					 turnReq = ptrLoc->List_movementDB->List_maxYaw * adjust;
				  ptrLoc->List_movementDB->List_curRotY = -turnReq;
				  ptrLoc->List_movementDB->m_yawIsUsed = TRUE;
			  }
			}
		   }
           else if(locationLoc.x > 0.0f){
	        //TurnLeft(ptrLoc->List_movementDB);
			if(ptrLoc->m_aiObj->m_exponentTurning == FALSE)
			   TurnRight(ptrLoc->List_movementDB);
			else{
			  if(locationLoc.z != 0.0f){
                  float turnReq = atan(fabs(locationLoc.x)/fabs(locationLoc.z))* 57.3f;

				  if(turnReq > ptrLoc->List_movementDB->List_maxYaw * adjust)
					 turnReq = ptrLoc->List_movementDB->List_maxYaw * adjust;
                  ptrLoc->List_movementDB->List_curRotY = turnReq;
				  ptrLoc->List_movementDB->m_yawIsUsed = TRUE;
			  }
			}
		   }
           //---------------------------------//
		   //YAW
		   //---------------------------------//
           if(locationLoc.y < 0.0f){//pitch dive
	        
              if(ptrLoc->m_aiObj->m_exponentTurning == FALSE)
			   Dive(ptrLoc->List_movementDB);
			  else{
			   if(locationLoc.z != 0.0f){
                  float turnReq = atan(fabs(locationLoc.y)/fabs(locationLoc.z))* 57.3f;
                  
				  if(turnReq > ptrLoc->List_movementDB->List_maxPitch * adjust)
					 turnReq = ptrLoc->List_movementDB->List_maxPitch * adjust;
				  ptrLoc->List_movementDB->List_curRotX = turnReq;
				  ptrLoc->List_movementDB->m_pitchIsUsed = TRUE;
			   }
			  }
		   }//end pitch dive
           else if(locationLoc.y > 0.0f){
		    if(ptrLoc->m_aiObj->m_exponentTurning == FALSE)
			   PullUp(ptrLoc->List_movementDB);
			else{
			  if(locationLoc.z != 0.0f){
                  float turnReq = atan(fabs(locationLoc.y)/fabs(locationLoc.z))* 57.3f;

				  if(turnReq > ptrLoc->List_movementDB->List_maxPitch * adjust)
					 turnReq = ptrLoc->List_movementDB->List_maxPitch * adjust;
                  ptrLoc->List_movementDB->List_curRotX  = -turnReq;
				  ptrLoc->List_movementDB->m_pitchIsUsed = TRUE;
			  }
			}
		   }
          
		 }//end steer
}
//------------------------------------------------------------------------------------//
BOOL UserController::USER_AI_Transform(CMovementObj * ptrLoc,CWaterZoneObjList * m_waterZoneDB,double timeAdjust,double lastTimeAdjust)
{
	float doYaw = ptrLoc->List_movementDB->List_curRotY;//(float)((ptrLoc->List_movementDB->List_curRotY * exp((fabs(ptrLoc->List_movementDB->List_curSpeed) ))));
	float doPitch = ptrLoc->List_movementDB->List_curRotX;//(float)((ptrLoc->List_movementDB->List_curRotX * exp((fabs(ptrLoc->List_movementDB->List_curSpeed) ))));
	if(ptrLoc->List_baseFrame == NULL)
		return FALSE;

	//ADDITIONAL ENTITY FORCES
	float AdditionalForceX=0.0f;
	float AdditionalForceY=0.0f;
	float AdditionalForceZ=0.0f;
	
	//EXTERNAL FORCES
	float ExternalForceX=m_gStatus->m_environmentPtr->m_windDirectionX + ptrLoc->List_externalVectorTemp.x;
	float ExternalForceY=m_gStatus->m_environmentPtr->m_windDirectionY + ptrLoc->List_externalVectorTemp.y;
	float ExternalForceZ=m_gStatus->m_environmentPtr->m_windDirectionZ + ptrLoc->List_externalVectorTemp.z;
	ptrLoc->List_externalVectorTemp.x = 0.0f;
	ptrLoc->List_externalVectorTemp.y = 0.0f;
	ptrLoc->List_externalVectorTemp.z = 0.0f;
	          
	if(ptrLoc->List_movementDB->m_yawIsUsed==FALSE)
		DecelTurn(ptrLoc->List_movementDB);
	if(ptrLoc->List_movementDB->m_pitchIsUsed==FALSE)
		DecelPitch(ptrLoc->List_movementDB);
	if(ptrLoc->List_movementDB->m_rollIsUsed==FALSE)
		DecelRoll(ptrLoc->List_movementDB);
	if(ptrLoc->List_movementDB->m_speedControlInUse==FALSE)
		DecelSpeed(ptrLoc->List_movementDB);
	if(ptrLoc->List_movementDB->m_sidestepsInUse==FALSE)
		DecelSideStep(ptrLoc->List_movementDB);

	D3DVECTOR orig;
	float submergeValue = 0.0f;
	ptrLoc->List_baseFrame->GetPosition(NULL,&orig);
	BOOL inWater = m_waterZoneDB->IsPositionInWater(orig,&submergeValue);

	ptrLoc->List_movementDB->DoPhysics(	timeAdjust,
										ptrLoc->List_baseFrame,
										ptrLoc->List_movementDB,
										AdditionalForceX,
										AdditionalForceY,
										AdditionalForceZ,
										ExternalForceX,
										ExternalForceY,
										ExternalForceZ,
										doYaw,
										doPitch,
										ptrLoc->List_movementDB->m_rollIsUsed,
										ptrLoc->List_movementDB->m_pitchIsUsed,
										ptrLoc->List_movementDB->m_yawIsUsed,
										submergeValue,
										m_gStatus->m_environmentPtr->m_seaViscusDrag,
										m_gStatus->m_environmentPtr->m_gravity,
										lastTimeAdjust,
										ptrLoc->m_collisionRaySys,
										m_gStatus->m_systemReadout);

	AutoLevelEntity(ptrLoc,
					ptrLoc->List_movementDB->m_autoLevelXStrength * ptrLoc->List_movementDB->m_autoLevelX,
					ptrLoc->List_movementDB->m_autoLevelZStrength * ptrLoc->List_movementDB->m_autoLevelZ,
					ptrLoc->List_movementDB->m_autoLevelTolerance);

	return TRUE;
}



//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void UserController::USER_AIFollowAttackTargetState(CMovementObj * ptrLoc,
								                    float & timeAdjust,
								                    CMovementObjList * entObjRuntime,
										            DWORD & currentTime,
													int & targetTrace)
{
     ptrLoc->List_AI_target = targetTrace;

     if( ptrLoc->List_AI_target < 0)
	 {
		 ptrLoc->List_currentlyFireing = FALSE;
		 return;
	 }

	 
     ptrLoc->List_targetRef = entObjRuntime->GetByTraceAlive(ptrLoc->List_AI_target, ptrLoc->List_traceNumber);

	 if(!ptrLoc->List_targetRef)
		 return;
 
	 int followMode = 1;

	 //calc positions
	 D3DVECTOR positionWldEyeLevel,targetPosition;
     targetPosition.x  = ptrLoc->List_targetRef->List_baseFrame->List_frameMatrix._41;
	 targetPosition.y  = ptrLoc->List_targetRef->List_baseFrame->List_frameMatrix._42 + 5.0f;//adjust to above ground
     targetPosition.z  = ptrLoc->List_targetRef->List_baseFrame->List_frameMatrix._43;

     positionWldEyeLevel.x  = ptrLoc->List_baseFrame->List_frameMatrix._41;
     positionWldEyeLevel.y  = ptrLoc->List_baseFrame->List_frameMatrix._42 + ptrLoc->m_aiObj->m_eyeLevel;//adjust to eyelevel
     positionWldEyeLevel.z  = ptrLoc->List_baseFrame->List_frameMatrix._43;
	 
	 
	 if(followMode == 0)
	 {//simple mode
	    ptrLoc->m_aiObj->List_closeEnoughDist = ptrLoc->m_aiObj->m_travelingDesiredDist;;
		ptrLoc->m_aiObj->m_lookAt.x = ptrLoc->List_targetRef->List_baseFrame->List_frameMatrix._41;
		ptrLoc->m_aiObj->m_lookAt.y = ptrLoc->List_targetRef->List_baseFrame->List_frameMatrix._42 + ptrLoc->m_aiObj->m_prefernceAboveTarget;
		ptrLoc->m_aiObj->m_lookAt.z = ptrLoc->List_targetRef->List_baseFrame->List_frameMatrix._43;
	 }//end simple mode
	 else
	 {//path tracking mode
        D3DVECTOR myPos, targetPos;
        ptrLoc->List_baseFrame->GetPosition(NULL,&myPos);
		ptrLoc->List_targetRef->List_baseFrame->GetPosition(NULL,&targetPos);
		ptrLoc->m_aiObj->m_pathTracker.SetParams(ptrLoc->m_aiObj->m_travelingDesiredDist, 15.0f, 100);
	    ptrLoc->m_aiObj->m_pathTracker.Calculate(myPos,targetPos,ptrLoc->List_AI_target,currentTime);
	    ptrLoc->m_aiObj->m_lookAt = ptrLoc->m_aiObj->m_pathTracker.m_curTravelToPosition;
		//randomize and consolidate-check vision and reconsolidate if necessary
        if(rand()%100 < 10)
		{//10% 
           if(ptrLoc->m_aiObj->CheckVisionIsClearX(positionWldEyeLevel,targetPosition,ptrLoc->m_collisionVisionRefPtr))
		   {//vision is clear
              ptrLoc->m_aiObj->m_pathTracker.Consolidate(myPos,targetPos,ptrLoc->List_AI_target,currentTime);
		      //recalibrates happy distance during visiual attack
			  float totalDistance = ptrLoc->m_aiObj->m_attackMaxDistance - ptrLoc->m_aiObj->m_attackMinDistance;
              float percentage = ((float)(rand()%100))*.01f;
              ptrLoc->m_aiObj->List_closeEnoughDist = ptrLoc->m_aiObj->m_attackMinDistance + (percentage * totalDistance);
		      ptrLoc->m_aiObj->m_timeStampWhenTargetNotInSight = currentTime;
		   }//end vision is clear
		   else
		   {//vision is not clear
              ptrLoc->m_aiObj->List_closeEnoughDist = ptrLoc->m_aiObj->m_travelingDesiredDist;//closes gap for following
              //consider forgetting about target
              if((currentTime - ptrLoc->m_aiObj->m_timeStampWhenTargetNotInSight) > ptrLoc->m_aiObj->m_durationTillTargetLost)
			  {//target lost
                 ptrLoc->List_AI_target = -1;
                 ptrLoc->m_aiObj->m_aiMovementState = 6;
			  }//end target lost
		   }//end vision is not clear
		   if(ptrLoc->m_aiObj->m_bodyCollision)
	       {//currently blocked
         
		      if(rand()%100<20)
		         if(ptrLoc->m_aiObj->m_enableJumpAbility)
			           JumpMovementClassObj(ptrLoc);
		   }
		   
		}//end 10%
		ptrLoc->m_aiObj->m_lookAt.y += ptrLoc->m_aiObj->m_prefernceAboveTarget;
	 }//end path tracking mode
	 
	 if(m_gStatus->m_matLib.MagnitudeSquared(ptrLoc->List_targetRef->List_baseFrame->List_frameMatrix,ptrLoc->List_baseFrame->List_frameMatrix)>ptrLoc->m_aiObj->List_closeEnoughDist)
	 {//need to move closer to enemy
		D3DVECTOR localPos = m_gStatus->m_matLib.GetLocalPositionFromMatrixView(ptrLoc->List_baseFrame->List_frameMatrix,ptrLoc->m_aiObj->m_lookAt);
		AutoSteeringBasedOnAIObj(ptrLoc,localPos,timeAdjust);
        Accellerate(ptrLoc->List_movementDB,ptrLoc->m_aiObj->m_currentSpeedOverride,timeAdjust);
	 }
	 else
	 {//just face enemy
        D3DVECTOR localPos = m_gStatus->m_matLib.GetLocalPositionFromMatrixView(ptrLoc->List_baseFrame->List_frameMatrix,ptrLoc->m_aiObj->m_lookAt);
        AutoSteeringBasedOnAIObj(ptrLoc,localPos,timeAdjust);
		ZeroForces(ptrLoc->List_movementDB);
	 }
	 ptrLoc->m_aiObj->m_bodyCollision = FALSE;
}
//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void UserController::USER_AIFireAtEnemy(CMovementObj * ptrLoc,
							            float & timeAdjust,
							            CMovementObjList * entObjRuntime,
							            DWORD & currentTime)
{


    //DWORD timePassed = currentTime - ptrLoc->m_aiObj->m_attackStamp; 
	if(ptrLoc->List_targetRef)
    if(!ptrLoc->m_aiObj->m_attackingCurrently)
	if((currentTime - ptrLoc->m_aiObj->m_attackStamp) > ptrLoc->m_aiObj->m_attackTimeBetweenCurrent && ptrLoc->List_targetRef->List_energy > 0)
	if(m_gStatus->m_matLib.MagnitudeSquared(ptrLoc->List_baseFrame->List_frameMatrix,ptrLoc->List_targetRef->List_baseFrame->List_frameMatrix) < ptrLoc->m_aiObj->m_strikeShootRange)
	{//time to attack
       D3DVECTOR positionWldEyeLevel,targetPosition;
	   targetPosition.x  = ptrLoc->List_targetRef->List_baseFrame->List_frameMatrix._41;
       targetPosition.y  = ptrLoc->List_targetRef->List_baseFrame->List_frameMatrix._42 + (ptrLoc->List_boundingRadius * .5f);
	   targetPosition.z  = ptrLoc->List_targetRef->List_baseFrame->List_frameMatrix._43;

	   positionWldEyeLevel.x  = ptrLoc->List_baseFrame->List_frameMatrix._41;
       positionWldEyeLevel.y  = ptrLoc->List_baseFrame->List_frameMatrix._42 + ptrLoc->m_aiObj->m_eyeLevel;//adjust to eyelevel
	   positionWldEyeLevel.z  = ptrLoc->List_baseFrame->List_frameMatrix._43;
	   ptrLoc->m_aiObj->ResetVisionIsClearX();
	   if(ptrLoc->m_aiObj->CheckVisionIsClearX(positionWldEyeLevel,targetPosition,ptrLoc->m_collisionVisionRefPtr))
	   {//node is visible
	      float percentage = ((float)(rand()%100))*.01f;
	      ptrLoc->m_aiObj->m_attackDurationCurrent = ((long)(percentage * ((float)(ptrLoc->m_aiObj->m_attackDurationMax - ptrLoc->m_aiObj->m_attackDurationMin)) ))+ptrLoc->m_aiObj->m_attackDurationMin;
	      if(ptrLoc->m_aiObj->m_attackDurationCurrent < 2000)
		     ptrLoc->m_aiObj->m_attackDurationCurrent = 2000;
		  
		  ptrLoc->m_aiObj->m_attackingCurrently	   = TRUE;
	      ptrLoc->m_aiObj->m_attackStamp		   = currentTime;
		  ptrLoc->List_currentlyFireing            = TRUE;
	   }
	   else
	   {
          ptrLoc->m_aiObj->m_attackStamp		   = currentTime;//wait till in sight
		  ptrLoc->List_currentlyFireing            = FALSE;
	   }
	}//end time to attack

	
	
	if(ptrLoc->m_aiObj->m_attackingCurrently)
	if((currentTime - ptrLoc->m_aiObj->m_attackStamp) > ptrLoc->m_aiObj->m_attackDurationCurrent)
	{//wait
	   float percentage = ((float)(rand()%100))*.01f;
       ptrLoc->m_aiObj->m_attackTimeBetweenCurrent	= ((DWORD)(percentage * ((float)(ptrLoc->m_aiObj->m_attackTimeBetweenMax - ptrLoc->m_aiObj->m_attackTimeBetweenMin)) ))+ptrLoc->m_aiObj->m_attackTimeBetweenMin;
       ptrLoc->m_aiObj->m_attackStamp				= currentTime;
       ptrLoc->m_aiObj->m_attackingCurrently		= FALSE;
	   ptrLoc->List_currentlyFireing                = FALSE;
	}//end wait

	//if(ptrLoc->m_aiObj->m_attackingCurrently)
	//  ptrLoc->List_currentlyFireing  = TRUE;
}
void UserController::PointClickController(CMovementObj * entityPtr)
{
	bool menusHandledInput = m_menuManagement->HandleInput(m_input);

	//clear from last mode
	if(m_lastMode == 0)
	{//handles switch transition between control modes
		if(m_gStatus->m_tgSys_entitySelected > -1)
		{
			CMovementObj * selectedEntPtr = m_gStatus->m_entObjRuntime->GetObjByTrace(m_gStatus->m_tgSys_entitySelected);
			if(selectedEntPtr)
				selectedEntPtr->m_selectionEffectState = -1;

			m_gStatus->m_tgSys_entitySelected = -1;
		}

		entityPtr->List_baseFrame->GetPosition(NULL,&m_gStatus->m_tgSys_latestWldIntersection);
	}//end handles switch transition between control modes

    if(!menusHandledInput)
	{
		//--> IsDown() --> == 2 == true; checks BUTTONSTATE_RELEASED == 2
		if(m_input->m_shiftOn && m_input->m_mouseButtons[MOUSE_RIGHTBUTTON].IsDown() && m_gStatus->m_cameraModule->m_rightClickControl == TRUE && entityPtr->m_firstPersonMode == FALSE)
		{
		   float mouseControlXLoc =(m_input->m_mouseHiResX * 3.0f) / m_gStatus->m_timeAdjustFloat;//(float)GL_MS.lX;
		   float mouseControlYLoc =(m_input->m_mouseHiResY * 3.0f) / m_gStatus->m_timeAdjustFloat;//(float)GL_MS.lY;
		   m_gStatus->m_cameraModule->ApplyMouseRotation(mouseControlXLoc*.1f, mouseControlYLoc*.1f, (float)m_input->m_mouseWheelZ*-.005f);
		   if(m_input->m_keyboardKeys[DIK_TAB].IsDown() || m_input->m_keyboardKeys[DIK_1].IsDown())
			   if(GetTickCount()-m_gStatus->m_cameraModule->m_switchStamp > 500)
			   {
				  if(m_gStatus->m_cameraModule->m_reCorrectY > 0.0f)
					 m_gStatus->m_cameraModule->m_reCorrectY = 0.0f;
				  else
					 m_gStatus->m_cameraModule->m_reCorrectY = 10.0f;

				  m_gStatus->m_cameraModule->m_switchStamp = GetTickCount();
			   }
		
		}
		else
		   TargetingSystemCallback();
	}

     //handle riding entity
     CMovementObj * lookfirePtr = NULL;
	 DWORD currentTime           = GetTickCount();

	 CMovementObj * ptrLoc = NULL;
	 if(entityPtr->m_posses)
	 {//possesing somthing
		ptrLoc = entityPtr->m_posses;
		lookfirePtr = entityPtr;
	 }//end possesing somthing
	 else
	 {
	    ptrLoc = entityPtr;
	    lookfirePtr = entityPtr;
	 }

	 
	 BOOL jetsWereUsed = FALSE;
	 m_restrictMovement = FALSE;
	 BOOL restrictChat = FALSE;
	 float AdditionalForceX,AdditionalForceY,AdditionalForceZ;
	 CCameraObj * camPtrRef = m_gStatus->m_runtimeViewportDB->GetCameraObjectFromEntity(entityPtr);
	 
	 //Removed due to New Camera System
	 //if(!camPtrRef)
	 //		return;

	 POSITION posLoc = m_gStatus->m_controlsList->FindIndex(ptrLoc->List_controlConfig);
	 if(!posLoc)
	 {
		 m_gStatus->LoadControlCfgList("");
		 posLoc = m_gStatus->m_controlsList->FindIndex(ptrLoc->List_controlConfig);
	 }
	 if(!posLoc)
		 return;
	 
	 ControlDBObj * conPtr = ( ControlDBObj* ) m_gStatus->m_controlsList->GetAt(posLoc);	

	 if(!m_gStatus->m_editorModeOn && m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
	 {//client app activated
	    if(entityPtr->List_networkDefinedID > 0)
	    {
	       if((GetTickCount()-m_gStatus->m_multiplayerObject->GetLastCommunicationTime()) > m_gStatus->m_multiplayerObject->m_lagMSTolerance)
		   {//lagged
		      if(m_gStatus->m_systemReadout)
              if(!m_gStatus->m_lagged)
		          m_gStatus->m_systemReadout->AddLine("Connection Interuption...");
		      m_gStatus->m_lagged = TRUE;
		      return;
		   }//end lagged
		}
	 }
	 
	 m_gStatus->m_lagged = FALSE;

	 if(entityPtr->List_energy <= 0)
	 {//dead
		m_restrictMovement = TRUE;
		if(entityPtr->m_posses)
		   entityPtr->m_posses->List_energy = 0;

		if(m_gStatus->m_tgSys_entitySelected > -1)
		{
			CMovementObj * selectedEntPtr = m_gStatus->m_entObjRuntime->GetObjByTrace(m_gStatus->m_tgSys_entitySelected);
			if(selectedEntPtr)
				selectedEntPtr->m_selectionEffectState = -1;

			m_gStatus->m_tgSys_entitySelected = -1;
		}

		m_gStatus->m_tgSys_indexOfObjectSelected = -1;

		//if on ground and dead in network game respawn
		if(entityPtr->List_movementDB->List_doGravity == FALSE || entityPtr->List_movementDB->m_inLiquid == TRUE)
		if(m_gStatus->m_respawning == FALSE)
		{//on ground tell server to respawn
		   if((currentTime-m_gStatus->m_respawnTimeStamp) > m_gStatus->m_respawnWait)
		   {
			  if(m_gStatus->m_multiplayerObject->m_currentCharacterInUse < 0)
				 m_gStatus->m_multiplayerObject->m_currentCharacterInUse = 0;

			  m_gStatus->m_globalAvVar = TRUE;
			  m_gStatus->m_axLogger.WriteDebugFile("Play char command sent",0,1);
			  m_gStatus->m_netGameLogicObject->PlayCharacterOnAccount(m_gStatus->m_multiplayerObject->m_currentCharacterInUse,m_gStatus->m_multiplayerObject,m_gStatus->m_localContentClientVersion);
			  
			  m_gStatus->m_respawning = TRUE;
		   }
		}//end on ground tell server to respawn
	 }//end dead
	 else{
	   m_gStatus->m_respawning       = FALSE;
	   m_gStatus->m_respawnTimeStamp = currentTime;
	 }


     if(!ptrLoc->m_aiObj)
	 {
         CAIObj * aiPtrStncil = m_gStatus->m_aiList->GetByIndex(ptrLoc->List_movementDB->m_pckSys_aiControlModel);
		 if(aiPtrStncil)
            aiPtrStncil->Clone(&ptrLoc->m_aiObj);
	 } 

	 if(!ptrLoc->m_aiObj)
	 {
		 if(m_gStatus->m_systemReadout)
         m_gStatus->m_systemReadout->AddLine("ai index could not be aquired (check Physics settings)");
         return;
	 }
 
	 if( m_gStatus->m_tgSys_entitySelected > -1 && ptrLoc->List_traceNumber != m_gStatus->m_tgSys_entitySelected)
	 {//entity selected
		 m_gStatus->m_tgSys_expQue = -1;
	     //detect if NPC-ENEMY OR Friend Object - possible cursor selctions etc - list of cursors embeded on motionClass
	     CMovementObj * selectedEntPtr = m_gStatus->m_entObjRuntime->GetObjByTrace(m_gStatus->m_tgSys_entitySelected);//aquire entity by selected trace
		 if(selectedEntPtr)
		 {//valid ptr
			if(selectedEntPtr->List_controlType == 35 || selectedEntPtr->List_energy <= 0)
			{//interactive type

			     //get close to interactive NPC before actuating it or him
				 D3DMATRIX positionMatrix,focusMatrix;
			     selectedEntPtr->List_baseFrame->GetTransform(NULL,&positionMatrix);
				 ptrLoc->List_baseFrame->GetTransform(NULL,&focusMatrix);
				 D3DVECTOR bonePosition = m_gStatus->m_matLib.UpdatePolygonVertex(positionMatrix.m,selectedEntPtr->List_skeletalObject->GetBonePositionByIndex(selectedEntPtr->List_skeletalObject->m_calulationSphere->m_boneIndex));
                 D3DVECTOR bonePositionFocus = m_gStatus->m_matLib.UpdatePolygonVertex(focusMatrix.m,ptrLoc->List_skeletalObject->GetBonePositionByIndex(ptrLoc->List_skeletalObject->m_calulationSphere->m_boneIndex));
				 //if(m_gStatus->m_globalAutoTriggerLast != selectedEntPtr->List_traceNumber)
				 if(m_gStatus->m_matLib.BoundingSphereCheck(ptrLoc->List_skeletalObject->m_calulationSphere->m_sphereRadius*.9f,
					                                        selectedEntPtr->List_skeletalObject->m_calulationSphere->m_sphereRadius*.9f,
														    bonePositionFocus.x,
														    bonePositionFocus.y,
														    bonePositionFocus.z,
														    bonePosition.x,
														    bonePosition.y,
														    bonePosition.z) == TRUE)
				 {//in calculation radius
					 if(!m_restrictMovement)
					 {
						 if(GetTickCount()-m_npcQuertTimeStamp > 1000)
						 {
							m_npcQuertTimeStamp = GetTickCount();
							if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
							{
							   m_gStatus->SendChatString(m_gStatus->m_globalInteractCommand);						   
							}
							else if(m_gStatus->m_systemReadout)
							   m_gStatus->m_systemReadout->AddLine("**On server-this would send interaction msg request***");
							
							m_gStatus->m_globalAutoTriggerLast = selectedEntPtr->List_traceNumber;
							m_gStatus->m_tgSys_indexOfObjectSelected = -1;
							m_gStatus->m_tgSys_entitySelected = -1;
							selectedEntPtr->m_selectionEffectState = -1;
							selectedEntPtr->List_baseFrame->GetPosition(NULL,&m_gStatus->m_tgSys_latestWldIntersection);	
						 }
						 else
						 {
						    //face the NPC
							selectedEntPtr->List_baseFrame->GetPosition(NULL,&ptrLoc->m_aiObj->m_lookAt);
							D3DVECTOR localPos = m_gStatus->m_matLib.GetLocalPositionFromMatrixView(ptrLoc->List_baseFrame->List_frameMatrix,ptrLoc->m_aiObj->m_lookAt);
							AutoSteeringBasedOnAIObj(ptrLoc,localPos,m_gStatus->m_timeAdjustFloat);
						 }
					}
					//ProcessChat(ptrLoc,entityPtr);
					
					if(!menusHandledInput)
						ProcessKeyboardMouseButtons(ptrLoc, 
													entityPtr,
													lookfirePtr,
													currentTime,
													jetsWereUsed,
													AdditionalForceX,
													AdditionalForceY,
													AdditionalForceZ,
													restrictChat,
													NULL, //Changed from camPtrRef
													conPtr,
													m_input->m_mouseButtons[MOUSE_LEFTBUTTON].IsDown());
					ZeroForces(ptrLoc->List_movementDB);
				 }//in calculation radius
				 else
				 {//get closer
					 
                     if(!m_restrictMovement)
					 {
					    selectedEntPtr->List_baseFrame->GetPosition(NULL,&ptrLoc->m_aiObj->m_lookAt);
					    D3DVECTOR localPos = m_gStatus->m_matLib.GetLocalPositionFromMatrixView(ptrLoc->List_baseFrame->List_frameMatrix,ptrLoc->m_aiObj->m_lookAt);
			            AutoSteeringBasedOnAIObj(ptrLoc,localPos,m_gStatus->m_timeAdjustFloat);
					 }
			         //ProcessChat(ptrLoc,entityPtr);
			         if(!menusHandledInput)
						ProcessKeyboardMouseButtons(ptrLoc, 
													entityPtr,
													lookfirePtr,
													currentTime,
													jetsWereUsed,
													AdditionalForceX,
													AdditionalForceY,
													AdditionalForceZ,
													restrictChat,
													NULL, //Changed from camPtrRef
													conPtr,
													m_input->m_mouseButtons[MOUSE_LEFTBUTTON].IsDown());
			         Accellerate(ptrLoc->List_movementDB,ptrLoc->m_aiObj->m_currentSpeedOverride,m_gStatus->m_timeAdjustFloat);
				 }//end get closer
			
			}//end interactive type
			else
			{//general player entity
				//Move towards Entity
				USER_AIFollowAttackTargetState(ptrLoc,m_gStatus->m_timeAdjustFloat,m_gStatus->m_entObjRuntime,currentTime,m_gStatus->m_tgSys_entitySelected);

				//Attack if health is above zero
				if(selectedEntPtr->List_energy > 0)
					USER_AIFireAtEnemy(ptrLoc,m_gStatus->m_timeAdjustFloat,m_gStatus->m_entObjRuntime,currentTime);
					
				if(!menusHandledInput)
					ProcessKeyboardMouseButtons(ptrLoc, 
												entityPtr,
												lookfirePtr,
												currentTime,
												jetsWereUsed,
												AdditionalForceX,
												AdditionalForceY,
												AdditionalForceZ,
												restrictChat,
												NULL, //Changed from camPtrRef
												conPtr,
												m_input->m_mouseButtons[MOUSE_LEFTBUTTON].IsDown());
			}//end general player entity
		 }//end valid ptr
		 else
		 {//reset targeting systems
            m_gStatus->m_tgSys_indexOfObjectSelected = -1;
            m_gStatus->m_tgSys_entitySelected = -1;
		 }//end reset targeting systems

		 USER_AI_Transform(ptrLoc,m_gStatus->m_waterZoneDB,m_gStatus->m_timeAdjust,m_gStatus->m_lastTimeAdjust);
	 }//end entity selected
	 else if( m_gStatus->m_tgSys_indexOfObjectSelected > -1 )
	 {//ground selected
		 //non aggressive simple mevement
		 lookfirePtr->List_currentlyFireing = FALSE;

		 ptrLoc->m_aiObj->m_lookAt = m_gStatus->m_tgSys_latestWldIntersection;
		 ptrLoc->m_aiObj->m_lookAt.y += ptrLoc->m_aiObj->m_prefernceAboveTarget;




		 float dist = m_gStatus->m_matLib.MagnitudeSquared(ptrLoc->List_baseFrame->List_frameMatrix,ptrLoc->m_aiObj->m_lookAt);
		 ptrLoc->m_aiObj->List_closeEnoughDist = ptrLoc->m_aiObj->m_travelingDesiredDist;//global for tree state
		 if(dist <= ptrLoc->m_aiObj->List_closeEnoughDist)
		 {//close enough to node get next node
		    D3DVECTOR localPos = m_gStatus->m_matLib.GetLocalPositionFromMatrixView(ptrLoc->List_baseFrame->List_frameMatrix,ptrLoc->m_aiObj->m_lookAt);
		    AutoSteeringBasedOnAIObj(ptrLoc,localPos,m_gStatus->m_timeAdjustFloat);
			//ProcessChat(ptrLoc,entityPtr);
			if(!menusHandledInput)
				ProcessKeyboardMouseButtons(ptrLoc, 
											entityPtr,
											lookfirePtr,
											currentTime,
											jetsWereUsed,
											AdditionalForceX,
											AdditionalForceY,
											AdditionalForceZ,
											restrictChat,
											NULL, //Changed from camPtrRef
											conPtr,
											m_input->m_mouseButtons[MOUSE_LEFTBUTTON].IsDown());
			ZeroForces(ptrLoc->List_movementDB);
		 }
		 else
		 {
			D3DVECTOR localPos = m_gStatus->m_matLib.GetLocalPositionFromMatrixView(ptrLoc->List_baseFrame->List_frameMatrix,ptrLoc->m_aiObj->m_lookAt);
			 
			localPos.y += ptrLoc->m_aiObj->m_prefernceAboveTarget;
			AutoSteeringBasedOnAIObj(ptrLoc,localPos,m_gStatus->m_timeAdjustFloat);
			//ProcessChat(ptrLoc,entityPtr);
			if(!menusHandledInput)
				ProcessKeyboardMouseButtons(ptrLoc, 
											entityPtr,
											lookfirePtr,
											currentTime,
											jetsWereUsed,
											AdditionalForceX,
											AdditionalForceY,
											AdditionalForceZ,
											restrictChat,
											NULL, //Changed from camPtrRef
											conPtr,
											m_input->m_mouseButtons[MOUSE_LEFTBUTTON].IsDown());
			Accellerate(ptrLoc->List_movementDB,ptrLoc->m_aiObj->m_currentSpeedOverride,m_gStatus->m_timeAdjustFloat);
		 }
		 USER_AI_Transform(ptrLoc,m_gStatus->m_waterZoneDB,m_gStatus->m_timeAdjust,m_gStatus->m_lastTimeAdjust);
	 }//end ground selected
	 else 
	 {//Nothing selected
		 //ProcessChat(ptrLoc,entityPtr);

		 //Reset firing bool in case we just killed something
		 lookfirePtr->List_currentlyFireing = FALSE;
		 
		 if(!menusHandledInput)
			 ProcessKeyboardMouseButtons(ptrLoc, 
										entityPtr,
										lookfirePtr,
										currentTime,
										jetsWereUsed,
										AdditionalForceX,
										AdditionalForceY,
										AdditionalForceZ,
										restrictChat,
										NULL, //Changed from camPtrRef
										conPtr,
										m_input->m_mouseButtons[MOUSE_LEFTBUTTON].IsDown());
        ZeroForces(ptrLoc->List_movementDB);
		USER_AI_Transform(ptrLoc,m_gStatus->m_waterZoneDB,m_gStatus->m_timeAdjust,m_gStatus->m_lastTimeAdjust);
	 }//end nothing selected

	//Regenerate Jets / Energy
	RegenJet(ptrLoc,10);
	m_lastMode = 1;
}
//------------------------------------------------------------------------------------//
void UserController::TargetingSystemCallback()
{
	if(!m_gStatus->m_dxRenderer)
		return;

	if(!m_gStatus->m_showMenuCursor)
	    return;

    DWORD currentTime = GetTickCount();

	 float remappedX = 0.0f;
	 float remappedY = 0.0f;

	 //m_gStatus->m_tgSys_camMatrixBasis = matrixBase;//store for targeting basis
	 //m_gStatus->m_tgSys_projMatrixBasis = matProj;//store for targeting basis
	 	 remappedY = m_gStatus->m_mouseCursors[m_gStatus->m_currentCursor]->m_posY + m_gStatus->m_mouseCursors[m_gStatus->m_currentCursor]->m_height;
	 //remap coordinates to center 
	 CoordinateRemapper( m_gStatus->m_mouseCursors[m_gStatus->m_currentCursor]->m_posX,//int & orthoX,
						 remappedY,//int & orthoY,
						 m_gStatus->m_dxRenderer->m_currentWidth,//int & orthoScreenWidth,
						 m_gStatus->m_dxRenderer->m_currentHeight,//int & orthoScreenHeight,
						 m_gStatus->m_tgSys_clipWidth,//float & virtualWidth,
						 m_gStatus->m_tgSys_clipHeight,//float & virtualHeight,
						 &remappedX,
						 &remappedY);//change to front clip planne coordinates

	 CoordinateCornerToCenter( m_gStatus->m_tgSys_clipWidth,
						       m_gStatus->m_tgSys_clipHeight,
						       remappedX,
						       remappedY);//remap coordinated to 0,0 cenetered

	 D3DVECTOR startVect,endVect;
	 ZeroMemory(&startVect,sizeof(D3DVECTOR));
	 //startVect.x = m_gStatus->m_tgSys_camMatrixBasis._41;
	 //startVect.y = m_gStatus->m_tgSys_camMatrixBasis._42;
	 //startVect.z = m_gStatus->m_tgSys_camMatrixBasis._43;
	 
	 endVect.x = remappedX;
	 endVect.y = remappedY;
	 endVect.z = m_gStatus->m_tgSys_projMatrixBasis._34;

	 //generate ray vector
	 //endVect.x = endVect.x - m_gStatus->m_tgSys_camMatrixBasis._41;
	 //endVect.y = endVect.y - m_gStatus->m_tgSys_camMatrixBasis._42;
	 //endVect.z = endVect.z - m_gStatus->m_tgSys_camMatrixBasis._43;

	 endVect = m_gStatus->m_matLib.Normalize(endVect);
     //force specified range
	 endVect.x = endVect.x * m_gStatus->m_tgSys_validRange;
	 endVect.y = endVect.y * m_gStatus->m_tgSys_validRange;
	 endVect.z = endVect.z * m_gStatus->m_tgSys_validRange;

	 endVect = m_gStatus->m_matLib.UpdatePolygonVertex(m_gStatus->m_tgSys_camMatrixBasis.m,endVect);
	 startVect = m_gStatus->m_matLib.UpdatePolygonVertex(m_gStatus->m_tgSys_camMatrixBasis.m,startVect);

	 //VISUAL TUNING TEST
	 //D3DVECTOR startVectMod = startVect;
	 //startVectMod.y += .01f;
	 //m_gStatus->m_dVectors.Clear();
     //m_gStatus->m_dVectors.AddLineWLD(startVectMod,endVect);
	 //END VISUAL TUNING TEST

	 //Reset the current cursor
	 m_gStatus->m_currentCursor = CURSOR_NORMAL;

     BOOL mouseLbuttonIsDown = FALSE;
	 if(m_input->m_mouseButtons[MOUSE_LEFTBUTTON].IsDown())
	    mouseLbuttonIsDown = TRUE;

	 if(mouseLbuttonIsDown == FALSE || (currentTime-m_gStatus->m_tgSys_quedButtonDownTimeStamp) > m_gStatus->m_tgSys_quedButtonDownTimeDuration)
		m_gStatus->m_tgSys_quedButtonDown = FALSE;

	 m_gStatus->m_tgSys_expQue = -1;//reset click effect
	 int selectionMode = 0;//hover state
	 if(m_gStatus->m_tgSys_quedButtonDown == FALSE)
	   if(mouseLbuttonIsDown == TRUE)//mouse selection is down or always calculating
	   {
	      selectionMode = 1;//click state
          m_gStatus->m_tgSys_quedButtonDown = 1;
		  m_gStatus->m_tgSys_quedButtonDownTimeStamp = currentTime;
	   }

     if(selectionMode == 1)
	 {//direct selection mode
		if(m_gStatus->m_tgSys_entitySelected)
		{
			CMovementObj * selectedEntPtr = m_gStatus->m_entObjRuntime->GetObjByTrace(m_gStatus->m_tgSys_entitySelected);
			if(selectedEntPtr)
				selectedEntPtr->m_selectionEffectState = -1;

			m_gStatus->m_tgSys_entitySelected = -1;//reset
		}
		m_gStatus->m_tgSys_indexOfObjectSelected = -1;//reset
	 }//direct selection mode
	 else
	 {//hover selection mode
         m_gStatus->m_tgSys_hoverEntitySelected	= -1;//reset
		 m_gStatus->m_tgSys_hoverState			= 0;//reset
	 }//end hover selection mode

     /*byte type_temp = m_gStatus->m_tgSys_validRange * 2.0f;*/
	 byte type_temp = (byte)(m_gStatus->m_tgSys_validRange * 2.0f);
	 //WARNING: C4244 --> conversion from float to byte, possible loss of data

     int indexOfObjectSelected = 0;
	 //m_gStatus->m_tgSys_entitySelected = -1;
	 //m_gStatus->m_tgSys_indexOfObjectSelected = -1;
	 D3DVECTOR intersection, lastNormal;
	 float collision_dist = 0.0f;
     if(m_gStatus->m_collisionBase->GetClosesIntersectionUseSeg(startVect,
																endVect,
																startVect,
																endVect,
																&intersection.x,
																&intersection.y,
																&intersection.z,
																&indexOfObjectSelected,
																&lastNormal,
																true,
																collision_dist,
																type_temp) == TRUE)
		{//collided
		    if(selectionMode == 1)
			{
               //m_gStatus->m_tgSys_expQue = menu->m_cursor->m_expEffectOnClick; //HANDLE for CLICK EFFECT
			   m_gStatus->m_tgSys_indexOfObjectSelected = indexOfObjectSelected;
               m_gStatus->m_tgSys_latestWldIntersection = intersection;
               m_gStatus->m_tgSys_latestNormal = lastNormal;
			   m_gStatus->m_tgSys_dist = collision_dist;
			   m_gStatus->m_tgSys_type = type_temp;
			}	
		}//end collided

        //check ray to entities
	    float rayThickness = .01f;
		float curDist = 0.0f;
		float hvRadius = m_gStatus->m_tgSys_validRange * .5f;
		D3DMATRIX positionMatrix;
		D3DVECTOR bnPosition,centerSegVect;
        for(POSITION posLoc2 = m_gStatus->m_entObjRuntime->GetHeadPosition();posLoc2 != NULL;)
	    {//Begin entity loop
	        CMovementObj * entPtr = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetNext(posLoc2);
			
			entPtr->List_baseFrame->GetTransform(NULL,&positionMatrix);
            if(entPtr->List_skeletalObject->m_calulationSphere)
			{//valid collision calc object
			   D3DVECTOR bonePosition = m_gStatus->m_matLib.UpdatePolygonVertex(positionMatrix.m,entPtr->List_skeletalObject->GetBonePositionByIndex(entPtr->List_skeletalObject->m_calulationSphere->m_boneIndex));
               //get virtual Radius
			   centerSegVect.x = startVect.x + ((endVect.x - startVect.x) * .5f);
			   centerSegVect.y = startVect.y + ((endVect.y - startVect.y) * .5f);
			   centerSegVect.z = startVect.z + ((endVect.z - startVect.z) * .5f);
			   if(m_gStatus->m_matLib.BoundingSphereCheckX(hvRadius,
                                                           entPtr->List_skeletalObject->m_calulationSphere->m_sphereRadius,
													       bonePosition.x,
													       bonePosition.y,
													       bonePosition.z,
                                                           centerSegVect.x,
													       centerSegVect.y,
												 	       centerSegVect.z))
			   {//in calculation radius
			     
			     if(entPtr->List_skeletalObject->m_sphereLocationalDamageList){//valid locational damage list
		         for(POSITION dmgPos = entPtr->List_skeletalObject->m_sphereLocationalDamageList->GetHeadPosition();dmgPos != NULL;)
				 {//damage sphere loop
		             CSphereCollisionObj * dmgSph = (CSphereCollisionObj *)entPtr->List_skeletalObject->m_sphereLocationalDamageList->GetNext(dmgPos);
		             POSITION bnPos = entPtr->List_skeletalObject->m_bonesList->FindIndex(dmgSph->m_boneIndex);
                     if(bnPos){//valid
                        CBoneObject * bnPtr = (CBoneObject *)entPtr->List_skeletalObject->m_bonesList->GetAt(bnPos);
						bnPtr->m_boneFrame->GetPosition(NULL,&bnPosition);
                        bnPosition = m_gStatus->m_matLib.UpdatePolygonVertex(positionMatrix.m,bnPosition);
						
						if(m_gStatus->m_matLib.CollisionSphereSeg(startVect,
																  endVect,
					                                              dmgSph->m_sphereRadius,
																  bnPosition,
													              FALSE,
																  NULL,
																  rayThickness,
																  &intersection)==TRUE)
						{//hit
					         curDist = m_gStatus->m_matLib.MagnitudeSquared(startVect.x,startVect.y,startVect.z,intersection.x,intersection.y,intersection.z);
							 if(curDist < type_temp)
							 {//hit an entity
							    /*type_temp = curDist;*/
								 type_temp = (byte)curDist;
								//WARNING: C4244 --> conversion from float to byte, possible loss of data

							    if(selectionMode == 1)
								{//direct selection
                                   m_gStatus->m_tgSys_expQue = -1;//deselect ground effect because entity is selected/clicked
								   m_gStatus->m_tgSys_entitySelected = entPtr->List_traceNumber;
								   entPtr->m_selectionEffectState = 0;
								}//end direct selection
								else
								{//hover mode
                                   m_gStatus->m_tgSys_hoverEntitySelected = entPtr->List_traceNumber;
								   if(entPtr->List_controlType != 0)
								   {//hover logic code section
										if(entPtr->List_controlType == 35 || entPtr->List_energy <= 0)
										{
											m_gStatus->m_currentCursor = CURSOR_EXAMINE;
											m_gStatus->m_tgSys_hoverState = 2;//AI NPC (PICKUP CLASS NPC)
										}
										else
										{
											m_gStatus->m_currentCursor = CURSOR_ENEMYHOVER;
											m_gStatus->m_tgSys_hoverState = 1;//NPC MOVING AI SERVER CONTROLLED
										}
								   }//end hover logic code section
								}//end hover mode
								//CopyMemory(&closestIntesection,&intersection,sizeof(D3DVECTOR));   
							 }//end hit an entity
						}//end hit   
					 }//end valid
				 }//end damage sphere loop
			  }//end valid locational damage list
			 }//end in calculation radius
			}//end valid collision calc object
		}//end entity loop

		//-------------------
        //TEST
		if(0)//0 to turn test off
		{//enable test
        if(m_gStatus->m_tgSys_entitySelected > -1)
		{
		   CMovementObj * entPtrTemp = m_gStatus->m_entObjRuntime->GetObjByTrace(m_gStatus->m_tgSys_entitySelected);
		   if(entPtrTemp)
		   if(m_gStatus->m_systemReadout)
		   {
			  m_gStatus->m_systemReadout->AddLine("selected Entity Name: " + entPtrTemp->List_name);
			  m_gStatus->m_systemReadout->AddLine("selected Entity Trace: ", (DWORD)m_gStatus->m_tgSys_entitySelected);
			  D3DVECTOR entityPos;
			  entPtrTemp->List_baseFrame->GetPosition(NULL,&entityPos);
			  m_gStatus->m_systemReadout->AddLine("selected Entity position: ", entityPos);
			  
		   }
		}
		else if(m_gStatus->m_tgSys_indexOfObjectSelected > -1)
		{//check world
           CWldObject * wldObjTemp = m_gStatus->m_collisionBase->m_wldObjectReferenceArray[m_gStatus->m_tgSys_indexOfObjectSelected].worldObjectReference;//crashes housing
	       if(wldObjTemp)
		   if(m_gStatus->m_systemReadout)
	       {
			   m_gStatus->m_systemReadout->AddLine("object Name: " + wldObjTemp->m_fileNameRef);
               m_gStatus->m_systemReadout->AddLine("Intersection: ", m_gStatus->m_tgSys_latestWldIntersection);
			   m_gStatus->m_systemReadout->AddLine("Latest Normal: ", m_gStatus->m_tgSys_latestNormal);
	       }
		}//end check world
		}//end enable test
		//END TEST
	    //--------------------
	
	return;
}
//------------------------------------------------------------------------------------//
void UserController::CoordinateRemapper( float & orthoX,
										 float & orthoY,
										 int & orthoScreenWidth,
										 int & orthoScreenHeight,
										 float & virtualWidth,
										 float & virtualHeight,
										 float * newX,
										 float * newY)
{
         if(orthoScreenWidth == 0 || orthoScreenHeight == 0)
		    return;

         *newX = (orthoX/(float)orthoScreenWidth)*virtualWidth;
         *newY = (orthoY/(float)orthoScreenHeight)*virtualHeight;
}
//-------------------------------------------------------------------------------------//
void UserController::CoordinateCornerToCenter(  float & Width,
										       float & Height,
										       float & posX,
										       float & posY)
{
     posX -= Width/2.0f;
	 //posY = -posY;
     posY -= Height/2.0f;
}
//--------------Digital Controls In Loop----------------------------------------------//
void UserController::GlobalControlsInLoop()
{
	if(!m_input->m_directKeyboard)
		return;

	if(m_input->m_keyboardKeys[DIK_RCONTROL].IsDown() && m_input->m_keyboardKeys[DIK_F2].IsDown())
	{//snap shot
		m_gStatus->m_logRenderSection = TRUE;
		m_gStatus->m_totalWorldPolys = 0;
		CString filename = "rendersection.log";
		CFileException exc;
		CFile fl;
		CArchive the_out_Archive(&fl,CArchive::store);
		BOOL res = fl.Open(filename,CFile::modeCreate | CFile::modeWrite, &exc);
		the_out_Archive.WriteString( "Cleared...");  
		the_out_Archive.WriteString( "\n");   
		the_out_Archive.Close();
		fl.Close(); 
		return;
	}//end snap shot

	if(m_input->m_ctrlOn && m_input->m_keyboardKeys[DIK_Q].IsDown())
	{
		//Start Game Shutdown
		m_gStatus->SetCloseDownCallState(TRUE);
		return;
	}

	if(m_input->m_ctrlOn && m_input->m_keyboardKeys[DIK_F10].IsDown())
	{//test
		
        CMovementObj * mPtr = m_gStatus->m_entObjRuntime->GetObjByIndex(0);
		if(mPtr)
		{
           CMovementObj * DBPtr = m_gStatus->m_entObjList->GetByID(mPtr->List_refModel);
		   if(!DBPtr)
			  return;

		   m_gStatus->m_itemManager->ArmEntity(133,mPtr,DBPtr,m_gStatus->m_3dDevice);

		}


		return;
	}
 
	if(m_input->m_keyboardKeys[DIK_ESCAPE].JustPressed())
	{
		//const unsigned int GAME_CURSOR_ID = 172;	//TODO: Switch this to use a value from the "important menu" array

		//if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
		//{
			// Init cursor visibility
			if(m_gStatus->m_showMenuCursor >= 1) m_bShowCursor = true;
			else m_bShowCursor = false;

			//Close All Menus
			bool bClosedAllMenus = m_gStatus->m_runtimeMenuDatabase->CloseAll(&m_gStatus->m_showMenuCursor);

			
			// If we have any menus remaining open, always show cursor
			if(!bClosedAllMenus)
				m_bShowCursor = true;
			// No menus have been closed, so just toggle the cursor visiblity
			else
			{
				m_gStatus->m_showMenuCursor = 0;
				m_bShowCursor = !m_bShowCursor;
			}

			// IF the cursor is still visible, SHOW cursor notification message
			if(m_bShowCursor)
			{
				m_gStatus->OpenMenu(GAME_CURSOR_ID, true);
				m_gStatus->OpenMenu(CURSOR_NOTIFICATION_MENU, false);
			}
			else
			{
				m_gStatus->ShowHoverText(ID_HOVER_TEXT_MENU, "");
				m_gStatus->m_runtimeMenuDatabase->CloseByID(ID_HOVER_TEXT_MENU, &m_gStatus->m_showMenuCursor);
			}
		//}
		//else
		//if(!m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
		//m_gStatus->m_dxRenderer->ToggleMouse();
	}

	// Toggle the in-game system menu
	if(m_input->m_keyboardKeys[DIK_F1].JustPressed())
	{
		CMenuVisual *pVisual = m_gStatus->m_runtimeMenuDatabase->GetVisualByID(SYSTEM_MENU_ID, true);
		if(pVisual)
		{
			m_gStatus->m_runtimeMenuDatabase->CloseByID(SYSTEM_MENU_ID, &m_gStatus->m_showMenuCursor);
			//bool bClosedAllMenus = m_gStatus->m_runtimeMenuDatabase->CloseAll(&m_gStatus->m_showMenuCursor);
		}
		else
		{
			m_gStatus->OpenMenu(SYSTEM_MENU_ID, false);
		}
	}
 
	if(m_input->m_keyboardKeys[DIK_F2].JustPressed())
	{
		

		/* DEPRECATED /////////////////////////////////////////////////////////
		//Close All Menus and Open Actions Menu
		bool bClosedAllMenus = m_gStatus->m_runtimeMenuDatabase->CloseAll(&m_gStatus->m_showMenuCursor);
		if(bClosedAllMenus)
			m_gStatus->OpenMenu(206, false); //TODO: Switch this to use a value from the "important menu" array
		/////////////////////////////////////////////////////////////////////*/
	}

	if(m_input->m_keyboardKeys[DIK_F3].JustPressed())
	{
		//Add Three Blank Lines to Chat Window
/*		if(m_gStatus->m_systemReadout)
		{
			m_gStatus->m_systemReadout->AddLine("   ");
			m_gStatus->m_systemReadout->AddLine("   ");
			m_gStatus->m_systemReadout->AddLine("   ");
		}
*/		if(!m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
		{
			if(m_gStatus->m_renderStateObject->m_forceWireframe)
				m_gStatus->m_renderStateObject->m_forceWireframe = 0;
			else
				m_gStatus->m_renderStateObject->m_forceWireframe = 1;

			m_gStatus->m_renderStateObject->SetGlobalWireFrameMode(m_gStatus->m_renderStateObject->m_forceWireframe,m_gStatus->m_dxRenderer->m_pd3dDevice);
		}
	}

	if(m_input->m_keyboardKeys[DIK_F4].JustPressed())
	{
		if(!m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
		{
			if(m_gStatus->m_renderStateObject->m_forceShadeMode)
				m_gStatus->m_renderStateObject->m_forceShadeMode = 0;
			else
				m_gStatus->m_renderStateObject->m_forceShadeMode = 1;
		}
	}

	//Close All Menus and Open Inventory
	if(m_input->m_keyboardKeys[DIK_F5].IsDown())
		m_gStatus->OpenMenu(ID_PLAYER_INVENTORY_MENU, false);

	if(m_input->m_keyboardKeys[DIK_F11].IsDown())
	{
		//Toggle Showing FPS and Error Reports
		m_gStatus->GL_showFps  = !m_gStatus->GL_showFps;
		m_gStatus->ErrorOutPut = !m_gStatus->ErrorOutPut;
	}

	if(m_input->m_keyboardKeys[DIK_F12].JustPressed())
	{
		//Release Mouse to Windows	
		m_gStatus->m_dxRenderer->ToggleMouse();
	}
}

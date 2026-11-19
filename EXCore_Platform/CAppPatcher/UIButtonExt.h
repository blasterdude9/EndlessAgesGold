#pragma once
#include "UIImageExt.h"

class CUIButtonExt : public CObject
{
public:
	CUIButtonExt(void);
public:
	~CUIButtonExt(void);


	 HBITMAP                         m_buttonUp;
	 HBITMAP                         m_buttonDown;
	 HBITMAP                         m_buttonOver;

	 CStatic                        *m_stControlRef;

	 int                             m_buttonID;
	 int                             m_state;
     int							 m_functionID;
	 int                             m_currentStatus;//0=up 1=hover 2=down

};

class CUIButtonExtList : public CObList
{
   public:

	   CUIButtonExtList(){};
	   ~CUIButtonExtList()
	   {
	      for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
		  {
              CUIButtonExt * ptr = (CUIButtonExt *)GetNext(posLoc);
			  delete ptr;
			  ptr = 0;
		  }
	      RemoveAll();
	   };

	   CUIImageExt imgBtn;

	   BOOL RemoveButton(int buttonID)
	   {

		    POSITION posLast;
            for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;)
		    {
              CUIButtonExt * ptr = (CUIButtonExt *)GetNext(posLoc);
			  if(ptr->m_buttonID == buttonID)
			  {
			     ptr->m_stControlRef->ShowWindow(0);
				 Sleep(0);
			     ptr->m_stControlRef->SetBitmap(NULL);
			     delete ptr;
			     ptr = 0;
				 RemoveAt(posLast);
				 return TRUE;
			  }
		    }
			return FALSE;
	   }

	   BOOL AddButton(int resourceUp, int resourceOver, int resourceDown, CStatic & controlRef, int buttonID, int functionID)
	   {
            CUIButtonExt * uiBtn = new CUIButtonExt();

			uiBtn->m_buttonUp		= imgBtn.LoadAnImage("",resourceUp);
			uiBtn->m_buttonOver		= imgBtn.LoadAnImage("",resourceOver);
			uiBtn->m_buttonDown		= imgBtn.LoadAnImage("",resourceDown);
			uiBtn->m_stControlRef	= &controlRef;
			uiBtn->m_buttonID		= buttonID;
			uiBtn->m_functionID		= functionID;
			uiBtn->m_state          = -1;

			AddTail(uiBtn);
			uiBtn->m_stControlRef->ShowWindow(1);
			uiBtn->m_stControlRef->SetBitmap(uiBtn->m_buttonUp);
			uiBtn->m_state = 0;
			return TRUE;
	   }

	   int Evaluate(CPoint & point, BOOL mButtonDown, CWnd * parentWnd)
	   {
            RECT wRect;
			int buttonDown = -1;
			int localStatus = 0;
			BOOL inRect = 0;
		    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
			{
                CUIButtonExt * ptr = (CUIButtonExt *)GetNext(posLoc);

				
				ptr->m_stControlRef->GetClientRect(&wRect);
				localStatus = 0;

				parentWnd->MapWindowPoints(ptr->m_stControlRef,&point,1);
				if(point.x >= wRect.left && point.x <= wRect.right)
				   if(point.y >= wRect.top && point.y <= wRect.bottom)
				   {//inside rect
                      localStatus = 1;
					  if(mButtonDown)
					  {
                         localStatus = 2;
                         
					  }
				   }//end inside rect

			    if(localStatus != ptr->m_currentStatus)
				{//update render state
                   switch(localStatus)
				   {
				       case 0://up state
                           ptr->m_stControlRef->SetBitmap(ptr->m_buttonUp);
						   ptr->m_state = 0;
					   break;
					   case 1://hover state
                           ptr->m_stControlRef->SetBitmap(ptr->m_buttonOver);
                           if(ptr->m_state == 2)
						      buttonDown = ptr->m_functionID;

						   ptr->m_state = 1;
					   break;
					   case 2://down state
                           ptr->m_stControlRef->SetBitmap(ptr->m_buttonDown);
						   ptr->m_state = 2;
					   break;
				   }
                   ptr->m_currentStatus = localStatus;
				}//end update render state
			    
			}
			return buttonDown;
	   }

};


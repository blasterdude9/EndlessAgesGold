#include "globaldefines.h"
#include "CabFunctionLib.h"
//DirectDraw 7 removal: CABD3DFunctionLib.h deleted - keep its non-DDraw
//transitive includes explicit so downstream translation units still build
#include <mmsystem.h>
#include "dsound.h"
#include "dinput.h"
#include "clientmodule.h"

#pragma once

struct DisplayMode
{
    int width,hight,depth;
};

class RMWin : public CFrameWnd
{
public:
	
	//organized interfaces to be referenced
	CClientModule           * m_clientEngine;
    GlobalStatusVariables	* m_gStatus;
    UserController			* m_userControls;
	SceneAssembly			* m_sceneAssembler;
	CClientNetworkingLogic	* m_cNetworkObject;
	CMenuManagement			* m_menuManagement;
	CRenderingManagement    * m_rendermanagerEd;
	//DxDeviceClass * m_gStatus->m_dxRenderer;
    //--------Base Varials--------//
	//int m_localContentClientVersion;
    //MODES
    int                    m_curModeHieght;
    int                    m_curModeWidth;
	int                    m_curModeDepth;
	int                    m_lastModeHieght;
    int                    m_lastModeWidth;
	int                    m_lastModeDepth;
	BOOL                   m_wideScreen;
	int                    m_szXMonitor;
    int                    m_szYMonitor;
	UINT                   m_windowStateGL;
	BOOL                   m_appFullscreen;
	BOOL                   m_showEditor;
	BOOL CursorIsDisplayed;
    CRect GuiRect;
    CString m_publicNotifyDeviceType;
	HINSTANCE instanceHandle;
    //Port Libraries
	HWND hwndWindow;
	//public functions
	virtual void RenderLoop() = 0;
	RMWin();
	RMWin(int w,int h);
	BOOL Create(const CString& sTitle,int icon,int menu);  
protected:
	static int GetMouseX()    { return mousex; }
	static int GetMouseY()    { return mousey; }
	virtual void OnIdle( long ) { }
protected:
	//{{AFX_MSG(RMWin)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnActivate(UINT state, CWnd* other, BOOL minimize);
	afx_msg void OnSize(UINT type, int cx, int cy);
	afx_msg void OnMouseMove( UINT state, CPoint point );
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void Initvars();
	virtual BOOL CreateScene() = 0;
	virtual void FreeAllThenClose() = 0;
	
protected:

private:
	static long mousex;
	static long mousey;
	static UINT mousestate;
	CRect winrect;
	
	//friend class RMApp;
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


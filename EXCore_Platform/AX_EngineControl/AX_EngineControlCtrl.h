#pragma once

// AX_EngineControlCtrl.h : Declaration of the CAX_EngineControlCtrl ActiveX Control class.
#include "HttpFileDownloader.h"
#include "Cabinet.hpp"
#include "clientmodule.h"
#include "AXContentClient.h"
#include "wmp.h"
#include "loggingclass.h"
#include "crcgenerator.h"
#include "DirectoryRegistryWrapper.h"
#include "ax_xmlConnector.h"
#include "crcgenerator.h"
#include "CABFunctionLib.h"

// CAX_EngineControlCtrl : See AX_EngineControlCtrl.cpp for implementation.

class CAX_EngineControlCtrl : public COleControl
{
	DECLARE_DYNCREATE(CAX_EngineControlCtrl)

// Constructor
public:
	CAX_EngineControlCtrl();


	CABFunctionLib      m_cablib;
	CClientModule *		m_engineClient;
	HANDLE				m_renderThread;
	DWORD				m_renderThreadID;
	BOOL				m_renderThreadRuinning;
	HANDLE				m_xmlThread;
	DWORD				m_xmlThreadID;
	BOOL				m_xmlThreadRuinning;
	CRITICAL_SECTION	m_updateCriticalSection;
	BOOL                m_closeDown;
	int                 m_maxTextureSize;
	int                 m_shadowCount;
	CDirectoryRegistryWrapper m_abTool;
	CLoggingClass       m_axLogger;

	//socket interface for xml streaming
	CAX_XMLConnector * m_xmlInterfaceServer;
	//functions
	
    void FireEngineUp();
	void BeginRendering();
	void XmlCommandParser(CAX_XMLCommandQueObj * cmdQue);
	bool GetNextOpenTag(BYTE * data, long & currentPosition, long & length, CString & tag, CString & value);
	bool GetValueByTagFromCursor(BYTE * data, long & currentPosition, long & length,CString inputTag, CString & value, CString limitTag);
	bool SeekPositionToNextValue(BYTE * data, long & currentPosition, long & length,CString & nextValue);
	bool GetValueByTagFromCursor(BYTE * data, long & currentPosition, long & length,CString inputTag, float & value, CString limitTag);
    bool GetValueByTagFromCursor(BYTE * data, long & currentPosition, long & length,CString inputTag, int & value, CString limitTag);

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// Implementation
protected:
	~CAX_EngineControlCtrl();

	DECLARE_OLECREATE_EX(CAX_EngineControlCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CAX_EngineControlCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CAX_EngineControlCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CAX_EngineControlCtrl)		// Type name and misc status

// Message maps
	DECLARE_MESSAGE_MAP()

	int                 m_port;
	BOOL                m_autoStart;
// Dispatch maps
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBoxX();
    void SetPort(int port);
	void Startup(int port);
// Event maps
	DECLARE_EVENT_MAP()

	
// Dispatch and event IDs
public:
	enum {
	};
	
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


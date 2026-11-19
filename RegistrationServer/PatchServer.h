// PatchServer.h : main header file for the PATCHSERVER application
//

#if !defined(AFX_PATCHSERVER_H__189C7D12_3437_4E98_B16E_A37B8FE3CD33__INCLUDED_)
#define AFX_PATCHSERVER_H__189C7D12_3437_4E98_B16E_A37B8FE3CD33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPatchServerApp:
// See PatchServer.cpp for the implementation of this class
//

class CPatchServerApp : public CWinApp
{
public:
	CPatchServerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPatchServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPatchServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATCHSERVER_H__189C7D12_3437_4E98_B16E_A37B8FE3CD33__INCLUDED_)

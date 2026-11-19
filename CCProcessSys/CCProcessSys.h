// CCProcessSys.h : main header file for the CCPROCESSSYS application
//

#if !defined(AFX_CCPROCESSSYS_H__83EE7829_2F8D_4549_BB96_D3B3723D6F95__INCLUDED_)
#define AFX_CCPROCESSSYS_H__83EE7829_2F8D_4549_BB96_D3B3723D6F95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCCProcessSysApp:
// See CCProcessSys.cpp for the implementation of this class
//

class CCCProcessSysApp : public CWinApp
{
public:
	CCCProcessSysApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCCProcessSysApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCCProcessSysApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCPROCESSSYS_H__83EE7829_2F8D_4549_BB96_D3B3723D6F95__INCLUDED_)

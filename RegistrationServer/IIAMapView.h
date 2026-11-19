#if !defined(AFX_IIAMAPVIEW_H__E461E58D_15FE_4F35_92FD_EFD06798895C__INCLUDED_)
#define AFX_IIAMAPVIEW_H__E461E58D_15FE_4F35_92FD_EFD06798895C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IIAMapView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIIAMapView dialog

class CIIAMapView : public CDialog
{
// Construction
public:
	CIIAMapView(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIIAMapView)
	enum { IDD = IDD_DIALOG_MAPVIEW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIIAMapView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIIAMapView)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IIAMAPVIEW_H__E461E58D_15FE_4F35_92FD_EFD06798895C__INCLUDED_)

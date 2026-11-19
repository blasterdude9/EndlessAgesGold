// OpenURLView.cpp : implementation of the COpenURLView class
//

#include "stdafx.h"
#include "OpenURL.h"

#include "OpenURLDoc.h"
#include "OpenURLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenURLView

IMPLEMENT_DYNCREATE(COpenURLView, CHtmlView)

BEGIN_MESSAGE_MAP(COpenURLView, CHtmlView)
END_MESSAGE_MAP()

// COpenURLView construction/destruction

COpenURLView::COpenURLView()
{
    
}

COpenURLView::~COpenURLView()
{
}

BOOL COpenURLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CHtmlView::PreCreateWindow(cs);
}

void COpenURLView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();
	Navigate2(_T("https://www.endlessagesportal.com/prepurchaseoptions.aspx"),NULL,NULL);
	
}


// COpenURLView diagnostics

#ifdef _DEBUG
void COpenURLView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void COpenURLView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

COpenURLDoc* COpenURLView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenURLDoc)));
	return (COpenURLDoc*)m_pDocument;
}
#endif //_DEBUG


// COpenURLView message handlers

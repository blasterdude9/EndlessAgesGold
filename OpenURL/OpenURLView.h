// OpenURLView.h : interface of the COpenURLView class
//


#pragma once


class COpenURLView : public CHtmlView
{
protected: // create from serialization only
	COpenURLView();
	DECLARE_DYNCREATE(COpenURLView)

// Attributes
public:
	COpenURLDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~COpenURLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in OpenURLView.cpp
inline COpenURLDoc* COpenURLView::GetDocument() const
   { return reinterpret_cast<COpenURLDoc*>(m_pDocument); }
#endif


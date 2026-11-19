// OpenURLDoc.h : interface of the COpenURLDoc class
//


#pragma once

class COpenURLDoc : public CDocument
{
protected: // create from serialization only
	COpenURLDoc();
	DECLARE_DYNCREATE(COpenURLDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~COpenURLDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};



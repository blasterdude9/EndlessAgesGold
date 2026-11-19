#pragma once

// AbDeploymentCntrlPropPage.h : Declaration of the CAbDeploymentCntrlPropPage property page class.


// CAbDeploymentCntrlPropPage : See AbDeploymentCntrlPropPage.cpp for implementation.

class CAbDeploymentCntrlPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CAbDeploymentCntrlPropPage)
	DECLARE_OLECREATE_EX(CAbDeploymentCntrlPropPage)

// Constructor
public:
	CAbDeploymentCntrlPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_ABDEPLOYMENTCNTRL };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};


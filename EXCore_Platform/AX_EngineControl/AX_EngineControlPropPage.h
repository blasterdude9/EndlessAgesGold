#pragma once

// AX_EngineControlPropPage.h : Declaration of the CAX_EngineControlPropPage property page class.


// CAX_EngineControlPropPage : See AX_EngineControlPropPage.cpp for implementation.

class CAX_EngineControlPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CAX_EngineControlPropPage)
	DECLARE_OLECREATE_EX(CAX_EngineControlPropPage)

// Constructor
public:
	CAX_EngineControlPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_AX_ENGINECONTROL };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};


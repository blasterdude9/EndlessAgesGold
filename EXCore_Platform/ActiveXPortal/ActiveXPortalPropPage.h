#pragma once

//ActiveXPortalPropPage.h : Declaration of the CActiveXPortalPropPage property page class.


//CActiveXPortalPropPage : See ActiveXPortalPropPage.cpp for implementation.

class CActiveXPortalPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CActiveXPortalPropPage)
	DECLARE_OLECREATE_EX(CActiveXPortalPropPage)

//Constructor
public:
	CActiveXPortalPropPage();

//Dialog Data
	enum { IDD = IDD_PROPPAGE_ACTIVEXPORTAL };

//Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    //DDX/DDV support

//Message maps
protected:
	DECLARE_MESSAGE_MAP()
};


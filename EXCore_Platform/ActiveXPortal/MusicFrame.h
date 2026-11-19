#pragma once


// CMusicFrame dialog

class CMusicFrame : public CDialog
{
	DECLARE_DYNAMIC(CMusicFrame)

public:
	CMusicFrame(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMusicFrame();

// Dialog Data
	enum { IDD = IDD_DIALOG_musicPlayer };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

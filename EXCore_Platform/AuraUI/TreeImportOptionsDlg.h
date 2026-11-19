#pragma once


// CTreeImportOptionsDlg dialog

class CTreeImportOptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(CTreeImportOptionsDlg)

public:
	CTreeImportOptionsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTreeImportOptionsDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_TreeImportSettings };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_leafLodCount;
	int m_sizeOverride;
};

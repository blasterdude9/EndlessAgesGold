// CRCGeneratorDlg.h : header file
//

#pragma once


// CCRCGeneratorDlg dialog
class CCRCGeneratorDlg : public CDialog
{
// Construction
public:
	CCRCGeneratorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CRCGENERATOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
    unsigned long crcTable[256];
	void crcgen( );
	unsigned long getcrc( CFile *fp );
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPickfile();
	LONGLONG m_CRCOUT;
	afx_msg void OnBnClickedButtonCabtest();
};

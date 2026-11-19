#pragma once
#include "CSoundTrackClass.h"
#include "CABFunctionLib.h"
#include "afxwin.h"

// CMusicDlg dialog

class CMusicDlg : public CDialog
{
	DECLARE_DYNAMIC(CMusicDlg)

public:
	CMusicDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMusicDlg();

	CSoundTrackObjList *  m_soundTrackModuleRef;
    CSoundTrackObjList ** m_soundTrackMemoryRef;
	CABFunctionLib		   m_cabLib;
	LPDIRECTSOUND          m_dsoundRef;
	HWND                   m_hwndMainRef;
	CString					m_soundFilePath;
// Dialog Data
	enum { IDD = IDD_DIALOG_MusicDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    void InList(int sel);

	CString m_soundTrackName;
	CString m_urlPath;
	afx_msg void OnBnClickedButtonAddsoundtrack();
	afx_msg void OnBnClickedButtonDeletesoundtrack();
	afx_msg void OnBnClickedButtonReplacesoundtrack();
	afx_msg void OnLbnSelchangeListSoundtracklist();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnLoadsddb();
	afx_msg void OnBnClickedBtnSavesddb();
	CListBox m_musicList;


	afx_msg void OnBnClickedBtnTestmusic();
	CString m_urlStatus;
};

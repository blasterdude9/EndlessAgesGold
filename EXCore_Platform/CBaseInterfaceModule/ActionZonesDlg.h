#pragma once
#include "afxwin.h"
#include "actionzones.h"
#include "CABFunctionLib.h"

// CActionZonesDlg dialog

class CActionZonesDlg : public CDialog
{
	DECLARE_DYNAMIC(CActionZonesDlg)

public:
	CActionZonesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CActionZonesDlg();

	CActionZoneDB * m_actionZoneDB;
	CABFunctionLib m_cabLib;
// Dialog Data
	enum { IDD = IDD_DIALOG_ActionZonesDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox CWorldChannelList;
	CListBox CActionZoneList;
	float m_maxX;
	float m_maxY;
	float m_maxZ;
	float m_minX;
	float m_minY;
	float m_minZ;
	int m_actionValue;
	CComboBox m_actionType;
	afx_msg void OnBnClickedButtonAcaddchannel();
	afx_msg void OnBnClickedButtonAcdeletechannel();
	afx_msg void OnBnClickedButtonAcadd();
	afx_msg void OnBnClickedButtonAcreplace();
	afx_msg void OnBnClickedButtonAcdelete();
	afx_msg void OnBnClickedButtonLoadarw();
	virtual BOOL OnInitDialog();

	void InListChannels(int sel);
    void InListZones(int sel);
	CString m_zoneName;
	afx_msg void OnLbnSelchangeListChannels();
	afx_msg void OnLbnSelchangeListZonelist();
	afx_msg void OnBnClickedButtonLoaddb();
	afx_msg void OnBnClickedButtonSavedb();
	int m_attributeValue;
};

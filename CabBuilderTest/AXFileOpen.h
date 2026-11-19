#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "SetupAPI.h"
#pragma comment(lib, "SetupAPI.lib")

// CAXFileOpen dialog
struct S_fileInfo
{
    CString s_filePath;
	DWORD s_fileSize;
};

class CAXFileOpen : public CDialog
{
	DECLARE_DYNAMIC(CAXFileOpen)

public:
	CAXFileOpen(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAXFileOpen();

	SP_CLASSIMAGELIST_DATA m_ImageListData;
	CImageList* m_imageList;
	CString m_currentFolder;
	//CImageList m_imgList;
	//HIMAGELIST m_himageListTemp;
	CString m_hotList[1000];
	int m_hotDirCount;

	S_fileInfo * m_fileInfo;
	int m_fileInfoCount;

	SHFILEINFO m_globalHanldeImgssi;
	//CString ** m_glbuffer;
	//int * m_gBufferCount;
// Dialog Data
	enum { IDD = IDD_DIALOG_AxFileOpen };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_folderList;
	int GetFileIconIndex( CString strFileName , BOOL bSmallIcon );
	int GetDirIconIndex(BOOL bSmallIcon );
	HIMAGELIST GetSystemImageListHandle( BOOL bSmallIcon ,SHFILEINFO * ssfiRet);
	CString GetDeskTopFolder();
	CString ConvertLPWSTR(LPWSTR & string);
	void AddComboItem(int index, 
							   int image, 
                               CString str,
							   int indentCount,
							   CComboBoxEx * comboBox);


	HICON GetFileIconHandle(CString strFileName, BOOL bSmallIcon);
	void InListFolder(CString rootFolder, int sel);
	void InListDirHotLinks(CString currentFolder,int sel);
	CString ConvertLongToString(long number);
	afx_msg void OnBnClickedOk();
	//void InListDirHotLinks(int sel);
	afx_msg void OnNMDblclkListfolderview(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	CComboBoxEx m_comboHotLinks;
	afx_msg void OnBnClickedButtonUpalevel();
	int GetFileCount(CString path, int & currentCount, DWORD & byteSize);
	CButton m_upLevelBtnHandle;
	int FillFileInfo(CString path, int & currentCount);
	afx_msg void OnCbnSelchangeComboHotlinks();
	afx_msg void OnBnClickedButtonupdirlevel();
};

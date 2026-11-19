#pragma once
#include "afxcmn.h"
#include "CABFunctionLib.h"
#include "CEXScriptClass.h"
#include "afxwin.h"

// CLoadScriptsDlg dialog
//
// Modeless editor for a CEXScriptObjList (.aut startup script).
// Two modes:
//   m_fromFile == TRUE  : edits GameFiles\autoexec.aut (or whatever file was
//                         loaded with "Load From File").  The dialog OWNS the list.
//   m_fromFile == FALSE : edits the engine's scene script list through
//                         m_scriptListMemoryRef.  The engine owns the list; the
//                         dialog never deletes it and always follows the live
//                         pointer (the engine may free/re-create it while this
//                         dialog is open).

class CLoadScriptsDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoadScriptsDlg)

public:
	CLoadScriptsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoadScriptsDlg();


	CEXScriptObjList	*  m_scriptListRef;        // dialog-owned list (file mode)
	CEXScriptObjList	** m_scriptListMemoryRef;  // engine's list pointer (scene mode)
	BOOL				   m_fromFile;//TRUE runs the autoexec bat false uses pass in vars
	CABFunctionLib		   m_cabLib;
	BOOL                   m_sceneBased;
	CString				   m_filePath;             // file the list was loaded from / saves to

	CEdit				*  m_currentEdit;


	CEXScriptObjList * List();                     // never NULL
	void InListScripts(int sel);
	void InListEvents(int sel);
	CString GetAttribNameByType(int attribute);
// Dialog Data
	enum { IDD = IDD_DIALOG_LoadScripts };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();

	CEXScriptObjList * LoadListFromFile(const CString & path);
	BOOL SaveListToFile(const CString & path);
	void ReplaceListContents(CEXScriptObjList * loaded);
	void UpdateTitle();
	CString DefaultScriptPath();

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl CScriptsList;
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedListscriptslist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRdblclkListscriptslist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListscriptslist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonAddscriptevent();
	afx_msg void OnBnClickedButtonDeletescriptevent();
	afx_msg void OnBnClickedButtonReplacescriptevent();
	CComboBox m_typeList;
	int m_miscInt;
	CString m_miscString;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
public:
	afx_msg void OnBnClickedBtnLoadfromfile();
public:
	afx_msg void OnBnClickedBtnSavetofile();
};

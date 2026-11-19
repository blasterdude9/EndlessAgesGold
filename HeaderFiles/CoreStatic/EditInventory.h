#ifndef EDITINVENTORY_H
#define EDITINVENTORY_H

#include "resource.h"
#include "GlobalStatusVariables.h"
#include "MaterialDlg.h"
#include "EditScript.h"

class CEditInventory: public CDialog
{
	DECLARE_DYNAMIC(CEditInventory)
	DECLARE_MESSAGE_MAP()

public:
	// Properties
	enum { IDD = IDD_DIALOG_MenuSysEditInventory };

	CMenuInventory* InventoryMenu() { return this->m_pInventory; }
	void InventoryMenu(CMenuInventory *pInventory) { this->m_pInventory = pInventory; }
	GlobalStatusVariables* GlobalStatusVariable() { return this->m_gStatus; }
	void GlobalStatusVariable(GlobalStatusVariables *pStatus) { this->m_gStatus = pStatus; }
	int VisualID() { return this->m_freeVisualID; }
	void VisualID(int iVisualID) { this->m_freeVisualID = iVisualID; }

	// Member Functions
	CEditInventory(CWnd* pParent = NULL);
	void UpdateTextureCoords();

	// Input Callback Functions
	afx_msg void OnButtonGetFreeID();
	afx_msg void OnChangeVisualCoords();
	afx_msg void OnChangeTextureCoords();
	afx_msg void OnButtonEditTexture();

	// Scripts
	afx_msg void OnButtonEditClickScript();
	afx_msg void OnButtonEditMouseOverScript();
	afx_msg void OnButtonEditMouseOutScript();
	afx_msg void OnButtonEditDropScript();
	afx_msg void OnButtonEditUpdateScript();
	afx_msg void OnButtonEditRightClickScript();

	// Other Control Events
	afx_msg void OnButtonNext();
	afx_msg void OnBnClickedButtonClearInventoryGrid();
	afx_msg void OnBnClickedButtonCreateInventoryGrid();
	afx_msg void OnDeltaposSpinInventoryNumRows(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinInventoryNumColumns(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinInventoryRowSpacing(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinInventoryColumnSpacing(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnChangeInventoryNumRows();
	afx_msg void OnChangeInventoryNumColumns();
	afx_msg void OnChangeInventoryRowSpacing();
	afx_msg void OnChangeInventoryColumnSpacing();

protected:
	virtual BOOL OnInitDialog();

private:
	static const int		SPIN_MIN_RANGE = 0,
							SPIN_MAX_RANGE = 100;
	bool					m_initialized;
	int						m_freeVisualID;
	CMenuInventory			*m_pInventory;
	GlobalStatusVariables	*m_gStatus;
public:
	afx_msg void OnBnClickedButtonEditPrototypeClickScript();
	afx_msg void OnBnClickedButtonEditPrototypeMouseOverScript();
	afx_msg void OnBnClickedButtonEditPrototypeMouseOutScript();
	afx_msg void OnBnClickedButtonEditPrototypeDropScript();
	afx_msg void OnBnClickedButtonEditPrototypeUpdateScript();
};

#endif
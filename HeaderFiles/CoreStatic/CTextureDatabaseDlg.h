#ifndef CTextureDatabaseDlg_H
#define CTextureDatabaseDlg_H

#define WINVER 0x0502

#include <afx.h>
#include "resource.h"
#include "direct.h"
#include "globalstatusvariables.h"
#include "AddTextureDlg.h"
#include "cTextureDBclass.h"

///////////////////////////////////////////////////////////////////////////////
// Function:	PROTOTYPES
///////////////////////////////////////////////////////////////////////////////

void InListTextureDB(int sel, GlobalStatusVariables* pStatus, CDialog *pTextureDatabaseDlg = 0);
void DestroyAllTexturesFromDatabase(GlobalStatusVariables* pStatus, CDialog *pTextureDatabaseDlg);
BOOL SaveTextureBank(CString fileName,int compressed,int maxTextureSize, GlobalStatusVariables *pStatus = 0);

///////////////////////////////////////////////////////////////////////////////
// Function:	CTextureDatabaseDlg
// Description:	The texture database dialog control.
///////////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CTextureDatabaseDlg : public CDialog
{
	friend void InListTextureDB(int sel) ;
	friend void DestroyAllTexturesFromDatabase(GlobalStatusVariables* pStatus, CDialog *pTextureDatabaseDlg);
	friend BOOL SaveTextureBank(CString fileName, int compressed, int maxTextureSize, GlobalStatusVariables *pStatus);

public:
	enum { IDD = IDD_DIALOG_TEXTUREDATA };

	CTextureDatabaseDlg(CWnd *pParent = 0);
	~CTextureDatabaseDlg();

	inline void		Status(void *pValue) { this->m_pStatus = static_cast<GlobalStatusVariables*>(pValue); }

	BOOL	OnInitDialog();
	void	OnOK();
	void TextureRebuildWithSearch(int mipLevels);
	DECLARE_DYNAMIC(CTextureDatabaseDlg)

	afx_msg void OnBtnTxtdbAdd();
	afx_msg void OnBtnTxtdbDelete();
	afx_msg void OnBtnTxtdbEdit();
	afx_msg void OnBtnTxtdbReinitAll();
	afx_msg void OnBTNTXTDBBrowse();
	afx_msg void OnBTNTXTDBTextureReport();
	afx_msg void OnSelchangeLISTTXTDBDatabase();
	afx_msg void OnBtnTxtdbAddUrl();
	afx_msg void OnBTNTXSaveTextureBank();
	afx_msg void OnBTNTXLoadTextureBank();
	afx_msg void OnBTNTXBSetMipLevels();
	afx_msg void OnBTNTXBRemoveDuplicates();
	afx_msg void OnBtnUpdateAllTextures();

protected:
	DECLARE_MESSAGE_MAP()
	void	DoDataExchange(CDataExchange* pDX);

private:
	GlobalStatusVariables *m_pStatus;
};

#endif
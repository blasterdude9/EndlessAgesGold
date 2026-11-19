//---------------------------------------------------------------------------------------------------------------//
//
//												New Menu System Section
//
//---------------------------------------------------------------------------------------------------------------//
void EngineXWin::OnRefreshListThemes()
{
	RefreshMenuThemesList(m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MenuThemes, LB_GETCURSEL));
}

void EngineXWin::OnButtonAddTheme()
{
}

void EngineXWin::OnButtonEditTheme()
{
}

void EngineXWin::OnButtonTestTheme()
{
}

void EngineXWin::OnButtonApplyTheme()
{
}

void EngineXWin::OnButtonExportTheme()
{
}

void EngineXWin::OnButtonImportTheme()
{
}

void EngineXWin::OnButtonDeleteTheme()
{
}

void EngineXWin::OnRefreshListMenus()
{
	RefreshMenuList(m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_Menus, LB_GETCURSEL));
}

void EngineXWin::OnButtonAddMenu() 
{
#ifndef _ClientOutput
	//Delete the old Add Menu dialog
	if(m_addMenuDlg)
		delete m_addMenuDlg;

	//Create a new menu
	CMenuWindow *menu	= new CMenuWindow();
	menu->m_name		= "Unnamed";
	menu->m_visualID	= m_gStatus->m_menuDatabase->GetFreeID(-1);

	//Create a new Add Menu dialog
	m_addMenuDlg				 = new CAddMenu();
	m_addMenuDlg->m_pMenu		 = menu;
	m_addMenuDlg->m_gStatus		 = m_gStatus;
	m_addMenuDlg->m_freeVisualID = menu->m_visualID;

	//Show the dialog
	m_addMenuDlg->Create(IDD_DIALOG_MenuSysAddMenu);

	//Add the menu to the list and show the menu
	m_gStatus->m_menuDatabase->AddHead(menu);
	RefreshMenuList(m_gStatus->m_menuDatabase->GetCount() - 1);
	m_gStatus->OpenMenu(menu->m_visualID, false);
#endif
}

void EngineXWin::OnButtonEditMenu()
{
#ifndef _ClientOutput
	//If they have a menu selected
	int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_Menus, LB_GETCURSEL, 0, 0);
	if(menuIndex != -1)
	{
		//Get a pointer to the menu
		POSITION posMenu = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
		if(posMenu)
		{
			//Get a pointer to the selected menu
			CMenuWindow *menu = (CMenuWindow*)m_gStatus->m_menuDatabase->GetAt(posMenu);

			//Delete the old Edit Menu dialog
			if(m_addMenuDlg)
				delete m_addMenuDlg;

			//Create a new Edit Menu dialog
			m_addMenuDlg				 = new CAddMenu();
			m_addMenuDlg->m_pMenu		 = menu;
			m_addMenuDlg->m_gStatus		 = m_gStatus;
			m_addMenuDlg->m_freeVisualID = m_gStatus->m_menuDatabase->GetFreeID(menu->m_visualID);

			//Show the dialog
			m_addMenuDlg->Create(IDD_DIALOG_MenuSysAddMenu);

			//Show the menu
			m_gStatus->OpenMenu(menu->m_visualID, false);
		}
	}
	else
		MessageBox("You must select a Menu to Edit");
#endif
}

void EngineXWin::OnButtonMoveMenuUpList()
{
#ifndef _ClientOutput
	// GET menu selection
	CWnd *pMenuWnd = m_menuDBDlgBar.GetDlgItem(IDC_LIST_Menus);
	CListBox *pMenuList = (CListBox*)(pMenuWnd);
	if(!pMenuList) return;

	// GET the current selection index
	int iMenuIndex = pMenuList->GetCurSel();
	if(iMenuIndex == -1) return;
	
	// GET a pointer to the menu position
	POSITION pos = m_gStatus->m_menuDatabase->FindIndex(iMenuIndex);
	if(!pos) return;
	
	// GET the selected menu
	CMenuWindow *pMenu = dynamic_cast<CMenuWindow*>(m_gStatus->m_menuDatabase->GetAt(pos));
	if(!pMenu) return;

	// MOVE the selected menu BACK one element in the list
	POSITION posPrevious = pos;
	m_gStatus->m_menuDatabase->GetPrev(posPrevious);
	m_gStatus->m_menuDatabase->InsertBefore(posPrevious, pMenu);

	// REMOVE the element at the OLD position
	m_gStatus->m_menuDatabase->RemoveAt(pos);
	
	// SET the selection to the newly added menu
	pMenuList->SetCurSel(iMenuIndex-1);
	
	// REFRESH the menus list
	this->RefreshMenuList(iMenuIndex-1);
#endif
}

void EngineXWin::OnButtonMoveMenuDownList()
{
#ifndef _ClientOutput
	// GET menu selection
	CWnd *pMenuWnd = m_menuDBDlgBar.GetDlgItem(IDC_LIST_Menus);
	CListBox *pMenuList = (CListBox*)(pMenuWnd);
	if(!pMenuList) return;

	// GET the current selection index
	int iMenuIndex = pMenuList->GetCurSel();
	if(iMenuIndex == -1) return;
	
	// GET a pointer to the menu position
	POSITION pos = m_gStatus->m_menuDatabase->FindIndex(iMenuIndex);
	if(!pos) return;
	
	// GET the selected menu
	CMenuWindow *pMenu = dynamic_cast<CMenuWindow*>(m_gStatus->m_menuDatabase->GetAt(pos));
	if(!pMenu) return;

	// MOVE the selected menu FORWARD one element in the list
	POSITION posNext = pos;
	m_gStatus->m_menuDatabase->GetNext(posNext);
	m_gStatus->m_menuDatabase->InsertAfter(posNext, pMenu);

	// REMOVE the element at the OLD position
	m_gStatus->m_menuDatabase->RemoveAt(pos);
	
	// SET the selection to the newly added menu
	pMenuList->SetCurSel(iMenuIndex+1);
	
	// REFRESH the menus list
	this->RefreshMenuList(iMenuIndex+1);
#endif
}

void EngineXWin::OnButtonTestMenu()
{
	//If they have a menu selected
	int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_Menus, LB_GETCURSEL, 0, 0);
	if(menuIndex != -1)
	{
		//Get a pointer to the menu
		POSITION posMenu = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
		if(posMenu)
			m_gStatus->OpenMenu(((CMenuWindow*)m_gStatus->m_menuDatabase->GetAt(posMenu))->m_visualID, false);
	}
}

void EngineXWin::OnButtonCloseAllMenus()
{
	//Delete all temp textures
	for(POSITION posCurMenu = m_gStatus->m_menuDatabase->GetHeadPosition(); posCurMenu != NULL;)
	{
		CMenuWindow *curMenu = (CMenuWindow*)m_gStatus->m_menuDatabase->GetNext(posCurMenu);
		m_gStatus->m_textureDataBase->RemoveAllTempTexturesByChannel(curMenu->m_visualID + 5);
	}

	//Clear the runtime menu list
	m_gStatus->m_runtimeMenuDatabase->CloseAll(&m_gStatus->m_showMenuCursor, false);
}

void EngineXWin::OnButtonExportMenu()
{
}

void EngineXWin::OnButtonImportMenu()
{
}

void 
EngineXWin::OnButtonCopyMenuWindow()
{
#ifndef _ClientOutput
	// GET menu selection
	CWnd *pMenuWnd = m_menuDBDlgBar.GetDlgItem(IDC_LIST_Menus);
	CListBox *pMenuList = (CListBox*)(pMenuWnd);
	if(!pMenuList) return;

	// GET the current selection index
	int iMenuIndex = pMenuList->GetCurSel();
	if(iMenuIndex == -1) return;
	
	// GET a pointer to the menu position
	POSITION pos = m_gStatus->m_menuDatabase->FindIndex(iMenuIndex);
	if(!pos) return;
	
	// GET the selected menu
	CMenuWindow *pMenu = dynamic_cast<CMenuWindow*>(m_gStatus->m_menuDatabase->GetAt(pos));
	if(!pMenu) return;

	// SET the clipboard pointer to the selected menu
	this->m_pClipboardMenuWindow = pMenu;
#endif	
}

void 
EngineXWin::OnButtonPasteMenuWindow()
{
#ifndef _ClientOutput
	// ERROR checking: Do we have a valid reference to a menu window in the clipboard pointer?
	if (!this->m_pClipboardMenuWindow)
	{
		MessageBox("There is no valid reference to a menu window on the clipboard!");
		return;
	}

	// GET menu selections
	CWnd *pMenuWnd = m_menuDBDlgBar.GetDlgItem(IDC_LIST_Menus);
	CListBox *pMenuList = (CListBox*)(pMenuWnd);
	if(!pMenuList) return;
	int iMenuIndex = pMenuList->GetCurSel();
	if(iMenuIndex == -1) return;
	
	// GET a pointer to the menu position
	POSITION pos = m_gStatus->m_menuDatabase->FindIndex(iMenuIndex);
	if(!pos) return;
	
	// GET the selected menu
	CMenuWindow *pMenu = dynamic_cast<CMenuWindow*>(m_gStatus->m_menuDatabase->GetAt(pos));
	if(!pMenu) return;

	// ADD the clipboard menu window to the global menu database
	CMenuVisual *pNewWindow = NULL;
	this->m_pClipboardMenuWindow->Clone(&pNewWindow, m_gStatus->m_3dDevice);
	m_gStatus->m_menuDatabase->AddHead(pNewWindow);

	// SET the selection to the newly added menu
	pMenuList->SetCurSel(0);
	
	// REFRESH the menus list
	this->RefreshMenuList(0);
#endif	
}

void 
EngineXWin::OnButtonCopyMenuVisual()
{
#ifndef _ClientOutput
	// GET menu selections
	CWnd *pMenuWnd = m_menuDBDlgBar.GetDlgItem(IDC_LIST_Menus);
	CWnd *pVisualWnd = m_menuDBDlgBar.GetDlgItem(IDC_LIST_MenuVisuals);

	CListBox *pMenuList = (CListBox*)(pMenuWnd);
	CListBox *pVisualList = (CListBox*)(pVisualWnd);
	if(!pMenuList || !pVisualList) return;

	int iMenuIndex = pMenuList->GetCurSel();
	int iVisualIndex = pVisualList->GetCurSel();
	if((iMenuIndex == -1) || (iVisualIndex == -1)) return;
	
	// GET a pointer to the menu position
	POSITION pos = m_gStatus->m_menuDatabase->FindIndex(iMenuIndex);
	if(!pos) return;
	
	// GET the selected menu
	CMenuWindow *pMenu = dynamic_cast<CMenuWindow*>(m_gStatus->m_menuDatabase->GetAt(pos));
	if(!pMenu) return;
	if(!pMenu->m_childList)
		pMenu->m_childList = new CMenuVisualList();

	// GET the selected visual position
	pos = pMenu->m_childList->FindIndex(iVisualIndex);
	if(!pos) return;

	// GET the selected menu visual
	CMenuVisual *pVisual = dynamic_cast<CMenuVisual*>(pMenu->m_childList->GetAt(pos));

	// SET the clipboard pointer to the selected visual
	this->m_pClipboardMenuVisual = pVisual;
#endif	
}

void 
EngineXWin::OnButtonPasteMenuVisual()
{
#ifndef _ClientOutput
	// ERROR checking: Do we have a valid referenceto a visual in the clipboard pointer?
	if (!this->m_pClipboardMenuVisual)
	{
		MessageBox("There is no valid reference to a menu visual on the clipboard!");
		return;
	}

	// GET menu selections
	CWnd *pMenuWnd = m_menuDBDlgBar.GetDlgItem(IDC_LIST_Menus);
	CWnd *pVisualWnd = m_menuDBDlgBar.GetDlgItem(IDC_LIST_MenuVisuals);

	CListBox *pMenuList = (CListBox*)(pMenuWnd);
	CListBox *pVisualList = (CListBox*)(pVisualWnd);
	if(!pMenuList || !pVisualList) return;

	int iMenuIndex = pMenuList->GetCurSel();
	int iVisualIndex = pVisualList->GetCurSel();
	if((iMenuIndex == -1) || (iVisualIndex == -1)) return;
	
	// GET a pointer to the menu position
	POSITION pos = m_gStatus->m_menuDatabase->FindIndex(iMenuIndex);
	if(!pos) return;
	
	// GET the selected menu
	CMenuWindow *pMenu = dynamic_cast<CMenuWindow*>(m_gStatus->m_menuDatabase->GetAt(pos));
	if(!pMenu) return;
	if(!pMenu->m_childList)
		pMenu->m_childList = new CMenuVisualList();

	// ADD the clipboard visual to the selected menu window's child list
	CMenuVisual *pNewVisual = NULL;
	this->m_pClipboardMenuVisual->Clone(&pNewVisual, m_gStatus->m_3dDevice);
	pMenu->m_childList->AddHead(pNewVisual);

	// REINIT all menu textures
	pMenu->ReinitMenuTextures(m_gStatus->m_textureDataBase,
		m_gStatus->m_menuFontList,
		m_gStatus->m_3dDevice,
		m_gStatus->m_draggingItem);

	// SET the selection to the newly added menu
	pVisualList->SetCurSel(0);
	
	// CLEAR and REFRESH the visuals list
	RefreshMenuVisualsList(0);
#endif	
}

void 
EngineXWin::OnButtonCloneMenu()
{
	// If a valid menu is selected
	CListBox *pMenuList = (CListBox*)m_menuDBDlgBar.GetDlgItem(IDC_LIST_Menus);
	if(!pMenuList) return;

	int iMenuIndex = pMenuList->GetCurSel(); 
	if(iMenuIndex != -1)
	{
		// Get a pointer to the menu
		POSITION pos = m_gStatus->m_menuDatabase->FindIndex(iMenuIndex);
		if(pos)
		{
			// Clone the selected menu
			CMenuWindow *pClone = NULL;
			CMenuWindow *pMenu = dynamic_cast<CMenuWindow*>(m_gStatus->m_menuDatabase->GetAt(pos));
			pMenu->Clone(((CMenuVisual**)&pClone), m_gStatus->m_3dDevice);

			// Get a new visual ID for the cloned menu
			pClone->m_visualID = m_gStatus->m_menuDatabase->GetFreeID(-1);

			// Add cloned menu as a new item in the menu list and to the GlobalStatusVariable's menu database
			pClone->m_name += "_Clone";
			pMenuList->AddString(pClone->m_name);
			m_gStatus->m_menuDatabase->AddHead(pClone);
			
			// Set the selection to the newly added menu
			pMenuList->SetCurSel(0/*pMenuList->GetCount()*/);
		}
	}

	this->RefreshMenuList(iMenuIndex);
}

void 
EngineXWin::OnButtonCloneVisual()
{
	// If a valid menu is selected
	CWnd *pMenuWnd = m_menuDBDlgBar.GetDlgItem(IDC_LIST_Menus);
	CWnd *pVisualWnd = m_menuDBDlgBar.GetDlgItem(IDC_LIST_MenuVisuals);

	CListBox *pMenuList = (CListBox*)(pMenuWnd);
	CListBox *pVisualList = (CListBox*)(pVisualWnd);
	if(!pMenuList || !pVisualList) return;

	int iMenuIndex = pMenuList->GetCurSel();
	int iVisualIndex = pVisualList->GetCurSel();
	if((iMenuIndex == -1) || (iVisualIndex == -1)) return;
	
	// Get a pointer to the menu
	POSITION pos = m_gStatus->m_menuDatabase->FindIndex(iMenuIndex);
	if(!pos) return;
	
	// Get the selected menu
	CMenuWindow *pMenu = dynamic_cast<CMenuWindow*>(m_gStatus->m_menuDatabase->GetAt(pos));
	if(!pMenu) return;
	if(!pMenu->m_childList)
		pMenu->m_childList = new CMenuVisualList();

	pos = pMenu->m_childList->FindIndex(iVisualIndex);
	if(!pos) return;

	// Clone the selected menu
	CMenuVisual *pVisual = dynamic_cast<CMenuVisual*>(pMenu->m_childList->GetAt(pos));
	CMenuVisual *pClone = NULL;
	pVisual->Clone(((CMenuVisual**)&pClone), m_gStatus->m_3dDevice);

	// Get a new visual ID for the cloned menu
	int iNewVisualID = m_gStatus->m_menuDatabase->GetFreeID(-1);
	pClone->m_visualID = iNewVisualID;

	// Add cloned menu as a new item in the menu list and to the GlobalStatusVariable's menu database
	pClone->m_name += "_Clone";
	pVisualList->AddString(pClone->m_name);
	pMenu->m_childList->AddHead(pClone);
	
	// Set the selection to the newly added menu
	pVisualList->SetCurSel(0/*pVisualList->GetCount()*/);
	
	//Clear and refill the visuals list
	RefreshMenuVisualsList(pVisualList->GetCurSel() + 1);
}

void EngineXWin::OnButtonDeleteMenu() 
{
	//If they have a menu selected
	int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_Menus, LB_GETCURSEL, 0, 0);
	if(menuIndex != -1)
	{
		if(AfxMessageBox("Are you sure you want to delete selected menu?", MB_OKCANCEL, 0)!=IDOK)
			return;

		//Get a pointer to the menu
		POSITION posMenu = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
		if(posMenu)
		{
			CMenuWindow *menu = (CMenuWindow*)m_gStatus->m_menuDatabase->GetAt(posMenu);

			//Make sure there aren't any pointers to it in the runtime database
			POSITION posLastMenu = NULL;
			for(POSITION posCurMenu = m_gStatus->m_runtimeMenuDatabase->GetHeadPosition(); (posLastMenu = posCurMenu) != NULL;)
			{
				CMenuWindow *curMenu = (CMenuWindow*)m_gStatus->m_runtimeMenuDatabase->GetNext(posCurMenu);
				if(curMenu->m_visualID == menu->m_visualID)
					m_gStatus->m_runtimeMenuDatabase->RemoveAt(posLastMenu);
			}

			//Delete the menu
			delete menu;

			//Remove it from the database and rebuild the list
			m_gStatus->m_menuDatabase->RemoveAt(posMenu);
			RefreshMenuList(menuIndex);
		}
	}
}

void EngineXWin::OnRefreshListMenuVisuals()
{
	RefreshMenuVisualsList(m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MenuVisuals, LB_GETCURSEL));
}

void EngineXWin::OnButtonAddVisual()
{
#ifndef _ClientOutput
	//If they have a menu selected
	int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_Menus, LB_GETCURSEL, 0, 0);
	if(menuIndex != -1)
	{
		//Get a pointer to the menu
		POSITION posMenu = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
		if(posMenu)
		{
			CMenuWindow *menu = (CMenuWindow*)m_gStatus->m_menuDatabase->GetAt(posMenu);

			//Create an Add Visual dialog
			CMenuAddUnitDlg addUnitDlg;
			addUnitDlg.m_freeVisualID = m_gStatus->m_menuDatabase->GetFreeID(-1);

			if(addUnitDlg.DoModal() == IDOK)
			{
				if(!menu->m_childList)
					menu->m_childList = new CMenuVisualList();

				//Create the visual based on the selected type
				CMenuVisual *newVisual = NULL;
				switch(addUnitDlg.m_visualType)
				{
					case MENU_VISTYPE_COMBOBOX:
					{
						//Make sure we have a font first
						if(m_gStatus->m_menuFontList->IsEmpty())
						{
							MessageBox("You must create a font first.");
							return;
						}

						//Create a new visual and copy across its info
						CMenuComboBox *comboBox	= new CMenuComboBox();
						newVisual				= (CMenuVisual*)comboBox;
						newVisual->m_name		= addUnitDlg.m_name;
						newVisual->m_visualID	= addUnitDlg.m_visualID;

						//Delete the old dialog
						if(m_editComboBoxDlg)
							delete m_editComboBoxDlg;

						//Make sure we have all textures loaded for all fonts
						LoadFontTextures();

						//Create default colors for the text box
						comboBox->m_textBox.m_numColors = 1;
						comboBox->m_textBox.m_red	= new float[1];		comboBox->m_textBox.m_red[0]	= 1.0f;
						comboBox->m_textBox.m_green = new float[1];		comboBox->m_textBox.m_green[0]	= 1.0f;
						comboBox->m_textBox.m_blue	= new float[1];		comboBox->m_textBox.m_blue[0]	= 1.0f;
						comboBox->m_textBox.m_alpha = new float[1];		comboBox->m_textBox.m_alpha[0]	= 1.0f;

						//Create a new Dialog
						m_editComboBoxDlg					= new CEditComboBox();
						m_editComboBoxDlg->m_pComboBox		= comboBox;
						m_editComboBoxDlg->m_gStatus		= m_gStatus;
						m_editComboBoxDlg->m_freeVisualID	= m_gStatus->m_menuDatabase->GetFreeID(newVisual->m_visualID);
						m_editComboBoxDlg->m_freezeInput	= &m_gStatus->m_runtimeMenuDatabase->m_freezeInput;

						m_editComboBoxDlg->Create(IDD_DIALOG_MenuSysEditComboBox);
						break;
					}
					case MENU_VISTYPE_LISTBOX:
					{
						//Make sure we have a font first
						if(m_gStatus->m_menuFontList->IsEmpty())
						{
							MessageBox("You must create a font first.");
							return;
						}

						//Create a new visual and copy across its info
						newVisual				= new CMenuListBox();
						newVisual->m_name		= addUnitDlg.m_name;
						newVisual->m_visualID	= addUnitDlg.m_visualID;

						//Delete the old dialog
						if(m_editListBoxDlg)
							delete m_editListBoxDlg;

						//Make sure we have all textures loaded for all fonts
						LoadFontTextures();

						//Create a new Dialog
						m_editListBoxDlg				 = new CEditListBox();
						m_editListBoxDlg->m_pListBox	 = (CMenuListBox*)newVisual;
						m_editListBoxDlg->m_gStatus		 = m_gStatus;
						m_editListBoxDlg->m_freeVisualID = m_gStatus->m_menuDatabase->GetFreeID(newVisual->m_visualID);
						m_editListBoxDlg->m_freezeInput	 = &m_gStatus->m_runtimeMenuDatabase->m_freezeInput;

						m_editListBoxDlg->Create(IDD_DIALOG_MenuSysEditListBox);
						break;
					}
					case MENU_VISTYPE_TEXTBOX:
					{
						//Make sure we have a font first
						if(m_gStatus->m_menuFontList->IsEmpty())
						{
							MessageBox("You must create a font first.");
							return;
						}

						//Create a new visual and copy across its info
						CMenuTextBox *textBox	= new CMenuTextBox();
						newVisual				= (CMenuVisual*)textBox;
						newVisual->m_name		= addUnitDlg.m_name;
						newVisual->m_visualID	= addUnitDlg.m_visualID;

						//Delete the old dialog
						if(m_editTextBoxDlg)
							delete m_editTextBoxDlg;

						//Make sure we have all textures loaded for all fonts
						LoadFontTextures();

						//Create default colors for the text box
						textBox->m_numColors = 1;
						textBox->m_red	 = new float[1];	textBox->m_red[0]	= 1.0f;
						textBox->m_green = new float[1];	textBox->m_green[0]	= 1.0f;
						textBox->m_blue	 = new float[1];	textBox->m_blue[0]	= 1.0f;
						textBox->m_alpha = new float[1];	textBox->m_alpha[0]	= 1.0f;

						//Create a new Dialog
						m_editTextBoxDlg				 = new CEditTextBox();
						m_editTextBoxDlg->m_pTextBox	 = textBox;
						m_editTextBoxDlg->m_gStatus		 = m_gStatus;
						m_editTextBoxDlg->m_freeVisualID = m_gStatus->m_menuDatabase->GetFreeID(newVisual->m_visualID);
						m_editTextBoxDlg->m_freezeInput	 = &m_gStatus->m_runtimeMenuDatabase->m_freezeInput;

						m_editTextBoxDlg->Create(IDD_DIALOG_MenuSysEditTextBox);
						break;
					}
					case MENU_VISTYPE_TEXT:
					{
						//Make sure we have a font first
						if(m_gStatus->m_menuFontList->IsEmpty())
						{
							MessageBox("You must create a font first.");
							return;
						}

						//Create a new visual and copy across its info
						newVisual				= new CMenuText();
						newVisual->m_name		= addUnitDlg.m_name;
						newVisual->m_visualID	= addUnitDlg.m_visualID;

						//Delete the old dialog
						if(m_editTextDlg)
							delete m_editTextDlg;

						//Make sure we have all textures loaded for all fonts
						LoadFontTextures();

						//Create a new Dialog
						m_editTextDlg					= new CEditText();
						m_editTextDlg->m_pText			= (CMenuText*)newVisual;
						m_editTextDlg->m_gStatus		= m_gStatus;
						m_editTextDlg->m_freeVisualID	= m_gStatus->m_menuDatabase->GetFreeID(newVisual->m_visualID);

						m_editTextDlg->Create(IDD_DIALOG_MenuSysEditText);
						break;
					}
					case MENU_VISTYPE_SCROLLBAR:
					{
						//Create a new visual and copy across its info
						newVisual				= new CMenuScrollBar();
						newVisual->m_name		= addUnitDlg.m_name;
						newVisual->m_visualID	= addUnitDlg.m_visualID;

						//Delete the old dialog
						if(m_editScrollBarDlg)
							delete m_editScrollBarDlg;

						//Create a new Dialog
						m_editScrollBarDlg					= new CEditScrollBar();
						m_editScrollBarDlg->m_pScrollBar	= (CMenuScrollBar*)newVisual;
						m_editScrollBarDlg->m_gStatus		= m_gStatus;
						m_editScrollBarDlg->m_freeVisualID	= m_gStatus->m_menuDatabase->GetFreeID(newVisual->m_visualID);
						m_editScrollBarDlg->m_freezeInput	= &m_gStatus->m_runtimeMenuDatabase->m_freezeInput;

						m_editScrollBarDlg->Create(IDD_DIALOG_MenuSysEditScrollBar);
						break;
					}
					case MENU_VISTYPE_BUTTON:
					{
						//Create a new visual and copy across its info
						newVisual				= new CMenuButton();
						newVisual->m_name		= addUnitDlg.m_name;
						newVisual->m_visualID	= addUnitDlg.m_visualID;

						//Delete the old dialog
						if(m_editButtonDlg)
							delete m_editButtonDlg;

						//Create a new Dialog
						m_editButtonDlg					= new CEditButton();
						m_editButtonDlg->m_pButton		= (CMenuButton*)newVisual;
						m_editButtonDlg->m_gStatus		= m_gStatus;
						m_editButtonDlg->m_freeVisualID	= m_gStatus->m_menuDatabase->GetFreeID(newVisual->m_visualID);
						m_editButtonDlg->m_freezeInput	= &m_gStatus->m_runtimeMenuDatabase->m_freezeInput;

						m_editButtonDlg->Create(IDD_DIALOG_MenuSysEditButton);
						break;
					}
					case MENU_VISTYPE_TOGGLE:
					{
						//Create a new visual and copy across its info
						newVisual				= new CMenuToggle();
						newVisual->m_name		= addUnitDlg.m_name;
						newVisual->m_visualID	= addUnitDlg.m_visualID;

						//Delete the old dialog
						if(m_editToggleDlg)
							delete m_editToggleDlg;

						//Create a new Dialog
						m_editToggleDlg					= new CEditToggle();
						m_editToggleDlg->m_pToggle		= (CMenuToggle*)newVisual;
						m_editToggleDlg->m_gStatus		= m_gStatus;
						m_editToggleDlg->m_freeVisualID	= m_gStatus->m_menuDatabase->GetFreeID(newVisual->m_visualID);
						m_editToggleDlg->m_freezeInput	= &m_gStatus->m_runtimeMenuDatabase->m_freezeInput;

						m_editToggleDlg->Create(IDD_DIALOG_MenuSysEditToggle);
						break;
					}
					case MENU_VISTYPE_PROGRESSBAR:
					{
						//Create a new visual and copy across its info
						newVisual				= new CMenuProgressBar();
						newVisual->m_name		= addUnitDlg.m_name;
						newVisual->m_visualID	= addUnitDlg.m_visualID;

						//Delete the old dialog
						if(m_editProgBarDlg)
							delete m_editProgBarDlg;

						//Create a new Dialog
						m_editProgBarDlg					= new CEditProgBar();
						m_editProgBarDlg->m_pProgressBar	= (CMenuProgressBar*)newVisual;
						m_editProgBarDlg->m_gStatus			= m_gStatus;
						m_editProgBarDlg->m_freeVisualID	= m_gStatus->m_menuDatabase->GetFreeID(newVisual->m_visualID);

						m_editProgBarDlg->Create(IDD_DIALOG_MenuSysEditProgBar);
						break;
					}
					case MENU_VISTYPE_IMAGE:
					{
						//Create a new visual and copy across its info
						newVisual				= new CMenuImage();
						newVisual->m_name		= addUnitDlg.m_name;
						newVisual->m_visualID	= addUnitDlg.m_visualID;

						//Delete the old dialog
						if(m_editImageDlg)
							delete m_editImageDlg;

						//Create a new Dialog
						m_editImageDlg					= new CEditImage();
						m_editImageDlg->m_pImage		= (CMenuImage*)newVisual;
						m_editImageDlg->m_gStatus		= m_gStatus;
						m_editImageDlg->m_freeVisualID	= m_gStatus->m_menuDatabase->GetFreeID(newVisual->m_visualID);

						m_editImageDlg->Create(IDD_DIALOG_MenuSysEditImage);
						break;
					}
					case MENU_VISTYPE_ENTITY:
					{
						//Create a new visual and copy across its info
						newVisual				= new CMenuEntity();
						newVisual->m_name		= addUnitDlg.m_name;
						newVisual->m_visualID	= addUnitDlg.m_visualID;

						//Delete the old dialog
						if(m_editEntityDlg)
							delete m_editEntityDlg;

						//Create a new Dialog
						m_editEntityDlg					= new CEditEntity();
						m_editEntityDlg->m_pEntity		= (CMenuEntity*)newVisual;
						m_editEntityDlg->m_gStatus		= m_gStatus;
						m_editEntityDlg->m_freeVisualID	= m_gStatus->m_menuDatabase->GetFreeID(newVisual->m_visualID);

						m_editEntityDlg->Create(IDD_DIALOG_MenuSysEditEntity);
						break;
					}
					case MENU_VISTYPE_INVENTORY:
					{
						//Create a new visual and copy across its info
						newVisual				= new CMenuInventory();
						newVisual->m_name		= addUnitDlg.m_name;
						newVisual->m_visualID	= addUnitDlg.m_visualID;

						//Delete the old dialog
						SafeDelete(m_editInventoryDlg);

						//Create a new Dialog
						m_editInventoryDlg = new CEditInventory();
						m_editInventoryDlg->InventoryMenu(static_cast<CMenuInventory*>(newVisual));
						m_editInventoryDlg->GlobalStatusVariable(m_gStatus);
						m_editInventoryDlg->VisualID(m_gStatus->m_menuDatabase->GetFreeID(newVisual->m_visualID));

						m_editInventoryDlg->Create(IDD_DIALOG_MenuSysEditInventory);
						break;
					}
				}

				//Add the new child to the list and refresh it
				menu->m_childList->AddHead(newVisual);
				RefreshMenuVisualsList(menu->m_childList->GetCount() - 1);

				//Begin rendering the menu with the visual in it
				m_gStatus->OpenMenu(menu->m_visualID, false);
			}
		}
	}
	else
		MessageBox("You must select a menu to add the visual to");
#endif
}

void EngineXWin::OnButtonEditVisual()
{
#ifndef _ClientOutput
	//If they have a menu selected
	int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_Menus, LB_GETCURSEL, 0, 0);
	if(menuIndex != -1)
	{
		//If they have a visual selected
		int visualIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MenuVisuals, LB_GETCURSEL, 0, 0);
		if(visualIndex != -1)
		{
			//Get a pointer to the menu
			POSITION posMenu = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
			if(posMenu)
			{
				
				CMenuWindow *menu = (CMenuWindow*)m_gStatus->m_menuDatabase->GetAt(posMenu);

				//Get a pointer to the visual
				POSITION posVisual = menu->m_childList->FindIndex(visualIndex/*menu->m_childList->GetCount() - visualIndex - 1*/);
				if(posVisual)
				{
					CMenuVisual *visual = (CMenuVisual*)menu->m_childList->GetAt(posVisual);
					//Open the correct edit dialog based on the visual type
					switch(visual->GetType())
					{
					    default:
							m_gStatus->UI_Log("OnButtonEditVisual()-Cannot find visual Type:",m_gStatus->m_cabLib.ConvToString(visual->GetType()));
						break;
						case MENU_VISTYPE_COMBOBOX:
						{
							//Delete the old dialog
							if(m_editComboBoxDlg)
								delete m_editComboBoxDlg;

							//Make sure we have all textures loaded for all fonts
							LoadFontTextures();

							//Create a new Dialog
							m_editComboBoxDlg					= new CEditComboBox();
							m_editComboBoxDlg->m_pComboBox		= (CMenuComboBox*)visual;
							m_editComboBoxDlg->m_gStatus		= m_gStatus;
							m_editComboBoxDlg->m_freeVisualID	= m_gStatus->m_menuDatabase->GetFreeID(visual->m_visualID);
							m_editComboBoxDlg->m_freezeInput	= &m_gStatus->m_runtimeMenuDatabase->m_freezeInput;

							m_editComboBoxDlg->Create(IDD_DIALOG_MenuSysEditComboBox);
							break;
						}
						case MENU_VISTYPE_LISTBOX:
						{
							//Delete the old dialog
							if(m_editListBoxDlg)
								delete m_editListBoxDlg;

							//Make sure we have all textures loaded for all fonts
							LoadFontTextures();

							//Create a new Dialog
							m_editListBoxDlg				 = new CEditListBox();
							m_editListBoxDlg->m_pListBox	 = (CMenuListBox*)visual;
							m_editListBoxDlg->m_gStatus		 = m_gStatus;
							m_editListBoxDlg->m_freeVisualID = m_gStatus->m_menuDatabase->GetFreeID(visual->m_visualID);
							m_editListBoxDlg->m_freezeInput	 = &m_gStatus->m_runtimeMenuDatabase->m_freezeInput;

							m_editListBoxDlg->Create(IDD_DIALOG_MenuSysEditListBox);
							break;
						}
						case MENU_VISTYPE_TEXTBOX:
						{
							//Delete the old dialog
							if(m_editTextBoxDlg)
								delete m_editTextBoxDlg;

							//Make sure we have all textures loaded for all fonts
							LoadFontTextures();

							//Create a new Dialog
							m_editTextBoxDlg				 = new CEditTextBox();
							m_editTextBoxDlg->m_pTextBox	 = (CMenuTextBox*)visual;
							m_editTextBoxDlg->m_gStatus		 = m_gStatus;
							m_editTextBoxDlg->m_freeVisualID = m_gStatus->m_menuDatabase->GetFreeID(visual->m_visualID);
							m_editTextBoxDlg->m_freezeInput	 = &m_gStatus->m_runtimeMenuDatabase->m_freezeInput;

							m_editTextBoxDlg->Create(IDD_DIALOG_MenuSysEditTextBox);
							break;
						}
						case MENU_VISTYPE_TEXT:
						{
							//Delete the old dialog
							if(m_editTextDlg)
								delete m_editTextDlg;

							//Make sure we have all textures loaded for all fonts
							LoadFontTextures();

							//Create a new Dialog
							m_editTextDlg					= new CEditText();
							m_editTextDlg->m_pText			= (CMenuText*)visual;
							m_editTextDlg->m_gStatus		= m_gStatus;
							m_editTextDlg->m_freeVisualID	= m_gStatus->m_menuDatabase->GetFreeID(visual->m_visualID);

							m_editTextDlg->Create(IDD_DIALOG_MenuSysEditText);
							break;
						}
						case MENU_VISTYPE_SCROLLBAR:
						{
							//Delete the old dialog
							if(m_editScrollBarDlg)
								delete m_editScrollBarDlg;

							//Create a new Dialog
							m_editScrollBarDlg					= new CEditScrollBar();
							m_editScrollBarDlg->m_pScrollBar	= (CMenuScrollBar*)visual;
							m_editScrollBarDlg->m_gStatus		= m_gStatus;
							m_editScrollBarDlg->m_freeVisualID	= m_gStatus->m_menuDatabase->GetFreeID(visual->m_visualID);
							m_editScrollBarDlg->m_freezeInput	= &m_gStatus->m_runtimeMenuDatabase->m_freezeInput;

							m_editScrollBarDlg->Create(IDD_DIALOG_MenuSysEditScrollBar);
							break;
						}
						case MENU_VISTYPE_BUTTON:
						{
							//Delete the old dialog
							if(m_editButtonDlg)
								delete m_editButtonDlg;

							//Create a new Dialog
							m_editButtonDlg					= new CEditButton();
							m_editButtonDlg->m_pButton		= (CMenuButton*)visual;
							m_editButtonDlg->m_gStatus		= m_gStatus;
							m_editButtonDlg->m_freeVisualID	= m_gStatus->m_menuDatabase->GetFreeID(visual->m_visualID);
							m_editButtonDlg->m_freezeInput	= &m_gStatus->m_runtimeMenuDatabase->m_freezeInput;

							m_editButtonDlg->Create(IDD_DIALOG_MenuSysEditButton);
							break;
						}
						case MENU_VISTYPE_TOGGLE:
						{
							//Delete the old dialog
							if(m_editToggleDlg)
								delete m_editToggleDlg;

							//Create a new Dialog
							m_editToggleDlg					= new CEditToggle();
							m_editToggleDlg->m_pToggle		= (CMenuToggle*)visual;
							m_editToggleDlg->m_gStatus		= m_gStatus;
							m_editToggleDlg->m_freeVisualID	= m_gStatus->m_menuDatabase->GetFreeID(visual->m_visualID);
							m_editToggleDlg->m_freezeInput	= &m_gStatus->m_runtimeMenuDatabase->m_freezeInput;

							m_editToggleDlg->Create(IDD_DIALOG_MenuSysEditToggle);
							break;
						}
						case MENU_VISTYPE_PROGRESSBAR:
						{
							//Delete the old dialog
							if(m_editProgBarDlg)
								delete m_editProgBarDlg;

							//Create a new Dialog
							m_editProgBarDlg				 = new CEditProgBar();
							m_editProgBarDlg->m_pProgressBar = (CMenuProgressBar*)visual;
							m_editProgBarDlg->m_gStatus		 = m_gStatus;
							m_editProgBarDlg->m_freeVisualID = m_gStatus->m_menuDatabase->GetFreeID(visual->m_visualID);

							m_editProgBarDlg->Create(IDD_DIALOG_MenuSysEditProgBar);
							break;
						}
						case MENU_VISTYPE_IMAGE:
						{
							//Delete the old dialog
							if(m_editImageDlg)
								delete m_editImageDlg;

							//Create a new Dialog
							m_editImageDlg					= new CEditImage();
							m_editImageDlg->m_pImage		= (CMenuImage*)visual;
							m_editImageDlg->m_gStatus		= m_gStatus;
							m_editImageDlg->m_freeVisualID	= m_gStatus->m_menuDatabase->GetFreeID(visual->m_visualID);

							m_editImageDlg->Create(IDD_DIALOG_MenuSysEditImage);
							break;
						}
						case MENU_VISTYPE_ENTITY:
						{
							//Delete the old dialog
							if(m_editEntityDlg)
								delete m_editEntityDlg;

							//Create a new Dialog
							m_editEntityDlg					= new CEditEntity();
							m_editEntityDlg->m_pEntity		= (CMenuEntity*)visual;
							m_editEntityDlg->m_gStatus		= m_gStatus;
							m_editEntityDlg->m_freeVisualID	= m_gStatus->m_menuDatabase->GetFreeID(visual->m_visualID);

							m_editEntityDlg->Create(IDD_DIALOG_MenuSysEditEntity);
							break;
						}
						case MENU_VISTYPE_INVENTORY:
						{
							//Delete the old dialog
							SafeDelete(m_editInventoryDlg);

							//Create a new Dialog
							m_editInventoryDlg = new CEditInventory();
							m_editInventoryDlg->InventoryMenu(static_cast<CMenuInventory*>(visual));
							m_editInventoryDlg->GlobalStatusVariable(m_gStatus);
							m_editInventoryDlg->VisualID(m_gStatus->m_menuDatabase->GetFreeID(visual->m_visualID));

							m_editInventoryDlg->Create(IDD_DIALOG_MenuSysEditInventory);
							break;
						}
					}

					//Begin rendering the menu with the visual in it
					m_gStatus->OpenMenu(menu->m_visualID, false);
				}
			}
		}
		else
			MessageBox("You must select a visual to edit");
	}
	else
		MessageBox("You must select a menu first");
#endif
}

void EngineXWin::OnButtonMoveVisualUpList()
{
#ifndef _ClientOutput
	// GET menu selections
	CWnd *pMenuWnd = m_menuDBDlgBar.GetDlgItem(IDC_LIST_Menus);
	CWnd *pVisualWnd = m_menuDBDlgBar.GetDlgItem(IDC_LIST_MenuVisuals);

	CListBox *pMenuList = (CListBox*)(pMenuWnd);
	CListBox *pVisualList = (CListBox*)(pVisualWnd);
	if(!pMenuList || !pVisualList) return;

	int iMenuIndex = pMenuList->GetCurSel();
	int iVisualIndex = pVisualList->GetCurSel();
	if((iMenuIndex == -1) || (iVisualIndex == -1)) return;
	
	// GET a pointer to the menu position
	POSITION pos = m_gStatus->m_menuDatabase->FindIndex(iMenuIndex);
	if(!pos) return;
	
	// GET the selected menu
	CMenuWindow *pMenu = dynamic_cast<CMenuWindow*>(m_gStatus->m_menuDatabase->GetAt(pos));
	if(!pMenu) return;
	if(!pMenu->m_childList)
		pMenu->m_childList = new CMenuVisualList();

	// GET the selected visual position
	pos = pMenu->m_childList->FindIndex(iVisualIndex);
	if(!pos) return;

	// GET the selected menu visual
	CMenuVisual *pVisual = dynamic_cast<CMenuVisual*>(pMenu->m_childList->GetAt(pos));

	// MOVE the selected visual BACK one element in the list
	POSITION posPrevious = pos;
	pMenu->m_childList->GetPrev(posPrevious);
	pMenu->m_childList->InsertBefore(posPrevious, pVisual);

	// REMOVE the element at the OLD position
	pMenu->m_childList->RemoveAt(pos);
	
	// SET the selection to the newly added menu
	pVisualList->SetCurSel(iVisualIndex-1);
	
	// CLEAR and REFRESH the visuals list
	RefreshMenuVisualsList(iVisualIndex-1);
#endif
}

void EngineXWin::OnButtonMoveVisualDownList()
{
#ifndef _ClientOutput
	// GET menu selections
	CWnd *pMenuWnd = m_menuDBDlgBar.GetDlgItem(IDC_LIST_Menus);
	CWnd *pVisualWnd = m_menuDBDlgBar.GetDlgItem(IDC_LIST_MenuVisuals);

	CListBox *pMenuList = (CListBox*)(pMenuWnd);
	CListBox *pVisualList = (CListBox*)(pVisualWnd);
	if(!pMenuList || !pVisualList) return;

	int iMenuIndex = pMenuList->GetCurSel();
	int iVisualIndex = pVisualList->GetCurSel();
	if((iMenuIndex == -1) || (iVisualIndex == -1)) return;
	
	// GET a pointer to the menu position
	POSITION pos = m_gStatus->m_menuDatabase->FindIndex(iMenuIndex);
	if(!pos) return;
	
	// GET the selected menu
	CMenuWindow *pMenu = dynamic_cast<CMenuWindow*>(m_gStatus->m_menuDatabase->GetAt(pos));
	if(!pMenu) return;
	if(!pMenu->m_childList)
		pMenu->m_childList = new CMenuVisualList();

	// GET the selected visual position
	pos = pMenu->m_childList->FindIndex(iVisualIndex);
	if(!pos) return;

	// GET the selected menu visual
	CMenuVisual *pVisual = dynamic_cast<CMenuVisual*>(pMenu->m_childList->GetAt(pos));

	// MOVE the selected visual FORWARD one element in the list
	POSITION posNext = pos;
	pMenu->m_childList->GetNext(posNext);
	pMenu->m_childList->InsertAfter(posNext, pVisual);

	// REMOVE the element at the OLD position
	pMenu->m_childList->RemoveAt(pos);
	
	// SET the selection to the newly added menu
	pVisualList->SetCurSel(iVisualIndex+1);
	
	// CLEAR and REFRESH the visuals list
	RefreshMenuVisualsList(iVisualIndex+1);
#endif
}

void EngineXWin::OnButtonExportVisual()
{
	//If they have a menu selected
	int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_Menus, LB_GETCURSEL, 0, 0);
	if(menuIndex != -1)
	{
		//If they have a visual selected
		int visualIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MenuVisuals, LB_GETCURSEL, 0, 0);
		if(visualIndex != -1)
		{

			//Get a pointer to the menu
			POSITION posMenu = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
			if(posMenu)
			{
				CMenuWindow *menu = (CMenuWindow*)m_gStatus->m_menuDatabase->GetAt(posMenu);

				//Get a pointer to the visual
				POSITION posVisual = menu->m_childList->FindIndex(menu->m_childList->GetCount() - visualIndex - 1);
				if(posVisual)
				{
				     CMenuVisual * mVis = (CMenuVisual*)menu->m_childList->GetAt(posVisual);
				     int typeTest = mVis->GetType();
					 
					 static char BASED_CODE filter[] = "MIU Files|*.MIU|All Files (*.*)|*.*||";
					 CFileDialog opendialog( FALSE, "MIU", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
					 if ( opendialog.DoModal() == IDOK )
					 {
					   CString fileName = opendialog.GetFileName();
					   CFileException exc;
					   CFile fl;
					   CXMLFileIO xmlOut;
		               bool res = xmlOut.Open(fileName.GetBuffer(), true);
					   if(res)
					   {
                           mVis->Serialize(xmlOut);
						   xmlOut.Close();
					   }
					   else
						   AfxMessageBox("Cannot create file : "+fileName);
					 }
					 SetCurrentDirectory(m_gStatus->GL_workingDirectory);
				}
			}
		}
		else
			MessageBox("You must select a visual to delete");
	}
	else
		MessageBox("You must select a menu first");
}

void EngineXWin::OnButtonImportVisual()
{
	//If they have a menu selected
	int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_Menus, LB_GETCURSEL, 0, 0);
	if(menuIndex != -1)
	{
			//Get a pointer to the menu
			POSITION posMenu = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
			if(posMenu)
			{
				 CMenuWindow *menu = (CMenuWindow*)m_gStatus->m_menuDatabase->GetAt(posMenu);
			     static char BASED_CODE filter[] = "MIU Files|*.MIU|All Files (*.*)|*.*||";
	             CFileDialog opendialog(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this);
				 if(opendialog.DoModal() == IDOK)
				 {
				   CString fileName = opendialog.GetFileName();
				   CFileException exc;
				   CFile fl;
				   CXMLFileIO xmlOut;
	               bool res = xmlOut.Open(fileName.GetBuffer(), false);
				   if(res)
				   {
					   CMenuVisual * mVis = new CMenuVisual();
                       mVis->Serialize(xmlOut);
					   xmlOut.Close();
                       int typeTest = mVis->GetType();

                       if(!menu->m_childList)
						   menu->m_childList = new CMenuVisualList();

					   menu->m_childList->AddTail(mVis);
					   RefreshMenuVisualsList(menu->m_childList->GetCount()-1);
				   }
				   else
					   AfxMessageBox("Cannot load file : "+fileName);
				 }
				 SetCurrentDirectory(m_gStatus->GL_workingDirectory);
			}
	}
	else
		MessageBox("You must select a menu first");
}

void EngineXWin::OnButtonDeleteVisual()
{
	//If they have a menu selected
	int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_Menus, LB_GETCURSEL, 0, 0);
	if(menuIndex != -1)
	{
		//If they have a visual selected
		int visualIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MenuVisuals, LB_GETCURSEL, 0, 0);
		if(visualIndex != -1)
		{
			if(AfxMessageBox("Are you sure you want to delete the selected visual?", MB_OKCANCEL, 0) != IDOK)
				return;

			//Get a pointer to the menu
			POSITION posMenu = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
			if(posMenu)
			{
				CMenuWindow *menu = (CMenuWindow*)m_gStatus->m_menuDatabase->GetAt(posMenu);

				//Get a pointer to the visual
				POSITION posVisual = menu->m_childList->FindIndex(visualIndex);
				if(posVisual)
				{
					CMenuVisual * mVis = (CMenuVisual*)menu->m_childList->GetAt(posVisual);
					//Delete the visual, remove it from the list, and refresh the list
					menu->m_childList->DeleteByObject(mVis);
					menu->m_childList->RemoveAt(posVisual);
					RefreshMenuVisualsList(visualIndex-1);
				}
			}
		}
		else
			MessageBox("You must select a visual to delete");
	}
	else
		MessageBox("You must select a menu first");

}

void EngineXWin::OnButtonIDCheck()
{
	// Look at menus and build a list of all ids that occur more than once
	POSITION posMenu = m_gStatus->m_menuDatabase->GetHeadPosition();
	while(posMenu != NULL)
	{
		CMenuVisual *visual = (CMenuVisual*)m_gStatus->m_menuDatabase->GetNext(posMenu); 

		POSITION posMenuCheck = m_gStatus->m_menuDatabase->GetHeadPosition();
		while(posMenuCheck != NULL)
		{
			CMenuVisual *visualCheck = (CMenuVisual*)m_gStatus->m_menuDatabase->GetNext(posMenuCheck); 
			if((visualCheck->m_visualID == visual->m_visualID) && (visual != visualCheck))
				MessageBox("Conflicting ID: " + visual->m_name + " / " + visualCheck->m_name + " (" + m_gStatus->m_cabLib.ConvToString(visual->m_visualID) + ")");
		}
	}

	MessageBox("Check Complete.");
}

void EngineXWin::OnButtonTestAsInstance()
{
    //If they have a menu selected
	int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_Menus, LB_GETCURSEL, 0, 0);
	if(menuIndex == -1) return;
	
	//Get a pointer to the menu
	POSITION posMenu = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
	if(!posMenu) return;

	m_gStatus->InstanceMenu(
		((CMenuWindow*)m_gStatus->m_menuDatabase->GetAt(posMenu))->m_visualID,
		((CMenuWindow*)m_gStatus->m_menuDatabase->GetAt(posMenu))->m_visualID + 1000
		);
}


void EngineXWin::OnButtonEditMenuSystemFonts()
{
	static bool runAlready = false;
	static CMenuImage fontImage;
	//On the first run, show an image so we can visibly edit each character
	if(!runAlready)
	{
		fontImage.m_width	= 0;
		fontImage.m_height	= 0;

		if(!m_fontsWindow.m_childList)
			m_fontsWindow.m_childList = new CMenuVisualList();

		m_fontsWindow.m_showCursor	= true;
		m_fontsWindow.m_draggable	= true;
		m_fontsWindow.m_width		= 300;
		m_fontsWindow.m_height		= 300;
		m_fontsWindow.m_visualID	= 999999998;
		m_fontsWindow.m_childList->RemoveAll();
		m_fontsWindow.m_childList->AddTail(&fontImage);
		m_gStatus->m_runtimeMenuDatabase->AddHead(&m_fontsWindow);
		runAlready = true;
		return;
	}

	//Make sure we have all textures loaded for all fonts
	LoadFontTextures();

	//Delete the old dialog
	if(m_fontsDlg)
		delete m_fontsDlg;

	//Create a new Dialog
	m_fontsDlg						= new CEditFont();
	m_fontsDlg->m_gStatus			= m_gStatus;
	m_fontsDlg->m_pFontImage		= &fontImage;

	//Show the dialog
	m_fontsDlg->Create(IDD_DIALOG_MenuSysFonts);

	//Reset it for the next time we edit
	runAlready = false;
}

void EngineXWin::OnButtonEditMenuIcons()
{
	static bool runAlready = false;
	static CMenuImage iconImage;
	//On the first run, show an image so we can visibly edit each character
	if(!runAlready)
	{
		iconImage.m_width	= 0;
		iconImage.m_height	= 0;

		if(!m_iconsWindow.m_childList)
			m_iconsWindow.m_childList = new CMenuVisualList();

		m_iconsWindow.m_showCursor	= true;
		m_iconsWindow.m_draggable	= true;
		m_iconsWindow.m_width		= 300;
		m_iconsWindow.m_height		= 300;
		m_iconsWindow.m_visualID	= 999999997;
		m_iconsWindow.m_childList->RemoveAll();
		m_iconsWindow.m_childList->AddTail(&iconImage);
		m_gStatus->m_runtimeMenuDatabase->AddHead(&m_iconsWindow);
		runAlready = true;
		return;
	}

	//Make sure we have all textures loaded for all fonts
	LoadIconTextures();

	//Delete the old dialog
	if(m_iconsDlg)
		delete m_iconsDlg;

	//Create a new Dialog
	m_iconsDlg					= new CEditIcons();
	m_iconsDlg->m_pIconList		= m_gStatus->m_menuIconDatabase;
	m_iconsDlg->m_pIconWindow	= &m_iconsWindow;
	m_iconsDlg->m_gStatus		= m_gStatus;

	//Show the dialog
	m_iconsDlg->Create(IDD_DIALOG_MenuSysIcons);

	//Reset it for the next time we edit
	runAlready = false;
}

void EngineXWin::OnButtonEditMenuSystemCursor()
{
	static bool runAlready = false;
	static CMenuImage cursorImage;
	//On the first run, show an image so we can visibly edit each cursor
	if(!runAlready)
	{
		cursorImage.m_width	 = 0;
		cursorImage.m_height = 0;

		if(!m_cursorsWindow.m_childList)
			m_cursorsWindow.m_childList = new CMenuVisualList();

		m_cursorsWindow.m_showCursor = true;
		m_cursorsWindow.m_draggable	 = true;
		m_cursorsWindow.m_width		 = 300;
		m_cursorsWindow.m_height	 = 300;
		m_cursorsWindow.m_visualID	 = 999999999;
		m_cursorsWindow.m_childList->RemoveAll();
		m_cursorsWindow.m_childList->AddTail(&cursorImage);
		m_gStatus->m_runtimeMenuDatabase->AddHead(&m_cursorsWindow);
		runAlready = true;
		return;
	}

	//Make sure we have all textures loaded for all fonts
	LoadCursorTextures();

	//Delete the old dialog
	if(m_cursorsDlg)
		delete m_cursorsDlg;

	//Create a new Dialog
	m_cursorsDlg					= new CEditCursors();
	m_cursorsDlg->m_gStatus			= m_gStatus;
	m_cursorsDlg->m_pCursorImage	= &cursorImage;

	//Show the dialog
	m_cursorsDlg->Create(IDD_DIALOG_MenuSysCursors);

	//Reset it for the next time we edit
	runAlready = false;
}

void EngineXWin::OnButtonSaveMenuSystem()
{
	CString mainDir;
	char buffer[_MAX_PATH];
	_getcwd(buffer, _MAX_PATH);
	mainDir = buffer;
	CString gameFilesDir = m_gStatus ? m_gStatus->GL_pathGameFiles : CString();
	if(gameFilesDir.IsEmpty())
		gameFilesDir = mainDir + "\\GameFiles\\";
	if(!SetCurrentDirectory(gameFilesDir))
	{
		MessageBox("Could not open the GameFiles folder:\n\n" + gameFilesDir,
		           "Save Menu System", MB_OK | MB_ICONWARNING);
		return;
	}

	static char BASED_CODE filter[] = "Menu system (*.MOD)|*.MOD|All Files (*.*)|*.*||";
	CFileDialog saveDialog(FALSE, "MOD", "EARevolution.MOD",
	                       OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST,
	                       filter, this);
	if(saveDialog.DoModal() == IDOK)
	{
		CString filePath = saveDialog.GetPathName();
		CString backupPath, backupError;
		if(!EngineXCreateDatabaseBackup(filePath, backupPath, backupError))
		{
			SetCurrentDirectory(mainDir);
			MessageBox("The menu system was not saved because its backup could not be made.\n\n" + backupError,
			           "Save Menu System", MB_OK | MB_ICONWARNING);
			return;
		}

		CXMLFileIO xmlOut;
		char * writablePath = filePath.GetBuffer();
		bool opened = xmlOut.Open(writablePath, true);
		filePath.ReleaseBuffer();
		if(!opened)
		{
			SetCurrentDirectory(mainDir);
			MessageBox("Could not save the menu system to:\n\n" + filePath,
			           "Save Menu System", MB_OK | MB_ICONWARNING);
			return;
		}
		xmlOut.WriteAttribute("FileVersion", "1.0");

		//Write out the Font List
		xmlOut.WriteOpeningTag("FontList");
		m_gStatus->m_menuFontList->Serialize(xmlOut);
		xmlOut.WriteClosingTag("FontList");

		//Write out the Icon Database
		xmlOut.WriteOpeningTag("IconDatabase");
		m_gStatus->m_menuIconDatabase->Serialize(xmlOut);
		xmlOut.WriteClosingTag("IconDatabase");

		//Write out the Mouse Cursor
		for(int i = 0; i < CURSOR_MAX; i++)
		{
			xmlOut.WriteOpeningTag("MouseCursor");
			m_gStatus->m_mouseCursors[i]->Serialize(xmlOut);
			xmlOut.WriteClosingTag("MouseCursor");
		}

		//Write out the Menu Database
		xmlOut.WriteOpeningTag("MenuDatabase");
		m_gStatus->m_menuDatabase->Serialize(xmlOut);
		xmlOut.WriteClosingTag("MenuDatabase");

		xmlOut.Close();
		m_quickMenuSystemPath = filePath;
		m_gStatus->m_loadedMenuSystemPath = filePath;
		SetCurrentDirectory(mainDir);
	}

	SetCurrentDirectory(mainDir);
}

void EngineXWin::OnButtonLoadMenuSystem()
{
	//use gamefiles folder for organization
	CString CurDir;
	CString MainDir;
	char buffer[_MAX_PATH];
	_getcwd(buffer, _MAX_PATH);
	MainDir = CurDir = buffer;
	CurDir = CurDir+"\\GameFiles\\";
	SetCurrentDirectory(CurDir);
	//end use gamefiles folder for organization

	static char BASED_CODE filter[] = "MOD Files|*.MOD|All Files (*.*)|*.*||";
	CFileDialog opendialog(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this);
	if(opendialog.DoModal() == IDOK)
	{
		SetCurrentDirectory(MainDir);
		CString filePath = opendialog.GetPathName();

		if(m_gStatus->LoadMenuSys(filePath))
		{
			m_quickMenuSystemPath = filePath;
			RefreshMenuList(-1);

			//Remove all menus
			m_gStatus->m_runtimeMenuDatabase->RemoveAll();

			//Open all menus to load all textures and fonts
			POSITION posMenu = m_gStatus->m_menuDatabase->GetHeadPosition();
			while(posMenu)
				m_gStatus->OpenMenu(((CMenuWindow*)m_gStatus->m_menuDatabase->GetNext(posMenu))->m_visualID, false);
		}
	}
	SetCurrentDirectory(MainDir);
}

void EngineXWin::OnButtonSaveKeyConfig()
{
	ControlDBObj *curControlDB = (ControlDBObj*)m_gStatus->m_controlsList->GetHead();
	curControlDB->List_controlInfoList->RemoveAll();

	curControlDB->List_controlInfoList->AddControlEvent("Left Click", "Shoot", 0, 0, 0, 0, FALSE, FALSE);
	curControlDB->List_controlInfoList->AddControlEvent("Right Click", "Use Jet Pack", 0, 0, 0, 0, FALSE, FALSE);
	curControlDB->List_controlInfoList->AddControlEvent("W", "Forward", 0, 0, 0, 0, FALSE, FALSE);
	curControlDB->List_controlInfoList->AddControlEvent("S", "Backward", 0, 0, 0, 0, FALSE, FALSE);
	curControlDB->List_controlInfoList->AddControlEvent("A", "Strafe Left", 0, 0, 0, 0, FALSE, FALSE);
	curControlDB->List_controlInfoList->AddControlEvent("D", "Strafe Right", 0, 0, 0, 0, FALSE, FALSE);
	curControlDB->List_controlInfoList->AddControlEvent("Q", "Turn Left", 0, 0, 0, 0, FALSE, FALSE);
	curControlDB->List_controlInfoList->AddControlEvent("E", "Turn Right", 0, 0, 0, 0, FALSE, FALSE);
	curControlDB->List_controlInfoList->AddControlEvent("R", "Toggle Autorun", 0, 0, 0, 0, FALSE, FALSE);
	curControlDB->List_controlInfoList->AddControlEvent("Space Bar", "Jump", 0, 0, 0, 0, FALSE, FALSE);
	curControlDB->List_controlInfoList->AddControlEvent("Enter", "Chat", 0, 0, 0, 0, FALSE, FALSE);
	curControlDB->List_controlInfoList->AddControlEvent("X", "Examine", 0, 0, 0, 0, FALSE, FALSE);
	curControlDB->List_controlInfoList->AddControlEvent("B", "Open Inventory", 0, 0, 0, 0, FALSE, FALSE);
	curControlDB->List_controlInfoList->AddControlEvent("Tab", "Toggle View Mode", 0, 0, 0, 0, FALSE, FALSE);

	m_gStatus->SaveControlConfig("KeyConfig.cnt");
}

void EngineXWin::RefreshMenuThemesList(int selection)
{
	//Clear the menu list
	m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MenuThemes, LB_RESETCONTENT, 0, 0);

	//Refill the menu list
	for(POSITION posCurTheme = m_menuThemeDatabase->GetHeadPosition(); posCurTheme != NULL;)
	{
		CMenuWindow *curTheme = (CMenuWindow*)m_menuThemeDatabase->GetNext(posCurTheme);
		m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MenuThemes, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)curTheme->m_name);
	}

	//Make sure we're not trying to select beyond the list
	if(selection >= m_menuThemeDatabase->GetCount())
		selection = m_menuThemeDatabase->GetCount() - 1;

	//Set the currently selected theme
	if(selection != -1)
		m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MenuThemes, LB_SETCURSEL, selection, 0);
}

void EngineXWin::RefreshMenuList(int selection)
{
	if(!m_gStatus)
		return;

	//Clear the menu list
	CListBox *pMenuList = (CListBox*)m_menuDBDlgBar.GetDlgItem(IDC_LIST_Menus);
	if(!pMenuList) return;
	int iTopIndex = pMenuList->GetTopIndex(); // Get index of the first visible item in the listbox
	pMenuList->ResetContent();

	//Refill the menu list
	for(POSITION posCurMenu = m_gStatus->m_menuDatabase->GetHeadPosition(); posCurMenu != NULL;)
	{
		CMenuWindow *curMenu = (CMenuWindow*)m_gStatus->m_menuDatabase->GetNext(posCurMenu);
		pMenuList->AddString(curMenu->m_name.GetBuffer());
		curMenu->m_name.ReleaseBuffer();
	}

	//Make sure we're not trying to select beyond the list
	if(selection >= m_gStatus->m_menuDatabase->GetCount())
		selection = m_gStatus->m_menuDatabase->GetCount() - 1;

	//Set the currently selected menu
	if(selection > -1)
		pMenuList->SetCurSel(selection);
	pMenuList->SetTopIndex(iTopIndex);

	//Clear and refill the visuals list
	RefreshMenuVisualsList(0);
}

void EngineXWin::RefreshMenuVisualsList(int selection)
{
	CListBox *pMenuList = (CListBox*)m_menuDBDlgBar.GetDlgItem(IDC_LIST_Menus);
	CListBox *pVisualList = (CListBox*)m_menuDBDlgBar.GetDlgItem(IDC_LIST_MenuVisuals);
	if(!pMenuList || !pVisualList) return;

	int iTopIndex = pVisualList->GetTopIndex(); // Get index of the first visible item in the listbox
	pVisualList->ResetContent();

	//Get the currently selected menu
	int menuIndex = pMenuList->GetCurSel();
	if(menuIndex > -1)
	{
		POSITION posMenu = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
		if(posMenu)
		{
			//Refill the visuals list
			CMenuWindow *menu = (CMenuWindow*)m_gStatus->m_menuDatabase->GetAt(posMenu);
			if(menu && menu->m_childList)
			{
				for(POSITION posCurVisual = menu->m_childList->GetHeadPosition(); posCurVisual != NULL;)
				{
					CMenuVisual *curVisual = NULL;
					try
					{
						curVisual = dynamic_cast<CMenuVisual*>(menu->m_childList->GetNext(posCurVisual));
						if (!curVisual) continue;
					}
					catch(std::bad_typeid ex)
					{
						continue;
					}
					pVisualList->AddString(curVisual->m_name.GetBuffer());
					curVisual->m_name.ReleaseBuffer();
				}

				//Make sure we're not trying to select beyond the list
				if(selection >= menu->m_childList->GetCount())
					selection = menu->m_childList->GetCount() - 1;
			}
		}
	}

	//Set the currently selected visual
	if(selection > -1)
		pVisualList->SetCurSel(selection);
	pVisualList->SetTopIndex(iTopIndex);
}

void EngineXWin::LoadCursorTextures()
{
	//Loop through all fonts in the list
	for(int i = 0; i < CURSOR_MAX; i++)
	{
		//Get a pointer to the current font
		CMenuImage *curCursor = m_gStatus->m_mouseCursors[i];

		//Load the font texture(if its not already loaded)
		curCursor->m_textureSelect = m_gStatus->m_textureDataBase->AddAssignTempTexture(m_gStatus->m_dxRenderer->m_pd3dDevice, curCursor->m_textureFile, 0);
	}
}

void EngineXWin::LoadFontTextures()
{
	//Loop through all fonts in the list
	POSITION posFont = m_gStatus->m_menuFontList->GetHeadPosition();
	while(posFont != NULL)
	{
		//Get a pointer to the current font
		CMenuFont *curFont = (CMenuFont*)m_gStatus->m_menuFontList->GetNext(posFont);

		//Load the font texture(if its not already loaded)
		curFont->m_textureSelect = m_gStatus->m_textureDataBase->AddAssignTempTexture(m_gStatus->m_dxRenderer->m_pd3dDevice, curFont->m_textureFile, 0);
	}
}

void EngineXWin::LoadIconTextures()
{
	//Loop through all fonts in the list
	POSITION posIcon = m_gStatus->m_menuIconDatabase->GetHeadPosition();
	while(posIcon != NULL)
	{
		//Get a pointer to the current font
		CMenuImage *curIcon = (CMenuImage*)m_gStatus->m_menuIconDatabase->GetNext(posIcon);

		//Load the font texture(if its not already loaded)
		curIcon->m_textureSelect = m_gStatus->m_textureDataBase->AddAssignTempTexture(m_gStatus->m_dxRenderer->m_pd3dDevice, curIcon->m_textureFile, 0);
	}
}

/*void EngineXWin::OnButtonEditTextureMenuBottomLeftCorner()
{
	CMaterialDlg dlg;
	CMenuWindow *menu = (CMenuWindow*)m_gStatus->m_runtimeMenuDatabase->GetHead();

	dlg.m_pxShaderSystemRef = NULL;
	dlg.m_vxShaderSystemRef = NULL;
	dlg.m_globalShaderRef = NULL;
	dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
	dlg.meshRef = menu->m_cornerBottomLeft->m_imageMesh;
	if(dlg.DoModal() == IDOK)
		menu->m_cornerBottomLeft->m_imageMesh->List_materialObject->Reset();
}

void EngineXWin::OnEditMenuBottomLeftCornerTextureCoords()
{
	POSITION pos = m_gStatus->m_runtimeMenuDatabase->GetHeadPosition();
	if(pos)
	{
		CMenuWindow *pMenu = (CMenuWindow*)m_gStatus->m_runtimeMenuDatabase->GetAt(pos);

		//Get the texture's dimensions
		float textureWidth = 512.0f;
		float textureHeight = 512.0f;
		pos = m_gStatus->m_textureDataBase->FindIndex(pMenu->m_cornerBottomLeft->m_imageMesh->List_textureSelect[0]);
		if(pos != NULL)
		{
			CTextureDBObj *texture = (CTextureDBObj*)m_gStatus->m_textureDataBase->GetAt(pos);
			textureWidth = texture->m_textureObject->m_widthBasis;
			textureHeight = texture->m_textureObject->m_hieghtBasis;
		}

		pMenu->m_cornerBottomLeft->SetTextureCoords(textureWidth, textureHeight);
		}//end lock buffer
	}
}*/
//---------------------------------------------------------------------------------------------------------------//
//
//											End of New Menu System Section
//
//---------------------------------------------------------------------------------------------------------------//

/*/---------------------edit display unit----------------------------------------------//
void EngineXWin::OnBTNMNUeditClickZone() 
{
#ifndef _ClientOutput
    int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_MainDB,LB_GETCURSEL ,0,0);
	if(menuIndex == -1){
		MessageBox("must select a menu");
	   return;
	}
    int clIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_GETCURSEL ,0,0);
	if(clIndex == -1){
	   MessageBox("must select a click zone");
	   return;
	}
	
	POSITION posloc = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
	if(!posloc)
		return;
        CMenu3DObj * menuObj = (CMenu3DObj *)m_gStatus->m_menuDatabase->GetAt(posloc);

		POSITION clPos = menuObj->m_clickActions->FindIndex(clIndex);
		if(!clPos)
			return;
	        CCLickZoneObj * cPtr = ( CCLickZoneObj* )menuObj->m_clickActions->GetAt(clPos);
			CMenuClickZoneDlg dlg;
		    dlg.menu3DObjRef = menuObj;
		    dlg.clickZoneRef = cPtr;
		    if(dlg.DoModal()==IDOK){
			      InListClickZones(clIndex);
			}
#endif
}
//---------------------edit display unit----------------------------------------------//
void EngineXWin::InListClickZones(int sel)
{
int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_MainDB,LB_GETCURSEL ,0,0);
	if(menuIndex == -1){
	   return;
	}
    POSITION posloc = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
	if(!posloc)
		return;
        CMenu3DObj * menuObj = (CMenu3DObj *)m_gStatus->m_menuDatabase->GetAt(posloc);
		if(!menuObj->m_clickActions)
			return;
            m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_RESETCONTENT,0,0);	
            for(POSITION posLoc = menuObj->m_clickActions->GetHeadPosition();posLoc != NULL;){//visual loop
	            CCLickZoneObj * mPtr = ( CCLickZoneObj* ) menuObj->m_clickActions->GetNext(posLoc); 

				
				if(mPtr->m_action == 0)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"None");	
				else if(mPtr->m_action == 1)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Close Menu");
				else if(mPtr->m_action == 2)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Edit Text");
                else if(mPtr->m_action == 3)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Connect TCPIP Global");
				else if(mPtr->m_action == 4)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Respawn (Network)");
				else if(mPtr->m_action == 5)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Load World And Spawn");
				else if(mPtr->m_action == 6)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Set Display Resolution");
				else if(mPtr->m_action == 7)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Mouse Invert/on off");
				else if(mPtr->m_action == 8)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Quit Game");
				else if(mPtr->m_action == 9)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Open Menu by Type");
				else if(mPtr->m_action == 10)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Delete character on Account Ref 50");
				else if(mPtr->m_action == 11)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Change Entity Item By Location ID");
				else if(mPtr->m_action == 12)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Create Character From Ids");
				else if(mPtr->m_action == 13)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Change Entity Def Next Item by Index");
				else if(mPtr->m_action == 14)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Change Entity Def Prev Item by Index");
				else if(mPtr->m_action == 15)
				m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"DEPRECATED: Arm Item On Entity");
				else if(mPtr->m_action == 16)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Disarm Item On Entity");
				else if(mPtr->m_action == 17)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Buy Store Item");
				else if(mPtr->m_action == 18)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Sell Item To Store");
				else if(mPtr->m_action == 19)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Loot Item");
				else if(mPtr->m_action == 20)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Bank Item");
				else if(mPtr->m_action == 21)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Take Item From Bank");
				else if(mPtr->m_action == 22)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"DEPRECATED: Use Item");
				else if(mPtr->m_action == 23)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Make Controls Default");
				else if(mPtr->m_action == 24)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Index Slider Up");
				else if(mPtr->m_action == 25)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Index Slider Down");
				else if(mPtr->m_action == 26)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Send Trade Offer");
				else if(mPtr->m_action == 27)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Move List Item");
				else if(mPtr->m_action == 28)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Accept Trade Offer");
				else if(mPtr->m_action == 29)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Cancel Trade");
				else if(mPtr->m_action == 30)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Withdrawal Amount");
				else if(mPtr->m_action == 31)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Deposit Amount");
				else if(mPtr->m_action == 32)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Add Control Event");
				else if(mPtr->m_action == 33)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Restore Control Defaults");
				else if(mPtr->m_action == 34)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Delete Control Key");
				else if(mPtr->m_action == 35)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Replace Control Key");
				else if(mPtr->m_action == 36)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Send Server Command");
				else if(mPtr->m_action == 37)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Accept Potential Group Joiner");
				else if(mPtr->m_action == 38)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Decline Potential Group Joiner");
				else if(mPtr->m_action == 39)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Delete Player From Group");
				else if(mPtr->m_action == 40)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Invite Player by Local Select");
				else if(mPtr->m_action == 41)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Call Group Menu");
				else if(mPtr->m_action == 42)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Quest Selection");
				else if(mPtr->m_action == 43)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Use Ability");
				else if(mPtr->m_action == 44)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Call Skills Menu");
				else if(mPtr->m_action == 45)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Use Ability by Inputs");
				else if(mPtr->m_action == 46)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"View Clan Roster");
				else if(mPtr->m_action == 47)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Execute passed in commands");
				else if(mPtr->m_action == 48)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Destroy Selcted Item");
				else if(mPtr->m_action == 49)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Add Item To Trade Offer");
				else if(mPtr->m_action == 50)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Add Item To House Store");
				else if(mPtr->m_action == 51)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Buy Item From House Store");
				else if(mPtr->m_action == 52)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Withdrawal Cash From House Store");
				else if(mPtr->m_action == 53)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Remove Item From House Store");
				else if(mPtr->m_action == 54)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Pending Control Assign");
				else if(mPtr->m_action == 55)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Call Temp Menu");
				else if(mPtr->m_action == 56)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Close Temp Menus");
				else if(mPtr->m_action == 57)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Increment Vis Up");
				else if(mPtr->m_action == 58)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Decrement Vis");
				else if(mPtr->m_action == 59)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Transfer Text Data");
				else if(mPtr->m_action == 60)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Set Macro");
				else if(mPtr->m_action == 61)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Abort Macro");
				else if(mPtr->m_action == 62)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Multibind On-Off");
				else if(mPtr->m_action == 63)
				m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Restore Default Controls 2");
				else if(mPtr->m_action == 64)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Call Quest Journal");
				else if(mPtr->m_action == 65)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Report Murderer");
				else if(mPtr->m_action == 66)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Enable/Disable Music");
				else if(mPtr->m_action == 67)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Open Menu X and Close Menu Y");
				else if(mPtr->m_action == 68)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Toggle Menu");
				else if(mPtr->m_action == 69)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Loot All Items");
				else if(mPtr->m_action == 70)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Activate Item");
				else if(mPtr->m_action == 71)
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Teleport");
				
				else
			    m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Uknown");
			}
            if(sel != -1)
	            m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_SETCURSEL ,sel,0);
}
//---------------------edit display unit----------------------------------------------//
void EngineXWin::OnBTNMNUAddClickZone() 
{
#ifndef _ClientOutput
	int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_MainDB,LB_GETCURSEL ,0,0);
	if(menuIndex == -1){
	   MessageBox("must select a menu");
	   return;
	}
   
	POSITION posloc = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
	if(!posloc)
		return;
        CMenu3DObj * menuObj = (CMenu3DObj *)m_gStatus->m_menuDatabase->GetAt(posloc);
	    CCLickZoneObj * newClickZone = new CCLickZoneObj();
		CMenuClickZoneDlg dlg;
		dlg.menu3DObjRef = menuObj;
		dlg.clickZoneRef = newClickZone;
		if(dlg.DoModal()==IDOK){
           if(!menuObj->m_clickActions)
			   menuObj->m_clickActions = new CCLickZoneList();

               menuObj->m_clickActions->AddTail(newClickZone);
			   InListClickZones(menuObj->m_clickActions->GetCount()-1);
		}
		else{
           delete newClickZone;	
		   newClickZone=0;
		}
#endif
}
//---------------------edit display unit----------------------------------------------//
void EngineXWin::OnBTNMENUDeleteMenu() 
{
if(AfxMessageBox("Are you sure?",MB_OKCANCEL,0)!=IDOK)
   return;

int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_MainDB,LB_GETCURSEL ,0,0);
	if(menuIndex == -1){
	   MessageBox("must select a menu");
	   return;
	}
   
	POSITION posloc = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
	if(!posloc)
		return;
        CMenu3DObj * menuObj = (CMenu3DObj *)m_gStatus->m_menuDatabase->GetAt(posloc);
		//menuObj->SafeDelete();
		//delete menuObj;
		//menuObj=0;
		m_gStatus->m_menuDatabase->RemoveAt(posloc);
		MenuInList(0);
}
//-----------------------------------------------------------------------------------//
void EngineXWin::OnBTNMenucursorDelete() 
{
	int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_MainDB,LB_GETCURSEL ,0,0);
	if(menuIndex == -1){
	   MessageBox("must select a menu");
	   return;
	}
   
	POSITION posloc = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
	if(!posloc)
		return;
        CMenu3DObj * menuObj = (CMenu3DObj *)m_gStatus->m_menuDatabase->GetAt(posloc);
        if(!menuObj->m_cursor){
            MessageBox("No cursor to delete");
            return;
		}
		
		menuObj->m_cursor->SafeDelete();
		delete menuObj->m_cursor;
		menuObj->m_cursor=0; 
}
//-----------------------------------------------------------------------------------//
void EngineXWin::OnBTNMenucursorScale() 
{
 #ifndef _ClientOutput
	int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_MainDB,LB_GETCURSEL ,0,0);
	if(menuIndex == -1){
	   MessageBox("must select a menu");
	   return;
	}
   
	POSITION posloc = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
	if(!posloc)
		return;
        CMenu3DObj * menuObj = (CMenu3DObj *)m_gStatus->m_menuDatabase->GetAt(posloc);
        if(!menuObj->m_cursor){
            MessageBox("No valid cursor");
            return;
		}
		
	    CCursorScaleDlg dlg;
		dlg.m_gStatus = m_gStatus;
		dlg.m_menuRef = menuObj;
		dlg.g_pd3dDeviceRef = m_gStatus->m_dxRenderer->m_pd3dDevice;
		dlg.m_renderStateObjectRef = m_gStatus->m_renderStateObject;
		dlg.m_cgGlobalShaderDB = m_gStatus->m_cgGlobalShaderDB;
		dlg.m_textureDataBase = m_gStatus->m_textureDataBase;
		dlg.m_dirSndRef = m_gStatus->m_dirSnd;
		dlg.m_elementDBREF = m_gStatus->m_elementDB;
		dlg.m_skillDatabaseREF = m_gStatus->m_skillDatabase;
		dlg.m_explosionList = m_gStatus->m_explosionDBList;
		dlg.m_scale = menuObj->m_cursor->m_scaleFactor;
		dlg.m_cursorRef = menuObj->m_cursor;
	    if(dlg.DoModal() == IDOK){
            //menuObj->m_cursor->m_scaleFactor = dlg.m_scale;
		}
#endif
}
//---------------------Save Menu Database---------------------------------------------//
void EngineXWin::OnSelchangeLISTMENUMainDB() 
{
	MenuVisualsInList(0);
	InListClickZones(0);
}
//---------------------Save Menu Database---------------------------------------------//
void EngineXWin::OnBTNMENUSave() 
{
	 //use gamefiles folder for organization
     CString CurDir;
     CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
     //end use gamefiles folder for organization
     static char BASED_CODE filter[] = "MOD Files|*.MOD|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "MOD", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   CFileException exc;
       CFile fl;
       fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
       CArchive the_out_Archive(&fl,CArchive::store);
       the_out_Archive << m_gStatus->m_menuDatabase;
       the_out_Archive.Close();
       fl.Close();
	 }
	 
	 SetCurrentDirectory(MainDir);
}
//---------------------Load Menu Database---------------------------------------------//
void EngineXWin::OnBTNMENULoad() 
{
   //use gamefiles folder for organization
     CString CurDir;
     CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
     //end use gamefiles folder for organization

     static char BASED_CODE filter[] = "MOD Files|*.MOD|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
	if ( opendialog.DoModal() == IDOK )
	 {
	   CString filePath = opendialog.GetPathName();
	   SetCurrentDirectory(MainDir);
	   if(m_gStatus->LoadMenuSys(filePath))
	   {
	      m_quickMenuSystemPath = filePath;
	      if(m_gStatus->m_editorModeOn == TRUE)
	         MenuInList(0);
	   }
	 }
     SetCurrentDirectory(MainDir);
}
//---------------------edit display unit----------------------------------------------//
void EngineXWin::OnBTNMENUDeleteUnit() 
{
	if(AfxMessageBox("Are you sure?",MB_OKCANCEL,0)!=IDOK)
   return;

    int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_MainDB,LB_GETCURSEL ,0,0);
	if(menuIndex == -1){
	   MessageBox("must select a menu");
	   return;
	}
    int visIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_UnitList,LB_GETCURSEL ,0,0);
	if(visIndex == -1){
	   MessageBox("must select a visual");
	   return;
	}

    POSITION posloc = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
	if(!posloc)
		return;
        CMenu3DObj * menuObj = (CMenu3DObj *)m_gStatus->m_menuDatabase->GetAt(posloc);

		posloc = menuObj->m_visualItems->FindIndex(visIndex);
	    if(!posloc)
		    return;
            CMenuVisObj * visObj = (CMenuVisObj *)menuObj->m_visualItems->GetAt(posloc);
            //visObj->SafeDelete();
			//delete visObj;
			//visObj=0;

            menuObj->m_visualItems->RemoveAt(posloc);
			MenuVisualsInList(visIndex-1);
}
//---------------------edit display unit----------------------------------------------//
void EngineXWin::OnBTNMENUEditDisplayUnit() 
{
#ifndef _ClientOutput
    int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_MainDB,LB_GETCURSEL ,0,0);
	if(menuIndex == -1){
	   MessageBox("must select a menu");
	   return;
	}
    int visIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_UnitList,LB_GETCURSEL ,0,0);
	if(visIndex == -1){
	   MessageBox("must select a visual");
	   return;
	}

    POSITION posloc = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
	if(!posloc)
		return;
        CMenu3DObj * menuObj = (CMenu3DObj *)m_gStatus->m_menuDatabase->GetAt(posloc);

		posloc = menuObj->m_visualItems->FindIndex(visIndex);
	    if(!posloc)
		    return;
            CMenuVisObj * visObj = (CMenuVisObj *)menuObj->m_visualItems->GetAt(posloc);
		    //visObj->m_skeletonObject
			CMenuAddUnitDlg dlg;
			dlg.g_pd3dDeviceRef = m_gStatus->m_dxRenderer->m_pd3dDevice;
		    //CMenuVisObj * visual = new CMenuVisObj();
		    dlg.m_visualObjectRef = visObj;
		    dlg.directSoundRef = m_gStatus->m_dirSnd;
		    dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
		    if(dlg.DoModal()==IDOK)
			{
		       MenuVisualsInList(visIndex);
			}
#endif
}
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNMenuCloseAllMenus() 
{
   for(POSITION posLoc = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
   {
       CMenu3DObj * menu = (CMenu3DObj *)m_gStatus->m_runtimeMenuDataBase->GetNext(posLoc);
	   menu->CloseMenu(&m_gStatus->m_globalCursorPos);
   }
}
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNMenuTest() 
{
    int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_MainDB,LB_GETCURSEL ,0,0);
	if(menuIndex == -1){
	   MessageBox("must select a menu");
	   return;
	}
	m_gStatus->UseMenu(menuIndex,0,0,0);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNMenuCursor() 
{
    int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_MainDB,LB_GETCURSEL ,0,0);
	if(menuIndex == -1){
	   MessageBox("must select a menu");
	   return;
	}
   
	POSITION posloc = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
	if(!posloc)
		return;
        CMenu3DObj * menuObj = (CMenu3DObj *)m_gStatus->m_menuDatabase->GetAt(posloc);
        if(!menuObj->m_cursor){//initialize
            
             //end use mapsmodels folder for organization
             static char BASED_CODE filter[] = "ARB Files|*.ARB|All Files (*.*)|*.*||";
	         CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
             if ( opendialog.DoModal() != IDOK ){
                   return;
	          }

			 menuObj->m_cursor = new CBoneCursorObj();
			 CString fileName = opendialog.GetPathName();
			 menuObj->m_cursor->m_skeletonObject = new CSkeletonObject();
			 menuObj->m_cursor->m_skeletonObject->InitBoneSystem(fileName,m_gStatus->m_dxRenderer->m_pd3dDevice);
			 menuObj->m_cursor->m_skeletonObject->m_SkeletalName = fileName;
			 CSkeletalAnimationDlg dlg;
			 dlg.m_renderStateObjectRef = m_gStatus->m_renderStateObject;
			 dlg.m_globalShaderRef = m_gStatus->m_cgGlobalShaderDB;
			 dlg.m_elementDBREF = NULL;
			 dlg.m_skillDatabaseREF = NULL;
			 dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
			 dlg.m_skeletonReferencePtr = menuObj->m_cursor->m_skeletonObject;
			 dlg.directSoundRef = m_gStatus->m_dirSnd;
			 dlg.DoModal();
			 m_gStatus->SkeletalTextureReassignmentWCompile(menuObj->m_cursor->m_skeletonObject);
            
			 return;
		}
		else{//edit
            CSkeletalAnimationDlg dlg;
		    dlg.m_renderStateObjectRef = m_gStatus->m_renderStateObject;
		    dlg.m_globalShaderRef = m_gStatus->m_cgGlobalShaderDB;
		    dlg.m_elementDBREF = NULL;
		    dlg.m_skillDatabaseREF = NULL;
		    dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
		    dlg.m_skeletonReferencePtr = menuObj->m_cursor->m_skeletonObject;
		    dlg.directSoundRef = m_gStatus->m_dirSnd;
		    dlg.DoModal();

		}
}
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNMENUAddMenu() 
{
#ifndef _ClientOutput
     CAddMenu dlg;
	 CMenu3DObj* menu = new CMenu3DObj();
		dlg.m_menuObjRef = menu;
		dlg.soundTrackDBRef = m_gStatus->m_soundTrackList;
		dlg.m_indMenuID  = m_gStatus->m_menuDatabase->GetFreeID();
     if(dlg.DoModal()==IDOK){
		if(!menu->m_visualItems)
            menu->m_visualItems = new CMenuVisObjList();
		if(!menu->m_clickActions)
			menu->m_clickActions = new CCLickZoneList();
        m_gStatus->m_menuDatabase->AddTail(menu);
		MenuInList(m_gStatus->m_menuDatabase->GetCount()-1);
	 }
#endif
}
//------------------------------------------------------------------------------------//
void EngineXWin::MenuInList(int sel)
{  
     m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_MainDB,LB_RESETCONTENT,0,0);	
     for(POSITION posLoc = m_gStatus->m_menuDatabase->GetHeadPosition();posLoc != NULL;){//visual loop
	     CMenu3DObj * mPtr = ( CMenu3DObj* ) m_gStatus->m_menuDatabase->GetNext(posLoc); 
         m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_MainDB,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)mPtr->m_menuName);	
	 }
     if(sel != -1)
	    m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_MainDB,LB_SETCURSEL ,sel,0);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::MenuVisualsInList(int sel)
{  
	int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_MainDB,LB_GETCURSEL ,0,0);
	if(menuIndex == -1){
	   return;
	}
    POSITION posloc = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
	if(!posloc)
		return;
        CMenu3DObj * menuObj = (CMenu3DObj *)m_gStatus->m_menuDatabase->GetAt(posloc);
		if(!menuObj->m_visualItems)
			return;
            m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_UnitList,LB_RESETCONTENT,0,0);	
            for(POSITION posLoc = menuObj->m_visualItems->GetHeadPosition();posLoc != NULL;){//visual loop
	            CMenuVisObj * mPtr = ( CMenuVisObj* ) menuObj->m_visualItems->GetNext(posLoc); 
                m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_UnitList,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)mPtr->m_visualName);	
			}
            if(sel != -1)
	            m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_UnitList,LB_SETCURSEL ,sel,0);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------------add a unit to -------------------//
void EngineXWin::OnBTNMENUAddUnit() 
{
#ifndef _ClientOutput
	int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_MainDB,LB_GETCURSEL ,0,0);
	if(menuIndex == -1){
	   MessageBox("must select a menu");
	   return;
	}
    POSITION posloc = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
	if(!posloc)
		return;
        CMenu3DObj * menuObj = (CMenu3DObj *)m_gStatus->m_menuDatabase->GetAt(posloc);

        CMenuAddUnitDlg dlg;
		dlg.g_pd3dDeviceRef = m_gStatus->m_dxRenderer->m_pd3dDevice;
		CMenuVisObj * visual = new CMenuVisObj();
		dlg.m_visualObjectRef = visual;
		dlg.directSoundRef = m_gStatus->m_dirSnd;
		dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
		if(dlg.DoModal()==IDOK)
		{
		       menuObj->m_visualItems->AddTail(visual);
		       MenuVisualsInList(menuObj->m_visualItems->GetCount()-1);
		}
#endif
}
//---------------------edit a menu----------------------------------------------------//
void EngineXWin::OnBTNMENUEditMenu() 
{
#ifndef _ClientOutput
	int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_MainDB,LB_GETCURSEL ,0,0);
	if(menuIndex == -1){
	   MessageBox("must select a menu");
	   return;
	}
    POSITION posloc = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
	if(!posloc)
		return;
        CMenu3DObj * menuObj = (CMenu3DObj *)m_gStatus->m_menuDatabase->GetAt(posloc);
      CAddMenu dlg;
	  dlg.m_menuObjRef = menuObj;
	  dlg.soundTrackDBRef = m_gStatus->m_soundTrackList;
	  dlg.m_indMenuID  = m_gStatus->m_menuDatabase->GetFreeID();
     if(dlg.DoModal()==IDOK){
		MenuInList(menuIndex);
	 }
#endif
}
*/

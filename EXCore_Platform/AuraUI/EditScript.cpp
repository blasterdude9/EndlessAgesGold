// C:\Projects\EXCore_Platform\AuraUI\EditScript.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "EditScript.h"

// CEditScript dialog
IMPLEMENT_DYNAMIC(CEditScript, CDialog)

CEditScript::CEditScript(CWnd* pParent /*=NULL*/)
	: CDialog(CEditScript::IDD, pParent)
{
	m_script = NULL;
}

BEGIN_MESSAGE_MAP(CEditScript, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_Functions, OnSelectFunction)
	ON_LBN_DBLCLK(IDC_LIST_Variables, OnDoubleClickVariable)
	//IDCANCEL is handled by CDialog::OnCancel()
	ON_BN_CLICKED(IDOK, OnButtonNext)
END_MESSAGE_MAP()

BOOL CEditScript::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Get the script from the text box
	CString temp = m_script->m_scriptText;
	temp.Replace("\n", "\r\n");
	SetDlgItemText(IDC_EDIT_Script, temp.GetBuffer());

	//Fill the combo
	FillFunctionList();
	FillVariableList();

	this->m_script->m_isBeingEdited = true;
	return TRUE;
}

void CEditScript::OnSelectFunction()
{
	char temp[255];

	//Get the double clicked variable's text
	int funcIndex = SendDlgItemMessage(IDC_COMBO_Functions, CB_GETCURSEL);
	SendDlgItemMessage(IDC_COMBO_Functions, CB_GETLBTEXT, funcIndex, (LPARAM)temp);

	//NULL terminate the string after the (
	int i = 0;
	while(temp[i] != '(')
		i++;
	temp[i + 1] = '\0';

	//Replace the selected text
	SendDlgItemMessage(IDC_EDIT_Script, EM_REPLACESEL, TRUE, (LPARAM)temp);
}

void CEditScript::OnDoubleClickVariable()
{
	char temp[255];
	
	//Get the double clicked variable's text
	int varIndex = SendDlgItemMessage(IDC_LIST_Variables, LB_GETCURSEL);
	SendDlgItemMessage(IDC_LIST_Variables, LB_GETTEXT, varIndex, (LPARAM)temp);

	//NULL terminate the string at the (
	int i = 0;
	while(temp[i] != '(')
		i++;
	temp[i] = '\0';

	//Replace the selected text
	SendDlgItemMessage(IDC_EDIT_Script, EM_REPLACESEL, TRUE, (LPARAM)temp);
}

void CEditScript::OnButtonNext()
{
	//Get the script from the text box
	char script[2048];
	GetDlgItemText(IDC_EDIT_Script, script, 2048);

	//Clear the script
	m_script->ClearScript();
	
	//Add every new line to the menu script
	int i = 0, start = 0;
	while(script[i] != '\0')
	{
		if(script[i] == '\r')
		{
			script[i] = '\n';
			script[++i] = '\0';

			m_script->AddLine(script + start);

			start = ++i;
		}
		else
			i++;
	}

	//Add the last line to the script
	if(start != i)
	{
		script[i] = '\n';
		script[++i] = '\0';
		m_script->AddLine(script + start);
	}

	CDialog::OnOK();
	this->m_script->m_isBeingEdited = false;
}

void CEditScript::FillFunctionList()
{
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"QuitGame()");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"OpenMenu(int MenuID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"ToggleMenu(int MenuID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"CloseMenu(int MenuID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"CloseAllMenus()");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"RequestCharacters(int ListBoxID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"PlayCharacter(int ListBoxID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"RequestInventory(int ListBoxID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"RequestSkills(int ListID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"RequestSpells(int ListID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"RequestStoreInventory(int ListID, int SendServerCommand)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"RequestLootInventory(int ListID, int SendServerCommand)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"RequestInteraction(int VisualID, int SendServerCommand)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"InitQuantity(int EditBoxID, int Quantity)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"BuyItem(int EditBoxID, int ListBoxID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"SellItem(int EditBoxID, int ListBoxID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"QuantityPopup(int ListBoxID, int QuantityMenuID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"LootItem(int ListToLootID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"LootAllItems(int ListToLootID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"DisplayCredits(int TextID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"DisplayHealth(int TextID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"DisplayEnergy(int TextID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"Teleport(float XPos, float YPos, float ZPos)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"SendServerCommand(int commandId, int visualReference)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"SetSystemOutput(int systemBoxID, int chatBoxID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"SetChatInput(int MenuVisualID, int TextBoxVisualID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"SendChatText()");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateCompass(float TextureStartX, float TextureFullWidth, float VisibleWidth)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateHealthProgressBar(int VisualID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateEnergyProgressBar(int VisualID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"DisplayCrystals(int VisualID, int CrystalType)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdatePowerupStatProgressBar(int VisualID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdatePowerupResistProgressBar(int VisualID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdatePowerupDamageProgressBar(int VisualID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"RequestCash()");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateTextInt(int Text)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateTextString(string Text)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"AcceptSelectedQuest(int QuestMenuID, int QuestListID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"SetToggleState(int VisualID, int NewState[0-Up, 1-Down])");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"SetVisualVisibleState(int VisualID, int HideShow[0-Hide, 1-Show])");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"AcceptGroupInvite()");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"DeclineGroupInvite()");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"InitRadar(Region, Map, Compass, X, Y, Zoom, GroupDot, PlayerDot, NPCDot, MobDot, MapScale, StartZoom)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateRadar()");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"ZoomRadarIn()");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"ZoomRadarOut()");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"RequestSkillTitles(int SkillTitleListVisualID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"SendSelectedSkillTitle(int SkillTitleListVisualID)");
	//SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"RequestControls(int invMsToggle,int msSenstivityTxt,int ListBoxID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"OnSelectedSetKeyMapListener(int ListBoxID)");
    SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"RenderMainCharacter(imageID,camDist,camYaw,camPitch,rezX,resY,R,G,B)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"RenderListGlidToImage(imageID,glidList,camDist,camPitch,rezX,resY,R,G,B)");
    SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"IndexDimUp(entityID,dimIndex)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"IndexDimDown(entityID,dimIndex)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"CreateCharacter(entityID,nameEdit,spawnIndex)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"InitEntityFromDBID(entityID,entityDbID)");
    SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"RenderEntityObject(imageID,camDist,camYaw,camPitch,rezX,resY,R,G,B,entityID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateEntityAnimation(entityID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"SetEntityAnimation(entityID,animIndex)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"DeleteCharacter(int ListBoxID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"DisplayStat(int TextID, int Stat(0-str, 1-dex, 2-wis))");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"DisplayResist(int TextID, int ResistChannel (0-7))");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"DisplayWound(int TextID, int WoundChannel (0-7))");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"DisplayCharName(int TextID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateResistProgress(int BarID, int Channel (0-7))");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateAmmoProgress(int BarID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateHPLight(int imageID, int lightMode(0 on, 1 blink))");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateResistLight(int imageID, int ResistChannel (0-7), int lightMode(0 on, 1 blink))");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateQuickslotHighlight(int imageID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateBuffIcon(int imageID, int +(0) or -(1), int Channel(0-7))");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateStoreEntity(int EntityID, int storeListID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateJetProgress(int progressID)");
    SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"SelectQuest(int storeListID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"ClearList(int storeListID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"InvertMouse(int ToggleID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"IncreaseMouseSensitivity(int SensitivityTextValueID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"DecreaseMouseSensitivity(int SensitivityTextValueID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"SetKeyControl(int ListBoxID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"SetDefaultKeyConfigs(int ListBoxID, int ToggleID, int SensitivityTextValueID, int volumeID, int multiToggleID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"WriteKeyConfigToFile(int ListBoxID, int ToggleID, int SensitivityTextValueID, int volumeID, int multiToggleID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"ReadKeyConfigFromFile(int ListBoxID, int ToggleID, int SensitivityTextValueID, int volumeID, int multiToggleID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"PlaySong(int soundIndex)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"DepositCredits(int EditBoxID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"WithdrawlCredits(int EditBoxID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"DepositItem(int EditBoxID, int InventoryListID, int bankListID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"TakeItem(int EditBoxID, int BankListID, int InventoryListID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"SelectDragItem(int MenuID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"DragQuickItem(int ImageID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"LoadQuickbarConfig(int QuickBarNumber, int StartingVisualID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"AddItemToQuickSlot(int VisualID, int QuickBarID, int SlotID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"SetQuickItemVisual(int VisualID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"ShowQuickItemHover(int BarID, int SlotID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"ShowEmblemHover(int ImageID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"ShowHover(int menuID, int VisualID, String DisplayText)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateGroupInfo(int ListID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"InitArenaBanners()");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"InitArenaBadges()");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"FadeImage(int ImageID, float FadeRate, float FadeDecrement)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"SetNotificationString(int textID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"CheckNPCCollision(int MenuID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateArenaTimer(int TextID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"SetRGB(int ImageID, float Red, float Green, float Blue)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"TradeCancel()");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"TradeConfirm()");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"TradeOffer(int ListID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"DropDragItemToList(int ListID, int EditID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"InitArenaReg(int MenuID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"EnableMenuButton(int ButtonID, bool IsEnabled)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"SetGroupedToggle(int MenuID, int ToggleID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"RequestArenaTypes(int MenuID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"RequestArenaInfo(int MenuID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"RequestArenaLeave()");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"ShowArenaTypesByMode()");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"DisplaySelectedArenaInfo(int ListboxID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateArenaCountdown(int ListboxID, int TextID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateArenaTypeDescription(int ToggleID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"JoinArena()");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"ShowImage(int ImageID, bool IsVisible)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateResistImage(int channelID, int lowVisID, int midVisID, int hiVisID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"EvaluateLevel(int textID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"EvaluateXP(int textID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateProgress(int ProgID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"InviteToSquad(int EditID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"RemoveFromParty(int ListID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"LeaveParty()");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"PromoteMember(int ListID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"InitTrade(int ListID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"SaveLastDragItem()");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateRecentJournalItem(int ListID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"RequestBadges()");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateBadgeView(int itemID, int badgeID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"RequestFullJournal(int ListID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"TextFlash(int textID,int alpha1,int alpha2,int duration)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"RemoveFromTrade(int ListID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"AddItemToTrade(int EditID, int inventoryListID, int tradeListID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"ResetList(int ListID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"DisplayQuestJournalDetails(int QuestListID, int DescriptionListID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateLoadingItems(int ListID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"CreateIconGrid(int MenuID, int ImageID, int NumRows, int NumColumns, int RowSpacing, int ColumnSpacing)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"DestroyIconGrid(int MenuID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"InitMenu(int MenuID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"SetActiveInventory(int MenuID, int InventoryID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"ActivateItem(int InventoryID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"DisarmItem(int InventoryID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"DeleteItem(int InventoryID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"SelectInventoryItem(int InventoryID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"AddToQuickbar(int InventoryID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"DepositItemToBank(int FromInventoryID, int ToInventoryID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"WithdrawItemFromBank(int FromInventoryID, int ToInventoryID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateFactionBonus(int imageID, int bonusID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateFactionTag(int imageID, int tagID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"PageGM()");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"SendToMessageChannel(int iMessageChannel (0 - whisper, 1 - group, 2 - clan, 3 - all))");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateMessageChannel(int visualID, int iMessageChannel (0 - whisper, 1 - group, 2 - clan, 3 - all))");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"SetScrollBar(int editID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"StartTrade()");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"SeeGroupInvite(int groupBoxID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"ButtonFlash(int buttonID,int alpha1,int alpha2,int duration)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"ProgressFlash(int progressID,int alpha1,int alpha2,int duration)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateVisibilityOn(int visualID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"DisplayNearbyPlayers(int ListID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UpdateAmmoRecharge(int BarID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"ShowHPHover(int BarID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"ShowXPHover(int BarID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"ShowFactionHover(int ImageID, int bonusValue)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"UseSkill(int abilityVal, int skillLevel, int skillType)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"ShowBasicHover(str Message)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"SendCommand(str command)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"RequestJoinClan(int ListID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"CreateClan(int EditID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"RemoveClanMember(int ListID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"SwapMenus(int menuToShowID, int menuToHideID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"InvertMulti(int ToggleID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"TestAH(int ListID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"BuyFromAH(int ListID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"AddToAH(int InventoryID, int priceText, int quantityText)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"PopulateStatsAH(int ListID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"ListByItemAH(int EditID)");
	SendDlgItemMessage(IDC_COMBO_Functions, CB_ADDSTRING, 0, (LPARAM)"NextItemsAH(int TextID)");
}

void CEditScript::FillVariableList()
{
	SendDlgItemMessage(IDC_LIST_Variables, LB_ADDSTRING, 0, (LPARAM)"$FPS(int)");
	SendDlgItemMessage(IDC_LIST_Variables, LB_ADDSTRING, 0, (LPARAM)"$Ping(int)");
	SendDlgItemMessage(IDC_LIST_Variables, LB_ADDSTRING, 0, (LPARAM)"$monkey(float)");
	SendDlgItemMessage(IDC_LIST_Variables, LB_ADDSTRING, 0, (LPARAM)"$monkey(string)");
}

// CEditScript message handlers

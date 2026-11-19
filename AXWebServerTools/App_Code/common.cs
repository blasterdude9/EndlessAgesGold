using System;
using System.Data;
using System.Configuration;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using System.Data.SqlClient;

/// <summary>
/// Summary description for common
/// </summary>
public class common
{
    DatabaseConnectionModule dbConModule = new DatabaseConnectionModule();

	public common()
	{
		//
		// TODO: Add constructor logic here
		//
	}

    public void FillListItemsBySearchTerm(string searchTerm,ref ListBox listLocal, SqlConnection hubConnection)
    {
        listLocal.Items.Clear();
        string sqlX = "SELECT ITEMNAME,GIL FROM GM_GIL WHERE ITEMNAME LIKE '%" + dbConModule.SanitizeIn(searchTerm) + "%'";
        SqlDataReader dbreader = dbConModule.sqlQuery(sqlX, hubConnection);
        while (dbreader.Read())
        {
            ListItem lItem = new ListItem(dbConModule.SanitizeOut(dbreader.GetSqlValue(1).ToString() + " " + dbreader.GetSqlValue(0).ToString()), dbreader.GetSqlValue(1).ToString());
            listLocal.Items.Add(lItem);
        }
        dbreader.Close();

        sqlX = "SELECT ITEMNAME,glid FROM gm_DynamicItems WHERE ITEMNAME LIKE '%" + dbConModule.SanitizeIn(searchTerm) + "%'";
        SqlDataReader xreader = dbConModule.sqlQuery(sqlX, hubConnection);
        while (xreader.Read())
        {
            ListItem lItem = new ListItem(dbConModule.SanitizeOut(xreader.GetSqlValue(1).ToString() + " " + xreader.GetSqlValue(0).ToString()), xreader.GetSqlValue(1).ToString());
            listLocal.Items.Add(lItem);
        }
        xreader.Close();
    }

    public void FillListItemsBySearchTerm(string searchTerm, ref DropDownList listLocal, SqlConnection hubConnection)
    {
        listLocal.Items.Clear();
        string sqlX = "SELECT ITEMNAME,GIL FROM GM_GIL WHERE ITEMNAME LIKE '%" + dbConModule.SanitizeIn(searchTerm) + "%'";
        SqlDataReader dbreader = dbConModule.sqlQuery(sqlX, hubConnection);
        while (dbreader.Read())
        {
            ListItem lItem = new ListItem(dbConModule.SanitizeOut(dbreader.GetSqlValue(1).ToString() + " " + dbreader.GetSqlValue(0).ToString()), dbreader.GetSqlValue(1).ToString());
            listLocal.Items.Add(lItem);
        }
        dbreader.Close();

        sqlX = "SELECT ITEMNAME,glid FROM gm_DynamicItems WHERE ITEMNAME LIKE '%" + dbConModule.SanitizeIn(searchTerm) + "%'";
        SqlDataReader xreader = dbConModule.sqlQuery(sqlX, hubConnection);
        while (xreader.Read())
        {
            ListItem lItem = new ListItem(dbConModule.SanitizeOut(xreader.GetSqlValue(1).ToString() + " " + xreader.GetSqlValue(0).ToString()), xreader.GetSqlValue(1).ToString());
            listLocal.Items.Add(lItem);
        }
        xreader.Close();
    }

    public void InListWorldRules(ref DropDownList listLocal,SqlConnection connection)
    {
        if (connection == null)
            return;
            
        SqlDataReader sqlReader = dbConModule.sqlQuery("Select ruleName from gm_worldRules order by orderindex",connection);
        while (sqlReader.Read())
            listLocal.Items.Add(sqlReader.GetSqlValue(0).ToString());

        sqlReader.Close();

        listLocal.SelectedIndex = 0;
      
    }

    public string GetItemNameByGil(string gil,SqlConnection connection)
    {
        string itemName = "";
        int gilInt = Convert.ToInt32(gil);

        if (gilInt < 15000)
        {//dynamic item
            SqlDataReader tempReader = dbConModule.sqlQuery("SELECT itemname FROM GM_GIL WHERE GIL = '" + gil + "'", connection);
            if (tempReader.Read())
            {
                itemName = dbConModule.SanitizeOut(tempReader.GetSqlValue(0).ToString());
            }
            tempReader.Close();
        }//end dynamic item
        else
        {//static apx item
            SqlDataReader tempReader = dbConModule.sqlQuery("SELECT itemname FROM gm_DynamicItems WHERE Glid = '" + gil + "'", connection);
            if (tempReader.Read())
            {
                itemName = dbConModule.SanitizeOut(tempReader.GetSqlValue(0).ToString());
            }
            tempReader.Close();
        }//end static apx item
        return itemName;
    }

    public void InListFunctions(ref DropDownList listLocal)
    {
        listLocal.Items.Clear();

        ListItem newListItem1 = new ListItem("OpenMenu(i:menu type)", "This function will call menu by type on the clients computer. [br]Example: OpenMenu(91)[br]");
        listLocal.Items.Add(newListItem1);

        ListItem newListItem2 = new ListItem("CloseAllMenus()", "This function will close any open menus. [br]Example: CloseAllMenus(91)[br]");
        listLocal.Items.Add(newListItem2);

        ListItem newListItem3 = new ListItem("SetMenuObjectTexture(s:visualname,s:texturename,i:texturelevel)",
                     "This set a texture to a specific visual on a menu, this can be used with urls from a web" +
                     "site to support dynamic content based on pictures and reconfigure the same menu many different ways. " +
                     "[br]Example: SetMenuObjectTexture(author picture,http://wwww.picscom.com/gilbert.jpg,0)[br]");
        listLocal.Items.Add(newListItem3);
        ListItem newListItem4 = new ListItem("AddMiscTextToList(s:visualName,f:lineLength)", "this function will send normal quest response to a list visual in a menu. Example: [br]AddMiscTextToList(textList,.4)[br]");
        listLocal.Items.Add(newListItem4);
        ListItem newListItem5 = new ListItem("StartSoundtrack(i:songTrackIndex, i:fadeTime, i:loopcount)", "This function will play a track or streaming station out of the soundtrack Database, loop count and fade time dont apply to streaming music at this time and -1 loop count will go forever untill a new song is played. [br]Example: StartSoundtrack(59,1500,-1)[br]");
        listLocal.Items.Add(newListItem5);
        ListItem newListItem6 = new ListItem("ForcePad(f:xForce, i:yForce, i:zForce)", "This function will boost a player based on a xyz world coordinate vector, the following example will launche the player straight up into the air. [br]Example: ForcePad(0.0,3.0,0.0)[br]");
        listLocal.Items.Add(newListItem6);
        ListItem newListItem7 = new ListItem("ArmItem(i:globalID)", "This function will arm a player with an item based on global id, you can aquire a global id from any item search panel. [br]Example: ArmItem(78)[br]");
        listLocal.Items.Add(newListItem7);
        ListItem newListItem8 = new ListItem("DoExplosionEffect(i:explosionIndex)", "This cause an explosion effect around a primary player and make that effect follow the player till expired. [br]Example: DoExplosionEffect(50)[br]");
        listLocal.Items.Add(newListItem8);
        ListItem newListItem9 = new ListItem("Console(s:command)", "Used to trigger console command on player. [br]Example: Console(:BANK)[br]");
        listLocal.Items.Add(newListItem9);
        ListItem newListItem10 = new ListItem("UseItem(i:glid)", "Used to trigger item use on player. [br]Example: UseItem(50)[br]");
        listLocal.Items.Add(newListItem10);
        ListItem newListItem11 = new ListItem("SetCountDownTimer(i:duration)", "Used to trigger count down link on local client, milliseconds. [br]Example: SetCountDownTimer(20000)[br]");
        listLocal.Items.Add(newListItem11);
        ListItem newListItem12 = new ListItem("LocalTeleport(f:posX,f:posY,f:posZ)", "Use to securly move somone on the current world map without respawn. [br]Example: LocalTeleport(30,45,2000)[br]");
        listLocal.Items.Add(newListItem12);
        ListItem newListItem13 = new ListItem("ToggleNPCAnimDir()", "Changes animation direction, like a non-loopable close and open state on a door. [br]Example: ToggleNPCAnimDir()[br]");
        listLocal.Items.Add(newListItem13);
        ListItem newListItem14 = new ListItem("SetNPCAnim(i:indexAnimation)", "used to change animation on a npc to a different animation. [br]Example: SetNPCAnim(6)[br]");
        listLocal.Items.Add(newListItem14);
        ListItem newListItem15 = new ListItem("OptimizeInventory()", "Used to arm a person with the best equipment. [br]Example: OptimizeInventory()[br]");
        listLocal.Items.Add(newListItem15);
        ListItem newListItem16 = new ListItem("SendText(s:message)", "Used to send a text message to specific player. [br]Example: SendText(Hello world)[br]");
        listLocal.Items.Add(newListItem16);
        ListItem newListItem17 = new ListItem("SpawnAI(i:aiCfg,s:exgName,f:posX,f:posY,f:posZ)", "Spawns a monster to a world. [br]Example: SpawnAI(5,nexxus.exg,1,100,50)[br]");
        listLocal.Items.Add(newListItem17);
        ListItem newListItem18 = new ListItem("AddPoi(f:radius,f:posX,f:posY,f:posZ)", "Adds a reference point into a world, like team arena start points. [br]Example: AddPoi(5.0,1,100,50)[br]");
        listLocal.Items.Add(newListItem18);
        ListItem newListItem19 = new ListItem("SetHP(i:healthPercentage)", "Sets a players health amount on zone in.  Example:  SetHP(100) sets to 100% health");
        listLocal.Items.Add(newListItem19);
        ListItem newListItem20 = new ListItem("ResetArenaStats(i:arenaType)", "Sets a players spawn point to inside an arena.  Example:  ResetArenaStats(");
        listLocal.Items.Add(newListItem20);
        ListItem newListItem21 = new ListItem("AddJournalEntry(s:npcName,s:description,s:keyStatement)", "Set a manual forced journal entry.  Example:  AddJournalEntry(Magnifisus,use jet pack to go to nexus,the portal in the air)");
        listLocal.Items.Add(newListItem21);
        ListItem newListItem22 = new ListItem("CloseJournalByNpc(s:npcName)", "Deletes journal entry by npc name and updates.  Example:  CloseJournalByNpc(Magnifisus)");
        listLocal.Items.Add(newListItem22);



        
    }
}

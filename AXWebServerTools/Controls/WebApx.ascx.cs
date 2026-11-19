using System;
using System.Data;
using System.Configuration;
using System.Collections;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using System.Data.SqlClient;

public partial class Controls_WebApx : System.Web.UI.UserControl
{
    DatabaseConnectionModule dbConnect = new DatabaseConnectionModule();
    ConnectionHelper m_conHelper = new ConnectionHelper();

    protected void Page_Load(object sender, EventArgs e)
    {
        if (m_itemList.Items.Count > 0)
            return;
        
        //in list exsiting items alphabetically-they will search by name, this is a unique here in this place
        SqlConnection connection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());

        InListItems(connection,-1);

        if (m_requireSkillList.Items.Count > 0)
            return;
        //skills
        string query = "SELECT skillName,skilltype FROM gm_Skills";
        SqlDataReader skReader = dbConnect.sqlQuery(query, connection);
        while (skReader.Read())
        {
            m_requireSkillList.Items.Add(skReader.GetSqlValue(0).ToString());
        }
        skReader.Close();

        //elements
        m_damageElementOvrd.Items.Add("NONE");
        query = "SELECT name FROM gm_Elements order by id";
        SqlDataReader elReader = dbConnect.sqlQuery(query, connection);
        while (elReader.Read())
        {
            m_availDefenseList.Items.Add(elReader.GetSqlValue(0).ToString());
            m_damageElementOvrd.Items.Add(elReader.GetSqlValue(0).ToString());
        }
        elReader.Close();
        m_availDefenseList.SelectedIndex = 0;
        m_damageElementOvrd.SelectedIndex = 0;

        //stats
        query = "SELECT statName FROM gm_Stats order by statid";
        SqlDataReader slReader = dbConnect.sqlQuery(query, connection);
        while (slReader.Read())
        {
            m_availStatsList.Items.Add(slReader.GetSqlValue(0).ToString());
        }
        slReader.Close();
        m_availStatsList.SelectedIndex = 0;

        //edb - m_raceSpecificList
        query = "SELECT entityName FROM gm_edb order by edb_id";
        m_raceSpecificList.Items.Add("ALL");
        SqlDataReader edbReader = dbConnect.sqlQuery(query, connection);
        while (edbReader.Read())
        {
            m_raceSpecificList.Items.Add(edbReader.GetSqlValue(0).ToString());
        }
        edbReader.Close();
        m_raceSpecificList.SelectedIndex = 0;



        if (!DoesIdsExist(connection))
            GenerateIds(15000, 15000, connection);

        //use types
        m_useTypeList.Items.Add("NONE");
        m_useTypeList.Items.Add("HEAL");
        m_useTypeList.Items.Add("AMMO");
        m_useTypeList.Items.Add("SKILL");
        m_useTypeList.Items.Add("OPEN MENU");
        m_useTypeList.Items.Add("COMBINE");
        m_useTypeList.Items.Add("HOUSE PLACEMENT");
        m_useTypeList.Items.Add("HOUSE INSTALLATION");
        m_useTypeList.Items.Add("SPAWN AI");
        m_useTypeList.Items.Add("POWERUP");
        m_useTypeList.Items.Add("MISC ITEM");
        m_useTypeList.Items.Add("FACTION ITEM");
        m_useTypeList.Items.Add("NEW SKILL USE");
        m_useTypeList.Items.Add("EMBLEM");
        m_useTypeList.SelectedIndex = 0;


        //search catagories
        m_catagorieSearch.Items.Add("NONE");
        m_catagorieSearch.Items.Add("AMMO");
        m_catagorieSearch.Items.Add("GUN");
        m_catagorieSearch.Items.Add("BLADE");
        m_catagorieSearch.Items.Add("RETICLE");
        m_catagorieSearch.Items.Add("BARREL");
        m_catagorieSearch.SelectedIndex = 0;

        connection.Close();
    }

    protected void addBtn_Click(object sender, EventArgs e)
    {
        m_errorOutput.Text = "";
        SqlConnection connection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
        //prevent blank name
        if (m_itemName.Text.Length <= 0)
        {
            m_errorOutput.Text = "Must fill in the itemName";
            return;
        }
        //prevent item name duplicate
        string query = "SELECT itemName FROM gm_DynamicItems where itemName = '" + dbConnect.SanitizeIn(m_itemName.Text) + "'";
        SqlDataReader dbReader = dbConnect.sqlQuery(query, connection);
        if(dbReader.Read())
        {//disallow duplicate names
            m_errorOutput.Text = "This name already exists";
            return;
        }
        dbReader.Close();

        query = "INSERT INTO gm_DynamicItems(       Glid," +
                                                    "ItemName," +
                                                    "MarketCost," +
                                                    "LocationID," +
                                                    "AnimationPref," +
                                                    "AnimationDefault," +
                                                    "HideFromInventory," +
                                                    "NonLootable," +
                                                    "NonTransferable," +
                                                    "DestroyOnUse," +
                                                    "Stackable," +
                                                    "CurrentCharges," +
                                                    "MaxCharges," +
                                                    "ChargeType," +
                                                    "ShotsPerFire," +
                                                    "AmmoRegenRate," +
                                                    "SecondsForRegen," +
                                                    "UseType," +
                                                    "requiredSkill," +
                                                    "RequiredSkillLevel," +
                                                    "AttachmentParse," +
                                                    "DefenseParse," +
                                                    "StatBonusParse," +
                                                    "RequiredDex," +
                                                    "RequiredStr," +
                                                    "RequiredWiz," +
                                                    "VisualRef," +
                                                    "HitPoints," +
                                                    "DestroyOnBreak," +
                                                    "FireQueIndexOvrd," +
                                                    "DamageOvrd," +
                                                    "DamageElementOvrd" +
                                                    ") Values(" +
                                                    "'$GLID'," +
                                                    "'$ItemName'," +
                                                    "'$MarketCost'," +
                                                    "'$LocationID'," +
                                                    "'$AnimationPref'," +
                                                    "'$AnimationDefault'," +
                                                    "'$HideFromInventory'," +
                                                    "'$NonLootable'," +
                                                    "'$NonTransferable'," +
                                                    "'$DestroyOnUse'," +
                                                    "'$Stackable'," +
                                                    "'$CurrentCharges'," +
                                                    "'$MaxCharges'," +
                                                    "'$ChargeType'," +
                                                    "'$ShotsPerFire'," +
                                                    "'$AmmoRegenRate'," +
                                                    "'$SecondsForRegen'," +
                                                    "'$UseType'," +
                                                    "'$requiredSkill'," +
                                                    "'$RequiredSkillLevel'," +
                                                    "'$AttachmentParse'," +
                                                    "'$DefenseParse'," +
                                                    "'$StatBonusParse'," +
                                                    "'$RequiredDex'," +
                                                    "'$RequiredStr'," +
                                                    "'$RequiredWiz'," +
                                                    "'$VisualRef'," +
                                                    "'$HitPoints'," +
                                                    "'$DestroyOnBreak'," +
                                                    "'$FireQueIndexOvrd'," +
                                                    "'$DamageOvrd'," +
                                                    "'$DamageElementOvrd'" +
                                                    ")";

        query = query.Replace("$DamageElementOvrd",     m_damageElementOvrd.SelectedIndex.ToString());
        query = query.Replace("$DamageOvrd",            dbConnect.SanitizeIn(m_damageOvrd.Text));
        query = query.Replace("$FireQueIndexOvrd",      dbConnect.SanitizeIn(m_fireQueIndexOvrd.Text));
        query = query.Replace("$GLID",                  GetOpenGLID(connection));
        query = query.Replace("$DestroyOnBreak",        GetStringFromBool(m_destroyOnBreak.Checked));
        query = query.Replace("$HitPoints",             dbConnect.SanitizeIn(m_hitPoints.Text));
        query = query.Replace("$VisualRef",             dbConnect.SanitizeIn(m_visualRef.Text));
        query = query.Replace("$RequiredDex",           dbConnect.SanitizeIn(m_requiredDex.Text));
        query = query.Replace("$RequiredStr",           dbConnect.SanitizeIn(m_requiredStr.Text));
        query = query.Replace("$RequiredWiz",           dbConnect.SanitizeIn(m_requiredWiz.Text));
        query = query.Replace("$ItemName",              dbConnect.SanitizeIn(m_itemName.Text));
        query = query.Replace("$MarketCost",            dbConnect.SanitizeIn(m_marketCost.Text));
        query = query.Replace("$LocationID",            dbConnect.SanitizeIn(m_locationID.Text));
        query = query.Replace("$AnimationPref",         dbConnect.SanitizeIn(m_animationPref.Text));
        query = query.Replace("$AnimationDefault",      dbConnect.SanitizeIn(m_animationDefault.Text));
        query = query.Replace("$HideFromInventory",     GetStringFromBool(m_hideInInventory.Checked));
        query = query.Replace("$NonLootable",           GetStringFromBool(m_nonDrop.Checked));
        query = query.Replace("$NonTransferable",       GetStringFromBool(m_nonTrade.Checked));
        query = query.Replace("$DestroyOnUse",          GetStringFromBool(m_destroyOnUse.Checked));
        query = query.Replace("$Stackable",             GetStringFromBool(m_stackable.Checked));
        query = query.Replace("$ChargeType",            dbConnect.SanitizeIn(m_chargeType.Text));
        query = query.Replace("$MaxCharges",            dbConnect.SanitizeIn(m_maxCharges.Text));
        query = query.Replace("$CurrentCharges",        dbConnect.SanitizeIn(m_currentCharges.Text));
        query = query.Replace("$ShotsPerFire",          dbConnect.SanitizeIn(m_shotsPerFire.Text));
        query = query.Replace("$AmmoRegenRate",         dbConnect.SanitizeIn(m_ammoRegenRate.Text));
        query = query.Replace("$SecondsForRegen",       dbConnect.SanitizeIn(m_secondsPerRegen.Text));
        query = query.Replace("$UseType",               dbConnect.SanitizeIn(m_useTypeList.SelectedValue));
        query = query.Replace("$requiredSkill",         dbConnect.SanitizeIn(m_requireSkillList.SelectedValue));
        query = query.Replace("$RequiredSkillLevel",    dbConnect.SanitizeIn(m_requiredSkillLevel.Text));
        query = query.Replace("$AttachmentParse",       dbConnect.SanitizeIn(GetAttachmentParseString()));
        query = query.Replace("$DefenseParse",          dbConnect.SanitizeIn(GetDefenseParseString()));
        query = query.Replace("$StatBonusParse",        dbConnect.SanitizeIn(GetStatParseString()));

        dbConnect.ExecuteQuery(query, connection);

        InListItems(connection,-2);

        connection.Close();
    }

    protected void InListItems(SqlConnection connection,int sel)
    {
        string selX = m_itemList.SelectedValue;
        m_itemList.Items.Clear();
        string query = "SELECT itemName FROM gm_DynamicItems order by ItemName";
        SqlDataReader dbReader = dbConnect.sqlQuery(query, connection);
        while (dbReader.Read())
        {
            m_itemList.Items.Add(dbReader.GetSqlValue(0).ToString());
        }
        dbReader.Close();

        if (sel > -1)
            m_itemList.SelectedIndex = sel;
        else if (sel == -2 && m_itemList.Items.Count > 0)
            m_itemList.SelectedIndex = m_itemList.Items.Count - 1;
        else if (sel == -3 && m_itemList.Items.Count > 0)
            m_itemList.SelectedValue = selX;
    }

    protected void InListItems(SqlConnection connection, string sel)
    {
        m_itemList.Items.Clear();
        string query = "SELECT itemName FROM gm_DynamicItems order by ItemName";
        SqlDataReader dbReader = dbConnect.sqlQuery(query, connection);
        while (dbReader.Read())
        {
            m_itemList.Items.Add(dbReader.GetSqlValue(0).ToString());
        }
        dbReader.Close();

        m_itemList.SelectedValue = sel;
    }

    protected string GetOpenGLID(SqlConnection connection)
    {
        SqlConnection connection2 = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());

        string query = "SELECT openID FROM GM_DYNAMICIDPOOL ORDER BY OPENID";
        string finalID = "0";

        SqlDataReader dbReader = dbConnect.sqlQuery(query, connection);
        while (dbReader.Read())
        {//loop through open till you delete one and use it, reason for continue loop is to handle a async delete
            finalID = dbReader.GetSqlValue(0).ToString();
            int rowsAffected = dbConnect.ExecuteQuery("DELETE FROM GM_DYNAMICIDPOOL WHERE OPENID = '" + finalID + "'", connection2);
            if (rowsAffected > 0)
                break;//successfully removed from pool
        }
        dbReader.Close();

        connection2.Close();
        return finalID;
    }

    protected int MakeGlidAvailable(string glid, SqlConnection hubConnection)
    {
        string query = "INSERT INTO GM_DYNAMICIDPOOL(OPENID) VALUES('" + glid + "')";
        return dbConnect.ExecuteQuery(query, hubConnection);
    }

    protected int MakeGlidAvailableByItemName(string itemName, SqlConnection hubConnection)
    {
        string glid = "";
        string query = "SELECT GLID FROM gm_DynamicItems where itemName = '" + itemName + "'";
        SqlDataReader dbReader = dbConnect.sqlQuery(query, hubConnection);
        if (dbReader.Read())
        {
            glid = dbReader.GetSqlValue(0).ToString();
        }
        dbReader.Close();
        if (glid.Length <= 0)
            return 0;

        return MakeGlidAvailable(glid, hubConnection);
    }

    protected void GenerateIds(int startID, int count, SqlConnection hubConnection)
    {
        for (int i = startID; i < (startID + count); i++)
        {
            MakeGlidAvailable(i.ToString(), hubConnection);
        }
    }

    protected bool DoesIdsExist(SqlConnection hubConnection)
    {
        string query = "SELECT * FROM GM_DYNAMICIDPOOL";
        SqlDataReader dbReader = dbConnect.sqlQuery(query, hubConnection);
        if (dbReader.Read())
        {
            dbReader.Close();
            return true;
        }
        dbReader.Close();
        return false;
    }

    protected void ParseToList(string data,ref ListBox list)
    {
        list.Items.Clear();
        string build = "";
        string name = "";
        int sw = 0;
        for (int i = 0; i < data.Length; i++)
        {
            if (data[i] == '|' && sw == 0)
            {
                name = build;
                build += data[i].ToString();
                sw = 1;
            }
            else if (data[i] == ';')
            {
                ListItem item = new ListItem(name, build);

                list.Items.Add(item);
                build = "";
                sw = 0;
            }
            else
                build += data[i].ToString();

        }
    }

    protected void ParseToListPerc(string data, ref ListBox list)
    {
        list.Items.Clear();
        string build = "";
        for (int i = 0; i < data.Length; i++)
        {
            if (data[i] == ';')
            {
                string displ = build;
                displ = displ.Replace("|", " ");
                displ += "%";
                ListItem item = new ListItem(displ, build);

                list.Items.Add(item);
                build = "";
            }
            else
                build += data[i].ToString();

        }
    }

    protected void ParseToListBasic(string data, ref ListBox list)
    {
        list.Items.Clear();
        string build = "";
        for (int i = 0; i < data.Length; i++)
        {
            if (data[i] == ';')
            {
                string displ = build;
                displ = displ.Replace("|", " ");
                ListItem item = new ListItem(displ, build);

                list.Items.Add(item);
                build = "";
            }
            else
                build += data[i].ToString();

        }
    }

    protected void UpdateSelectedItemInfo(string itemName)
    {
        SqlConnection connection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
        string query = "SELECT ItemName,MarketCost,LocationID,AnimationPref,AnimationDefault,HideFromInventory," +//5
                              "NonLootable,NonTransferable,DestroyOnUse,Stackable,CurrentCharges," +//10
                              "MaxCharges,ChargeType,ShotsPerFire,AmmoRegenRate,SecondsForRegen," +//15
                              "UseType,requiredSkill,RequiredSkillLevel,AttachmentParse,DefenseParse,StatBonusParse,Glid," +//22
                              "requiredDex,requiredStr,requiredWiz,VisualRef,HitPoints,DestroyOnBreak,FireQueIndexOvrd,DamageOvrd,DamageElementOvrd" + //31
                              " FROM gm_DynamicItems where ItemName = '" + itemName + "'";
        SqlDataReader dbReader = dbConnect.sqlQuery(query, connection);
        if (dbReader.Read())
        {//disallow duplicate names
            
            m_itemName.Text                     = dbConnect.SanitizeOut(dbReader.GetSqlValue(0).ToString());
            m_marketCost.Text                   = dbConnect.SanitizeOut(dbReader.GetSqlValue(1).ToString());
            m_locationID.Text                   = dbConnect.SanitizeOut(dbReader.GetSqlValue(2).ToString());
            m_animationPref.Text                = dbConnect.SanitizeOut(dbReader.GetSqlValue(3).ToString());
            m_animationDefault.Text             = dbConnect.SanitizeOut(dbReader.GetSqlValue(4).ToString());
            m_hideInInventory.Checked           = StringToBool(dbReader.GetSqlValue(5).ToString());
            m_nonDrop.Checked                   = StringToBool(dbReader.GetSqlValue(6).ToString());
            m_nonTrade.Checked                  = StringToBool(dbReader.GetSqlValue(7).ToString());
            m_destroyOnUse.Checked              = StringToBool(dbReader.GetSqlValue(8).ToString());
            m_stackable.Checked                 = StringToBool(dbReader.GetSqlValue(9).ToString());
            m_currentCharges.Text               = dbConnect.SanitizeOut(dbReader.GetSqlValue(10).ToString());
            m_maxCharges.Text                   = dbConnect.SanitizeOut(dbReader.GetSqlValue(11).ToString());
            m_chargeType.Text                   = dbConnect.SanitizeOut(dbReader.GetSqlValue(12).ToString());
            m_shotsPerFire.Text                 = dbConnect.SanitizeOut(dbReader.GetSqlValue(13).ToString());
            m_ammoRegenRate.Text                = dbConnect.SanitizeOut(dbReader.GetSqlValue(14).ToString());
            m_secondsPerRegen.Text              = dbConnect.SanitizeOut(dbReader.GetSqlValue(15).ToString());
            m_useTypeList.SelectedValue         = dbConnect.SanitizeOut(dbReader.GetSqlValue(16).ToString());
            m_requireSkillList.SelectedValue    = dbConnect.SanitizeOut(dbReader.GetSqlValue(17).ToString());
            m_requiredSkillLevel.Text           = dbConnect.SanitizeOut(dbReader.GetSqlValue(18).ToString());
            m_glid.Text                         = dbConnect.SanitizeOut(dbReader.GetSqlValue(22).ToString());
            m_requiredDex.Text                  = dbConnect.SanitizeOut(dbReader.GetSqlValue(23).ToString());
            m_requiredStr.Text                  = dbConnect.SanitizeOut(dbReader.GetSqlValue(24).ToString());
            m_requiredWiz.Text                  = dbConnect.SanitizeOut(dbReader.GetSqlValue(25).ToString());
            m_visualRef.Text                    = dbConnect.SanitizeOut(dbReader.GetSqlValue(26).ToString());
            m_hitPoints.Text                    = dbConnect.SanitizeOut(dbReader.GetSqlValue(27).ToString());
            m_destroyOnBreak.Checked            = StringToBool(dbReader.GetSqlValue(28).ToString());

            m_fireQueIndexOvrd.Text             = dbConnect.SanitizeOut(dbReader.GetSqlValue(29).ToString());
            m_damageOvrd.Text                   = dbConnect.SanitizeOut(dbReader.GetSqlValue(30).ToString());
            m_damageElementOvrd.SelectedIndex   = Convert.ToInt32(dbReader.GetSqlValue(31).ToString());

            ParseToList(dbConnect.SanitizeOut(dbReader.GetSqlValue(19).ToString()),ref m_attachments);
            ParseToListPerc(dbConnect.SanitizeOut(dbReader.GetSqlValue(20).ToString()), ref m_defenseAttributesList);
            ParseToListBasic(dbConnect.SanitizeOut(dbReader.GetSqlValue(21).ToString()), ref m_statBonusList);


        }
        dbReader.Close();
        connection.Close();

    }

    protected bool StringToBool(string stBool)
    {
        if (stBool == "1")
            return true;
        else
            return false;

    }

    protected string GetStatParseString()
    {
        string build = "";
        for (int i = 0; i < m_statBonusList.Items.Count; i++)
        {
            build += m_statBonusList.Items[i].Value + ";";
        }
        return build;
    }

    protected string GetDefenseParseString()
    {
        string build = "";
        for (int i = 0; i < m_defenseAttributesList.Items.Count; i++)
        {
            build += m_defenseAttributesList.Items[i].Value + ";";
        }
        return build;
    }

    protected string GetAttachmentParseString()
    {
        string build = "";
        for (int i = 0; i < m_attachments.Items.Count; i++)
        {
            build += m_attachments.Items[i].Value + ";";
        }
        return build;
    }

    protected string GetStringFromBool(bool booltest)
    {
        if (booltest == true)
            return "1";
        else
            return "0";
    }

    protected void replaceBtn_Click(object sender, EventArgs e)
    {
        SqlConnection connection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
        string query = "UPDATE gm_DynamicItems SET MarketCost = '$MarketCost'," +
                                                    "LocationID = '$LocationID'," +
                                                    "AnimationPref = '$AnimationPref'," +
                                                    "AnimationDefault = '$AnimationDefault'," +
                                                    "HideFromInventory = '$HideFromInventory'," +
                                                    "NonLootable = '$NonLootable'," +
                                                    "NonTransferable = '$NonTransferable'," +
                                                    "DestroyOnUse = '$DestroyOnUse'," +
                                                    "Stackable = '$Stackable'," +
                                                    "CurrentCharges = '$CurrentCharges'," +
                                                    "MaxCharges = '$MaxCharges'," +
                                                    "ChargeType = '$ChargeType'," +
                                                    "ShotsPerFire = '$ShotsPerFire'," +
                                                    "AmmoRegenRate = '$AmmoRegenRate'," +
                                                    "SecondsForRegen = '$SecondsForRegen'," +
                                                    "UseType = '$UseType'," +
                                                    "requiredSkill = '$requiredSkill'," +
                                                    "RequiredSkillLevel = '$RequiredSkillLevel'," +
                                                    "AttachmentParse = '$AttachmentParse'," +
                                                    "DefenseParse = '$DefenseParse'," +
                                                    "StatBonusParse = '$StatBonusParse'," +
                                                    "requiredDex = '$RequiredDex'," +
                                                    "requiredStr = '$RequiredStr'," +
                                                    "requiredWiz = '$RequiredWiz'," +
                                                    "ItemName = '$ItemName'," +
                                                    "HitPoints = '$HitPoints'," +
                                                    "VisualRef = '$VisualRef'," +
                                                    "FireQueIndexOvrd = '$FireQueIndexOvrd'," +
                                                    "DamageOvrd = '$DamageOvrd'," +
                                                    "DamageElementOvrd = '$DamageElementOvrd'," +
                                                    "DestroyOnBreak = '$DestroyOnBreak'" +
                                                    " WHERE GLID = '$GLID'";


        
        query = query.Replace("$FireQueIndexOvrd", dbConnect.SanitizeIn(m_fireQueIndexOvrd.Text));
        query = query.Replace("$DamageOvrd", dbConnect.SanitizeIn(m_damageOvrd.Text));
        query = query.Replace("$DamageElementOvrd", m_damageElementOvrd.SelectedIndex.ToString());

        query = query.Replace("$DestroyOnBreak", GetStringFromBool(m_destroyOnBreak.Checked));
        query = query.Replace("$HitPoints", dbConnect.SanitizeIn(m_hitPoints.Text));
        query = query.Replace("$MarketCost", dbConnect.SanitizeIn(m_marketCost.Text));
        query = query.Replace("$LocationID", dbConnect.SanitizeIn(m_locationID.Text));
        query = query.Replace("$AnimationPref", dbConnect.SanitizeIn(m_animationPref.Text));
        query = query.Replace("$AnimationDefault", dbConnect.SanitizeIn(m_animationDefault.Text));
        query = query.Replace("$HideFromInventory", GetStringFromBool(m_hideInInventory.Checked));
        query = query.Replace("$NonLootable", GetStringFromBool(m_nonDrop.Checked));
        query = query.Replace("$NonTransferable", GetStringFromBool(m_nonTrade.Checked));
        query = query.Replace("$DestroyOnUse", GetStringFromBool(m_destroyOnUse.Checked));
        query = query.Replace("$Stackable", GetStringFromBool(m_stackable.Checked));
        query = query.Replace("$CurrentCharges", dbConnect.SanitizeIn(m_chargeType.Text));
        query = query.Replace("$MaxCharges", dbConnect.SanitizeIn(m_maxCharges.Text));
        query = query.Replace("$ChargeType", dbConnect.SanitizeIn(m_currentCharges.Text));
        query = query.Replace("$ShotsPerFire", dbConnect.SanitizeIn(m_shotsPerFire.Text));
        query = query.Replace("$AmmoRegenRate", dbConnect.SanitizeIn(m_ammoRegenRate.Text));
        query = query.Replace("$SecondsForRegen", dbConnect.SanitizeIn(m_secondsPerRegen.Text));
        query = query.Replace("$UseType", dbConnect.SanitizeIn(m_useTypeList.SelectedValue));
        query = query.Replace("$requiredSkill", dbConnect.SanitizeIn(m_requireSkillList.SelectedValue));
        query = query.Replace("$RequiredSkillLevel", dbConnect.SanitizeIn(m_requiredSkillLevel.Text));
        query = query.Replace("$AttachmentParse", dbConnect.SanitizeIn(GetAttachmentParseString()));
        query = query.Replace("$DefenseParse", dbConnect.SanitizeIn(GetDefenseParseString()));
        query = query.Replace("$StatBonusParse", dbConnect.SanitizeIn(GetStatParseString()));
        query = query.Replace("$ItemName", dbConnect.SanitizeIn(m_itemName.Text));
        query = query.Replace("$GLID", dbConnect.SanitizeIn(m_glid.Text));
        query = query.Replace("$RequiredDex", dbConnect.SanitizeIn(m_requiredDex.Text));
        query = query.Replace("$RequiredStr", dbConnect.SanitizeIn(m_requiredStr.Text));
        query = query.Replace("$RequiredWiz", dbConnect.SanitizeIn(m_requiredWiz.Text));
        query = query.Replace("$VisualRef", dbConnect.SanitizeIn(m_visualRef.Text));

        int recordChange = dbConnect.ExecuteQuery(query, connection);

        InListItems(connection, m_itemName.Text);

                                                  
    }
    protected void DeleteBtn_Click(object sender, EventArgs e)
    {
        SqlConnection connection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());

        MakeGlidAvailableByItemName(m_itemList.SelectedValue, connection);
        string query = "DELETE from gm_DynamicItems where itemName = '" + m_itemList.SelectedValue +"'";
        dbConnect.ExecuteQuery(query, connection);
        m_itemList.Items.RemoveAt(m_itemList.SelectedIndex);
        connection.Close();
    }
    protected void AddAttachmentBtn_Click(object sender, EventArgs e)
    {
        m_errorOutput.Text = "";
        if (m_attachments.Items.Count > 10)
        {//max visual limit HIT
            m_errorOutput.Text = "Cannot add anymore visuals to this item";
            return;
        }

        if (m_availVisualList.SelectedValue.Length < 1)
        {
            m_errorOutput.Text = "Invalid Visual Selection";
            return;
        }

        int selection = m_raceSpecificList.SelectedIndex - 1;

        ListItem item = new ListItem(m_availVisualList.SelectedValue, m_availVisualList.SelectedValue + "|" + m_fireQue_DPathOvrd.Text + "|" + selection.ToString() + "|" + m_dimIndex.Text + "|" + m_dimChange.Text + "|");
        m_attachments.Items.Add(item);

        //m_attachments.Items.Add(m_availVisualList.SelectedValue + "|" + m_fireQue_DPathOvrd.Text + "|" + selection.ToString() + "|" + m_dimIndex.Text + "|" + m_dimChange.Text + "|");
    }
    protected void deleteAttachmentBtn_Click(object sender, EventArgs e)
    {
        m_attachments.Items.RemoveAt(m_attachments.SelectedIndex);
    }

    protected void SearchVisualBtn_Click(object sender, EventArgs e)
    {
        m_availVisualList.Items.Clear();
        SqlConnection connection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
        string query = "SELECT visualName FROM gm_itemVisualDump where visualName like '%" + m_visualSearchEdit.Text + "%'";
        SqlDataReader dbReader = dbConnect.sqlQuery(query, connection);
        while (dbReader.Read())
        {
            m_availVisualList.Items.Add(dbReader.GetSqlValue(0).ToString());
        }
        dbReader.Close();
        connection.Close();
    }

    protected void addDefenseBtn_Click(object sender, EventArgs e)
    {
        m_errorOutput.Text = "";
        if (m_defenseAttributesList.Items.Count > 10)
        {//max visual limit HIT
            m_errorOutput.Text = "Cannot add anymore defenses to this item";
            return;
        }

        if (m_defensePercentage.Text.Length < 1)
        {
            m_errorOutput.Text = "must have a Defense value set";
            return;
        }

        if (m_availDefenseList.SelectedValue.Length < 1)
        {
            m_errorOutput.Text = "Invalid Defense Selection";
            return;
        }

        if (DoesSectionalValueExist(ref m_defenseAttributesList, m_availDefenseList.SelectedValue))
        {
            m_errorOutput.Text = "Defense is already in use.";
            return;
        }

        ListItem item = new ListItem(m_availDefenseList.SelectedValue + " " + m_defensePercentage.Text + "%", m_availDefenseList.SelectedValue + "|" + m_defensePercentage.Text);

        m_defenseAttributesList.Items.Add(item);
    }

    protected void ReplaceAttachmentBtn_Click(object sender, EventArgs e)
    {
        m_errorOutput.Text = "";
        if (m_defenseAttributesList.Items.Count > 10)
        {//max visual limit HIT
            m_errorOutput.Text = "Cannot add anymore defenses to this item";
            return;
        }

        if (m_fireQue_DPathOvrd.Text.Length < 1)
        {
            m_errorOutput.Text = "must have a Damage Path-Fire Que value set";
            return;
        }

        if (m_availVisualList.SelectedValue.Length < 1)
        {
            m_errorOutput.Text = "Invalid available visual Selection";
            return;
        }

        int curSel = m_attachments.SelectedIndex;
        int selection = m_raceSpecificList.SelectedIndex - 1;

        string valText = m_availVisualList.SelectedValue + "|" + m_fireQue_DPathOvrd.Text + "|" + selection.ToString() + "|" + m_dimIndex.Text + "|" + m_dimChange.Text + "|";

        m_attachments.Items[m_attachments.SelectedIndex].Text = m_availVisualList.SelectedValue;
        m_attachments.Items[m_attachments.SelectedIndex].Value = valText;

        m_attachments.SelectedIndex = curSel;

    }

    protected void DeleteDefenseBtn_Click(object sender, EventArgs e)
    {
        m_defenseAttributesList.Items.RemoveAt(m_defenseAttributesList.SelectedIndex);
    }


    protected bool DoesSectionalValueExist(ref ListBox list, string compareX)
    {
        int cnt = list.Items.Count;
        for (int i = 0; i < cnt; i++)
        {
            if (string.Compare(list.Items[i].Value, 0, compareX, 0, compareX.Length) == 0)
                return true;
        }

        return false;
    }

    protected void addStatBtn_Click(object sender, EventArgs e)
    {
        m_errorOutput.Text = "";
        if (m_statBonusList.Items.Count > 10)
        {//max visual limit HIT
            m_errorOutput.Text = "Cannot add anymore stats to this item";
            return;
        }

        if (m_statValue.Text.Length < 1)
        {
            m_errorOutput.Text = "must have a stat value set";
            return;
        }

        if (m_availStatsList.SelectedValue.Length < 1)
        {
            m_errorOutput.Text = "Invalid stat Selection";
            return;
        }

        if (DoesSectionalValueExist(ref m_statBonusList, m_availStatsList.SelectedValue))
        {
            m_errorOutput.Text = "Stat is already in use.";
            return;
        }

        ListItem item = new ListItem(m_availStatsList.SelectedValue + " " + m_statValue.Text, m_availStatsList.SelectedValue + "|" + m_statValue.Text);

        m_statBonusList.Items.Add(item);
    }

    protected void DeleteStatBtn_Click(object sender, EventArgs e)
    {
        m_statBonusList.Items.RemoveAt(m_statBonusList.SelectedIndex);
    }


    protected void m_itemList_SelectedIndexChanged(object sender, EventArgs e)
    {
        UpdateSelectedItemInfo(m_itemList.SelectedValue);
    }
    protected void m_updateLive_Click(object sender, EventArgs e)
    {
        string repRequester = (string)Session["user"];
        string serverName = Session["ServerName"].ToString();
        ConnectionHelper conHlper = new ConnectionHelper();
        SqlConnection sqlAXServerConnection = conHlper.GetServerConnection(serverName);

        conHlper.SendCommandToServer(
            serverName,
            "27",
            m_glid.Text.ToString(),
            m_glid.Text.ToString(),
            "",
            "",
            "",
            repRequester,
            sqlAXServerConnection);

        sqlAXServerConnection.Close();
    }

    protected void m_catagorieSearch_SelectedIndexChanged(object sender, EventArgs e)
    {
        m_availVisualList.Items.Clear();
        SqlConnection connection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
        string query = "SELECT visualName FROM gm_itemVisualDump where visualName like '%" + m_catagorieSearch.SelectedValue.ToString() + "%'";
        SqlDataReader dbReader = dbConnect.sqlQuery(query, connection);
        while (dbReader.Read())
        {
            m_availVisualList.Items.Add(dbReader.GetSqlValue(0).ToString());
        }
        dbReader.Close();
        connection.Close();
    }
    protected void m_attachments_SelectedIndexChanged(object sender, EventArgs e)
    {
        string  parse           = m_attachments.SelectedValue;
        int     length          = parse.Length;
        int     parseSect       = 0;

        string build = "";

        for (int loop = 0; loop < length; loop++)
        {
            if (parse[loop] == '|')
            {
                //int selection = m_raceSpecificList.SelectedIndex - 1;
                //m_attachments.Items.Add(m_availVisualList.SelectedValue + "|" + m_fireQue_DPathOvrd.Text + "|" + selection.ToString() + "|" + m_dimIndex.Text + "|" + m_dimChange.Text);
                switch(parseSect)
                {
                    case 0:
                        m_availVisualList.Items.Clear();

                        m_availVisualList.Items.Add(build);
                        m_availVisualList.SelectedValue     = build;
                    
                    break;
                    case 1:
                        m_fireQue_DPathOvrd.Text            = build;
                    break;
                    case 2:
                        m_raceSpecificList.SelectedIndex    = Convert.ToInt32(build)+1;
                    break;
                    case 3:
                        m_dimIndex.Text                     = build;
                    break;
                    case 4:
                        m_dimChange.Text                    = build;
                    break;
                }
                parseSect++;
                build = "";
            }
            else
                build += parse[loop];
        }
    }
}

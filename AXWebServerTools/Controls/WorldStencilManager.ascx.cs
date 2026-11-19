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
using System.Threading;

public partial class Controls_WorldStencilManager : System.Web.UI.UserControl
{
    DatabaseConnectionModule    m_dbConnectionPtr   = new DatabaseConnectionModule();
    ConnectionHelper            m_conHelper         = new ConnectionHelper();
    common                      m_common            = new common();

    protected void Page_Load(object sender, EventArgs e)
    {
        AuthenticationModule m_authPtr = new AuthenticationModule();
        m_authPtr.AuthenticateUser("Main.aspx", "DEVELOPER");


        if (m_stencilWorldsList.Items.Count == 0)
        {
            string serverName = (string)Session["ServerName"];
            SqlConnection connectionToServerDB = m_conHelper.GetServerHUBConnection(serverName);//aquire target server db
            //SqlConnection connectionToServerDB = m_dbConnectionPtr.CreateNativeConnectionObject();
            RefreshStencilWorlds(connectionToServerDB,-1);

            if (m_worldRuleDropList.Items.Count == 0)
                m_common.InListWorldRules(ref m_worldRuleDropList, connectionToServerDB);


            connectionToServerDB.Close();
        }

        
    }

    private void RefreshStencilWorlds(SqlConnection connectionToServerDB,int sel)
    {
        m_stencilWorldsList.Items.Clear();
        SqlDataReader dbReader = m_dbConnectionPtr.sqlQuery("Select channel,exgname,aiExgname,maxConnections from sysWorldStencils order by channel", connectionToServerDB);
        int cntTrace = 0;
        string wldIdStore = "0";
        while (dbReader.Read())
        {
            m_stencilWorldsList.Items.Add("Channel:" + dbReader.GetSqlValue(0).ToString() + " \t\t" + dbReader.GetSqlValue(1).ToString());
            if(sel == cntTrace)
               wldIdStore = dbReader.GetSqlValue(0).ToString();
            cntTrace++;
        }
        dbReader.Close();
        if (sel > -1)
        {
            m_stencilWorldsList.SelectedIndex = sel;
            RefreshWorldRules(connectionToServerDB, -1, wldIdStore);
        }

        if (sel == -2)
            m_stencilWorldsList.SelectedIndex = cntTrace-1;
    }

    private void RefreshWorldRules(SqlConnection connectionToServerDB, int sel, string wldID)
    {
        m_worldRuleList.Items.Clear();
        SqlDataReader dbReader = m_dbConnectionPtr.sqlQuery("Select pKey,ruleName,ruleID,wldID,param1 from sysWorldRules where WLDID = '" + wldID + "' order by pKey", connectionToServerDB);
        int cntTrace = 0;
        while (dbReader.Read())
        {
            m_worldRuleList.Items.Add(dbReader.GetSqlValue(1).ToString());
            cntTrace++;
        }
        dbReader.Close();
        if (sel > -1)
            m_worldRuleList.SelectedIndex = sel;

        if (sel == -2)
            m_worldRuleList.SelectedIndex = cntTrace - 1;
    }



    protected void AddBtn_Click(object sender, EventArgs e)
    {
        m_errorOutput.Text = "";
        string serverName = (string)Session["ServerName"];
        SqlConnection connectionToServerDB = m_conHelper.GetServerHUBConnection(serverName);//aquire target server db
        //check to see if channel already exists
        string channelCheck = "SELECT * FROM sysWorldStencils where channel = '" + m_worldChannel.Text + "'";
        SqlDataReader dbReader = m_dbConnectionPtr.sqlQuery(channelCheck, connectionToServerDB);
        if (dbReader.Read())
        {//channel already exists
            m_errorOutput.Text = "Channel already exists, please select a different channel id";
            dbReader.Close();
            connectionToServerDB.Close();
            return;
        }
        dbReader.Close();
        //add
        string qry = "insert into sysWorldStencils (channel,exgname,aiExgname,maxConnections) Values('" + m_worldChannel.Text + "','" + m_worldEXG.Text + "','" + m_aiEXG.Text + "','" + m_maxConnections.Text + "')";
        m_dbConnectionPtr.ExecuteQuery(qry, connectionToServerDB);
        RefreshStencilWorlds(connectionToServerDB,-2);
        connectionToServerDB.Close();
    }
    protected void m_stencilWorldsList_SelectedIndexChanged(object sender, EventArgs e)
    {
        string serverName = (string)Session["ServerName"];
        SqlConnection connectionToServerDB = m_conHelper.GetServerHUBConnection(serverName);//aquire target server db
        SqlDataReader dbReader = m_dbConnectionPtr.sqlQuery("Select channel,exgname,aiExgname,maxConnections from sysWorldStencils order by channel", connectionToServerDB);
        int trace = 0;
        while (dbReader.Read())
        {
            if (m_stencilWorldsList.SelectedIndex == trace)
            {
                m_worldChannel.Text     = dbReader.GetSqlValue(0).ToString();
                m_worldEXG.Text         = dbReader.GetSqlValue(1).ToString();
                m_aiEXG.Text            = dbReader.GetSqlValue(2).ToString();
                m_maxConnections.Text   = dbReader.GetSqlValue(3).ToString();                
                break;
            }
            trace++;
        }
        dbReader.Close();
        RefreshWorldRules(connectionToServerDB, -1, m_worldChannel.Text);
        connectionToServerDB.Close();
        //
    }
    protected void m_worldRuleDropList_SelectedIndexChanged(object sender, EventArgs e)
    {

        string serverName = (string)Session["ServerName"];
        SqlConnection connectionToServerDB = m_conHelper.GetServerHUBConnection(serverName);//aquire target server db

        SqlDataReader readerPtr = m_dbConnectionPtr.sqlQuery("Select paramDesc from gm_worldRules where rulename = '" + m_worldRuleDropList.SelectedValue.ToString() + "'", connectionToServerDB);
        if (readerPtr.Read())
        {
            m_param1Desc.Text = readerPtr.GetSqlValue(0).ToString();
        }
        readerPtr.Close();

        connectionToServerDB.Close();
    }
    protected void ReplaceBtn_Click(object sender, EventArgs e)
    {
        m_errorOutput.Text = "";
        string serverName = (string)Session["ServerName"];
        SqlConnection connectionToServerDB = m_conHelper.GetServerHUBConnection(serverName);//aquire target server db

        string updateSQL = "UPDATE sysWorldStencils set exgname = '" + m_worldEXG.Text + "',aiExgname = '" + m_aiEXG.Text + "',maxConnections = '" + m_maxConnections.Text + "' where channel = '" + m_worldChannel.Text + "'";
        m_dbConnectionPtr.ExecuteQuery(updateSQL, connectionToServerDB);
        RefreshStencilWorlds(connectionToServerDB, m_stencilWorldsList.SelectedIndex);
        connectionToServerDB.Close();
    }
    protected void DeleteBtn_Click(object sender, EventArgs e)
    {
        m_errorOutput.Text = "";
        if(m_stencilWorldsList.SelectedIndex < 0)
        {
           m_errorOutput.Text = "Must select a world stencil";
           return;
        }

        string serverName = (string)Session["ServerName"];
        SqlConnection connectionToServerDB = m_conHelper.GetServerHUBConnection(serverName);//aquire target server db
        SqlDataReader dbReader = m_dbConnectionPtr.sqlQuery("Select channel,exgname,aiExgname,maxConnections from sysWorldStencils order by channel", connectionToServerDB);

        int trace = 0;
        string channelDelete = "";
        while (dbReader.Read())
        {
            if (m_stencilWorldsList.SelectedIndex == trace)
            {
                channelDelete = dbReader.GetSqlValue(0).ToString();
                break;
            }
            trace++;
        }
        dbReader.Close();

        if (channelDelete.Length > 0)
        {
            string sDelete = "DELETE FROM sysWorldStencils where channel = '" + channelDelete + "'";
            string rDelete = "DELETE FROM sysWorldRules where wldId = '" + channelDelete + "'";
            m_dbConnectionPtr.ExecuteQuery(sDelete, connectionToServerDB);
            m_dbConnectionPtr.ExecuteQuery(rDelete, connectionToServerDB);
            RefreshStencilWorlds(connectionToServerDB, trace - 1);
        }


        connectionToServerDB.Close();
    }
    protected void AddWorldRuleBtn_Click(object sender, EventArgs e)
    {
        string serverName = (string)Session["ServerName"];
        SqlConnection connectionToServerDB = m_conHelper.GetServerHUBConnection(serverName);//aquire target server db
        SqlDataReader dbReader = m_dbConnectionPtr.sqlQuery("Select channel,exgname,aiExgname,maxConnections from sysWorldStencils order by channel", connectionToServerDB);
        int trace = 0;
        string selectedChannel = "";
        while (dbReader.Read())
        {
            if (m_stencilWorldsList.SelectedIndex == trace)
            {
                selectedChannel = dbReader.GetSqlValue(0).ToString();
                break;
            }
            trace++;
        }
        dbReader.Close();

        if (selectedChannel.Length > 0)
        {
            string addqry = "insert into sysWorldRules (wldID,ruleName,ruleID,param1) Values('" + selectedChannel + "','" + m_worldRuleDropList.Text + "','" + m_worldRuleDropList.SelectedIndex.ToString() + "','" + m_param1.Text + "')";
            m_dbConnectionPtr.ExecuteQuery(addqry, connectionToServerDB);
            RefreshWorldRules(connectionToServerDB, -2, selectedChannel);
        }
        connectionToServerDB.Close();
    }
    protected void DeleteWorldRuleBtn_Click(object sender, EventArgs e)
    {
        string serverName = (string)Session["ServerName"];
        SqlConnection connectionToServerDB = m_conHelper.GetServerHUBConnection(serverName);//aquire target server db
        SqlDataReader dbReader = m_dbConnectionPtr.sqlQuery("Select channel,exgname,aiExgname,maxConnections from sysWorldStencils order by channel", connectionToServerDB);
        int trace = 0;
        string selectedChannel = "";
        while (dbReader.Read())
        {
            if (m_stencilWorldsList.SelectedIndex == trace)
            {
                selectedChannel = dbReader.GetSqlValue(0).ToString();
                break;
            }
            trace++;
        }
        dbReader.Close();

        if (selectedChannel.Length > 0)
        {
            string addqry = "DELETE FROM sysWorldRules WHERE wldID = " + selectedChannel + " AND ruleName= '" + m_worldRuleList.SelectedValue.ToString() + "'";
            m_dbConnectionPtr.ExecuteQuery(addqry, connectionToServerDB);
            RefreshWorldRules(connectionToServerDB, m_worldRuleList.SelectedIndex - 1, selectedChannel);
        }
        connectionToServerDB.Close();
    }
    protected void m_worldRuleList_SelectedIndexChanged(object sender, EventArgs e)
    {
        string serverName = (string)Session["ServerName"];
        SqlConnection connectionToServerDB = m_conHelper.GetServerHUBConnection(serverName);//aquire target server db
        SqlDataReader dbReader = m_dbConnectionPtr.sqlQuery("Select channel,exgname,aiExgname,maxConnections from sysWorldStencils order by channel", connectionToServerDB);
        int trace = 0;
        string selectedChannel = "";
        while (dbReader.Read())
        {
            if (m_stencilWorldsList.SelectedIndex == trace)
            {
                selectedChannel = dbReader.GetSqlValue(0).ToString();
                break;
            }
            trace++;
        }
        dbReader.Close();

        dbReader = m_dbConnectionPtr.sqlQuery("Select param1,ruleID from sysWorldRules where ruleName = '" + m_worldRuleList.SelectedValue.ToString() + "' AND wldID = '" + selectedChannel.ToString() + "'", connectionToServerDB);
        if(dbReader.Read())
        {
            m_param1.Text = dbReader.GetSqlValue(0).ToString();
            m_worldRuleDropList.SelectedValue = m_worldRuleList.SelectedValue.ToString();
        }
        dbReader.Close();
        connectionToServerDB.Close();
    }
    protected void replaceWorldRulesBtn_Click(object sender, EventArgs e)
    {
        string serverName = (string)Session["ServerName"];
        SqlConnection connectionToServerDB = m_conHelper.GetServerHUBConnection(serverName);//aquire target server db
        SqlDataReader dbReader = m_dbConnectionPtr.sqlQuery("Select channel,exgname,aiExgname,maxConnections from sysWorldStencils order by channel", connectionToServerDB);
        int trace = 0;
        string selectedChannel = "";
        while (dbReader.Read())
        {
            if (m_stencilWorldsList.SelectedIndex == trace)
            {
                selectedChannel = dbReader.GetSqlValue(0).ToString();
                break;
            }
            trace++;
        }
        dbReader.Close();

        if (selectedChannel.Length > 0)
        {
            string addqry = "UPDATE sysWorldRules set param1 = '" + m_param1.Text + "', ruleName = '" + m_worldRuleDropList.SelectedValue.ToString() + "' WHERE wldID= '" + selectedChannel.ToString() + "' AND rulename = '" + m_worldRuleList.SelectedValue.ToString() + "'";
            m_dbConnectionPtr.ExecuteQuery(addqry, connectionToServerDB);
            RefreshWorldRules(connectionToServerDB, m_worldRuleList.SelectedIndex, selectedChannel);
        }
        connectionToServerDB.Close();
    }
}

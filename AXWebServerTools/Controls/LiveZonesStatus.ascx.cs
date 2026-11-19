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

public partial class LiveZonesStatus : System.Web.UI.UserControl
{

    DatabaseConnectionModule dbConModule = new DatabaseConnectionModule();
    ConnectionHelper m_conHelper = new ConnectionHelper();

    protected void Page_Load(object sender, EventArgs e)
    {
        AuthenticationModule m_authPtr = new AuthenticationModule();
        m_authPtr.AuthenticateUser("Main.aspx", "STATUSADMIN");


        SqlConnection connection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
        SqlDataReader stR = dbConModule.sqlQuery("select sysLiveZoneDB.NODEID,sysLiveZoneDB.ZONENAME,sysLiveZoneDB.CHANNEL,sysLiveZoneDB.CUR_CONNECTIONS from sysLiveZoneDB,sysLiveServerNodes where sysLiveServerNodes.nodeid = sysLiveZoneDB.nodeid AND DATEADD(SECOND,25,CHECKINTIME) > GETDATE() ORDER BY ZONENAME", connection);
        m_zonesStatusList.Items.Clear();
        string build;
        int trace = 0;
        while (stR.Read())
        {
            build = "NODEID:" + stR.GetSqlValue(0).ToString() + "\t EXG:" + stR.GetSqlValue(1).ToString() + "\t CHANNEL:" + stR.GetSqlValue(2).ToString() + "\t CONNECTIONS:" + stR.GetSqlValue(3).ToString();
            m_zonesStatusList.Items.Add(build);
            trace++;
        }
        stR.Close();
        if(trace == 0)
            m_zonesStatusList.Items.Add("None");


        m_onlineNodes.Text = "";
        bool nodesOnline = false;
        SqlDataReader nodeReader = dbConModule.sqlQuery("Select NODEID,CONNECTIONCOUNT,NODEIP FROM SYSLIVESERVERNODES WHERE DATEADD(SECOND,25,CHECKINTIME) > GETDATE()", connection);
        m_onlineNodes.Text = "Cluster Nodes Online<br>";
        while (nodeReader.Read())
        {
            m_onlineNodes.Text += "NodeID:" + nodeReader.GetValue(0).ToString() + " Ip:" + nodeReader.GetValue(2).ToString() + " Live Connections:" + nodeReader.GetValue(1).ToString() + "<br>";
            nodesOnline = true;
        }

        nodeReader.Close();
        if (!nodesOnline)
        {
            m_onlineNodes.Text = "<font color=red>No Cluster Nodes Online</font>";
        }
        
        
        
        connection.Close();
    }
}

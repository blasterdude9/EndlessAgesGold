
	using System;
	using System.Data;
	using System.Drawing;
	using System.Web;
	using System.Web.UI.WebControls;
	using System.Web.UI.HtmlControls;
	using System.Data.SqlClient;

	/// <summary>
	///		Summary description for ServerStatusControl.
	/// </summary>
	public partial class ServerStatusControl : System.Web.UI.UserControl
	{

		private void Page_Load(object sender, System.EventArgs e)
		{
			string serverName = (string)Session["ServerName"];
			DatabaseConnectionModule dbConModule = new DatabaseConnectionModule();

			AuthenticationModule m_authPtr = new AuthenticationModule();
			m_authPtr.AuthenticateUser("Main.aspx","STATUSADMIN");
			
			if(serverName == null)
			{
				Server.Transfer("MainPanel.aspx?errorid=2"); 
				return;
			}
			if(serverName == "")
			{
				Server.Transfer("MainPanel.aspx?errorid=2"); 
				return;
			}

			   
			ConnectionHelper conHlper = new ConnectionHelper();
			SqlConnection sqlAXServerConnection = conHlper.GetServerConnection(serverName);
				
			if(sqlAXServerConnection == null)
			{
				m_statusOutput.Text = "Could not connect to servers database";
				return;
			}
			SqlDataReader sqlMainServerStatusReader = dbConModule.sqlQuery("select currentConnections,connectionsPeak,msgSendPerSecond,msgReceivedPerSecond,bandwidthOutgoingPerSecond,bandwidthIncommingPerSecond,bandwidthintotal,bandwidthOutTotal,bandwidthtotal,lastupdated from tblMainServerStatus",sqlAXServerConnection);
		    string row1_txt = "";
			string row2_txt = "";
			if(sqlMainServerStatusReader.Read())
			{
				//MAIN SERVER
				
				row1_txt += "Server Name:<BR>Updated Time:<BR><BR>";
				row1_txt += "Current Connections:<BR>";
				row1_txt += "Current Peak:<BR>";
				row1_txt += "Messages Send per second:<BR>";
				row1_txt += "Messages Received per second:<BR>";
				row1_txt += "Bandwidth out per second:<BR>";
				row1_txt += "Bandwidth in per second:<BR>";
				row1_txt += "Bandwidth in total:<BR>";
				row1_txt += "Bandwidth out total:<BR>";
				row1_txt += "Bandwidth use total:<BR>";
				//AI SERVER

				
				row2_txt += serverName + "<BR>"+sqlMainServerStatusReader.GetSqlValue(9).ToString()+"<BR><BR>";
				row2_txt += sqlMainServerStatusReader.GetSqlValue(0).ToString() + "<BR>";
				row2_txt += sqlMainServerStatusReader.GetSqlValue(1).ToString() + "<BR>";
				row2_txt += sqlMainServerStatusReader.GetSqlValue(2).ToString() + "<BR>";
				row2_txt += sqlMainServerStatusReader.GetSqlValue(3).ToString() + "<BR>";
				row2_txt += sqlMainServerStatusReader.GetSqlValue(4).ToString() + "<BR>";
				row2_txt += sqlMainServerStatusReader.GetSqlValue(5).ToString() + "<BR>";
				row2_txt += sqlMainServerStatusReader.GetSqlValue(6).ToString() + " MB<BR>";
				row2_txt += sqlMainServerStatusReader.GetSqlValue(7).ToString() + " MB<BR>";
				row2_txt += sqlMainServerStatusReader.GetSqlValue(8).ToString() + " MB<BR>";
				sqlMainServerStatusReader.Close();
				SqlDataReader sqlAIServerStatusReader = dbConModule.sqlQuery("select currentBots,currentBotPeak,worldmngcount,totalbotcount,packetsoutpersecond,packetsinpersecond,totalbandwidthin,totalbandwidthout,bandwidthincommingpersecond,bandwidthoutgoingpersecond,totalbandwidthuse,lastupdated from tblAIServerStatus",sqlAXServerConnection);
				if(sqlAIServerStatusReader.Read())
				{
					row1_txt += "<BR><BR>AI Server Name:<BR>Updated Time:<BR><BR>";
					row1_txt += "Current Bots:<BR>";
					row1_txt += "Current Bot Peak:<BR>";
					row1_txt += "World Manage Count:<BR>";
					row1_txt += "Packets out per second:<BR>";
					row1_txt += "Packets in per second:<BR>";
					row1_txt += "Total Bandwidth in:<BR>";
					row1_txt += "Total Bandwidth out:<BR>";
					row1_txt += "Bandwidth incomming per sec.:<BR>";
					row1_txt += "Bandwidth outgoing per sec.:<BR>";
					row1_txt += "Total Bandwidth use:<BR>";


					row2_txt += "<BR><BR>AI_" + serverName + "<BR>"+sqlAIServerStatusReader.GetSqlValue(11).ToString()+"<BR><BR>";
					row2_txt += sqlAIServerStatusReader.GetSqlValue(0).ToString() + "<BR>";
					row2_txt += sqlAIServerStatusReader.GetSqlValue(1).ToString() + "<BR>";
					row2_txt += sqlAIServerStatusReader.GetSqlValue(2).ToString() + "<BR>";
					row2_txt += sqlAIServerStatusReader.GetSqlValue(4).ToString() + "<BR>";
					row2_txt += sqlAIServerStatusReader.GetSqlValue(5).ToString() + "<BR>";
					row2_txt += sqlAIServerStatusReader.GetSqlValue(6).ToString() + " MB<BR>";
					row2_txt += sqlAIServerStatusReader.GetSqlValue(7).ToString() + " MB<BR>";
					row2_txt += sqlAIServerStatusReader.GetSqlValue(8).ToString() + "<BR>";
					row2_txt += sqlAIServerStatusReader.GetSqlValue(9).ToString() + "<BR>";
					row2_txt += sqlAIServerStatusReader.GetSqlValue(10).ToString() + " MB<BR>";
				}
				sqlAIServerStatusReader.Close();

				
			}
			else
			{
				sqlMainServerStatusReader.Close();
				m_statusOutput.Text = "Cannot find server";
			}

            //display online nodes that make up the cluster
            m_onlineNodes.Text = "";
            bool nodesOnline = false;
            SqlConnection hubConnect = conHlper.GetServerHUBConnection(serverName);
            SqlDataReader nodeReader = dbConModule.sqlQuery("Select NODEID,CONNECTIONCOUNT,NODEIP FROM SYSLIVESERVERNODES WHERE DATEADD(SECOND,25,CHECKINTIME) > GETDATE()", hubConnect);
            m_onlineNodes.Text = "Cluster Nodes Online<br>";
            while (nodeReader.Read())
            {
                m_onlineNodes.Text += "NodeID:" + nodeReader.GetValue(0).ToString() + " Ip:" + nodeReader.GetValue(2).ToString() + " Live Connections:" + nodeReader.GetValue(1).ToString() + "<br>";
                nodesOnline = true;
            }
            
            nodeReader.Close();
            hubConnect.Close();
            if (!nodesOnline)
            {
                m_onlineNodes.Text = "<font color=red>No Cluster Nodes Online</font>";
            }


            //MAKE NEW PANEL FOR PATCHER, THEY ARE NOW DIFFERENT SQL DATABASES
			/*row1_txt += "<BR><BR>PATCH SERVER STATUS<BR>";
			row2_txt += "<BR><BR>";
			//append patch server status
			SqlConnection axHubConn = dbConModule.CreateNativeConnectionObject();
			SqlDataReader patcherReader = dbConModule.sqlQuery("select username, ip, timeconnected,connectionIndex, kbs, lastUpdated, currentFilename, currentPosition, (SELECT sum(ptc_CurrentConnections.kbs) from ptc_CurrentConnections) as var1 from ptc_CurrentConnections",axHubConn);
			bool read1 = true;
			
			while(patcherReader.Read())
			{
				if(read1)
				{

				   row2_txt += patcherReader.GetSqlValue(8).ToString()+" total kbs<BR>";
                   read1 = false;
				}


				row1_txt += patcherReader.GetSqlValue(0).ToString()+" ( "+patcherReader.GetSqlValue(1).ToString()+" ) "+patcherReader.GetSqlValue(2).ToString()+"<BR>";
                row2_txt += "("+patcherReader.GetSqlValue(3).ToString()+") kbs:"+patcherReader.GetSqlValue(4).ToString()+" UPT:"+patcherReader.GetSqlValue(5).ToString()+" FILE:"+patcherReader.GetSqlValue(6).ToString()+" Pos:"+patcherReader.GetSqlValue(7).ToString()+"<BR>";

			}
			patcherReader.Close();
			axHubConn.Close();*/

			m_statusOutput.Text = row1_txt;
			m_row2StatusOut.Text = row2_txt;
			
		}

		#region Web Form Designer generated code
		override protected void OnInit(EventArgs e)
		{
			//
			// CODEGEN: This call is required by the ASP.NET Web Form Designer.
			//
			InitializeComponent();
			base.OnInit(e);
		}
		
		/// <summary>
		///		Required method for Designer support - do not modify
		///		the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.Load += new System.EventHandler(this.Page_Load);

		}
		#endregion
	}

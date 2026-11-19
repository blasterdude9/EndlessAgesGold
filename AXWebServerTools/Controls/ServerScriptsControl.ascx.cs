    using System;
	using System.Data;
	using System.Drawing;
	using System.Web;
	using System.Data.SqlClient;
	using System.Web.UI.WebControls;
	using System.Web.UI.HtmlControls;

	/// <summary>
	///		Summary description for AI_WorldManagerControl.
	/// </summary>
	public partial class ServerScripts : System.Web.UI.UserControl
	{
		DatabaseConnectionModule dbConModule = new DatabaseConnectionModule();
        ConnectionHelper m_conHelper = new ConnectionHelper();
        common m_common = new common();

		private void Page_Load(object sender, System.EventArgs e)
		{
			AuthenticationModule m_authPtr = new AuthenticationModule();
			m_authPtr.AuthenticateUser("Main.aspx","DEVELOPER");

            if (m_scriptList.Items.Count == 0)
            {
                InListWorlds(-1);


                m_common.InListFunctions(ref m_functionList);
            }
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
			this.AddBtn.Click += new System.EventHandler(this.AddBtn_Click);
			this.ReplaceBtn.Click += new System.EventHandler(this.ReplaceBtn_Click);
			this.DeleteBtn.Click += new System.EventHandler(this.DeleteBtn_Click);
			this.Load += new System.EventHandler(this.Page_Load);

		}
		#endregion

		private void InListWorlds(int sel)
		{
			string serverName = (string)Session["ServerName"];

			
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
				return;
			}
			SqlDataReader sqlMainServerStatusReader = dbConModule.sqlQuery("select id, scriptName, Description, ScriptBasis, ScriptData from tblServerScripts",sqlAXServerConnection);
			m_scriptList.Items.Clear();
			while(sqlMainServerStatusReader.Read())
			{
				m_scriptList.Items.Add(sqlMainServerStatusReader.GetSqlValue(1).ToString());
			}
			sqlMainServerStatusReader.Close();
			sqlAXServerConnection.Close();
			if(sel > -1)
			   m_scriptList.SelectedIndex = sel;
		}

		private void AddBtn_Click(object sender, System.EventArgs e)
		{
			string serverName = (string)Session["ServerName"];
            m_errOut.Text = "";
			
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
				return;
			}

            SqlDataReader sqlMainServerStatusReader = dbConModule.sqlQuery("select id, scriptName, Description, ScriptBasis, ScriptData, gamespecific, edbSpecific from tblServerScripts where scriptName ='" + m_scriptName.Text + "'", sqlAXServerConnection);
            if (sqlMainServerStatusReader.Read())
            {
                m_errOut.Text = "Error:Script name already exists";
                return;
            }
            sqlMainServerStatusReader.Close();

			dbConModule.ExecuteQuery("insert into tblServerScripts (id, scriptName, Description, ScriptBasis, ScriptData, gameSpecific, edbSpecific) Values('"+m_scriptUniqueID.Text+
				"','"+m_scriptName.Text +
				"','"+m_description.Text +
				"','"+m_scriptBasis.Text +
				"','"+m_scriptData.Text +
				"','"+m_gameSpecific.Text +
				"','"+m_edbSpecific.Text +
				"')",
				sqlAXServerConnection);
			sqlAXServerConnection.Close();

			InListWorlds(m_scriptList.Items.Count);
		}

		private void ReplaceBtn_Click(object sender, System.EventArgs e)
		{  
			string serverName = (string)Session["ServerName"];

			
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
				return;
			}
			dbConModule.ExecuteQuery("update tblServerScripts set scriptName = '"+m_scriptName.Text+"', " +
				                                                 "id = '"+m_scriptUniqueID.Text+"', " +
				                                                 "ScriptBasis = '"+m_scriptBasis.Text+"', " +
				                                                 "ScriptData = '"+m_scriptData.Text+"', " +
																 "GameSpecific = '"+m_gameSpecific.Text+"', " +
				                                                 "edbSpecific = '"+m_edbSpecific.Text+"', " +
				                                                 "Description = '"+m_description.Text+"' where scriptName ='"+m_scriptList.SelectedValue.ToString()+"'",
																  sqlAXServerConnection);
			sqlAXServerConnection.Close();
			InListWorlds(m_scriptList.SelectedIndex);

		}

		private void DeleteBtn_Click(object sender, System.EventArgs e)
		{
			string serverName = (string)Session["ServerName"];

			
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
				return;
			}

			int selTry = m_scriptList.SelectedIndex -1;
			dbConModule.ExecuteQuery("delete from tblServerScripts where scriptName = '"+m_scriptList.SelectedValue.ToString()+"'",sqlAXServerConnection);
			sqlAXServerConnection.Close();

			InListWorlds(selTry);
		}

		

		
        protected void m_scriptList_SelectedIndexChanged(object sender, EventArgs e)
        {
            string serverName = (string)Session["ServerName"];


            if (serverName == null)
            {
                Server.Transfer("MainPanel.aspx?errorid=2");
                return;
            }
            if (serverName == "")
            {
                Server.Transfer("MainPanel.aspx?errorid=2");
                return;
            }

            ConnectionHelper conHlper = new ConnectionHelper();
            SqlConnection sqlAXServerConnection = conHlper.GetServerConnection(serverName);

            if (sqlAXServerConnection == null)
            {
                return;
            }
            SqlDataReader sqlMainServerStatusReader = dbConModule.sqlQuery("select id, scriptName, Description, ScriptBasis, ScriptData, gamespecific, edbSpecific from tblServerScripts where scriptName ='" + m_scriptList.SelectedValue.ToString() + "'", sqlAXServerConnection);
            if (sqlMainServerStatusReader.Read())
            {
                m_scriptName.Text = sqlMainServerStatusReader.GetSqlValue(1).ToString();
                m_scriptBasis.Text = sqlMainServerStatusReader.GetSqlValue(3).ToString();
                m_scriptUniqueID.Text = sqlMainServerStatusReader.GetSqlValue(0).ToString();
                m_description.Text = sqlMainServerStatusReader.GetSqlValue(2).ToString();
                m_scriptData.Text = sqlMainServerStatusReader.GetSqlValue(4).ToString();
                m_gameSpecific.Text = sqlMainServerStatusReader.GetSqlValue(5).ToString();
                m_edbSpecific.Text = sqlMainServerStatusReader.GetSqlValue(6).ToString();
            }
            sqlMainServerStatusReader.Close();
            sqlAXServerConnection.Close();
        }
        protected void AquireBtn_Click1(object sender, EventArgs e)
        {
            string serverName = (string)Session["ServerName"];

            SqlConnection connectionToServerDB = m_conHelper.GetServerConnection(serverName);
            if (connectionToServerDB == null)
                return;

            string repRequester = (string)Session["user"];
            //send request
            m_conHelper.SendCommandToServer(
                serverName,
                "26",
                "",//param1
                "",//param2
                "",//param3
                "",//param4
                "",//param5
                repRequester,
                connectionToServerDB);
            connectionToServerDB.Close();
            m_errOut.Text = "Server update sent:" + DateTime.Now.ToLongTimeString();
        }
}



	using System;
	using System.Data;
	using System.Drawing;
	using System.Web;
	using System.Web.UI.WebControls;
	using System.Web.UI.HtmlControls;
	using System.Data.SqlClient;

	/// <summary>
	///		Summary description for NavControl.
	/// </summary>
	public partial class NavControl : System.Web.UI.UserControl
	{
	
		DatabaseConnectionModule dataCon = new DatabaseConnectionModule();

		private void Page_Load(object sender, System.EventArgs e)
		{
			SqlConnection mainConnection = dataCon.CreateNativeConnectionObject();
			AuthenticationModule m_authPtr = new AuthenticationModule();
			m_authPtr.AuthenticateUser("Main.aspx","VIEWSVC");

			if(m_serverList.Items.Count==0)
			{
				SqlDataReader sqlReader = dataCon.sqlQuery("Select * from tblServers where servermanageip is not null",mainConnection);
				m_serverList.Items.Add("Select Server...");
				while(sqlReader.Read())
				{
					m_serverList.Items.Add(sqlReader.GetSqlValue(1).ToString());
				}
				sqlReader.Close();
				
			    string serverName = (string)Session["ServerName"];
					
					
				if(serverName == null || serverName == "")
				{
					serverName = (string)Request.QueryString["servername"];
					Session["ServerName"] = serverName;
				}
				
				
				m_serverList.SelectedValue = serverName;
			    //Session["ServerName"] = serverName;
				
			}
			mainConnection.Close();
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
			this.m_showStatus.Click += new System.EventHandler(this.m_showStatus_Click);
			this.LogPageBtn.Click += new System.EventHandler(this.LogPageBtn_Click);
			this.aisreverLogBtn.Click += new System.EventHandler(this.aisreverLogBtn_Click);
			this.AcManageBtn.Click += new System.EventHandler(this.AcManageBtn_Click);
			this.Button1.Click += new System.EventHandler(this.Button1_Click);
			this.MainSvrSettings.Click += new System.EventHandler(this.MainSvrSettings_Click);
			this.AI_BaseWorldManager.Click += new System.EventHandler(this.AI_BaseWorldManager_Click);
			this.SvrScriptsBtn.Click += new System.EventHandler(this.SvrScriptsBtn_Click);
			this.Load += new System.EventHandler(this.Page_Load);

		}
		#endregion

		private void m_showStatus_Click(object sender, System.EventArgs e)
		{
			Session["ServerName"] = m_serverList.SelectedValue.ToString();
			Server.Transfer("StatusPage.aspx");
		}

		private void LogPageBtn_Click(object sender, System.EventArgs e)
		{
			Session["ServerName"] = m_serverList.SelectedValue.ToString();
			Session["logModeName"] = "Main Server Log:";
			Server.Transfer("LogPage.aspx");
		}

		private void aisreverLogBtn_Click(object sender, System.EventArgs e)
		{
			Session["ServerName"] = m_serverList.SelectedValue.ToString();
			Session["logModeName"] = "AI Server Log:";
			Server.Transfer("LogPage.aspx");
		}

		private void AcManageBtn_Click(object sender, System.EventArgs e)
		{
			Session["ServerName"] = m_serverList.SelectedValue.ToString();
			Session["logModeName"] = "Account Manager:";
			Server.Transfer("AcManagePage.aspx");
		}

		private void Button1_Click(object sender, System.EventArgs e)
		{
			Session["ServerName"] = m_serverList.SelectedValue.ToString();
			Session["logModeName"] = "Server Tools:";
			Server.Transfer("ServerTools.aspx");
		}

		private void MainSvrSettings_Click(object sender, System.EventArgs e)
		{
			Session["ServerName"] = m_serverList.SelectedValue.ToString();
		    Server.Transfer("MainSvrSettingsPage.aspx");
		}

		private void AI_BaseWorldManager_Click(object sender, System.EventArgs e)
		{
			Session["ServerName"] = m_serverList.SelectedValue.ToString();
			Server.Transfer("AIWorldManagePage.aspx");
		}

		private void SvrScriptsBtn_Click(object sender, System.EventArgs e)
		{
			Session["ServerName"] = m_serverList.SelectedValue.ToString();
			Server.Transfer("ServerScriptsPage.aspx");
		}

        protected void QuestBtn_Click(object sender, System.EventArgs e)
		{
			Session["ServerName"] = m_serverList.SelectedValue.ToString();
			Server.Transfer("IGN_QuestMenuPage.aspx");
		}


        protected void zoneStatusBtn_Click(object sender, EventArgs e)
        {
            Session["ServerName"] = m_serverList.SelectedValue.ToString();
            Server.Transfer("LiveZonesStatusPage.aspx");
        }
        protected void WorldManagerBtn_Click1(object sender, EventArgs e)
        {
            Session["ServerName"] = m_serverList.SelectedValue.ToString();
            Server.Transfer("StencilZoneManager.aspx");
        }
        protected void testBTN_Click(object sender, EventArgs e)
        {

        }
        protected void webApxBtn_Click(object sender, EventArgs e)
        {
            Session["ServerName"] = m_serverList.SelectedValue.ToString();
            Server.Transfer("ApxPage.aspx");
        }
}

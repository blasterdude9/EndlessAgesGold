
	using System;
	using System.Data;
	using System.Drawing;
	using System.Web;
	using System.Data.SqlClient;
	using System.Web.UI.WebControls;
	using System.Web.UI.HtmlControls;

	/// <summary>
	///		Summary description for ServerSettings.
	/// </summary>
	public partial class ServerSettings : System.Web.UI.UserControl
	{
		DatabaseConnectionModule dbConModule = new DatabaseConnectionModule();

		private void Page_Load(object sender, System.EventArgs e)
		{
			AuthenticationModule m_authPtr = new AuthenticationModule();
			m_authPtr.AuthenticateUser("Main.aspx","DEVELOPER");

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

			if(m_version.Text.Length > 0)
			   return;

			   
			ConnectionHelper conHlper = new ConnectionHelper();
			SqlConnection sqlAXServerConnection = conHlper.GetServerConnection(serverName);
				
			if(sqlAXServerConnection == null)
			{
				return;
			}

			SqlDataReader sqlMainServerStatusReader = dbConModule.sqlQuery("select Version, Max_players, Frequency, Dead_PCEntity_Dissolve_Time, Dead_NPCEntity_Dissolve_Time, AIServer_Cycle, Murder_Level_Count, Workoff_Murder_Rate, Duration_between_Backups, MainServerIP, AIServerUsername, AIServerPassword, LogPriorityLevel, RegenTurnSpeed, DefaultHealthRegenAmount, LootMode, AgressorMaxTrack, MaxDotTrack, DamageMode, RespawnHealthPercentage from tblMainConfiguration",sqlAXServerConnection);
			if(sqlMainServerStatusReader.Read())
			{
					m_version.Text					= sqlMainServerStatusReader.GetSqlValue(0).ToString();
					m_maxPlayerLimit.Text			= sqlMainServerStatusReader.GetSqlValue(1).ToString();
					m_gameplayTurnSpeed.Text		= sqlMainServerStatusReader.GetSqlValue(2).ToString();
					m_deadPCDissolveTime.Text		= sqlMainServerStatusReader.GetSqlValue(3).ToString();
					m_deadNPCDissolveTime.Text		= sqlMainServerStatusReader.GetSqlValue(4).ToString();
					m_aiServerTurnSpeed.Text		= sqlMainServerStatusReader.GetSqlValue(5).ToString();
					m_murderLevelCount.Text			= sqlMainServerStatusReader.GetSqlValue(6).ToString();
					m_murderWorkOffRate.Text		= sqlMainServerStatusReader.GetSqlValue(7).ToString();
					m_durationBetweenBackups.Text	= sqlMainServerStatusReader.GetSqlValue(8).ToString();
					m_mainServerIP.Text				= sqlMainServerStatusReader.GetSqlValue(9).ToString();
					m_aiServerUsername.Text			= sqlMainServerStatusReader.GetSqlValue(10).ToString();
				    m_aiServerPassord.Text			= sqlMainServerStatusReader.GetSqlValue(11).ToString();
                    m_logPriority.Text			    = sqlMainServerStatusReader.GetSqlValue(12).ToString();
                    m_regenTurnSpeed.Text			= sqlMainServerStatusReader.GetSqlValue(13).ToString();
                    m_defaultHealthRegenAmount.Text = sqlMainServerStatusReader.GetSqlValue(14).ToString();
                    m_lootMode.Text			        = sqlMainServerStatusReader.GetSqlValue(15).ToString();
                    m_aggressorMaxTrack.Text	    = sqlMainServerStatusReader.GetSqlValue(16).ToString();
                    m_maxDotTrack.Text			    = sqlMainServerStatusReader.GetSqlValue(17).ToString();
                    m_damageMode.Text			    = sqlMainServerStatusReader.GetSqlValue(18).ToString();
                    m_respawnHealthPercentage.Text  = sqlMainServerStatusReader.GetSqlValue(19).ToString();
			}
			sqlMainServerStatusReader.Close();
			sqlAXServerConnection.Close();
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

        protected void ApplyBtn_Click1(object sender, EventArgs e)
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
            dbConModule.ExecuteQuery("Update tblMainConfiguration set Version  = '" + m_version.Text + "', " +
                                                                                    "Max_players" + "= '" + m_maxPlayerLimit.Text + "', " +
                                                                                    "Frequency" + "= '" + m_gameplayTurnSpeed.Text + "', " +
                                                                                    "Dead_PCEntity_Dissolve_Time" + "= '" + m_deadPCDissolveTime.Text + "', " +
                                                                                    "Dead_NPCEntity_Dissolve_Time" + "= '" + m_deadNPCDissolveTime.Text + "', " +
                                                                                    "AIServer_Cycle" + "= '" + m_aiServerTurnSpeed.Text + "', " +
                                                                                    "Murder_Level_Count" + "= '" + m_murderLevelCount.Text + "', " +
                                                                                    "Workoff_Murder_Rate" + "= '" + m_murderWorkOffRate.Text + "', " +
                                                                                    "Duration_between_Backups" + "= '" + m_durationBetweenBackups.Text + "', " +
                                                                                    "MainServerIP" + "= '" + m_mainServerIP.Text + "', " +
                                                                                    "AIServerUsername" + "= '" + m_aiServerUsername.Text + "', " +
                                                                                    "LogPriorityLevel" + "= '" + m_logPriority.Text + "', " +
                                                                                    "RegenTurnSpeed" + "= '" + m_regenTurnSpeed.Text + "', " +
                                                                                    "DefaultHealthRegenAmount" + "= '" + m_defaultHealthRegenAmount.Text + "', " +
                                                                                    "LootMode" + "= '" + m_lootMode.Text + "', " +
                                                                                    "AgressorMaxTrack" + "= '" + m_aggressorMaxTrack.Text + "', " +
                                                                                    "MaxDotTrack" + "= '" + m_maxDotTrack.Text + "', " +
                                                                                    "DamageMode" + "= '" + m_damageMode.Text + "', " +
                                                                                    "RespawnHealthPercentage" + "= '" + m_respawnHealthPercentage.Text + "', " +
                                                                                    "AIServerPassword" + "= '" + m_aiServerPassord.Text + "'", sqlAXServerConnection);

            
            sqlAXServerConnection.Close();
        }
}

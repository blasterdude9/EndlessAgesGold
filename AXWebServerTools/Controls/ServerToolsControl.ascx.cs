
	using System;
	using System.Data;
	using System.Drawing;
	using System.Web;
	using System.Web.UI.WebControls;
	using System.Web.UI.HtmlControls;
	using System.Data.SqlClient;
	using System.Threading;

	/// <summary>
	///		Summary description for ServerToolsControl.
	/// </summary>
	public partial class ServerToolsControl : System.Web.UI.UserControl
	{
		DatabaseConnectionModule m_dbConnectionPtr = new DatabaseConnectionModule();
		ConnectionHelper m_conHelper = new ConnectionHelper();
        public static AXencrypt ax_encryptionManager = new AXencrypt();

		private void Page_Load(object sender, System.EventArgs e)
		{
			AuthenticationModule m_authPtr = new AuthenticationModule();
			m_authPtr.AuthenticateUser("Main.aspx","DEVELOPER");
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
			this.RestartSystemBtn.Click += new System.EventHandler(this.RestartSystemBtn_Click);
			this.adminModeBtn.Click += new System.EventHandler(this.adminModeBtn_Click);
			this.ManualSaveBtn.Click += new System.EventHandler(this.ManualSaveBtn_Click);
			this.UpdateStatusTableBtn.Click += new System.EventHandler(this.UpdateStatusTableBtn_Click);
			this.DisconnectAllBtn.Click += new System.EventHandler(this.DisconnectAllBtn_Click);
			this.Load += new System.EventHandler(this.Page_Load);

		}
		#endregion

		private void RestartSystemBtn_Click(object sender, System.EventArgs e)
		{
			string serverName   = (string)Session["ServerName"];
			string repRequester = (string)Session["user"];
			//aquire target server db
			SqlConnection connectionToServerDB = m_conHelper.GetServerConnection(serverName);
			if(connectionToServerDB == null)
				return;
            
			//send request
			m_conHelper.SendCommandToServer(       
				serverName,
				"8",
				"",
				"",
				"",
				"",
				"",
				repRequester,
				connectionToServerDB);
			//wait for response
			bool foundEndCap = false;
			for(int loop=0;loop<5;loop++)
			{ 
				Thread.Sleep(1000);
				if(m_conHelper.GetServerResponseEnd(repRequester,connectionToServerDB))
				{
					foundEndCap = true;
					break;
				}
			}
			if(foundEndCap)
			{//process all and then delete response
				//resposition completed

				m_dbConnectionPtr.ExecuteQuery("delete from tblServerResponse where peram6 = '"+repRequester+"'",connectionToServerDB);
			}//end process all and then delete response
			else
			{
				m_editNameSectionLabel.Text = "Response Timed Out";

			}
			connectionToServerDB.Close();
		}

		private void adminModeBtn_Click(object sender, System.EventArgs e)
		{
			string serverName   = (string)Session["ServerName"];
			string repRequester = (string)Session["user"];
			//aquire target server db
			SqlConnection connectionToServerDB = m_conHelper.GetServerConnection(serverName);
			if(connectionToServerDB == null)
				return;
            
			//send request
			m_conHelper.SendCommandToServer(       
				serverName,
				"9",
				"",
				"",
				"",
				"",
				"",
				repRequester,
				connectionToServerDB);
			//wait for response
			bool foundEndCap = false;
			for(int loop=0;loop<5;loop++)
			{ 
				Thread.Sleep(1000);
				if(m_conHelper.GetServerResponseEnd(repRequester,connectionToServerDB))
				{
					foundEndCap = true;
					break;
				}
			}
			if(foundEndCap)
			{//process all and then delete response
		        SqlDataReader readerPtr = m_dbConnectionPtr.sqlQuery("Select peram1 from tblServerResponse where peram6 = '"+repRequester+"'",connectionToServerDB);
			    while(readerPtr.Read())
			    {
				      m_editNameSectionLabel.Text = readerPtr.GetSqlValue(0).ToString();					
			    }
			    readerPtr.Close();

				m_dbConnectionPtr.ExecuteQuery("delete from tblServerResponse where peram6 = '"+repRequester+"'",connectionToServerDB);
			}//end process all and then delete response
			else
			{
				m_editNameSectionLabel.Text = "Response Timed Out";

			}
			connectionToServerDB.Close();
		}

		private void ManualSaveBtn_Click(object sender, System.EventArgs e)
		{
			string serverName   = (string)Session["ServerName"];
			string repRequester = (string)Session["user"];
			//aquire target server db
			SqlConnection connectionToServerDB = m_conHelper.GetServerConnection(serverName);
			if(connectionToServerDB == null)
				return;
            
			//send request
			m_conHelper.SendCommandToServer(       
				serverName,
				"10",
				"",
				"",
				"",
				"",
				"",
				repRequester,
				connectionToServerDB);
			//wait for response
			
			m_editNameSectionLabel.Text = "Task has been sent";
			connectionToServerDB.Close();
		}

		private void UpdateStatusTableBtn_Click(object sender, System.EventArgs e)
		{
			string serverName   = (string)Session["ServerName"];
			string repRequester = (string)Session["user"];
			//aquire target server db
			SqlConnection connectionToServerDB = m_conHelper.GetServerConnection(serverName);
			if(connectionToServerDB == null)
				return;
            
			//send request
			m_conHelper.SendCommandToServer(       
				serverName,
				"14",
				"",
				"",
				"",
				"",
				"",
				repRequester,
				connectionToServerDB);
			//wait for response
			
			m_editNameSectionLabel.Text = "SQL Task has been sent";
			connectionToServerDB.Close();
		}

		private void DisconnectAllBtn_Click(object sender, System.EventArgs e)
		{
			string serverName   = (string)Session["ServerName"];
			string repRequester = (string)Session["user"];
			//aquire target server db
			SqlConnection connectionToServerDB = m_conHelper.GetServerConnection(serverName);
			if(connectionToServerDB == null)
				return;
            
			//send request
			m_conHelper.SendCommandToServer(       
				serverName,
				"15",
				"",
				"",
				"",
				"",
				"",
				repRequester,
				connectionToServerDB);
			//wait for response
			
			m_editNameSectionLabel.Text = "SQL Task has been sent";
			connectionToServerDB.Close();
		}


        protected void GenerateTestAccountsBtn_Click(object sender, EventArgs e)
        {
            string serverName = (string)Session["ServerName"];
            SqlConnection connectionToServerDB = m_conHelper.GetServerHUBConnection(serverName);
            if (connectionToServerDB == null)
                return;

            SqlConnection connectionToRegistrationDB = m_conHelper.GetServerHUBConnection(serverName+"-REG");
            if (connectionToRegistrationDB == null)
                return;

            if (m_adminCode.Text != "234")
                return;

            

            //Aquire character stencil
            string queryTemp,newUsername, charName, accesslevel, respawnWorld, currentWorldID, currentWorldMap,dbCfg,altCfg,cfgStasis,originaldbCfg,energy,gamename,respawnworldname;
            SqlDataReader charReader = m_dbConnectionPtr.sqlQuery("Select charName,accesslevel,respawnWorld,currentWorldID,currentWorldMap,dbCfg,altCfg,cfgStasis,originaldbCfg,energy,gamename,respawnworldname from gmCharacters where username = '" + m_accountUsrStencil.Text + "' AND inuse = '1'", connectionToServerDB);
            if (charReader.Read())
            {//only take the first one
                charName = charReader.GetSqlValue(0).ToString();
                accesslevel = charReader.GetSqlValue(1).ToString();
                respawnWorld = charReader.GetSqlValue(2).ToString();
                currentWorldID = charReader.GetSqlValue(3).ToString();
                currentWorldMap = charReader.GetSqlValue(4).ToString();
                dbCfg = charReader.GetSqlValue(5).ToString();
                altCfg = charReader.GetSqlValue(6).ToString();
                cfgStasis = charReader.GetSqlValue(7).ToString();
                originaldbCfg = charReader.GetSqlValue(8).ToString();
                energy = charReader.GetSqlValue(9).ToString();
                gamename = charReader.GetSqlValue(10).ToString();
                respawnworldname = charReader.GetSqlValue(11).ToString();

                charReader.Close();
            }
            else
            {//if theres no chars theres nothing to clone
                charReader.Close();
                return;
            }

            //create accounts and characters loop
            int accountsToCreate = Convert.ToInt32(m_genQuanity.Text);

            string encryptedPassword = ax_encryptionManager.Encrypt(m_dbConnectionPtr.SanitizeIn(m_newPassBase.Text));

            for (int loop = 0; loop < accountsToCreate; loop++)
            {
                newUsername = m_dbConnectionPtr.SanitizeIn(m_newUsernameBase.Text) + loop.ToString();
                //create new account and get handle
                queryTemp = "insert into gmAccountBase (username,password,inGameName,recurringdate,loggedon,maxcharacters,mutedeaf,minutesplayed) Values('" + newUsername + "','" + encryptedPassword + "','" + gamename + "',DateAdd(year,3,GetDate()),'0','3','0','0')";
                m_dbConnectionPtr.ExecuteQuery(queryTemp, connectionToRegistrationDB);

                
                //end create new account and get handle
                //add character
                queryTemp = "insert into gmCharacters (respawnworldname,curDbCfg,username,charName, accesslevel, respawnWorld, currentWorldID, currentWorldMap,dbCfg,altCfg,cfgStasis,originaldbCfg,energy,gameName,inuse,rebirthY) ";
                queryTemp += "Values('"+respawnworldname+"','" + dbCfg + "','" + newUsername + "','" + charName + "','" + accesslevel + "','" + respawnWorld + "','" + currentWorldID + "','" + currentWorldMap + "','" + dbCfg + "','" + altCfg + "','" + cfgStasis + "','" + dbCfg + "','" + energy + "','" + gamename + "','1','3')";
                m_dbConnectionPtr.ExecuteQuery(queryTemp, connectionToServerDB);
                //end add character
            }

            connectionToServerDB.Close();
            connectionToRegistrationDB.Close();
        }
    }

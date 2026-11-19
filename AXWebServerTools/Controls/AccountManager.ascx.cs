
using System;
using System.Data;
using System.Configuration;
using System.Drawing;
using System.Web;
using System.Web.UI.WebControls;
using System.Web.UI.HtmlControls;
using System.Data.SqlClient;
using System.Threading;


	/// <summary>
	///		Summary description for AccountManager.
	/// </summary>
	public partial class AccountManager : System.Web.UI.UserControl
	{
		DatabaseConnectionModule m_dbConnectionPtr = new DatabaseConnectionModule();
		ConnectionHelper m_conHelper = new ConnectionHelper();
        common m_common = new common();

		private void Page_Load(object sender, System.EventArgs e)
		{
			AuthenticationModule m_authPtr = new AuthenticationModule();
			m_authPtr.AuthenticateUser("Main.aspx","ACCOUNTADMIN");

			string showItems = (string)Request.QueryString["showItems"];
            
            if(m_showStatPanel.Checked)
			   m_statPanel.Visible = true;
			else
			   m_statPanel.Visible = false;

			if(m_showInventoryPanel.Checked)
				m_charInvPanel.Visible = true;
			else
				m_charInvPanel.Visible = false;

			if(m_showBankPanel.Checked)
				m_bankPanel.Visible = true;
			else
				m_bankPanel.Visible = false;


			if(m_reposLocations.Items.Count == 0)
			{
				string serverName   = (string)Session["ServerName"];
                SqlConnection connectionToServerDB = m_conHelper.GetServerHUBConnection(serverName);//aquire target server db
				//SqlConnection connectionToServerDB = m_dbConnectionPtr.CreateNativeConnectionObject();
			    RefreshRepositionList(connectionToServerDB);	
				
				connectionToServerDB.Close();
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
			this.m_currentPlayersList.SelectedIndexChanged += new System.EventHandler(this.m_currentPlayersList_SelectedIndexChanged);
			this.GetPlayerInfoBtn.Click += new System.EventHandler(this.GetPlayerInfoBtn_Click);
			this.DisconnectBtn.Click += new System.EventHandler(this.DisconnectBtn_Click);
			this.m_RepositionBtn.Click += new System.EventHandler(this.m_RepositionBtn_Click);
			this.BanToggleBtn.Click += new System.EventHandler(this.BanToggleBtn_Click);
			this.GMToggleBtn.Click += new System.EventHandler(this.GMToggleBtn_Click);
			this.SetStatBtn.Click += new System.EventHandler(this.SetStatBtn_Click);
			this.SetSkillBtn.Click += new System.EventHandler(this.SetSkillBtn_Click);
			this.RefreshInvBtn.Click += new System.EventHandler(this.RefreshInvBtn_Click);
			this.AddItemBtn.Click += new System.EventHandler(this.AddItemBtn_Click);
			this.DeleteItemBtn.Click += new System.EventHandler(this.DeleteItemBtn_Click);
			this.SetBankCreditsBtn.Click += new System.EventHandler(this.SetBankCreditsBtn_Click);
			this.Load += new System.EventHandler(this.Page_Load);

		}
		#endregion

		private void HideAllDynControls()
		{

		}

		private void GetPlayerInfoBtn_Click(object sender, System.EventArgs e)
		{
			//Request Characters, invis stuff
			
			m_currentDataList.Items.Clear();
			m_currentBankInvenList.Items.Clear();
			m_skillList.Items.Clear();
			m_statList.Items.Clear();
			//m_currentPlayersList.ClearSelection();
			string serverName   = (string)Session["ServerName"];
			string repRequester = (string)Session["user"];
			//aquire target server db
            SqlConnection connectionToServerDB = m_conHelper.GetServerHUBConnection(serverName);//aquire target server db
			//SqlConnection connectionToServerDB = m_dbConnectionPtr.CreateNativeConnectionObject();
			if(connectionToServerDB == null)
				return;

			if(m_currentPlayersList.SelectedIndex < 0)
			    return;

			RefreshPlayerInventories(connectionToServerDB);
			RefreshPlayerStats(connectionToServerDB);
			RefreshPlayerSkills(connectionToServerDB);
			
			
			connectionToServerDB.Close();
		}

		private void RefreshPlayerSkills(SqlConnection connectionToServerDB)
		{
			string handleLoc = m_currentPlayersList.SelectedValue.ToString();
			//STATS
			string qryStats = "Select (SELECT skillName from GM_SKILLS as inC where inC.skillType = xCI.skillType) as var1,currentExperience,skillType from gmSkills as xCI where handle = '"+handleLoc+"'";
			m_skillList.Items.Clear();
			SqlDataReader statReaderPtr = m_dbConnectionPtr.sqlQuery(qryStats,connectionToServerDB);
			while(statReaderPtr.Read())
			{
				ListItem lItem = new ListItem(statReaderPtr.GetSqlValue(0).ToString()+":"+statReaderPtr.GetSqlValue(1).ToString(),statReaderPtr.GetSqlValue(2).ToString());
				m_skillList.Items.Add(lItem);
			}
			statReaderPtr.Close(); 
		}

		private void RefreshPlayerStats(SqlConnection connectionToServerDB)
		{
			string handleLoc = m_currentPlayersList.SelectedValue.ToString();
            //character stats pos/world



			
			//STATS
			string qryStats = "Select (SELECT statName from GM_STATS where statID = statType) as var1,currentAmount,statType from gmStats where handle = '"+handleLoc+"'";
			m_statList.Items.Clear();
			SqlDataReader statReaderPtr = m_dbConnectionPtr.sqlQuery(qryStats,connectionToServerDB);
			while(statReaderPtr.Read())
			{
				ListItem lItem = new ListItem(statReaderPtr.GetSqlValue(0).ToString()+":"+statReaderPtr.GetSqlValue(1).ToString(),statReaderPtr.GetSqlValue(2).ToString());
				m_statList.Items.Add(lItem);
			}
			statReaderPtr.Close(); 
		}

		private void RefreshPlayerInventories(SqlConnection connectionToServerDB)
		{
			string handleLoc = m_currentPlayersList.SelectedValue.ToString();
          
			m_currentDataList.Items.Clear();
			m_currentBankInvenList.Items.Clear();
			//INVENTORIES
			string dmgAppend = "";
            string itemName = "";
            string qry = "Select (SELECT itemName from GM_GIL where gil = itemGilType) as var1,armed,storageType,dmgMod,quanity,itemGilType,(SELECT itemName from GM_DYNAMICITEMS where glid = itemGilType) as var2 from gmInventory where handle = '" + handleLoc + "' and uniqueidbind = '0'";
			SqlDataReader readerPtr = m_dbConnectionPtr.sqlQuery(qry,connectionToServerDB);
			while(readerPtr.Read())
			{
                dmgAppend = "";
				if(readerPtr.GetSqlValue(3).ToString() != "0")
					dmgAppend = "+"+readerPtr.GetSqlValue(3).ToString();


                itemName = readerPtr.GetSqlValue(0).ToString();
                if (itemName == "Null")
                {
                    itemName = readerPtr.GetSqlValue(6).ToString();

                }
                
                
                if(readerPtr.GetSqlValue(2).ToString() == "0")
				{
					if(readerPtr.GetSqlValue(1).ToString() == "1")
					{
                        ListItem lItem = new ListItem("ARMED-" + readerPtr.GetSqlValue(4).ToString() + ")" + itemName + dmgAppend, readerPtr.GetSqlValue(5).ToString());
						m_currentDataList.Items.Add(lItem);
					}
					else
					{
                        ListItem lItem = new ListItem(readerPtr.GetSqlValue(4).ToString() + ")" + itemName + dmgAppend, readerPtr.GetSqlValue(5).ToString());
						m_currentDataList.Items.Add(lItem);
					}

					
				}
				if(readerPtr.GetSqlValue(2).ToString() == "1")
				{
                    ListItem lItem = new ListItem(readerPtr.GetSqlValue(4).ToString() + ")" + itemName + dmgAppend, readerPtr.GetSqlValue(5).ToString());
					m_currentBankInvenList.Items.Add(lItem);
				}
			}
			readerPtr.Close();    
    

		}

		private void RefreshRepositionList(SqlConnection connectionToServerDB)
		{
			m_reposLocations.Items.Clear();
			SqlDataReader errorReader = m_dbConnectionPtr.sqlQuery("Select Descriptive,Worldname,WorldID,PosX,PosY,PosZ,GameSpecific from locations_SpawnPoints",connectionToServerDB);
			while(errorReader.Read())
			{
				m_reposLocations.Items.Add(errorReader.GetSqlValue(0).ToString());			
			}
			errorReader.Close();
		}

		private void RefreshItemsList(SqlConnection connectionToServerDB)
		{
        
		}

		private void AddItemBtn_Click(object sender, System.EventArgs e)
		{
			ItemErrorOutput.Text = "";

			if(m_itemDataList.SelectedIndex < 0)
				return;

			if(m_currentPlayersList.SelectedIndex < 0)
				return;
			
			//m_currentPlayersList.ClearSelection();
			string serverName   = (string)Session["ServerName"];
			string repRequester = (string)Session["user"];
			//aquire target server db
			//SqlConnection connectionToServerDB = m_dbConnectionPtr.CreateNativeConnectionObject();
            SqlConnection connectionToServerDB = m_conHelper.GetServerHUBConnection(serverName);//aquire target server db
			if(connectionToServerDB == null)
				return;


			string handleLoc = m_currentPlayersList.SelectedValue.ToString();
			string glidID = m_itemDataList.SelectedValue.ToString();

            string insertInventory	        = "INSERT INTO GMINVENTORY (HANDLE, ITEMUNIQUEID, ITEMGILTYPE, UNIQUEIDBIND, CURRENTCHARGES, STORAGETYPE, ARMED, QUANITY,NEWFLAG,MISCINT1,MISCFLOAT1,MISCFLOAT2,DMGMOD) "+
				                              "Values('$HANDLE','$ITEMUNIQUEID','$ITEMGILTYPE','$UNIQUEIDBIND','$CURRENTCHARGES','$STORAGETYPE','$ARMED','$QUANITY','1','$MISCINT1','$MISCFLOAT1','$MISCFLOAT2','$DMGMOD')";
           

			insertInventory = insertInventory.Replace("$HANDLE",handleLoc);
			insertInventory = insertInventory.Replace("$UNIQUEIDBIND","0");
			insertInventory = insertInventory.Replace("$ITEMUNIQUEID","999");
			insertInventory = insertInventory.Replace("$ITEMGILTYPE",glidID);
			insertInventory = insertInventory.Replace("$CURRENTCHARGES","0");
			insertInventory = insertInventory.Replace("$STORAGETYPE","1");
			insertInventory = insertInventory.Replace("$ARMED","0");
			insertInventory = insertInventory.Replace("$QUANITY",m_addItemQuanity.Text);
			insertInventory = insertInventory.Replace("$MISCINT1","0");
			insertInventory = insertInventory.Replace("$MISCFLOAT1","0");
			insertInventory = insertInventory.Replace("$MISCFLOAT2","0");
			insertInventory = insertInventory.Replace("$DMGMOD",m_dmgMod.Text);
					   
			m_dbConnectionPtr.ExecuteQuery(insertInventory,connectionToServerDB);

            RefreshPlayerInventories(connectionToServerDB);
		
			connectionToServerDB.Close();
		}

		private void DeleteItemBtn_Click(object sender, System.EventArgs e)
		{
			if(m_currentBankInvenList.SelectedIndex < 0)
			   return;

			if(m_currentPlayersList.SelectedIndex < 0)
			   return;

			string handleLoc = m_currentPlayersList.SelectedValue.ToString();
            string serverName = (string)Session["ServerName"];
			//SqlConnection connectionToServerDB = m_dbConnectionPtr.CreateNativeConnectionObject();
            SqlConnection connectionToServerDB = m_conHelper.GetServerHUBConnection(serverName);//aquire target server db
			if(connectionToServerDB == null)
				return;

 
            string delQry = "DELETE FROM gmInventory WHERE handle = '"+handleLoc+"' and itemGilType = '"+m_currentBankInvenList.SelectedValue.ToString()+"'";
            m_dbConnectionPtr.ExecuteQuery(delQry,connectionToServerDB);
            int sel = m_currentBankInvenList.SelectedIndex;
			m_currentBankInvenList.Items.RemoveAt(m_currentBankInvenList.SelectedIndex);
			m_currentBankInvenList.SelectedIndex = sel-1;
			
			connectionToServerDB.Close();
		}

		private void RefreshInvBtn_Click(object sender, System.EventArgs e)
		{	
			//m_currentPlayersList.ClearSelection();
			string serverName   = (string)Session["ServerName"];
			string repRequester = (string)Session["user"];
			//aquire target server db

			m_itemDataList.Items.Clear();
			//SqlConnection connectionToServerDB = m_dbConnectionPtr.CreateNativeConnectionObject();
            SqlConnection connectionToServerDB = m_conHelper.GetServerHUBConnection(serverName);//aquire target server db
			if(connectionToServerDB == null)
				return;

            m_common.FillListItemsBySearchTerm(m_searchWord.Text, ref m_itemDataList, connectionToServerDB);

			/*string srchQuery = "SELECT ITEMNAME,GIL FROM GM_GIL WHERE ITEMNAME LIKE '%"+m_searchWord.Text+"%'";
			SqlDataReader readerPtr = m_dbConnectionPtr.sqlQuery(srchQuery,connectionToServerDB);
			while(readerPtr.Read())
			{
                 ListItem lItem = new ListItem(readerPtr.GetSqlValue(0).ToString(),readerPtr.GetSqlValue(1).ToString());
                 m_itemDataList.Items.Add(lItem);
			}
			readerPtr.Close();*/
			connectionToServerDB.Close();
		}

	
		private void m_RepositionBtn_Click(object sender, System.EventArgs e)
		{
			string serverName   = (string)Session["ServerName"];
			string repRequester = (string)Session["user"];
			//aquire target server db
			SqlConnection connectionToServerDB = m_conHelper.GetServerConnection(serverName);
			if(connectionToServerDB == null)
				return;

			//SqlConnection nativeConnection = m_dbConnectionPtr.CreateNativeConnectionObject();
            SqlConnection nativeConnection = m_conHelper.GetServerHUBConnection(serverName);//aquire target server db
			if(nativeConnection == null)
				return;
            //get db position
			SqlDataReader posreader = m_dbConnectionPtr.sqlQuery("Select Descriptive,Worldname,WorldID,PosX,PosY,PosZ,GameSpecific from locations_SpawnPoints where Descriptive = '"+m_reposLocations.SelectedValue.ToString()+"'",nativeConnection);
			string worldName, worldID, posX, posY, posZ;
			
			
			if(posreader.Read())
			{
				worldName		= posreader.GetSqlValue(1).ToString();		
				worldID			= posreader.GetSqlValue(2).ToString();	
				posX			= posreader.GetSqlValue(3).ToString();	
				posY			= posreader.GetSqlValue(4).ToString();	
				posZ			= posreader.GetSqlValue(5).ToString();	
			}
			else
			{
                posreader.Close();
				return;
			}
			posreader.Close();
			//set DB values
			string handleLoc = m_currentPlayersList.SelectedValue.ToString();
            string updatePosDBQRY = "UPDATE GMCHARACTERS SET POSX = '"+posX+"', POSY = '"+posY+"', POSZ = '"+posZ+"', CURRENTWORLDMAP = '"+worldName+"', currentWorldID = '"+worldID+"' WHERE HANDLE = '"+handleLoc+"'";
            m_dbConnectionPtr.ExecuteQuery(updatePosDBQRY,nativeConnection);

			//send request
			m_conHelper.SendCommandToServer(       
				serverName,
				"6",
				m_username.Text,//param1
				worldName,//param2
				posX,//param3
				posY,//param4
				posZ,//param5
				repRequester,
				connectionToServerDB);
			//wait for response
			nativeConnection.Close();
			connectionToServerDB.Close();
		}

		private void SetStatBtn_Click(object sender, System.EventArgs e)
		{
            if(m_statList.SelectedIndex < 0)
			   return;

			if(m_currentPlayersList.SelectedIndex < 0)
				return;

            string serverName = (string)Session["ServerName"];
			//SqlConnection nativeConnection = m_dbConnectionPtr.CreateNativeConnectionObject();
            SqlConnection nativeConnection = m_conHelper.GetServerHUBConnection(serverName);//aquire target server db
			if(nativeConnection == null)
				return;

			string handleLoc = m_currentPlayersList.SelectedValue.ToString();
            string statType = m_statList.SelectedValue.ToString();
 
			string updateSql = "Update gmStats set currentAmount = '"+m_newStatValue.Text+"' where handle = '"+handleLoc+"' and statType = '"+statType+"'";
            m_dbConnectionPtr.ExecuteQuery(updateSql,nativeConnection);
			RefreshPlayerStats(nativeConnection);

			nativeConnection.Close();
		}

		private void SetSkillBtn_Click(object sender, System.EventArgs e)
		{
			if(m_skillList.SelectedIndex < 0)
				return;

			if(m_currentPlayersList.SelectedIndex < 0)
				return;

            string serverName = (string)Session["ServerName"];
			//SqlConnection nativeConnection = m_dbConnectionPtr.CreateNativeConnectionObject();
            SqlConnection nativeConnection = m_conHelper.GetServerHUBConnection(serverName);//aquire target server db
			if(nativeConnection == null)
				return;

			string handleLoc = m_currentPlayersList.SelectedValue.ToString();
			string skillType = m_skillList.SelectedValue.ToString();
 
			string updateSql = "Update gmSkills set currentExperience = '"+m_newSkillVal.Text+"' where handle = '"+handleLoc+"' and skillType = '"+skillType+"'";
			m_dbConnectionPtr.ExecuteQuery(updateSql,nativeConnection);
			RefreshPlayerSkills(nativeConnection);

			nativeConnection.Close();
		}

		private void BanToggleBtn_Click(object sender, System.EventArgs e)
		{
	
		}

		private void GMToggleBtn_Click(object sender, System.EventArgs e)
		{
			
		}

	

		private void DisconnectBtn_Click(object sender, System.EventArgs e)
		{
			//m_currentPlayersList.ClearSelection();
			string serverName   = (string)Session["ServerName"];
			string repRequester = (string)Session["user"];
			//aquire target server db
			SqlConnection connectionToServerDB = m_conHelper.GetServerConnection(serverName);
			if(connectionToServerDB == null)
				return;
            
			//send request
			m_conHelper.SendCommandToServer(
				serverName,
				"16",//disconnect force
				m_username.Text,
				"",
				"",
				"",
				"",
				repRequester,
				connectionToServerDB);
			connectionToServerDB.Close();
		}

		private void SetBankCreditsBtn_Click(object sender, System.EventArgs e)
		{
		
		}

		private void m_currentPlayersList_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			m_currentDataList.Items.Clear();
			m_currentBankInvenList.Items.Clear();
			m_skillList.Items.Clear();
			m_statList.Items.Clear();
			//m_currentPlayersList.ClearSelection();
			string serverName   = (string)Session["ServerName"];
			string repRequester = (string)Session["user"];
			//aquire target server db
			//SqlConnection connectionToServerDB = m_dbConnectionPtr.CreateNativeConnectionObject();
            SqlConnection connectionToServerDB = m_conHelper.GetServerHUBConnection(serverName);//aquire target server db
			if(connectionToServerDB == null)
				return;

			if(m_currentPlayersList.SelectedIndex < 0)
				return;

			RefreshPlayerInventories(connectionToServerDB);
			RefreshPlayerStats(connectionToServerDB);
			RefreshPlayerSkills(connectionToServerDB);
			
			
			connectionToServerDB.Close();
		}
        protected void AquireAcBtn_Click1(object sender, EventArgs e)
        {
            //Request Characters, invis stuff
            HideAllDynControls();
            m_currentPlayersList.Items.Clear();
            string serverName = (string)Session["ServerName"];
            string repRequester = (string)Session["user"];
            //aquire target server db
            //SqlConnection connectionToServerDB = m_dbConnectionPtr.CreateNativeConnectionObject();
            SqlConnection connectionToServerDB = m_conHelper.GetServerHUBConnection(serverName);//aquire target server db
            if (connectionToServerDB == null)
                return;


            SqlDataReader readerPtr = m_dbConnectionPtr.sqlQuery("Select charName,handle from gmCharacters where username = '" + m_username.Text + "'", connectionToServerDB);
            while (readerPtr.Read())
            {
                ListItem lItem = new ListItem(readerPtr.GetSqlValue(0).ToString(), readerPtr.GetSqlValue(1).ToString());
                m_currentPlayersList.Items.Add(lItem);
            }
            readerPtr.Close();


            connectionToServerDB.Close();
        }
}

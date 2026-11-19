    using System;
	using System.Data;
	using System.Drawing;
	using System.Web;
	using System.Web.UI.WebControls;
	using System.Web.UI.HtmlControls;
	using System.Data.SqlClient;
	using System.Threading;

	/// <summary>
	///		Summary description for IGNEntityBaseControl.
	/// </summary>
	public partial class IGNEntityBaseControl : System.Web.UI.UserControl
	{
        DatabaseConnectionModule dataCon = new DatabaseConnectionModule();
		ConnectionHelper m_conHelper = new ConnectionHelper();
		LogSystem m_logReporter = new LogSystem();
        common m_common = new common();

		private void Page_Load(object sender, System.EventArgs e)
		{
 
			if(Session["SPAWNID"] == null)
			{
				this.Visible = false;
				return;
			}

			if(Session["QUESTMODEON"] != null)
			{
				if(Session["QUESTMODEON"].ToString() != "0")
				{
					this.Visible = false;
					return;
				}
			}

            string organizeID = "";
            if (Request.QueryString["expID"] != null)
                organizeID = Request.QueryString["expID"].ToString().Replace("'", "");

			 
			string spawnID = Session["SPAWNID"].ToString();
            this.Visible = true;
			
			if(InteractionTypeDropList.Items.Count > 0)
			   return;

			int indexTemp = 0;
			ListItem iItem = new ListItem("Chest/Container",indexTemp.ToString());
			InteractionTypeDropList.Items.Add(iItem);
            indexTemp++;
			ListItem iItem2 = new ListItem("AI Random speech",indexTemp.ToString());
			InteractionTypeDropList.Items.Add(iItem2);
			indexTemp++;
			ListItem iItem3 = new ListItem("AI Sequenced speech",indexTemp.ToString());
			InteractionTypeDropList.Items.Add(iItem3);
			indexTemp++;
			ListItem iItem4 = new ListItem("AI Selection Action",indexTemp.ToString());
			InteractionTypeDropList.Items.Add(iItem4);
			indexTemp++;
			ListItem iItem5 = new ListItem("Arena Bookie",indexTemp.ToString());
			InteractionTypeDropList.Items.Add(iItem5);
			indexTemp++;
			ListItem iItem6 = new ListItem("Bounty NPC",indexTemp.ToString());
			InteractionTypeDropList.Items.Add(iItem6);

            indexTemp = 0;
            ListItem iItem7 = new ListItem("Off",indexTemp.ToString());
            m_autoTriggerDropList.Items.Add(iItem7);
            indexTemp++;
            ListItem iItem8 = new ListItem("Constant", indexTemp.ToString());
            m_autoTriggerDropList.Items.Add(iItem8);
            indexTemp++;
            ListItem iItem9 = new ListItem("One time per level", indexTemp.ToString());
            m_autoTriggerDropList.Items.Add(iItem9);
            indexTemp++;
            ListItem iItem10 = new ListItem("Once while in radius", indexTemp.ToString());
            m_autoTriggerDropList.Items.Add(iItem10);
            indexTemp++;

            SqlConnection nativeConnection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());


            //folder move options in list
            SqlDataReader dbReader = dataCon.sqlQuery("SELECT FOLDERNAME,ORGANIZEGROUPID FROM gm_IGNFolders ORDER BY ORGANIZEGROUPID", nativeConnection);
            while (dbReader.Read())
            {
                string folderName = dataCon.SanitizeOutHtml(dbReader.GetSqlValue(0).ToString());
                string organizeGroupID = dbReader.GetSqlValue(1).ToString();

                ListItem itemTemp = new ListItem(folderName, organizeGroupID);
                m_folderToMoveToList.Items.Add(itemTemp);
            }
            dbReader.Close();

            m_folderToMoveToList.SelectedValue = organizeID;


			//edb List
			//edbDropList
			edbDropList.Items.Clear();
			string sqlX = "SELECT ENTITYNAME,EDB_ID FROM GM_EDB ORDER BY EDB_ID";
			SqlDataReader edbReader = dataCon.sqlQuery(sqlX,nativeConnection);
			while(edbReader.Read())
			{
				ListItem lItem = new ListItem(edbReader.GetSqlValue(0).ToString(),edbReader.GetSqlValue(1).ToString());
				edbDropList.Items.Add(lItem);
			}
			edbReader.Close();
			//world Channel Select Drop List
			if(Session["ServerName"] != null)
			{
				ConnectionHelper conHelper = new ConnectionHelper();
				SqlConnection svConnect = conHelper.GetServerConnection(Session["ServerName"].ToString());
				SelectWorldChannelDropList.Items.Add("Get World Channel from List.");
				SelectWorldChannelDropList.SelectedIndex = 0;
				sqlX = "SELECT WORLDNAME,WORLDCHANNEL FROM tblSupportedWorlds";
				SqlDataReader svReader = dataCon.sqlQuery(sqlX,svConnect);
				while(svReader.Read())
				{
					ListItem lItem = new ListItem(svReader.GetSqlValue(0).ToString(),svReader.GetSqlValue(1).ToString());
					SelectWorldChannelDropList.Items.Add(lItem);
				}
				svReader.Close();
				svConnect.Close();
			}


			string sqlQ = "SELECT generatorname, displayname, respawnDuration, worldChannel, edbID, PosX, PosY, PosZ, CurAnimation,"+
                          " Glid, RequiredKey, RotationAdjust, interactRadius, autoTrigger, dimCfgCount, "+
                          "itemType, chatStringCount, randomAnimation, randomExpEffect, randomInterval, mountoption,"+
                          " noResponseMsg, sightRange, botbrain  FROM GM_IGNENTITY WHERE SPAWNID = '"+spawnID+"'";
			SqlDataReader dReader = dataCon.sqlQuery(sqlQ,nativeConnection);
			if(dReader.Read())
			{
				generatorNameEdit.Text			= dataCon.SanitizeOutHtml(dReader.GetSqlValue(0).ToString());
				DisplayedName.Text				= dataCon.SanitizeOutHtml(dReader.GetSqlValue(1).ToString());
				RespawnDuration.Text			= dReader.GetSqlValue(2).ToString();
				WorldChannel.Text				= dReader.GetSqlValue(3).ToString();
				EdbID.Text						= dReader.GetSqlValue(4).ToString();
				edbDropList.SelectedValue       = dReader.GetSqlValue(4).ToString();
				PosX.Text						= dReader.GetSqlValue(5).ToString();
				PosY.Text						= dReader.GetSqlValue(6).ToString();
				PosZ.Text						= dReader.GetSqlValue(7).ToString();
				CurAnimation.Text				= dReader.GetSqlValue(8).ToString();
				RequiredKey.Text				= dReader.GetSqlValue(10).ToString();
				RotationAdjust.Text				= dReader.GetSqlValue(11).ToString();
				InteractRadius.Text				= dReader.GetSqlValue(12).ToString();
				AutoTrigger.Text				= dReader.GetSqlValue(13).ToString();
				ItemType.Text					= dReader.GetSqlValue(15).ToString();
                m_autoTriggerDropList.SelectedIndex = Convert.ToInt32(dReader.GetSqlValue(13).ToString());
				InteractionTypeDropList.SelectedValue = dReader.GetSqlValue(15).ToString();
				RandomAnimation.Text			= dReader.GetSqlValue(17).ToString();
				RandomExpEffect.Text			= dReader.GetSqlValue(18).ToString();
				RandomTimeInterval.Text			= dReader.GetSqlValue(19).ToString();
				MountOption.Text				= dReader.GetSqlValue(20).ToString();
				noResponseMsg.Text				= dataCon.SanitizeOutHtml(dReader.GetSqlValue(21).ToString());
				SightRange.Text					= dReader.GetSqlValue(22).ToString();

				m_botBrain.Text                 = dReader.GetSqlValue(23).ToString();
			}
			dReader.Close();
			//in list entity inventory
			InListInventory(spawnID,nativeConnection,0);
            // dim cfgs
			//DimCfgList.Items.Clear();
			sqlQ = "SELECT SPAWNID,DIMCFG,INDEXDIM FROM GM_IGNDIMCFGS WHERE SPAWNID = '"+spawnID+"' ORDER BY INDEXDIM";
			SqlDataReader dimReader = dataCon.sqlQuery(sqlQ,nativeConnection);
			while(dimReader.Read())
			{
                ListItem lItem = new ListItem(dimReader.GetSqlValue(2).ToString()+")DIMCFG:"+dimReader.GetSqlValue(1).ToString(),dimReader.GetSqlValue(2).ToString());
				DimCfgList.Items.Add(lItem);
			}
            dimReader.Close();
            
  

			nativeConnection.Close();
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
			this.ApplyChanges.Click += new System.EventHandler(this.ApplyChanges_Click);
			this.QuestPanelBtn.Click += new System.EventHandler(this.QuestPanelBtn_Click);
			this.NewIGNEntity.Click += new System.EventHandler(this.NewIGNEntity_Click);
			this.SelectWorldChannelDropList.SelectedIndexChanged += new System.EventHandler(this.SelectWorldChannelDropList_SelectedIndexChanged);
			this.AquireFromCharName.Click += new System.EventHandler(this.AquireFromCharName_Click);
			this.edbDropList.SelectedIndexChanged += new System.EventHandler(this.edbDropList_SelectedIndexChanged);
			this.EntityInventoryList.SelectedIndexChanged += new System.EventHandler(this.EntityInventoryList_SelectedIndexChanged);
			this.AddEntityInvItemBtn.Click += new System.EventHandler(this.AddEntityInvItemBtn_Click);
			this.DeleteEntityInventoryBtn.Click += new System.EventHandler(this.DeleteEntityInventoryBtn_Click);
			this.ArmedCheck.CheckedChanged += new System.EventHandler(this.ArmedCheck_CheckedChanged);
			this.SearchInvBtn.Click += new System.EventHandler(this.SearchInvBtn_Click);
			this.SetDimCfg.Click += new System.EventHandler(this.SetDimCfg_Click);
			this.Load += new System.EventHandler(this.Page_Load);

		}
		#endregion

		private void ApplyChanges_Click(object sender, System.EventArgs e)
		{
			
			if(Session["SPAWNID"] == null)
			   return;

			if(Session["EXPID"] == null)
			   return;
			 
		    string spawnID = Session["SPAWNID"].ToString();

			string expID = Session["EXPID"].ToString();

            SqlConnection nativeConnection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());

			m_logReporter.LogEvent("AX QUEST UPDATE","Updating quest entity("+generatorNameEdit.Text+") user:"+Session["user"].ToString(),nativeConnection);

            string sqlQ = "Update GM_IGNENTITY set GENERATORNAME= '" + dataCon.SanitizeIn(generatorNameEdit.Text) + "'," +
													"respawnDuration= '" + RespawnDuration.Text + "'," +
													"worldChannel= '" + WorldChannel.Text + "'," +
													"edbID= '" + edbDropList.SelectedValue + "'," +
													"PosX= '" + PosX.Text + "'," +
													"PosY= '" + PosY.Text + "'," +
													"PosZ= '" + PosZ.Text + "'," +
													"CurAnimation= '" + CurAnimation.Text + "'," +
													"RequiredKey= '" + RequiredKey.Text + "'," +
													"RotationAdjust= '" + RotationAdjust.Text + "'," +
													"interactRadius= '" + InteractRadius.Text + "'," +
                                                    "autoTrigger= '" + m_autoTriggerDropList.SelectedIndex + "'," +
													"itemType= '" + InteractionTypeDropList.SelectedValue + "'," +
													"randomAnimation= '" + RandomAnimation.Text + "'," +
													"randomExpEffect= '" + RandomExpEffect.Text + "'," +
													"randomInterval= '" + RandomTimeInterval.Text + "'," +
													"mountoption= '" + MountOption.Text + "'," +
				                                    "dimCfgCount= '" + DimCfgList.Items.Count.ToString() + "'," +
													"noResponseMsg= '" + dataCon.SanitizeIn(noResponseMsg.Text) + "'," +
				                                    "sightRange= '" + dataCon.SanitizeIn(SightRange.Text) + "'," +
				                                    "botBrain= '" + dataCon.SanitizeIn(m_botBrain.Text) + "'," +
													"DISPLAYNAME= '"+dataCon.SanitizeIn(DisplayedName.Text)+"' WHERE SPAWNID = '"+spawnID+"'";
			 

	
			dataCon.ExecuteQuery(sqlQ,nativeConnection);
			dataCon.ExecuteQuery("DELETE FROM GM_IGNDIMCFGS WHERE SPAWNID = '"+spawnID+"'",nativeConnection);
			for(int loop=0;loop<DimCfgList.Items.Count;loop++)
			{
				
				sqlQ = "insert into GM_IGNDIMCFGS (SPAWNID,DIMCFG,INDEXDIM) Values('"+spawnID+"','"+ParseDimIndex(DimCfgList.Items[loop].Text)+"','"+loop.ToString()+"')";
			    dataCon.ExecuteQuery(sqlQ,nativeConnection);
			}
		
			nativeConnection.Close();


			Response.Redirect("IGN_QuestMenuPage.aspx?expID=" + expID.ToString() + "&SpawnID=" + spawnID.ToString());	
		}

		private void NewIGNEntity_Click(object sender, System.EventArgs e)
		{
		
		}

		private void InListInventory(string spawnID,SqlConnection nativeConnection,int sel)
		{
			string glid = "";
			string itemName = "";
			int armed = 0;
            SqlConnection innerConnection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
			string sqlQ = "SELECT GLID,ARMED FROM GM_IGNINVENTORY WHERE TYPE = '0' AND SPAWNID = '"+spawnID+"'";
			SqlDataReader invReader = dataCon.sqlQuery(sqlQ,nativeConnection);
			EntityInventoryList.Items.Clear();
			while(invReader.Read())
			{
				glid = invReader.GetSqlValue(0).ToString();

				armed = Convert.ToInt32(invReader.GetSqlValue(1).ToString());

                itemName = m_common.GetItemNameByGil(glid, innerConnection);

				if(armed == 1)
					EntityInventoryList.Items.Add(itemName + " Armed");
				else
					EntityInventoryList.Items.Add(itemName);
			}
			invReader.Close();
			innerConnection.Close();

			if(sel > -1)
			   EntityInventoryList.SelectedIndex = sel;
		}

		private void AddEntityInvItemBtn_Click(object sender, System.EventArgs e)
		{
			if(Session["SPAWNID"] == null)
			   return;

           SqlConnection innerConnection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
			string sqlQ = "INSERT INTO GM_IGNINVENTORY(SPAWNID,GLID,TYPE,questIndex) VALUES('"+Session["SPAWNID"].ToString()+"','"+ItemGILResultList.SelectedValue+"','0','-1')";
			dataCon.ExecuteQuery(sqlQ,innerConnection);
            InListInventory(Session["SPAWNID"].ToString(),innerConnection,EntityInventoryList.SelectedIndex);
			innerConnection.Close();
		}

		private void DeleteEntityInventoryBtn_Click(object sender, System.EventArgs e)
		{
			if(Session["SPAWNID"] == null)
				return;

            SqlConnection innerConnection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
			string sqlQ = "SELECT GLID,ARMED,TYPE FROM GM_IGNINVENTORY WHERE TYPE = '0' AND SPAWNID = '"+Session["SPAWNID"].ToString()+"'";
			SqlDataReader invReader = dataCon.sqlQuery(sqlQ,innerConnection);
			int trace = 0;
			while(invReader.Read())
			{
				if(trace == EntityInventoryList.SelectedIndex)
				{
				   string glid			= invReader.GetSqlValue(0).ToString();
				   string armed		    = invReader.GetSqlValue(1).ToString();
                   string type			= invReader.GetSqlValue(2).ToString();
				   invReader.Close();

				   sqlQ = "DELETE FROM GM_IGNINVENTORY WHERE SPAWNID = '"+Session["SPAWNID"].ToString()+"' AND GLID = '"+glid+"' AND TYPE = '"+type+"' AND ARMED = '"+armed+"'";
				   dataCon.ExecuteQuery(sqlQ,innerConnection);
				   InListInventory(Session["SPAWNID"].ToString(),innerConnection,EntityInventoryList.SelectedIndex-1);
				   innerConnection.Close();
				   return;
				}
			    trace++;
			}
			invReader.Close();
			innerConnection.Close();
		}

		private void ArmedCheck_CheckedChanged(object sender, System.EventArgs e)
		{
			if(Session["SPAWNID"] == null)
				return;

            SqlConnection innerConnection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
			string sqlQ = "SELECT GLID,ARMED,TYPE FROM GM_IGNINVENTORY WHERE TYPE = '0' AND SPAWNID = '"+Session["SPAWNID"].ToString()+"'";
			SqlDataReader invReader = dataCon.sqlQuery(sqlQ,innerConnection);
			int trace = 0;
			while(invReader.Read())
			{
				if(trace == EntityInventoryList.SelectedIndex)
				{
					string glid			= invReader.GetSqlValue(0).ToString();
					string armed		= invReader.GetSqlValue(1).ToString();
					string type			= invReader.GetSqlValue(2).ToString();
					invReader.Close();

					string newArmedAState = "0";
					if(ArmedCheck.Checked == true)
					   newArmedAState = "1";

					sqlQ = "UPDATE GM_IGNINVENTORY SET ARMED = '"+newArmedAState+"'WHERE SPAWNID = '"+Session["SPAWNID"].ToString()+"' AND GLID = '"+glid+"' AND TYPE = '"+type+"' AND ARMED = '"+armed+"'";
					dataCon.ExecuteQuery(sqlQ,innerConnection);
					InListInventory(Session["SPAWNID"].ToString(),innerConnection,EntityInventoryList.SelectedIndex);
					innerConnection.Close();
					return;
				}
				trace++;
			}
			invReader.Close();
			innerConnection.Close();
		}

		

		private void EntityInventoryList_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			if(Session["SPAWNID"] == null)
			{
				return;
			}

			int selectedObject = EntityInventoryList.SelectedIndex;
            SqlConnection innerConnection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
			string sqlQ = "SELECT GLID,ARMED FROM GM_IGNINVENTORY WHERE TYPE = '0' AND SPAWNID = '"+Session["SPAWNID"].ToString()+"'";
			SqlDataReader invReader = dataCon.sqlQuery(sqlQ,innerConnection);
			int trace = 0;
			while(invReader.Read())
			{
				if(selectedObject == trace)
				{
				   if(Convert.ToInt32(invReader.GetSqlValue(1).ToString())==1)
                      ArmedCheck.Checked = true;
					else
				      ArmedCheck.Checked = false;
				}
				trace++;
			}
			invReader.Close();
			innerConnection.Close();
		}

		private void SetDimCfg_Click(object sender, System.EventArgs e)
		{
		
            DimCfgList.SelectedItem.Value	= DimCfgList.SelectedIndex.ToString();
            DimCfgList.SelectedItem.Text	= DimCfgList.SelectedIndex.ToString()+")DIMCFG:"+DimCfgEdit.Text;
			
		}

		private void edbDropList_SelectedIndexChanged(object sender, System.EventArgs e)
		{
            SqlConnection innerConnection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
			string sqlX = "SELECT ENTITYNAME,EDB_ID,DIMCOUNT FROM GM_EDB WHERE EDB_ID = '"+edbDropList.SelectedValue+"'";
			SqlDataReader edbReader = dataCon.sqlQuery(sqlX,innerConnection);
			if(edbReader.Read())
			{
				int newdimCount = Convert.ToInt32(edbReader.GetSqlValue(2).ToString());
				DimCfgList.Items.Clear();
				string curDim = "0";
				for(int i=0;i<newdimCount;i++)
				{
					ListItem lItem = new ListItem(i.ToString()+")DIMCFG:"+curDim,curDim);
					DimCfgList.Items.Add(lItem);
				}
				/*if(newdimCount < DimCfgList.Items.Count)
				{//process lesser difference
				   int diff = DimCfgList.Items.Count - newdimCount;
				   int startPoint = DimCfgList.Items.Count-1;
					for(int i=0;i<diff;i++)
                       DimCfgList.Items.RemoveAt(startPoint--);

				}//end process lesser difference
				else
				{//process greater difference
                    int diff = newdimCount - DimCfgList.Items.Count;
					int startIndex = DimCfgList.Items.Count;
					string curDim = "0";
					for(int i=0;i<diff;i++)
					{
						ListItem lItem = new ListItem(startIndex.ToString()+")DIMCFG:"+curDim,curDim);
						DimCfgList.Items.Add(lItem);
						startIndex++;
					}
				}//end process greater difference*/
				edbReader.Close();
				innerConnection.Close();
				return;
			}
			
			edbReader.Close();
			innerConnection.Close();
			return;
		}

		private void SelectWorldChannelDropList_SelectedIndexChanged(object sender, System.EventArgs e)
		{
		    WorldChannel.Text = SelectWorldChannelDropList.SelectedValue;
		}

		private string ParseDimIndex(string desctext)
		{
            char [] arrayText = desctext.ToCharArray();
			int length = desctext.Length;
			string build = "";
			for(int i=length-1;i>-1;i--)
			{
                if(arrayText[i] == ':')
				   break;

				build = arrayText[i].ToString() + build;
			}
			return build;
		}

		protected void SearchInvBtn_Click(object sender, System.EventArgs e)
		{
            SqlConnection innerConnection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());

            m_common.FillListItemsBySearchTerm(searchCriteria.Text, ref ItemGILResultList, innerConnection);
            
            innerConnection.Close();

		}

		private void AquireFromCharName_Click(object sender, System.EventArgs e)
		{
		    charaquisitionError.Text = "";
			if(Session["ServerName"] == null)
			{
				charaquisitionError.Text = "must select a server";
				return;
			}
			//m_currentPlayersList.ClearSelection();
			string serverName   = (string)Session["ServerName"];
			string repRequester = (string)Session["user"];
			//aquire target server db
			SqlConnection connectionToServerDB = m_conHelper.GetServerConnection(serverName);
			if(connectionToServerDB == null)
				return;
            
			//send request
			m_conHelper.SendCommandToServer(       serverName,
				"23",//player info request
				charName.Text,
				"",
				"",
				"",
				"",
				repRequester,
				connectionToServerDB);
			//wait for response
			bool foundEndCap = false;
			for(int loop=0;loop<10;loop++)
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
				SqlDataReader errorReader = dataCon.sqlQuery("Select peram1,peram2,peram3,peram4 from tblServerResponse where peram6 = '"+repRequester+"' and peram5 ='completed'",connectionToServerDB);
				while(errorReader.Read())
				{
					if(errorReader.GetSqlValue(0).ToString() == "FAILED")
						charaquisitionError.Text = "Cannot Find character...";	
					else
					{
                        PosX.Text = errorReader.GetSqlValue(0).ToString();
						PosY.Text = errorReader.GetSqlValue(1).ToString();
						PosZ.Text = errorReader.GetSqlValue(2).ToString();
						WorldChannel.Text = errorReader.GetSqlValue(3).ToString();
                        
                        charaquisitionError.Text = "Aquired...";	
					}
				}
				errorReader.Close();
				
				//flush all requestors
				dataCon.ExecuteQuery("delete from tblServerResponse where peram6 = '"+repRequester+"'",connectionToServerDB);
			}//end process all and then delete response
			connectionToServerDB.Close();
		}

		private void QuestPanelBtn_Click(object sender, System.EventArgs e)
		{
		    Session["QUESTMODEON"] = 1;
		}

        protected void m_folderToMoveToList_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (Request.QueryString["expID"] == null)
                return;

            if (Request.QueryString["SpawnID"] == null)
                return;

            if (Session["ServerName"] == null)
            {
                charaquisitionError.Text = "must select a server";
                return;
            }
            //m_currentPlayersList.ClearSelection();

            SqlConnection hubConnection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());

            string organizeID = Request.QueryString["expID"].ToString().Replace("'", "");
            string spawnID = Request.QueryString["SpawnID"].ToString().Replace("'", "");

            dataCon.ExecuteQuery("UPDATE GM_IGNENTITY SET ORGANIZEGROUPID = '" + m_folderToMoveToList.SelectedValue + "' WHERE SPAWNID = '" + spawnID + "'", hubConnection);

            hubConnection.Close();

            Response.Redirect("IGN_QuestMenuPage.aspx?expID=" + m_folderToMoveToList.SelectedValue + "&SpawnID=" + spawnID);
            //IGN_QuestMenuPage.aspx?expID=18&SpawnID=535
        }
}

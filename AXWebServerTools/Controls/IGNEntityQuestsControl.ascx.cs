	using System;
	using System.Data;
	using System.Drawing;
	using System.Web;
	using System.Web.UI.WebControls;
	using System.Web.UI.HtmlControls;
	using System.Data.SqlClient;

	/// <summary>
	///		Summary description for IGNEntityQuestsControl.
	/// </summary>
	public partial class IGNEntityQuestsControl : System.Web.UI.UserControl
	{
		int m_questCount = 0;
		DatabaseConnectionModule dataCon = new DatabaseConnectionModule();
		LogSystem m_logReporter = new LogSystem();
        ConnectionHelper m_conHelper = new ConnectionHelper();
        common m_common = new common();

		private void Page_Load(object sender, System.EventArgs e)
		{
			

			if(Session["expID"] == null)
			{
				this.Visible = false;
				return;
			}

			if(Session["SPAWNID"] == null)
			{
				this.Visible = false;
				return;
			}

			if(Session["QUESTMODEON"] == null)
			{
			   this.Visible = false;
			   return;
			}

			if(Session["QUESTMODEON"].ToString() != "1")
			{
                this.Visible = false;
				return;
			}

			if(Request.QueryString["QUESTSELECTION"] != null)
			{
				Session["QUESTSELECTION"] = Request.QueryString["QuestSelection"].ToString().Replace("'","");
			}
			else
			    Session["QUESTSELECTION"] = null;

			this.Visible = true;


			if(JournalCommandOption.Items.Count > 0)
				return;

            //manage check boxes
            string showRewardPanel = "0";
            if (Session["SHOWREWARDPANEL"] != null)
                showRewardPanel = Session["SHOWREWARDPANEL"].ToString();
            if (ShowRewardPanel.Checked == true || showRewardPanel == "1")
            {
                Session["SHOWREWARDPANEL"] = "1";
                RewardPanel.Visible = true;
                ShowRewardPanel.Checked = true;
            }
            else
            {
                Session["SHOWREWARDPANEL"] = "0";
                RewardPanel.Visible = false;
                ShowRewardPanel.Checked = false;
            }


            string showSpeechPanel = "0";
            if (Session["SHOWSPEECHPANEL"] != null)
                showSpeechPanel = Session["SHOWSPEECHPANEL"].ToString();
            if (ShowSpeechPanel.Checked == true || showSpeechPanel == "1")
            {
                SpeechPanel.Visible = true;
                ShowSpeechPanel.Checked = true;
                Session["SHOWSPEECHPANEL"] = "1";
            }
            else
            {
                SpeechPanel.Visible = false;
                ShowSpeechPanel.Checked = false;
                Session["SHOWSPEECHPANEL"] = "0";
            }

            string showRequiredPanel = "0";
            if (Session["SHOWREQUIREDPANEL"] != null)
                showRequiredPanel = Session["SHOWREQUIREDPANEL"].ToString();
            if (ShowRequiredItemsPanel.Checked == true || showRequiredPanel == "1")
            {
                RequiredPanel.Visible = true;
                ShowRequiredItemsPanel.Checked = true;
                Session["SHOWREQUIREDPANEL"] = "1";
            }
            else
            {
                RequiredPanel.Visible = false;
                ShowRequiredItemsPanel.Checked = false;
                Session["SHOWREQUIREDPANEL"] = "0";
            }


            string showJournalPanel = "0";
            if (Session["SHOWJOURNALPANEL"] != null)
                showJournalPanel = Session["SHOWJOURNALPANEL"].ToString();
            if (ShowJournalPanel.Checked == true || showJournalPanel == "1")
            {
                JournalPanel.Visible = true;
                ShowJournalPanel.Checked = true;
                Session["SHOWJOURNALPANEL"] = "1";
            }
            else
            {
                JournalPanel.Visible = false;
                ShowJournalPanel.Checked = false;
                Session["SHOWJOURNALPANEL"] = "0";
            }


            string showTargetDropPanel = "0";
            if (Session["SHOWTARGETDROPPANEL"] != null)
                showTargetDropPanel = Session["SHOWTARGETDROPPANEL"].ToString();
            if (CheckShowTragetDropPanel.Checked == true || showTargetDropPanel == "1")
            {
                TargetDropPanel.Visible = true;
                CheckShowTragetDropPanel.Checked = true;
                Session["SHOWTARGETDROPPANEL"] = "1";
            }
            else
            {
                TargetDropPanel.Visible = false;
                CheckShowTragetDropPanel.Checked = false;
                Session["SHOWTARGETDROPPANEL"] = "0";
            }


            string showSpawnPanel = "0";
            if (Session["SHOWSPAWNPANEL"] != null)
                showSpawnPanel = Session["SHOWSPAWNPANEL"].ToString();
            if (ShowSpawnPanel.Checked == true || showSpawnPanel == "1")
            {
                SpawnPanel.Visible = true;
                ShowSpawnPanel.Checked = true;
                Session["SHOWSPAWNPANEL"] = "1";
            }
            else
            {
                SpawnPanel.Visible = false;
                ShowSpawnPanel.Checked = false;
                Session["SHOWSPAWNPANEL"] = "0";
            }

			if(JournalKey.Items.Count > 0)
			    return;


            m_common.InListFunctions(ref ScriptFunctionList);
            InListMacros(ref ScriptMacroList);
            m_common.InListFunctions(ref RFunctionDropList);
			InListMacros(ref RMacroList);
            m_common.InListFunctions(ref rqFunctionList);
			InListMacros(ref rqMacroList);
			
			//inlist Constants
			JournalKey.Items.Add("NONE");
			JournalKey.Items.Add("Log Quest Description");
			JournalKey.Items.Add("Log Quest Completed (Keep Alive)");
			JournalKey.Items.Add("Log Both");
			JournalKey.Items.Add("Log Both (hide Required items)");
			JournalKey.Items.Add("Log Quest Completed (Complete It)");
			
			JournalCommandOption.Items.Add("NONE");
			JournalCommandOption.Items.Add("Access Store");
			JournalCommandOption.Items.Add("Access Bank");

            m_instanceGenList.Items.Add("NONE");
            m_instanceGenList.Items.Add("Group Based");
            m_instanceGenList.Items.Add("Arena Based");


            SqlConnection connectionLoc = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());


            if (m_dynWorldRulesDList.Items.Count == 0)
            {
                m_common.InListWorldRules(ref m_dynWorldRulesDList, connectionLoc);
                
            }


            
			InListQuests(Convert.ToInt32(Session["SPAWNID"].ToString()),connectionLoc);
			connectionLoc.Close();
            
		}


		private void InListQuests(int spawnID, SqlConnection connection )
		{
			m_miscTest.Text = "";
			m_glidDropLabel.Text = "";
			//PropertiesPanel.Visible = false;
			ApplyChanges.Visible = false;
            m_miscTest.Text = "";
			string spawnIDstring = spawnID.ToString();
			string expIDstring = Session["expID"].ToString();

			//ai configs

			TargetDropAICFGList.Items.Clear();
			SpawnAICFGdropList.Items.Clear();
			string sqlQ = "SELECT AIGENERATORNAME,GENINDEX FROM GM_AII";
			SqlDataReader aiReader = dataCon.sqlQuery(sqlQ,connection);
			ListItem itemstart1 = new ListItem("NONE","-1");
			TargetDropAICFGList.Items.Add(itemstart1);
			ListItem itemstart2 = new ListItem("NONE","-1");
			SpawnAICFGdropList.Items.Add(itemstart2);
			while(aiReader.Read())
			{
				ListItem item1 = new ListItem(aiReader.GetSqlValue(0).ToString(),aiReader.GetSqlValue(1).ToString());
				TargetDropAICFGList.Items.Add(item1);
				ListItem item2 = new ListItem(aiReader.GetSqlValue(0).ToString(),aiReader.GetSqlValue(1).ToString());
				SpawnAICFGdropList.Items.Add(item2);

			}
			aiReader.Close();

            //skills
			RequiredSkillTypeDropList.Items.Clear();
			ExpSkilltypeDropList.Items.Clear();
			sqlQ = "SELECT SKILLNAME,SKILLTYPE FROM GM_SKILLS";
			SqlDataReader skReader = dataCon.sqlQuery(sqlQ,connection);
			ListItem itemstart3 = new ListItem("NONE","-1");
			RequiredSkillTypeDropList.Items.Add(itemstart3);
			ListItem itemstart4 = new ListItem("NONE","-1");
			ExpSkilltypeDropList.Items.Add(itemstart4);
			while(skReader.Read())
			{
				ListItem item1 = new ListItem(skReader.GetSqlValue(0).ToString(),skReader.GetSqlValue(1).ToString());
				RequiredSkillTypeDropList.Items.Add(item1);
				ListItem item2 = new ListItem(skReader.GetSqlValue(0).ToString(),skReader.GetSqlValue(1).ToString());
				ExpSkilltypeDropList.Items.Add(item2);

			}
			skReader.Close();

			//edb 
			RaceSpecificRequiredList.Items.Clear();
			sqlQ = "SELECT ENTITYNAME,EDB_ID FROM GM_EDB";
			SqlDataReader edReader = dataCon.sqlQuery(sqlQ,connection);
			ListItem itemstart6 = new ListItem("NONE","-1");
			RaceSpecificRequiredList.Items.Add(itemstart6);
			while(edReader.Read())
			{
				ListItem item1 = new ListItem(edReader.GetSqlValue(0).ToString(),edReader.GetSqlValue(1).ToString());
				RaceSpecificRequiredList.Items.Add(item1);
			}
			edReader.Close();

            //spawn panel
            m_spawnPanelWorldDropList.Items.Clear();
            sqlQ = "SELECT CHANNEL, EXGNAME FROM SYSWORLDSTENCILS ORDER BY EXGNAME";
            SqlDataReader worldReader = dataCon.sqlQuery(sqlQ, connection);
            ListItem itemstart7 = new ListItem("NONE", "-1");
            m_spawnPanelWorldDropList.Items.Add(itemstart7);
            while (worldReader.Read())
            {
                string strDisplayText = worldReader.GetSqlValue(1).ToString() + " (" + worldReader.GetSqlValue(0).ToString() + ")";
                ListItem item1 = new ListItem(strDisplayText, worldReader.GetSqlValue(0).ToString());
                m_spawnPanelWorldDropList.Items.Add(item1);
            }
            worldReader.Close();

			string selectedQuest = "-1";
			if(Session["QUESTSELECTION"] != null)
			   selectedQuest = Session["QUESTSELECTION"].ToString();

           sqlQ = "SELECT QuestIndex, Title,QuestDescription, QuestCompletionMessage, SpawnID, SpawnID," +//5
				          "skillTypeBonus, expBonus, maxExpLimit, spawnAiCfg, spawnAiCfg, spawnPosX, spawnPosY, spawnPosZ,"+//13
                          "worldID, spawnedAiLifeLine, randomRewardType, spawnToLocationOverride, makeTargetRebirthPoint,"+//18
				          "raceSpecificAccess, optionalCommand, miscAction, progressionBasedByQuestJournal, cashReward,"+//23
				          "requiredSkillType, requiredSkillLevel, mobToHuntID, maxDropCount, dropGlid, dropPercentage,"+//29
				          "autoComplete, openDialogWindow, spawnOnFail, optionalInstruction, listProgressionOptions, undeletable,"+ //35
				          "mustOwnKingdomID, expIDRequired, completedKeyEnabled, portalVSMessage, clientScript, requiredscript, rewardscript, closeJournalEntryByName,"+//43
                          "clearInventory, deleteAllByRequiredGLID, deleteByLocationID, despawnFlag, timeStampFlag, requiredTimeStamp, expFailMessage, minDmgMod, maxDmgMod, flagHostileOnSuccess, SPAWNARRAYCOUNT, SPAWNARRAYOFFSET, SpawnRotationRange,spawnWorldName,instanceGenerateType,instanceGenerateParam,instanceGenerateParam2,instanceGenerateParam3,instanceGenerateParam4,deleteAllByRewardGLID" +//60
                          " FROM gm_IGNQuestData where spawnID = '"+spawnIDstring+"' order by questIndex";

			m_questCount = 0;
			SqlDataReader dbReader = dataCon.sqlQuery(sqlQ,connection);
			while(dbReader.Read())
			{
				if(dbReader.GetSqlValue(0).ToString() != selectedQuest)
				{
					//m_miscTest.Text += "<img src='images/led.jpg'><a href=" + "IGN_QuestMenuPage.aspx?expID=" + expIDstring + "&spawnid=" +spawnIDstring+ "&QUESTSELECTION=" +dbReader.GetSqlValue(0).ToString()+ ">&nbsp;" + "("+dbReader.GetSqlValue(0).ToString()+")" + dbReader.GetSqlValue(1).ToString() + "</a><br>";
					m_miscTest.Text += "<img src='images/led.jpg'><a href=" + "IGN_QuestMenuPage.aspx?expID=" + expIDstring + "&spawnid=" +spawnIDstring+ "&QUESTSELECTION=" +dbReader.GetSqlValue(0).ToString()+urlPersist()+ ">&nbsp;" + "("+dbReader.GetSqlValue(0).ToString()+")" + dbReader.GetSqlValue(1).ToString() + "</a><br>";
				}
				else
				{
					m_miscTest.Text += "<img src='images/yellowled.jpg'>&nbsp;<FONT COLOR = YELLOW><STRONG>" + "("+dbReader.GetSqlValue(0).ToString()+")" + dbReader.GetSqlValue(1).ToString().ToUpper() + "</STRONG></FONT><br>";
					//PropertiesPanel.Visible = true;
					ApplyChanges.Visible = true;
					//in list current quest data   RequiredSkillTypeDropList          
					title.Text								= dataCon.SanitizeOutHtml(dbReader.GetSqlValue(1).ToString());
					QuestFailedMsg.Text						= dataCon.SanitizeOutHtml(dbReader.GetSqlValue(2).ToString());
					CompletedMessage.Text					= dataCon.SanitizeOutHtml(dbReader.GetSqlValue(3).ToString());
					OptionalInstruction.Text				= dataCon.SanitizeOutHtml(dbReader.GetSqlValue(33).ToString());
					CashReward.Text							= dbReader.GetSqlValue(23).ToString();
					RequiredSkillLevel.Text					= dbReader.GetSqlValue(25).ToString();
					ClientScriptEdit.Text                   = dbReader.GetSqlValue(40).ToString();
					requiredScriptEdit.Text                 = dataCon.SanitizeOutHtml(dbReader.GetSqlValue(41).ToString());
					rewardScriptEdit.Text                   = dataCon.SanitizeOutHtml(dbReader.GetSqlValue(42).ToString());
					m_closeJnlEntryByName.Text              = dataCon.SanitizeOutHtml(dbReader.GetSqlValue(43).ToString());

					m_expFailMsg.Text                       = dataCon.SanitizeOutHtml(dbReader.GetSqlValue(50).ToString());
					m_maxDamage.Text                        = dbReader.GetSqlValue(52).ToString();
					m_minDamage.Text                        = dbReader.GetSqlValue(51).ToString();
                    m_aiOptionArrayCount.Text               = dbReader.GetSqlValue(54).ToString();
                    m_aiArrayOptionOffset.Text              = dbReader.GetSqlValue(55).ToString();
                    SpawnRotation.Text                      = dbReader.GetSqlValue(56).ToString();


					try
					{
						RequiredSkillTypeDropList.SelectedValue	= dbReader.GetSqlValue(24).ToString();
					}
					catch{}
					ExperienceBonus.Text					= dbReader.GetSqlValue(7).ToString();
					ExpSkilltypeDropList.SelectedValue   	= dbReader.GetSqlValue(6).ToString();
					ExperienceMaxGain.Text					= dbReader.GetSqlValue(8).ToString();
					RequiredExpansionLevel.Text				= dbReader.GetSqlValue(37).ToString();
					mustOwnKingdomID.Text					= dbReader.GetSqlValue(36).ToString();
					RaceSpecificRequiredList.SelectedValue  = dbReader.GetSqlValue(19).ToString();

					TargetDropAICFGList.SelectedValue       = dbReader.GetSqlValue(26).ToString();
					aiTargetConfiguration.Text              = dbReader.GetSqlValue(26).ToString();
					dropGlid.Text                           = dbReader.GetSqlValue(28).ToString();
					//decorate Glid
					
					
					
					maxDropCount.Text                       = dbReader.GetSqlValue(27).ToString();
					dropTargetPercentage.Text               = dbReader.GetSqlValue(29).ToString();

					//SpawnAiCfg.Text							= dbReader.GetSqlValue(9).ToString();
					SpawnAICFGdropList.SelectedValue        = dbReader.GetSqlValue(10).ToString();
					SpawnLifeDuration.Text					= dbReader.GetSqlValue(15).ToString();
					SpawnWorldChannel.Text					= dbReader.GetSqlValue(14).ToString();
					//PortalLimitVS.Text						= dbReader.GetSqlValue(37).ToString();
					PortalLimitVSMsg.Text					= dbReader.GetSqlValue(39).ToString();//missing
					PosX.Text								= dbReader.GetSqlValue(11).ToString();
					PosY.Text								= dbReader.GetSqlValue(12).ToString();
					PosZ.Text								= dbReader.GetSqlValue(13).ToString();
					m_timeStampRequired.Text				= dbReader.GetSqlValue(49).ToString();
                    m_instanceGenParam.Text                 = dbReader.GetSqlValue(59).ToString();
                    m_instanceGenParam2.Text                = dbReader.GetSqlValue(60).ToString();
                    m_instanceGenParam3.Text                = dbReader.GetSqlValue(61).ToString();
                    m_instanceGenParam4.Text                = dbReader.GetSqlValue(62).ToString();

                    string spnWorldName = dbReader.GetSqlValue(57).ToString();
                    
                    for (int selLoop = 0; selLoop < m_spawnPanelWorldDropList.Items.Count; selLoop++)
                    {
                        char[] SplitParamsLocal = " ".ToCharArray();
                        string[] WorldSelectionPartsLocal = m_spawnPanelWorldDropList.Items[selLoop].Text.Split(SplitParamsLocal);

                        if (String.Compare(WorldSelectionPartsLocal[0].Trim(), spnWorldName, true) == 0)
                        {
                            m_spawnPanelWorldDropList.SelectedIndex = selLoop;
                            break;
                        }
                    }

                    m_instanceGenList.SelectedIndex         = Convert.ToInt32(dbReader.GetSqlValue(58).ToString());
                    switch (m_instanceGenList.SelectedIndex)
                    {
                        case 0://none
                            m_instanceParam.Text        = "N/A";
                            m_instanceParam2.Text       = "N/A";
                            m_instanceParam3.Text       = "N/A";
                            m_instanceParam4.Text       = "N/A";
                        break;
                        case 1://group
                            m_instanceParam.Text        = "N/A";
                            m_instanceParam2.Text       = "N/A";
                            m_instanceParam3.Text       = "N/A";
                            m_instanceParam4.Text       = "N/A";
                        break;
                        case 2://arena
                            m_instanceParam.Text        = "Max Player Count before autostarting.";
                            m_instanceParam2.Text       = "Internal Use only";
                            m_instanceParam3.Text       = "Minimum player count before autostart";
                            m_instanceParam4.Text       = "Duration after minimum player count hit (Milliseconds)";
                        break;
                    }

                

					if(dbReader.GetSqlValue(53).ToString() == "0")
						m_flagHostileOnSuccessCheck.Checked					= false;
					else
						m_flagHostileOnSuccessCheck.Checked					= true;

					if(dbReader.GetSqlValue(47).ToString() == "0")
						m_despawnFlag.Checked					= false;
					else
						m_despawnFlag.Checked					= true;

					if(dbReader.GetSqlValue(48).ToString() == "0")
						m_flagTimerOnFail.Checked					= false;
					else
						m_flagTimerOnFail.Checked					= true;

					if(dbReader.GetSqlValue(44).ToString() == "0")
						m_clearInventory.Checked					= false;
					else
						m_clearInventory.Checked					= true;

					if(dbReader.GetSqlValue(45).ToString() == "0")
						deleteAllByRequiredGLID.Checked					= false;
					else
						deleteAllByRequiredGLID.Checked					= true;

                    if (dbReader.GetSqlValue(63).ToString() == "0")
                        deleteAllByRewardGLID.Checked = false;
                    else
                        deleteAllByRewardGLID.Checked = true;

					if(dbReader.GetSqlValue(46).ToString() == "0")
						m_deleteByLocationID.Checked					= false;
					else
						m_deleteByLocationID.Checked					= true;
               
					if(dbReader.GetSqlValue(16).ToString() == "0")
						m_checkRandomRewardType.Checked					= false;
					else
						m_checkRandomRewardType.Checked					= true;

					if(dbReader.GetSqlValue(31).ToString() == "0")
						CheckOpenFileDialogWin.Checked					= false;
					else
						CheckOpenFileDialogWin.Checked					= true;

					if(dbReader.GetSqlValue(17).ToString() == "0")
						SpawnPlayerToPositionCheck.Checked					= false;
					else
						SpawnPlayerToPositionCheck.Checked					= true;
					
					if(dbReader.GetSqlValue(18).ToString() == "0")
						MakeTargetRebirthPoint.Checked						= false;
					else
						MakeTargetRebirthPoint.Checked						= true;

					if(dbReader.GetSqlValue(32).ToString() == "0")
						SpawnOnFail.Checked									= false;
					else
						SpawnOnFail.Checked									= true;
                    
					if(dbReader.GetSqlValue(22).ToString() == "0")
						ProgressionBased.Checked = false;
					else
						ProgressionBased.Checked = true;

					if(dbReader.GetSqlValue(34).ToString() == "0")
						CheckListProgression.Checked = false;
					else
						CheckListProgression.Checked = true;

					if(dbReader.GetSqlValue(30).ToString() == "0")
						CheckAutoComplete.Checked = false;
					else
						CheckAutoComplete.Checked = true;

					if(dbReader.GetSqlValue(35).ToString() == "0")
						UndeletableCheck.Checked = false;
					else
						UndeletableCheck.Checked = true;
		
					if(dbReader.GetSqlValue(21).ToString().Length > 0)
						JournalCommandOption.SelectedIndex	    = Convert.ToInt32(dbReader.GetSqlValue(21).ToString());
		            
					
					if(dbReader.GetSqlValue(38).ToString().Length > 0)
						JournalKey.SelectedIndex				= Convert.ToInt32(dbReader.GetSqlValue(38).ToString());//MISSING
				
					//end in list current quest data
				}
				m_questCount++;
			}
			dbReader.Close();
			
			if(dropGlid.Text.Length > 0)
			{

                string itemName = m_common.GetItemNameByGil(dropGlid.Text, connection);
                if (itemName.Length > 0)
                    m_glidDropLabel.Text = itemName;
			}

			InListRequiredItems(spawnID.ToString(),selectedQuest,connection,0);
			InListRewardItems(spawnID.ToString(),selectedQuest,connection,0);
            InListDynRules(spawnID.ToString(), selectedQuest, connection, -1);

			connection.Close();
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
			this.NewQuestSelection.Click += new System.EventHandler(this.NewQuestSelection_Click);
			this.DeleteQuestBtn.Click += new System.EventHandler(this.DeleteQuestBtn_Click);
			this.DeleteQuestConfBtn.Click += new System.EventHandler(this.DeleteQuestConfBtn_Click);
			this.DeleteQuestCancelBtn.Click += new System.EventHandler(this.DeleteQuestCancelBtn_Click);
			this.InsertQuestBtn.Click += new System.EventHandler(this.InsertQuestBtn_Click);
			this.ShowRequiredItemsPanel.CheckedChanged += new System.EventHandler(this.ShowRequiredItemsPanel_CheckedChanged);
			this.ShowRewardPanel.CheckedChanged += new System.EventHandler(this.ShowRewardPanel_CheckedChanged);
			this.ShowJournalPanel.CheckedChanged += new System.EventHandler(this.ShowJournalPanel_CheckedChanged);
			this.CheckShowTragetDropPanel.CheckedChanged += new System.EventHandler(this.CheckShowTragetDropPanel_CheckedChanged);
			this.ShowSpawnPanel.CheckedChanged += new System.EventHandler(this.ShowSpawnPanel_CheckedChanged);
			this.ScriptFunctionList.SelectedIndexChanged += new System.EventHandler(this.ScriptFunctionList_SelectedIndexChanged);
			this.addFunctionBtn.Click += new System.EventHandler(this.addFunctionBtn_Click);
			this.ScriptMacroList.SelectedIndexChanged += new System.EventHandler(this.ScriptMacroList_SelectedIndexChanged);
			this.addMacroBtn.Click += new System.EventHandler(this.addMacroBtn_Click);
			this.DeleteRequiredItemBtn.Click += new System.EventHandler(this.DeleteRequiredItemBtn_Click);
			this.SearchRequiredInvBtn.Click += new System.EventHandler(this.SearchRequiredInvBtn_Click);
			this.rqFunctionList.SelectedIndexChanged += new System.EventHandler(this.rqFunctionList_SelectedIndexChanged);
			this.RqAddFunctionBtn.Click += new System.EventHandler(this.RqAddFunctionBtn_Click);
			this.rqMacroList.SelectedIndexChanged += new System.EventHandler(this.rqMacroList_SelectedIndexChanged);
			this.rqAddMacroBtn.Click += new System.EventHandler(this.rqAddMacroBtn_Click);
			this.AddRewardItem.Click += new System.EventHandler(this.AddRewardItem_Click);
			this.DeleteRewardItem.Click += new System.EventHandler(this.DeleteRewardItem_Click);
			this.RFunctionDropList.SelectedIndexChanged += new System.EventHandler(this.RFunctionDropList_SelectedIndexChanged);
			this.rAddFunction.Click += new System.EventHandler(this.rAddFunction_Click);
			this.RMacroList.SelectedIndexChanged += new System.EventHandler(this.RMacroList_SelectedIndexChanged);
			this.rAddMacro.Click += new System.EventHandler(this.rAddMacro_Click);
			this.Load += new System.EventHandler(this.Page_Load);

		}
		#endregion


		private void InListRequiredItems(string spawnID,string questIndex,SqlConnection connection,int sel)
		{
			requiredItemList.Items.Clear();
			string glid = "";
			string itemName = "";
			int armed = 0;
			string type = "";
            SqlConnection innerConnection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
			string sqlQ = "SELECT GLID,ARMED,TYPE FROM GM_IGNINVENTORY WHERE TYPE = '1' AND SPAWNID = '"+spawnID+"' AND QUESTINDEX = '"+questIndex+"'";
			SqlDataReader invReader = dataCon.sqlQuery(sqlQ,connection);
			while(invReader.Read())
			{
				glid = invReader.GetSqlValue(0).ToString();
				type = invReader.GetSqlValue(2).ToString();
				armed = Convert.ToInt32(invReader.GetSqlValue(1).ToString());

                itemName = m_common.GetItemNameByGil(glid, innerConnection);
        			
                requiredItemList.Items.Add(itemName);
			}
			invReader.Close();
            if (sel > -1 && requiredItemList.Items.Count > sel)
			    requiredItemList.SelectedIndex = sel;
		}

		private void InListRewardItems(string spawnID,string questIndex,SqlConnection connection,int sel)
		{
			rewardItemList.Items.Clear();
			string glid = "";
			string itemName = "";
			int armed = 0;
			string type = "";
            SqlConnection innerConnection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
			string sqlQ = "SELECT GLID,ARMED,TYPE FROM GM_IGNINVENTORY WHERE TYPE = '2' AND SPAWNID = '"+spawnID+"' AND QUESTINDEX = '"+questIndex+"'";
			SqlDataReader invReader = dataCon.sqlQuery(sqlQ,connection);
			while(invReader.Read())
			{
				glid = invReader.GetSqlValue(0).ToString();
				type = invReader.GetSqlValue(2).ToString();
				armed = Convert.ToInt32(invReader.GetSqlValue(1).ToString());

                itemName = m_common.GetItemNameByGil(glid, innerConnection);
                	
                rewardItemList.Items.Add(itemName);
			}
			invReader.Close();
            if (sel > -1 && rewardItemList.Items.Count > sel)
				rewardItemList.SelectedIndex = sel;
		}

		private string BoolToString(bool i)
		{
             if(i == true)
			    return "1";
			 
             return "0";

		}

		private string BoolToString(bool i,int ovrdSuccessVal)
		{
			if(i == true)
				return ovrdSuccessVal.ToString();
			 
			return "0";

		}

		public void ShowSpeechPanel_CheckedChanged(object sender, System.EventArgs e)
		{
            if (ShowSpeechPanel.Checked == true)
            {
                SpeechPanel.Visible = true;
                ShowSpeechPanel.Checked = true;
                Session["SHOWSPEECHPANEL"] = "1";
            }
            else
            {
                SpeechPanel.Visible = false;
                ShowSpeechPanel.Checked = false;
                Session["SHOWSPEECHPANEL"] = "0";
            }
		}

		private void NewQuestSelection_Click(object sender, System.EventArgs e)
		{
			string spawnID = "-1";
			if(Session["SPAWNID"] != null)
				spawnID = Session["SPAWNID"].ToString();

            SqlConnection connectionLoc = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
			
			string sqlQ = "SELECT * FROM gm_IGNQuestData where spawnID = '"+spawnID+"' order by questIndex";
			SqlDataReader xreader = dataCon.sqlQuery(sqlQ,connectionLoc);
			int questCnt = 0;
			while(xreader.Read())
			{


                  questCnt++;
			}
			xreader.Close();
			
			
			dataCon.ExecuteQuery("insert into gm_IGNQuestData (SPAWNID,QUESTINDEX,TITLE) Values('"+spawnID+"','"+questCnt.ToString()+"','"+NewQuestTitle.Text+"')",connectionLoc);
			InListQuests(Convert.ToInt32(spawnID),connectionLoc);
			//m_questCount++;
			
			connectionLoc.Close();
		}

		private void DeleteQuestBtn_Click(object sender, System.EventArgs e)
		{
			DeleteQuestBtn.Visible = false;
			DeleteQuestConfBtn.Visible = true;
			DeleteQuestCancelBtn.Visible = true;
			AreYouSureLabel.Visible = true;
		}

		private void DeleteQuestConfBtn_Click(object sender, System.EventArgs e)
		{
			DeleteQuestBtn.Visible = true;
			DeleteQuestConfBtn.Visible = false;
			DeleteQuestCancelBtn.Visible = false;
			AreYouSureLabel.Visible = false;
			//delete stuff
            string spawnID = "-1";
			if(Session["SPAWNID"] != null)
			   spawnID = Session["SPAWNID"].ToString();

			string selectedQuest = "-1";
			if(Session["QUESTSELECTION"] != null)
				selectedQuest = Session["QUESTSELECTION"].ToString();

            SqlConnection connectionLoc = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
			dataCon.ExecuteQuery("DELETE FROM gm_IGNQuestData WHERE questindex = '"+selectedQuest+"' and spawnID = '"+spawnID+"'",connectionLoc);
            //reindex quests
			string sqlQ = "SELECT QUESTINDEX FROM gm_IGNQuestData where spawnID = '"+spawnID+"' order by questIndex";
			SqlDataReader xreader = dataCon.sqlQuery(sqlQ,connectionLoc);
			int tracer = 0;
            SqlConnection connectionInner = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
			while(xreader.Read())
			{
				int qIndex = Convert.ToInt32(xreader.GetSqlValue(0).ToString());
				dataCon.ExecuteQuery("UPDATE gm_IGNQuestData SET QUESTINDEX = '"+tracer.ToString()+"' WHERE SPAWNID = '"+spawnID+"' AND QUESTINDEX = '"+qIndex.ToString()+"'",connectionInner);
				tracer++;
			}
			xreader.Close();
			connectionInner.Close();
			//edit reindex quests
			InListQuests(Convert.ToInt32(spawnID),connectionLoc);
			connectionLoc.Close();

		}

		private void DeleteQuestCancelBtn_Click(object sender, System.EventArgs e)
		{
			DeleteQuestBtn.Visible = true;
			DeleteQuestConfBtn.Visible = false;
			DeleteQuestCancelBtn.Visible = false;
			AreYouSureLabel.Visible = false;
		}

		private void SearchRequiredInvBtn_Click(object sender, System.EventArgs e)
		{
            SqlConnection innerConnection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());

            m_common.FillListItemsBySearchTerm(searchRequiredCriteria.Text, ref ItemGILRequiredResultList, innerConnection);

            innerConnection.Close();
		}

		protected void SearchRewardBtn_Click(object sender, System.EventArgs e)
		{
            SqlConnection innerConnection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());

            m_common.FillListItemsBySearchTerm(RewardSearchCriteria.Text, ref ItemGILRewardList, innerConnection);

            innerConnection.Close();
		}

		private void DeleteRequiredItemBtn_Click(object sender, System.EventArgs e)
		{
			if(Session["SPAWNID"] == null)
				return;

			if(Session["QUESTSELECTION"] == null)
			    return;

            SqlConnection innerConnection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
			string sqlQ = "SELECT GLID,ARMED,TYPE FROM GM_IGNINVENTORY WHERE TYPE = '1' AND SPAWNID = '"+Session["SPAWNID"].ToString()+"' AND QUESTINDEX = '"+Session["QUESTSELECTION"].ToString()+"'";
			SqlDataReader invReader = dataCon.sqlQuery(sqlQ,innerConnection);
			int trace = 0;
			while(invReader.Read())
			{
				if(trace == requiredItemList.SelectedIndex)
				{
					string glid			= invReader.GetSqlValue(0).ToString();
					string armed		    = invReader.GetSqlValue(1).ToString();
					string type			= invReader.GetSqlValue(2).ToString();
					invReader.Close();

					sqlQ = "DELETE FROM GM_IGNINVENTORY WHERE SPAWNID = '"+Session["SPAWNID"].ToString()+"' AND GLID = '"+glid+"' AND TYPE = '"+type+"' AND ARMED = '"+armed+"' AND QUESTINDEX = '"+Session["QUESTSELECTION"].ToString()+"'";
					dataCon.ExecuteQuery(sqlQ,innerConnection);
					InListRequiredItems(Session["SPAWNID"].ToString(),Session["QUESTSELECTION"].ToString(),innerConnection,requiredItemList.SelectedIndex-1);
					innerConnection.Close();
					return;
				}
				trace++;
			}
			invReader.Close();
			innerConnection.Close();
		}

		private void AddRewardItem_Click(object sender, System.EventArgs e)
		{
			if(Session["SPAWNID"] == null)
				return;

			if(Session["QUESTSELECTION"] == null)
				return;

            SqlConnection innerConnection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
			string sqlQ = "INSERT INTO GM_IGNINVENTORY(SPAWNID,GLID,TYPE,QUESTINDEX) VALUES('"+Session["SPAWNID"].ToString()+"','"+ItemGILRewardList.SelectedValue+"','2','"+Session["QUESTSELECTION"].ToString()+"')";
			dataCon.ExecuteQuery(sqlQ,innerConnection);
			InListRewardItems(Session["SPAWNID"].ToString(),Session["QUESTSELECTION"].ToString(),innerConnection,rewardItemList.SelectedIndex);
			innerConnection.Close();
		}

		private void DeleteRewardItem_Click(object sender, System.EventArgs e)
		{
			if(Session["SPAWNID"] == null)
				return;

			if(Session["QUESTSELECTION"] == null)
				return;

            SqlConnection innerConnection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
			string sqlQ = "SELECT GLID,ARMED,TYPE FROM GM_IGNINVENTORY WHERE TYPE = '2' AND SPAWNID = '"+Session["SPAWNID"].ToString()+"' AND QUESTINDEX = '"+Session["QUESTSELECTION"].ToString()+"'";
			SqlDataReader invReader = dataCon.sqlQuery(sqlQ,innerConnection);
			int trace = 0;
			while(invReader.Read())
			{
				if(trace == rewardItemList.SelectedIndex)
				{
					string glid			= invReader.GetSqlValue(0).ToString();
					string armed		= invReader.GetSqlValue(1).ToString();
					string type			= invReader.GetSqlValue(2).ToString();
					invReader.Close();

					sqlQ = "DELETE FROM GM_IGNINVENTORY WHERE SPAWNID = '"+Session["SPAWNID"].ToString()+"' AND GLID = '"+glid+"' AND TYPE = '"+type+"' AND ARMED = '"+armed+"' AND QUESTINDEX = '"+Session["QUESTSELECTION"].ToString()+"'";
					dataCon.ExecuteQuery(sqlQ,innerConnection);
					InListRewardItems(Session["SPAWNID"].ToString(),Session["QUESTSELECTION"].ToString(),innerConnection,rewardItemList.SelectedIndex-1);
					innerConnection.Close();
					return;
				}
				trace++;
			}
			invReader.Close();
			innerConnection.Close();
		}

		private void InsertQuestBtn_Click(object sender, System.EventArgs e)
		{
			string spawnID = "-1";
			if(Session["SPAWNID"] != null)
				spawnID = Session["SPAWNID"].ToString();

			int questIndex = 0;
			if(Session["QUESTSELECTION"] == null)
				return;

			questIndex = Convert.ToInt32(Session["QUESTSELECTION"].ToString());

            SqlConnection connectionLoc = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());

            SqlConnection connectionInner = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
			
			string sqlQ = "SELECT QUESTINDEX FROM gm_IGNQuestData where spawnID = '"+spawnID+"' order by questIndex desc";
			SqlDataReader xreader = dataCon.sqlQuery(sqlQ,connectionLoc);
			bool incAfter = false;
			int setQuestIndex = 0;
			int newTrace = 0;
			while(xreader.Read())
			{
				int qIndex = Convert.ToInt32(xreader.GetSqlValue(0).ToString());
				if(qIndex == questIndex)
				{
				   setQuestIndex = qIndex;
				   incAfter = true;
				   newTrace = qIndex+1;
				   dataCon.ExecuteQuery("UPDATE gm_IGNQuestData SET QUESTINDEX = '"+newTrace.ToString()+"' WHERE SPAWNID = '"+spawnID+"' AND QUESTINDEX = '"+qIndex.ToString()+"'",connectionInner);
				}
				if(incAfter == false)
				{
					newTrace = qIndex+1;
					dataCon.ExecuteQuery("UPDATE gm_IGNQuestData SET QUESTINDEX = '"+newTrace.ToString()+"' WHERE SPAWNID = '"+spawnID+"' AND QUESTINDEX = '"+qIndex.ToString()+"'",connectionInner);
				}
			}
			xreader.Close();
			
			connectionInner.Close();
            if(incAfter == true)
			   dataCon.ExecuteQuery("insert into gm_IGNQuestData (SPAWNID,QUESTINDEX,TITLE) Values('"+spawnID+"','"+questIndex.ToString()+"','"+NewQuestTitle.Text+"')",connectionLoc);
			
			
			InListQuests(Convert.ToInt32(spawnID),connectionLoc);
			//m_questCount++;
			
			connectionLoc.Close();
		}

		private string urlPersist()
		{
			string spanel = "";
			if(Request.QueryString["SPN"] != null)
			   spanel = "&SPN="+Request.QueryString["SPN"].ToString();

			string rwdpanel = "";
			if(Request.QueryString["RWD"] != null)
				rwdpanel = "&RWD="+Request.QueryString["RWD"].ToString();

			string rqdpanel = "";
			if(Request.QueryString["RQD"] != null)
				rqdpanel = "&RQD="+Request.QueryString["RQD"].ToString();

			string tgdpanel = "";
			if(Request.QueryString["TGD"] != null)
				tgdpanel = "&TGD="+Request.QueryString["TGD"].ToString();

			string sjnpanel = "";
			if(Request.QueryString["SJN"] != null)
				sjnpanel = "&SJN="+Request.QueryString["SJN"].ToString();

			string spwpanel = "";
			if(Request.QueryString["SPW"] != null)
				spwpanel = "&SPW="+Request.QueryString["SPW"].ToString();

              string build = spanel+rwdpanel+rqdpanel+tgdpanel+sjnpanel+spwpanel;

           return build;
		}

		private void HandleCheckBoxes()
		{
			string spawnID = "-1";
			if(Session["SPAWNID"] != null)
				spawnID = Session["SPAWNID"].ToString();

			string selectedQuest = "-1";
			if(Session["QUESTSELECTION"] != null)
				selectedQuest = Session["QUESTSELECTION"].ToString();

			string expIDstring = "-1";
			if(Session["expID"] != null)
				expIDstring = Session["expID"].ToString();
	         
			string url = "IGN_QuestMenuPage.aspx?expID=" + expIDstring + "&spawnid=" +spawnID+ "&QUESTSELECTION=" +
				selectedQuest+"&SPN="+BoolToString(ShowSpeechPanel.Checked)+"&RWD="+BoolToString(ShowRewardPanel.Checked)+
				"&RQD="+BoolToString(ShowRequiredItemsPanel.Checked)+"&TGD="+BoolToString(CheckShowTragetDropPanel.Checked)+
				"&SJN="+BoolToString(ShowJournalPanel.Checked)+"&SPW="+BoolToString(ShowSpawnPanel.Checked);
			Response.Redirect(url);
		}

		private void ShowRewardPanel_CheckedChanged(object sender, System.EventArgs e)
		{
            if (ShowRewardPanel.Checked == true)
            {
                Session["SHOWREWARDPANEL"] = "1";
                RewardPanel.Visible = true;
                ShowRewardPanel.Checked = true;
            }
            else
            {
                Session["SHOWREWARDPANEL"] = "0";
                RewardPanel.Visible = false;
                ShowRewardPanel.Checked = false;
            }
		}

		private void ShowRequiredItemsPanel_CheckedChanged(object sender, System.EventArgs e)
		{
            if (ShowRequiredItemsPanel.Checked == true)
            {
                Session["SHOWREQUIREDPANEL"] = "1";
                RequiredPanel.Visible = true;
                ShowRequiredItemsPanel.Checked = true;
            }
            else
            {
                Session["SHOWREQUIREDPANEL"] = "0";
                RequiredPanel.Visible = false;
                ShowRequiredItemsPanel.Checked = false;
            }
		}

		private void CheckShowTragetDropPanel_CheckedChanged(object sender, System.EventArgs e)
		{
            if (CheckShowTragetDropPanel.Checked == true)
            {
                TargetDropPanel.Visible = true;
                CheckShowTragetDropPanel.Checked = true;
                Session["SHOWTARGETDROPPANEL"] = "1";
            }
            else
            {
                TargetDropPanel.Visible = false;
                CheckShowTragetDropPanel.Checked = false;
                Session["SHOWTARGETDROPPANEL"] = "0";
            }
		}

		private void ShowSpawnPanel_CheckedChanged(object sender, System.EventArgs e)
		{
            if (ShowSpawnPanel.Checked == true)
            {
                SpawnPanel.Visible = true;
                ShowSpawnPanel.Checked = true;
                Session["SHOWSPAWNPANEL"] = "1";
            }
            else
            {
                SpawnPanel.Visible = false;
                ShowSpawnPanel.Checked = false;
                Session["SHOWSPAWNPANEL"] = "0";
            }
		}

		private void ShowJournalPanel_CheckedChanged(object sender, System.EventArgs e)
		{
            if (ShowJournalPanel.Checked == true)
            {
                JournalPanel.Visible = true;
                ShowJournalPanel.Checked = true;
                Session["SHOWJOURNALPANEL"] = "1";
            }
            else
            {
                JournalPanel.Visible = false;
                ShowJournalPanel.Checked = false;
                Session["SHOWJOURNALPANEL"] = "0";
            }
		}

		/*
		.DropDownList ScriptFunctionList;
		protected System.Web.UI.WebControls.DropDownList ScriptMacroList;
		protected System.Web.UI.WebControls.Label scripFunctionDescription;
		protected System.Web.UI.WebControls.Label scriptMacroDescription;
		*/
		



		public void InListMacros(ref DropDownList listLocal)
		{
			listLocal.Items.Clear();

			ListItem newListItem1 = new ListItem("$CHARNAME","Uses the players name that is talking to the bot. [br]Example: Very well done $CHARNAME.[br]");
			listLocal.Items.Add(newListItem1);

			ListItem newListItem2 = new ListItem("$G_URL","Instead of typing full urls, the system stores a global root for your images, it is always a good item to use this instead of full urls because of a case where you change web servers, you can change the root globally and you will not have to go back and change every single quest. [br]Example: $G_URL/images/john.jpg[br]");
			listLocal.Items.Add(newListItem2);

		}

        

		private void addFunctionBtn_Click(object sender, System.EventArgs e)
		{
		       ClientScriptEdit.Text += ScriptFunctionList.SelectedItem.Text;
		}

		private void addMacroBtn_Click(object sender, System.EventArgs e)
		{
		       ClientScriptEdit.Text += ScriptMacroList.SelectedItem.Text;
		}

		private void ScriptFunctionList_SelectedIndexChanged(object sender, System.EventArgs e)
		{
		        scripFunctionDescription.Text = ScriptFunctionList.SelectedValue.Replace("[br]","<BR>");
		}

		private void ScriptMacroList_SelectedIndexChanged(object sender, System.EventArgs e)
		{
		    scriptMacroDescription.Text = ScriptMacroList.SelectedValue.Replace("[br]","<BR>");
		}

		private void rAddFunction_Click(object sender, System.EventArgs e)
		{
		    rewardScriptEdit.Text += RFunctionDropList.SelectedItem.Text;
		}

		private void rAddMacro_Click(object sender, System.EventArgs e)
		{
		    rewardScriptEdit.Text += RMacroList.SelectedItem.Text;
		}

		private void RFunctionDropList_SelectedIndexChanged(object sender, System.EventArgs e)
		{
		    rFunctionListDesc.Text = RFunctionDropList.SelectedValue.Replace("[br]","<BR>");
		}

		private void RMacroList_SelectedIndexChanged(object sender, System.EventArgs e)
		{
		    rMacroDescription.Text = RMacroList.SelectedValue.Replace("[br]","<BR>");
		}

		private void RqAddFunctionBtn_Click(object sender, System.EventArgs e)
		{
		    requiredScriptEdit.Text += rqFunctionList.SelectedItem.Text;
		}

		private void rqAddMacroBtn_Click(object sender, System.EventArgs e)
		{
		    requiredScriptEdit.Text += rqMacroList.SelectedItem.Text;
		} 

		private void rqFunctionList_SelectedIndexChanged(object sender, System.EventArgs e)
		{
		     rqFunctionDescription.Text = rqFunctionList.SelectedValue.Replace("[br]","<BR>");
		}

		private void rqMacroList_SelectedIndexChanged(object sender, System.EventArgs e)
		{
		     rqMacroDescription.Text = rqMacroList.SelectedValue.Replace("[br]","<BR>");
		}

        protected void ApplyChanges_Click1(object sender, EventArgs e)
        {
            string spawnID = "-1";
            if (Session["SPAWNID"] == null)
            {
                return;
            }

            


            spawnID = Session["SPAWNID"].ToString();

            string selectedQuest = "-1";
            if (Session["QUESTSELECTION"] != null)
                selectedQuest = Session["QUESTSELECTION"].ToString();
            else
                return;


            m_lastAppliedChanges.Text = "Last changed:" + DateTime.Now.ToLongTimeString();


            SqlConnection connectionLoc = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());

            m_logReporter.LogEvent("AX QUEST UPDATE", "Updating quest entity(" + dataCon.SanitizeIn(title.Text) + ") user:" + dataCon.SanitizeIn(Session["user"].ToString()), connectionLoc);

            // PARSE the EXG name from the World Selection drop-down list
            char[] SplitParams = " ".ToCharArray();
            string[] WorldSelectionParts = m_spawnPanelWorldDropList.SelectedItem.Text.Split(SplitParams);

            dataCon.ExecuteQuery("update gm_IGNQuestData set Title = '" + dataCon.SanitizeIn(title.Text) + "', " +
                                        "QuestDescription = '" + dataCon.SanitizeIn(QuestFailedMsg.Text) + "', " +
                                        "QuestCompletionMessage = '" + dataCon.SanitizeIn(CompletedMessage.Text) + "', " +
                                        "skillTypeBonus = '" + ExpSkilltypeDropList.SelectedValue + "', " +
                                        "maxExpLimit = '" + ExperienceMaxGain.Text + "', " +
                                        "spawnAiCfg = '" + SpawnAICFGdropList.SelectedValue + "', " +
                                        "spawnPosX = '" + PosX.Text + "', " +
                                        "spawnPosY = '" + PosY.Text + "', " +
                                        "spawnPosZ = '" + PosZ.Text + "', " +
                                        "worldID = '" + SpawnWorldChannel.Text + "', " +
                                        "spawnedAiLifeLine = '" + SpawnLifeDuration.Text + "', " +
                                        "randomRewardType = '" + BoolToString(m_checkRandomRewardType.Checked) + "', " +
                                        "spawnToLocationOverride = '" + BoolToString(SpawnPlayerToPositionCheck.Checked) + "', " +
                                        "makeTargetRebirthPoint = '" + BoolToString(MakeTargetRebirthPoint.Checked) + "', " +
                                        "raceSpecificAccess = '" + RaceSpecificRequiredList.SelectedValue + "', " +
                                        "optionalCommand = '" + dataCon.SanitizeIn(OptionalInstruction.Text) + "', " +
                                        "miscAction = '" + JournalCommandOption.SelectedIndex.ToString() + "', " +
                                        "progressionBasedByQuestJournal = '" + BoolToString(ProgressionBased.Checked) + "', " +
                                        "cashReward = '" + CashReward.Text + "', " +
                                        "requiredSkillType = '" + RequiredSkillTypeDropList.SelectedValue + "', " +
                                        "requiredSkillLevel = '" + RequiredSkillLevel.Text + "', " +
                                        "mobToHuntID = '" + TargetDropAICFGList.SelectedValue + "', " +
                                        "maxDropCount = '" + maxDropCount.Text + "', " +
                                        "dropGlid = '" + dropGlid.Text + "', " +
                                        "dropPercentage = '" + dropTargetPercentage.Text + "', " +
                                        "autoComplete = '" + BoolToString(CheckAutoComplete.Checked) + "', " +
                                        "openDialogWindow = '" + BoolToString(CheckOpenFileDialogWin.Checked) + "', " +
                                        "spawnOnFail = '" + BoolToString(SpawnOnFail.Checked) + "', " +
                                        "optionalInstruction = '" + dataCon.SanitizeIn(OptionalInstruction.Text) + "', " +
                                        "listProgressionOptions = '" + BoolToString(CheckListProgression.Checked) + "', " +
                                        "undeletable = '" + BoolToString(UndeletableCheck.Checked) + "', " +
                                        "mustOwnKingdomID = '" + mustOwnKingdomID.Text + "', " +
                                        "expIDRequired = '" + RequiredExpansionLevel.Text + "', " +
                                        "completedKeyEnabled = '" + JournalKey.SelectedIndex.ToString() + "', " +
                                        "portalVSMessage = '" + dataCon.SanitizeIn(PortalLimitVSMsg.Text) + "', " +
                                        "clientScript = '" + dataCon.SanitizeIn(ClientScriptEdit.Text) + "', " +
                                        "requiredScript = '" + dataCon.SanitizeIn(requiredScriptEdit.Text) + "', " +
                                        "rewardScript = '" + dataCon.SanitizeIn(rewardScriptEdit.Text) + "', " +
                                        "clearInventory = '" + BoolToString(m_clearInventory.Checked) + "', " +
                                        "deleteAllByRequiredGLID = '" + BoolToString(deleteAllByRequiredGLID.Checked) + "', " +
                                        "deleteAllByRewardGLID = '" + BoolToString(deleteAllByRewardGLID.Checked) + "', " +
                                        "deleteByLocationID = '" + BoolToString(m_deleteByLocationID.Checked) + "', " +
                                        "closeJournalEntryByName = '" + dataCon.SanitizeIn(m_closeJnlEntryByName.Text) + "', " +
                                        "despawnFlag = '" + BoolToString(m_despawnFlag.Checked) + "', " +
                                        "timeStampFlag = '" + BoolToString(m_flagTimerOnFail.Checked) + "', " +
                                        "requiredTimeStamp = '" + m_timeStampRequired.Text + "', " +
                                        "expfailmessage = '" + m_expFailMsg.Text + "', " +
                                        "maxDmgMod = '" + m_maxDamage.Text + "', " +
                                        "minDmgMod = '" + m_minDamage.Text + "', " +
                                        "SPAWNARRAYCOUNT = '" + m_aiOptionArrayCount.Text + "', " +
                                        "SPAWNARRAYOFFSET = '" + m_aiArrayOptionOffset.Text + "', " + 
                                        "SpawnRotationRange = '" + SpawnRotation.Text + "', " +
                                        "spawnWorldName = '" + WorldSelectionParts[0].Trim() + "', " +
                                        "instanceGenerateType = '" + m_instanceGenList.SelectedIndex.ToString() + "', " +
                                        "instanceGenerateParam = '" + m_instanceGenParam.Text + "', " +
                                        "instanceGenerateParam2 = '" + m_instanceGenParam2.Text + "', " +
                                        "instanceGenerateParam3 = '" + m_instanceGenParam3.Text + "', " +
                                        "instanceGenerateParam4 = '" + m_instanceGenParam4.Text + "', " +
                                        "flagHostileOnSuccess = '" + BoolToString(m_flagHostileOnSuccessCheck.Checked, 2) + "', " +
                                        "expBonus = '" + ExperienceBonus.Text + "' where spawnID ='" + spawnID.ToString() + "' AND QuestIndex ='" + selectedQuest + "'",
                                        connectionLoc);

            dataCon.ExecuteQuery("update gm_IGNQuestData set " +
            "progressionBasedByQuestJournal = '" + BoolToString(ProgressionBased.Checked) + "', " +
            "listProgressionOptions = '" + BoolToString(CheckListProgression.Checked) + "' " +
            "where spawnID ='" + spawnID.ToString() + "'",
            connectionLoc);


            if (dropGlid.Text.Length > 0)
            {
                string itemName = m_common.GetItemNameByGil(dropGlid.Text, connectionLoc);
                if(itemName.Length > 0)
                    m_glidDropLabel.Text = itemName;
            }

            connectionLoc.Close();
        }
        protected void m_instanceGenList_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (m_instanceGenList.SelectedIndex)
            {
                case 0://none
                    m_instanceParam.Text = "N/A";
                    m_instanceParam2.Text = "N/A";
                    m_instanceParam3.Text = "N/A";
                    m_instanceParam4.Text = "N/A";
                    break;
                case 1://group
                    m_instanceParam.Text = "N/A";
                    m_instanceParam2.Text = "N/A";
                    m_instanceParam3.Text = "N/A";
                    m_instanceParam4.Text = "N/A";
                    break;
                case 2://arena
                    m_instanceParam.Text = "Max Player Count before autostarting.";
                    m_instanceParam2.Text = "Internal Use only";
                    m_instanceParam3.Text = "Minimum player count before autostart";
                    m_instanceParam4.Text = "Duration after minimum player count hit (Milliseconds)";
                    break;
            }
        }
        protected void addDynRuleBtn_Click(object sender, EventArgs e)
        {
            m_dynRuleOutput.Text = "";
            if (Session["SPAWNID"] == null)
                return;

            if (Session["QUESTSELECTION"] == null)
                return;

            SqlConnection innerConnection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
            string sqlQ = "SELECT SPAWNID,QUESTINDEX,RULEID,PARAM1,DECORATEDNAME FROM GM_IGNDYNAMICRULES WHERE SPAWNID = '" + Session["SPAWNID"].ToString() + "' AND QUESTINDEX = '" + Session["QUESTSELECTION"].ToString() + "'";
            SqlDataReader invReader = dataCon.sqlQuery(sqlQ, innerConnection);
            int trace = 0;
            while (invReader.Read())
            {
                if (invReader.GetSqlValue(4).ToString() == m_dynWorldRulesDList.SelectedValue.ToString())
                {
                    m_dynRuleOutput.Text = "rule already exists";
                    invReader.Close();
                    innerConnection.Close();
                    return;
                }
                trace++;
            }
            invReader.Close();

            
            sqlQ = "INSERT INTO GM_IGNDYNAMICRULES(SPAWNID,QUESTINDEX,RULEID,PARAM1,DECORATEDNAME) VALUES('" + Session["SPAWNID"].ToString() + "','" + Session["QUESTSELECTION"].ToString() + "','" + m_dynWorldRulesDList.SelectedIndex.ToString() + "','" + m_dynWorldRuleParam.Text + "','" + m_dynWorldRulesDList.SelectedValue.ToString() + "')";
            dataCon.ExecuteQuery(sqlQ, innerConnection);
            InListDynRules(Session["SPAWNID"].ToString(), Session["QUESTSELECTION"].ToString(), innerConnection, trace);
            innerConnection.Close();

        }
        private void InListDynRules(string spawnID, string questIndex, SqlConnection connection, int sel)
        {
            dynWldRulesListBox.Items.Clear();
            string sqlQ = "SELECT SPAWNID,QUESTINDEX,RULEID,PARAM1,DECORATEDNAME FROM GM_IGNDYNAMICRULES WHERE SPAWNID = '" + spawnID + "' AND QUESTINDEX = '" + questIndex + "'";
            SqlDataReader invReader = dataCon.sqlQuery(sqlQ, connection);
            while (invReader.Read())
            {
                string decoratedName = invReader.GetSqlValue(4).ToString();
                dynWldRulesListBox.Items.Add(decoratedName);
            }
            invReader.Close();
            if (sel > -1 && dynWldRulesListBox.Items.Count > sel)
                dynWldRulesListBox.SelectedIndex = sel;
        }
        protected void DeleteDynRuleBtn_Click(object sender, EventArgs e)
        {
            m_dynRuleOutput.Text = "";
            if (Session["SPAWNID"] == null)
                return;

            if (Session["QUESTSELECTION"] == null)
                return;

            int sel = dynWldRulesListBox.SelectedIndex;
            SqlConnection innerConnection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
            string sqlQ = "DELETE FROM GM_IGNDYNAMICRULES WHERE DECORATEDNAME = '" + dynWldRulesListBox.SelectedValue.ToString() + "' AND SPAWNID = '" + Session["SPAWNID"].ToString() +"' AND QUESTINDEX = '" + Session["QUESTSELECTION"].ToString() + "'";
            dataCon.ExecuteQuery(sqlQ, innerConnection);
            InListDynRules(Session["SPAWNID"].ToString(), Session["QUESTSELECTION"].ToString(), innerConnection, sel-1);
            innerConnection.Close();

        }
        protected void dynWldRulesListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            m_dynRuleOutput.Text = "";
            if (Session["SPAWNID"] == null)
                return;

            if (Session["QUESTSELECTION"] == null)
                return;

            SqlConnection innerConnection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
            string sqlQ = "SELECT SPAWNID,QUESTINDEX,RULEID,PARAM1,DECORATEDNAME FROM GM_IGNDYNAMICRULES WHERE SPAWNID = '" + Session["SPAWNID"].ToString() + "' AND QUESTINDEX = '" + Session["QUESTSELECTION"].ToString() + "'";
            SqlDataReader invReader = dataCon.sqlQuery(sqlQ, innerConnection);
            while (invReader.Read())
            {
                if (invReader.GetSqlValue(4).ToString() == dynWldRulesListBox.SelectedValue.ToString())
                {
                    m_dynWorldRulesDList.SelectedValue = invReader.GetSqlValue(4).ToString();
                    m_dynWorldRuleParam.Text = invReader.GetSqlValue(3).ToString();
                    
                    break;
                }
            }
            invReader.Close();
            innerConnection.Close();

        }
        protected void NewRequiredItemBtn_Click1(object sender, EventArgs e)
        {
            if (Session["SPAWNID"] == null)
                return;

            if (Session["QUESTSELECTION"] == null)
                return;

            SqlConnection innerConnection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
            string sqlQ = "INSERT INTO GM_IGNINVENTORY(SPAWNID,GLID,TYPE,QUESTINDEX) VALUES('" + Session["SPAWNID"].ToString() + "','" + ItemGILRequiredResultList.SelectedValue + "','1','" + Session["QUESTSELECTION"].ToString() + "')";
            dataCon.ExecuteQuery(sqlQ, innerConnection);
            InListRequiredItems(Session["SPAWNID"].ToString(), Session["QUESTSELECTION"].ToString(), innerConnection, requiredItemList.SelectedIndex);
            innerConnection.Close();
        }
        
}

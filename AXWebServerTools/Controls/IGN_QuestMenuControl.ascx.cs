	using System;
	using System.Data;
	using System.Drawing;
	using System.Web;
	using System.Web.UI.WebControls;
	using System.Web.UI.HtmlControls;
	using System.Data.SqlClient;


	/// <summary>
	///		Summary description for IGN_QuestMenuControl.
	/// </summary>
	public partial class IGN_QuestMenuControl : System.Web.UI.UserControl
	{
		DatabaseConnectionModule dbConnect = new DatabaseConnectionModule();
		LogSystem m_logSys = new LogSystem();
        ConnectionHelper m_conHelper = new ConnectionHelper();

		private void Page_Load(object sender, System.EventArgs e)
		{
			AuthenticationModule m_authPtr = new AuthenticationModule();
            m_authPtr.AuthenticateUser("Main.aspx","QUESTADMIN");
			
			Session["SPAWNID"] = null;
			if(Request.QueryString["spawnID"] != null)
			   Session["SPAWNID"] = Request.QueryString["spawnID"].ToString().Replace("'","");

            int idLocal = -1;
			
			if(Request.QueryString["expID"] != null)
			   idLocal = Convert.ToInt32(Request.QueryString["expID"].ToString().Replace("'",""));

            if(Request.QueryString["QMODE"] != null)
			   Session["QUESTMODEON"] = Convert.ToInt32(Request.QueryString["QMODE"].ToString().Replace("'",""));

			Session["EXPID"] = idLocal;
            
            if(m_menuOutput.Text.Length > 0)
			   return;

			EntitySearchResultsList.Visible = false;
			//SqlConnection connection = dbConnect.CreateNativeConnectionObject();
            string serverName = (string)Session["ServerName"];
            SqlConnection connection = m_conHelper.GetServerHUBConnection(serverName);//aquire target server db
			StreamMenu(idLocal,connection);
			connection.Close();
		}

		private void StreamMenu(int expandedID,SqlConnection sqlConnection)
		{
			string spawnID = "-1";
			if(Session["SPAWNID"] != null)
               spawnID = Session["SPAWNID"].ToString();
  

			m_menuOutput.Text = "";
			string expandedIDstring = expandedID.ToString();
			SqlDataReader dbReader = dbConnect.sqlQuery("SELECT FOLDERNAME,ORGANIZEGROUPID FROM gm_IGNFolders ORDER BY ORGANIZEGROUPID",sqlConnection);
			while(dbReader.Read())
			{
				string folderName = dbConnect.SanitizeOutHtml(dbReader.GetSqlValue(0).ToString());
				string organizeGroupID = dbReader.GetSqlValue(1).ToString();
 
				if(organizeGroupID != expandedIDstring)
					m_menuOutput.Text += "<img src='images/plus.gif'><a href=" + "IGN_QuestMenuPage.aspx?expID="+organizeGroupID.ToString()+ ">&nbsp;" + folderName + "</a><br>";
				else
				{
					NewFolderName.Text = folderName;
                    SqlConnection innerConnect = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());//aquire target server db
					//SqlConnection innerConnect = dbConnect.CreateNativeConnectionObject();
					m_menuOutput.Text += "<img src='images/minus.gif'><a href=" + "IGN_QuestMenuPage.aspx?expID=" +organizeGroupID.ToString()+ ">&nbsp;" + folderName + "</a><br>";
			        
					SqlDataReader listOurReader = dbConnect.sqlQuery("SELECT GENERATORNAME, SPAWNID FROM gm_IGNEntity where ORGANIZEGROUPID = '"+expandedIDstring+"' ORDER BY GENERATORNAME",innerConnect);
					while(listOurReader.Read())
					{
						  string spawnIDlocal = listOurReader.GetSqlValue(1).ToString();
						if(spawnID == spawnIDlocal)
						{
							m_menuOutput.Text += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<FONT COLOR = YELLOW><STRONG>" + dbConnect.SanitizeOutHtml(listOurReader.GetSqlValue(0).ToString().ToUpper()) + "</STRONG></FONT><br>";
						    
						}
						else
							m_menuOutput.Text += "<a href=IGN_QuestMenuPage.aspx?expID=" +organizeGroupID.ToString() + "&SpawnID=" +listOurReader.GetSqlValue(1).ToString() + ">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" + dbConnect.SanitizeOutHtml(listOurReader.GetSqlValue(0).ToString()) + "</a><br>";
                          
					}
					listOurReader.Close();
					innerConnect.Close();
				}
			}
			dbReader.Close();
		}


		private void UpdateLiveServer()
		{
			string serverName   = (string)Session["ServerName"];
			string repRequester = (string)Session["user"];
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
  
			string spawnID = "-1";
			if(Session["SPAWNID"] != null)
				spawnID = Session["SPAWNID"].ToString();

			if(spawnID == "-1")
			{
				return;
			}
			   
			ConnectionHelper conHlper = new ConnectionHelper();
			SqlConnection sqlAXServerConnection = conHlper.GetServerConnection(serverName);

			conHlper.SendCommandToServer(       
				serverName,
				"22",
				spawnID.ToString(),
				spawnID.ToString(),
				"",
				"",
				"",
				repRequester,
				sqlAXServerConnection);

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
			this.Properties.Click += new System.EventHandler(this.Properties_Click);
			this.QuestBtn.Click += new System.EventHandler(this.QuestBtn_Click);
			this.NewEntityBtn.Click += new System.EventHandler(this.NewEntityBtn_Click);
			this.NewFolderButton.Click += new System.EventHandler(this.NewFolderButton_Click);
			this.ReplaceFolder.Click += new System.EventHandler(this.ReplaceFolder_Click);
			this.DeleteFolderBtn.Click += new System.EventHandler(this.DeleteFolderBtn_Click);
			this.DeleteFolderConfBtn.Click += new System.EventHandler(this.DeleteFolderConfBtn_Click);
			this.DeleteFolderCancelBtn.Click += new System.EventHandler(this.DeleteFolderCancelBtn_Click);
			this.EntitySearchBtn.Click += new System.EventHandler(this.EntitySearchBtn_Click);
			this.EntitySearchResultsList.SelectedIndexChanged += new System.EventHandler(this.EntitySearchResultsList_SelectedIndexChanged);
			this.ChatDataBtn.Click += new System.EventHandler(this.ChatDataBtn_Click);
			this.Load += new System.EventHandler(this.Page_Load);

		}
		#endregion

		protected void UpdateIGNServer_Click(object sender, System.EventArgs e)
		{
		    UpdateLiveServer();
		}

		private void Properties_Click(object sender, System.EventArgs e)
		{
			string spawnID = "-1";
			if(Session["SPAWNID"] != null)
				spawnID = Session["SPAWNID"].ToString();

			int expID = -1;
			
			if(Request.QueryString["expID"] != null)
				expID = Convert.ToInt32(Request.QueryString["expID"].ToString().Replace("'",""));
	

			string urlTemp = "IGN_QuestMenuPage.aspx?expID=" + expID + "&SpawnID=" + spawnID + "&QMODE=0";
			Response.Redirect(urlTemp);
		}

		public void QuestBtn_Click(object sender, System.EventArgs e)
		{
			string spawnID = "-1";
			if(Session["SPAWNID"] != null)
				spawnID = Session["SPAWNID"].ToString();

			int expID = -1;
			
			if(Request.QueryString["expID"] != null)
				expID = Convert.ToInt32(Request.QueryString["expID"].ToString().Replace("'",""));
	

			string urlTemp = "IGN_QuestMenuPage.aspx?expID=" + expID + "&SpawnID=" + spawnID + "&QMODE=1";
			Response.Redirect(urlTemp);
		}

		private void NewFolderButton_Click(object sender, System.EventArgs e)
		{
		      FolderErrorOutput.Text				= "";
			  if(NewFolderName.Text.Length == 0)
			     FolderErrorOutput.Text = "Folder cannot be blank";


              int orgID = 0;

              SqlConnection connection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
			  SqlDataReader dbReader = dbConnect.sqlQuery("SELECT FOLDERNAME,ORGANIZEGROUPID FROM gm_IGNFolders ORDER BY ORGANIZEGROUPID DESC",connection);
			  if(dbReader.Read())
			  {
                 orgID = Convert.ToInt32(dbReader.GetSqlValue(1).ToString())+1;
			  }
			  dbReader.Close();
			  dbConnect.ExecuteQuery("insert into gm_IGNFolders (FOLDERNAME,ORGANIZEGROUPID) Values('"+dbConnect.SanitizeIn(NewFolderName.Text)+"','"+orgID.ToString()+"')",connection);
              StreamMenu(orgID,connection);
			  connection.Close();
		}

		private void DeleteFolderBtn_Click(object sender, System.EventArgs e)
		{
		
			FolderErrorOutput.Text				= "";
			DeleteFolderBtn.Visible				= false;
			NewFolderButton.Visible				= false;
			ReplaceFolder.Visible               = false;
			NewEntityBtn.Visible                = false;
			DeleteFolderCancelBtn.Visible		= true;
			DeleteFolderConfBtn.Visible			= true;
			AreYouSureLabel.Visible				= true;
		}

		private void DeleteFolderConfBtn_Click(object sender, System.EventArgs e)
		{
			if(Request.QueryString["expID"] == null)
				return;

			string idLocal = Request.QueryString["expID"].ToString();

			FolderErrorOutput.Text				= "";
			
			DeleteFolderBtn.Visible				= true;
			NewFolderButton.Visible				= true;
			ReplaceFolder.Visible               = true;
			NewEntityBtn.Visible                = true;
			DeleteFolderCancelBtn.Visible		= false;
			DeleteFolderConfBtn.Visible			= false;
			AreYouSureLabel.Visible				= false;
			//check to see if folder contains anything befoire deletion
            SqlConnection connection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
			if(Request.QueryString["spawnID"] == null)
			{
				SqlDataReader readerD = dbConnect.sqlQuery("SELECT * FROM gm_IGNEntity WHERE ORGANIZEGROUPID = '"+idLocal+"'",connection);
				if(readerD.Read())
				{//has contents cannot delete this
					FolderErrorOutput.Text = "Cannot delete a folder that has contents";
					readerD.Close();
					connection.Close();
					return;
				}//end has contents cannot delete this
				readerD.Close();
				dbConnect.ExecuteQuery("DELETE FROM gm_IGNFolders WHERE ORGANIZEGROUPID = '"+idLocal+"'",connection);
				m_logSys.LogEvent("AX QUEST UPDATE","Deleted folder:"+Session["user"].ToString(),connection);
			}
			else
			{//entity delete 
                dbConnect.ExecuteQuery("DELETE FROM gm_IGNEntity WHERE ORGANIZEGROUPID = '"+idLocal+"' AND SPAWNID = '"+Request.QueryString["spawnID"].ToString()+"'",connection);
				dbConnect.ExecuteQuery("DELETE FROM gm_IGNDimCfgs WHERE SPAWNID = '"+Request.QueryString["spawnID"].ToString()+"'",connection);
				dbConnect.ExecuteQuery("DELETE FROM gm_IGNQuestData WHERE SPAWNID = '"+Request.QueryString["spawnID"].ToString()+"'",connection);
				dbConnect.ExecuteQuery("DELETE FROM gm_IGNInventory WHERE SPAWNID = '"+Request.QueryString["spawnID"].ToString()+"'",connection);
				dbConnect.ExecuteQuery("DELETE FROM gm_IGNChatStrings WHERE SPAWNID = '"+Request.QueryString["spawnID"].ToString()+"'",connection);
			
			    m_logSys.LogEvent("AX QUEST UPDATE","Deleted entity:"+Session["user"].ToString(),connection);
			}//end entity delete
			
			StreamMenu(Convert.ToInt32(idLocal),connection);
			connection.Close();

			


		}

		private void DeleteFolderCancelBtn_Click(object sender, System.EventArgs e)
		{
			FolderErrorOutput.Text				= "";
			DeleteFolderBtn.Visible				= true;
			NewFolderButton.Visible				= true;
			NewEntityBtn.Visible                = true;
			ReplaceFolder.Visible               = true;
			DeleteFolderCancelBtn.Visible		= false;
			DeleteFolderConfBtn.Visible			= false;
			AreYouSureLabel.Visible				= false;
			


		}

		private void ReplaceFolder_Click(object sender, System.EventArgs e)
		{
			if(Request.QueryString["expID"] == null)
				return;

		    string idLocal = Request.QueryString["expID"].ToString();

            SqlConnection connection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
			
			dbConnect.ExecuteQuery("UPDATE gm_IGNFolders set foldername ='"+dbConnect.SanitizeIn(NewFolderName.Text)+"' WHERE ORGANIZEGROUPID = '"+idLocal+"'",connection);
            StreamMenu(Convert.ToInt32(idLocal),connection);
			connection.Close();

			//SqlDataReader dbReader = dbConnect.sqlQuery("SELECT FOLDERNAME,ORGANIZEGROUPID FROM gm_IGNFolders ORDER BY ORGANIZEGROUPID",sqlConnection);


		}

		private void NewEntityBtn_Click(object sender, System.EventArgs e)
		{
			if(Request.QueryString["expID"] == null)
				return;

			 string idLocal = Request.QueryString["expID"].ToString();

             SqlConnection connection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());

				int newSpawnID = 0;
				SqlDataReader dbReader = dbConnect.sqlQuery("SELECT SPAWNID FROM GM_IGNENTITY ORDER BY SPAWNID DESC",connection);
				if(dbReader.Read())
				{
					newSpawnID = Convert.ToInt32(dbReader.GetSqlValue(0).ToString())+1;
				}
				dbReader.Close();
             string sqlQ = "INSERT INTO GM_IGNENTITY (GENERATORNAME,SPAWNID,ORGANIZEGROUPID) Values('"+dbConnect.SanitizeIn(NewFolderName.Text)+"','"+newSpawnID.ToString()+"','"+idLocal+"')";
			 dbConnect.ExecuteQuery(sqlQ,connection);
             StreamMenu(Convert.ToInt32(idLocal),connection);
			 connection.Close();
		}

		private void EntitySearchResultsList_SelectedIndexChanged(object sender, System.EventArgs e)
		{
            SqlConnection connection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
			SqlDataReader dbReader = dbConnect.sqlQuery("SELECT GENERATORNAME,DISPLAYNAME,SPAWNID,ORGANIZEGROUPID FROM GM_IGNENTITY WHERE SPAWNID = '"+EntitySearchResultsList.SelectedValue+"'",connection);
			if(dbReader.Read())
			{
				string url = "IGN_QuestMenuPage.aspx?expID=" +dbReader.GetSqlValue(3).ToString() + "&SpawnID=" +dbReader.GetSqlValue(2).ToString();
                dbReader.Close();
			    connection.Close();
                Response.Redirect(url);
				EntitySearchResultsList.Visible = false;
				return;
			}
			dbReader.Close();
			connection.Close();
			EntitySearchResultsList.Visible = false;
		}

		private void EntitySearchBtn_Click(object sender, System.EventArgs e)
		{
            SqlConnection connection = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
			EntitySearchResultsList.Items.Clear();
			SqlDataReader dbReader = dbConnect.sqlQuery("SELECT GENERATORNAME,DISPLAYNAME,SPAWNID FROM GM_IGNENTITY WHERE GENERATORNAME LIKE '%"+dbConnect.SanitizeIn(entitySearchCriteria.Text)+"%' OR DISPLAYNAME LIKE '%"+dbConnect.SanitizeIn(entitySearchCriteria.Text)+"%'",connection);
			while(dbReader.Read())
			{
				ListItem newItem = new ListItem(dbReader.GetSqlValue(0).ToString()+"/"+dbReader.GetSqlValue(1).ToString(),dbReader.GetSqlValue(2).ToString());
				EntitySearchResultsList.Items.Add(newItem);
			}
			dbReader.Close();
            connection.Close();
            EntitySearchResultsList.Visible = true;
		}

		private void ChatDataBtn_Click(object sender, System.EventArgs e)
		{
			string spawnID = "-1";
			if(Session["SPAWNID"] != null)
				spawnID = Session["SPAWNID"].ToString();

			int expID = -1;
			
			if(Request.QueryString["expID"] != null)
				expID = Convert.ToInt32(Request.QueryString["expID"].ToString().Replace("'",""));
	

			string urlTemp = "IGN_QuestMenuPage.aspx?expID=" + expID + "&SpawnID=" + spawnID + "&QMODE=2";
			Response.Redirect(urlTemp);
		}
}

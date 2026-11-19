
	using System;
	using System.Data;
	using System.Drawing;
	using System.Web;
	using System.Web.UI.WebControls;
	using System.Web.UI.HtmlControls;
	using System.Data.SqlClient;

	/// <summary>
	///		Summary description for IGNEntityChatStrings.
	/// </summary>
	public partial class IGNEntityChatStrings : System.Web.UI.UserControl
	{
		DatabaseConnectionModule dataCon = new DatabaseConnectionModule();
		LogSystem m_logReporter = new LogSystem();
        ConnectionHelper m_conHelper = new ConnectionHelper();

		private void Page_Load(object sender, System.EventArgs e)
		{
         
			if(Session["QUESTMODEON"] == null)
			{
				this.Visible = false;
				return;
			}

			if(Session["QUESTMODEON"].ToString() != "2")
			{
				this.Visible = false;
				return;
			}

			this.Visible = true;


            if(chatStringsList.Items.Count > 0)
			   return;

			if(Session["SPAWNID"] == null)
			   return;




            SqlConnection connectionLoc = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
			
			InListChatStrings(-1,connectionLoc);
			connectionLoc.Close();
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
			this.AddNewBtn.Click += new System.EventHandler(this.AddNewBtn_Click);
			this.DeleteBtn.Click += new System.EventHandler(this.DeleteBtn_Click);
			this.ReplaceBtn.Click += new System.EventHandler(this.ReplaceBtn_Click);
			this.chatStringsList.SelectedIndexChanged += new System.EventHandler(this.chatStringsList_SelectedIndexChanged);
			this.Load += new System.EventHandler(this.Page_Load);

		}
		#endregion


		private void InListChatStrings(int sel,SqlConnection connectionLoc)
		{
			chatStringsList.Items.Clear();
			string sqlQ = "SELECT CHATSTRING FROM gm_IGNChatStrings where spawnID = '"+Session["SPAWNID"].ToString()+"' ORDER BY CHATINDEX";
			SqlDataReader xreader = dataCon.sqlQuery(sqlQ,connectionLoc);
			int trace = 0;
			while(xreader.Read())
			{
				chatStringsList.Items.Add(dataCon.SanitizeOutHtml(xreader.GetSqlValue(0).ToString()));
				trace++;
			}
			xreader.Close();
			if(sel > -1)
			   chatStringsList.SelectedIndex = sel;

		}

		private void AddNewBtn_Click(object sender, System.EventArgs e)
		{
            SqlConnection connectionLoc = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());

			m_logReporter.LogEvent("AX QUEST UPDATE","Add Chat spawnID:"+Session["SPAWNID"].ToString()+" user:"+Session["user"].ToString(),connectionLoc);

		    string sqlQ = "INSERT INTO gm_IGNChatStrings (SPAWNID,CHATSTRING,CHATINDEX) VALUES('"+Session["SPAWNID"].ToString()+"','"+dataCon.SanitizeIn(m_sequencedSpeechEdit.Text)+"','"+chatStringsList.Items.Count.ToString()+"')";
            dataCon.ExecuteQuery(sqlQ,connectionLoc);
			InListChatStrings(chatStringsList.Items.Count,connectionLoc);
            
			int newCount = chatStringsList.Items.Count + 1;
			sqlQ = "Update GM_IGNENTITY set chatStringCount = '" + newCount.ToString() + "' WHERE SPAWNID = '"+Session["SPAWNID"].ToString()+"'";
			dataCon.ExecuteQuery(sqlQ,connectionLoc);
			
			connectionLoc.Close();
		}

		private void DeleteBtn_Click(object sender, System.EventArgs e)
		{
			if(Session["SPAWNID"] == null)
				return;

            SqlConnection connectionLoc = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());

			m_logReporter.LogEvent("AX QUEST UPDATE","Delete Chat spawnID:"+Session["SPAWNID"].ToString()+" user:"+Session["user"].ToString(),connectionLoc);

			string sqlQ = "SELECT CHATSTRING FROM gm_IGNChatStrings WHERE SPAWNID = '"+Session["SPAWNID"].ToString()+"'";
			SqlDataReader dReader = dataCon.sqlQuery(sqlQ,connectionLoc);
			int tracer = 0;
			int doneDelete = 0;
			while(dReader.Read())
			{

				if(chatStringsList.SelectedIndex == tracer)
				{
					sqlQ = "DELETE FROM gm_IGNChatStrings WHERE SPAWNID = '"+Session["SPAWNID"].ToString()+"' AND CHATSTRING = '"+dReader.GetSqlValue(0).ToString()+"'";
					dataCon.ExecuteQuery(sqlQ);
					doneDelete = 1;
				}
				if(doneDelete == 1)
				{//update 
					sqlQ = "UPDATE gm_IGNChatStrings SET CHATINDEX = '"+tracer.ToString()+"' WHERE SPAWNID = '"+Session["SPAWNID"].ToString()+"' AND CHATSTRING = '"+dReader.GetSqlValue(0).ToString()+"'";
					dataCon.ExecuteQuery(sqlQ);
				}//end update
                  
				tracer++;
			}
			dReader.Close();

			InListChatStrings(chatStringsList.SelectedIndex-1,connectionLoc);

			int newCount = chatStringsList.Items.Count - 1;
			sqlQ = "Update GM_IGNENTITY set chatStringCount = '" + newCount.ToString() + "' WHERE SPAWNID = '"+Session["SPAWNID"].ToString()+"'";
			dataCon.ExecuteQuery(sqlQ,connectionLoc);

			connectionLoc.Close();
		}

		private void ReorderIndexStrings(SqlConnection connectionLoc)
		{
			if(Session["SPAWNID"] == null)
				return;

            SqlConnection connectionLoc2 = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());
            string sqlQ = "SELECT CHATSTRING FROM gm_IGNChatStrings WHERE SPAWNID = '"+Session["SPAWNID"].ToString()+"'";
            SqlDataReader dReader = dataCon.sqlQuery(sqlQ,connectionLoc);
			int tracer = 0;
			while(dReader.Read())
			{
                dataCon.ExecuteQuery("UPDATE gm_IGNChatStrings SET CHATINDEX = '"+tracer.ToString()+"' WHERE CHATSTRING = '"+dataCon.SanitizeIn(dReader.GetSqlValue(0).ToString())+"' AND SPAWNID = '"+Session["SPAWNID"].ToString()+"'",connectionLoc2);

				tracer++;
			}
			dReader.Close();
			connectionLoc2.Close();

		}

		private void ReplaceBtn_Click(object sender, System.EventArgs e)
		{
			if(Session["SPAWNID"] == null)
				return;

            SqlConnection connectionLoc = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());

			m_logReporter.LogEvent("AX QUEST UPDATE","Replace Chat spawnID:"+Session["SPAWNID"].ToString()+" user:"+Session["user"].ToString(),connectionLoc);

			string sqlQ = "SELECT CHATSTRING FROM gm_IGNChatStrings WHERE SPAWNID = '"+Session["SPAWNID"].ToString()+"'";
			SqlDataReader dReader = dataCon.sqlQuery(sqlQ,connectionLoc);
			int tracer = 0;
			while(dReader.Read())
			{

				if(chatStringsList.SelectedIndex == tracer)
				{
					int newIndex = tracer+1;
					sqlQ = "UPDATE gm_IGNChatStrings set CHATSTRING = '"+dataCon.SanitizeIn(m_sequencedSpeechEdit.Text)+"',chatIndex = '"+newIndex.ToString()+"' WHERE SPAWNID = '"+Session["SPAWNID"].ToString()+"' AND CHATSTRING = '"+dReader.GetSqlValue(0).ToString()+"'";
					dataCon.ExecuteQuery(sqlQ);
				}
				tracer++;
			}
			dReader.Close();


			//string sqlQ = "UPDATE gm_IGNChatStrings set CHATSTRING = '"+dataCon.SanitizeIn(m_sequencedSpeechEdit.Text)+"' WHERE SPAWNID = '"+Session["SPAWNID"].ToString()+"' AND CHATSTRING = '"+dataCon.SanitizeIn(chatStringsList.SelectedItem.Text)+"'";
			//dataCon.ExecuteQuery(sqlQ,connectionLoc);
			//ReorderIndexStrings(connectionLoc);
			InListChatStrings(chatStringsList.SelectedIndex,connectionLoc);
			connectionLoc.Close();
		}

		private void chatStringsList_SelectedIndexChanged(object sender, System.EventArgs e)
		{
            if(Session["SPAWNID"] == null)
			   return;

           SqlConnection connectionLoc = m_conHelper.GetServerHUBConnection(Session["ServerName"].ToString());

			string sqlQ = "SELECT CHATSTRING FROM gm_IGNChatStrings where spawnID = '"+Session["SPAWNID"].ToString()+"' ORDER BY CHATINDEX";
			SqlDataReader xreader = dataCon.sqlQuery(sqlQ,connectionLoc);
			int trace = 0;
			while(xreader.Read())
			{
				if(trace == chatStringsList.SelectedIndex)
				   m_sequencedSpeechEdit.Text = dataCon.SanitizeOutHtml(xreader.GetSqlValue(0).ToString());
				
				trace++;
			}
			xreader.Close();
			connectionLoc.Close();
		}
	}

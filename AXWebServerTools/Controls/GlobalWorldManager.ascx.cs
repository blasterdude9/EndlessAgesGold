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
	public partial class GlobalWorldManager : System.Web.UI.UserControl
	{
		DatabaseConnectionModule dbConModule = new DatabaseConnectionModule();

		private void Page_Load(object sender, System.EventArgs e)
		{
			AuthenticationModule m_authPtr = new AuthenticationModule();
			m_authPtr.AuthenticateUser("Main.aspx","DEVELOPER");

			if(m_aiWorldsList.Items.Count == 0)
			   InListWorlds(-1);
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
			this.AquireBtn.Click += new System.EventHandler(this.AquireBtn_Click);
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
			SqlDataReader sqlMainServerStatusReader = dbConModule.sqlQuery("select id, worldname, WorldChannel from tblSupportedWorlds",sqlAXServerConnection);
			m_aiWorldsList.Items.Clear();
			while(sqlMainServerStatusReader.Read())
			{
				m_aiWorldsList.Items.Add(sqlMainServerStatusReader.GetSqlValue(0).ToString() + " " + sqlMainServerStatusReader.GetSqlValue(1).ToString() + " Channel:"+sqlMainServerStatusReader.GetSqlValue(2).ToString());
			}
			sqlMainServerStatusReader.Close();
			sqlAXServerConnection.Close();
			if(sel > -1)
			   m_aiWorldsList.SelectedIndex = sel;
		}

		private void AddBtn_Click(object sender, System.EventArgs e)
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

			dbConModule.ExecuteQuery("insert into tblSupportedWorlds (worldname,id,WorldChannel) Values('"+m_worldFile.Text+
				"','"+m_worldIndex.Text +
				"','"+m_worldChannel.Text +
				"')",
				sqlAXServerConnection);
			sqlAXServerConnection.Close();

			InListWorlds(m_aiWorldsList.Items.Count);
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

            string id = "";
			char [] array = m_aiWorldsList.SelectedValue.ToString().ToCharArray();
			for(int loop=0;loop<m_aiWorldsList.SelectedValue.ToString().Length;loop++)
			{
                if(array[loop] == ' ')
				   break;
 
                id += array[loop];
			}
			

		    dbConModule.ExecuteQuery("update tblSupportedWorlds set worldname = '"+m_worldFile.Text+"', " +
				                                                "id = '"+m_worldIndex.Text+"', " +
				                                                "WorldChannel = '"+m_worldChannel.Text+"' where id ='"+id+"'",
				                                                sqlAXServerConnection);
			sqlAXServerConnection.Close();
			InListWorlds(m_aiWorldsList.SelectedIndex);
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

			string id = "";
			char [] array = m_aiWorldsList.SelectedValue.ToString().ToCharArray();
			for(int loop=0;loop<m_aiWorldsList.SelectedValue.ToString().Length;loop++)
			{
				if(array[loop] == ' ')
					break;
 
				id += array[loop];
			}

			int selTry = m_aiWorldsList.SelectedIndex -1;
			dbConModule.ExecuteQuery("delete from tblSupportedWorlds where id = '"+id+"'",sqlAXServerConnection);
			sqlAXServerConnection.Close();

			InListWorlds(selTry);
		}

		

		private void AquireBtn_Click(object sender, System.EventArgs e)
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
  
			string id = "";
			char [] array = m_aiWorldsList.SelectedValue.ToString().ToCharArray();
			for(int loop=0;loop<m_aiWorldsList.SelectedValue.ToString().Length;loop++)
			{
				if(array[loop] == ' ')
					break;
 
				id += array[loop];
			}


			SqlDataReader sqlMainServerStatusReader = dbConModule.sqlQuery("select worldname, id, WorldChannel from tblSupportedWorlds where id ='"+id+"'",sqlAXServerConnection);
			if(sqlMainServerStatusReader.Read())
			{
				m_worldFile.Text			= sqlMainServerStatusReader.GetSqlValue(0).ToString();
				m_worldIndex.Text		    = sqlMainServerStatusReader.GetSqlValue(1).ToString();
				m_worldChannel.Text			= sqlMainServerStatusReader.GetSqlValue(2).ToString();
			}
			sqlMainServerStatusReader.Close();
			sqlAXServerConnection.Close();
		}
	}

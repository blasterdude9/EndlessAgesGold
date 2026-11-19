
	using System;
	using System.Data;
	using System.Drawing;
	using System.Web;
	using System.Web.UI.WebControls;
	using System.Web.UI.HtmlControls;
	using System.Data.SqlClient;

	/// <summary>
	///		Summary description for LogViewerControl.
	/// </summary>
	public partial class LogViewerControl : System.Web.UI.UserControl
	{
		DatabaseConnectionModule dataCon = new DatabaseConnectionModule();

		private void Page_Load(object sender, System.EventArgs e)
		{
			AuthenticationModule m_authPtr = new AuthenticationModule();
			m_authPtr.AuthenticateUser("Main.aspx","STATUSADMIN");
			 if(m_statusOutput.Text.Length == 0)
			    RefreshLog(Convert.ToInt32(m_logCountToShow.Text));

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
			this.m_refreshBtn.Click += new System.EventHandler(this.m_refreshBtn_Click);
			this.Load += new System.EventHandler(this.Page_Load);

		}
		#endregion

		private void m_refreshBtn_Click(object sender, System.EventArgs e)
		{
		        RefreshLog(Convert.ToInt32(m_logCountToShow.Text));
		}

		private void RefreshLog(int count)
		{
			
		
			m_statusOutput.Text = "Update Time<BR><BR>";
			m_statusOutputCol2.Text = "Notification<BR><BR>";
			string serverName = (string)Session["ServerName"];
			string logTableName = (string)Session["logTableName"];
			string logModeName = (string)Session["logModeName"];
			if(serverName == null)
			{
				m_statusOutput.Text = "Server Name is NULL";
				return;
			}
			if(serverName == "")
			{
				m_statusOutput.Text = "Server Name is NULL";
				return;
			}
			m_logtitle.Text = logModeName + " " + serverName;
			ConnectionHelper conHlper = new ConnectionHelper();
			SqlConnection sqlAXServerConnection = conHlper.GetServerConnection(serverName);
			if(sqlAXServerConnection == null)
			{
				m_statusOutput.Text = "Could not connect to servers database";

				return;
			}
            string tableLog = "tblAiServerLog";
			if(logModeName == "Main Server Log:")
			   tableLog = "tblServerLog";


			string cmdString = "SELECT TOP "+count.ToString()+" logMsg, timePosted FROM "+tableLog+" ORDER BY timePosted DESC";
			SqlDataReader sqlMainServerStatusReader = dataCon.sqlQuery(cmdString,sqlAXServerConnection);
		    int tracer = 0;
			while(sqlMainServerStatusReader.Read())
			{

                m_statusOutput.Text += sqlMainServerStatusReader.GetSqlValue(1) + "<BR>";
				m_statusOutputCol2.Text += sqlMainServerStatusReader.GetSqlValue(0) + "<BR>";

				tracer++;
				if(tracer == count)
				   break;
			}
			sqlMainServerStatusReader.Close();
			sqlAXServerConnection.Close();
		}
	}

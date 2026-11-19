using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Web;
using System.Web.SessionState;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.HtmlControls;
using System.Data.SqlClient;

	/// <summary>
	/// Summary description for WebForm2.
	/// </summary>
	public partial class WebForm2 : System.Web.UI.Page
	{
		
		DatabaseConnectionModule dataCon = new DatabaseConnectionModule();
		LogSystem m_logReporter = new LogSystem();
        public static AXencrypt ax_encryptionManager = new AXencrypt();
	
		private void Page_Load(object sender, System.EventArgs e)
		{
			
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
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{    
			this.ChangePasswordBtn.Click += new System.EventHandler(this.ChangePasswordBtn_Click);
			this.ChangePassBtn.Click += new System.EventHandler(this.ChangePassBtn_Click);
			this.Load += new System.EventHandler(this.Page_Load);

		}
		#endregion

		protected void m_sendButton_Click(object sender, System.EventArgs e)
		{
			m_output.Text = "";
            SqlConnection myConnection = dataCon.CreateNativeConnectionObject();

			//test
			/*string enyptedInput0 = "Ý\fú€{W4<Gîƒ”%Eï…W•®Š…­\bµ]ÙÄ)<¼$";
			string testCH = "UPDATE gmAccountBase SET password = '"+enyptedInput0+"' where username = 'arb'";
			dataCon.ExecuteQuery(testCH,myConnection);*/
			
			
			Session["user"] = null;

			string test = "SELECT password FROM gmAccountBase where username = '"+m_repName.Text.Replace("'","")+"'";
			SqlDataReader sReader = dataCon.sqlQuery(test,myConnection);
			if(sReader.Read())
			{
				string decryptedInput = "";
                string passFromSQL = sReader.GetSqlValue(0).ToString();
				decryptedInput = ax_encryptionManager.Decrypt(passFromSQL);

				
				if(m_password.Text==decryptedInput)
				{
					Session["user"] =  m_repName.Text.Replace("'","");
					m_logReporter.LogEvent("AX Authenticated Session:",m_repName.Text,null);
				}
				else
				{
					m_output.Text = "Access Denied";
					m_logReporter.LogEvent("AX Authentication Failed:",m_repName.Text,null);
					sReader.Close();
					myConnection.Close();
					return;
				}
				
			}
			sReader.Close();
			myConnection.Close();
			
		       string redirect = "";
			   if(Session["redirect"] != null)
			      redirect = (string)Session["redirect"];
			   
			   if(redirect.Length == 0)
		          Server.Transfer("Main.aspx");
			   else
			      Server.Transfer(redirect);
	    
		}

		private void ChangePasswordBtn_Click(object sender, System.EventArgs e)
		{
			AuthPanel.Visible = false;
			ChangePassPanel.Visible = true;
		}

		private void ChangePassBtn_Click(object sender, System.EventArgs e)
		{

			AuthPanel.Visible = true;
			ChangePassPanel.Visible = false;

			m_output.Text = "";

			if(String.Compare(confPasswordEdit.Text,NewPasswordEdit.Text,true)!=0)
			{
			    m_output.Text = "new passwords dont match";
				
               return;
			}
			


			SqlConnection myConnection = dataCon.CreateNativeConnectionObject();

			
			Session["user"] = null;

			string test = "SELECT password FROM gmAccountBase where username = '"+ChangePassUsernameEdit.Text.Replace("'","")+"'";
			SqlDataReader sReader = dataCon.sqlQuery(test,myConnection);
			if(sReader.Read())
			{
				string passFromSQL = sReader.GetSqlValue(0).ToString();
				string decryptedInput = "";
				if(passFromSQL.Length > 0)
				   decryptedInput = ax_encryptionManager.Decrypt(passFromSQL);
				
				if(ExistingPassEdit.Text==decryptedInput)
				{
					string enyptedInput0 = ax_encryptionManager.Encrypt(dataCon.SanitizeInBasic(NewPasswordEdit.Text));
					string testCH = "UPDATE gmAccountBase SET password = '"+enyptedInput0+"' where username = '"+ChangePassUsernameEdit.Text.Replace("'","")+"'";
					sReader.Close();
					dataCon.ExecuteQuery(testCH,myConnection);
					m_output.Text = "Password Changed";
					
				}
				else
				{
                    m_output.Text = "Password Change Failed";
					sReader.Close();
				}
			}
			else
			   sReader.Close();

            m_logReporter.LogEvent("AX Password Change:",ChangePassUsernameEdit.Text,myConnection);

			myConnection.Close();
			


		}
       
}


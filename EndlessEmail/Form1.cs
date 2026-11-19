using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Web;
using System.Web.Mail;
using System.Data.SqlClient;
using System.Xml.XPath;
using System.Text;
using System.Xml;
using System.Net;
using System.IO;
using System.Threading;
namespace EndlessAdmin
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button CloseBtn;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private System.Windows.Forms.Label Messaging;
		private System.Windows.Forms.ProgressBar emailProgress;
		private System.Windows.Forms.Button EmailAllNewOnly;
		private System.Windows.Forms.TextBox MassMailEmailBody;
		private System.Windows.Forms.Button UpdateMassEmail;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Button MergeNewMail;
		private System.Windows.Forms.TextBox MassMailSubject;
		private System.Windows.Forms.TextBox MSdelay;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Button TestEmailBtn;
		private System.Windows.Forms.TextBox SendLimit;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TextBox m_testUsername;
		DatabaseConnectionModule dbConn = new DatabaseConnectionModule();
		private System.Windows.Forms.TabControl m_tabController;
		private System.Windows.Forms.TabPage tabPage1;
		private System.Windows.Forms.TabPage tabPage2;
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.TextBox m_mailType;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.CheckBox m_testModeCheck;
		private System.Windows.Forms.CheckBox m_htmlBasedCheck;
		private System.Windows.Forms.TextBox m_SMTPOvverride;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.TextBox testUsernameEdit;
		private System.Windows.Forms.Label m_statusLine;
		private System.Windows.Forms.TabPage tabPage3;
		private System.Windows.Forms.Button PreviewSendListBtn;
		private System.Windows.Forms.ListBox m_status;
		private System.Windows.Forms.GroupBox groupBox2;


		

		public Form1()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			SqlConnection conectionPtr = dbConn.CreateNativeConnectionObject();

			/*string strSQLall = "SELECT email,username,minutes from tblUsers where active = 1 and minutes <> '0'";
			SqlDataReader reader1 = dbConn.sqlQuery(strSQLall,conectionPtr);
			int trace = 0;
			while(reader1.Read())
			{

                  trace++;
			}
			reader1.Close();
            Messaging.Text = "Ready to email: " +  trace.ToString();*/

		


			


			conectionPtr.Close();
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.CloseBtn = new System.Windows.Forms.Button();
			this.Messaging = new System.Windows.Forms.Label();
			this.m_testUsername = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.SendLimit = new System.Windows.Forms.TextBox();
			this.TestEmailBtn = new System.Windows.Forms.Button();
			this.label3 = new System.Windows.Forms.Label();
			this.MSdelay = new System.Windows.Forms.TextBox();
			this.MergeNewMail = new System.Windows.Forms.Button();
			this.label2 = new System.Windows.Forms.Label();
			this.MassMailSubject = new System.Windows.Forms.TextBox();
			this.UpdateMassEmail = new System.Windows.Forms.Button();
			this.MassMailEmailBody = new System.Windows.Forms.TextBox();
			this.EmailAllNewOnly = new System.Windows.Forms.Button();
			this.emailProgress = new System.Windows.Forms.ProgressBar();
			this.m_tabController = new System.Windows.Forms.TabControl();
			this.tabPage1 = new System.Windows.Forms.TabPage();
			this.m_htmlBasedCheck = new System.Windows.Forms.CheckBox();
			this.m_mailType = new System.Windows.Forms.TextBox();
			this.button1 = new System.Windows.Forms.Button();
			this.tabPage2 = new System.Windows.Forms.TabPage();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.label5 = new System.Windows.Forms.Label();
			this.testUsernameEdit = new System.Windows.Forms.TextBox();
			this.m_SMTPOvverride = new System.Windows.Forms.TextBox();
			this.m_testModeCheck = new System.Windows.Forms.CheckBox();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.label4 = new System.Windows.Forms.Label();
			this.m_statusLine = new System.Windows.Forms.Label();
			this.tabPage3 = new System.Windows.Forms.TabPage();
			this.PreviewSendListBtn = new System.Windows.Forms.Button();
			this.m_status = new System.Windows.Forms.ListBox();
			this.m_tabController.SuspendLayout();
			this.tabPage1.SuspendLayout();
			this.tabPage2.SuspendLayout();
			this.groupBox2.SuspendLayout();
			this.groupBox1.SuspendLayout();
			this.tabPage3.SuspendLayout();
			this.SuspendLayout();
			// 
			// CloseBtn
			// 
			this.CloseBtn.BackColor = System.Drawing.SystemColors.ControlLight;
			this.CloseBtn.Location = new System.Drawing.Point(608, 344);
			this.CloseBtn.Name = "CloseBtn";
			this.CloseBtn.Size = new System.Drawing.Size(64, 24);
			this.CloseBtn.TabIndex = 0;
			this.CloseBtn.Text = "Close";
			this.CloseBtn.Click += new System.EventHandler(this.CloseBtn_Click);
			// 
			// Messaging
			// 
			this.Messaging.Font = new System.Drawing.Font("Microsoft Sans Serif", 7F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.Messaging.Location = new System.Drawing.Point(296, 16);
			this.Messaging.Name = "Messaging";
			this.Messaging.Size = new System.Drawing.Size(352, 216);
			this.Messaging.TabIndex = 5;
			// 
			// m_testUsername
			// 
			this.m_testUsername.Location = new System.Drawing.Point(560, 232);
			this.m_testUsername.Name = "m_testUsername";
			this.m_testUsername.Size = new System.Drawing.Size(72, 20);
			this.m_testUsername.TabIndex = 13;
			this.m_testUsername.Text = "";
			this.m_testUsername.Visible = false;
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(96, 48);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(87, 16);
			this.label1.TabIndex = 11;
			this.label1.Text = "Maximum Send";
			// 
			// SendLimit
			// 
			this.SendLimit.Location = new System.Drawing.Point(8, 48);
			this.SendLimit.Name = "SendLimit";
			this.SendLimit.Size = new System.Drawing.Size(80, 20);
			this.SendLimit.TabIndex = 10;
			this.SendLimit.Text = "50";
			// 
			// TestEmailBtn
			// 
			this.TestEmailBtn.Location = new System.Drawing.Point(560, 232);
			this.TestEmailBtn.Name = "TestEmailBtn";
			this.TestEmailBtn.Size = new System.Drawing.Size(71, 22);
			this.TestEmailBtn.TabIndex = 9;
			this.TestEmailBtn.Text = "Test Mail Msg";
			this.TestEmailBtn.Visible = false;
			this.TestEmailBtn.Click += new System.EventHandler(this.TestEmailBtn_Click);
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(96, 24);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(56, 17);
			this.label3.TabIndex = 8;
			this.label3.Text = "MS delay";
			// 
			// MSdelay
			// 
			this.MSdelay.Location = new System.Drawing.Point(8, 24);
			this.MSdelay.Name = "MSdelay";
			this.MSdelay.Size = new System.Drawing.Size(80, 20);
			this.MSdelay.TabIndex = 7;
			this.MSdelay.Text = "20";
			// 
			// MergeNewMail
			// 
			this.MergeNewMail.Location = new System.Drawing.Point(560, 232);
			this.MergeNewMail.Name = "MergeNewMail";
			this.MergeNewMail.TabIndex = 12;
			this.MergeNewMail.Visible = false;
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(16, 104);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(80, 16);
			this.label2.TabIndex = 5;
			this.label2.Text = "Email Subject:";
			// 
			// MassMailSubject
			// 
			this.MassMailSubject.Location = new System.Drawing.Point(96, 104);
			this.MassMailSubject.Name = "MassMailSubject";
			this.MassMailSubject.Size = new System.Drawing.Size(160, 20);
			this.MassMailSubject.TabIndex = 4;
			this.MassMailSubject.Text = "textBox1";
			// 
			// UpdateMassEmail
			// 
			this.UpdateMassEmail.Location = new System.Drawing.Point(16, 272);
			this.UpdateMassEmail.Name = "UpdateMassEmail";
			this.UpdateMassEmail.Size = new System.Drawing.Size(103, 24);
			this.UpdateMassEmail.TabIndex = 3;
			this.UpdateMassEmail.Text = "Update Email";
			this.UpdateMassEmail.Click += new System.EventHandler(this.UpdateMassEmail_Click);
			// 
			// MassMailEmailBody
			// 
			this.MassMailEmailBody.Location = new System.Drawing.Point(16, 128);
			this.MassMailEmailBody.Multiline = true;
			this.MassMailEmailBody.Name = "MassMailEmailBody";
			this.MassMailEmailBody.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.MassMailEmailBody.Size = new System.Drawing.Size(456, 140);
			this.MassMailEmailBody.TabIndex = 2;
			this.MassMailEmailBody.Text = "";
			// 
			// EmailAllNewOnly
			// 
			this.EmailAllNewOnly.Location = new System.Drawing.Point(8, 240);
			this.EmailAllNewOnly.Name = "EmailAllNewOnly";
			this.EmailAllNewOnly.Size = new System.Drawing.Size(72, 32);
			this.EmailAllNewOnly.TabIndex = 1;
			this.EmailAllNewOnly.Text = "Do Email";
			this.EmailAllNewOnly.Click += new System.EventHandler(this.EmailAllNewOnly_Click);
			// 
			// emailProgress
			// 
			this.emailProgress.Location = new System.Drawing.Point(8, 280);
			this.emailProgress.Name = "emailProgress";
			this.emailProgress.Size = new System.Drawing.Size(640, 8);
			this.emailProgress.TabIndex = 0;
			// 
			// m_tabController
			// 
			this.m_tabController.Controls.Add(this.tabPage1);
			this.m_tabController.Controls.Add(this.tabPage2);
			this.m_tabController.Controls.Add(this.tabPage3);
			this.m_tabController.Location = new System.Drawing.Point(8, 8);
			this.m_tabController.Name = "m_tabController";
			this.m_tabController.SelectedIndex = 0;
			this.m_tabController.Size = new System.Drawing.Size(664, 328);
			this.m_tabController.TabIndex = 7;
			// 
			// tabPage1
			// 
			this.tabPage1.Controls.Add(this.m_statusLine);
			this.tabPage1.Controls.Add(this.m_htmlBasedCheck);
			this.tabPage1.Controls.Add(this.m_mailType);
			this.tabPage1.Controls.Add(this.button1);
			this.tabPage1.Controls.Add(this.MassMailEmailBody);
			this.tabPage1.Controls.Add(this.MassMailSubject);
			this.tabPage1.Controls.Add(this.label2);
			this.tabPage1.Controls.Add(this.UpdateMassEmail);
			this.tabPage1.Location = new System.Drawing.Point(4, 22);
			this.tabPage1.Name = "tabPage1";
			this.tabPage1.Size = new System.Drawing.Size(656, 302);
			this.tabPage1.TabIndex = 0;
			this.tabPage1.Text = "Setup";
			// 
			// m_htmlBasedCheck
			// 
			this.m_htmlBasedCheck.Location = new System.Drawing.Point(16, 72);
			this.m_htmlBasedCheck.Name = "m_htmlBasedCheck";
			this.m_htmlBasedCheck.TabIndex = 8;
			this.m_htmlBasedCheck.Text = "HTML Format";
			// 
			// m_mailType
			// 
			this.m_mailType.Location = new System.Drawing.Point(200, 32);
			this.m_mailType.Name = "m_mailType";
			this.m_mailType.Size = new System.Drawing.Size(128, 20);
			this.m_mailType.TabIndex = 7;
			this.m_mailType.Text = "Enter Mail Type";
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(16, 32);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(176, 23);
			this.button1.TabIndex = 6;
			this.button1.Text = "Aquire Mail By Type";
			this.button1.Click += new System.EventHandler(this.button1_Click_1);
			// 
			// tabPage2
			// 
			this.tabPage2.Controls.Add(this.groupBox2);
			this.tabPage2.Controls.Add(this.EmailAllNewOnly);
			this.tabPage2.Controls.Add(this.TestEmailBtn);
			this.tabPage2.Controls.Add(this.m_testUsername);
			this.tabPage2.Controls.Add(this.emailProgress);
			this.tabPage2.Controls.Add(this.Messaging);
			this.tabPage2.Controls.Add(this.MergeNewMail);
			this.tabPage2.Location = new System.Drawing.Point(4, 22);
			this.tabPage2.Name = "tabPage2";
			this.tabPage2.Size = new System.Drawing.Size(656, 302);
			this.tabPage2.TabIndex = 1;
			this.tabPage2.Text = "Send Mail";
			// 
			// groupBox2
			// 
			this.groupBox2.Controls.Add(this.label5);
			this.groupBox2.Controls.Add(this.testUsernameEdit);
			this.groupBox2.Controls.Add(this.m_SMTPOvverride);
			this.groupBox2.Controls.Add(this.m_testModeCheck);
			this.groupBox2.Controls.Add(this.groupBox1);
			this.groupBox2.Controls.Add(this.label4);
			this.groupBox2.Location = new System.Drawing.Point(8, 8);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(240, 224);
			this.groupBox2.TabIndex = 15;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "Settings";
			// 
			// label5
			// 
			this.label5.Location = new System.Drawing.Point(136, 64);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(88, 16);
			this.label5.TabIndex = 18;
			this.label5.Text = "Test Username";
			// 
			// testUsernameEdit
			// 
			this.testUsernameEdit.Location = new System.Drawing.Point(8, 64);
			this.testUsernameEdit.Name = "testUsernameEdit";
			this.testUsernameEdit.Size = new System.Drawing.Size(120, 20);
			this.testUsernameEdit.TabIndex = 17;
			this.testUsernameEdit.Text = "";
			// 
			// m_SMTPOvverride
			// 
			this.m_SMTPOvverride.Location = new System.Drawing.Point(8, 88);
			this.m_SMTPOvverride.Name = "m_SMTPOvverride";
			this.m_SMTPOvverride.Size = new System.Drawing.Size(120, 20);
			this.m_SMTPOvverride.TabIndex = 15;
			this.m_SMTPOvverride.Text = "";
			// 
			// m_testModeCheck
			// 
			this.m_testModeCheck.Checked = true;
			this.m_testModeCheck.CheckState = System.Windows.Forms.CheckState.Checked;
			this.m_testModeCheck.Location = new System.Drawing.Point(8, 16);
			this.m_testModeCheck.Name = "m_testModeCheck";
			this.m_testModeCheck.TabIndex = 13;
			this.m_testModeCheck.Text = "TEST MODE";
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.MSdelay);
			this.groupBox1.Controls.Add(this.label3);
			this.groupBox1.Controls.Add(this.SendLimit);
			this.groupBox1.Controls.Add(this.label1);
			this.groupBox1.Location = new System.Drawing.Point(8, 120);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(192, 96);
			this.groupBox1.TabIndex = 14;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Throttling";
			// 
			// label4
			// 
			this.label4.Location = new System.Drawing.Point(136, 88);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(40, 16);
			this.label4.TabIndex = 16;
			this.label4.Text = "SMTP";
			// 
			// m_statusLine
			// 
			this.m_statusLine.Location = new System.Drawing.Point(16, 8);
			this.m_statusLine.Name = "m_statusLine";
			this.m_statusLine.Size = new System.Drawing.Size(200, 16);
			this.m_statusLine.TabIndex = 10;
			// 
			// tabPage3
			// 
			this.tabPage3.Controls.Add(this.PreviewSendListBtn);
			this.tabPage3.Controls.Add(this.m_status);
			this.tabPage3.Location = new System.Drawing.Point(4, 22);
			this.tabPage3.Name = "tabPage3";
			this.tabPage3.Size = new System.Drawing.Size(656, 302);
			this.tabPage3.TabIndex = 2;
			this.tabPage3.Text = "Preview";
			// 
			// PreviewSendListBtn
			// 
			this.PreviewSendListBtn.Location = new System.Drawing.Point(8, 8);
			this.PreviewSendListBtn.Name = "PreviewSendListBtn";
			this.PreviewSendListBtn.Size = new System.Drawing.Size(120, 23);
			this.PreviewSendListBtn.TabIndex = 11;
			this.PreviewSendListBtn.Text = "Preview Send List";
			this.PreviewSendListBtn.Click += new System.EventHandler(this.PreviewSendListBtn_Click);
			// 
			// m_status
			// 
			this.m_status.Location = new System.Drawing.Point(8, 40);
			this.m_status.Name = "m_status";
			this.m_status.Size = new System.Drawing.Size(632, 238);
			this.m_status.TabIndex = 9;
			this.m_status.Visible = false;
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(688, 382);
			this.Controls.Add(this.m_tabController);
			this.Controls.Add(this.CloseBtn);
			this.Name = "Form1";
			this.Text = "Emailer";
			this.m_tabController.ResumeLayout(false);
			this.tabPage1.ResumeLayout(false);
			this.tabPage2.ResumeLayout(false);
			this.groupBox2.ResumeLayout(false);
			this.groupBox1.ResumeLayout(false);
			this.tabPage3.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new Form1());
		}

		private void CloseBtn_Click(object sender, System.EventArgs e)
		{
			Application.Exit();
		}

	
		//--------------------------------------------------------------------//
		//-------------------------process cards------------------------------//
		//--------------------------------------------------------------------//

		//Send Mail String
		public void SendMail(string targetEmail,
			                 string mailmsg,
			                 string subject,
			                 bool htmlBased)
		{
			SmtpMail.SmtpServer = m_SMTPOvverride.Text;
			//SmtpMail.SmtpServer = "mail.endlessagesportal.com";
			//SmtpMail.SmtpServer = "mail.bellsouth.net";

			MailMessage myMail = new MailMessage();
			myMail.From = "updates@mmolands.com";
			myMail.To = targetEmail;
			myMail.Subject = subject;
			myMail.Priority = MailPriority.Normal;
			if(htmlBased == false)
			{
				myMail.BodyFormat = MailFormat.Text;
				myMail.Body = mailmsg;
			}
			else
			{
				myMail.BodyFormat = MailFormat.Html;
				myMail.Body = "<HTML><body>" + mailmsg + "</body></HTML>";
			}
			SmtpMail.Send(myMail);
		}
		//Process all recurring
		

		//-------------------mail processor----------------------//
		//-------------------------------------------------------//
		//------------test only sends to admin only--------------//

		public bool DoEmailProcess(bool testOnly, string emailType)
		{

			return true;
		}

		private void EmailAllNewOnly_Click(object sender, System.EventArgs e)
		{
			SqlConnection conectionPtr = dbConn.CreateNativeConnectionObject();
            //--------------------//
			//-----BUILD EMAIL----//
            //--------------------//
			SqlDataReader reader = dbConn.sqlQuery("SELECT subject,body,htmlBased from stencil_mail where mailType = '"+m_mailType.Text+"'", conectionPtr);
			string emailSubject = "";
			string emailBody = "";
			bool htmlBased = false;
			int delay = Convert.ToInt32(MSdelay.Text);
			if(reader.Read())
			{
				emailBody = reader.GetSqlValue(1).ToString();
				emailSubject = reader.GetSqlValue(0).ToString();
				if(reader.GetSqlValue(2).ToString() == "1")
				   htmlBased = true;
				else
				   htmlBased = false;
			}
			else
			{
				Messaging.Text = "Mail request has failed...";
				Messaging.Update();
				reader.Close();
				conectionPtr.Close();
				return;
			}
			reader.Close();
			//-------------------------//
			//-----AQUIRE SEND SIZE----//
			//-------------------------//
           



			//-------------------------//
			//--------SEND MAIL--------//  
			//-------------------------//
			SqlConnection updateConn = dbConn.CreateNativeConnectionObject();
			emailProgress.Maximum = Convert.ToInt32(SendLimit.Text);
			emailProgress.Minimum = 0;
			string strSQLall = "select TOP "+SendLimit.Text+" username, email, minutesPlayed from gmAccountBase where newsletter = '1' and minutesplayed > '0' AND LASTLETTERTYPE <> '"+m_mailType.Text+"' ORDER BY JOINED";
			if(m_testModeCheck.Checked)
			{
				strSQLall = "select username, email, minutesPlayed from gmAccountBase where username = '"+testUsernameEdit.Text+"'";
                emailProgress.Maximum = 1;
			}

			SqlDataReader reader1 = dbConn.sqlQuery(strSQLall,conectionPtr);
			int trace = 0;
			
			
            int currentPosition = 0;
			while(reader1.Read())
			{
				string minutes = "0";
				string username = reader1.GetSqlValue(0).ToString();
				minutes = reader1.GetSqlValue(2).ToString();
				string email = reader1.GetSqlValue(1).ToString();
				
				
				Messaging.Text = "("+trace.ToString()+")Sending Mail To:"+email;
				trace++;
				Messaging.Update();
				int succeeded = 1;
				string emailNew = emailBody.Replace("-username",username);
				emailNew = emailNew.Replace("-minutes",minutes);
				string error = "";
				string finalSubject = emailSubject.Replace("-username",username);
				try
				{
					SendMail(email,emailNew,finalSubject,htmlBased);
				}
				catch(System.Web.HttpException ehttp)
				{
					succeeded = 0;
					error = ehttp.ToString();
				}	
				if(succeeded == 0)
				{
					Messaging.Text = "Failed to send:"+error;
					dbConn.ExecuteQuery("UPDATE gmAccountBase SET lastLetterType = '"+m_mailType.Text+"' where username = '"+username+"'",updateConn);
				}
				else
				{
					//update lastLetterType to mail type
					dbConn.ExecuteQuery("UPDATE gmAccountBase SET lastLetterType = '"+m_mailType.Text+"' where username = '"+username+"'",updateConn);
				}
				Thread.Sleep(delay);
                currentPosition++;
				emailProgress.Value = currentPosition;
			}
			reader1.Close();
			updateConn.Close();
			conectionPtr.Close();
		}

		private void UpdateMassEmail_Click(object sender, System.EventArgs e)
		{
            string htmlFormatting = "0";
            if(m_htmlBasedCheck.Checked == true)
			   htmlFormatting = "1";

		    dbConn.ExecuteQuery("Update stencil_mail set subject = '" + MassMailSubject.Text + "', body = '" + MassMailEmailBody.Text + "',htmlBased = '"+htmlFormatting+"' where mailtype = 'Community'");
		}

		//make subscription/recurring payment

		private void SendTestMail()
		{
            /*SqlConnection conectionPtr = dbConn.CreateNativeConnectionObject();

		    string strSQLall = "SELECT email,username,minutes from tblUsers where username = '"+m_testUsername.Text+"'";
			SqlDataReader reader1 = dbConn.sqlQuery(strSQLall,conectionPtr);
			int trace = 0;
			string minutes = "0";
			string username = m_testUsername.Text;
			if(reader1.Read())
			{
                minutes = reader1.GetSqlValue(2).ToString();
				trace++;
			}
			reader1.Close();


			SqlDataReader reader = dbConn.sqlQuery("SELECT * from tblEmails where emailType = 'MASS'", conectionPtr);
			string emailSubject = "";
			string emailBody = "";
			int delay = Convert.ToInt32(MSdelay.Text);
			if(reader.Read())
			{
				emailBody = reader.GetSqlValue(1).ToString();
				emailSubject = reader.GetSqlValue(2).ToString();
			}
			else
			{
				Messaging.Text = "Mail request has failed...";
				Messaging.Update();
				reader.Close();
				conectionPtr.Close();
				return;
			}
			reader.Close();

			Messaging.Text = "Sending Test Mail...";
			Messaging.Update();
			int succeeded = 1;
			string emailNew = emailBody.Replace("-username",username);
			emailNew = emailNew.Replace("-minutes",minutes);
            string error = "";
			emailSubject = emailSubject.Replace("-username",username);
			try
			{
				SendMail("avaria@endlessagesportal.net",emailNew,emailSubject);
			}
			catch(System.Web.HttpException ehttp)
			{
                succeeded = 0;
				error = ehttp.ToString();
			}	
			if(succeeded == 0)
			{
               Messaging.Text = "Failed to send:"+error;
			}

			conectionPtr.Close();*/
		}

		
		

		private void TestEmailBtn_Click(object sender, System.EventArgs e)
		{
			
            //SendTestMail();

	    
		}

		private void button1_Click(object sender, System.EventArgs e)
		{
		
		}

		private void button1_Click_1(object sender, System.EventArgs e)
		{
			
			SqlConnection conectionPtr = dbConn.CreateNativeConnectionObject();
			SqlDataReader reader = dbConn.sqlQuery("SELECT subject,body,htmlBased from stencil_mail where mailType = '"+m_mailType.Text+"'", conectionPtr);
			if(reader.Read())
			{
				MassMailEmailBody.Text = reader.GetSqlValue(1).ToString();
				MassMailSubject.Text = reader.GetSqlValue(0).ToString();
				
				if(reader.GetSqlValue(2).ToString() == "1")
				   m_htmlBasedCheck.Checked = true;
				else
				   m_htmlBasedCheck.Checked = false;
			}
			reader.Close();


            //get mail send count
			string strSQLall = "select COUNT(*) as totalUsers from gmAccountBase where newsletter = '1' and minutesplayed > '0' AND LASTLETTERTYPE <> '"+m_mailType.Text+"'";
			SqlDataReader readerX = dbConn.sqlQuery(strSQLall,conectionPtr);
			if(readerX.Read())
			{ 
			  m_statusLine.Text = "Send Que Size:"+readerX.GetSqlValue(0).ToString();
              //m_status. = readerX.GetSqlValue(0).ToString();
   
			}
			readerX.Close();
			conectionPtr.Close();
		}

		private void PreviewSendListBtn_Click(object sender, System.EventArgs e)
		{
			SqlConnection conectionPtr = dbConn.CreateNativeConnectionObject();
		    m_status.Visible = true;
             
            string build = "";
			m_status.Items.Clear();
			//get mail send count
			string strSQLall = "select TOP "+SendLimit.Text+" email,username,minutesplayed from gmAccountBase where newsletter = '1' and minutesplayed > '0' AND LASTLETTERTYPE <> '"+m_mailType.Text+"' ORDER BY JOINED";
			SqlDataReader readerX = dbConn.sqlQuery(strSQLall,conectionPtr);
			while(readerX.Read())
			{ 
				build = readerX.GetSqlValue(0).ToString()+" "+readerX.GetSqlValue(1).ToString()+" "+readerX.GetSqlValue(2).ToString();
				m_status.Items.Add(build);
   
			}
			readerX.Close();
            conectionPtr.Close();


		}
			

	
	}
}

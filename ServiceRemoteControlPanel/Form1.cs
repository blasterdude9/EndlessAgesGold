using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Data.SqlClient;

namespace ServiceRemoteControlPanel
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.NotifyIcon m_notifyIconObj;
		private System.Windows.Forms.Button HideMenuBtn;
		private System.Windows.Forms.Button CloseAppBtn;
		private System.ComponentModel.IContainer components;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.ListBox m_listBoxCnt;
		private System.Windows.Forms.Label m_errorOutputObj;
		private System.Windows.Forms.Timer timer1;
		bool m_showComponent;

		public Form1()
		{
			m_showComponent = false;
			
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
            m_errorOutputObj.Text = "Initializing";
			RefreshServiceList();
			
            
			//
			// TODO: Add any constructor code after InitializeComponent call
			//
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
			this.components = new System.ComponentModel.Container();
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(Form1));
			this.m_notifyIconObj = new System.Windows.Forms.NotifyIcon(this.components);
			this.HideMenuBtn = new System.Windows.Forms.Button();
			this.CloseAppBtn = new System.Windows.Forms.Button();
			this.m_listBoxCnt = new System.Windows.Forms.ListBox();
			this.label1 = new System.Windows.Forms.Label();
			this.m_errorOutputObj = new System.Windows.Forms.Label();
			this.timer1 = new System.Windows.Forms.Timer(this.components);
			this.SuspendLayout();
			// 
			// m_notifyIconObj
			// 
			this.m_notifyIconObj.Icon = ((System.Drawing.Icon)(resources.GetObject("m_notifyIconObj.Icon")));
			this.m_notifyIconObj.Text = "Service Control";
			this.m_notifyIconObj.Visible = true;
			this.m_notifyIconObj.DoubleClick += new System.EventHandler(this.m_notifyIconObj_DoubleClick);
			// 
			// HideMenuBtn
			// 
			this.HideMenuBtn.BackColor = System.Drawing.SystemColors.ControlDark;
			this.HideMenuBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.HideMenuBtn.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.HideMenuBtn.Location = new System.Drawing.Point(1, 271);
			this.HideMenuBtn.Name = "HideMenuBtn";
			this.HideMenuBtn.Size = new System.Drawing.Size(53, 23);
			this.HideMenuBtn.TabIndex = 0;
			this.HideMenuBtn.Text = "Hide";
			this.HideMenuBtn.Click += new System.EventHandler(this.HideMenuBtn_Click);
			// 
			// CloseAppBtn
			// 
			this.CloseAppBtn.BackColor = System.Drawing.SystemColors.ControlDark;
			this.CloseAppBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.CloseAppBtn.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.CloseAppBtn.Location = new System.Drawing.Point(175, 0);
			this.CloseAppBtn.Name = "CloseAppBtn";
			this.CloseAppBtn.Size = new System.Drawing.Size(16, 20);
			this.CloseAppBtn.TabIndex = 1;
			this.CloseAppBtn.Text = "x";
			this.CloseAppBtn.Click += new System.EventHandler(this.CloseAppBtn_Click);
			// 
			// m_listBoxCnt
			// 
			this.m_listBoxCnt.Font = new System.Drawing.Font("Microsoft Sans Serif", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.m_listBoxCnt.ItemHeight = 12;
			this.m_listBoxCnt.Location = new System.Drawing.Point(4, 23);
			this.m_listBoxCnt.Name = "m_listBoxCnt";
			this.m_listBoxCnt.Size = new System.Drawing.Size(178, 244);
			this.m_listBoxCnt.TabIndex = 2;
			this.m_listBoxCnt.DoubleClick += new System.EventHandler(this.m_listBoxCnt_DoubleClick);
			this.m_listBoxCnt.SelectedIndexChanged += new System.EventHandler(this.m_listBoxCnt_SelectedIndexChanged);
			// 
			// label1
			// 
			this.label1.ForeColor = System.Drawing.SystemColors.Window;
			this.label1.Location = new System.Drawing.Point(2, 7);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(92, 14);
			this.label1.TabIndex = 5;
			this.label1.Text = "Remote Services";
			// 
			// m_errorOutputObj
			// 
			this.m_errorOutputObj.ForeColor = System.Drawing.SystemColors.Window;
			this.m_errorOutputObj.Location = new System.Drawing.Point(63, 286);
			this.m_errorOutputObj.Name = "m_errorOutputObj";
			this.m_errorOutputObj.Size = new System.Drawing.Size(127, 16);
			this.m_errorOutputObj.TabIndex = 6;
			this.m_errorOutputObj.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// timer1
			// 
			this.timer1.Enabled = true;
			this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
			// 
			// Form1
			// 
			this.AutoScale = false;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.ClientSize = new System.Drawing.Size(191, 302);
			this.ControlBox = false;
			this.Controls.Add(this.m_errorOutputObj);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.m_listBoxCnt);
			this.Controls.Add(this.CloseAppBtn);
			this.Controls.Add(this.HideMenuBtn);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.Name = "Form1";
			this.Opacity = 0.85;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.TopMost = true;
			this.Load += new System.EventHandler(this.Form1_Load);
			this.DoubleClick += new System.EventHandler(this.Form1_DoubleClick);
			this.Activated += new System.EventHandler(this.Form1_Activated);
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

		//sql Querry
		public SqlDataReader sqlQuery(String query , bool isStoredProc)
		{
		    string strConnection = "server=YOUR_SQL_SERVER;database=servicecontrol;uid=YOUR_SQL_USER;password=YOUR_SQL_PASSWORD;";
			SqlConnection myConnection = new SqlConnection(strConnection);
			SqlCommand myCommand = new SqlCommand(query);
			if (isStoredProc == true)
			{
				myCommand.CommandType= CommandType.StoredProcedure;
			}
			myCommand.Connection = myConnection;
			myConnection.Open();
			SqlDataReader reader = myCommand.ExecuteReader();

			return reader;
		}

		private void RefreshServiceList()
		{
			try
			{
				m_listBoxCnt.Items.Clear();
				SqlDataReader reader = sqlQuery("SELECT serverNameID,enabled,servicename FROM tblServiceControl", false);
				while(reader.Read())
				{
					string stateLoc = reader.GetSqlValue(1).ToString();
					if(stateLoc == "1")
					   m_listBoxCnt.Items.Add("On:"+reader.GetSqlValue(0).ToString());
					else
					   m_listBoxCnt.Items.Add("Off:"+reader.GetSqlValue(0).ToString());
				}
				reader.Close();
				m_errorOutputObj.Text = "Connected";
			}
			catch
			{
                m_errorOutputObj.Text = "SQL Connection Error";
			}
		}

		private void CloseAppBtn_Click(object sender, System.EventArgs e)
		{
		    Application.Exit();
		}

		private void ToggleOpenClose()
		{
			if(m_showComponent == true)
			{
				//this.Visible = false;
				this.Hide();
				m_showComponent = false;
			}
			else
			{
				
				this.Show();
				RefreshServiceList();
                m_showComponent = true;
                //this.Visible = true;
			}
			
		}

		private void HideMenuBtn_Click(object sender, System.EventArgs e)
		{
		     ToggleOpenClose();
			 
		}

		private void m_notifyIconObj_DoubleClick(object sender, System.EventArgs e)
		{
		     ToggleOpenClose();
		}

		private void Form1_Activated(object sender, System.EventArgs e)
		{
		   
		}

		private void m_listBoxCnt_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			
		}

		private void m_listBoxCnt_DoubleClick(object sender, System.EventArgs e)
		{
			try
			{
				int persist = m_listBoxCnt.SelectedIndex;
				SqlDataReader reader = sqlQuery("SELECT serverNameID,enabled FROM tblServiceControl", false);
				int tracer = 0;
				while(reader.Read())
				{
					if(tracer == m_listBoxCnt.SelectedIndex)
					{
						string stateLoc = reader.GetSqlValue(1).ToString();
						if(stateLoc == "1")
						{
							SqlDataReader readerTemp = sqlQuery("Update tblServiceControl set enabled='0' where serverNameID = '"+reader.GetSqlValue(0).ToString()+"'", false);
							readerTemp.Close();
							break;
						}
						else
						{
							SqlDataReader readerTemp = sqlQuery("Update tblServiceControl set enabled='1' where serverNameID = '"+reader.GetSqlValue(0).ToString()+"'", false);
							readerTemp.Close();
							break;
						}
					}
					tracer++;
				}
				reader.Close();
				RefreshServiceList();
				m_errorOutputObj.Text = "Connected";
				m_listBoxCnt.SelectedIndex = persist;
			}
			catch
			{
				m_errorOutputObj.Text = "SQL Connection Error";
			}
		}

		private void Form1_Load(object sender, System.EventArgs e)
		{
          
            Point pointObj = this.DesktopLocation;
			
		    
			this.SetDesktopLocation((pointObj.X*2),(pointObj.Y*2));
			this.Update();
	
		}

		private void Form1_DoubleClick(object sender, System.EventArgs e)
		{
			//Rectangle xRect = SystemInformation.VirtualScreen;
            
			//this.SetDesktopLocation(0,0);
			
			//Form1.MousePosition
		     
		}

		private void timer1_Tick(object sender, System.EventArgs e)
		{
		    this.Hide();
			timer1.Stop();
		}
	}
}

using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Data.SqlClient;
namespace EndlessAdmin
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button CloseBtn;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Button PrintShipmantsBtn;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private System.Windows.Forms.ListBox ShipmentsList;
		private System.Windows.Forms.Button button1;

		public string strConnection = "server=YOUR_SQL_SERVER;database=endlessagescom;uid=YOUR_SQL_USER;password=YOUR_SQL_PASSWORD;";

		public Form1()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			string strSQLall = "SELECT * FROM tblShipments where shipped = '0' and ProcessingStatus <> 'Awaiting Payment'";
			DataSet objDataSet3 = new DataSet();
			SqlConnection objConnection = new SqlConnection(strConnection);
			SqlDataAdapter objDataAdapter3 = new SqlDataAdapter(strSQLall, objConnection);
			int recordCount = objDataAdapter3.Fill(objDataSet3,"dataBlock");
			DataView objDataView3 = new DataView(objDataSet3.Tables["dataBlock"]);
			DataTable tblObj3 = objDataSet3.Tables["dataBlock"];
			Object tempVar;
			for(int loop=0;loop<recordCount;loop++)
			{
				DataRow rowLocal = tblObj3.Rows[loop];
				ShipmentsList.Items.Add(rowLocal["Name"]);
			}
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
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.ShipmentsList = new System.Windows.Forms.ListBox();
			this.PrintShipmantsBtn = new System.Windows.Forms.Button();
			this.button1 = new System.Windows.Forms.Button();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// CloseBtn
			// 
			this.CloseBtn.BackColor = System.Drawing.SystemColors.ControlLight;
			this.CloseBtn.Location = new System.Drawing.Point(360, 176);
			this.CloseBtn.Name = "CloseBtn";
			this.CloseBtn.Size = new System.Drawing.Size(48, 32);
			this.CloseBtn.TabIndex = 0;
			this.CloseBtn.Text = "Close";
			this.CloseBtn.Click += new System.EventHandler(this.CloseBtn_Click);
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.AddRange(new System.Windows.Forms.Control[] {
																					this.ShipmentsList,
																					this.PrintShipmantsBtn});
			this.groupBox1.Location = new System.Drawing.Point(16, 72);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(304, 128);
			this.groupBox1.TabIndex = 1;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Current Shipments";
			// 
			// ShipmentsList
			// 
			this.ShipmentsList.Location = new System.Drawing.Point(8, 24);
			this.ShipmentsList.Name = "ShipmentsList";
			this.ShipmentsList.Size = new System.Drawing.Size(192, 95);
			this.ShipmentsList.TabIndex = 1;
			// 
			// PrintShipmantsBtn
			// 
			this.PrintShipmantsBtn.Location = new System.Drawing.Point(208, 88);
			this.PrintShipmantsBtn.Name = "PrintShipmantsBtn";
			this.PrintShipmantsBtn.Size = new System.Drawing.Size(88, 32);
			this.PrintShipmantsBtn.TabIndex = 0;
			this.PrintShipmantsBtn.Text = "Print Ship List";
			this.PrintShipmantsBtn.Click += new System.EventHandler(this.PrintShipmantsBtn_Click);
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(208, 24);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(104, 23);
			this.button1.TabIndex = 2;
			this.button1.Text = "Generate Codes";
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(408, 213);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.button1,
																		  this.groupBox1,
																		  this.CloseBtn});
			this.Name = "Form1";
			this.Text = "Admin";
			this.groupBox1.ResumeLayout(false);
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

		private void PrintShipmantsBtn_Click(object sender, System.EventArgs e)
		{
			string strSQLall = "SELECT * FROM tblShipments where shipped = '0' and ProcessingStatus <> 'Awaiting Payment'";
			DataSet objDataSet3 = new DataSet();
			SqlConnection objConnection = new SqlConnection(strConnection);
			SqlDataAdapter objDataAdapter3 = new SqlDataAdapter(strSQLall, objConnection);
			int recordCount = objDataAdapter3.Fill(objDataSet3,"dataBlock");
			DataView objDataView3 = new DataView(objDataSet3.Tables["dataBlock"]);
			DataTable tblObj3 = objDataSet3.Tables["dataBlock"];
			Object tempVar;

            string [] addressArray = new string[recordCount];

			for(int loop=0;loop<recordCount;loop++)
			{
				DataRow rowLocal = tblObj3.Rows[loop];
				string build = "  " + rowLocal["Name"].ToString() + "\n" + "  " + rowLocal["ShipAddress"].ToString() + "\n";
				build += "  " + rowLocal["ShipCity"].ToString() + "," + rowLocal["ShipState"].ToString() + " " + rowLocal["ShipZip"].ToString();
			    addressArray[loop] = build;
			}
			
			LabelFunctions lblobj = new LabelFunctions();
			lblobj.PrintLabelArray(recordCount,addressArray,"Labels_Template2.dot",false,true);
		}
		public void ExecuteQuery(string querry)

		{

			string strSQLall = querry;

			DataSet objDataSet = new DataSet();

			SqlConnection objConnection = new SqlConnection(strConnection);

			SqlCommand myCommand = new SqlCommand(strSQLall,objConnection);

			myCommand.CommandTimeout = 15;

			myCommand.CommandType = CommandType.Text;

			myCommand.Connection.Open();

			SqlDataReader myReader = myCommand.ExecuteReader(CommandBehavior.CloseConnection);

			myReader.Close();

		}

		private void button1_Click(object sender, System.EventArgs e)
		{
			int amount = 50;
			string[] strArray = new string[amount];
			while(amount > 1)

			{
					ExecuteQuery("Insert Into tblSerials (Type, Active, UserID) Values('L',0,-1)");
					amount = amount -1;
			}
			
			string strSQLall = "SELECT * FROM tblSerials where Type= 'L'";
			DataSet objDataSet3 = new DataSet();
			SqlConnection objConnection = new SqlConnection(strConnection);
			SqlDataAdapter objDataAdapter3 = new SqlDataAdapter(strSQLall, objConnection);
			int recordCount = objDataAdapter3.Fill(objDataSet3,"dataBlock");
			DataTable tblObj3 = objDataSet3.Tables["dataBlock"];
			while(recordCount > 0)

			{
				DataRow rowLocal = tblObj3.Rows[recordCount-1];
				strArray[recordCount] = rowLocal["SerialNo"].ToString();
					recordCount = recordCount -1;

			}

			ExecuteQuery("Update tblSerials set Type='S' where Type='L'");
		}
	}
}

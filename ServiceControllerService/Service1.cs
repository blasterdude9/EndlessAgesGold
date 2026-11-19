using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.ServiceProcess;
using System.Data.SqlClient;

namespace ServiceControllerService
{
	public class ARBSVC : System.ServiceProcess.ServiceBase
	{
		private System.Windows.Forms.Timer TimerSys;
		private System.ComponentModel.IContainer components;
		public string strConnection = "server=YOUR_SQL_SERVER;database=YOUR_DB;uid=YOUR_SQL_USER;password=YOUR_SQL_PASSWORD;";

		public ARBSVC()
		{
			// This call is required by the Windows.Forms Component Designer.
			InitializeComponent();

			// TODO: Add any initialization after the InitComponent call
		}

		// The main entry point for the process
		static void Main()
		{
			System.ServiceProcess.ServiceBase[] ServicesToRun;
	
			// More than one user Service may run within the same process. To add
			// another service to this process, change the following line to
			// create a second service object. For example,
			//
			//   ServicesToRun = new System.ServiceProcess.ServiceBase[] {new Service1(), new MySecondUserService()};
			//
			ServicesToRun = new System.ServiceProcess.ServiceBase[] { new ARBSVC() };

			System.ServiceProcess.ServiceBase.Run(ServicesToRun);
		}

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this.TimerSys = new System.Windows.Forms.Timer(this.components);
			// 
			// TimerSys
			// 
			this.TimerSys.Interval = 15000;
			this.TimerSys.Tick += new System.EventHandler(this.timer1_Tick);
			// 
			// ARBSVC
			// 
			this.CanPauseAndContinue = true;
			this.ServiceName = "Arb Svc Controller";

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

		/// <summary>
		/// Set things in motion so your service can do its work.
		/// </summary>
		protected override void OnStart(string[] args)
		{
			TimerSys.Start();
		}
 
		/// <summary>
		/// Stop this service.
		/// </summary>
		protected override void OnStop()
		{
			TimerSys.Stop();
		}

		//service function
		bool ControlService(string svcName,int statusSet)
		{
			System.ServiceProcess.ServiceController tmpSC = new System.ServiceProcess.ServiceController();
			tmpSC.MachineName= System.Environment.MachineName;
			tmpSC.DisplayName=svcName;
			try
			{
				if(statusSet == 0)//attemp turn off
					if(tmpSC.Status==ServiceControllerStatus.Running )
					{
						tmpSC.Stop();
					}

				if(statusSet == 1)//attemp turn on
					if(tmpSC.Status!=ServiceControllerStatus.Running )
					{
						tmpSC.Start();
					}
			}
			catch
			{

			}
			return true;
		}

		private void callback()
		{
			try
			{
				SqlDataReader reader3 = sqlQuery("SELECT * FROM ARBSCV", false);
				while(reader3.Read())
				{
					string temp = reader3.GetSqlValue(2).ToString();
					ControlService(reader3.GetSqlValue(1).ToString(),Convert.ToInt32(temp));
				}
				reader3.Close();
			}
			catch
			{

			}
		}

		public SqlDataReader sqlQuery(String query , bool isStoredProc)
		{
		
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

		private void timer1_Tick(object sender, System.EventArgs e)
		{
		    callback();
		}
	}
}

using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.ServiceProcess;
using System.Data.SqlClient;
using System.Threading;

namespace ARBSVC
{
	public class ARBSCV : System.ServiceProcess.ServiceBase
	{
		private System.ComponentModel.IContainer components;
		public string strConnection = "server=YOUR_SQL_SERVER;database=YOUR_DB;uid=YOUR_SQL_USER;password=YOUR_SQL_PASSWORD;";
		System.Timers.Timer Timer1 = new System.Timers.Timer();
		

		private void CreateTimer()
		{
			
			Timer1.Enabled = true;
			Timer1.Interval = 10000;
			Timer1.Elapsed += new System.Timers.ElapsedEventHandler(Timer1_Elapsed);
		}



		public ARBSCV()
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
			ServicesToRun = new System.ServiceProcess.ServiceBase[] { new ARBSCV() };

			System.ServiceProcess.ServiceBase.Run(ServicesToRun);
		
			
		}

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			
			this.components = new System.ComponentModel.Container();
			// 
			// ARBSCV
			// 
			this.ServiceName = "ARBSVC";

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
			// TODO: Add code here to start your service.
			//TimerObj.Start();
			CreateTimer();
		}

		/*protected override void OnIdle(string[] args)
		{
			// TODO: Add code here to start your service.
			//TimerObj.Start();
			
		}*/

		/// <summary>
		/// Stop this service.
		/// </summary>
		protected override void OnStop()
		{
			// TODO: Add code here to perform any tear-down necessary to stop your service.
			Timer1.Enabled = false;
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

		private void callback()
		{
			try
			{
				SqlConnection myConnection = new SqlConnection(strConnection);
				SqlCommand myCommand = new SqlCommand("SELECT * FROM ARBSCV");
				myCommand.Connection = myConnection;
				myConnection.Open();
				SqlDataReader reader = myCommand.ExecuteReader();

				while(reader.Read())
				{
					string temp = reader.GetSqlValue(2).ToString();
					ControlService(reader.GetSqlValue(1).ToString(),Convert.ToInt32(temp));
				}
				reader.Close();
				myCommand.Dispose();
				myConnection.Close();
				myConnection.Dispose();
			}
			catch
			{

			}
		}

		private void Timer1_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
		{
		    callback();
		}

		//service function
		bool ControlService(string svcName,int statusSet)
		{
			try
			{
				System.ServiceProcess.ServiceController tmpSC = new System.ServiceProcess.ServiceController();
				tmpSC.MachineName= System.Environment.MachineName;
				tmpSC.DisplayName= svcName;

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

				tmpSC.Close();
				tmpSC.Dispose();
			}
			catch
			{

			}


			return true;
		}
	}
}

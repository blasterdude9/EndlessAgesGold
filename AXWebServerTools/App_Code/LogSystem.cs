using System;
using System.Data.SqlClient;

	/// <summary>
	/// Summary description for LogSystem.
	/// </summary>
	/// 

	public class LogSystem
	{
		public LogSystem()
		{
			//
			// TODO: Add constructor logic here
			//
		}

		DatabaseConnectionModule dataCon = new DatabaseConnectionModule();

		public void LogEvent(string eventDesc, string eventInfo,SqlConnection myConnection)
		{
            eventDesc = dataCon.SanitizeIn(eventDesc);
			eventInfo = dataCon.SanitizeIn(eventInfo);


			bool createdConnection = false;
			if(myConnection == null)
			{
				myConnection = dataCon.CreateNativeConnectionObject();
				createdConnection = true;
			}
			dataCon.ExecuteQuery("INSERT INTO gm_LOGmisc(eventname,data) Values('"+eventDesc+"','"+eventInfo+"')",myConnection);

			if(createdConnection == true)
				myConnection.Close();
		}

	}

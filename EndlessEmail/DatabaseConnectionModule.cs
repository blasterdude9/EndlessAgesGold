using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;

namespace EndlessAdmin
{
	/// <summary>
	/// Summary description for DatabaseConnectionModule.
	/// </summary>
	public class DatabaseConnectionModule
	{
		public string conStr = "server=YOUR_SQL_SERVER;database=AXManagementHub;uid=YOUR_SQL_USER;password=YOUR_SQL_PASSWORD;";
        //-------------------------------------------//
		//-------------------------------------------//
		public DatabaseConnectionModule()
		{
			//
			// TODO: Add constructor logic here
			//
		}
		//--------------------------------------------//
		//-----------executes single------------------//
		//----------querry then close-----------------//
		public void ExecuteQuery(string querry)
		{
			string strSQLall = querry;
			DataSet objDataSet = new DataSet();
			SqlConnection objConnection = new SqlConnection(conStr);
			SqlCommand myCommand = new SqlCommand(strSQLall,objConnection);
			myCommand.CommandTimeout = 15;
			myCommand.CommandType = CommandType.Text;
			myCommand.Connection.Open();
			SqlDataReader myReader = myCommand.ExecuteReader(CommandBehavior.CloseConnection);
			//myReader.Read();
			myReader.Close();
			objConnection.Close();
		}
		//--------------------------------------------//
		//-----------executes single------------------//
		//----------querry then close-----------------//
		public void ExecuteQuery(string querry, SqlConnection myConnection)
		{
			string strSQLall = querry;
			DataSet objDataSet = new DataSet();
			SqlCommand myCommand = new SqlCommand(strSQLall,myConnection);
			myCommand.CommandTimeout = 15;
			myCommand.CommandType = CommandType.Text;
			
			SqlDataReader myReader = myCommand.ExecuteReader(CommandBehavior.Default);
			myReader.Close();
		}
		//--------------------------------------------//
		//-----------returns datareader --------------//
		//----------from querry results---------------//
		public SqlDataReader sqlQuery(String query, bool isStoredProc)
		{
			SqlConnection myConnection = new SqlConnection(conStr);
			SqlCommand myCommand = new SqlCommand(query);
			myCommand.CommandTimeout = 15;
			if (isStoredProc == true)
				myCommand.CommandType = CommandType.StoredProcedure;
			myCommand.Connection = myConnection;
			myConnection.Open();
			SqlDataReader reader = myCommand.ExecuteReader();
			return reader;
		}
		//--------------------------------------------//
		//-----------returns datareader --------------//
		//----------from querry results---------------//
		public SqlDataReader sqlQuery(String query, string extrenalConnectionStr)
		{
			SqlConnection myConnection = new SqlConnection(extrenalConnectionStr);
			SqlCommand myCommand = new SqlCommand(query);
			myCommand.CommandTimeout = 15;
			myCommand.Connection = myConnection;
			SqlDataReader reader = myCommand.ExecuteReader();
			return reader;
		}
		//--------------------------------------------//
		//-----------returns datareader --------------//
		//----------from querry results---------------//
		public SqlDataReader sqlQuery(String query, SqlConnection myConnection)
		{
			SqlCommand myCommand = new SqlCommand(query);
			myCommand.CommandTimeout = 15;
			myCommand.Connection = myConnection;
			SqlDataReader reader = myCommand.ExecuteReader();
			return reader;
		}
		//--------------------------------------------//
		//-----------returns datareader --------------//
		//----------from querry results---------------//
		public SqlConnection CreateConnectionObject(string connectionString)
		{
			SqlConnection myConnection = new SqlConnection(connectionString);
			myConnection.Open();
			return myConnection;
		}
		//--------------------------------------------//
		//-----------returns datareader --------------//
		//----------from querry results---------------//
		public SqlConnection CreateNativeConnectionObject()
		{
			SqlConnection myConnection = new SqlConnection(conStr);
			myConnection.Open();
		
			
			return myConnection;
		}
        //--------------------------------------------//
		//-----------verify one record exists---------//
		//--------------------------------------------//
		public bool VerifyOneCount(String query, bool isStoredProc)
		{
			SqlConnection myConnection = new SqlConnection(conStr);
			SqlCommand myCommand = new SqlCommand(query);
			myCommand.CommandTimeout = 15;
			if (isStoredProc == true)
				myCommand.CommandType = CommandType.StoredProcedure;
			myCommand.Connection = myConnection;
			myConnection.Open();
			SqlDataReader reader = myCommand.ExecuteReader();
			if(reader.Read())
			{
				reader.Close();
				myConnection.Close();
				return true;
			}
			reader.Close();
			myConnection.Close();
			return false;
		}
	}
}

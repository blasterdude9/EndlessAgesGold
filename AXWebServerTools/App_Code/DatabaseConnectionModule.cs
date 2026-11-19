using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;

	/// <summary>
	/// Summary description for DatabaseConnectionModule.
	/// </summary>
	public class DatabaseConnectionModule
	{

        // Connection strings now live in Web.Config (<connectionStrings>), not in code.
        public string conStr  = System.Configuration.ConfigurationManager.ConnectionStrings["GameDB"].ConnectionString;
        public string authStr = System.Configuration.ConfigurationManager.ConnectionStrings["AuthDB"].ConnectionString;
        //-------------------------------------------//
		//-------------------------------------------//
        public DatabaseConnectionModule()
        {
        }

		public DatabaseConnectionModule(string connectionOveride)
		{
			//
			// TODO: Add constructor logic here
			//
		}
		//--------------------------------------------//
		//-----------executes single------------------//
		//----------querry then close-----------------//
		public int ExecuteQuery(string querry)
		{
			string strSQLall = querry;
			DataSet objDataSet = new DataSet();
			SqlConnection objConnection = new SqlConnection(conStr);
			SqlCommand myCommand = new SqlCommand(strSQLall,objConnection);
			myCommand.CommandTimeout = 60;
			myCommand.CommandType = CommandType.Text;
			myCommand.Connection.Open();
			SqlDataReader myReader = myCommand.ExecuteReader(CommandBehavior.CloseConnection);
            int recordsAffected = myReader.RecordsAffected;
			//myReader.Read();
			myReader.Close();
			objConnection.Close();
            return recordsAffected;
		}
		//--------------------------------------------//
		//-----------executes single------------------//
		//----------querry then close-----------------//
		public int ExecuteQuery(string querry, SqlConnection myConnection)
		{
			string strSQLall = querry;
			DataSet objDataSet = new DataSet();
			SqlCommand myCommand = new SqlCommand(strSQLall,myConnection);
			myCommand.CommandTimeout = 60;
			myCommand.CommandType = CommandType.Text;
			
			SqlDataReader myReader = myCommand.ExecuteReader(CommandBehavior.Default);
            int recordsAffected = myReader.RecordsAffected;
			myReader.Close();
            return recordsAffected;
		}
		//--------------------------------------------//
		//-----------returns datareader --------------//
		//----------from querry results---------------//
		public SqlDataReader sqlQuery(String query, bool isStoredProc)
		{
			SqlConnection myConnection = new SqlConnection(conStr);
			SqlCommand myCommand = new SqlCommand(query);
			myCommand.CommandTimeout = 60;
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
			myCommand.CommandTimeout = 60;
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
			myCommand.CommandTimeout = 60;
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
		public bool VerifyOneCount(String query, SqlConnection myConnection)
		{
			SqlDataReader reader = sqlQuery(query, myConnection);
			if(reader.Read())
			{
                reader.Close();
				return true;
			}
			reader.Close();
			return false;
		}
		//--------------------------------------------//
		public string SanitizeOutHtml(string input)
		{
			char quote = '"';
			input = input.Replace(quote.ToString(),"'");
			input = input.Replace("<","[");
			input = input.Replace(">","]");

			return input;
		}
		//--------------------------------------------//
		public string SanitizeIn(string input)
		{
			char quote = '"';
			input = input.Replace("'",quote.ToString());
			input = input.Replace("<","[");
			input = input.Replace(">","]");

			return input;
		}
		//--------------------------------------------//
		public string SanitizeInBasic(string input)
		{
			char quote = '"';
			input = input.Replace("'",quote.ToString());
			return input;
		}
		//--------------------------------------------//
		public string SanitizeOut(string input)
		{
			char quote = '"';
			input = input.Replace(quote.ToString(),"'");
			
			return input;
		}
	}

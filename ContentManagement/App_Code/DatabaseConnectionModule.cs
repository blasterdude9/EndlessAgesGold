using System;
using System.Configuration;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;

    public class DatabaseConnectionModule
    {
        

        // Connection string now lives in Web.config (<connectionStrings>), not in code.
        public string conStr = System.Configuration.ConfigurationManager.ConnectionStrings["GameDB"].ConnectionString;
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
            SqlCommand myCommand = new SqlCommand(strSQLall, objConnection);
            myCommand.CommandTimeout = 15;
            myCommand.CommandType = CommandType.Text;
            myCommand.Connection.Open();
            try
            {
                SqlDataReader myReader = myCommand.ExecuteReader(CommandBehavior.CloseConnection);
                myReader.Close();
            }
            catch (Exception ex)
            {

            }

            objConnection.Close();
        }
        //--------------------------------------------//
        //-----------executes single------------------//
        //----------querry then close-----------------//
        public void ExecuteQuery(string querry, SqlConnection myConnection)
        {
            string strSQLall = querry;
            DataSet objDataSet = new DataSet();
            SqlCommand myCommand = new SqlCommand(strSQLall, myConnection);
            myCommand.CommandTimeout = 15;
            myCommand.CommandType = CommandType.Text;
            try
            {
                SqlDataReader myReader = myCommand.ExecuteReader(CommandBehavior.Default);
                myReader.Close();
            }
            catch (Exception ex)
            {

            }
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
        //-----------returns datareader --------------//
        //----------from querry results---------------//
        public SqlConnection CreateConnectionObject(string serverIP,string database, string uid, string password)
        {
            //specific connection
            string conStrLoc = "server=" + serverIP +
                                    ";database=" + database +
                                    ";uid=" + uid +
                                    ";password=" + password + ";";
            SqlConnection myConnection = new SqlConnection(conStrLoc);
            myConnection.Open();


            return myConnection;
        }
        //--------------------------------------------//
        //-----------verify one record exists---------//
        //--------------------------------------------//
        public bool VerifyOneCount(String query, SqlConnection myConnection)
        {
            SqlDataReader reader = sqlQuery(query, myConnection);
            if (reader.Read())
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
            input = input.Replace(quote.ToString(), "'");
            input = input.Replace("<", "[");
            input = input.Replace(">", "]");

            return input;
        }
        //--------------------------------------------//
        public string SanitizeIn(string input)
        {
            char quote = '"';
            input = input.Replace("'", quote.ToString());
            input = input.Replace("<", "[");
            input = input.Replace(">", "]");

            return input;
        }
        //--------------------------------------------//
        public string SanitizeInBasic(string input)
        {
            char quote = '"';
            input = input.Replace("'", quote.ToString());
            return input;
        }
        //--------------------------------------------//
        public string SanitizeOut(string input)
        {
            char quote = '"';
            input = input.Replace(quote.ToString(), "'");

            return input;
        }
    }

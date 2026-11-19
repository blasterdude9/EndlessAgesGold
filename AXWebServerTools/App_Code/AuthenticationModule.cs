using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Web;
using System.Web.SessionState;

	/// <summary>
	/// Summary description for AuthenticationModule.
	/// </summary>
	public class AuthenticationModule : System.Web.UI.Page
	{
        DatabaseConnectionModule dataCon = new DatabaseConnectionModule();
		bool m_authenticationModeOn = true;
		LogSystem m_logReporter = new LogSystem();

		public AuthenticationModule()
		{
			//
			// TODO: Add constructor logic here
			//
		}

		public string TestSan(string input)
		{
			if(input == null)
				return "";
			input = input.Replace("'","");
			input = input.Replace(",","");
			return input;
		}

		public bool ValidateUserSVC(string username, string password,SqlConnection myConnection)
		{
			//m_logReporter.LogEvent("AX Attempting Logon:",username,myConnection);
			username = TestSan(username);
			password = TestSan(password);
			string sqlValidate = "SELECT * FROM tblAdministration where username = '" + username + "' and password = '" + password + "'";
			return dataCon.VerifyOneCount(sqlValidate,myConnection);

			
		}

		public bool ValidateUserUniPlat(string username, string sectionAccess, SqlConnection myConnection)
		{
			if(username == null)
			   return false;

			//m_logReporter.LogEvent("AX Attempting Logon:",username,myConnection);
			username = TestSan(username);
			sectionAccess = TestSan(sectionAccess);

            string sqlValidate = "SELECT * FROM gmAccountBase INNER JOIN tblUserPerms ON gmAccountBase.Username = tblUserPerms.username WHERE tblUserPerms.username = '"+username+"' AND tblUserPerms.permissionName = '"+sectionAccess+"'";
			
			
			return dataCon.VerifyOneCount(sqlValidate,myConnection);
		}

		public bool AuthenticateUser(string redirectUrl,string sectionAccess)
		{
            if(!m_authenticationModeOn)
				return true;

			bool createdConnection = false;
			//if(myConnection == null)
			//{
            SqlConnection myConnection = new SqlConnection(dataCon.authStr);
            myConnection.Open();
            createdConnection = true;
			//}

			bool allow = false;
            if(Session["user"] != null)	
			   if(ValidateUserUniPlat(Session["user"].ToString(),sectionAccess, myConnection))
			      allow = true;
			
			if(allow == false)
			{
				if(Session["user"] != null)	
				   m_logReporter.LogEvent("Rejected Access:",Session["user"].ToString(),myConnection);
				myConnection.Close();
				Session["redirect"] = redirectUrl;
				Server.Transfer("SupportAuthentication.aspx");
				return false;
			}

			

            if(createdConnection == true)
			   myConnection.Close();
			
			return true;
		}

		public string ValidateUserSVCRetEmail(string username, string password)
		{
			username = TestSan(username);
			password = TestSan(password);
			string sqlValidate = "SELECT emailRef FROM tblAdministration where username = '" + username + "' and password = '" + password + "'";
			SqlDataReader reader3 = dataCon.sqlQuery(sqlValidate, false);
			while(reader3.Read())
			{//list
				string temp = reader3.GetSqlValue(0).ToString();
				reader3.Close();
				return temp;
			}//end list
			reader3.Close();
			return "NONE";
		}
	}

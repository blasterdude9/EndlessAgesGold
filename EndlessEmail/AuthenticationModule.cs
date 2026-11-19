using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Web;
using System.Web.SessionState;

namespace EndlessAdmin
{
	/// <summary>
	/// Summary description for AuthenticationModule.
	/// </summary>
	public class AuthenticationModule : System.Web.UI.Page
	{
        EndlessAdmin.DatabaseConnectionModule dataCon = new EndlessAdmin.DatabaseConnectionModule();
		bool m_authenticationModeOn = true;

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
			input = input.Replace("-","");
			input = input.Replace(",","");
			return input;
		}

		public bool ValidateUserSVC(string username, string password)
		{
			username = TestSan(username);
			password = TestSan(password);
			string sqlValidate = "SELECT * FROM tblAdministration where username = '" + username + "' and password = '" + password + "'";
			return dataCon.VerifyOneCount(sqlValidate,false);
		}

		public bool AuthenticateUser(string redirectUrl)
		{
            if(!m_authenticationModeOn)
				return true;

            string username = (string)Session["user"];
			string password = (string)Session["pass"];
			username = TestSan(username);
			password = TestSan(password);

			if(!ValidateUserSVC(username,password))
			{
                Session["redirect"] = redirectUrl;
				Server.Transfer("SupportAuthentication.aspx");
				return false;
			}
			
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
}

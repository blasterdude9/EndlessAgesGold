using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Web;
using System.Web.SessionState;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.HtmlControls;
using System.Data.SqlClient;

	public partial class ContentManager : System.Web.UI.Page
	{
        DatabaseConnectionModule dbConn = new DatabaseConnectionModule();
        public static AXencrypt ax_encryptionManager = new AXencrypt();
        ConnectionHelper conHelper = new ConnectionHelper();

		protected void Page_Load(object sender, System.EventArgs e)
		{
			if(Request.QueryString["contenttype"] != null)
			{
				SqlConnection sqlConnect = dbConn.CreateNativeConnectionObject();
                
				string gameType = "";
				if(Request.QueryString["gametype"] != null)
					gameType = dbConn.SanitizeIn(Request.QueryString["gametype"].ToString());
				
				string contentType = dbConn.SanitizeIn(Request.QueryString["contenttype"].ToString());
				SqlDataReader cReader = dbConn.sqlQuery("SELECT filename, filesize, cabname, offsetfolder, filecrc, Verify, cabCRC FROM GMCONTENT WHERE CONTENTTYPE = '"+contentType+"' OR CONTENTTYPE = '"+gameType+"'",sqlConnect);
				while(cReader.Read())
				{
					Response.Write(cReader.GetSqlValue(0).ToString());
					Response.Write("\r\n");
					Response.Write(cReader.GetSqlValue(1).ToString());
					Response.Write("\r\n");
					Response.Write(cReader.GetSqlValue(2).ToString());
					Response.Write("\r\n");
					Response.Write(cReader.GetSqlValue(3).ToString());
					Response.Write("\r\n");
					Response.Write(cReader.GetSqlValue(4).ToString());
					Response.Write("\r\n");
					Response.Write(cReader.GetSqlValue(5).ToString());
					Response.Write("\r\n");
					Response.Write(cReader.GetSqlValue(6).ToString());
					Response.Write("\r\n");
				}
				cReader.Close();
				sqlConnect.Close();
			}
			else if(Request.QueryString["gameapp"] != null)
			{//game client download
				SqlConnection sqlConnect = dbConn.CreateNativeConnectionObject();
				SqlDataReader cReader = dbConn.sqlQuery("SELECT ipaddress FROM SVR_IPLIST WHERE TYPE  = 'GAMEAPPLICATION'",sqlConnect);
				if(cReader.Read())
				{
						Response.Write(cReader.GetSqlValue(0).ToString());
				}
				cReader.Close();
				sqlConnect.Close();
			}
			else if(Request.QueryString["installer_patcher"] != null)
			{//game client download
				string cabNameLoc = Request.QueryString["installer_patcher"].ToString()+"INSTALL";
				SqlConnection sqlConnect = dbConn.CreateNativeConnectionObject();
				SqlDataReader cReader = dbConn.sqlQuery("SELECT ipaddress FROM SVR_IPLIST WHERE GAMENAME  = '"+cabNameLoc+"'",sqlConnect);
				if(cReader.Read())
				{
					Response.Write(cReader.GetSqlValue(0).ToString());
					Response.Write("\r\n");
					Response.Write(cabNameLoc + ".cab");
				}
				cReader.Close();
				sqlConnect.Close();
			}
            else if (Request.QueryString["server_list"] != null)
            {//server list
                SqlConnection sqlConnect = dbConn.CreateNativeConnectionObject();
                SqlDataReader cReader = null;

                string strAccessType = "external";
                if (Request.QueryString["access_type"] != null)
                    strAccessType = Request.QueryString["access_type"].ToString();

                if (strAccessType.Equals("all")) 
                    cReader = dbConn.sqlQuery("SELECT ipaddress,decoratedName,hubname FROM SVR_IPLIST WHERE VISIBLE ='1' AND GAMENAME='" + Request.QueryString["server_list"].ToString()+ "'", sqlConnect);
                else if (strAccessType.Equals("internal"))
                    cReader = dbConn.sqlQuery("SELECT ipaddress,decoratedName,hubname FROM SVR_IPLIST WHERE VISIBLE ='1' AND GAMENAME='" + Request.QueryString["server_list"].ToString() + "' AND IPADDRESS LIKE '10.0.%'", sqlConnect);
                else /*if (strAccessType == "external")*/
                    cReader = dbConn.sqlQuery("SELECT ipaddress,decoratedName,hubname FROM SVR_IPLIST WHERE VISIBLE ='1' AND GAMENAME='" + Request.QueryString["server_list"].ToString() + "' AND IPADDRESS NOT LIKE '10.0.%'", sqlConnect);

                while (cReader.Read())
                {//get hub link from clusters, then aquire HUB
                    string hubServer = cReader.GetSqlValue(2).ToString();
                    if (hubServer.Length < 2)
                    {
                        //decorated name
                        Response.Write(cReader.GetSqlValue(1).ToString());
                        Response.Write("\r\n");
                        //hub load ip
                        Response.Write(cReader.GetSqlValue(0).ToString());
                        Response.Write("\r\n");
                    }
                    else
                    {
                        SqlConnection hubConnection = conHelper.GetServerHUBConnection(hubServer);
                        if (hubConnection != null)
                        {
                            //lowest connection in cluster ip address
                            SqlDataReader hubReader = dbConn.sqlQuery("SELECT NODEIP FROM SYSLIVESERVERNODES WHERE DATEADD(SECOND,25,CHECKINTIME) > GETDATE() ORDER BY CONNECTIONCOUNT", hubConnection);
                            if (hubReader.Read())
                            {
                                //decorated name
                                Response.Write(cReader.GetSqlValue(1).ToString());
                                Response.Write("\r\n");
                                //hub load ip
                                Response.Write(hubReader.GetSqlValue(0).ToString());
                                Response.Write("\r\n");
                            }
                            hubReader.Close();
                            hubConnection.Close();
                        }
                    }
                }
                cReader.Close();
                sqlConnect.Close();
            }
            else if (Request.QueryString["server_x"] != null)
            {//server list
                SqlConnection sqlConnect = dbConn.CreateNativeConnectionObject();
                SqlDataReader cReader = null;

                //string strAccessType = "external";
                //if (Request.QueryString["access_type"] != null)
                //    strAccessType = Request.QueryString["access_type"].ToString();

                //if (strAccessType.Equals("all"))
                cReader = dbConn.sqlQuery("SELECT ipaddress,decoratedName,hubname,gamename FROM SVR_IPLIST WHERE VISIBLE ='1'", sqlConnect);
                //else if (strAccessType.Equals("internal"))
                //      cReader = dbConn.sqlQuery("SELECT ipaddress,decoratedName,hubname,gamename FROM SVR_IPLIST WHERE VISIBLE ='1' AND IPADDRESS LIKE '10.0.%'", sqlConnect);
                //else /*if (strAccessType == "external")*/
                //      cReader = dbConn.sqlQuery("SELECT ipaddress,decoratedName,hubname,gamename FROM SVR_IPLIST WHERE VISIBLE ='1' AND IPADDRESS NOT LIKE '10.0.%'", sqlConnect);

                while (cReader.Read())
                {//get hub link from clusters, then aquire HUB
                    string hubServer = cReader.GetSqlValue(2).ToString();
                    if (hubServer.Length < 2)
                    {
                        //decorated name
                        /*Response.Write(cReader.GetSqlValue(1).ToString());
                        Response.Write("\r\n");
                        //hub load ip
                        Response.Write(cReader.GetSqlValue(0).ToString());
                        Response.Write("\r\n");
                        //gameName
                        Response.Write(cReader.GetSqlValue(3).ToString());
                        Response.Write("\r\n");*/
                    }
                    else
                    {
                        SqlConnection hubConnection = conHelper.GetServerHUBConnection(hubServer);
                        if (hubConnection != null)
                        {
                            //lowest connection in cluster ip address
                            SqlDataReader hubReader = dbConn.sqlQuery("SELECT NODEIP FROM SYSLIVESERVERNODES WHERE DATEADD(SECOND,25,CHECKINTIME) > GETDATE() ORDER BY CONNECTIONCOUNT", hubConnection);
                            if (hubReader.Read())
                            {
                                //decorated name
                                Response.Write(cReader.GetSqlValue(1).ToString());
                                Response.Write("\r\n");
                                //hub load ip
                                Response.Write(hubReader.GetSqlValue(0).ToString());
                                Response.Write("\r\n");
                                //gameName
                                Response.Write(cReader.GetSqlValue(3).ToString());
                                Response.Write("\r\n");
                            }
                            hubReader.Close();
                            hubConnection.Close();
                        }
                    }
                }
                cReader.Close();
                sqlConnect.Close();
            }
			else if(Request.QueryString["tcpserver"] != null)
			{
				SqlConnection sqlConnect = dbConn.CreateNativeConnectionObject();
				SqlDataReader cReader = dbConn.sqlQuery("SELECT tcpIP,port FROM GMTCPSERVERS WHERE ACTIVE = '1'",sqlConnect);
				if(cReader.Read())
				{
					if(Request.QueryString["tcpserver"].ToString() == "1")
					{
						Response.Write(cReader.GetSqlValue(0).ToString());
						Response.Write("\r\n");
					}
					else
					{
						Response.Write(cReader.GetSqlValue(1).ToString());
						Response.Write("\r\n");
					}
				}
				cReader.Close();
				sqlConnect.Close();
			}
			else if(Request.QueryString["gameserver"] != null && Request.QueryString["game"] != null)
			{
				SqlConnection sqlConnect = dbConn.CreateNativeConnectionObject();
				SqlDataReader cReader = dbConn.sqlQuery("SELECT ipaddress FROM SVR_IPLIST WHERE type = 'gameserverip' and gamename = '"+Request.QueryString["game"].ToString()+"'",sqlConnect);
				if(cReader.Read())
				{
						Response.Write(cReader.GetSqlValue(0).ToString());
				}
				cReader.Close();
				sqlConnect.Close();
			}
			else if(Request.QueryString["music"] != null)
			{
				if(Request.QueryString["desc"] == null)
				{//send channel redirect
					SqlConnection sqlConnect = dbConn.CreateNativeConnectionObject();
					SqlDataReader cReader = dbConn.sqlQuery("SELECT stationlink FROM GMMUSICSTATIONS WHERE KEYSTRING = '"+Request.QueryString["music"].ToString()+"'",sqlConnect);
					if(cReader.Read())
					{
						string stationUrl = cReader.GetSqlValue(0).ToString();
						cReader.Close();
						sqlConnect.Close();
						Response.Redirect(stationUrl);
					}
					cReader.Close();
					sqlConnect.Close();
				}
				else
				{//send description
					SqlConnection sqlConnect = dbConn.CreateNativeConnectionObject();
					SqlDataReader cReader = dbConn.sqlQuery("SELECT stationdesc FROM GMMUSICSTATIONS WHERE KEYSTRING = '"+Request.QueryString["music"].ToString()+"'",sqlConnect);
					if(cReader.Read())
					{
						string stationUrl = cReader.GetSqlValue(0).ToString();
						cReader.Close();
						sqlConnect.Close();
						Response.Write(stationUrl);
					}
					cReader.Close();
					sqlConnect.Close();
				}
			}
			else if(Request.QueryString["fontpallet"] != null)
			{
				Response.Redirect("ContentFiles/Test.ETF");
			}
			else if(Request.QueryString["fonttexture"] != null)
			{
				Response.Redirect("ContentFiles/256Fonttest4.jpg");
			}
			else if(Request.QueryString["adv"] != null)
			{
				SqlConnection sqlConnect = dbConn.CreateNativeConnectionObject();
				Advertiser bannerSys = new Advertiser();
				string url = bannerSys.GetBanner(Request.QueryString["adv"].ToString(),sqlConnect);
				sqlConnect.Close();
				Response.Redirect(url);
			}
			else if(Request.QueryString["svrOnlineStatus"] != null)
			{//get server online status by tblName
				ConnectionHelper conHelper = new ConnectionHelper();
				SqlConnection sqlConn = conHelper.GetServerConnection(Request.QueryString["svrOnlineStatus"].ToString());
				if(sqlConn != null)
				{//server connection object aquired
					string sqlQry = "SELECT * FROM TBLMAINSERVERSTATUS WHERE DATEDIFF(s,LASTUPDATED,GETDATE()) < 15";
					SqlDataReader onlineReader = dbConn.sqlQuery(sqlQry,sqlConn);
					if(onlineReader.Read())
					{//online
						onlineReader.Close();
						Response.Write("Online");
					}
					else
					{//offline
						onlineReader.Close();
						Response.Write("Temporarily Offline");
					}
					sqlConn.Close();
				}//end server connection object aquired
				else
				{//cannot fine server
					Response.Write("Server not found");
				}//end cannot fine server
			}//end get server online status by tblName
			else if(Request.QueryString["kingdomOwner"] != null)
			{//get server kingdom owner
				if(Request.QueryString["svr"] == null)
					return;
				ConnectionHelper conHelper = new ConnectionHelper();
				SqlConnection sqlConn = conHelper.GetServerConnection(Request.QueryString["svr"].ToString());
				if(sqlConn != null)
				{//server connection object aquired
					string sqlQry = "SELECT STATUSVALUE FROM TBLMISCSTATUS WHERE STATUSITEM = '"+Request.QueryString["kingdomOwner"].ToString()+"'";
					SqlDataReader onlineReader = dbConn.sqlQuery(sqlQry,sqlConn);
					if(onlineReader.Read())
					{//online
						Response.Write(onlineReader.GetSqlValue(0).ToString());
						onlineReader.Close();
					}
					else
					{//offline
						onlineReader.Close();
						Response.Write("Cannot find Key");
					}
					sqlConn.Close();
				}//end server connection object aquired
				else
				{//cannot fine server
					Response.Write("Server not found");
				}//end cannot fine server
			}//end get server kingdom owner
            else if (Request.QueryString["setcurgame"] != null)
            {//get server kingdom owner
                if (Request.QueryString["user"] == null)
                    return;
                string username = Request.QueryString["user"].ToString();

                if (Request.QueryString["psw"] == null)
                    return;
                string password = Request.QueryString["psw"].ToString();

                if (Request.QueryString["game"] == null)
                    return;
                string gameName = Request.QueryString["game"].ToString();
                //decrypt

                username = ax_encryptionManager.Decrypt(username);
                password = ax_encryptionManager.Decrypt(password);

                //build query
                SqlConnection sqlConn = dbConn.CreateNativeConnectionObject();
                string sqlQry = "UPDATE GMACCOUNTBASE SET INGAMENAME = '$GAME' WHERE USERNAME = '$USER' AND PASSWORD = '$PASS'";
                //broadcast update accross game manager servers
                //note does not need to pend que if server is down, because this is always done before playing
                //exec current
                string tempQry = sqlQry;
                tempQry = tempQry.Replace("$GAME", gameName);
                tempQry = tempQry.Replace("$USER", username);
                tempQry = tempQry.Replace("$PASS", password);
                dbConn.ExecuteQuery(tempQry, sqlConn);
                //exec on dist
                string loopQry = "SELECT HUBIP,USERNAME,PASSWORD,DBNAME FROM TBLSQLHUBS WHERE ACTIVE=1";
                SqlDataReader distReader = dbConn.sqlQuery(loopQry, sqlConn);
                if (distReader.Read())
                {//loop through distribution list
                    SqlConnection sqlConnInside = dbConn.CreateConnectionObject(distReader.GetSqlValue(0).ToString(),//string serverIP,
                                                                                distReader.GetSqlValue(3).ToString(),//string database, 
                                                                                distReader.GetSqlValue(1).ToString(),//string uid, 
                                                                                distReader.GetSqlValue(2).ToString()//string password
                                                                                );
                    dbConn.ExecuteQuery(tempQry, sqlConnInside);

                    sqlConnInside.Close();
                }//end loop through distribution list
                distReader.Close();
                sqlConn.Close();
            }//end get server kingdom owner
            else
                Response.Write("UNKNOWN");
		}

		#region Web Form Designer generated code
		override protected void OnInit(EventArgs e)
		{
			//
			// CODEGEN: This call is required by the ASP.NET Web Form Designer.
			//
			InitializeComponent();
			base.OnInit(e);
		}
		
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{    
		}
		#endregion
	}

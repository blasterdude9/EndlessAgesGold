using System;
using System.ComponentModel;
using System.Collections;
using System.Diagnostics;
using System.Data.SqlClient;

	/// <summary>
	/// Summary description for ConnectionHelper.
	/// </summary>
	public class ConnectionHelper : System.ComponentModel.Component
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		DatabaseConnectionModule dbConModule = new DatabaseConnectionModule();



		public ConnectionHelper(System.ComponentModel.IContainer container)
		{
			///
			/// Required for Windows.Forms Class Composition Designer support
			///
			container.Add(this);
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		public string TestSan(string input)
		{
			if(input == null)
				return "";
			input = input.Replace("'","");

			return input;
		}

		public SqlConnection GetServerConnection(string serverName)
		{
			SqlConnection mainConnection = dbConModule.CreateNativeConnectionObject();
            SqlDataReader sqlReader = dbConModule.sqlQuery("Select serverManageIP,ServerUsername,ServerPass,DatabaseName from tblServers where servername ='"+serverName+"'",mainConnection);
			if(sqlReader.Read())
			{
				//connect to servers database
				string conStrLoc =	"server="+sqlReader.GetSqlValue(0).ToString()+
					";database="+sqlReader.GetSqlValue(3).ToString()+
					";uid="+sqlReader.GetSqlValue(1).ToString()+
					";password="+sqlReader.GetSqlValue(2).ToString()+";";
				sqlReader.Close();
				mainConnection.Close();

				SqlConnection sqlAXServerConnection = dbConModule.CreateConnectionObject(conStrLoc);
				if(sqlAXServerConnection == null)
				{
					return null;
				}
				return sqlAXServerConnection;
			}
			else
			   sqlReader.Close();

			mainConnection.Close();
			return null;
		}

        public SqlConnection GetServerHUBConnection(string serverName)
        {
            SqlConnection mainConnection = dbConModule.CreateNativeConnectionObject();
            SqlDataReader sqlReader = dbConModule.sqlQuery("Select HUBSqlIP,HUBUsername,HUBPassword,HUBDBName from tblServers where servername ='" + serverName + "'", mainConnection);
            if (sqlReader.Read())
            {
                //connect to servers database
                string conStrLoc = "server=" + sqlReader.GetSqlValue(0).ToString() +
                    ";database=" + sqlReader.GetSqlValue(3).ToString() +
                    ";uid=" + sqlReader.GetSqlValue(1).ToString() +
                    ";password=" + sqlReader.GetSqlValue(2).ToString() + ";";
                sqlReader.Close();
                mainConnection.Close();

                SqlConnection sqlAXServerConnection = dbConModule.CreateConnectionObject(conStrLoc);
                if (sqlAXServerConnection == null)
                {
                    return null;
                }
                return sqlAXServerConnection;
            }
            else
                sqlReader.Close();

            mainConnection.Close();
            return null;
        }

		public bool SendCommandToServer(            string serverName,
			                                        string commandType,
			                                        string peram1,
													string peram2,
													string peram3,
													string peram4,
													string peram5,
													string peram6,
			                                        SqlConnection connectionToServerDB)
		{

			peram1 = TestSan(peram1);
			peram2 = TestSan(peram2);
			peram3 = TestSan(peram3);
			peram4 = TestSan(peram4);
			peram5 = TestSan(peram5);
			peram6 = TestSan(peram6);
			
			dbConModule.ExecuteQuery("insert into tblCommandsToServer (CommandType,peram1,peram2,peram3,peram4,peram5,peram6) Values('"+commandType+
                                      "','"+peram1+
                                      "','"+peram2+
									  "','"+peram3+
									  "','"+peram4+
									  "','"+peram5+
									  "','"+peram6+
                                      "')",
				                      connectionToServerDB);

            //connectionToServerDB.Close();
			return true;
		}

		public bool GetServerResponseEnd(string rep,
			                             SqlConnection connectionToServerDB)
		{
             AXParamBlock pBlock = new AXParamBlock();
             SqlDataReader readerPtr = dbConModule.sqlQuery("Select * from tblServerResponse where peram6 = '"+rep+"' and peram5 = 'completed'",connectionToServerDB);
			 if(readerPtr.Read())
			 {
				 readerPtr.Close();
                 return true;
			 }
			 readerPtr.Close();
			 return false;
		}

		public ConnectionHelper()
		{
			///
			/// Required for Windows.Forms Class Composition Designer support
			///
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		/// <summary> 
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}


		#region Component Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			components = new System.ComponentModel.Container();
		}
		#endregion
	}

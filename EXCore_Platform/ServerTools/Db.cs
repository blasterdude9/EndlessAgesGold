// Db.cs - SQL access for Server Tools.
//
// Same layout the old web tools used:
//   main database (EARevolution)  -> tblServers lists each game server and where its
//                                    two databases are:
//     "server" database (ServerNode) : status tables, logs, tblMainConfiguration,
//                                      tblCommandsToServer / tblServerResponse
//     "hub" database (EARevolution)  : accounts, characters, inventory, items, zones
//
// Every query here uses parameters - nothing typed into the tool is pasted into SQL.
using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;
using System.IO;
using System.Threading;

namespace EAServerTools
{
    public class GameServer
    {
        public string Name;
        public string ServerConn;   // ServerNode-style database (status, logs, settings, commands)
        public string HubConn;      // EARevolution-style database (accounts, characters, items)
        public override string ToString() { return Name; }
    }

    public static class Db
    {
        public static string SqlServer = "127.0.0.1";
        public static string MainDatabase = "EARevolution";
        public static bool WindowsLogin = true;      // Login=windows (default) or sql
        public static string User = "", Password = "";
        public static string Requester = "ServerTools";

        public static void LoadConfig(string exeDir)
        {
            string cfg = Path.Combine(exeDir, "ServerTools.cfg");
            if (!File.Exists(cfg))
            {
                File.WriteAllText(cfg,
                    "; Endless Ages Server Tools\r\n" +
                    "; SQL Server holding the game databases\r\n" +
                    "SqlServer=127.0.0.1\r\n" +
                    "; Main database (has tblServers, accounts, characters)\r\n" +
                    "MainDatabase=EARevolution\r\n" +
                    "; windows = your Windows login (like sqlcmd -E).  sql = use User/Password below\r\n" +
                    "Login=windows\r\n" +
                    "User=\r\n" +
                    "Password=\r\n");
                return;
            }
            foreach (string raw in File.ReadAllLines(cfg))
            {
                string l = raw.Trim();
                if (l.Length == 0 || l[0] == ';' || l[0] == '#') continue;
                int eq = l.IndexOf('=');
                if (eq < 0) continue;
                string k = l.Substring(0, eq).Trim().ToLowerInvariant(), v = l.Substring(eq + 1).Trim();
                if (k == "sqlserver" && v.Length > 0) SqlServer = v;
                else if (k == "maindatabase" && v.Length > 0) MainDatabase = v;
                else if (k == "login") WindowsLogin = !v.Equals("sql", StringComparison.OrdinalIgnoreCase);
                else if (k == "user") User = v;
                else if (k == "password") Password = v;
            }
        }

        public static string Build(string server, string database, string uid = null, string pwd = null)
        {
            var b = new SqlConnectionStringBuilder();
            b.DataSource = string.IsNullOrEmpty(server) ? SqlServer : server;
            b.InitialCatalog = database;
            b.ConnectTimeout = 8;
            if (WindowsLogin && string.IsNullOrEmpty(uid)) b.IntegratedSecurity = true;
            else if (!string.IsNullOrEmpty(uid)) { b.UserID = uid; b.Password = pwd ?? ""; }
            else if (!WindowsLogin) { b.UserID = User; b.Password = Password; }
            else b.IntegratedSecurity = true;
            return b.ConnectionString;
        }

        public static string MainConn { get { return Build(SqlServer, MainDatabase); } }

        // Connection string from tblServers: use the login stored there, and if that login
        // doesn't work fall back to our own (Windows) login on the same server/database.
        static string Pick(string server, string db, string uid, string pwd)
        {
            if (string.IsNullOrEmpty(db)) db = MainDatabase;
            if (!string.IsNullOrEmpty(uid))
            {
                string withTable = Build(server, db, uid, pwd);
                try { using (var c = new SqlConnection(withTable)) { c.Open(); } return withTable; }
                catch { }
            }
            return Build(server, db);
        }

        public static List<GameServer> LoadServers()
        {
            var list = new List<GameServer>();
            var t = Query(MainConn,
                "SELECT servername, serverManageIP, ServerUsername, ServerPass, DatabaseName, HUBSqlIP, HUBUsername, HUBPassword, HUBDBName " +
                "FROM tblServers WHERE servermanageip IS NOT NULL");
            foreach (DataRow r in t.Rows)
            {
                var g = new GameServer();
                g.Name = Str(r, 0);
                g.ServerConn = Pick(Str(r, 1), Str(r, 4), Str(r, 2), Str(r, 3));
                g.HubConn = Pick(Str(r, 5), Str(r, 8), Str(r, 6), Str(r, 7));
                list.Add(g);
            }
            return list;
        }

        public static string Str(DataRow r, int i) { return r.IsNull(i) ? "" : r[i].ToString().Trim(); }

        static void AddParams(SqlCommand cmd, object[] args)
        {
            if (args == null) return;
            for (int i = 0; i + 1 < args.Length; i += 2)
                cmd.Parameters.AddWithValue((string)args[i], args[i + 1] ?? DBNull.Value);
        }

        // args: "@name", value, "@name2", value2 ...
        public static DataTable Query(string conn, string sql, params object[] args)
        {
            using (var c = new SqlConnection(conn))
            using (var cmd = new SqlCommand(sql, c))
            {
                cmd.CommandTimeout = 30;
                AddParams(cmd, args);
                c.Open();
                var t = new DataTable();
                using (var r = cmd.ExecuteReader()) t.Load(r);
                return t;
            }
        }

        public static int Exec(string conn, string sql, params object[] args)
        {
            using (var c = new SqlConnection(conn))
            using (var cmd = new SqlCommand(sql, c))
            {
                cmd.CommandTimeout = 30;
                AddParams(cmd, args);
                c.Open();
                return cmd.ExecuteNonQuery();
            }
        }

        public static object Scalar(string conn, string sql, params object[] args)
        {
            using (var c = new SqlConnection(conn))
            using (var cmd = new SqlCommand(sql, c))
            {
                AddParams(cmd, args);
                c.Open();
                return cmd.ExecuteScalar();
            }
        }

        public static bool ColumnExists(string conn, string table, string column)
        {
            object o = Scalar(conn, "SELECT COL_LENGTH(@t, @c)", "@t", table, "@c", column);
            return o != null && o != DBNull.Value;
        }

        // ---- commands to the running game server (it polls tblCommandsToServer) ----
        public static void SendCommand(GameServer s, int type, string p1 = "", string p2 = "", string p3 = "", string p4 = "", string p5 = "")
        {
            Exec(s.ServerConn,
                "INSERT INTO tblCommandsToServer (CommandType, peram1, peram2, peram3, peram4, peram5, peram6) VALUES (@t, @p1, @p2, @p3, @p4, @p5, @p6)",
                "@t", type.ToString(), "@p1", p1 ?? "", "@p2", p2 ?? "", "@p3", p3 ?? "", "@p4", p4 ?? "", "@p5", p5 ?? "", "@p6", Requester);
        }

        // waits for the server's "completed" answer; returns its peram1 lines, or null on time-out
        public static List<string> WaitResponse(GameServer s, int seconds)
        {
            for (int i = 0; i < seconds; i++)
            {
                Thread.Sleep(1000);
                var done = Query(s.ServerConn, "SELECT COUNT(*) FROM tblServerResponse WHERE peram6 = @r AND peram5 = 'completed'", "@r", Requester);
                if (Convert.ToInt32(done.Rows[0][0]) > 0)
                {
                    var lines = new List<string>();
                    foreach (DataRow r in Query(s.ServerConn, "SELECT peram1 FROM tblServerResponse WHERE peram6 = @r", "@r", Requester).Rows)
                        lines.Add(Str(r, 0));
                    Exec(s.ServerConn, "DELETE FROM tblServerResponse WHERE peram6 = @r", "@r", Requester);
                    return lines;
                }
            }
            return null;
        }
    }
}

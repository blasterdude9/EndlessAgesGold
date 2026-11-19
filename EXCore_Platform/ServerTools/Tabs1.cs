// Tabs1.cs - Status, Live Zones, Server Settings, Server Tools, Logs
using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace EAServerTools
{
    // ------------------------------------------------------------------ Status
    public class StatusTab : TabBase
    {
        public override string Title { get { return "Status"; } }
        TextBox m_out = new TextBox { Multiline = true, ReadOnly = true, ScrollBars = ScrollBars.Vertical, Font = new Font("Consolas", 10f), BackColor = SystemColors.Window };
        CheckBox m_auto = new CheckBox { Text = "Refresh every 5 seconds", Left = 110, Top = 10, AutoSize = true, Checked = true };
        Timer m_timer = new Timer { Interval = 5000 };

        public StatusTab()
        {
            B(this, "Refresh", 10, 6, 90, (s, e) => Refresh2());
            Controls.Add(m_auto);
            m_out.SetBounds(10, 40, 900, 600);
            m_out.Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(m_out);
            m_timer.Tick += (s, e) => { if (m_auto.Checked && Visible && Server != null) Refresh2(); };
            m_timer.Start();
        }
        public override void OnShown() { Refresh2(); }
        public override void OnServerChanged() { if (Visible) Refresh2(); }

        void Refresh2()
        {
            if (!NeedServer()) return;
            var s = Server;
            Run("Reading status", () =>
            {
                var sb = new StringBuilder();
                var m = Db.Query(s.ServerConn, "SELECT currentConnections, connectionsPeak, msgSendPerSecond, msgReceivedPerSecond, bandwidthOutgoingPerSecond, bandwidthIncommingPerSecond, bandwidthintotal, bandwidthOutTotal, bandwidthtotal, lastupdated FROM tblMainServerStatus");
                sb.AppendLine("GAME SERVER  " + s.Name);
                if (m.Rows.Count == 0) sb.AppendLine("  no status row (server never ran?)");
                else
                {
                    var r = m.Rows[0];
                    sb.AppendLine("  Updated:                 " + Db.Str(r, 9) + Age(r, 9));
                    sb.AppendLine("  Connections now / peak:  " + Db.Str(r, 0) + " / " + Db.Str(r, 1));
                    sb.AppendLine("  Messages out / in (s):   " + Db.Str(r, 2) + " / " + Db.Str(r, 3));
                    sb.AppendLine("  Bandwidth out / in (s):  " + Db.Str(r, 4) + " / " + Db.Str(r, 5));
                    sb.AppendLine("  Total in / out / all:    " + Db.Str(r, 6) + " MB / " + Db.Str(r, 7) + " MB / " + Db.Str(r, 8) + " MB");
                }
                sb.AppendLine();
                var a = Db.Query(s.ServerConn, "SELECT currentBots, currentBotPeak, worldmngcount, totalbotcount, packetsoutpersecond, packetsinpersecond, totalbandwidthin, totalbandwidthout, bandwidthincommingpersecond, bandwidthoutgoingpersecond, totalbandwidthuse, lastupdated FROM tblAIServerStatus");
                sb.AppendLine("AI SERVER");
                if (a.Rows.Count == 0) sb.AppendLine("  no status row");
                else
                {
                    var r = a.Rows[0];
                    sb.AppendLine("  Updated:                 " + Db.Str(r, 11) + Age(r, 11));
                    sb.AppendLine("  Bots now / peak:         " + Db.Str(r, 0) + " / " + Db.Str(r, 1));
                    sb.AppendLine("  AI worlds running:       " + Db.Str(r, 2));
                    sb.AppendLine("  Packets out / in (s):    " + Db.Str(r, 4) + " / " + Db.Str(r, 5));
                    sb.AppendLine("  Bandwidth in / out (s):  " + Db.Str(r, 8) + " / " + Db.Str(r, 9));
                    sb.AppendLine("  Total in / out / all:    " + Db.Str(r, 6) + " MB / " + Db.Str(r, 7) + " MB / " + Db.Str(r, 10) + " MB");
                }
                sb.AppendLine();
                sb.AppendLine("CLUSTER NODES ONLINE");
                var n = Db.Query(s.HubConn, "SELECT NODEID, CONNECTIONCOUNT, NODEIP FROM SYSLIVESERVERNODES WHERE DATEADD(SECOND, 25, CHECKINTIME) > GETDATE()");
                if (n.Rows.Count == 0) sb.AppendLine("  none - the game server is not running (or not checking in)");
                foreach (DataRow r in n.Rows) sb.AppendLine("  Node " + Db.Str(r, 0) + "   IP " + Db.Str(r, 2) + "   live connections " + Db.Str(r, 1));
                return sb.ToString();
            }, txt => m_out.Text = txt);
        }

        static string Age(DataRow r, int i)
        {
            if (r.IsNull(i) || !(r[i] is DateTime)) return "";
            var secs = (DateTime.Now - (DateTime)r[i]).TotalSeconds;
            return secs > 60 ? "   (" + (int)(secs / 60) + " min ago - not running?)" : "";
        }
    }

    // ------------------------------------------------------------------ Live zones
    public class LiveZonesTab : TabBase
    {
        public override string Title { get { return "Live Zones"; } }
        DataGridView m_zones, m_nodes;
        public LiveZonesTab()
        {
            B(this, "Refresh", 10, 6, 90, (s, e) => Refresh2());
            L(this, "Zones running now:", 10, 40);
            m_zones = Grid(this, 10, 62, 900, 380, AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right);
            L(this, "Cluster nodes online:", 10, 452);
            m_nodes = Grid(this, 10, 474, 900, 150, AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right);
        }
        public override void OnShown() { Refresh2(); }
        public override void OnServerChanged() { if (Visible) Refresh2(); }
        void Refresh2()
        {
            if (!NeedServer()) return;
            var s = Server;
            Run("Reading live zones", () => new[] {
                Db.Query(s.HubConn, "SELECT z.NODEID AS Node, z.ZONENAME AS Zone, z.CHANNEL AS Channel, z.CUR_CONNECTIONS AS Players " +
                                    "FROM sysLiveZoneDB z JOIN sysLiveServerNodes n ON n.nodeid = z.nodeid " +
                                    "WHERE DATEADD(SECOND, 25, n.CHECKINTIME) > GETDATE() ORDER BY z.ZONENAME"),
                Db.Query(s.HubConn, "SELECT NODEID AS Node, NODEIP AS IP, CONNECTIONCOUNT AS Connections, CHECKINTIME AS LastCheckIn FROM SYSLIVESERVERNODES WHERE DATEADD(SECOND, 25, CHECKINTIME) > GETDATE()")
            }, t => { m_zones.DataSource = t[0]; m_nodes.DataSource = t[1]; });
        }
    }

    // ------------------------------------------------------------------ Server settings (tblMainConfiguration)
    public class SettingsTab : TabBase
    {
        public override string Title { get { return "Server Settings"; } }
        static readonly string[][] Fields = {
            new[] { "Version", "Version (clients must match)" }, new[] { "Max_players", "Max players" },
            new[] { "Frequency", "Gameplay turn speed" }, new[] { "Dead_PCEntity_Dissolve_Time", "Dead player dissolve time" },
            new[] { "Dead_NPCEntity_Dissolve_Time", "Dead NPC dissolve time" }, new[] { "AIServer_Cycle", "AI server turn speed" },
            new[] { "Murder_Level_Count", "Murder level count" }, new[] { "Workoff_Murder_Rate", "Murder work-off rate" },
            new[] { "Duration_between_Backups", "Time between backups" }, new[] { "MainServerIP", "Main server IP (for the AI server)" },
            new[] { "AIServerUsername", "AI server username" }, new[] { "AIServerPassword", "AI server password" },
            new[] { "LogPriorityLevel", "Log priority level" }, new[] { "RegenTurnSpeed", "Regen turn speed" },
            new[] { "DefaultHealthRegenAmount", "Default health regen" }, new[] { "LootMode", "Loot mode" },
            new[] { "AgressorMaxTrack", "Aggressor max track" }, new[] { "MaxDotTrack", "Max DOT track" },
            new[] { "DamageMode", "Damage mode" }, new[] { "RespawnHealthPercentage", "Respawn health %" } };
        Dictionary<string, TextBox> m_box = new Dictionary<string, TextBox>();

        public SettingsTab()
        {
            AutoScroll = true;
            B(this, "Reload", 10, 6, 90, (s, e) => Load2());
            B(this, "Apply", 110, 6, 90, (s, e) => Apply());
            L(this, "Changes take effect the next time the game server starts.", 215, 8).ForeColor = Color.DimGray;
            for (int i = 0; i < Fields.Length; i++)
            {
                int col = i / 10, row = i % 10;
                L(this, Fields[i][1], 10 + col * 440, 46 + row * 32, 220);
                m_box[Fields[i][0]] = T(this, 235 + col * 440, 46 + row * 32, 180);
            }
        }
        public override void OnShown() { Load2(); }
        public override void OnServerChanged() { if (Visible) Load2(); }

        void Load2()
        {
            if (!NeedServer()) return;
            var s = Server;
            var cols = new List<string>(); foreach (var f in Fields) cols.Add(f[0]);
            Run("Reading tblMainConfiguration", () => Db.Query(s.ServerConn, "SELECT TOP 1 " + string.Join(", ", cols) + " FROM tblMainConfiguration"), t =>
            {
                if (t.Rows.Count == 0) { Host.SetStatus("tblMainConfiguration is empty", true); return; }
                foreach (var f in Fields) m_box[f[0]].Text = Db.Str(t.Rows[0], t.Columns.IndexOf(f[0]));
            });
        }

        void Apply()
        {
            if (!NeedServer()) return;
            if (!Confirm("Save these settings to tblMainConfiguration?")) return;
            var s = Server;
            var sets = new List<string>(); var args = new List<object>();
            foreach (var f in Fields) { sets.Add(f[0] + " = @" + f[0]); args.Add("@" + f[0]); args.Add(m_box[f[0]].Text.Trim()); }
            Run("Saving settings", () => Db.Exec(s.ServerConn, "UPDATE tblMainConfiguration SET " + string.Join(", ", sets), args.ToArray()));
        }
    }

    // ------------------------------------------------------------------ Server tools (commands to the running server)
    public class ToolsTab : TabBase
    {
        public override string Title { get { return "Server Tools"; } }
        TextBox m_out;
        TextBox m_stencil, m_newBase, m_newPass, m_count;

        public ToolsTab()
        {
            int y = 10;
            Action<string, string, EventHandler> row = (btn, help, click) =>
            {
                B(this, btn, 10, y, 190, click);
                L(this, help, 210, y + 2).ForeColor = Color.DimGray;
                y += 34;
            };
            row("Restart server (10 min warning)", "Admin-only logins, warns players, then restarts.", (s, e) => Cmd(8, "Restart queued", true));
            row("Toggle admin-only mode", "Only admin accounts can log in while this is on.", (s, e) => Cmd(9, "Admin mode", true));
            row("Save everything now", "Writes all players to the database.", (s, e) => Cmd(10, "Save sent", false));
            row("Update status tables", "Makes the server refresh its status rows.", (s, e) => Cmd(14, "Status update sent", false));
            row("Disconnect everyone", "Kicks all players off.", (s, e) => { if (Confirm("Disconnect every player?")) Cmd(15, "Disconnect-all sent", false); });

            y += 10;
            L(this, "Make test accounts (copies the first character of a stencil account):", 10, y); y += 26;
            L(this, "Stencil account", 10, y); m_stencil = T(this, 120, y, 140);
            L(this, "New name base", 280, y); m_newBase = T(this, 380, y, 120, "test");
            L(this, "Password", 515, y); m_newPass = T(this, 580, y, 110);
            L(this, "How many", 705, y); m_count = T(this, 770, y, 50, "5");
            B(this, "Create", 835, y - 1, 80, (s, e) => MakeTestAccounts());
            y += 40;
            L(this, "Result:", 10, y); y += 22;
            m_out = new TextBox { Multiline = true, ReadOnly = true, ScrollBars = ScrollBars.Vertical, Left = 10, Top = y, Width = 900, Height = 200, Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right };
            Controls.Add(m_out);
        }

        void Cmd(int type, string label, bool waitAnswer)
        {
            if (!NeedServer()) return;
            var s = Server;
            Run(label, () =>
            {
                Db.SendCommand(s, type);
                if (!waitAnswer) return label + ". The server picks it up within a few seconds.";
                var ans = Db.WaitResponse(s, 6);
                return ans == null ? label + ": no answer from the server in 6 seconds (is it running?)" : label + ": " + string.Join(" / ", ans);
            }, txt => m_out.AppendText(DateTime.Now.ToString("HH:mm:ss  ") + txt + "\r\n"));
        }

        void MakeTestAccounts()
        {
            if (!NeedServer()) return;
            int n; if (!int.TryParse(m_count.Text, out n) || n < 1 || n > 100) { Host.SetStatus("How many: 1-100", true); return; }
            string stencil = m_stencil.Text.Trim(), baseName = m_newBase.Text.Trim(), pass = m_newPass.Text;
            if (stencil == "" || baseName == "" || pass == "") { Host.SetStatus("Fill in stencil account, name base and password", true); return; }
            if (!Confirm("Create " + n + " test accounts " + baseName + "0.." + baseName + (n - 1) + "?")) return;
            var s = Server;
            Run("Creating test accounts", () =>
            {
                var c = Db.Query(s.HubConn, "SELECT TOP 1 charName, accesslevel, respawnWorld, currentWorldID, currentWorldMap, dbCfg, altCfg, cfgStasis, originaldbCfg, energy, gamename, respawnworldname FROM gmCharacters WHERE username = @u AND inuse = '1'", "@u", stencil);
                if (c.Rows.Count == 0) return "The stencil account has no character in use - nothing to copy.";
                var r = c.Rows[0];
                int made = 0;
                for (int i = 0; i < n; i++)
                {
                    string u = baseName + i;
                    if (Convert.ToInt32(Db.Scalar(s.HubConn, "SELECT COUNT(*) FROM gmAccountBase WHERE Username = @u", "@u", u)) > 0) continue;
                    AccountsTab.InsertAccount(s.HubConn, u, pass, 4, Db.Str(r, 10));
                    Db.Exec(s.HubConn,
                        "INSERT INTO gmCharacters (respawnworldname, curDbCfg, username, charName, accesslevel, respawnWorld, currentWorldID, currentWorldMap, dbCfg, altCfg, cfgStasis, originaldbCfg, energy, gameName, inuse, rebirthY) " +
                        "VALUES (@rwn, @cfg, @u, @cn, @al, @rw, @wid, @wmap, @cfg, @alt, @st, @cfg, @en, @gn, '1', '3')",
                        "@rwn", Db.Str(r, 11), "@cfg", Db.Str(r, 5), "@u", u, "@cn", Db.Str(r, 0), "@al", Db.Str(r, 1), "@rw", Db.Str(r, 2),
                        "@wid", Db.Str(r, 3), "@wmap", Db.Str(r, 4), "@alt", Db.Str(r, 6), "@st", Db.Str(r, 7), "@en", Db.Str(r, 9), "@gn", Db.Str(r, 10));
                    made++;
                }
                return "Created " + made + " test accounts (existing names skipped).";
            }, txt => m_out.AppendText(DateTime.Now.ToString("HH:mm:ss  ") + txt + "\r\n"));
        }
    }

    // ------------------------------------------------------------------ Logs
    public class LogsTab : TabBase
    {
        public override string Title { get { return "Logs"; } }
        RadioButton m_main = new RadioButton { Text = "Game server log", Left = 10, Top = 10, AutoSize = true, Checked = true };
        RadioButton m_ai = new RadioButton { Text = "AI server log", Left = 140, Top = 10, AutoSize = true };
        TextBox m_count, m_filter;
        CheckBox m_auto = new CheckBox { Text = "Auto refresh (5 s)", Left = 700, Top = 10, AutoSize = true };
        DataGridView m_grid;
        Timer m_timer = new Timer { Interval = 5000 };

        public LogsTab()
        {
            Controls.Add(m_main); Controls.Add(m_ai); Controls.Add(m_auto);
            L(this, "Lines", 260, 8); m_count = T(this, 300, 7, 60, "200");
            L(this, "Contains", 375, 8); m_filter = T(this, 435, 7, 150);
            B(this, "Refresh", 595, 6, 90, (s, e) => Refresh2());
            m_main.CheckedChanged += (s, e) => { if (m_main.Checked) Refresh2(); };
            m_ai.CheckedChanged += (s, e) => { if (m_ai.Checked) Refresh2(); };
            m_grid = Grid(this, 10, 40, 900, 600, AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right);
            m_grid.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.AllCells;
            m_timer.Tick += (s, e) => { if (m_auto.Checked && Visible && Server != null) Refresh2(); };
            m_timer.Start();
        }
        public override void OnShown() { Refresh2(); }
        public override void OnServerChanged() { if (Visible) Refresh2(); }

        void Refresh2()
        {
            if (!NeedServer()) return;
            int n; if (!int.TryParse(m_count.Text, out n) || n < 1) n = 200; if (n > 5000) n = 5000;
            string table = m_main.Checked ? "tblServerLog" : "tblAiServerLog";
            string filter = m_filter.Text.Trim();
            var s = Server;
            Run("Reading " + table, () => Db.Query(s.ServerConn,
                "SELECT TOP (@n) timePosted AS [Time], logMsg AS [Message] FROM " + table +
                (filter.Length > 0 ? " WHERE logMsg LIKE @f" : "") + " ORDER BY timePosted DESC",
                "@n", n, "@f", "%" + filter + "%"), t => m_grid.DataSource = t);
        }
    }
}

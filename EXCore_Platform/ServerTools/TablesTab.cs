// TablesTab.cs - everything else the old web tools edited, as editable tables:
// server scripts, AI worlds, global worlds, world stencils + rules, items (APX),
// NPCs, NPC inventory, quests, quest menus, NPC chat.  Plus a raw SQL box.
//
// Edit cells / add rows at the bottom / select a row and press Delete, then Save.
// Each changed row is written with its own UPDATE/INSERT/DELETE inside one
// transaction; if any UPDATE/DELETE would touch more than one row, nothing is saved.
using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;

namespace EAServerTools
{
    public class TablesTab : TabBase
    {
        public override string Title { get { return "Game Data"; } }

        class Def
        {
            public string Name, Table, Order; public bool Hub; public int Cmd; public string CmdCol;
            public override string ToString() { return Name; }
        }

        static readonly Def[] Defs =
        {
            new Def { Name = "Server Scripts",          Table = "tblServerScripts",   Hub = false, Order = "scriptName", Cmd = 26 },
            new Def { Name = "AI Worlds",               Table = "tblAIBaseWorlds",    Hub = false, Order = "EXGBaseName" },
            new Def { Name = "Global Worlds",           Table = "tblSupportedWorlds", Hub = false, Order = "id" },
            new Def { Name = "Stencil Zones",           Table = "sysWorldStencils",   Hub = true,  Order = "channel" },
            new Def { Name = "Stencil Zone Rules",      Table = "sysWorldRules",      Hub = true,  Order = "wldID, pKey" },
            new Def { Name = "World Rule Names",        Table = "gm_worldRules",      Hub = true,  Order = "orderindex" },
            new Def { Name = "Items (APX)",             Table = "gm_DynamicItems",    Hub = true,  Order = "itemName", Cmd = 27, CmdCol = "GLID" },
            new Def { Name = "Item ID Pool",            Table = "gm_DynamicIDPool",   Hub = true },
            new Def { Name = "Item Visuals",            Table = "gm_ItemVisualDump",  Hub = true },
            new Def { Name = "NPCs",                    Table = "gm_IGNEntity",       Hub = true,  Order = "GENERATORNAME", Cmd = 22, CmdCol = "SPAWNID" },
            new Def { Name = "NPC Inventory",           Table = "gm_IGNInventory",    Hub = true },
            new Def { Name = "NPC Folders",             Table = "gm_IGNFolders",      Hub = true },
            new Def { Name = "NPC Dimension Configs",   Table = "gm_IGNDimCfgs",      Hub = true },
            new Def { Name = "NPC Quests",              Table = "gm_IGNQuestData",    Hub = true },
            new Def { Name = "NPC Quest Rules",         Table = "gm_IGNDynamicRules", Hub = true },
            new Def { Name = "NPC Chat",                Table = "gm_IGNChatStrings",  Hub = true },
            new Def { Name = "EDB",                     Table = "gm_EDB",             Hub = true },
            new Def { Name = "Skills",                  Table = "gm_Skills",          Hub = true },
            new Def { Name = "Stats",                   Table = "gm_Stats",           Hub = true },
            new Def { Name = "Elements",                Table = "gm_Elements",        Hub = true },
        };

        ComboBox m_pick = new ComboBox { DropDownStyle = ComboBoxStyle.DropDownList, Width = 200 };
        TextBox m_where;
        DataGridView m_grid;
        Button m_live;
        Label m_info;
        DataTable m_data;
        Def m_cur;
        string m_curConn;

        public TablesTab()
        {
            var top = new Panel { Dock = DockStyle.Top, Height = 74 };
            Controls.Add(top);
            L(top, "Table:", 8, 8);
            m_pick.Left = 52; m_pick.Top = 8; top.Controls.Add(m_pick);
            foreach (var d in Defs) m_pick.Items.Add(d);
            L(top, "Where:", 262, 8);
            m_where = T(top, 306, 8, 300);
            B(top, "Load", 614, 7, 70, (s, e) => LoadTable());
            B(top, "Save changes", 690, 7, 100, (s, e) => Save());
            m_live = B(top, "Push live", 796, 7, 150, (s, e) => PushLive());
            m_live.Enabled = false;
            m_info = L(top, "Pick a table and press Load.  Where is optional, e.g.  itemName LIKE '%sword%'", 8, 40);
            m_info.ForeColor = Color.DimGray;
            m_pick.SelectedIndexChanged += (s, e) => { m_where.Text = ""; LoadTable(); };

            m_grid = Grid(this, 0, 74, 100, 100, AnchorStyles.None);
            m_grid.Dock = DockStyle.Fill;
            m_grid.ReadOnly = false;
            m_grid.AllowUserToAddRows = true;
            m_grid.AllowUserToDeleteRows = true;
            m_grid.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.None;
            m_grid.DataError += (s, e) => { Host.SetStatus("Bad value in " + m_grid.Columns[e.ColumnIndex].Name + ": " + e.Exception.Message, true); e.ThrowException = false; };
            m_grid.BringToFront();

            var sqlBox = new SqlBox(this);
            Controls.Add(sqlBox);
        }

        public override void OnServerChanged() { m_data = null; m_grid.DataSource = null; }

        void LoadTable()
        {
            if (!NeedServer()) return;
            var d = m_pick.SelectedItem as Def;
            if (d == null) return;
            string conn = d.Hub ? Server.HubConn : Server.ServerConn;
            string where = m_where.Text.Trim();
            string sql = "SELECT TOP 5000 * FROM " + d.Table + (where.Length > 0 ? " WHERE " + where : "") + (d.Order != null ? " ORDER BY " + d.Order : "");
            Run("Loading " + d.Name, () => Db.Query(conn, sql), t =>
            {
                m_cur = d; m_curConn = conn; m_data = t;
                for (int i = 0; i < t.Rows.Count; i++) t.Rows[i].AcceptChanges();
                foreach (DataColumn c in t.Columns) c.ReadOnly = false;
                m_grid.DataSource = t;
                foreach (DataGridViewColumn c in m_grid.Columns) { c.Width = 120; c.SortMode = DataGridViewColumnSortMode.NotSortable; }
                m_live.Enabled = d.Cmd != 0;
                m_live.Text = d.Cmd == 26 ? "Reload scripts on server" : d.Cmd == 27 ? "Push selected item live" : d.Cmd == 22 ? "Push selected NPC live" : "Push live";
                m_info.Text = d.Table + ": " + t.Rows.Count + " rows" + (t.Rows.Count == 5000 ? " (first 5000 - use Where to narrow)" : "") + ".  Edit cells, add at the bottom, Delete key removes a row, then Save changes.";
            });
        }

        // the row's original values, so the WHERE matches exactly the row we loaded
        static void AddWhere(SqlCommand cmd, List<string> parts, DataRow r, DataColumnCollection cols, Dictionary<string, bool> skip)
        {
            int i = 0;
            foreach (DataColumn c in cols)
            {
                if (skip.ContainsKey(c.ColumnName)) continue;
                object v = r[c, DataRowVersion.Original];
                if (v == DBNull.Value) parts.Add("[" + c.ColumnName + "] IS NULL");
                else { parts.Add("[" + c.ColumnName + "] = @w" + i); cmd.Parameters.AddWithValue("@w" + i, v); }
                i++;
            }
        }

        void Save()
        {
            if (m_data == null) return;
            m_grid.EndEdit();
            var changes = m_data.GetChanges();
            if (changes == null) { Host.SetStatus("Nothing changed.", false); return; }
            var t = m_data; var d = m_cur; string conn = m_curConn;
            int nDel = changes.Select("", "", DataViewRowState.Deleted).Length;
            if (nDel > 0 && !Confirm("Delete " + nDel + " row(s) from " + d.Table + "?")) return;

            Run("Saving " + d.Name, () =>
            {
                // text/ntext/image can't be compared with =, and identity/timestamp can't be written
                var noCompare = new Dictionary<string, bool>();
                var noWrite = new Dictionary<string, bool>();
                foreach (DataRow r in Db.Query(conn,
                    "SELECT c.name, t.name AS typ, c.is_identity, c.is_computed FROM sys.columns c JOIN sys.types t ON t.user_type_id = c.user_type_id WHERE c.object_id = OBJECT_ID(@t)", "@t", d.Table).Rows)
                {
                    string n = Db.Str(r, 0), typ = Db.Str(r, 1).ToLowerInvariant();
                    if (typ == "text" || typ == "ntext" || typ == "image" || typ == "xml") noCompare[n] = true;
                    if (typ == "timestamp" || typ == "rowversion" || Convert.ToBoolean(r[2]) || Convert.ToBoolean(r[3])) { noWrite[n] = true; noCompare[n] = true; }
                }
                int up = 0, ins = 0, del = 0;
                using (var c = new SqlConnection(conn))
                {
                    c.Open();
                    using (var tx = c.BeginTransaction())
                    {
                        foreach (DataRow r in t.Rows)
                        {
                            if (r.RowState == DataRowState.Unchanged) continue;
                            var cmd = new SqlCommand { Connection = c, Transaction = tx };
                            var where = new List<string>();
                            if (r.RowState == DataRowState.Deleted)
                            {
                                AddWhere(cmd, where, r, t.Columns, noCompare);
                                cmd.CommandText = "DELETE TOP (2) FROM " + d.Table + " WHERE " + string.Join(" AND ", where);
                                Check(cmd.ExecuteNonQuery(), tx, "delete"); del++;
                            }
                            else if (r.RowState == DataRowState.Modified)
                            {
                                var sets = new List<string>(); int i = 0;
                                foreach (DataColumn col in t.Columns)
                                {
                                    if (noWrite.ContainsKey(col.ColumnName)) continue;
                                    if (Equals(r[col, DataRowVersion.Original], r[col, DataRowVersion.Current])) continue;
                                    sets.Add("[" + col.ColumnName + "] = @s" + i); cmd.Parameters.AddWithValue("@s" + i, r[col]); i++;
                                }
                                if (sets.Count == 0) continue;
                                AddWhere(cmd, where, r, t.Columns, noCompare);
                                cmd.CommandText = "UPDATE TOP (2) " + d.Table + " SET " + string.Join(", ", sets) + " WHERE " + string.Join(" AND ", where);
                                Check(cmd.ExecuteNonQuery(), tx, "update"); up++;
                            }
                            else if (r.RowState == DataRowState.Added)
                            {
                                var names = new List<string>(); var vals = new List<string>(); int i = 0;
                                foreach (DataColumn col in t.Columns)
                                {
                                    if (noWrite.ContainsKey(col.ColumnName) || r[col] == DBNull.Value) continue;
                                    names.Add("[" + col.ColumnName + "]"); vals.Add("@v" + i); cmd.Parameters.AddWithValue("@v" + i, r[col]); i++;
                                }
                                if (names.Count == 0) continue;
                                cmd.CommandText = "INSERT INTO " + d.Table + " (" + string.Join(", ", names) + ") VALUES (" + string.Join(", ", vals) + ")";
                                cmd.ExecuteNonQuery(); ins++;
                            }
                        }
                        tx.Commit();
                    }
                }
                return up + " updated, " + ins + " added, " + del + " deleted";
            }, msg => { Host.SetStatus("Saved " + d.Name + ": " + msg, false); LoadTable(); });
        }

        static void Check(int n, SqlTransaction tx, string what)
        {
            if (n == 1) return;
            tx.Rollback();
            throw new Exception(n == 0
                ? "a row changed in the database since you loaded it - nothing saved, press Load and try again"
                : "the " + what + " matched more than one identical row - nothing saved");
        }

        void PushLive()
        {
            if (!NeedServer() || m_cur == null || m_cur.Cmd == 0) return;
            string p = "";
            if (m_cur.CmdCol != null)
            {
                p = Cell(m_grid, m_cur.CmdCol);
                if (string.IsNullOrEmpty(p)) { Host.SetStatus("Select a row first (needs " + m_cur.CmdCol + ").", true); return; }
            }
            var s = Server; int cmd = m_cur.Cmd;
            Run("Sending update to server", () => Db.SendCommand(s, cmd, p, p));
        }

        // raw SQL box at the bottom, for anything the tables above don't cover
        class SqlBox : Panel
        {
            public SqlBox(TablesTab tab)
            {
                Dock = DockStyle.Bottom; Height = 34;
                var db = new ComboBox { DropDownStyle = ComboBoxStyle.DropDownList, Left = 8, Top = 5, Width = 110 };
                db.Items.AddRange(new object[] { "Hub DB", "Server DB" }); db.SelectedIndex = 0;
                var q = new TextBox { Left = 124, Top = 5, Width = 600, Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right };
                var go = new Button { Text = "Run SQL", Top = 4, Width = 90, Height = 26, Anchor = AnchorStyles.Top | AnchorStyles.Right };
                Controls.Add(db); Controls.Add(q); Controls.Add(go);
                Resize += (s, e) => { go.Left = Width - 98; q.Width = go.Left - 132; };
                go.Click += (s, e) =>
                {
                    if (!tab.NeedServer() || q.Text.Trim().Length == 0) return;
                    string sql = q.Text.Trim();
                    bool read = sql.StartsWith("select", StringComparison.OrdinalIgnoreCase);
                    if (!read && !tab.Confirm("Run this on the " + db.Text + "?\r\n\r\n" + sql)) return;
                    string conn = db.SelectedIndex == 0 ? tab.Server.HubConn : tab.Server.ServerConn;
                    if (read)
                        tab.Run("Running query", () => Db.Query(conn, sql), t =>
                        {
                            tab.m_data = null; tab.m_cur = null; tab.m_live.Enabled = false;
                            tab.m_grid.DataSource = t;
                            tab.m_info.Text = "Query result: " + t.Rows.Count + " rows (read only - use the table picker to edit).";
                        });
                    else
                        tab.Run("Running SQL", () => Db.Exec(conn, sql), n => tab.Host.SetStatus("Done: " + n + " row(s) affected", false));
                };
            }
        }
    }
}

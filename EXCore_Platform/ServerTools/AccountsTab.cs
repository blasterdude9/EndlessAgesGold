// AccountsTab.cs - accounts and characters (old AcManagePage / AccountManager control, plus
// the account actions it never finished: password, admin, ban, unstick, create).
using System;
using System.Data;
using System.Drawing;
using System.Windows.Forms;

namespace EAServerTools
{
    public class AccountsTab : TabBase
    {
        public override string Title { get { return "Accounts"; } }
        TextBox m_search, m_newUser, m_newPass, m_setPass, m_setMax;
        DataGridView m_accounts, m_chars, m_inv, m_stats, m_skills;
        TextBox m_itemSearch, m_qty, m_dmg, m_statVal, m_skillVal;
        ListBox m_itemResults;
        ComboBox m_spawns;

        public static void InsertAccount(string hub, string user, string pass, int maxChars, string inGameName = null)
        {
            bool hasIgn = !string.IsNullOrEmpty(inGameName) && Db.ColumnExists(hub, "dbo.gmAccountBase", "inGameName");
            Db.Exec(hub,
                "INSERT INTO dbo.gmAccountBase (Username, Password, GameAccess, PortalAccess, active, accessLevel, maxCharacters, LoggedOn, isadmin, muteDeaf, MinutesPlayed, Joined, recurringDate" + (hasIgn ? ", inGameName" : "") + ") " +
                "VALUES (@u, @p, 1, 1, '1', 1, @m, 0, 0, 0, 0, GETDATE(), DATEADD(year, 10, GETDATE())" + (hasIgn ? ", @ign" : "") + ")",
                "@u", user, "@p", pass, "@m", maxChars, "@ign", inGameName ?? "");
        }

        public AccountsTab()
        {
            var top = new Panel { Dock = DockStyle.Top, Height = 72 };
            L(top, "Find account", 10, 8); m_search = T(top, 95, 7, 160);
            B(top, "Search", 262, 6, 75, (s, e) => Find(false));
            B(top, "Who's logged on", 343, 6, 120, (s, e) => Find(true));
            m_search.KeyDown += (s, e) => { if (e.KeyCode == Keys.Enter) { Find(false); e.SuppressKeyPress = true; } };
            L(top, "New account:  user", 500, 8); m_newUser = T(top, 615, 7, 110);
            L(top, "password", 732, 8); m_newPass = T(top, 790, 7, 100);
            B(top, "Create", 897, 6, 75, (s, e) => Create());
            L(top, "Selected account:", 10, 42);
            m_setPass = T(top, 120, 41, 100); B(top, "Set password", 224, 40, 95, (s, e) => AccountUpdate("Password = @v", m_setPass.Text, "Password changed"));
            m_setMax = T(top, 330, 41, 40, "4"); B(top, "Set max chars", 374, 40, 95, (s, e) => { int m; if (int.TryParse(m_setMax.Text, out m)) AccountUpdate("maxCharacters = @v", m, "Max characters set"); });
            B(top, "Toggle admin", 480, 40, 95, (s, e) => AccountUpdate("isadmin = CASE WHEN isadmin = 1 THEN 0 ELSE 1 END", null, "Admin toggled"));
            B(top, "Unstick login", 580, 40, 95, (s, e) => AccountUpdate("LoggedOn = 0", null, "Logged-on flag cleared"));
            B(top, "Kick", 680, 40, 55, (s, e) => Kick());
            B(top, "Ban", 740, 40, 55, (s, e) => Ban(true));
            B(top, "Unban", 800, 40, 60, (s, e) => Ban(false));

            var split = new SplitContainer { Dock = DockStyle.Fill };
            bool splitSet = false;   // SplitterDistance can only be set once the control has its real size
            split.SizeChanged += (s2, e2) => { if (!splitSet && split.Width > 600) { split.SplitterDistance = 430; splitSet = true; } };
            m_accounts = Grid(split.Panel1, 0, 0, 10, 10, AnchorStyles.None); m_accounts.Dock = DockStyle.Fill;
            m_accounts.SelectionChanged += (s, e) => LoadChars();

            var right = split.Panel2;
            m_chars = Grid(right, 0, 0, 10, 150, AnchorStyles.None); m_chars.Dock = DockStyle.Top;
            m_chars.SelectionChanged += (s, e) => LoadCharDetails();
            var sub = new TabControl { Dock = DockStyle.Fill };
            right.Controls.Add(sub); sub.BringToFront();

            // inventory
            var pInv = new TabPage("Inventory");
            m_inv = Grid(pInv, 0, 0, 10, 10, AnchorStyles.None); m_inv.Dock = DockStyle.Fill;
            var invBar = new Panel { Dock = DockStyle.Bottom, Height = 150 };
            pInv.Controls.Add(invBar);
            B(invBar, "Delete selected item", 6, 6, 150, (s, e) => DeleteItem());
            L(invBar, "Find item", 6, 40); m_itemSearch = T(invBar, 70, 39, 160);
            B(invBar, "Search", 236, 38, 70, (s, e) => SearchItems());
            m_itemSearch.KeyDown += (s, e) => { if (e.KeyCode == Keys.Enter) { SearchItems(); e.SuppressKeyPress = true; } };
            m_itemResults = new ListBox { Left = 6, Top = 68, Width = 300, Height = 76 }; invBar.Controls.Add(m_itemResults);
            L(invBar, "Qty", 320, 70); m_qty = T(invBar, 350, 69, 50, "1");
            L(invBar, "Dmg +", 410, 70); m_dmg = T(invBar, 455, 69, 40, "0");
            B(invBar, "Give to character (bank)", 320, 100, 175, (s, e) => GiveItem());
            sub.TabPages.Add(pInv);

            // stats
            var pStats = new TabPage("Stats");
            m_stats = Grid(pStats, 0, 0, 10, 10, AnchorStyles.None); m_stats.Dock = DockStyle.Fill;
            var sBar = new Panel { Dock = DockStyle.Bottom, Height = 36 }; pStats.Controls.Add(sBar);
            L(sBar, "New value for selected stat", 6, 8); m_statVal = T(sBar, 180, 7, 80);
            B(sBar, "Set", 266, 6, 60, (s, e) => SetStatOrSkill(true));
            sub.TabPages.Add(pStats);

            // skills
            var pSkills = new TabPage("Skills");
            m_skills = Grid(pSkills, 0, 0, 10, 10, AnchorStyles.None); m_skills.Dock = DockStyle.Fill;
            var kBar = new Panel { Dock = DockStyle.Bottom, Height = 36 }; pSkills.Controls.Add(kBar);
            L(kBar, "New experience for selected skill", 6, 8); m_skillVal = T(kBar, 210, 7, 80);
            B(kBar, "Set", 296, 6, 60, (s, e) => SetStatOrSkill(false));
            sub.TabPages.Add(pSkills);

            // move
            var pMove = new TabPage("Move character");
            L(pMove, "Spawn point", 10, 14);
            m_spawns = new ComboBox { Left = 100, Top = 12, Width = 300, DropDownStyle = ComboBoxStyle.DropDownList }; pMove.Controls.Add(m_spawns);
            B(pMove, "Move there", 410, 11, 100, (s, e) => Move2());
            L(pMove, "Moves the character in the database and tells the server (if they are online they are sent there).", 10, 50).ForeColor = Color.DimGray;
            sub.TabPages.Add(pMove);

            Controls.Add(split);
            Controls.Add(top);
        }

        string SelUser { get { return Cell(m_accounts, "Username") ?? Cell(m_accounts, "USERNAME"); } }
        string SelHandle { get { return Cell(m_chars, "handle"); } }

        public override void OnServerChanged() { m_accounts.DataSource = null; m_chars.DataSource = null; m_spawns.Items.Clear(); }
        public override void OnShown() { if (Server != null && m_spawns.Items.Count == 0) LoadSpawns(); }

        void Find(bool loggedOn)
        {
            if (!NeedServer()) return;
            var s = Server; string q = m_search.Text.Trim();
            Run("Searching accounts", () => loggedOn
                ? Db.Query(s.HubConn, "SELECT TOP 500 * FROM gmAccountBase WHERE LoggedOn <> 0 ORDER BY Username")
                : Db.Query(s.HubConn, "SELECT TOP 500 * FROM gmAccountBase WHERE Username LIKE @u ORDER BY Username", "@u", "%" + q + "%"),
                t => m_accounts.DataSource = t);
            if (m_spawns.Items.Count == 0) LoadSpawns();
        }

        void Create()
        {
            if (!NeedServer()) return;
            string u = m_newUser.Text.Trim(), p = m_newPass.Text;
            if (u.Length < 3 || p.Length < 4) { Host.SetStatus("Username 3+ characters, password 4+", true); return; }
            var s = Server;
            Run("Creating account " + u, () =>
            {
                if (Convert.ToInt32(Db.Scalar(s.HubConn, "SELECT COUNT(*) FROM gmAccountBase WHERE Username = @u", "@u", u)) > 0)
                    throw new Exception("that username already exists");
                InsertAccount(s.HubConn, u, p, 4);
            }, () => { m_search.Text = u; Find(false); });
        }

        void AccountUpdate(string set, object value, string done)
        {
            if (!NeedServer()) return;
            string u = SelUser; if (string.IsNullOrEmpty(u)) { Host.SetStatus("Select an account first", true); return; }
            if (set.StartsWith("Password") && string.IsNullOrEmpty(value as string)) { Host.SetStatus("Type the new password first", true); return; }
            var s = Server;
            Run(done + " for " + u, () => Db.Exec(s.HubConn, "UPDATE gmAccountBase SET " + set + " WHERE Username = @u", "@u", u, "@v", value ?? ""), () => Find(false));
        }

        void Kick()
        {
            if (!NeedServer()) return;
            string u = SelUser; if (string.IsNullOrEmpty(u)) return;
            var s = Server;
            Run("Kicking " + u, () => Db.SendCommand(s, 16, u));
        }

        void Ban(bool ban)
        {
            if (!NeedServer()) return;
            string u = SelUser; if (string.IsNullOrEmpty(u)) { Host.SetStatus("Select an account first", true); return; }
            if (ban && !Confirm("Ban " + u + " and kick them off?")) return;
            var s = Server;
            Run((ban ? "Banning " : "Unbanning ") + u, () =>
            {
                if (ban)
                {
                    if (Convert.ToInt32(Db.Scalar(s.HubConn, "SELECT COUNT(*) FROM GMBANNEDINFO WHERE USERNAME = @u", "@u", u)) == 0)
                        Db.Exec(s.HubConn, "INSERT INTO GMBANNEDINFO (USERNAME) VALUES (@u)", "@u", u);
                    Db.SendCommand(s, 16, u);
                }
                else Db.Exec(s.HubConn, "DELETE FROM GMBANNEDINFO WHERE USERNAME = @u", "@u", u);
            });
        }

        void LoadChars()
        {
            string u = SelUser; if (Server == null || string.IsNullOrEmpty(u)) return;
            var s = Server;
            Run("Reading characters of " + u, () => Db.Query(s.HubConn, "SELECT charName, handle, currentWorldMap, inuse FROM gmCharacters WHERE username = @u", "@u", u), t => m_chars.DataSource = t);
        }

        void LoadCharDetails()
        {
            string h = SelHandle; if (Server == null || string.IsNullOrEmpty(h)) return;
            var s = Server;
            Run("Reading character", () => new[] {
                Db.Query(s.HubConn, "SELECT COALESCE((SELECT TOP 1 itemName FROM GM_GIL WHERE gil = i.itemGilType), (SELECT TOP 1 itemName FROM GM_DYNAMICITEMS WHERE glid = i.itemGilType)) AS Item, " +
                                    "i.quanity AS Qty, CASE WHEN i.storageType = 1 THEN 'bank' ELSE 'on person' END AS [Where], CASE WHEN i.armed = 1 THEN 'yes' ELSE '' END AS Armed, i.dmgMod AS DmgMod, i.itemGilType AS Gil, i.storageType " +
                                    "FROM gmInventory i WHERE i.handle = @h AND i.uniqueidbind = '0' ORDER BY i.storageType, Item", "@h", h),
                Db.Query(s.HubConn, "SELECT (SELECT TOP 1 statName FROM GM_STATS WHERE statID = s.statType) AS Stat, s.currentAmount AS Value, s.statType FROM gmStats s WHERE s.handle = @h", "@h", h),
                Db.Query(s.HubConn, "SELECT (SELECT TOP 1 skillName FROM GM_SKILLS WHERE skillType = k.skillType) AS Skill, k.currentExperience AS Experience, k.skillType FROM gmSkills k WHERE k.handle = @h", "@h", h)
            }, t => { m_inv.DataSource = t[0]; m_stats.DataSource = t[1]; m_skills.DataSource = t[2]; });
        }

        void SearchItems()
        {
            if (!NeedServer()) return;
            var s = Server; string q = "%" + m_itemSearch.Text.Trim() + "%";
            Run("Searching items", () => Db.Query(s.HubConn,
                "SELECT CAST(GIL AS varchar(20)) + '  ' + ITEMNAME AS Label, GIL AS Id FROM GM_GIL WHERE ITEMNAME LIKE @q " +
                "UNION ALL SELECT CAST(glid AS varchar(20)) + '  ' + ITEMNAME, glid FROM gm_DynamicItems WHERE ITEMNAME LIKE @q ORDER BY Label", "@q", q),
                t =>
                {
                    m_itemResults.DataSource = null;
                    m_itemResults.DisplayMember = "Label"; m_itemResults.ValueMember = "Id";
                    m_itemResults.DataSource = t;
                });
        }

        void GiveItem()
        {
            string h = SelHandle; if (!NeedServer() || string.IsNullOrEmpty(h)) { Host.SetStatus("Select a character first", true); return; }
            if (m_itemResults.SelectedValue == null) { Host.SetStatus("Search and pick an item first", true); return; }
            int qty, dmg; if (!int.TryParse(m_qty.Text, out qty) || qty < 1) qty = 1; int.TryParse(m_dmg.Text, out dmg);
            string gil = m_itemResults.SelectedValue.ToString();
            var s = Server;
            Run("Giving item", () => Db.Exec(s.HubConn,
                "INSERT INTO GMINVENTORY (HANDLE, ITEMUNIQUEID, ITEMGILTYPE, UNIQUEIDBIND, CURRENTCHARGES, STORAGETYPE, ARMED, QUANITY, NEWFLAG, MISCINT1, MISCFLOAT1, MISCFLOAT2, DMGMOD) " +
                "VALUES (@h, '999', @g, '0', '0', '1', '0', @q, '1', '0', '0', '0', @d)", "@h", h, "@g", gil, "@q", qty, "@d", dmg), LoadCharDetails);
        }

        void DeleteItem()
        {
            string h = SelHandle, gil = Cell(m_inv, "Gil"), st = Cell(m_inv, "storageType"), name = Cell(m_inv, "Item");
            if (!NeedServer() || string.IsNullOrEmpty(h) || string.IsNullOrEmpty(gil)) { Host.SetStatus("Select an item first", true); return; }
            if (!Confirm("Delete " + name + " from this character?")) return;
            var s = Server;
            Run("Deleting item", () => Db.Exec(s.HubConn, "DELETE TOP (1) FROM gmInventory WHERE handle = @h AND itemGilType = @g AND storageType = @st AND uniqueidbind = '0'", "@h", h, "@g", gil, "@st", st), LoadCharDetails);
        }

        void SetStatOrSkill(bool stat)
        {
            string h = SelHandle; if (!NeedServer() || string.IsNullOrEmpty(h)) return;
            var g = stat ? m_stats : m_skills;
            string type = Cell(g, stat ? "statType" : "skillType");
            string val = (stat ? m_statVal : m_skillVal).Text.Trim();
            int v; if (string.IsNullOrEmpty(type) || !int.TryParse(val, out v)) { Host.SetStatus("Pick a row and type a number", true); return; }
            var s = Server;
            Run(stat ? "Setting stat" : "Setting skill", () => Db.Exec(s.HubConn, stat
                ? "UPDATE gmStats SET currentAmount = @v WHERE handle = @h AND statType = @t"
                : "UPDATE gmSkills SET currentExperience = @v WHERE handle = @h AND skillType = @t", "@v", v, "@h", h, "@t", type), LoadCharDetails);
        }

        void LoadSpawns()
        {
            var s = Server; if (s == null) return;
            Run("Reading spawn points", () => Db.Query(s.HubConn, "SELECT Descriptive FROM locations_SpawnPoints ORDER BY Descriptive"), t =>
            {
                m_spawns.Items.Clear();
                foreach (DataRow r in t.Rows) m_spawns.Items.Add(Db.Str(r, 0));
            });
        }

        void Move2()
        {
            string h = SelHandle, u = SelUser, spot = m_spawns.SelectedItem as string;
            if (!NeedServer() || string.IsNullOrEmpty(h) || string.IsNullOrEmpty(spot)) { Host.SetStatus("Select a character and a spawn point", true); return; }
            var s = Server;
            Run("Moving character to " + spot, () =>
            {
                var p = Db.Query(s.HubConn, "SELECT TOP 1 Worldname, WorldID, PosX, PosY, PosZ FROM locations_SpawnPoints WHERE Descriptive = @d", "@d", spot);
                if (p.Rows.Count == 0) throw new Exception("spawn point not found");
                var r = p.Rows[0];
                Db.Exec(s.HubConn, "UPDATE GMCHARACTERS SET POSX = @x, POSY = @y, POSZ = @z, CURRENTWORLDMAP = @w, currentWorldID = @id WHERE HANDLE = @h",
                    "@x", Db.Str(r, 2), "@y", Db.Str(r, 3), "@z", Db.Str(r, 4), "@w", Db.Str(r, 0), "@id", Db.Str(r, 1), "@h", h);
                Db.SendCommand(s, 6, u, Db.Str(r, 0), Db.Str(r, 2), Db.Str(r, 3), Db.Str(r, 4));
            });
        }
    }
}

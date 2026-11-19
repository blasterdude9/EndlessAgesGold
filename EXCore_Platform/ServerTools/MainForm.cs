// MainForm.cs - window, server picker, tab host, shared UI helpers.
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EAServerTools
{
    static class Program
    {
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Db.LoadConfig(AppDomain.CurrentDomain.BaseDirectory);
            Application.Run(new MainForm());
        }
    }

    // Base for every tab: knows the selected server, runs work off the UI thread,
    // and reports errors in the status bar instead of crashing.
    public class TabBase : UserControl
    {
        public MainForm Host;
        public GameServer Server { get { return Host == null ? null : Host.CurrentServer; } }
        public virtual string Title { get { return "Tab"; } }
        public virtual void OnServerChanged() { }
        public virtual void OnShown() { }

        public TabBase() { Dock = DockStyle.Fill; Font = new Font("Segoe UI", 9f); }

        protected bool NeedServer()
        {
            if (Server != null) return true;
            Host.SetStatus("Pick a server at the top first.", true);
            return false;
        }

        // run fn in the background, then ui(result) on the UI thread
        protected async void Run<T>(string what, Func<T> fn, Action<T> ui)
        {
            Host.SetStatus(what + "...", false);
            try
            {
                T r = await Task.Run(fn);
                ui(r);
                Host.SetStatus(what + " - done", false);
            }
            catch (Exception ex) { Host.SetStatus(what + " FAILED: " + ex.Message, true); }
        }
        protected void Run(string what, Action fn, Action ui = null)
        {
            Run<int>(what, () => { fn(); return 0; }, _ => { if (ui != null) ui(); });
        }

        // tiny layout helpers (absolute positions keep the code short)
        protected Label L(Control p, string text, int x, int y, int w = 0)
        {
            var c = new Label { Text = text, Left = x, Top = y + 3, AutoSize = w == 0 };
            if (w > 0) c.Width = w;
            p.Controls.Add(c); return c;
        }
        protected TextBox T(Control p, int x, int y, int w, string text = "")
        {
            var c = new TextBox { Left = x, Top = y, Width = w, Text = text };
            p.Controls.Add(c); return c;
        }
        protected Button B(Control p, string text, int x, int y, int w, EventHandler click)
        {
            var c = new Button { Text = text, Left = x, Top = y, Width = w, Height = 26 };
            c.Click += click; p.Controls.Add(c); return c;
        }
        protected DataGridView Grid(Control p, int x, int y, int w, int h, AnchorStyles anchor)
        {
            var g = new DataGridView
            {
                Left = x, Top = y, Width = w, Height = h, Anchor = anchor,
                ReadOnly = true, AllowUserToAddRows = false, AllowUserToDeleteRows = false,
                SelectionMode = DataGridViewSelectionMode.FullRowSelect, MultiSelect = false,
                RowHeadersVisible = false, AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.DisplayedCells,
                BackgroundColor = SystemColors.Window
            };
            p.Controls.Add(g); return g;
        }
        protected static string Cell(DataGridView g, string col)
        {
            if (g.CurrentRow == null || !g.Columns.Contains(col)) return null;
            object v = g.CurrentRow.Cells[col].Value;
            return v == null || v == DBNull.Value ? "" : v.ToString().Trim();
        }
        protected bool Confirm(string q)
        {
            return MessageBox.Show(this, q, "Endless Ages Server Tools", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes;
        }
    }

    public class MainForm : Form
    {
        ComboBox m_servers = new ComboBox { DropDownStyle = ComboBoxStyle.DropDownList, Width = 220 };
        TabControl m_tabs = new TabControl { Dock = DockStyle.Fill };
        ToolStripStatusLabel m_status = new ToolStripStatusLabel { Spring = true, TextAlign = ContentAlignment.MiddleLeft };
        List<TabBase> m_pages = new List<TabBase>();
        public GameServer CurrentServer { get { return m_servers.SelectedItem as GameServer; } }

        public MainForm()
        {
            Text = "Endless Ages Server Tools";
            Width = 1180; Height = 780; StartPosition = FormStartPosition.CenterScreen;
            Font = new Font("Segoe UI", 9f);
            MinimumSize = new Size(900, 600);

            var top = new Panel { Dock = DockStyle.Top, Height = 38 };
            top.Controls.Add(new Label { Text = "Server:", Left = 10, Top = 11, AutoSize = true });
            m_servers.Left = 62; m_servers.Top = 7;
            top.Controls.Add(m_servers);
            var reload = new Button { Text = "Reload server list", Left = 292, Top = 6, Width = 130, Height = 26 };
            reload.Click += (s, e) => LoadServers();
            top.Controls.Add(reload);
            top.Controls.Add(new Label { Text = "SQL: " + Db.SqlServer + " / " + Db.MainDatabase + (Db.WindowsLogin ? " (Windows login)" : " (SQL login)"), Left = 440, Top = 11, AutoSize = true, ForeColor = Color.DimGray });

            var strip = new StatusStrip();
            strip.Items.Add(m_status);

            Controls.Add(m_tabs);
            Controls.Add(top);
            Controls.Add(strip);

            Add(new StatusTab());
            Add(new LiveZonesTab());
            Add(new AccountsTab());
            Add(new TablesTab());
            Add(new SettingsTab());
            Add(new ToolsTab());
            Add(new LogsTab());

            m_servers.SelectedIndexChanged += (s, e) => { foreach (var p in m_pages) p.OnServerChanged(); };
            m_tabs.SelectedIndexChanged += (s, e) => { var p = m_tabs.SelectedTab?.Tag as TabBase; if (p != null) p.OnShown(); };
            Shown += (s, e) => LoadServers();
        }

        void Add(TabBase p)
        {
            p.Host = this;
            var tp = new TabPage(p.Title) { Tag = p };
            tp.Controls.Add(p);
            m_tabs.TabPages.Add(tp);
            m_pages.Add(p);
        }

        async void LoadServers()
        {
            SetStatus("Loading servers from tblServers...", false);
            try
            {
                var list = await Task.Run(() => Db.LoadServers());
                m_servers.Items.Clear();
                foreach (var g in list) m_servers.Items.Add(g);
                if (m_servers.Items.Count > 0) m_servers.SelectedIndex = 0;
                SetStatus(list.Count == 0 ? "No servers in tblServers (servermanageip is empty)." : "Servers loaded: " + list.Count, list.Count == 0);
                var p = m_tabs.SelectedTab?.Tag as TabBase; if (p != null) p.OnShown();
            }
            catch (Exception ex)
            {
                SetStatus("Cannot read tblServers from " + Db.SqlServer + "/" + Db.MainDatabase + ": " + ex.Message + "  (check ServerTools.cfg)", true);
            }
        }

        public void SetStatus(string text, bool error)
        {
            if (InvokeRequired) { BeginInvoke(new Action(() => SetStatus(text, error))); return; }
            m_status.Text = DateTime.Now.ToString("HH:mm:ss") + "  " + text;
            m_status.ForeColor = error ? Color.Firebrick : SystemColors.ControlText;
        }
    }
}

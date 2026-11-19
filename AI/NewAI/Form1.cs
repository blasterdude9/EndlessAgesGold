using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Runtime.InteropServices;

namespace NewAI
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		[DllImport("AILinguistics.dll")]
		public static extern string ExternalInit(string datadir);
		[DllImport("AILinguistics.dll")]
		public static extern int ReleaseChatter();
		[DllImport("AILinguistics.dll")]
		public static extern string SimpleTest();
		[DllImport("AILinguistics.dll")]
		public static extern string TalkToBot(string speech);

		public static string ChatWBot(string input)
		{
			return TalkToBot(input);

		}
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.TextBox txt_Input;
		private System.Windows.Forms.TextBox txt_Output;
		private System.ComponentModel.Container components = null;

		public Form1()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
			ExternalInit("C:\\L_Projects\\NewAI\\");

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
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.txt_Input = new System.Windows.Forms.TextBox();
			this.txt_Output = new System.Windows.Forms.TextBox();
			this.button1 = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// txt_Input
			// 
			this.txt_Input.Location = new System.Drawing.Point(0, 192);
			this.txt_Input.Name = "txt_Input";
			this.txt_Input.Size = new System.Drawing.Size(288, 22);
			this.txt_Input.TabIndex = 0;
			this.txt_Input.Text = "Say Hello";
			// 
			// txt_Output
			// 
			this.txt_Output.Location = new System.Drawing.Point(0, 32);
			this.txt_Output.Multiline = true;
			this.txt_Output.Name = "txt_Output";
			this.txt_Output.Size = new System.Drawing.Size(288, 144);
			this.txt_Output.TabIndex = 1;
			this.txt_Output.Text = "";
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(208, 232);
			this.button1.Name = "button1";
			this.button1.TabIndex = 2;
			this.button1.Text = "Talk";
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(6, 15);
			this.ClientSize = new System.Drawing.Size(292, 260);
			this.Controls.Add(this.button1);
			this.Controls.Add(this.txt_Output);
			this.Controls.Add(this.txt_Input);
			this.Name = "Form1";
			this.Text = "Form1";
			this.Load += new System.EventHandler(this.Form1_Load);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new Form1());
		}

		private void Form1_Load(object sender, System.EventArgs e)
		{
		
		}

		private void button1_Click(object sender, System.EventArgs e)
		{
			txt_Output.Text = ChatWBot(txt_Input.Text);
		}

		
	}
}

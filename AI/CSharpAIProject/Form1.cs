using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Runtime.InteropServices;


namespace CSharpAIProject
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TextBox txt_Input;
		private System.Windows.Forms.TextBox txt_Output;
		private System.Windows.Forms.Button btn_Speak;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		[DllImport("AILinguistics.dll")]
		public static extern string ExternalInit(string datapath);
		[DllImport("AILinguistics.dll")]
		public static extern int ReleaseChatter();
		[DllImport("AILinguistics.dll")]
		public static extern string TalkToBot(string speech);


		[DllImport("AXEncryptDLL.dll")]
	    public static extern void ExternalInitX(string key);
		[DllImport("AXEncryptDLL.dll")]
		public static extern string DecryptStringX(string data);
		[DllImport("AXEncryptDLL.dll")]
		public static extern string EncryptStringX(string data);
		[DllImport("AXEncryptDLL.dll")]
		public static extern void LockX();
		[DllImport("AXEncryptDLL.dll")]
		public static extern void UnLockX();
		
		public Form1()
		{

            string key = "gshatdisoangdtsy";
			ExternalInitX(key);

			string testEn = "hello world";
			LockX();
			testEn = EncryptStringX(testEn);
			UnLockX();
			
			LockX();
			testEn = DecryptStringX(testEn);
			UnLockX();

		
			
			ExternalInit("C:\\L_Projects\\CSharpAIProject\\bin\\Debug\\");
			//
			// Required for Windows Form Designer support
			//
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
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );

			ReleaseChatter();
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
			this.btn_Speak = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// txt_Input
			// 
			this.txt_Input.Location = new System.Drawing.Point(16, 176);
			this.txt_Input.Name = "txt_Input";
			this.txt_Input.Size = new System.Drawing.Size(264, 22);
			this.txt_Input.TabIndex = 0;
			this.txt_Input.Text = "textBox1";
			// 
			// txt_Output
			// 
			this.txt_Output.Location = new System.Drawing.Point(16, 16);
			this.txt_Output.Multiline = true;
			this.txt_Output.Name = "txt_Output";
			this.txt_Output.Size = new System.Drawing.Size(264, 80);
			this.txt_Output.TabIndex = 1;
			this.txt_Output.Text = "textBox2";
			// 
			// btn_Speak
			// 
			this.btn_Speak.Location = new System.Drawing.Point(208, 208);
			this.btn_Speak.Name = "btn_Speak";
			this.btn_Speak.TabIndex = 2;
			this.btn_Speak.Text = "Speak!";
			this.btn_Speak.Click += new System.EventHandler(this.btn_Speak_Click);
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(6, 15);
			this.ClientSize = new System.Drawing.Size(292, 260);
			this.Controls.Add(this.btn_Speak);
			this.Controls.Add(this.txt_Output);
			this.Controls.Add(this.txt_Input);
			this.Name = "Form1";
			this.Text = "Form1";
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
		[DllImport("AILinguistics.dll")]
		public static extern string SimpleTest();
		//public static extern int puts(string c);

		private void btn_Speak_Click(object sender, System.EventArgs e)
		{
			string input = txt_Input.Text;
			txt_Output.Text = TalkToBot(input);

			string test = SimpleTest();
			//char [] testoutput = "testing 123";
			//string test = SimpleTest();
			//string test = ExternalInit("hello");
		    
			//puts("hi");
			//extern string ExternalInit(string speech);
			//public static extern int puts(string c);
			//internal static extern int _flushall();

		}

	}
}

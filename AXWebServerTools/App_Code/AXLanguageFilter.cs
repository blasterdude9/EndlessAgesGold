using System;
using System.Runtime.InteropServices;

	/// <summary>
	/// Summary description for AXencrypt.
	/// </summary>
	public class AXLanguageFilter : System.Web.UI.Page
	{
  
		[DllImport("LanguageFilterDLL.dll")]private static extern void ExternalInitLanguageFilter(string path);
		[DllImport("LanguageFilterDLL.dll")]private static extern void LanguageLock();
		[DllImport("LanguageFilterDLL.dll")]private static extern void LanguageUnlock();
		[DllImport("LanguageFilterDLL.dll")]private static extern string LanguageProcess(string data,char [] buffer);
		[DllImport("LanguageFilterDLL.dll")]private static extern string AXGetLastError(char [] buffer);

        string m_initError = "";

		public AXLanguageFilter()
		{
			ExternalInitLanguageFilter(Server.MapPath("") + "\\bin\\filter.wwx");
			m_initError = AXGetLError();
		}

		public string Filter(string inputString)
		{
			LanguageLock();
			char [] buffer = new char[1024];
			string ec = LanguageProcess(inputString,buffer);
			LanguageUnlock();
			return ec;
		}

		public string AXGetLError()
		{
			LanguageLock();
			char [] buffer = new char[1024];
			string ec = AXGetLastError(buffer);
			LanguageUnlock();
			return ec;
		}
	}

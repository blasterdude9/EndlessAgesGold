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
using System.Web.Mail;
using System.Xml.XPath;
using System.Text;
using System.Xml;
using System.Net;
using System.IO;

	/// <summary>
	/// Summary description for Advertiser.
	/// </summary>
	public class Advertiser
	{
		DatabaseConnectionModule dbConnect = new DatabaseConnectionModule();

		public Advertiser()
		{
			//
			// TODO: Add constructor logic here
			//
		}

		public string GetBanner(string cat, SqlConnection connection)
		{
			string url = "";
			string image = "";
			string lastshown = "";
			string isactive = "";
			string remimpressions = "";
			int endimpressions = 0;
			string endingid = "";
			int curid = 0;
			int total = 0;

			SqlDataReader GetLastShown = dbConnect.sqlQuery("SELECT lastshown FROM adv_Sequence",connection);
			if(GetLastShown.Read())
			{
				lastshown = GetLastShown.GetSqlValue(0).ToString();
				curid = Convert.ToInt32(lastshown)+1;					
			}
			GetLastShown.Close();

			SqlDataReader CheckID = dbConnect.sqlQuery("SELECT COUNT(idi) as cnt FROM adv_Banners where active = '1'",connection);
			if(CheckID.Read())
			{
				total = Convert.ToInt32(CheckID.GetSqlValue(0).ToString());
			}
			CheckID.Close();

			if(curid > total)
				curid = 0;

			while(curid < total+1)
			{
				SqlDataReader GetImage = dbConnect.sqlQuery("SELECT image,url,active,remimpressions,idi,loadingimg,basicimg,vendimg,siteimg,category FROM adv_Banners where idi = '"+curid+"'",connection);
				if(GetImage.Read())
				{
					isactive = GetImage.GetSqlValue(2).ToString();
					remimpressions = GetImage.GetSqlValue(3).ToString();
			
					if(isactive == "1" && remimpressions != "0")
					{
						if(string.Compare(cat,"1")==0)
							image = GetImage.GetSqlValue(0).ToString();
						if(string.Compare(cat,"loadingimg")==0)
						   image = GetImage.GetSqlValue(5).ToString();
						else if(string.Compare(cat,"basicimg")==0)
						   image = GetImage.GetSqlValue(6).ToString();
						else if(string.Compare(cat,"vendimg")==0)
							image = GetImage.GetSqlValue(7).ToString();
						else if(string.Compare(cat,"siteimg")==0)
							image = GetImage.GetSqlValue(8).ToString();
						
						url = GetImage.GetSqlValue(1).ToString();
						endingid = GetImage.GetSqlValue(4).ToString();
						GetImage.Close();
						break;
					}
				}
				GetImage.Close();
				curid++;
				
			}
			endimpressions = Convert.ToInt32(remimpressions)-1;
			if(image != null)
			{

				if(url != "None")
				{
					if(endimpressions != 0)
					{					
						dbConnect.ExecuteQuery("UPDATE adv_Banners set remimpressions = '"+endimpressions.ToString()+"' where idi = '"+endingid+"'",connection);
						dbConnect.ExecuteQuery("UPDATE adv_Sequence set lastshown = '"+endingid+"' where catid = '"+cat+"'",connection); 
				
					}
					else
					{
						dbConnect.ExecuteQuery("UPDATE adv_Banners set remimpressions = '"+endimpressions.ToString()+"', active = '0' where idi = '"+endingid+"'",connection); 
						dbConnect.ExecuteQuery("UPDATE adv_Sequence set lastshown = '"+endingid+"' where catid = '"+cat+"'",connection);			
					}

					string banner = "<a href='"+url+"'><img src='"+image+"'></a>";
					return banner;
				}
				else
				{
					if(endimpressions != 0)
					{					
						dbConnect.ExecuteQuery("UPDATE adv_Banners set remimpressions = '"+endimpressions.ToString()+"' where idi = '"+endingid+"'",connection);
						dbConnect.ExecuteQuery("UPDATE adv_Sequence set lastshown = '"+endingid+"'",connection); 
				
					}
					else
					{
						dbConnect.ExecuteQuery("UPDATE adv_Banners set remimpressions = '"+endimpressions.ToString()+"', active = '0' where idi = '"+endingid+"'",connection); 
						dbConnect.ExecuteQuery("UPDATE adv_Sequence set lastshown = '"+endingid+"'",connection);			
					}
					

					return image;
				}

				

			}
			return null;

		}

		
	}

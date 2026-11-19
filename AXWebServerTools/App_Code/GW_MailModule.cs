using System;
using System.Web.Mail;


	/// <summary>
	/// Summary description for GW_MailModule.
	/// </summary>
	public class GW_MailModule
	{
		public GW_MailModule()
		{
			//
			// TODO: Add constructor logic here
			//
		}

		public void SendMail(string targetEmail,string mailmsg,string subject)
		{
			//System.Web.Mail.SmtpMail.SmtpServer = "smtp-server";//"mail.endlessagesportal.com";
			//System.Web.Mail.SmtpMail.SmtpServer = "mail.endlessagesportal.com";
			try
			{
				MailMessage myMail = new MailMessage();
				myMail.From = "avaria@endlessages.com";
				myMail.To = targetEmail;
				myMail.Subject = subject;
				myMail.Priority = MailPriority.Normal;
				myMail.BodyFormat = MailFormat.Html;
				myMail.Body = "<HTML><body>" + mailmsg + "</body></HTML>";
				SmtpMail.Send(myMail);
			}
			catch
			{

			}
		}
		//Send Mail String
		public void SendMail(string targetEmail,string mailmsg,string subject,string replyMail)
		{
			//System.Web.Mail.SmtpMail.SmtpServer = "smtp-server";//"mail.endlessagesportal.com";
			//System.Web.Mail.SmtpMail.SmtpServer = "mail.endlessagesportal.com";
			try
			{
				MailMessage myMail = new MailMessage();
				myMail.From = replyMail;
				myMail.To = targetEmail;
				myMail.Subject = subject;
				myMail.Priority = MailPriority.Normal;
				myMail.BodyFormat = MailFormat.Html;
				myMail.Body = "<HTML><body>" + mailmsg + "</body></HTML>";
				SmtpMail.Send(myMail);
			}
			catch
			{

			}
		}
	}

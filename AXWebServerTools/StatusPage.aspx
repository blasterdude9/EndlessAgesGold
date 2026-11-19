<%@ Register TagPrefix="uc1" TagName="ServerStatusControl" Src="Controls\ServerStatusControl.ascx" %>
<%@ Page language="c#" CodeFile="StatusPage.aspx.cs" AutoEventWireup="true" Inherits="StatusPage" %>
<%@ Register TagPrefix="uc1" TagName="NavControl" Src="Controls\NavControl.ascx" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" >
<HTML>
	<HEAD>
		<title>StatusPage</title>
	</HEAD>
	<body MS_POSITIONING="GridLayout">
		<form id="Form1" method="post" runat="server">
			<TABLE id="Table1" style="Z-INDEX: 101; LEFT: 8px; POSITION: absolute; TOP: 8px" cellSpacing="1"
				cellPadding="1" width="300" border="0" bgColor="black">
				<TR>
					<TD>
						<uc1:NavControl id="NavControl1" runat="server"></uc1:NavControl></TD>
				</TR>
				<TR>
					<TD>
						<uc1:ServerStatusControl id="ServerStatusControl1" runat="server"></uc1:ServerStatusControl></TD>
				</TR>
			</TABLE>
			&nbsp;
		</form>
	</body>
</HTML>

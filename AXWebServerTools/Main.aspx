<%@ Page language="c#" CodeFile="Main.aspx.cs" AutoEventWireup="true" Inherits="WebForm1" %>
<%@ Register TagPrefix="uc1" TagName="NavControl" Src="Controls\NavControl.ascx" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" >
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <title>WebForm1</title>
</head>
<body>
    <form id="Form1" method="post" runat="server">
        <uc1:NavControl ID="NavControl1" runat="server"></uc1:NavControl>
    </form>
</body>
</html>

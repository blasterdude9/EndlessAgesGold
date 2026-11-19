<%@ Page language="c#" CodeFile="MainSvrSettingsPage.aspx.cs" AutoEventWireup="false" Inherits="MainSvrSettingsPage" %>
<%@ Register TagPrefix="uc1" TagName="ServerSettings" Src="Controls\ServerSettings.ascx" %>
<%@ Register TagPrefix="uc1" TagName="NavControl" Src="Controls\NavControl.ascx" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" >
<html>
<head>
    <title>MainSvrSettingsPage</title>
</head>
<body ms_positioning="GridLayout">
    <form id="Form1" method="post" runat="server">
        <table id="Table1" style="z-index: 101; left: 8px; position: absolute; top: 8px"
            cellspacing="1" cellpadding="1" width="300" bgcolor="black" border="0">
            <tr>
                <td>
                    <uc1:NavControl ID="NavControl1" runat="server"></uc1:NavControl>
                </td>
            </tr>
            <tr>
                <td>
                    <uc1:ServerSettings ID="ServerSettings1" runat="server"></uc1:ServerSettings>
                </td>
            </tr>
        </table>
    </form>
</body>
</html>

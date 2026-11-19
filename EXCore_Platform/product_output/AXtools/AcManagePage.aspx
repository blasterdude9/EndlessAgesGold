<%@ page language="c#" autoeventwireup="true" inherits="AcManagePage, App_Web_ivhlxkco" %>
<%@ Register TagPrefix="uc1" TagName="NavControl" Src="Controls\NavControl.ascx" %>
<%@ Register TagPrefix="uc1" TagName="AccountManager" Src="Controls\AccountManager.ascx" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" >
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <title>AcManagePage</title>
</head>
<body>
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
                    <uc1:AccountManager ID="AccountManager1" runat="server"></uc1:AccountManager>
                </td>
            </tr>
        </table>
    </form>
</body>
</html>

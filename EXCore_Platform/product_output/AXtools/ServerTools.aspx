<%@ Register TagPrefix="uc1" TagName="ServerToolsControl" Src="Controls\ServerToolsControl.ascx" %>
<%@ page language="c#" autoeventwireup="true" inherits="ServerTools, App_Web_ivhlxkco" %>
<%@ Register TagPrefix="uc1" TagName="NavControl" Src="Controls\NavControl.ascx" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" >
<HTML>
<head>
    <title>ServerTools</title>
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
                    <uc1:ServerToolsControl ID="ServerToolsControl1" runat="server"></uc1:ServerToolsControl>
                </td>
            </tr>
        </table>
    </form>
</body>
</html>

<%@ page language="c#" autoeventwireup="true" inherits="IGN_QuestMenuPage, App_Web_ivhlxkco" %>

<%@ Register Src="Controls\IGN_QuestMenuControl.ascx" TagName="IGN_QuestMenuControl" TagPrefix="uc2" %>
<%@ Register TagPrefix="uc1" TagName="NavControl" Src="Controls\NavControl.ascx" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" >
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <title>IGN_QuestMenuPage</title>
    <link rel="stylesheet" type="text/css" href="style.css" />
</head>
<body>
    <form id="Form1" method="post" runat="server">
        <table id="Table1" style="z-index: 101; left: 8px; position: absolute; top: 8px"
            cellspacing="1" cellpadding="1" width="300" bgcolor="black" border="0">
            <tr>
                <td>
                    <uc1:NavControl ID="NavControl1" runat="server"></uc1:NavControl>
                    <uc2:IGN_QuestMenuControl ID="IGN_QuestMenuControl1" runat="server" />
                </td>
            </tr>
            <tr>
                <td>
                   </td>
            </tr>
        </table>
    </form>
</body>
</html>

<%@ Page language="c#" CodeFile="GlobalWorldManagerPage.aspx.cs" AutoEventWireup="true" Inherits="GlobalWorldManagerPage" %>
<%@ Register TagPrefix="uc1" TagName="NavControl" Src="Controls\NavControl.ascx" %>
<%@ Register TagPrefix="uc1" TagName="GlobalWorldManager" Src="Controls\GlobalWorldManager.ascx" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" >
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <title>GlobalWorldManagerPage</title>
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
                    <uc1:GlobalWorldManager ID="GlobalWorldManager1" runat="server"></uc1:GlobalWorldManager>
                </td>
            </tr>
        </table>
    </form>
</body>
</html>

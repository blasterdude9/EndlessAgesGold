<%@ Register TagPrefix="uc1" TagName="LogViewerControl" Src="Controls\LogViewerControl.ascx"%>
<%@ Page language="c#" CodeFile="LogPage.aspx.cs" AutoEventWireup="true" Inherits="LogPage" %>
<%@ Register TagPrefix="uc1" TagName="NavControl" Src="Controls\NavControl.ascx" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" >
<html  xmlns="http://www.w3.org/1999/xhtml">
<head>
    <title>LogPage</title>
</head>
<body>
    <form id="Form1" method="post" runat="server">
        <table id="Table1" style="z-index: 101; left: 8px; position: absolute; top: 8px"
            cellspacing="1" cellpadding="1" width="300" border="0" bgcolor="black">
            <tr>
                <td>
                    <uc1:NavControl ID="NavControl1" runat="server"></uc1:NavControl>
                </td>
            </tr>
            <tr>
                <td>
                    <uc1:LogViewerControl id="LogViewerControl1" runat="server">
                    </uc1:LogViewerControl></td>
            </tr>
        </table>
        &nbsp;
    </form>
</body>
</html>

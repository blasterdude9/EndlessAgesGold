<%@ page language="C#" autoeventwireup="true" inherits="StencilZoneManager, App_Web_ivhlxkco" %>

<%@ Register Src="Controls/NavControl.ascx" TagName="NavControl" TagPrefix="uc1" %>
<%@ Register Src="Controls/WorldStencilManager.ascx" TagName="WorldStencilManager"
    TagPrefix="uc2" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>Untitled Page</title>
</head>
<body>
    <form id="form1" runat="server">
    <div>
        <table id="Table1" bgcolor="black" border="0" cellpadding="1" cellspacing="1" style="z-index: 101;
            left: 8px; position: absolute; top: 8px" width="300">
            <tr>
                <td>
                    <uc1:NavControl ID="NavControl1" runat="server" />
                </td>
            </tr>
            <tr>
                <td>
                    <uc2:WorldStencilManager ID="WorldStencilManager1" runat="server" />
                </td>
            </tr>
        </table>
    </div>
    </form>
</body>
</html>

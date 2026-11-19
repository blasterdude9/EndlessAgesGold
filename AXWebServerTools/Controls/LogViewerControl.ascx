<%@ Control Language="c#" AutoEventWireup="true" CodeFile="LogViewerControl.ascx.cs" Inherits="LogViewerControl"%>
<table id="Table2" style="width: 648px; height: 225px" cellspacing="1" cellpadding="1"
    width="648" border="0" bgcolor="gray">
    <tr>
        <td>
            <asp:Label ID="m_logtitle" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                ForeColor="WhiteSmoke"></asp:Label></td>
    </tr>
    <tr>
        <td>
            <asp:TextBox ID="m_logCountToShow" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                Height="23px" Width="48px">10</asp:TextBox>
            <asp:Button ID="m_refreshBtn" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                Height="23px" Text="Refresh"></asp:Button></td>
    </tr>
    <tr>
        <td>
            <div align="center">
                <table id="Table1" style="width: 626px; height: 88px" cellspacing="1" cellpadding="1"
                    width="626" border="0" align="center">
                    <tr>
                        <td valign="top" align="right" width="300" bgcolor="#99cc99" height="150">
                            <asp:Label ID="m_statusOutput" runat="server" Font-Names="Arial" Font-Size="XX-Small"></asp:Label></td>
                        <td valign="top" align="left" width="1000" bgcolor="#99cc99" height="150">
                            <asp:Label ID="m_statusOutputCol2" runat="server" Font-Names="Arial" Font-Size="XX-Small"></asp:Label></td>
                    </tr>
                </table>
            </div>
        </td>
    </tr>
</table>

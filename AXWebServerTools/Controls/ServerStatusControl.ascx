<%@ Control Language="c#" AutoEventWireup="true" CodeFile="ServerStatusControl.ascx.cs" Inherits="ServerStatusControl"%>
<table id="Table2" cellspacing="1" cellpadding="1" width="848" bgcolor="gray" border="0"
    style="width: 848px; height: 203px">
    <tr>
        <td>
            <asp:Label ID="m_onlineNodes" runat="server" Font-Bold="True" Font-Names="Arial"
                Font-Size="XX-Small" ForeColor="Lime"></asp:Label></td>
    </tr>
    <tr>
        <td>
            <table id="Table1" style="width: 840px; height: 152px" cellspacing="1" cellpadding="1"
                width="840" border="0">
                <tr>
                    <td valign="top" align="right" width="444" bgcolor="#99cc99" height="150" style="width: 444px">
                        <asp:Label ID="m_statusOutput" Font-Size="XX-Small" Font-Names="Arial" runat="server"></asp:Label></td>
                    <td valign="top" align="left" width="400" bgcolor="#99cc99" height="150">
                        <asp:Label ID="m_row2StatusOut" Font-Size="XX-Small" Font-Names="Arial" runat="server"></asp:Label></td>
                </tr>
            </table>
        </td>
    </tr>
</table>

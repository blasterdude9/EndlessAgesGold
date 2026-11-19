<%@ control language="C#" autoeventwireup="true" inherits="LiveZonesStatus, App_Web_ojrkv221" %>
<table id="Table1" border="0" cellpadding="1" cellspacing="1" style="width: 416px;
    height: 352px" width="416">
    <tr>
        <td bgcolor="#808080" style="height: 1px">
            <font color="#ffffff" face="Arial" size="2"><span style="color: lime">All Zones Status (inlcudes all nodes
                connected to this HUB)</span></font></td>
    </tr>
    <tr>
        <td bgcolor="#808080" style="height: 1px">
            <asp:Label ID="m_onlineNodes" runat="server" Font-Bold="True" Font-Names="Arial"
                Font-Size="XX-Small" ForeColor="Lime"></asp:Label></td>
    </tr>
    <tr>
        <td align="left" bgcolor="#99cc99" style="height: 4px" valign="top">
        </td>
    </tr>
    <tr>
        <td align="left" bgcolor="#99cc99" style="height: 129px" valign="top">
            <p align="center">
                <asp:ListBox ID="m_zonesStatusList" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                    Height="501px" Width="560px"></asp:ListBox></p>
        </td>
    </tr>
    <tr>
        <td align="left" bgcolor="#99cc99" style="height: 2px" valign="top">
        </td>
    </tr>
</table>

<%@ control language="c#" autoeventwireup="true" inherits="GlobalWorldManager, App_Web_ojrkv221" %>
<table id="Table1" style="width: 416px; height: 352px" cellspacing="1" cellpadding="1"
    width="416" border="0">
    <tr>
        <td style="height: 1px" bgcolor="#808080">
            <font face="Arial" color="#ffffff" size="2"><strong>Global World Manager</strong></font></td>
    </tr>
    <tr>
        <td style="height: 4px" valign="top" align="left" bgcolor="#99cc99">
        </td>
    </tr>
    <tr>
        <td style="height: 95px" valign="top" align="left" bgcolor="#99cc99">
            <p align="center">
                <asp:ListBox ID="m_aiWorldsList" runat="server" Width="382px" Font-Names="Arial"
                    Font-Size="XX-Small" Height="192px"></asp:ListBox></p>
        </td>
    </tr>
    <tr>
        <td style="height: 171px" bgcolor="#99cc99">
            <table id="Table2" style="width: 336px; height: 107px" cellspacing="1" cellpadding="1"
                width="336" border="0">
                <tr>
                    <td style="width: 106px">
                        <p align="right">
                            <font face="Arial" size="1">World Name</font></p>
                    </td>
                    <td>
                    </td>
                    <td>
                        <asp:TextBox ID="m_worldFile" runat="server" Width="216px" Font-Names="Arial" Font-Size="XX-Small"
                            Height="22px"></asp:TextBox></td>
                </tr>
                <tr>
                    <td style="width: 106px">
                        <p align="right">
                            <font face="Arial" size="1">World Index</font></p>
                    </td>
                    <td>
                    </td>
                    <td>
                        <asp:TextBox ID="m_worldIndex" runat="server" Width="56px" Font-Names="Arial" Font-Size="XX-Small"
                            Height="22px"></asp:TextBox></td>
                </tr>
                <tr>
                    <td style="width: 106px">
                        <p align="right">
                            <font face="Arial" size="1">World Channel</font></p>
                    </td>
                    <td>
                    </td>
                    <td>
                        <asp:TextBox ID="m_worldChannel" runat="server" Width="56px" Font-Names="Arial" Font-Size="XX-Small"
                            Height="22px"></asp:TextBox></td>
                </tr>
                <tr>
                    <td style="width: 106px">
                        <p align="right">
                            &nbsp;</p>
                    </td>
                    <td>
                    </td>
                    <td>
                    </td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td bgcolor="#808080">
            <asp:Button ID="AddBtn" runat="server" Width="80px" Font-Names="Arial" Font-Size="XX-Small"
                Text="Add" BorderStyle="Solid"></asp:Button>
            <asp:Button ID="ReplaceBtn" runat="server" Width="80px" Font-Names="Arial" Font-Size="XX-Small"
                Text="Replace" BorderStyle="Solid"></asp:Button>
            <asp:Button ID="DeleteBtn" runat="server" Width="80px" Font-Names="Arial" Font-Size="XX-Small"
                Text="Delete" BorderStyle="Solid"></asp:Button>
            <asp:Button ID="AquireBtn" runat="server" Width="80px" Font-Names="Arial" Font-Size="XX-Small"
                Text="Aquire" BorderStyle="Solid"></asp:Button></td>
    </tr>
</table>

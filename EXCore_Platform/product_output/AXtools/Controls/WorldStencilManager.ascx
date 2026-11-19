<%@ control language="C#" autoeventwireup="true" inherits="Controls_WorldStencilManager, App_Web_ojrkv221" %>
&nbsp;<asp:ScriptManager ID="ScriptManager1" runat="server">
</asp:ScriptManager>
<asp:UpdatePanel ID="UpdatePanel1" runat="server">
    <ContentTemplate>
<table id="Table1" border="0" cellpadding="1" cellspacing="1" style="font-size: x-small;
    width: 416px; font-family: Arial, Verdana; height: 352px" width="416">
    <tr>
        <td bgcolor="#808080" style="height: 1px">
            <font color="#ffffff" face="Arial" size="2"><strong>Zone Designer</strong></font></td>
    </tr>
    <tr>
        <td align="left" bgcolor="#99cc99" style="height: 4px" valign="top">
        </td>
    </tr>
    <tr>
        <td align="left" bgcolor="#99cc99" style="height: 4px" valign="top">
            Worlds-stencils that instances are based on</td>
    </tr>
    <tr>
        <td align="left" bgcolor="#99cc99" style="height: 95px" valign="top">
            <p align="center">
                <asp:ListBox ID="m_stencilWorldsList" runat="server"
                    Height="194px" OnSelectedIndexChanged="m_stencilWorldsList_SelectedIndexChanged"
                    Width="410px" AutoPostBack="True"></asp:ListBox></p>
        </td>
    </tr>
    <tr>
        <td align="left" bgcolor="#99cc99" valign="top">
            <table id="Table2" border="0" cellpadding="1" cellspacing="1" style="font-size: 8pt;
                width: 365px; font-family: Arial, Verdana; height: 107px">
                <tr>
                    <td style="width: 106px">
                        <p align="right">
                            EXG Base Name</p>
                    </td>
                    <td>
                    </td>
                    <td>
                        <asp:TextBox ID="m_worldEXG" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                            Height="15px" Width="215px"></asp:TextBox></td>
                </tr>
                <tr>
                    <td style="width: 106px">
                        <p align="right">
                            AI EXG</p>
                    </td>
                    <td>
                    </td>
                    <td>
                        <asp:TextBox ID="m_aiEXG" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                            Height="15px" Width="215px"></asp:TextBox></td>
                </tr>
                <tr>
                    <td style="width: 106px">
                        <p align="right">
                            World Channel ID</p>
                    </td>
                    <td>
                    </td>
                    <td>
                        <asp:TextBox ID="m_worldChannel" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                            Height="15px" Width="48px"></asp:TextBox></td>
                </tr>
                <tr>
                    <td style="width: 106px">
                        <p align="right">
                            Max Connections</p>
                    </td>
                    <td>
                    </td>
                    <td>
                        <asp:TextBox ID="m_maxConnections" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                            Height="15px" Width="48px"></asp:TextBox></td>
                </tr>
                <tr>
                    <td style="width: 106px; height: 21px">
                    </td>
                    <td style="height: 21px">
                    </td>
                    <td style="height: 21px">
                        <asp:Button ID="AddBtn" runat="server" BorderStyle="Solid" Font-Names="Arial" Font-Size="XX-Small"
                            Text="Add" Width="80px" OnClick="AddBtn_Click" />
                        <asp:Button ID="ReplaceBtn" runat="server" BorderStyle="Solid" Font-Names="Arial"
                            Font-Size="XX-Small" Text="Replace" Width="80px" OnClick="ReplaceBtn_Click" />
                        <asp:Button ID="DeleteBtn" runat="server" BorderStyle="Solid" Font-Names="Arial"
                            Font-Size="XX-Small" Text="Delete" Width="80px" OnClick="DeleteBtn_Click" /></td>
                </tr>
            </table>
            <asp:Label ID="m_errorOutput" runat="server" Font-Bold="True" ForeColor="Red"></asp:Label><br />
        </td>
    </tr>
    <tr>
        <td align="left" bgcolor="#99cc99" valign="top">
            Rules per world stencil</td>
    </tr>
    <tr>
        <td align="left" bgcolor="#99cc99" style="height: 95px" valign="top">
            <asp:ListBox ID="m_worldRuleList" runat="server" Height="91px" Width="409px" AutoPostBack="True" OnSelectedIndexChanged="m_worldRuleList_SelectedIndexChanged"></asp:ListBox></td>
    </tr>
    <tr>
        <td bgcolor="#99cc99" style="height: 93px">
            <table style="font-size: xx-small; width: 405px; font-family: Arial, Verdana">
                <tr>
                    <td style="width: 107px; text-align: right; height: 21px;">
                        World Rule</td>
                    <td style="height: 21px">
                        <asp:DropDownList ID="m_worldRuleDropList" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                            Width="200px" OnSelectedIndexChanged="m_worldRuleDropList_SelectedIndexChanged" AutoPostBack="True">
                        </asp:DropDownList></td>
                </tr>
                <tr>
                    <td style="width: 107px; text-align: right">
                        <asp:Label ID="m_param1Desc" runat="server" Text="N/A"></asp:Label></td>
                    <td>
                        <asp:TextBox ID="m_param1" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                            Height="15px" Width="48px"></asp:TextBox></td>
                </tr>
                <tr>
                    <td style="width: 107px">
                    </td>
                    <td>
                        <asp:Button ID="AddWorldRuleBtn" runat="server" BorderStyle="Solid" Font-Names="Arial" Font-Size="XX-Small"
                            Text="Add" Width="80px" OnClick="AddWorldRuleBtn_Click" />&nbsp;<asp:Button ID="replaceWorldRulesBtn"
                                runat="server" BorderStyle="Solid" Font-Names="Arial" Font-Size="XX-Small" ForeColor="Black"
                                OnClick="replaceWorldRulesBtn_Click" Text="Replace" Width="80px" />
                        <asp:Button ID="DeleteWorldRuleBtn" runat="server" BorderStyle="Solid" Font-Names="Arial" Font-Size="XX-Small"
                            Text="Delete" Width="80px" OnClick="DeleteWorldRuleBtn_Click" /></td>
                </tr>
                <tr>
                    <td style="width: 107px">
                    </td>
                    <td>
                    </td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td bgcolor="#808080">
            &nbsp;&nbsp;
        </td>
    </tr>
</table>
    </ContentTemplate>
</asp:UpdatePanel>

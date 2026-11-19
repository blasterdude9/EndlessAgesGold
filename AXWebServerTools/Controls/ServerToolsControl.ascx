<%@ Control Language="c#" AutoEventWireup="true" CodeFile="ServerToolsControl.ascx.cs" Inherits="ServerToolsControl"%>
<table id="Table1" style="width: 300px; height: 504px" cellspacing="1" cellpadding="1"
    width="300" bgcolor="#808080" border="0">
    <tr>
        <td style="height: 3px" valign="top">
            <font face="Arial" color="#ffffff" size="2"><strong>Server Tools</strong></font></td>
    </tr>
    <tr>
        <td style="height: 3px" valign="top">
        </td>
    </tr>
    <tr>
        <td valign="top" style="height: 182px">
            <table id="Table2" style="width: 144px; height: 84px" cellspacing="1" cellpadding="1"
                width="144" border="0">
                <tr>
                    <td>
                        <asp:Button ID="RestartSystemBtn" Text="Restart System" runat="server" Font-Names="Arial"
                            Font-Size="XX-Small" BorderStyle="Solid"></asp:Button></td>
                </tr>
                <tr>
                    <td>
                        <asp:Button ID="adminModeBtn" BorderStyle="Solid" Font-Size="XX-Small" Font-Names="Arial"
                            runat="server" Text="Admin Mode Toggle" Width="136px"></asp:Button></td>
                </tr>
                <tr>
                    <td>
                        <asp:Button ID="ManualSaveBtn" BorderStyle="Solid" Font-Size="XX-Small" Font-Names="Arial"
                            runat="server" Text="Manual Save" Width="136px"></asp:Button></td>
                </tr>
                <tr>
                    <td>
                        <asp:Button ID="UpdateStatusTableBtn" BorderStyle="Solid" Font-Size="XX-Small" Font-Names="Arial"
                            runat="server" Text="Update SQL Status" Width="136px"></asp:Button></td>
                </tr>
                <tr>
                    <td>
                        <asp:Button ID="DisconnectAllBtn" BorderStyle="Solid" Font-Size="XX-Small" Font-Names="Arial"
                            runat="server" Text="Disconnect All" Width="136px"></asp:Button></td>
                </tr>
                <tr>
                    <td>
                    </td>
                </tr>
            </table>
            <asp:Label ID="m_liveStatusOutput" Font-Size="XX-Small" Font-Names="Arial" runat="server"></asp:Label></td>
    </tr>
    <tr>
        <td valign="top" align="left">
            <span style="font-size: 9pt; color: #ffffff; font-family: Arial"><strong>Test Account
                Tools<table style="font-size: xx-small; width: 267px; color: white; font-family: Arial, Verdana;
                    height: 114px">
                    <tr>
                        <td style="width: 130px; height: 5px">
                            Account Stencil Username</td>
                        <td style="height: 5px">
                            <asp:TextBox ID="m_accountUsrStencil" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                Width="119px"></asp:TextBox></td>
                    </tr>
                    <tr>
                        <td style="width: 130px; height: 6px">
                            New Username Base</td>
                        <td style="height: 6px">
                            <asp:TextBox ID="m_newUsernameBase" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                Width="119px"></asp:TextBox></td>
                    </tr>
                    <tr>
                        <td style="width: 130px; height: 4px">
                            New Password Base</td>
                        <td style="height: 4px">
                            <asp:TextBox ID="m_newPassBase" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                Width="119px"></asp:TextBox></td>
                    </tr>
                    <tr>
                        <td style="width: 130px; height: 5px">
                            Account Quanity</td>
                        <td style="height: 5px">
                            <asp:TextBox ID="m_genQuanity" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                Width="54px"></asp:TextBox></td>
                    </tr>
                    <tr>
                        <td style="width: 130px">
                            Admin Code</td>
                        <td>
                            <asp:TextBox ID="m_adminCode" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                Width="54px"></asp:TextBox></td>
                    </tr>
                    <tr>
                        <td style="width: 130px">
                        </td>
                        <td style="text-align: right">
                            <asp:Button ID="GenerateTestAccountsBtn" runat="server" BorderStyle="Solid" Font-Bold="True"
                                Font-Names="Arial" Font-Size="X-Small" ForeColor="Maroon" OnClick="GenerateTestAccountsBtn_Click"
                                Text="Generate" /></td>
                    </tr>
                </table>
            </strong></span>
        </td>
    </tr>
    <tr>
        <td valign="bottom">
            <asp:Label ID="m_editNameSectionLabel" runat="server" Font-Names="Arial" Font-Size="XX-Small"></asp:Label></td>
    </tr>
</table>

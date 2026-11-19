<%@ control language="c#" autoeventwireup="true" inherits="ServerSettings, App_Web_ojrkv221" %>
<table id="Table1" style="width: 440px; height: 216px" cellspacing="1" cellpadding="1"
    width="440" border="0">
    <tr>
        <td style="height: 9px" bgcolor="#808080">
            <font face="Arial" size="2" color="white"><strong>Main Server Settings</strong></font></td>
    </tr>
    <tr>
        <td valign="top" align="left" bgcolor="#99cc99">
            <table id="Table2" style="width: 400px; height: 76px; font-size: xx-small; font-family: Arial, Verdana;" cellspacing="1" cellpadding="1"
                width="400" border="0">
                <tr>
                    <td style="width: 188px; height: 24px">
                    </td>
                    <td style="height: 24px">
                    </td>
                    <td style="height: 24px">
                    </td>
                </tr>
                <tr>
                    <td style="width: 188px; height: 24px">
                        <p align="right">
                            <font face="Arial" size="1">Version</font></p>
                    </td>
                    <td style="height: 24px">
                    </td>
                    <td style="height: 24px">
                        <asp:TextBox ID="m_version" runat="server" Width="130px" Font-Names="Arial" Font-Size="XX-Small"
                            Height="20px"></asp:TextBox><font face="Arial" size="1"></font></td>
                </tr>
                <tr>
                    <td style="width: 188px">
                        <p align="right">
                            <font face="Arial" size="1">Max Player Limit</font></p>
                    </td>
                    <td>
                    </td>
                    <td>
                        <font face="Arial" size="1">
                            <asp:TextBox ID="m_maxPlayerLimit" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                Width="130px" Height="20px"></asp:TextBox></font></td>
                </tr>
                <tr>
                    <td style="width: 188px">
                        <p align="right">
                            <font face="Arial" size="1">Gameplay Turn speed</font></p>
                    </td>
                    <td>
                    </td>
                    <td>
                        <font face="Arial" size="1">
                            <asp:TextBox ID="m_gameplayTurnSpeed" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                Width="130px" Height="20px"></asp:TextBox></font></td>
                </tr>
                <tr>
                    <td style="width: 188px">
                        <p align="right">
                            <font face="Arial" size="1">Dead PC Dissolve Time</font></p>
                    </td>
                    <td>
                    </td>
                    <td>
                        <font face="Arial" size="1">
                            <asp:TextBox ID="m_deadPCDissolveTime" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                Width="130px" Height="20px"></asp:TextBox></font></td>
                </tr>
                <tr>
                    <td style="width: 188px; height: 6px">
                        <p align="right">
                            <font face="Arial" size="1">Dead NPC Dissolve Time</font></p>
                    </td>
                    <td style="height: 6px">
                    </td>
                    <td style="height: 6px">
                        <font face="Arial" size="1">
                            <asp:TextBox ID="m_deadNPCDissolveTime" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                Width="130px" Height="20px"></asp:TextBox></font></td>
                </tr>
                <tr>
                    <td style="width: 188px">
                        <p align="right">
                            <font face="Arial" size="1">AI server turn speed</font></p>
                    </td>
                    <td>
                    </td>
                    <td>
                        <asp:TextBox ID="m_aiServerTurnSpeed" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                            Width="130px" Height="20px"></asp:TextBox><font face="Arial" size="1"></font></td>
                </tr>
                <tr>
                    <td style="width: 188px">
                        <p align="right">
                            <font face="Arial" size="1">Murder level count</font></p>
                    </td>
                    <td>
                    </td>
                    <td>
                        <asp:TextBox ID="m_murderLevelCount" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                            Width="130px" Height="20px"></asp:TextBox><font face="Arial" size="1"></font></td>
                </tr>
                <tr>
                    <td style="width: 188px">
                        <p align="right">
                            <font face="Arial" size="1">Morder work off Rate</font></p>
                    </td>
                    <td>
                    </td>
                    <td>
                        <asp:TextBox ID="m_murderWorkOffRate" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                            Width="130px" Height="20px"></asp:TextBox><font face="Arial" size="1"></font></td>
                </tr>
                <tr>
                    <td style="width: 188px">
                        <p align="right">
                            <font face="Arial" size="1">Duration between backups</font></p>
                    </td>
                    <td>
                    </td>
                    <td>
                        <asp:TextBox ID="m_durationBetweenBackups" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                            Width="130px" Height="20px"></asp:TextBox><font face="Arial" size="1"></font></td>
                </tr>
                <tr>
                    <td style="width: 188px">
                        <p align="right">
                            <font face="Arial" size="1">Main Server IP</font></p>
                    </td>
                    <td>
                        <font face="Arial" size="1"></font>
                    </td>
                    <td>
                        <asp:TextBox ID="m_mainServerIP" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                            Width="130px" Height="20px"></asp:TextBox><font face="Arial" size="1"></font></td>
                </tr>
                <tr>
                    <td style="width: 188px">
                        <p align="right">
                            <font face="Arial" size="1">AI Server Username</font></p>
                    </td>
                    <td>
                        <font face="Arial" size="1"></font>
                    </td>
                    <td>
                        <asp:TextBox ID="m_aiServerUsername" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                            Width="130px" Height="20px"></asp:TextBox><font face="Arial" size="1"></font></td>
                </tr>
                <tr>
                    <td style="width: 188px">
                        <p align="right">
                            <font face="Arial" size="1">AI Server Password</font></p>
                    </td>
                    <td>
                        <font face="Arial" size="1"></font>
                    </td>
                    <td>
                        <asp:TextBox ID="m_aiServerPassord" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                            Width="130px" Height="20px"></asp:TextBox><font face="Arial" size="1"></font></td>
                </tr>
                <tr>
                    <td style="width: 188px; text-align: right;">
                        <font face="Arial" size="1">Log Priority</font></td>
                    <td>
                        <font face="Arial" size="1"></font>
                    </td>
                    <td>
                        <asp:TextBox ID="m_logPriority" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                            Height="20px" Width="130px"></asp:TextBox></td>
                </tr>
                <tr>
                    <td style="width: 188px; text-align: right">
                        Regen Turn Speed</td>
                    <td>
                    </td>
                    <td>
                        <asp:TextBox ID="m_regenTurnSpeed" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                            Height="20px" Width="130px"></asp:TextBox></td>
                </tr>
                <tr>
                    <td style="width: 188px; text-align: right">
                        Default Health Regen Amount</td>
                    <td>
                    </td>
                    <td>
                        <asp:TextBox ID="m_defaultHealthRegenAmount" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                            Height="20px" Width="130px"></asp:TextBox></td>
                </tr>
                <tr>
                    <td style="width: 188px; text-align: right">
                        Loot Mode</td>
                    <td>
                    </td>
                    <td>
                        <asp:TextBox ID="m_lootMode" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                            Height="20px" Width="130px"></asp:TextBox></td>
                </tr>
                <tr>
                    <td style="width: 188px; text-align: right">
                        Aggressor Max Track</td>
                    <td>
                    </td>
                    <td>
                        <asp:TextBox ID="m_aggressorMaxTrack" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                            Height="20px" Width="130px"></asp:TextBox></td>
                </tr>
                <tr>
                    <td style="width: 188px; text-align: right">
                        Max Damage over time Track</td>
                    <td>
                    </td>
                    <td>
                        <asp:TextBox ID="m_maxDotTrack" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                            Height="20px" Width="130px"></asp:TextBox></td>
                </tr>
                <tr>
                    <td style="width: 188px; text-align: right">
                        Damage Mode</td>
                    <td>
                    </td>
                    <td>
                        <asp:TextBox ID="m_damageMode" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                            Height="20px" Width="130px"></asp:TextBox></td>
                </tr>
                <tr>
                    <td style="width: 188px; height: 15px; text-align: right">
                        Respawn Health Percentage</td>
                    <td style="height: 15px">
                    </td>
                    <td style="height: 15px">
                        <asp:TextBox ID="m_respawnHealthPercentage" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                            Height="20px" Width="130px" Wrap="False"></asp:TextBox></td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td bgcolor="#808080">
            <asp:Button ID="ApplyBtn" Text="Apply (Will take effect on reset)" runat="server"
                Font-Names="Arial" Font-Size="XX-Small" BorderStyle="Solid" Width="216px" OnClick="ApplyBtn_Click1"></asp:Button></td>
    </tr>
</table>

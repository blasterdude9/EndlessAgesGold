<%@ control language="c#" autoeventwireup="true" inherits="AccountManager, App_Web_ojrkv221" %>

&nbsp;<asp:ScriptManager ID="ScriptManager1" runat="server">
</asp:ScriptManager>
<asp:UpdatePanel ID="UpdatePanel1" runat="server">
    <ContentTemplate>

<table id="Table1" style="width: 520px; height: 904px" cellspacing="1" cellpadding="1"
    width="520" bgcolor="#99cc99" border="0">
    <tr>
        <td style="width: 212px; height: 2px">
            <font face="Arial Black" color="black" size="2">Account Manager</font></td>
    </tr>
    <tr>
        <td style="width: 212px; height: 56px" valign="top" align="left">
            <table id="Table9" style="width: 496px; height: 87px" cellspacing="0" cellpadding="0"
                width="496" border="0">
                <tr>
                    <td style="width: 228px; height: 69px" valign="top">
                        <table id="Table2" style="width: 176px; height: 68px" cellspacing="1" cellpadding="1"
                            width="176" bgcolor="gray" border="0">
                            <tr>
                                <td>
                                    <asp:TextBox ID="m_username" runat="server" Width="96px"></asp:TextBox><font color="#ffffff">&nbsp;<font
                                        face="Arial" size="1">Username</font></font></td>
                            </tr>
                            <tr>
                                <td align="right">
                                    <asp:Button ID="AquireAcBtn" runat="server" Text="Aquire" BorderStyle="Solid" Font-Names="Arial"
                                        Font-Size="XX-Small" ToolTip="Gets info by Account Username." OnClick="AquireAcBtn_Click1"></asp:Button></td>
                            </tr>
                        </table>
                    </td>
                    <td style="height: 69px" valign="top" align="right">
                        &nbsp;
                        <table id="Table10" cellspacing="0" cellpadding="0" width="300" border="0">
                            <tr>
                                <td align="right">
                                    <asp:Label ID="m_generalStatistics" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        ForeColor="Transparent" Font-Bold="True"></asp:Label></td>
                            </tr>
                            <tr>
                                <td>
                                    <asp:CheckBox ID="m_showStatPanel" runat="server" Text="Show Statistics Panel" Font-Names="Arial"
                                        Font-Size="XX-Small" AutoPostBack="True"></asp:CheckBox></td>
                            </tr>
                            <tr>
                                <td>
                                    <asp:CheckBox ID="m_showInventoryPanel" runat="server" Text="Show Inventory Panel"
                                        Font-Names="Arial" Font-Size="XX-Small" AutoPostBack="True"></asp:CheckBox></td>
                            </tr>
                            <tr>
                                <td>
                                    <asp:CheckBox ID="m_showBankPanel" runat="server" Text="Show Bank Panel" Font-Names="Arial"
                                        Font-Size="XX-Small" AutoPostBack="True"></asp:CheckBox></td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td style="width: 212px; height: 1px" valign="top" align="left">
            <asp:Label ID="m_currentPlayerLabel" runat="server" Font-Size="XX-Small" Font-Names="Arial"
                Font-Bold="True" ForeColor="Black" Visible="False">Current Players</asp:Label></td>
    </tr>
    <tr>
        <td style="width: 212px" valign="top" align="left">
            <asp:Panel ID="Panel1" runat="server" Width="360px" Height="64px">
               <font face="Arial" size="2"><strong>Character Managment</strong></font>
                    <table id="Table6" cellspacing="2" cellpadding="2" width="300" border="2">
                        <tr>
                            <td valign="top">
                                <asp:ListBox ID="m_currentPlayersList" Width="176px" runat="server" Font-Size="XX-Small"
                                    Font-Names="Arial" Height="84px"></asp:ListBox>
                                <asp:Button ID="GetPlayerInfoBtn" Width="104px" runat="server" ToolTip="Gets Players Information/Inventorys"
                                    Font-Size="XX-Small" Font-Names="Arial" BorderStyle="Solid" Text="Get Info"></asp:Button></td>
                            <td valign="top">
                                    <table id="Table4" style="width: 304px; height: 64px" cellspacing="1" cellpadding="1"
                                        width="304" border="0">
                                        <tr>
                                            <td>
                                                <asp:Button ID="DisconnectBtn" Width="96px" runat="server" Font-Size="XX-Small" Font-Names="Arial"
                                                    Text="Disconnect" Height="20px"></asp:Button></td>
                                            <td>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td>
                                                <p>
                                                    <asp:Button ID="m_RepositionBtn" Width="96px" runat="server" Font-Size="XX-Small"
                                                        Font-Names="Arial" Text="Reposition"></asp:Button></p>
                                            </td>
                                            <td>
                                                <asp:DropDownList ID="m_reposLocations" Width="195px" runat="server" Font-Size="XX-Small"
                                                    Font-Names="Arial" Height="40px">
                                                </asp:DropDownList></td>
                                        </tr>
                                        <tr>
                                            <td>
                                                <p>
                                                    <asp:Button ID="BanToggleBtn" Width="96px" runat="server" Font-Size="XX-Small" Font-Names="Arial"
                                                        Text="Ban Toggle"></asp:Button></p>
                                            </td>
                                            <td>
                                                <asp:Label ID="m_worldLink" runat="server" Font-Size="XX-Small" Font-Names="Arial"></asp:Label></td>
                                        </tr>
                                        <tr>
                                            <td>
                                                <p>
                                                    <asp:Button ID="GMToggleBtn" Width="96px" runat="server" Font-Size="XX-Small" Font-Names="Arial"
                                                        Text="GM Toggle"></asp:Button></p>
                                            </td>
                                            <td>
                                                <asp:Label ID="m_position" runat="server" Font-Size="XX-Small" Font-Names="Arial"></asp:Label></td>
                                        </tr>
                                        <tr>
                                            <td>
                                            </td>
                                            <td>
                                            </td>
                                        </tr>
                                    </table>
                            </td>
                        </tr>
                    </table>
                </asp:Panel>
                <asp:Panel ID="m_statPanel" runat="server" Width="432px" Height="150px">
                        <font face="Arial" size="2"><strong>Statistics/Skills Management</strong></font>
                    <table id="Table7" cellspacing="2" cellpadding="2" width="300" border="2">
                        <tr>
                            <td>
                                <font size="2">Statistics</font></td>
                            <td>
                                <font size="2">Skills</font></td>
                        </tr>
                        <tr>
                            <td>
                                <asp:ListBox ID="m_statList" runat="server" Width="184px" Font-Names="Arial" Font-Size="XX-Small"
                                    Height="84px"></asp:ListBox></td>
                            <td>
                                <p>
                                    <asp:ListBox ID="m_skillList" runat="server" Width="232px" Font-Names="Arial" Font-Size="XX-Small"
                                        Height="84px"></asp:ListBox></p>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <asp:TextBox ID="m_newStatValue" runat="server" Width="72px" Font-Names="Arial" Font-Size="XX-Small"></asp:TextBox>
                                <asp:Button ID="SetStatBtn" runat="server" Width="72px" Text="Set Stat" Font-Names="Arial"
                                    Font-Size="XX-Small"></asp:Button></td>
                            <td>
                                <asp:TextBox ID="m_newSkillVal" runat="server" Width="72px" Font-Names="Arial" Font-Size="XX-Small"></asp:TextBox>
                                <asp:Button ID="SetSkillBtn" runat="server" Width="72px" Text="Set Skill" Font-Names="Arial"
                                    Font-Size="XX-Small"></asp:Button></td>
                        </tr>
                    </table>
                </asp:Panel>
                <asp:Panel ID="m_charInvPanel" runat="server" Width="480px">
                    <p>
                        <font face="Arial" size="2"><strong>On Character Inventory</strong></font></p>
                            <table id="Table8" cellspacing="2" cellpadding="2" width="300" border="2">
                                <tr>
                                    <td>
                                        <asp:ListBox ID="m_currentDataList" Width="232px" runat="server" Font-Size="XX-Small"
                                            Font-Names="Arial" Height="170px"></asp:ListBox></td>
                                    <td valign="top">
                                        <asp:Label ID="m_editNameSectionLabel" runat="server" Font-Size="XX-Small" Font-Names="Arial"
                                            Font-Bold="True"></asp:Label>
                                        <p>
                                            <asp:Label ID="m_generalStatistics2Label" runat="server" Font-Size="XX-Small" Font-Names="Arial"
                                                ForeColor="Transparent"></asp:Label></p>
                                    </td>
                                </tr>
                            </table>
                </asp:Panel>
                <asp:Panel ID="m_bankPanel" runat="server" Width="365px" Height="74px">
                    <font face="Arial" size="2"><strong>Bank Inventory Panel</strong></font>
                        <table id="Table5" style="width: 477px; height: 164px" cellspacing="2" cellpadding="2"
                            width="477" border="2">
                            <tr>
                                <td>
                                    <asp:ListBox ID="m_currentBankInvenList" Width="232px" runat="server" Font-Size="XX-Small"
                                        Font-Names="Arial" Height="216px"></asp:ListBox></td>
                                <td>
                                    <table id="Table3" style="width: 216px; height: 84px" cellspacing="1" cellpadding="1"
                                        width="216" border="0">
                                        <tr>
                                            <td style="height: 24px">
                                                <asp:TextBox ID="m_searchWord" Width="96px" runat="server" Font-Size="XX-Small" Font-Names="Arial"
                                                    Height="20px"></asp:TextBox>
                                                <asp:Button ID="RefreshInvBtn" Width="86px" runat="server" Font-Size="6pt" Font-Names="Arial"
                                                    Text="Search" Height="22px" BorderColor="Red"></asp:Button></td>
                                        </tr>
                                        <tr>
                                            <td style="height: 12px" align="right">
                                                <asp:DropDownList ID="m_itemDataList" Width="214px" runat="server" Font-Size="XX-Small"
                                                    Font-Names="Arial">
                                                </asp:DropDownList></td>
                                        </tr>
                                        <tr>
                                            <td align="right">
                                                <font face="Arial" size="1">Damage Mod</font>
                                                <asp:TextBox ID="m_dmgMod" Width="50px" runat="server" Font-Size="XX-Small" Font-Names="Arial"
                                                    Height="20px"></asp:TextBox></td>
                                        </tr>
                                        <tr>
                                            <td align="right">
                                                <asp:TextBox ID="m_addItemQuanity" Width="41px" runat="server" ToolTip="Quanity"
                                                    Font-Size="XX-Small" Font-Names="Arial">1</asp:TextBox>
                                                <asp:Button ID="AddItemBtn" Width="112px" runat="server" ToolTip="Add Item To Bank Inventory"
                                                    Font-Size="XX-Small" Font-Names="Arial" BorderStyle="Solid" Text="Add Bank Item">
                                                </asp:Button></td>
                                        </tr>
                                        <tr>
                                            <td style="height: 49px">
                                                <p>
                                                    <asp:Label ID="EditDataLabel" runat="server" Font-Size="XX-Small" Font-Names="Arial"
                                                        Font-Bold="True" ForeColor="Black" Visible="False"></asp:Label></p>
                                                <p>
                                                    <asp:Label ID="m_currentBankInvenLabel" runat="server" Font-Size="XX-Small" Font-Names="Arial"
                                                        Font-Bold="True" ForeColor="Black" Visible="False"></asp:Label></p>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td>
                                                <asp:Button ID="DeleteItemBtn" Width="112px" runat="server" ToolTip="Delete item from bank inventory"
                                                    Font-Size="XX-Small" Font-Names="Arial" BorderStyle="Solid" Text="Delete Bank Item">
                                                </asp:Button></td>
                                        </tr>
                                        <tr>
                                            <td>
                                                <asp:Button ID="SetBankCreditsBtn" Width="112px" runat="server" ToolTip="Sets the Credits in the bank inventory."
                                                    Font-Size="XX-Small" Font-Names="Arial" BorderStyle="Solid" Text="Set Bank Credits">
                                                </asp:Button>&nbsp;
                                                <asp:TextBox ID="m_bankCreditsEdit" Width="41px" runat="server" ToolTip="Quanity"
                                                    Font-Size="XX-Small" Font-Names="Arial">1</asp:TextBox></td>
                                        </tr>
                                    </table>
                                    <asp:Label ID="ItemErrorOutput" runat="server" Font-Size="XX-Small" Font-Names="Arial"></asp:Label></td>
                            </tr>
                        </table>
                </asp:Panel>
        </td>
    </tr>
    <tr>
        <td style="width: 212px; height: 5px" valign="top" align="left">
            <asp:Label ID="m_currentDataDisplay" runat="server" Font-Size="XX-Small" Font-Names="Arial"
                Font-Bold="True" ForeColor="Black" Visible="False"></asp:Label></td>
    </tr>
</table>
    </ContentTemplate>
</asp:UpdatePanel>

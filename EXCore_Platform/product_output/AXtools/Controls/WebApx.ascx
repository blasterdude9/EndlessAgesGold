<%@ control language="C#" autoeventwireup="true" inherits="Controls_WebApx, App_Web_ojrkv221" %>
<table id="Table1" border="0" cellpadding="1" cellspacing="1" style="width: 496px;
    height: 216px">
    <tr>
        <td bgcolor="#808080" style="height: 9px">
            <font color="white" face="Arial" size="2"><strong>Web APX</strong></font></td>
    </tr>
    <tr>
        <td align="left" bgcolor="#99cc99" style="height: 495px" valign="top">
            <table style="vertical-align: top; width: 486px; height: 1px">
                <tr>
                    <td style="width: 92px; height: 182px">
                        &nbsp;<table style="font-size: xx-small; vertical-align: top; width: 207px; font-family: Arial;
                            height: 575px">
                            <tr>
                                <td>
                                    Web Created Items</td>
                            </tr>
                            <tr>
                                <td>
                                    <asp:ListBox ID="m_itemList" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Height="594px" Width="229px" OnSelectedIndexChanged="m_itemList_SelectedIndexChanged" AutoPostBack="True"></asp:ListBox></td>
                            </tr>
                            <tr>
                                <td style="text-align: center">
                                    <asp:Button ID="addBtn" runat="server" Font-Names="Arial" Font-Size="XX-Small" OnClick="addBtn_Click"
                                        Text="Add" Width="65px" />
                                    <asp:Button ID="replaceBtn" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        OnClick="replaceBtn_Click" Text="Replace" Width="65px" />
                                    <asp:Button ID="DeleteBtn" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        OnClick="DeleteBtn_Click" Text="Delete" Width="65px" /></td>
                            </tr>
                            <tr>
                                <td style="text-align: center; height: 90px;">
                                    <asp:Button ID="m_updateLive" runat="server" Font-Bold="True" Font-Names="Arial"
                                        Font-Size="XX-Small" ForeColor="Red" OnClick="m_updateLive_Click" Text="Update Live"
                                        Width="163px" /></td>
                            </tr>
                            <tr>
                                <td style="height: 156px">
                                    <asp:Label ID="m_errorOutput" runat="server" Font-Bold="True" Font-Names="Arial" Font-Size="X-Small"
                                        ForeColor="Red"></asp:Label></td>
                            </tr>
                            <tr>
                                <td style="height: 110px">
                                </td>
                            </tr>
                            <tr>
                                <td>
                                </td>
                            </tr>
                            <tr>
                                <td style="height: 18px">
                                </td>
                            </tr>
                            <tr>
                                <td style="height: 18px">
                                </td>
                            </tr>
                            <tr>
                                <td style="height: 18px">
                                </td>
                            </tr>
                            <tr>
                                <td>
                                </td>
                            </tr>
                            <tr>
                                <td>
                                </td>
                            </tr>
                            <tr>
                                <td>
                                </td>
                            </tr>
                            <tr>
                                <td style="height: 18px">
                                </td>
                            </tr>
                            <tr>
                                <td>
                                </td>
                            </tr>
                            <tr>
                                <td>
                                </td>
                            </tr>
                            <tr>
                                <td style="height: 18px">
                                </td>
                            </tr>
                            <tr>
                                <td style="height: 18px">
                                    <br />
                                    <br />
                                    <br />
                                    <br />
                                    &nbsp;<br />
                                    <br />
                                    <br />
                                    <br />
                                    <br />
                                    <br />
                                    <br />
                                    <br />
                                    <br />
                                    <br />
                                    &nbsp;</td>
                            </tr>
                        </table>
                    </td>
                    <td style="width: 138px; height: 182px">
                        <table cellspacing="0" style="font-size: xx-small; width: 330px; font-family: Arial">
                            <tr>
                                <td style="width: 80px; height: 14px">
                                    <asp:TextBox ID="m_itemName" runat="server" Width="150px" Font-Size="XX-Small"></asp:TextBox></td>
                                <td style="width: 177px; height: 14px">
                                    Item Name</td>
                            </tr>
                            <tr>
                                <td style="width: 80px">
                                    <asp:TextBox ID="m_marketCost" runat="server" Width="150px" Font-Size="XX-Small">0</asp:TextBox></td>
                                <td style="width: 177px">
                                    Market Cost</td>
                            </tr>
                            <tr>
                                <td style="width: 80px">
                                    <asp:TextBox ID="m_currentCharges" runat="server" Width="150px" Font-Size="XX-Small">0</asp:TextBox></td>
                                <td style="width: 177px">
                                    Current Charges</td>
                            </tr>
                            <tr>
                                <td style="width: 80px">
                                    <asp:TextBox ID="m_maxCharges" runat="server" Width="150px" Font-Size="XX-Small">0</asp:TextBox></td>
                                <td style="width: 177px">
                                    Max Charges</td>
                            </tr>
                            <tr>
                                <td style="width: 80px">
                                    <asp:TextBox ID="m_chargeType" runat="server" Width="150px" Font-Size="XX-Small">0</asp:TextBox></td>
                                <td style="width: 177px">
                                    Charge Type</td>
                            </tr>
                            <tr>
                                <td style="width: 80px">
                                    <asp:TextBox ID="m_shotsPerFire" runat="server" Width="150px" Font-Size="XX-Small">0</asp:TextBox></td>
                                <td style="width: 177px">
                                    Shots Per Fire</td>
                            </tr>
                            <tr>
                                <td style="width: 80px">
                                    <asp:TextBox ID="m_ammoRegenRate" runat="server" Width="150px" Font-Size="XX-Small">0</asp:TextBox></td>
                                <td style="width: 177px">
                                    Ammo Regen Rate</td>
                            </tr>
                            <tr>
                                <td style="width: 80px">
                                    <asp:TextBox ID="m_secondsPerRegen" runat="server" Width="150px" Font-Size="XX-Small">0</asp:TextBox></td>
                                <td style="width: 177px">
                                    Seconds Per Regen</td>
                            </tr>
                            <tr>
                                <td style="width: 80px; height: 18px">
                                    <asp:TextBox ID="m_locationID" runat="server" Width="150px" Font-Size="XX-Small">0</asp:TextBox></td>
                                <td style="width: 177px; height: 18px">
                                    Location ID</td>
                            </tr>
                            <tr>
                                <td style="width: 80px; height: 18px">
                                    <asp:TextBox ID="m_animationPref" runat="server" Width="150px" Font-Size="XX-Small">0</asp:TextBox></td>
                                <td style="width: 177px; height: 18px">
                                    Animation Preference</td>
                            </tr>
                            <tr>
                                <td style="width: 80px; height: 18px">
                                    <asp:TextBox ID="m_animationDefault" runat="server" Width="150px" Font-Size="XX-Small">0</asp:TextBox></td>
                                <td style="width: 177px; height: 18px">
                                    Animation Default</td>
                            </tr>
                            <tr>
                                <td style="width: 80px; height: 18px">
                                    <asp:TextBox ID="m_visualRef" runat="server" Font-Size="XX-Small" Width="150px">0</asp:TextBox></td>
                                <td style="width: 177px; height: 18px">
                                    Visual Ref (Icon ID)</td>
                            </tr>
                            <tr>
                                <td style="width: 80px; height: 18px">
                                    <asp:TextBox ID="m_requiredDex" runat="server" Font-Size="XX-Small" Width="150px">0</asp:TextBox></td>
                                <td style="width: 177px; height: 18px">
                                    Required DEX</td>
                            </tr>
                            <tr>
                                <td style="width: 80px; height: 18px">
                                    <asp:TextBox ID="m_requiredStr" runat="server" Font-Size="XX-Small" Width="150px">0</asp:TextBox></td>
                                <td style="width: 177px; height: 18px">
                                    Required STR</td>
                            </tr>
                            <tr>
                                <td style="width: 80px; height: 18px">
                                    <asp:TextBox ID="m_requiredWiz" runat="server" Font-Size="XX-Small" Width="150px">0</asp:TextBox></td>
                                <td style="width: 177px; height: 18px">
                                    Required WIZ</td>
                            </tr>
                            <tr>
                                <td style="width: 80px; height: 18px">
                                    <asp:TextBox ID="m_hitPoints" runat="server" Font-Size="XX-Small" Width="150px">0</asp:TextBox></td>
                                <td style="width: 177px; height: 18px">
                                    Hit Points (0=na 1=broke)</td>
                            </tr>
                            <tr>
                                <td style="width: 80px; height: 18px">
                                    <asp:TextBox ID="m_fireQueIndexOvrd" runat="server" Font-Size="XX-Small" Width="150px">0</asp:TextBox></td>
                                <td style="width: 177px; height: 18px">
                                    Fire Que Index Override</td>
                            </tr>
                            <tr>
                                <td style="width: 80px; height: 18px">
                                    <asp:TextBox ID="m_damageOvrd" runat="server" Font-Size="XX-Small" Width="150px">0</asp:TextBox></td>
                                <td style="width: 177px; height: 18px">
                                    Damage Override</td>
                            </tr>
                            <tr>
                                <td style="width: 80px; height: 18px">
                                    <asp:DropDownList ID="m_damageElementOvrd" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Width="161px">
                                    </asp:DropDownList></td>
                                <td style="width: 177px; height: 18px">
                                    Damage Element Override</td>
                            </tr>
                            <tr>
                                <td style="width: 80px; height: 18px">
                                    <asp:DropDownList ID="m_useTypeList" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Width="161px">
                                    </asp:DropDownList>&nbsp;</td>
                                <td style="width: 177px; height: 18px">
                                    Use Type</td>
                            </tr>
                            <tr>
                                <td style="width: 80px">
                                    <asp:DropDownList ID="m_requireSkillList" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Width="161px">
                                    </asp:DropDownList></td>
                                <td style="width: 177px">
                                    Required Skill</td>
                            </tr>
                            <tr>
                                <td style="width: 80px; text-align: left">
                                    <asp:TextBox ID="m_requiredSkillLevel" runat="server" Width="61px" Font-Names="Arial" Font-Size="XX-Small">0</asp:TextBox>&nbsp;</td>
                                <td style="width: 177px">
                                    Required Skill Level</td>
                            </tr>
                            <tr>
                                <td style="width: 80px">
                                </td>
                                <td style="width: 177px">
                                </td>
                            </tr>
                            <tr>
                                <td style="width: 80px">
                                </td>
                                <td style="width: 177px">
                                    <asp:CheckBox ID="m_destroyOnBreak" runat="server" Text="Destroy On Break" /></td>
                            </tr>
                            <tr>
                                <td style="width: 80px">
                                </td>
                                <td style="width: 177px">
                                    <asp:CheckBox ID="m_hideInInventory" runat="server" Text="Hide in Inventory" /></td>
                            </tr>
                            <tr>
                                <td style="width: 80px">
                                </td>
                                <td style="width: 177px">
                                    <asp:CheckBox ID="m_nonDrop" runat="server" Text="Non-Drop" /></td>
                            </tr>
                            <tr>
                                <td style="width: 80px">
                                </td>
                                <td style="width: 177px">
                                    <asp:CheckBox ID="m_stackable" runat="server" Text="Stackable" Checked="True" /></td>
                            </tr>
                            <tr>
                                <td style="width: 80px">
                                </td>
                                <td style="width: 177px">
                                    <asp:CheckBox ID="m_nonTrade" runat="server" Text="Non-Trade" /></td>
                            </tr>
                            <tr>
                                <td style="width: 80px; height: 18px">
                                    <asp:TextBox ID="m_glid" runat="server" Enabled="False" Font-Names="Arial" Font-Size="XX-Small"
                                        Width="76px" Visible="False">0</asp:TextBox></td>
                                <td style="width: 177px; height: 18px">
                                    <asp:CheckBox ID="m_destroyOnUse" runat="server" Text="Destroy On Use" /></td>
                            </tr>
                            <tr>
                                <td style="width: 80px; height: 18px">
                                </td>
                                <td style="width: 177px; height: 18px">
                                </td>
                            </tr>
                        </table>
                        <table style="font-size: xx-small; width: 330px; font-family: Arial">
                            <tr>
                                <td>
                                    Attachments</td>
                            </tr>
                            <tr>
                                <td>
                                    <asp:ListBox ID="m_attachments" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Height="99px" Width="320px" AutoPostBack="True" OnSelectedIndexChanged="m_attachments_SelectedIndexChanged"></asp:ListBox></td>
                            </tr>
                            <tr>
                                <td style="height: 24px">
                                    <asp:DropDownList ID="m_availVisualList" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Width="212px">
                                    </asp:DropDownList>
                                    Avail Visuals</td>
                            </tr>
                            <tr>
                                <td>
                                    <asp:DropDownList ID="m_raceSpecificList" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Width="212px">
                                    </asp:DropDownList>
                                    Race Specific</td>
                            </tr>
                            <tr>
                                <td>
                                    <asp:TextBox ID="m_dimIndex" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Width="61px">-1</asp:TextBox>
                                    Dim Index</td>
                            </tr>
                            <tr>
                                <td>
                                    <asp:TextBox ID="m_dimChange" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Width="61px">-1</asp:TextBox>
                                    Dim Change Option</td>
                            </tr>
                            <tr>
                                <td>
                                    <asp:TextBox ID="m_fireQue_DPathOvrd" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Width="61px" Visible="False">0</asp:TextBox></td>
                            </tr>
                            <tr>
                                <td>
                                    <asp:TextBox ID="m_visualSearchEdit" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Width="187px"></asp:TextBox>&nbsp;<asp:Button ID="SearchVisualBtn" runat="server"
                                            Font-Names="Arial" Font-Size="XX-Small" OnClick="SearchVisualBtn_Click" Text="Advanced Search"
                                            Width="118px" /></td>
                            </tr>
                            <tr>
                                <td>
                                    &nbsp;<asp:DropDownList ID="m_catagorieSearch" runat="server"
                                        Font-Names="Arial" Font-Size="XX-Small" OnSelectedIndexChanged="m_catagorieSearch_SelectedIndexChanged"
                                        Width="182px" AutoPostBack="True">
                                    </asp:DropDownList>
                                    Catagory search</td>
                            </tr>
                            <tr>
                                <td>
                                    &nbsp;<asp:Button ID="AddAttachmentBtn" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        OnClick="AddAttachmentBtn_Click" Text="Add" Width="65px" />
                                    <asp:Button ID="ReplaceAttachmentBtn" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        OnClick="ReplaceAttachmentBtn_Click" Text="Replace" Width="65px" />
                                    <asp:Button ID="deleteAttachmentBtn"
                                            runat="server" Font-Names="Arial" Font-Size="XX-Small" OnClick="deleteAttachmentBtn_Click"
                                            Text="Delete" Width="65px" /></td>
                            </tr>
                            <tr>
                                <td>
                                </td>
                            </tr>
                            <tr>
                                <td>
                                    Defense Attributes</td>
                            </tr>
                            <tr>
                                <td>
                                    <asp:ListBox ID="m_defenseAttributesList" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Height="99px" Width="320px"></asp:ListBox></td>
                            </tr>
                            <tr>
                                <td>
                                    <asp:DropDownList ID="m_availDefenseList" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Width="212px">
                                    </asp:DropDownList>
                                    Avail Defense</td>
                            </tr>
                            <tr>
                                <td>
                                    <asp:TextBox ID="m_defensePercentage" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Width="61px"></asp:TextBox>
                                    Defense Percentage</td>
                            </tr>
                            <tr>
                                <td>
                                    &nbsp;<asp:Button ID="addDefenseBtn" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Text="Add" Width="65px" OnClick="addDefenseBtn_Click" /><asp:Button ID="DeleteDefenseBtn" runat="server" Font-Names="Arial"
                                            Font-Size="XX-Small" Text="Delete" Width="65px" OnClick="DeleteDefenseBtn_Click" /></td>
                            </tr>
                            <tr>
                                <td>
                                </td>
                            </tr>
                            <tr>
                                <td>
                                    Stat Bonus</td>
                            </tr>
                            <tr>
                                <td>
                                    <asp:ListBox ID="m_statBonusList" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Height="99px" Width="320px"></asp:ListBox></td>
                            </tr>
                            <tr>
                                <td>
                                    <asp:DropDownList ID="m_availStatsList" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Width="212px">
                                    </asp:DropDownList>
                                    Avail Stats</td>
                            </tr>
                            <tr>
                                <td>
                                    <asp:TextBox ID="m_statValue" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Width="61px"></asp:TextBox>
                                    Stat value</td>
                            </tr>
                            <tr>
                                <td>
                                    &nbsp;<asp:Button ID="addStatBtn" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Text="Add" Width="65px" OnClick="addStatBtn_Click" /><asp:Button ID="DeleteStatBtn" runat="server" Font-Names="Arial"
                                            Font-Size="XX-Small" Text="Delete" Width="65px" OnClick="DeleteStatBtn_Click" /></td>
                            </tr>
                            <tr>
                                <td>
                                </td>
                            </tr>
                            <tr>
                                <td>
                                </td>
                            </tr>
                        </table>
                    </td>
                </tr>
                <tr>
                    <td style="width: 92px">
                        &nbsp;
                    </td>
                </tr>
                <tr>
                    <td style="width: 92px">
                    </td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td bgcolor="#808080">
        </td>
    </tr>
</table>

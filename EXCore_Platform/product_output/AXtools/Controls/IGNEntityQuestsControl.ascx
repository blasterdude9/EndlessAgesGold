<%@ control language="c#" autoeventwireup="true" inherits="IGNEntityQuestsControl, App_Web_ojrkv221" %>
<table id="Table2" style="width: 436px; height: 1006px" cellspacing="0" cellpadding="0"
    width="436" bgcolor="gray" border="0">
    <tr>
        <td style="height: 12px" bgcolor="appworkspace">
            <strong><font face="Arial" color="#ffffff" size="2">Quest Data</font></strong></td>
    </tr>
    <tr>
        <td style="height: 7px" valign="top" width="300" bgcolor="#99cc99">
            <div align="left">
                <table id="Table5" style="width: 432px; height: 83px" cellspacing="0" cellpadding="0"
                    width="432" border="0">
                    <tr>
                        <td style="width: 107px" valign="top">
                            <asp:Panel ID="questListpanel" Width="200px" Font-Size="XX-Small" Font-Names="Arial"
                                runat="server">
                                <p>
                                    <strong>Quest List</strong></p>
                                <p>
                                    <asp:Label ID="m_miscTest" runat="server" Font-Names="Arial" Font-Size="XX-Small"></asp:Label></p>
                            </asp:Panel>
                        </td>
                        <td>
                            <p align="right">
                                <asp:Panel ID="AddQuestPanel" Width="225px" Font-Size="XX-Small" Font-Names="Arial"
                                    runat="server" HorizontalAlign="Center" BorderStyle="Outset" BorderColor="DimGray"
                                    Height="119px">
                                    <p>
                                        New Quest Panel</p>
                                    <p>
                                    </p>
                                    <p align="left">
                                        <table id="Table6" style="width: 216px; height: 41px" cellspacing="0" cellpadding="0"
                                            width="216" border="0">
                                            <tr>
                                                <td style="width: 48px" align="right">
                                                    <asp:Button ID="NewQuestSelection" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                        Width="56px" BorderColor="Black" BorderStyle="Solid" Text="New"></asp:Button></td>
                                                <td style="width: 86px" align="right">
                                                    <asp:TextBox ID="NewQuestTitle" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                        Width="98px"></asp:TextBox></td>
                                                <td>
                                                    <font size="1">Quest Title</font></td>
                                            </tr>
                                            <tr>
                                                <td style="width: 48px" align="right">
                                                    <asp:Button ID="DeleteQuestBtn" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                        Width="56px" BorderColor="Black" BorderStyle="Solid" Text="Delete"></asp:Button></td>
                                                <td style="width: 86px" align="right">
                                                    <asp:Button ID="DeleteQuestConfBtn" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                        Width="32px" BorderColor="Black" BorderStyle="Solid" Text="Yes" Visible="False">
                                                    </asp:Button>
                                                    <asp:Button ID="DeleteQuestCancelBtn" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                        Width="32px" BorderColor="Black" BorderStyle="Solid" Text="No" Visible="False"></asp:Button></td>
                                                <td>
                                                    <asp:Label ID="AreYouSureLabel" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                        Visible="False">Are You Sure??</asp:Label></td>
                                            </tr>
                                            <tr>
                                                <td style="width: 48px" align="right">
                                                    <asp:Button ID="InsertQuestBtn" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                        Width="56px" BorderColor="Black" BorderStyle="Solid" Text="Insert"></asp:Button></td>
                                                <td style="width: 86px" align="right">
                                                </td>
                                                <td>
                                                </td>
                                            </tr>
                                        </table>
                                    </p>
                                    <p>
                                        &nbsp;</p>
                                </asp:Panel>
                        </td>
                    </tr>
                </table>
            </div>
                    <table id="Table7" style="width: 434px; height: 9px; font-size: xx-small; font-family: Arial, Verdana;" cellspacing="0" cellpadding="0"
                        width="434" border="0">
                        <tr>
                            <td style="height: 1px">
                                <p>
                                    <asp:CheckBox ID="ShowSpeechPanel" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Height="8px" Text="Show Speech Panel" AutoPostBack="True" oncheckedchanged="ShowSpeechPanel_CheckedChanged"></asp:CheckBox></p>
                            </td>
                        </tr>
                        <tr>
                            <td style="height: 8px">
                                <p>
                                    <asp:CheckBox ID="ShowRequiredItemsPanel" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Height="8px" Text="Show Required Panel" AutoPostBack="True"></asp:CheckBox></p>
                            </td>
                        </tr>
                        <tr>
                            <td style="height: 7px">
                                <asp:CheckBox ID="ShowRewardPanel" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                    Height="8px" Text="Show Reward Panel" AutoPostBack="True"></asp:CheckBox></td>
                        </tr>
                        <tr>
                            <td style="height: 7px">
                                <asp:CheckBox ID="ShowJournalPanel" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                    Height="8px" Text="Show Journal Panel" AutoPostBack="True"></asp:CheckBox></td>
                        </tr>
                        <tr>
                            <td style="height: 7px">
                                <asp:CheckBox ID="CheckShowTragetDropPanel" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                    Height="8px" Text="Show Target Drop Panel" AutoPostBack="True"></asp:CheckBox></td>
                        </tr>
                        <tr>
                            <td style="height: 7px">
                                <asp:CheckBox ID="ShowSpawnPanel" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                    Height="8px" Text="Show Spawn Panel" AutoPostBack="True"></asp:CheckBox></td>
                        </tr>
                        <tr>
                            <td style="height: 7px">
                                    <asp:Button ID="ApplyChanges" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Width="117px" BorderColor="Yellow" BorderStyle="Solid" Text="Apply Changes" OnClick="ApplyChanges_Click1"></asp:Button>
                                <asp:Label ID="m_lastAppliedChanges" runat="server" ForeColor="Gray"></asp:Label></td>
                        </tr>
                    </table>
        </td>
    </tr>
    <tr>
        <td style="height: 36px" valign="top" width="300" bgcolor="#99cc99">
            &nbsp;<asp:Panel ID="SpeechPanel" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                    BorderColor="DimGray" BorderStyle="Outset" HorizontalAlign="Center">
                                    Speech Panel
                                        <table id="Table1" style="width: 456px; height: 455px" cellspacing="0" cellpadding="0"
                                            width="456" border="0">
                                            <tr>
                                                <td>
                                                    <strong><font face="Arial" size="2"></font></strong>
                                                </td>
                                                <td>
                                                </td>
                                            </tr>
                                            <tr>
                                                <td>
                                                    <p align="right">
                                                        <asp:TextBox ID="title" runat="server" Font-Names="Arial" Font-Size="XX-Small" Width="202px"></asp:TextBox></p>
                                                </td>
                                                <td>
                                                    <font face="Arial" size="1">Title</font></td>
                                            </tr>
                                            <tr>
                                                <td>
                                                    <p align="right">
                                                        <asp:TextBox ID="CompletedMessage" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                            Width="348px" Height="120px" TextMode="MultiLine"></asp:TextBox></p>
                                                </td>
                                                <td>
                                                    <font face="Arial" size="1"><font face="Arial" size="1">Quest Completed Message</font></font></td>
                                            </tr>
                                            <tr>
                                                <td>
                                                    <p align="right">
                                                        <asp:TextBox ID="QuestFailedMsg" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                            Width="348px" Height="120px" TextMode="MultiLine"></asp:TextBox></p>
                                                </td>
                                                <td>
                                                    <font face="Arial" size="1">Quest Failed Message</font></td>
                                            </tr>
                                            <tr>
                                                <td style="height: 40px">
                                                    <p align="right">
                                                        <asp:TextBox ID="OptionalInstruction" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                            Width="348px" Height="56px" TextMode="MultiLine"></asp:TextBox></p>
                                                </td>
                                                <td style="height: 40px">
                                                    <font face="Arial" size="1">Optional Instruction</font></td>
                                            </tr>
                                            <tr>
                                                <td>
                                                </td>
                                                <td>
                                                </td>
                                            </tr>
                                            <tr>
                                                <td>
                                                    <p align="right">
                                                        <asp:CheckBox ID="CheckOpenFileDialogWin" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                            Text="Open File Dialog Window" TextAlign="Left"></asp:CheckBox></p>
                                                </td>
                                                <td>
                                                    <font face="Arial" size="1"></font>
                                                </td>
                                            </tr>
                                            <tr style="font-size: 12pt">
                                                <td align="right">
                                                    <asp:TextBox ID="ClientScriptEdit" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                        Width="348px" Height="136px" TextMode="MultiLine"></asp:TextBox></td>
                                                <td>
                                                    <font size="1">Client Script (Blank will ignore)</font></td>
                                            </tr>
                                            <tr>
                                                <td style="height: 20px" valign="top" align="right">
                                                    <asp:Label ID="scripFunctionDescription" runat="server" Font-Names="Arial" Font-Size="XX-Small"></asp:Label>
                                                    <asp:DropDownList ID="ScriptFunctionList" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                        Width="259px" AutoPostBack="True">
                                                    </asp:DropDownList></td>
                                                <td style="height: 20px" valign="top">
                                                    <asp:Button ID="addFunctionBtn" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                        Width="88px" BorderColor="Black" BorderStyle="Solid" Text="Add Function"></asp:Button></td>
                                            </tr>
                                            <tr>
                                                <td valign="top" align="right">
                                                    <asp:Label ID="scriptMacroDescription" runat="server" Font-Names="Arial" Font-Size="XX-Small"></asp:Label>
                                                    <asp:DropDownList ID="ScriptMacroList" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                        Width="259px" AutoPostBack="True">
                                                    </asp:DropDownList></td>
                                                <td valign="top">
                                                    <asp:Button ID="addMacroBtn" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                        Width="88px" BorderColor="Black" BorderStyle="Solid" Text="Add Macro"></asp:Button></td>
                                            </tr>
                                        </table>
                                </asp:Panel>
            
            <asp:Panel ID="RewardPanel" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                HorizontalAlign="Center" BorderStyle="Outset" BorderColor="DimGray">
                Reward Panel
                <table id="Table10" style="width: 424px; height: 75px" cellspacing="0" cellpadding="0"
                    width="424" border="0">
                    <tr>
                        <td style="width: 216px">
                            <p align="right">
                                <asp:TextBox ID="CashReward" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                    Width="89px"></asp:TextBox></p>
                        </td>
                        <td>
                            <font face="Arial" size="1">Cash Reward</font></td>
                    </tr>
                    <tr>
                        <td style="width: 216px; height: 19px" valign="top">
                            <table id="Table13" style="width: 232px; height: 26px" bordercolor="black" cellspacing="0"
                                cellpadding="0" width="232" border="1">
                                <tr>
                                    <td>
                                        <font face="Arial" size="1">Reward Items</font></td>
                                </tr>
                                <tr>
                                    <td>
                                        <p align="right">
                                            <asp:ListBox ID="rewardItemList" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                Width="198px" Height="88px"></asp:ListBox></p>
                                    </td>
                                </tr>
                                <tr>
                                    <td>
                                        <asp:Button ID="AddRewardItem" runat="server" Font-Size="XX-Small" Width="88px" BorderColor="Black"
                                            BorderStyle="Solid" Text="Add Item"></asp:Button>
                                        <asp:Button ID="DeleteRewardItem" runat="server" Font-Size="XX-Small" Width="88px"
                                            BorderColor="Black" BorderStyle="Solid" Text="Delete Item"></asp:Button></td>
                                </tr>
                            </table>
                        </td>
                        <td style="height: 19px" valign="top">
                            <font face="Arial" size="1">
                                <table id="Table11" style="width: 232px; height: 66px" bordercolor="black" cellspacing="0"
                                    cellpadding="0" width="232" border="1">
                                    <tr>
                                        <td>
                                            <font face="Arial" size="1">Item Data Manager/Search</font></td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <asp:ListBox ID="ItemGILRewardList" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                Width="200px" Height="87px"></asp:ListBox></td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <asp:TextBox ID="RewardSearchCriteria" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                Width="80px"></asp:TextBox>
                                            <asp:Button ID="SearchRewardBtn" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                Width="67px" BorderColor="Black" BorderStyle="Solid" Text="Search" OnClick="SearchRewardBtn_Click"></asp:Button></td>
                                    </tr>
                                </table>
                            </font>
                        </td>
                    </tr>
                    <tr>
                        <td style="width: 216px">
                            <p align="right">
                                <asp:TextBox ID="ExperienceBonus" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                    Width="89px"></asp:TextBox></p>
                        </td>
                        <td>
                            <font face="Arial" size="1">Experience Bonus</font></td>
                    </tr>
                    <tr>
                        <td style="width: 216px; height: 21px">
                            <p align="right">
                                <asp:TextBox ID="ExperienceSkillTypeBonus" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                    Width="58px" Visible="False"></asp:TextBox>
                                <asp:DropDownList ID="ExpSkilltypeDropList" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                    Width="133px" AutoPostBack="True">
                                </asp:DropDownList></p>
                        </td>
                        <td style="height: 21px">
                            <font face="Arial" size="1">Experience Skill Type Bonus</font></td>
                    </tr>
                    <tr>
                        <td style="width: 216px">
                            <p align="right">
                                <asp:TextBox ID="ExperienceMaxGain" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                    Width="58px"></asp:TextBox></p>
                        </td>
                        <td>
                            <font face="Arial" size="1">Experience Max Gain</font></td>
                    </tr>
                    <tr>
                        <td style="width: 216px; height: 20px;">
                        </td>
                        <td style="height: 20px">
                            <asp:CheckBox ID="m_checkRandomRewardType" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                Text="Random Reward Enabled"></asp:CheckBox></td>
                    </tr>
                    <tr>
                        <td style="width: 216px">
                            <p align="left">
                                &nbsp;</p>
                        </td>
                        <td>
                            <asp:CheckBox ID="m_clearInventory" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                Text="Clear Inventory before giving award"></asp:CheckBox></td>
                    </tr>
                    <tr>
                        <td style="width: 216px">
                        </td>
                        <td>
                            <asp:CheckBox ID="m_deleteByLocationID" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                Text="Clear By Rewards location IDs"></asp:CheckBox></td>
                    </tr>
                    <tr>
                        <td style="width: 216px; height: 20px;">                   
                        </td>
                        <td style="height: 20px">
                            <font face="Arial" size="1">
                                <asp:CheckBox ID="deleteAllByRequiredGLID" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                    Text="Clear By Required Item types"></asp:CheckBox></font></td>
                    </tr>
                    <tr>
                        <td style="width: 216px; height: 20px;">
                        </td>
                        <td style="height: 20px">
                            <asp:CheckBox ID="m_despawnFlag" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                Text="Despawn Flags"></asp:CheckBox></td>
                    </tr>
                    <tr>
                        <td style="width: 216px" align="right">
                            <asp:TextBox ID="m_maxDamage" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                Width="58px"></asp:TextBox></td>
                        <td>
                            <font size="1">Max Damage Bonus</font></td>
                    </tr>
                    <tr>
                        <td style="width: 216px" align="right">
                            <asp:TextBox ID="m_minDamage" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                Width="58px"></asp:TextBox></td>
                        <td>
                            <font size="1">Min Damage Bonus</font></td>
                    </tr>
                    <tr>
                        <td style="width: 216px; height: 20px;">
                        </td>
                        <td style="height: 20px">
                            <p align="left">
                                <asp:CheckBox ID="m_flagTimerOnFail" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                    Text="Flag Timer On Success"></asp:CheckBox></p>
                        </td>
                    </tr>
                    <tr>
                        <td style="width: 216px">
                        </td>
                        <td>
                            <asp:CheckBox ID="m_flagHostileOnSuccessCheck" runat="server" Font-Names="Arial"
                                Font-Size="XX-Small" Text="Flag Hostile On Success"></asp:CheckBox></td>
                    </tr>
                    <tr>
                        <td style="width: 216px; height: 20px;">                   
                        </td>
                        <td style="height: 20px">
                            <font face="Arial" size="1">
                                <asp:CheckBox ID="deleteAllByRewardGLID" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                    Text="Clear By Reward Item types"></asp:CheckBox></font></td>
                    </tr>
                </table>
                <table id="Table15" style="width: 454px; height: 174px" cellspacing="0" cellpadding="0"
                    width="454" border="0">
                    <tr>
                        <td style="width: 342px">
                            <asp:TextBox ID="rewardScriptEdit" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                Width="348px" Height="136px" TextMode="MultiLine"></asp:TextBox></td>
                        <td>
                            <font size="1">Reward&nbsp;Script (Blank will ignore)</font>&nbsp;&nbsp;
                        </td>
                    </tr>
                    <tr>
                        <td style="width: 342px; height: 28px" valign="top" align="right">
                            <asp:Label ID="rFunctionListDesc" runat="server" Font-Names="Arial" Font-Size="XX-Small"></asp:Label>
                            <asp:DropDownList ID="RFunctionDropList" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                Width="259px" AutoPostBack="True">
                            </asp:DropDownList></td>
                        <td style="height: 28px" valign="top">
                            <asp:Button ID="rAddFunction" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                Width="88px" BorderColor="Black" BorderStyle="Solid" Text="Add Function"></asp:Button></td>
                    </tr>
                    <tr>
                        <td style="width: 342px" valign="top" align="right">
                            <asp:Label ID="rMacroDescription" runat="server" Font-Names="Arial" Font-Size="XX-Small"></asp:Label>
                            <asp:DropDownList ID="RMacroList" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                Width="259px" AutoPostBack="True">
                            </asp:DropDownList></td>
                        <td valign="top">
                            <asp:Button ID="rAddMacro" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                Width="88px" BorderColor="Black" BorderStyle="Solid" Text="Add Macro"></asp:Button></td>
                    </tr>
                </table>
            </asp:Panel>
                                <asp:Panel ID="RequiredPanel" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                    BorderColor="DimGray" BorderStyle="Outset" HorizontalAlign="Center">
                                    Required Panel
                                        <table id="Table8" style="width: 424px; height: 75px" cellspacing="0" cellpadding="0"
                                            width="424" border="0">
                                            <tbody>
                                                <tr>
                                                    <td style="width: 213px">
                                                        <p align="right">
                                                            <asp:TextBox ID="RequiredSkillLevel" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                                Width="58px"></asp:TextBox></p>
                                                    </td>
                                                    <td>
                                                        <font face="Arial" size="1">Skill Level</font></td>
                                                </tr>
                                                <tr>
                                                    <td style="width: 213px">
                                                        <p align="right">
                                                            <asp:TextBox ID="RequiredSkillType" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                                Width="58px" Visible="False"></asp:TextBox>
                                                            <asp:DropDownList ID="RequiredSkillTypeDropList" runat="server" Font-Names="Arial"
                                                                Font-Size="XX-Small" Width="133px" AutoPostBack="True">
                                                            </asp:DropDownList></p>
                                                    </td>
                                                    <td>
                                                        <font face="Arial" size="1">Skill Type</font></td>
                                                </tr>
                                                <tr>
                                                    <td style="width: 213px">
                                                        <p align="right">
                                                            <asp:TextBox ID="RequiredExpansionLevel" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                                Width="58px"></asp:TextBox></p>
                                                    </td>
                                                    <td>
                                                        <font face="Arial" size="1">Expansion Level</font></td>
                                                </tr>
                                                <tr>
                                                    <td style="width: 213px" align="right">
                                                        <asp:TextBox ID="m_expFailMsg" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                            Width="192px" Height="88px"></asp:TextBox></td>
                                                    <td>
                                                        <font size="1">Expansion Level Fail Message-will stop spawn and display this msg or&nbsp;("RESPAWN"
                                                            will trigger a redirect players start point).</font></td>
                                                </tr>
                                                <tr style="font-size: 12pt">
                                                    <td style="width: 213px; height: 2px" align="right">
                                                        <asp:DropDownList ID="RaceSpecificRequiredList" runat="server" Font-Names="Arial"
                                                            Font-Size="XX-Small" Width="133px" AutoPostBack="True">
                                                        </asp:DropDownList></td>
                                                    <td style="height: 2px">
                                                        <font size="1">Race</font></td>
                                                </tr>
                                                <tr style="font-size: 12pt">
                                                    <td align="right" style="width: 213px; height: 2px">
                                                    </td>
                                                    <td style="height: 2px">
                                                    </td>
                                                </tr>
                                                <tr style="font-size: 12pt">
                                                    <td style="width: 213px; height: 4px">
                                                                <table id="Table9" style="width: 232px; height: 26px" bordercolor="black" cellspacing="0"
                                                                    cellpadding="0" width="232" border="1">
                                                                    <tr>
                                                                        <td>
                                                                            <font face="Arial" size="1">Required Items</font></td>
                                                                    </tr>
                                                                    <tr>
                                                                        <td>
                                                                            <p align="right">
                                                                                <asp:ListBox ID="requiredItemList" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                                                    Width="198px" Height="88px"></asp:ListBox></p>
                                                                        </td>
                                                                    </tr>
                                                                    <tr>
                                                                        <td>
                                                                            <asp:Button ID="NewRequiredItemBtn" runat="server" Font-Size="XX-Small" Width="88px"
                                                                                BorderColor="Black" BorderStyle="Solid" Text="Add Item" OnClick="NewRequiredItemBtn_Click1"></asp:Button>
                                                                            <asp:Button ID="DeleteRequiredItemBtn" runat="server" Font-Size="XX-Small" Width="88px"
                                                                                BorderColor="Black" BorderStyle="Solid" Text="Delete Item"></asp:Button></td>
                                                                    </tr>
                                                                </table>
                                                    </td>
                                                    <td style="height: 4px" valign="top">
                                <table id="Table12" style="width: 232px; height: 90px" bordercolor="black" cellspacing="0"
                                    cellpadding="0" width="232" border="1">
                                    <tr>
                                        <td>
                                            <font face="Arial" size="1">Item Data Manager/Search</font></td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <asp:ListBox ID="ItemGILRequiredResultList" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                Width="200px" Height="87px"></asp:ListBox></td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <asp:TextBox ID="searchRequiredCriteria" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                Width="80px"></asp:TextBox>
                                            <asp:Button ID="SearchRequiredInvBtn" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                Width="41px" BorderColor="Black" BorderStyle="Solid" Text="Search"></asp:Button></td>
                                    </tr>
                                </table>
                                                    </td>
                                                </tr>
                                                <tr style="font-size: 12pt">
                                                    <td style="width: 213px; height: 4px; text-align: right">
                                    <asp:TextBox ID="mustOwnKingdomID" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Width="58px"></asp:TextBox></td>
                                                    <td style="height: 4px" valign="top">
                                                        <span style="font-size: 7pt">must own kingdom ID</span></td>
                                                </tr>
                                                <tr style="font-size: 12pt">
                                                    <td style="width: 213px; height: 4px; text-align: right">
                                <asp:TextBox ID="m_timeStampRequired" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                    Width="113px"></asp:TextBox></td>
                                                    <td style="height: 4px" valign="top">
                                                        <span style="font-size: 7pt">0=disabled&nbsp;if this time (milliseconds) is greater then the duration
                                    of the last time stamp to now this will quest will fail, use this for obstacle courses.
                                    flag the timer with one bot (option above this one)&nbsp;and tell them to get to
                                    this bot in a certain time.</span></td>
                                                </tr>
                                                <tr style="font-size: 12pt">
                                                    <td style="width: 213px; height: 4px; text-align: right">
                            <asp:TextBox ID="requiredScriptEdit" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                Width="256px" Height="136px" TextMode="MultiLine"></asp:TextBox></td>
                                                    <td style="height: 4px" valign="top">
                                                        <span style="font-size: 7pt">Required Script (Blank will ignore)</span> &nbsp;
                                                    </td>
                                                </tr>
                                                <tr style="font-size: 12pt">
                                                    <td style="width: 213px; height: 4px; text-align: right">
                                                    </td>
                                                    <td style="height: 4px" valign="top">
                                                    </td>
                                                </tr>
                                                <tr style="font-size: 12pt">
                                                    <td style="width: 213px; height: 4px; text-align: right">
                            <asp:Label ID="rqFunctionDescription" runat="server" Font-Names="Arial" Font-Size="XX-Small"></asp:Label><asp:DropDownList ID="rqFunctionList" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                Width="259px" AutoPostBack="True">
                            </asp:DropDownList></td>
                                                    <td style="height: 4px" valign="top">
                            <asp:Button ID="RqAddFunctionBtn" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                Width="88px" BorderColor="Black" BorderStyle="Solid" Text="Add Function"></asp:Button></td>
                                                </tr>
                                                <tr style="font-size: 12pt">
                                                    <td style="width: 213px; height: 4px; text-align: right">
                            <asp:Label ID="rqMacroDescription" runat="server" Font-Names="Arial" Font-Size="XX-Small"></asp:Label><asp:DropDownList ID="rqMacroList" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                Width="259px" AutoPostBack="True">
                            </asp:DropDownList></td>
                                                    <td style="height: 4px" valign="top">
                            <asp:Button ID="rqAddMacroBtn" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                Width="88px" BorderColor="Black" BorderStyle="Solid" Text="Add Macro"></asp:Button></td>
                                                </tr>
                                   
                                </table>
                                </asp:Panel>
            &nbsp;
            <p>
                <asp:Panel ID="JournalPanel" Width="433px" Font-Size="XX-Small" Font-Names="Arial"
                    runat="server" HorizontalAlign="Center" BorderStyle="Outset" BorderColor="DimGray"
                    Height="125px">
                    <p>
                        Quest Journal Panel</p>
                    <table id="Table3" style="width: 424px; height: 75px" cellspacing="0" cellpadding="0"
                        width="424" border="0">
                        <tr>
                            <td style="width: 217px">
                                <p align="right">
                                    <asp:CheckBox ID="ProgressionBased" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Text="Progression Based" TextAlign="Left"></asp:CheckBox></p>
                            </td>
                            <td>
                                <font face="Arial" size="1"></font>
                            </td>
                        </tr>
                        <tr>
                            <td style="width: 217px; height: 19px">
                                <p align="right">
                                    <asp:CheckBox ID="CheckAutoComplete" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Text="Autocomplete" TextAlign="Left"></asp:CheckBox></p>
                            </td>
                            <td style="height: 19px">
                                <font face="Arial" size="1"></font>
                            </td>
                        </tr>
                        <tr>
                            <td style="width: 217px; height: 20px">
                                <p align="right">
                                    <asp:CheckBox ID="CheckListProgression" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Text="List Progression" TextAlign="Left"></asp:CheckBox></p>
                            </td>
                            <td style="height: 20px">
                                <font face="Arial" size="1"></font>
                            </td>
                        </tr>
                        <tr>
                            <td style="width: 217px">
                                <p align="right">
                                    <asp:CheckBox ID="UndeletableCheck" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Text="Undeletable" TextAlign="Left"></asp:CheckBox></p>
                            </td>
                            <td>
                                <font face="Arial" size="1"></font>
                            </td>
                        </tr>
                        <tr>
                            <td style="width: 217px; height: 14px">
                                <p align="right">
                                    <asp:DropDownList ID="JournalCommandOption" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Width="208px">
                                    </asp:DropDownList></p>
                            </td>
                            <td style="height: 14px">
                                <font face="Arial" size="1">Journal Command Option</font></td>
                        </tr>
                        <tr>
                            <td style="width: 217px; height: 19px" align="right">
                                <asp:DropDownList ID="JournalKey" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                    Width="208px" Height="8px">
                                </asp:DropDownList></td>
                            <td style="height: 19px">
                                <font size="1">Journal Key</font></td>
                        </tr>
                        <tr>
                            <td style="width: 217px">
                                <p align="right">
                                    <asp:TextBox ID="m_closeJnlEntryByName" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                        Width="89px"></asp:TextBox></p>
                            </td>
                            <td>
                                <font face="Arial" size="1">Close Journal Entry by bot name</font></td>
                        </tr>
                    </table>
                </asp:Panel>
                <asp:Panel ID="TargetDropPanel" Width="433px" Font-Size="XX-Small" Font-Names="Arial"
                        runat="server" HorizontalAlign="Center" BorderStyle="Outset" BorderColor="DimGray"
                        Height="125px">
                <p>
                    Target Drop&nbsp;Panel</p>
                <p>
                    <table id="Table4" style="width: 424px; height: 75px" cellspacing="0" cellpadding="0"
                        width="424" border="0">
                        <tr>
                            <td style="width: 213px">
                                <p align="right">
                                    <asp:TextBox ID="aiTargetConfiguration" Width="54px" Font-Size="XX-Small" Font-Names="Arial"
                                        runat="server" Visible="False"></asp:TextBox><asp:DropDownList ID="TargetDropAICFGList"
                                            Width="153px" Font-Size="XX-Small" Font-Names="Arial" runat="server" AutoPostBack="True">
                                        </asp:DropDownList></p>
                            </td>
                            <td>
                                <font face="Arial" size="1">AI Configuration</font></td>
                        </tr>
                        <tr>
                            <td style="width: 213px; height: 19px">
                                <p align="right">
                                    <asp:Label ID="m_glidDropLabel" Font-Size="XX-Small" Font-Names="Arial" runat="server"></asp:Label><asp:TextBox
                                        ID="dropGlid" Width="54px" Font-Size="XX-Small" Font-Names="Arial" runat="server"></asp:TextBox></p>
                            </td>
                            <td style="height: 19px">
                                <font face="Arial" size="1">Drop Glid</font></td>
                        </tr>
                        <tr>
                            <td style="width: 213px">
                                <p align="right">
                                    <asp:TextBox ID="maxDropCount" Width="54px" Font-Size="XX-Small" Font-Names="Arial"
                                        runat="server"></asp:TextBox></p>
                            </td>
                            <td>
                                <font face="Arial" size="1">Max Drop Count</font></td>
                        </tr>
                        <tr>
                            <td style="width: 213px">
                                <p align="right">
                                    <asp:TextBox ID="dropTargetPercentage" Width="54px" Font-Size="XX-Small" Font-Names="Arial"
                                        runat="server"></asp:TextBox></p>
                            </td>
                            <td>
                                <font face="Arial" size="1">Drop&nbsp;Percentage</font></td>
                        </tr>
                        <tr>
                            <td style="width: 213px">
                            </td>
                            <td>
                            </td>
                        </tr>
                    </table>
                    </asp:panel></p>
                    <asp:Panel ID="SpawnPanel" Width="433px" Font-Size="XX-Small" Font-Names="Arial"
                        runat="server" HorizontalAlign="Center" BorderStyle="Outset" BorderColor="DimGray"
                        Height="125px" Visible="False"><P>Spawn Panel</P><TABLE style="FONT-SIZE: xx-small; WIDTH: 504px; FONT-FAMILY: Arial, Verdana; HEIGHT: 75px" id="Table4" cellSpacing=0 cellPadding=0 border=0><TBODY><TR><TD style="WIDTH: 213px"><FONT face="Arial" size=2><STRONG><P align=right><FONT face="Arial" size=2><STRONG></STRONG></FONT></P></STRONG></FONT></TD><TD></TD></TR><TR><TD style="WIDTH: 213px"><P align=right><asp:DropDownList id="SpawnAICFGdropList" runat="server" Font-Names="Arial" Font-Size="XX-Small" Width="153px" AutoPostBack="True">
                                    </asp:DropDownList></P></TD><TD><FONT face="Arial" size=1>AI Configuration</FONT></TD></TR><TR><TD style="WIDTH: 213px; HEIGHT: 19px"><P align=right><asp:TextBox id="SpawnLifeDuration" runat="server" Font-Names="Arial" Font-Size="XX-Small" Width="54px"></asp:TextBox></P></TD><TD style="HEIGHT: 19px"><FONT face="Arial" size=1>Life Duration (Milliseconds)</FONT></TD></TR><TR><TD style="WIDTH: 213px"><P align=right><asp:TextBox id="SpawnWorldChannel" runat="server" Font-Names="Arial" Font-Size="XX-Small" Width="54px"></asp:TextBox></P></TD><TD><FONT face="Arial" size=1><SPAN style="COLOR: appworkspace">World Selection (Obsolete-Legacy access)</SPAN></FONT></TD></TR><TR><TD style="WIDTH: 213px; TEXT-ALIGN: right"><asp:DropDownList id="m_spawnPanelWorldDropList" runat="server" Font-Names="Arial" Font-Size="XX-Small" Width="153px" AutoPostBack="True">
                                </asp:DropDownList></TD><TD>World Selection</TD></TR><TR><TD style="WIDTH: 213px; TEXT-ALIGN: right"><asp:DropDownList id="m_instanceGenList" runat="server" Font-Names="Arial" Font-Size="XX-Small" Width="153px" AutoPostBack="True" OnSelectedIndexChanged="m_instanceGenList_SelectedIndexChanged">
                                </asp:DropDownList></TD><TD>Instance Generate Type</TD></TR><TR><TD style="WIDTH: 213px; HEIGHT: 13px; TEXT-ALIGN: right"><asp:TextBox id="m_instanceGenParam" runat="server" Font-Names="Arial" Font-Size="XX-Small" Width="54px" Wrap="False"></asp:TextBox></TD><TD style="HEIGHT: 13px"><asp:Label id="m_instanceParam" runat="server"></asp:Label></TD></TR><TR><TD style="WIDTH: 213px; HEIGHT: 13px; TEXT-ALIGN: right"><asp:TextBox id="m_instanceGenParam2" runat="server" Font-Names="Arial" Font-Size="XX-Small" Width="54px"></asp:TextBox></TD><TD style="HEIGHT: 13px"><asp:Label id="m_instanceParam2" runat="server"></asp:Label></TD></TR><TR><TD style="WIDTH: 213px; HEIGHT: 13px; TEXT-ALIGN: right"><asp:TextBox id="m_instanceGenParam3" runat="server" Font-Names="Arial" Font-Size="XX-Small" Width="54px"></asp:TextBox></TD><TD style="HEIGHT: 13px"><asp:Label id="m_instanceParam3" runat="server"></asp:Label></TD></TR><TR><TD style="WIDTH: 213px; HEIGHT: 13px; TEXT-ALIGN: right"><asp:TextBox id="m_instanceGenParam4" runat="server" Font-Names="Arial" Font-Size="XX-Small" Width="54px"></asp:TextBox></TD><TD style="HEIGHT: 13px"><asp:Label id="m_instanceParam4" runat="server"></asp:Label></TD></TR><TR><TD style="WIDTH: 213px; HEIGHT: 13px; TEXT-ALIGN: right"></TD><TD style="HEIGHT: 13px">&nbsp;</TD></TR><TR><TD style="WIDTH: 213px; HEIGHT: 13px; TEXT-ALIGN: right"><asp:ListBox id="dynWldRulesListBox" runat="server" Font-Names="Arial" Font-Size="XX-Small" Width="206px" Height="86px" AutoPostBack="True" OnSelectedIndexChanged="dynWldRulesListBox_SelectedIndexChanged"></asp:ListBox></TD><TD style="HEIGHT: 13px"><TABLE style="FONT-SIZE: xx-small; WIDTH: 287px; FONT-FAMILY: Arial, Verdana"><TBODY><TR><TD style="TEXT-ALIGN: right">Dynamic World Rules</TD><TD style="WIDTH: 161px"><asp:DropDownList id="m_dynWorldRulesDList" runat="server" Font-Names="Arial" Font-Size="XX-Small" Width="170px">
                                            </asp:DropDownList></TD></TR><TR><TD style="TEXT-ALIGN: right">Param 1</TD><TD style="WIDTH: 161px"><asp:TextBox id="m_dynWorldRuleParam" runat="server" Font-Names="Arial" Font-Size="XX-Small" Width="53px"></asp:TextBox></TD></TR><TR><TD style="TEXT-ALIGN: right"><asp:Button id="addDynRuleBtn" onclick="addDynRuleBtn_Click" runat="server" Font-Names="Arial" Font-Size="XX-Small" Width="102px" BorderStyle="Solid" Text="Add"></asp:Button></TD><TD style="WIDTH: 161px"><asp:Button id="DeleteDynRuleBtn" onclick="DeleteDynRuleBtn_Click" runat="server" Font-Names="Arial" Font-Size="XX-Small" Width="102px" BorderStyle="Solid" Text="Delete"></asp:Button></TD></TR></TBODY></TABLE><asp:Label id="m_dynRuleOutput" runat="server" ForeColor="Red" Font-Bold="True"></asp:Label></TD></TR><TR><TD style="WIDTH: 213px; HEIGHT: 13px; TEXT-ALIGN: right"></TD><TD style="HEIGHT: 13px">&nbsp;</TD></TR><TR><TD style="WIDTH: 213px; HEIGHT: 20px"><P align=right><asp:TextBox id="PortalLimitVS" runat="server" Font-Names="Arial" Font-Size="XX-Small" Width="54px"></asp:TextBox></P></TD><TD style="HEIGHT: 20px">Portal Limit VS</TD></TR><TR><TD style="WIDTH: 213px; HEIGHT: 20px"></TD><TD style="HEIGHT: 20px"></TD></TR><TR><TD style="WIDTH: 213px; HEIGHT: 14px"><P align=right><asp:TextBox id="PortalLimitVSMsg" runat="server" Font-Names="Arial" Font-Size="XX-Small" Width="206px" Height="66px" TextMode="MultiLine"></asp:TextBox></P></TD><TD style="WIDTH: 211px; HEIGHT: 14px"><FONT face="Arial" size=1>Portal Limit VS Message</FONT></TD></TR><TR><TD style="WIDTH: 213px; HEIGHT: 14px" align=right><asp:TextBox id="PosX" runat="server" Font-Names="Arial" Font-Size="XX-Small" Width="54px"></asp:TextBox></TD><TD style="WIDTH: 211px; HEIGHT: 14px"><FONT size=1>Position X</FONT></TD></TR><TR><TD style="WIDTH: 213px; HEIGHT: 14px" align=right><asp:TextBox id="PosY" runat="server" Font-Names="Arial" Font-Size="XX-Small" Width="54px"></asp:TextBox></TD><TD style="WIDTH: 211px; HEIGHT: 14px"><FONT size=1>Position Y</FONT></TD></TR><TR><TD style="WIDTH: 213px" align=right><asp:TextBox id="PosZ" runat="server" Font-Names="Arial" Font-Size="XX-Small" Width="54px"></asp:TextBox></TD><TD><FONT size=1>Position Z</FONT>&nbsp;</TD></TR><TR><TD style="WIDTH: 213px" align=right><asp:TextBox id="SpawnRotation" runat="server" Font-Names="Arial" Font-Size="XX-Small" Width="54px"></asp:TextBox></TD><TD><FONT size=1>Rotation</FONT>&nbsp;&gt;0=Random &lt;0=fixed rotation</TD></TR><TR><TD style="WIDTH: 213px; HEIGHT: 14px" align=right><asp:TextBox id="m_aiOptionArrayCount" runat="server" Font-Names="Arial" Font-Size="XX-Small" Width="54px"></asp:TextBox></TD><TD style="WIDTH: 211px; HEIGHT: 14px"><SPAN style="FONT-SIZE: 7pt">AI Array Option Count (&gt; 0 activates created a multiple spawn)</SPAN></TD></TR><TR><TD style="WIDTH: 213px; HEIGHT: 14px" align=right><asp:TextBox id="m_aiArrayOptionOffset" runat="server" Font-Names="Arial" Font-Size="XX-Small" Width="54px"></asp:TextBox></TD><TD style="WIDTH: 211px; HEIGHT: 14px"><SPAN style="FONT-SIZE: 7pt">AI Array Option Offset ( how big the circle of spawns gets )</SPAN></TD></TR><TR><TD style="WIDTH: 213px; HEIGHT: 14px" align=right><asp:CheckBox id="SpawnPlayerToPositionCheck" runat="server" Font-Names="Arial" Font-Size="XX-Small" Text="Spawn Player To Position" TextAlign="Left"></asp:CheckBox></TD><TD style="WIDTH: 211px; HEIGHT: 14px"><FONT size=1></FONT></TD></TR><TR><TD style="WIDTH: 213px; HEIGHT: 14px" align=right><asp:CheckBox id="MakeTargetRebirthPoint" runat="server" Font-Names="Arial" Font-Size="XX-Small" Text="Make Target Rebirth Point" TextAlign="Left"></asp:CheckBox></TD><TD style="WIDTH: 211px; HEIGHT: 14px"></TD></TR><TR><TD style="WIDTH: 213px; HEIGHT: 14px" align=right><asp:CheckBox id="SpawnOnFail" runat="server" Font-Names="Arial" Font-Size="XX-Small" Text="Spawn On Fail" TextAlign="Left"></asp:CheckBox></TD><TD style="WIDTH: 211px; HEIGHT: 14px"></TD></TR><TR><TD style="WIDTH: 213px; HEIGHT: 14px" align=right></TD><TD style="WIDTH: 211px; HEIGHT: 14px"></TD></TR></TBODY></TABLE></asp:panel>
        </td>
    </tr>
    <tr>
        <td align="center">
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
</table>
<tr>
    <td valign="top" width="300" bgcolor="#99cc99">
    </td>
</tr>
<tr>
    <td valign="top" width="300" bgcolor="#99cc99">
        <p align="left">
            &nbsp;</p>
    </td>
</tr>
<p>
</p>
    <asp:Panel ID="SpawnPanelTemp" Width="433px" Font-Size="XX-Small" Font-Names="Arial"
        runat="server" HorizontalAlign="Center" BorderStyle="Outset" BorderColor="DimGray"
        Height="125px" Visible="False" DESIGNTIMEDRAGDROP="30">
<p>
    stencil</p>
    <table id="Table4" style="width: 424px; height: 75px" cellspacing="0" cellpadding="0"
        width="424" border="0">
        <tr>
            <td style="width: 213px">
                <font face="Arial" size="2"><strong>
                    <p align="right">
                        <font face="Arial" size="2"><strong></strong></font>&nbsp;</p>
                </strong></font>
            </td>
            <td>
            </td>
        </tr>
        <tr>
            <td style="width: 213px">
                <p align="right">
                </p>
            </td>
            <td>
                <font face="Arial" size="1">AI Configuration</font></td>
        </tr>
        <tr>
            <td style="width: 213px; height: 19px">
                <p align="right">
                    &nbsp;</p>
            </td>
            <td style="height: 19px">
                <font face="Arial" size="1">Life Duration (Milliseconds)</font></td>
        </tr>
        <tr>
            <td style="width: 213px">
                <p align="right">
                    &nbsp;</p>
            </td>
            <td>
                <font face="Arial" size="1">World Channel/ID</font></td>
        </tr>
        <tr>
            <td style="width: 213px">
                <p align="right">
                    &nbsp;</p>
            </td>
            <td>
                <font face="Arial" size="1">Portal Limit VS</font></td>
        </tr>
        <tr>
            <td style="width: 213px; height: 14px">
                <p align="right">
                    &nbsp;</p>
            </td>
            <td style="height: 14px">
                <font face="Arial" size="1"></font>
            </td>
        </tr>
        <tr>
            <td style="width: 213px; height: 14px" align="right">
            </td>
            <td style="height: 14px">
                <font size="1"></font>
            </td>
        </tr>
        <tr>
            <td style="width: 213px; height: 14px" align="right">
            </td>
            <td style="height: 14px">
                <font size="1"></font>
            </td>
        </tr>
        <tr>
            <td style="width: 213px; height: 14px" align="right">
            </td>
            <td style="height: 14px">
                <font size="1"></font>
            </td>
        </tr>
        <tr>
            <td style="width: 213px; height: 14px" align="right">
            </td>
            <td style="height: 14px">
                <font size="1"></font>
            </td>
        </tr>
        <tr>
            <td style="width: 213px; height: 14px" align="right">
            </td>
            <td style="height: 14px">
            </td>
        </tr>
        <tr>
            <td style="width: 213px; height: 14px" align="right">
            </td>
            <td style="height: 14px">
            </td>
        </tr>
        <tr>
            <td style="width: 213px; height: 14px">
            </td>
            <td style="height: 14px">
            </td>
        </tr>
        
    </table>
    </asp:panel>

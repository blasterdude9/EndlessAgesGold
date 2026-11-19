<%@ control language="c#" autoeventwireup="true" inherits="IGNEntityBaseControl, App_Web_ojrkv221" %>

<table id="Table2" style="width: 496px; height: 1048px" cellspacing="1" cellpadding="1"
        width="496" bgcolor="gray" border="0">
        <tr>
            <td style="height: 12px" bgcolor="appworkspace">
                <strong><font face="Arial" color="#ffffff" size="2">Base Properties</font></strong></td>
        </tr>
        <tr>
            <td style="height: 7px" valign="top" width="300" bgcolor="#99cc99">
                <div align="left">
                    <asp:Button ID="ApplyChanges" runat="server" Width="117px" Text="Apply Changes" BorderColor="Yellow"
                        BorderStyle="Solid" Font-Size="XX-Small" Font-Names="Arial"></asp:Button>&nbsp;
                    <asp:Button ID="QuestPanelBtn" Font-Names="Arial" Font-Size="XX-Small" BorderStyle="Solid"
                        BorderColor="ActiveCaption" Text="Quest Panel" Width="80px" runat="server" Visible="False">
                    </asp:Button></div>
            </td>
        </tr>
        <tr>
            <td valign="top" width="300" bgcolor="#99cc99">
                <table id="Table1" style="width: 400px; height: 661px; font-size: xx-small; font-family: Arial, Verdana;" cellspacing="0" cellpadding="0"
                    width="400" border="0">
                    <tr>
                        <td>
                            <p align="right">
                                <font face="Arial" size="2"><strong><span style="font-size: 7pt">Naming</span></strong></font></p>
                        </td>
                        <td style="width: 2px; font-size: 7pt;">
                        </td>
                        <td style="width: 220px; font-size: 7pt;">
                            <div align="left">
                                <asp:Label ID="m_miscTest" runat="server" Font-Names="Arial" Font-Size="XX-Small"></asp:Label>
                                <asp:Button ID="NewIGNEntity" runat="server" Width="61px" Text="New" BorderColor="Black"
                                    BorderStyle="Solid" Font-Size="XX-Small" Visible="False"></asp:Button></div>
                        </td>
                        <td>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <p align="right">
                                <asp:TextBox ID="generatorNameEdit" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                    Width="183px"></asp:TextBox></p>
                        </td>
                        <td style="width: 2px">
                            <span style="font-size: 10pt; font-family: Arial">&nbsp;</span></td>
                        <td style="width: 220px; font-size: 10pt; font-family: Times New Roman;">
                            <font face="Arial" size="1"><span style="font-size: 7pt">Generator Name</span></font></td>
                        <td style="font-size: 7pt; font-family: Times New Roman">
                        </td>
                    </tr>
                    <tr style="font-size: 7pt; font-family: Times New Roman">
                        <td>
                            <p align="right">
                                <asp:TextBox ID="DisplayedName" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                    Width="182px"></asp:TextBox></p>
                        </td>
                        <td style="width: 2px">
                        </td>
                        <td style="width: 220px">
                            <font face="Arial" size="1">Displayed Name</font></td>
                        <td style="font-weight: bold; font-size: 10pt">
                        </td>
                    </tr>
                    <tr style="font-size: 7pt; font-family: Times New Roman">
                        <td>
                        </td>
                        <td style="width: 2px">
                        </td>
                        <td style="width: 220px">
                            &nbsp;</td>
                        <td style="font-weight: bold; font-size: 10pt">
                        </td>
                    </tr>
                    <tr style="font-size: 7pt; font-family: Times New Roman">
                        <td style="text-align: right">
                            <asp:DropDownList ID="m_folderToMoveToList" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                Width="182px" AutoPostBack="True" OnSelectedIndexChanged="m_folderToMoveToList_SelectedIndexChanged">
                            </asp:DropDownList></td>
                        <td style="width: 2px">
                        </td>
                        <td style="width: 220px">
                            <span style="font-size: xx-small; font-family: Arial">Move to folder</span></td>
                        <td style="font-weight: bold; font-size: 10pt">
                        </td>
                    </tr>
                    <tr style="font-weight: bold; font-size: 7pt">
                        <td>
                            <p align="right">
                                &nbsp;</p>
                        </td>
                        <td style="width: 2px">
                        </td>
                        <td style="width: 220px">
                            <font face="Arial" size="2">&nbsp;</font></td>
                        <td>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <p align="right">
                                <span>Position in world</span></p>
                        </td>
                        <td>
                        </td>
                        <td>
                        </td>
                        <td>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <p align="right">
                                <asp:TextBox ID="PosX" runat="server" Font-Names="Arial" Font-Size="XX-Small" Width="105px"></asp:TextBox></p>
                        </td>
                        <td style="width: 2px">
                        </td>
                        <td style="width: 220px">
                           PosX</td>
                        <td>
                        </td>
                    </tr>
                    <tr>
                        <td style="height: 21px">
                            <p align="right">
                                <asp:TextBox ID="PosY" runat="server" Font-Names="Arial" Font-Size="XX-Small" Width="105px"></asp:TextBox></p>
                        </td>
                        <td style="width: 2px; height: 21px;">
                        </td>
                        <td style="width: 220px; height: 21px;">
                            <span>PosY</span></td>
                        <td>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <p align="right">
                                <asp:TextBox ID="PosZ" runat="server" Font-Names="Arial" Font-Size="XX-Small" Width="105px"></asp:TextBox></p>
                        </td>
                        <td style="width: 2px">
                        </td>
                        <td style="width: 220px">
                            <font face="Arial" size="1">PosZ</font></td>
                        <td>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <p align="right">
                                <asp:DropDownList ID="SelectWorldChannelDropList" Font-Size="XX-Small" Font-Names="Arial"
                                    runat="server" Width="177px" AutoPostBack="True">
                                </asp:DropDownList><span style="font-size: 7pt; font-family: Arial"> </span>
                                <asp:TextBox ID="WorldChannel" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                                    Width="41px"></asp:TextBox></p>
                        </td>
                        <td style="width: 2px; font-size: 7pt; font-family: Arial;">
                        </td>
                        <td style="width: 220px; font-size: 7pt; font-family: Arial;">
                            <font face="Arial" size="1"><span>World Channel</span></font></td>
                        <td style="font-size: 10pt">
                        </td>
                    </tr>
                    <tr style="font-size: 10pt">
                        <td>
                            <p align="right">
                                <asp:Button ID="AquireFromCharName" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                                    Width="133px" BorderStyle="Solid" BorderColor="Black" Text="Aquire From Char Name">
                                </asp:Button></p>
                        </td>
                        <td style="width: 2px">
                        </td>
                        <td style="width: 220px">
                            <asp:TextBox ID="charName" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                                Width="80px"></asp:TextBox><span style="font-size: 7pt; font-family: Arial"> </span>
                            <asp:Label ID="charaquisitionError" Font-Names="Arial" Font-Size="XX-Small" runat="server"></asp:Label></td>
                        <td style="font-size: 12pt; font-family: Times New Roman">
                        </td>
                    </tr>
                    <tr style="font-size: 12pt; font-family: Times New Roman">
                        <td>
                        </td>
                        <td style="width: 2px">
                        </td>
                        <td style="width: 220px">
                        </td>
                        <td>
                        </td>
                    </tr>
                    <tr style="font-size: 12pt; font-family: Times New Roman">
                        <td style="height: 19px">
                            <p align="right">
                                <font face="Arial" size="2"><span style="font-size: 7pt">States</span></font></p>
                        </td>
                        <td style="width: 2px; height: 19px; font-size: 7pt; font-family: Times New Roman;">
                        </td>
                        <td style="width: 220px; height: 19px; font-size: 7pt; font-family: Times New Roman;">
                        </td>
                        <td style="height: 19px; font-size: 7pt; font-family: Times New Roman;">
                        </td>
                    </tr>
                    <tr style="font-size: 7pt; font-family: Times New Roman">
                        <td>
                            <p align="right">
                                <asp:TextBox ID="CurAnimation" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                                    Width="41px"></asp:TextBox></p>
                        </td>
                        <td style="width: 2px">
                        </td>
                        <td style="width: 220px">
                            <font face="Arial" size="1"><span>Cur Animation</span></font></td>
                        <td style="font-size: 12pt">
                        </td>
                    </tr>
                    <tr style="font-size: 12pt; font-family: Times New Roman">
                        <td style="height: 21px">
                            <p align="right">
                                <asp:TextBox ID="RotationAdjust" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                                    Width="41px"></asp:TextBox></p>
                        </td>
                        <td style="width: 2px; height: 21px;">
                        </td>
                        <td style="width: 220px; height: 21px;">
                            <font face="Arial" size="1">Rotation Adjust</font></td>
                        <td style="height: 21px">
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <p align="right">
                                <asp:TextBox ID="EdbID" Font-Size="XX-Small" Font-Names="Arial" runat="server" Width="41px"
                                    Enabled="False" Visible="False"></asp:TextBox><span style="font-size: 7pt"> </span>
                                <asp:DropDownList ID="edbDropList" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                                    Width="133px" AutoPostBack="True">
                                </asp:DropDownList></p>
                        </td>
                        <td style="width: 2px">
                        </td>
                        <td style="width: 220px">
                            EDB ID</td>
                        <td>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <p align="right">
                                <asp:TextBox ID="MountOption" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                                    Width="41px"></asp:TextBox></p>
                        </td>
                        <td style="width: 2px">
                        </td>
                        <td style="width: 220px">
                            Mount Option</td>
                        <td>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <p align="right">
                                <asp:TextBox ID="RespawnDuration" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                                    Width="74px"></asp:TextBox></p>
                        </td>
                        <td style="width: 2px">
                        </td>
                        <td style="width: 220px">
                            <font face="Arial" size="1">Respawn Duration</font></td>
                        <td>
                        </td>
                    </tr>
                    <tr>
                        <td>
                        </td>
                        <td style="width: 2px">
                        </td>
                        <td style="width: 220px">
                            <span style="font-size: 7pt; font-family: Arial">&nbsp;</span></td>
                        <td style="font-size: 7pt; font-family: Arial">
                        </td>
                    </tr>
                    <tr style="font-size: 7pt; font-family: Arial">
                        <td>
                            <font face="Arial" size="2">
                                    <font face="Arial" size="2"><span>
                                        Inventory/Appearence</span></font><strong> </strong>
                            </font>
                        </td>
                        <td style="width: 2px; font-weight: bold;">
                        </td>
                        <td style="width: 220px; font-weight: bold;">
                            <font face="Arial" size="1"></font>
                        </td>
                        <td style="font-weight: bold">
                        </td>
                    </tr>
                    <tr style="font-weight: bold">
                        <td style="height: 112px" valign="top">
                                <table id="Table3" style="width: 125px; height: 16px" cellspacing="0"
                                    cellpadding="0" width="125" border="1">
                                    <tr>
                                        <td>
                                            <font face="Arial" size="1">Current Inventory</font></td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <p align="right">
                                                <asp:ListBox ID="EntityInventoryList" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                                                    Width="214px" AutoPostBack="True" Height="112px"></asp:ListBox></p>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <asp:Button ID="AddEntityInvItemBtn" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                                                Width="72px" Text="Add" BorderColor="Black" BorderStyle="Solid"></asp:Button>
                                            <asp:Button ID="DeleteEntityInventoryBtn" Font-Size="XX-Small" Font-Names="Arial"
                                                runat="server" Width="72px" Text="Delete" BorderColor="Black" BorderStyle="Solid">
                                            </asp:Button></td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <asp:CheckBox ID="ArmedCheck" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                                                Text="Armed" AutoPostBack="True"></asp:CheckBox></td>
                                    </tr>
                                </table>
                        </td>
                        <td style="width: 2px; height: 112px">
                        </td>
                        <td style="width: 220px; height: 112px" valign="top">
                            <font face="Arial" size="1"></font>
                            <table id="Table5" style="width: 232px; height: 137px" cellspacing="0"
                                cellpadding="0" width="232" border="1">
                                <tr>
                                    <td>
                                        <font face="Arial" size="1">Item Data Manager/Search</font></td>
                                </tr>
                                <tr>
                                    <td>
                                        <asp:ListBox ID="ItemGILResultList" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                                            Width="200px" Height="87px"></asp:ListBox></td>
                                </tr>
                                <tr>
                                    <td>
                                        <asp:TextBox ID="searchCriteria" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                                            Width="80px"></asp:TextBox>
                                        <asp:Button ID="SearchInvBtn" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                                            Width="67px" Text="Search" BorderColor="Black" BorderStyle="Solid" OnClick="SearchInvBtn_Click"></asp:Button></td>
                                </tr>
                            </table>
                        </td>
                        <td style="height: 112px" valign="top">
                        </td>
                    </tr>
                    <tr>
                        <td style="height: 8px">
                            <p align="right">
                                <asp:ListBox ID="DimCfgList" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                                    Width="123px" Height="89px"></asp:ListBox></p>
                        </td>
                        <td style="width: 2px; height: 8px">
                        </td>
                        <td style="width: 220px; height: 8px">
                            <p>
                                <font face="Arial" size="1">Dim configuration Panel</font></p>
                            <table id="Table4" style="width: 119px; height: 22px" cellspacing="0" cellpadding="0"
                                width="119" border="0">
                                <tr>
                                    <td style="width: 60px">
                                        <font size="1">Dim Configs</font></td>
                                    <td>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="width: 60px">
                                        <p align="left">
                                            <asp:TextBox ID="DimCfgEdit" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                                                Width="40px"></asp:TextBox></p>
                                    </td>
                                    <td>
                                        <asp:Button ID="SetDimCfg" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                                            Width="41px" Text="Set" BorderColor="Black" BorderStyle="Solid"></asp:Button></td>
                                </tr>
                            </table>
                            </td>
                        <td style="height: 8px">
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <p align="right">
                                &nbsp;</p>
                        </td>
                        <td style="width: 2px">
                        </td>
                        <td style="width: 220px" valign="top">
                            <font face="Arial" size="1"><font face="Arial" size="1">&nbsp;</font></font></td>
                        <td valign="top">
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <p align="right">
                                <font face="Arial" size="2"><strong>Random States</strong></font></p>
                        </td>
                        <td style="width: 2px">
                        </td>
                        <td style="width: 220px">
                            <font face="Arial" size="1"></font>
                        </td>
                        <td>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <p align="right">
                                <asp:TextBox ID="RandomAnimation" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                                    Width="41px"></asp:TextBox></p>
                        </td>
                        <td style="width: 2px">
                        </td>
                        <td style="width: 220px">
                            <font face="Arial" size="1">Random Animation</font></td>
                        <td>
                        </td>
                    </tr>
                    <tr>
                        <td style="text-align: right">
                            <font face="Arial" size="2">
                                    <asp:TextBox ID="RandomExpEffect" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                                        Width="41px"></asp:TextBox>
                            </font>
                        </td>
                        <td style="width: 2px">
                        </td>
                        <td style="width: 220px">
                            <font face="Arial" size="1">Random Exp Effect</font></td>
                        <td>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <p align="right">
                                <asp:TextBox ID="RandomTimeInterval" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                                    Width="74px"></asp:TextBox></p>
                        </td>
                        <td style="width: 2px">
                        </td>
                        <td style="width: 220px">
                            <font face="Arial" size="1">Random Time Interval</font>
                        </td>
                        <td>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <p align="right">
                                &nbsp;</p>
                        </td>
                        <td style="width: 2px">
                        </td>
                        <td style="width: 220px">
                            <font face="Arial" size="1"></font>
                        </td>
                        <td>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <p align="right">
                                <font face="Arial" size="2"><strong>Misc</strong></font></p>
                        </td>
                        <td style="width: 2px">
                        </td>
                        <td style="width: 220px">
                        </td>
                        <td>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <p align="right">
                                <asp:TextBox ID="RequiredKey" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                                    Width="41px"></asp:TextBox></p>
                        </td>
                        <td style="width: 2px">
                        </td>
                        <td style="width: 220px">
                            <font face="Arial" size="1">RequiredKey</font></td>
                        <td>
                        </td>
                    </tr>
                    <tr>
                        <td style="height: 21px">
                            <p align="right">
                                &nbsp;<asp:TextBox ID="InteractRadius" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                                    Width="74px"></asp:TextBox></p>
                        </td>
                        <td style="width: 2px; height: 21px;">
                        </td>
                        <td style="width: 220px; height: 21px;">
                            <font face="Arial" size="1">Interact Radius</font></td>
                        <td style="height: 21px">
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <p align="right">
                                <asp:DropDownList ID="m_autoTriggerDropList" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                    Width="139px" AutoPostBack="True">
                                </asp:DropDownList>
                                <asp:TextBox ID="AutoTrigger" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                                    Width="74px" Enabled="False" Visible="False"></asp:TextBox></p>
                        </td>
                        <td style="width: 2px">
                        </td>
                        <td style="width: 220px">
                            <font face="Arial" size="1">Auto Trigger (0=off 1=constant 2=one time only unique</font></td>
                        <td>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <p align="right">
                                <asp:TextBox ID="ItemType" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                                    Width="74px" Visible="False"></asp:TextBox>
                                <asp:DropDownList ID="InteractionTypeDropList" Font-Size="XX-Small" Font-Names="Arial"
                                    runat="server" Width="139px" AutoPostBack="True">
                                </asp:DropDownList></p>
                        </td>
                        <td style="width: 2px">
                        </td>
                        <td style="width: 220px">
                            <font face="Arial" size="1">Interaction Type</font></td>
                        <td>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <p align="right">
                                <asp:TextBox ID="noResponseMsg" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                                    Width="272px" TextMode="MultiLine" Height="80px"></asp:TextBox></p>
                        </td>
                        <td style="width: 2px">
                        </td>
                        <td style="width: 220px">
                            <font face="Arial" size="1">No Response Message</font></td>
                        <td>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <p align="right">
                                <asp:TextBox ID="SightRange" Font-Size="XX-Small" Font-Names="Arial" runat="server"
                                    Width="74px"></asp:TextBox></p>
                        </td>
                        <td style="width: 2px">
                        </td>
                        <td style="width: 220px">
                            <font face="Arial" size="1">Sight Range</font></td>
                        <td>
                        </td>
                    </tr>
                    <tr>
                        <td align="right">
                            <asp:TextBox ID="m_botBrain" Font-Names="Arial" Font-Size="XX-Small" Width="74px"
                                runat="server"></asp:TextBox></td>
                        <td style="width: 2px">
                        </td>
                        <td style="width: 220px">
                            <font face="Arial" size="1">Bot Brain (blank is none)</font></td>
                        <td>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
        <tr>
            <td valign="top" width="300" bgcolor="#99cc99">
                <p align="left">
                    &nbsp;</p>
            </td>
        </tr>
    </table>

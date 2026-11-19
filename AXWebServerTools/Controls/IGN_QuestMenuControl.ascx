<%@ Register TagPrefix="uc1" TagName="IGNEntityChatStrings" Src="IGNEntityChatStrings.ascx" %>
<%@ Control Language="c#" AutoEventWireup="true" CodeFile="IGN_QuestMenuControl.ascx.cs" Inherits="IGN_QuestMenuControl"%>
<%@ Register TagPrefix="uc1" TagName="IGNEntityQuestsControl" Src="IGNEntityQuestsControl.ascx" %>
<%@ Register TagPrefix="uc1" TagName="IGNEntityBaseControl" Src="IGNEntityBaseControl.ascx" %>

<asp:ScriptManager ID="ScriptManager1" runat="server">
</asp:ScriptManager>
<asp:UpdatePanel ID="UpdatePanel1" runat="server">
    <ContentTemplate>

<table id="Table2" style="width: 744px; height: 203px" cellspacing="1" cellpadding="1"
        width="744" bgcolor="gray" border="0">
        <tr>
            <td style="height: 20px" bgcolor="appworkspace">
                <strong><font face="Arial" color="#ffffff" size="2">IGN Quest Bot Menu</font></strong></td>
            <td style="height: 20px" bgcolor="appworkspace">
                <font face="Arial" color="#ffffff" size="2"><strong></strong></font>
            </td>
        </tr>
        <tr>
            <td style="height: 22px" valign="top" bgcolor="appworkspace">
                <asp:Button ID="UpdateIGNServer" runat="server" Width="272px" BorderStyle="Solid"
                    BorderColor="Black" Text="Update Selected Entity On Selected Server" Height="24px" OnClick="UpdateIGNServer_Click">
                </asp:Button></td>
            <td style="height: 22px" valign="top" bgcolor="appworkspace">
                <table id="Table1" style="width: 165px; height: 8px" cellspacing="0" cellpadding="0"
                    width="165" border="0">
                    <tr>
                        <td>
                            <asp:Button ID="Properties" runat="server" Width="80px" BorderStyle="Solid" BorderColor="Black"
                                Text="Properties" Height="24px"></asp:Button></td>
                        <td>
                            <asp:Button ID="QuestBtn" runat="server" Width="80px" BorderStyle="Solid" BorderColor="Black"
                                Text="Quest Data" Height="24px"></asp:Button></td>
                        <td>
                            <asp:Button ID="ChatDataBtn" Height="24px" Text="Chat Data" BorderColor="Black" BorderStyle="Solid"
                                Width="80px" runat="server"></asp:Button></td>
                    </tr>
                </table>
            </td>
        </tr>
        <tr>
            <td valign="top" width="300" bgcolor="#99cc99" style="height: 231px">
                <div align="left">
                    <asp:Panel ID="FolderManagementPanel" Height="96px" BorderColor="DimGray" BorderStyle="Outset"
                        Width="288px" runat="server" Font-Names="Arial" Font-Size="XX-Small" HorizontalAlign="Left">
                        <p>
                            <strong>Management</strong></p>
                        <p>
                            <table id="Table4" style="width: 272px; height: 8px" cellspacing="0" cellpadding="0"
                                width="272" border="0">
                                <tr>
                                    <td>
                                        <p>
                                            <asp:TextBox ID="NewFolderName" Width="232px" runat="server" Font-Size="XX-Small"
                                                Font-Names="Arial"></asp:TextBox><font face="Arial" size="1">Name</font></p>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="height: 19px">
                                        <asp:Button ID="NewEntityBtn" Text="New Entity" BorderColor="Black" BorderStyle="Solid"
                                            Width="65px" runat="server" Font-Size="XX-Small" Font-Names="Arial"></asp:Button>
                                        <asp:Button ID="NewFolderButton" runat="server" BorderColor="Black" BorderStyle="Solid"
                                            Font-Names="Arial" Font-Size="XX-Small" Text="New Folder" Width="65px" />
                                        <asp:Button ID="ReplaceFolder" Text="Replace" BorderColor="Black" BorderStyle="Solid"
                                            Width="65px" runat="server" Font-Size="XX-Small" Font-Names="Arial"></asp:Button>
                                        <asp:Button ID="DeleteFolderBtn" Text="Delete" BorderColor="Black" BorderStyle="Solid"
                                            Width="65px" runat="server" Font-Size="XX-Small" Font-Names="Arial"></asp:Button></td>
                                </tr>
                                <tr>
                                    <td style="height: 2px">
                                        <table id="Table3" style="width: 168px; height: 8px" cellspacing="0" cellpadding="0"
                                            width="168" border="0">
                                            <tr>
                                                <td>
                                                    <asp:Button ID="DeleteFolderConfBtn" Text="Yes" BorderColor="Black" BorderStyle="Solid"
                                                        Width="32px" runat="server" Font-Size="XX-Small" Font-Names="Arial" Visible="False">
                                                    </asp:Button></td>
                                                <td style="width: 8px">
                                                    <asp:Button ID="DeleteFolderCancelBtn" Text="No" BorderColor="Black" BorderStyle="Solid"
                                                        Width="32px" runat="server" Font-Size="XX-Small" Font-Names="Arial" Visible="False">
                                                    </asp:Button></td>
                                                <td>
                                                    <asp:Label ID="AreYouSureLabel" runat="server" Font-Size="XX-Small" Font-Names="Arial"
                                                        Visible="False">Are You Sure??</asp:Label></td>
                                            </tr>
                                        </table>
                                        <asp:Label ID="FolderErrorOutput" runat="server" Font-Size="XX-Small" Font-Names="Arial"
                                            ForeColor="Yellow" Font-Bold="True"></asp:Label></td>
                                </tr>
                                <tr>
                                    <td style="height: 2px">
                                        <asp:TextBox ID="entitySearchCriteria" Width="88px" runat="server" Font-Size="XX-Small"
                                            Font-Names="Arial"></asp:TextBox>
                                        <asp:Button ID="EntitySearchBtn" Text="Entity Search" BorderColor="Black" BorderStyle="Solid"
                                            Width="80px" runat="server" Font-Size="XX-Small" Font-Names="Arial"></asp:Button></td>
                                </tr>
                                <tr>
                                    <td style="height: 2px">
                                        <asp:ListBox ID="EntitySearchResultsList" Height="152px" Width="288px" runat="server"
                                            Font-Size="XX-Small" Font-Names="Arial" AutoPostBack="True"></asp:ListBox></td>
                                </tr>
                            </table>
                        </p>
                    </asp:Panel>
                </div>
                <asp:Label ID="m_menuOutput" runat="server" Font-Names="Arial" Font-Size="XX-Small"></asp:Label>
            </td>
            <td valign="top" bgcolor="#99cc99" style="height: 231px">
                <uc1:IGNEntityBaseControl id="IGNEntityBaseControl1" runat="server">
                </uc1:IGNEntityBaseControl>
                <uc1:IGNEntityQuestsControl id="IGNEntityQuestsControl1" runat="server">
                </uc1:IGNEntityQuestsControl>
                <uc1:IGNEntityChatStrings id="IGNEntityChatStrings1" runat="server">
                </uc1:IGNEntityChatStrings></td>
        </tr>
        <tr>
            <td valign="top" width="300" bgcolor="#99cc99">
            </td>
            <td valign="top" bgcolor="#99cc99">
            </td>
        </tr>
</table>
    </ContentTemplate>
</asp:UpdatePanel>

<%@ Control Language="c#" AutoEventWireup="true" CodeFile="NavControl.ascx.cs" Inherits="NavControl"%>
<table id="Table1" style="width: 280px; height: 146px" cellspacing="1" cellpadding="1"
    width="280" bgcolor="gray" border="0">
    <tr>
        <td style="height: 6px">
            <font face="Arial Black" color="#ffffff">Navigation</font></td>
    </tr>
    <tr>
        <td style="height: 6px">
            <p>
                <asp:DropDownList ID="m_serverList" Width="184px" Font-Names="Arial" Font-Size="XX-Small"
                    runat="server">
                </asp:DropDownList>&nbsp;<font face="Arial" size="1" color="#ffffff">Select Server</font></p>
        </td>
    </tr>
    <tr>
        <td valign="top" align="left">
            <table id="Table2" style="width: 184px; height: 64px" cellspacing="1"
                cellpadding="1" width="184" bgcolor="darkslategray" border="0">
                <tr>
                    <td align="center" style="height: 62px">
                        <asp:Button ID="m_showStatus" Width="50px" Font-Names="Arial" Font-Size="XX-Small"
                            runat="server" Height="50px" Text="Status" ToolTip="Servers Status" BorderStyle="Solid"
                            BorderColor="Black"></asp:Button></td>
                    <td align="center" style="height: 62px">
                        <asp:Button ID="LogPageBtn" Width="50px" Font-Names="Arial" Font-Size="XX-Small"
                            runat="server" Height="50px" Text="M Log" ToolTip="Management Server Log" BorderStyle="Solid"
                            BorderColor="Black"></asp:Button></td>
                    <td align="center" style="height: 62px">
                        <asp:Button ID="aisreverLogBtn" Width="50px" Font-Names="Arial" Font-Size="XX-Small"
                            runat="server" Height="50px" Text="AILog" ToolTip="Ai Server Log" BorderStyle="Solid"
                            BorderColor="Black"></asp:Button></td>
                    <td align="center" style="height: 62px">
                        <asp:Button ID="AcManageBtn" runat="server" Font-Size="XX-Small" Font-Names="Arial"
                            Width="50px" BorderColor="Black" BorderStyle="Solid" ToolTip="Account Manager"
                            Text="ACM" Height="50px"></asp:Button></td>
                    <td align="center" style="height: 62px">
                        <asp:Button ID="Button1" runat="server" Font-Size="XX-Small" Font-Names="Arial" Width="50px"
                            BorderColor="Black" BorderStyle="Solid" ToolTip="Server Tools" Text="ST" Height="50px">
                        </asp:Button></td>
                    <td align="center" style="height: 62px">
                        <asp:Button ID="MainSvrSettings" runat="server" Font-Size="XX-Small" Font-Names="Arial"
                            Width="50px" BorderColor="Black" BorderStyle="Solid" ToolTip="Main Server Settings"
                            Text="SVRS" Height="50px"></asp:Button></td>
                    <td align="center" style="height: 62px">
                        <asp:Button ID="AI_BaseWorldManager" runat="server" Font-Size="XX-Small" Font-Names="Arial"
                            Width="50px" BorderColor="Black" BorderStyle="Solid" ToolTip="AI World Management (OBSOLETE)"
                            Text="AI wld" Height="50px" Enabled="False"></asp:Button></td>
                    <td align="center" style="height: 62px">
                        <asp:Button ID="WorldManagerBtn" runat="server" Font-Size="XX-Small" Font-Names="Arial"
                            Width="50px" BorderColor="Black" BorderStyle="Solid" ToolTip="Zone Stencil Designer"
                            Text="ZONES" Height="50px" OnClick="WorldManagerBtn_Click1"></asp:Button></td>
                    <td align="center" style="height: 62px">
                        <asp:Button ID="SvrScriptsBtn" runat="server" Font-Size="XX-Small" Font-Names="Arial"
                            Width="50px" BorderColor="Black" BorderStyle="Solid" ToolTip="Server Scripts"
                            Text="Svr. Scr" Height="50px"></asp:Button></td>
                    <td align="center" style="height: 62px">
                        <asp:Button ID="QuestBtn" runat="server" Font-Size="XX-Small" Font-Names="Arial"
                            Width="50px" BorderColor="Black" BorderStyle="Solid" ToolTip="IGN Quest Data"
                            Text="Quest" Height="50px" OnClick="QuestBtn_Click"></asp:Button></td>
                    <td align="center" style="height: 62px">
                        <asp:Button ID="zoneStatusBtn" runat="server" BorderColor="Black" BorderStyle="Solid"
                            Font-Names="Arial" Font-Size="XX-Small" Height="50px" Style="vertical-align: middle;
                            text-align: center" Text="Zone S." ToolTip="Zone Status" Width="50px" OnClick="zoneStatusBtn_Click" /></td>
                    <td align="center" style="width: 3px; height: 62px">
                        <asp:Button ID="m_apxButton" runat="server" BorderColor="Black" BorderStyle="Solid"
                            Font-Names="Arial" Font-Size="XX-Small" Height="50px" Style="vertical-align: middle;
                            text-align: center" Text="APX" ToolTip="Web Item Interface" Width="50px" OnClick="webApxBtn_Click" /></td>
                </tr>
            </table>
            </td>
    </tr>
</table>

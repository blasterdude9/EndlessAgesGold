<%@ Control Language="c#" AutoEventWireup="true" CodeFile="ServerScriptsControl.ascx.cs" Inherits="ServerScripts"%>
&nbsp;<asp:ScriptManager ID="ScriptManager1" runat="server">
</asp:ScriptManager>
<asp:UpdatePanel ID="UpdatePanel1" runat="server">
    <ContentTemplate>
<table id="Table1" style="width: 416px; height: 352px" cellspacing="1" cellpadding="1"
    width="416" border="0">
    <tr>
        <td style="height: 1px" bgcolor="#808080">
            <font face="Arial" color="#ffffff" size="2"><strong>Server Scripts</strong></font></td>
    </tr>
    <tr>
        <td style="height: 4px" valign="top" align="left" bgcolor="#99cc99">
        </td>
    </tr>
    <tr>
        <td style="height: 95px" valign="top" align="left" bgcolor="#99cc99">
            <p align="center">
                <asp:ListBox ID="m_scriptList" runat="server" Width="391px" Font-Names="Arial" Font-Size="XX-Small"
                    Height="192px" AutoPostBack="True" OnSelectedIndexChanged="m_scriptList_SelectedIndexChanged"></asp:ListBox></p>
        </td>
    </tr>
    <tr>
        <td style="height: 171px" bgcolor="#99cc99">
            <table id="Table2" style="width: 400px; height: 322px" cellspacing="1" cellpadding="1"
                width="400" border="0">
                <tr>
                    <td style="width: 106px">
                        <p align="right">
                            <font face="Arial" size="1">Script Name</font></p>
                    </td>
                    <td>
                    </td>
                    <td>
                        <asp:TextBox ID="m_scriptName" runat="server" Width="216px" Font-Names="Arial" Font-Size="XX-Small"
                            Height="22px"></asp:TextBox></td>
                </tr>
                <tr>
                    <td style="width: 106px" align="right">
                        <font face="Arial"><font face="Arial" size="1">Game Specific</font></font></td>
                    <td>
                    </td>
                    <td>
                        <asp:TextBox ID="m_gameSpecific" Height="22px" Font-Size="XX-Small" Font-Names="Arial"
                            Width="128px" runat="server"></asp:TextBox></td>
                </tr>
                <tr>
                    <td style="width: 106px">
                        <p align="right">
                            <font face="Arial" size="1">Script Basis</font></p>
                    </td>
                    <td>
                    </td>
                    <td>
                        <asp:TextBox ID="m_scriptBasis" runat="server" Width="56px" Font-Names="Arial" Font-Size="XX-Small"
                            Height="22px"></asp:TextBox></td>
                </tr>
                <tr>
                    <td style="width: 106px">
                        <p align="right">
                            <font face="Arial" size="1">Script Unique ID</font></p>
                    </td>
                    <td>
                    </td>
                    <td>
                        <asp:TextBox ID="m_scriptUniqueID" runat="server" Width="56px" Font-Names="Arial"
                            Font-Size="XX-Small" Height="22px"></asp:TextBox></td>
                </tr>
                <tr>
                    <td style="width: 106px" align="right">
                        <font face="Arial" size="1">edb specific</font></td>
                    <td style="font-size: 12pt; font-family: Times New Roman">
                    </td>
                    <td style="font-size: 12pt; font-family: Times New Roman">
                        <asp:TextBox ID="m_edbSpecific" Height="22px" Font-Size="XX-Small" Font-Names="Arial"
                            Width="56px" runat="server"></asp:TextBox></td>
                </tr>
                <tr style="font-size: 12pt; font-family: Times New Roman">
                    <td style="width: 106px">
                        <p align="right">
                            <font face="Arial" size="1">Description</font></p>
                    </td>
                    <td>
                        <font face="Arial" size="1"></font>
                    </td>
                    <td>
                        <asp:TextBox ID="m_description" Height="54px" Font-Size="XX-Small" Font-Names="Arial"
                            Width="281px" runat="server" TextMode="MultiLine"></asp:TextBox></td>
                </tr>
                <tr style="font-size: 12pt; font-family: Times New Roman">
                    <td style="width: 106px">
                        <p align="right">
                            <font face="Arial" size="1">Script Data</font></p>
                    </td>
                    <td>
                    </td>
                    <td>
                        <asp:TextBox ID="m_scriptData" Height="176px" Font-Size="XX-Small" Font-Names="Arial"
                            Width="280px" runat="server" TextMode="MultiLine"></asp:TextBox></td>
                </tr>
                <tr style="font-size: 12pt; font-family: Times New Roman">
                    <td style="width: 106px">
                    </td>
                    <td>
                    </td>
                    <td>
                        <asp:DropDownList ID="m_functionList" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                            Width="281px">
                        </asp:DropDownList></td>
                </tr>
            </table>
            <asp:Label ID="m_errOut" runat="server" Font-Bold="True" Font-Names="Arial" Font-Size="XX-Small"></asp:Label></td>
    </tr>
    <tr>
        <td bgcolor="#808080">
            <asp:Button ID="AddBtn" runat="server" Width="80px" Font-Names="Arial" Font-Size="XX-Small"
                Text="Add" BorderStyle="Solid"></asp:Button>
            <asp:Button ID="ReplaceBtn" runat="server" Width="80px" Font-Names="Arial" Font-Size="XX-Small"
                Text="Replace" BorderStyle="Solid"></asp:Button>
            <asp:Button ID="DeleteBtn" runat="server" Width="80px" Font-Names="Arial" Font-Size="XX-Small"
                Text="Delete" BorderStyle="Solid"></asp:Button>
            <asp:Button ID="AquireBtn" runat="server" Width="113px" Font-Names="Arial" Font-Size="XX-Small"
                Text="Update Server" BorderStyle="Solid" ForeColor="#C04000" OnClick="AquireBtn_Click1"></asp:Button></td>
    </tr>
</table>
    </ContentTemplate>
</asp:UpdatePanel>
&nbsp;

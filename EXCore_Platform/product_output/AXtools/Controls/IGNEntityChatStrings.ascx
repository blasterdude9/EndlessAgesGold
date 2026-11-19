<%@ control language="c#" autoeventwireup="true" inherits="IGNEntityChatStrings, App_Web_ojrkv221" %>
<table id="Table2" style="width: 544px; height: 237px" cellspacing="1" cellpadding="1"
    width="544" bgcolor="gray" border="0">
    <tr>
        <td style="width: 337px; height: 20px" bgcolor="appworkspace">
            <strong><font face="Arial" color="#ffffff" size="2">Sequenced Speech</font></strong></td>
    </tr>
    <tr>
        <td style="width: 337px" valign="top" width="337" bgcolor="#99cc99">
            <div align="left">
                <asp:ListBox ID="chatStringsList" runat="server" Width="536px" Height="216px" Font-Names="Arial"
                    Font-Size="XX-Small" AutoPostBack="True"></asp:ListBox></div>
        </td>
    </tr>
    <tr>
        <td style="width: 337px" valign="top" width="337" bgcolor="#99cc99">
            <div align="left">
                <asp:TextBox ID="m_sequencedSpeechEdit" runat="server" Width="536px" Height="88px"
                    Font-Names="Arial" Font-Size="XX-Small" TextMode="MultiLine"></asp:TextBox></div>
        </td>
    </tr>
    <tr>
        <td style="width: 337px" valign="top" width="337" bgcolor="#99cc99">
            <asp:Button ID="AddNewBtn" runat="server" Width="96px" BorderColor="Black" Text="Add"
                Font-Names="Arial" Font-Size="XX-Small"></asp:Button>&nbsp;
            <asp:Button ID="DeleteBtn" runat="server" Width="96px" BorderColor="Black" Text="Delete"
                Font-Names="Arial" Font-Size="XX-Small"></asp:Button>&nbsp;
            <asp:Button ID="ReplaceBtn" runat="server" Width="96px" BorderColor="Black" Text="Replace"
                Font-Names="Arial" Font-Size="XX-Small"></asp:Button></td>
    </tr>
    <tr>
        <td style="width: 337px" valign="top" width="337" bgcolor="#99cc99">
        </td>
    </tr>
</table>

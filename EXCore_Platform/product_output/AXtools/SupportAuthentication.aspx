<%@ page language="c#" autoeventwireup="false" inherits="WebForm2, App_Web_ivhlxkco" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" >
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <title>SupportManager</title>
</head>
<body>
    <form runat="server" id="Form1">
        <table id="Table2" cellspacing="2" cellpadding="2" width="272" border="0">
            <tr>
                <td valign="top" bgcolor="#839e9e" height="18">
                    <asp:Panel ID="AuthPanel" runat="server" Font-Size="X-Small" Font-Names="Arial">
                        <strong>&nbsp;Authentication</strong>
                            <table id="Table1" style="width: 243px; height: 8px" cellspacing="1" cellpadding="1"
                                width="243" align="right" border="0">
                                <tr>
                                    <td style="width: 91px" width="91">
                                        <font size="1">REP NAME</font></td>
                                    <td>
                                        <p align="right">
                                            <asp:TextBox ID="m_repName" runat="server" Width="131px"></asp:TextBox></p>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="width: 91px" width="91">
                                        <font size="1">PASSWORD</font></td>
                                    <td>
                                        <p align="right">
                                            <asp:TextBox ID="m_password" runat="server" Width="131px" TextMode="Password"></asp:TextBox></p>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="width: 91px" width="91">
                                        <font size="1"></font>
                                    </td>
                                    <td>
                                        <p align="right">
                                            <asp:Label ID="m_output" runat="server" Font-Size="XX-Small"></asp:Label></p>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="width: 91px" width="91">
                                        <p align="right">
                                            &nbsp;</p>
                                    </td>
                                    <td>
                                        <p align="right">
                                            <asp:Button ID="m_sendButton" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                Width="95px" BorderColor="Black" BorderStyle="Solid" Text="Login" OnClick="m_sendButton_Click"></asp:Button></p>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="width: 91px" width="91">
                                    </td>
                                    <td>
                                        <p align="right">
                                            <asp:Button ID="ChangePasswordBtn" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                Width="95px" BorderColor="Black" BorderStyle="Solid" Text="Change Password"></asp:Button></p>
                                    </td>
                                </tr>
                            </table>
                    </asp:Panel>
                    <asp:Panel ID="ChangePassPanel" runat="server" Font-Size="X-Small" Font-Names="Arial"
                        Font-Bold="True" Visible="False">
                        <p>Change Password Panel</p>
                            <table id="Table3" style="width: 243px; height: 104px" cellspacing="1"
                                cellpadding="1" width="243" align="right" border="0">
                                <tr>
                                    <td style="width: 91px" width="91">
                                        <asp:Label ID="usernameLabel" runat="server" Font-Names="Arial" Font-Size="XX-Small">USERNAME</asp:Label></td>
                                    <td>
                                        <p align="right">
                                            <asp:TextBox ID="ChangePassUsernameEdit" runat="server" Width="131px"></asp:TextBox></p>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="width: 91px" width="91">
                                        <font size="1">
                                            <asp:Label ID="ExistingPassLabel" runat="server" Font-Names="Arial" Font-Size="XX-Small">EXISTING PASS</asp:Label></font></td>
                                    <td>
                                        <p align="right">
                                            <asp:TextBox ID="ExistingPassEdit" runat="server" Width="131px" TextMode="Password"></asp:TextBox></p>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="width: 91px" width="91">
                                        <font size="1">
                                            <asp:Label ID="newPassLabel" runat="server">NEW PASS</asp:Label></font></td>
                                    <td>
                                        <p align="right">
                                            <asp:TextBox ID="NewPasswordEdit" runat="server" Width="131px" TextMode="Password"></asp:TextBox></p>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="width: 91px" width="91">
                                        <font size="1"><font size="1">
                                            <asp:Label ID="ConfPassLabel" runat="server">CONF PASS</asp:Label></font></font></td>
                                    <td>
                                        <p align="right">
                                            <asp:TextBox ID="confPasswordEdit" runat="server" Width="131px" TextMode="Password"></asp:TextBox></p>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="width: 91px" width="91">
                                    </td>
                                    <td>
                                        <p align="right">
                                            <asp:Button ID="ChangePassBtn" runat="server" Font-Names="Arial" Font-Size="XX-Small"
                                                Width="95px" BorderColor="Black" BorderStyle="Solid" Text="Change Password"></asp:Button></p>
                                    </td>
                                </tr>
                            </table>
                        <p align="right">
                            &nbsp;</p>
                    </asp:Panel>
                    <p align="right">
                        &nbsp;</p>
                </td>
            </tr>
        </table>
        <p align="right">
            &nbsp;</p>
        <p align="right">
            &nbsp;</p>
    </form>
</body>
</html>

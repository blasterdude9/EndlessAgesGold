<%@ Page language="c#" Codebehind="game.aspx.cs" AutoEventWireup="false" Inherits="GW_Network.Forms.game" %>
<%@ Register TagPrefix="uc1" TagName="GW_VoteViewCommentsComponent" Src="../Components/GW_VoteViewCommentsComponent.ascx" %>
<%@ Register TagPrefix="uc1" TagName="GW_AddVoteCommentComponent" Src="../Components/GW_AddVoteCommentComponent.ascx" %>
<%@ Register TagPrefix="uc1" TagName="GW_LeftMenuComponent" Src="../Components/GW_LeftMenuComponent.ascx" %>
<%@ Register TagPrefix="uc1" TagName="GW_VoteComponent" Src="../Components/GW_VoteComponent.ascx" %>
<%@ Register TagPrefix="uc1" TagName="GW_BannerFrontEndComponent" Src="../Components/GW_BannerFrontEndComponent.ascx" %>
<HTML>
	<HEAD>
		<title>GameWorlds :: Ancient</title>
		<meta content="Microsoft Visual Studio .NET 7.1" name="GENERATOR">
		<meta content="C#" name="CODE_LANGUAGE">
		<meta content="JavaScript" name="vs_defaultClientScript">
		<meta content="http://schemas.microsoft.com/intellisense/ie5" name="vs_targetSchema">
		<LINK href="../style/styleNN.css" type="text/css" rel="StyleSheet">
		<style type="text/css">@import url( ../style/style.css ); 
		</style>
	</HEAD>
	<body style="BACKGROUND-REPEAT: no-repeat" bottomMargin="0" bgColor="#000000" leftMargin="0"
		background="../ancientimages/placeholderbg.jpg" topMargin="0" rightMargin="0" MS_POSITIONING="GridLayout">
		<form id="Form1" method="post" runat="server">
			<!-- Start GWNetwork Ancient Code -->
			<table id="table6" cellSpacing="0" cellPadding="0" width="100%" border="0">
				<tr>
					<td vAlign="top" width="609">
						<div align="right">
							<table id="table20" cellSpacing="0" cellPadding="0" width="14%" border="0">
								<tr>
									<td><IMG height="73" src="../ancientimages/left-menu-top.jpg" width="168" border="0"></td>
								</tr>
								<tr>
									<td><IMG height="11" src="../ancientimages/left-menu-arch.jpg" width="168" border="0"></td>
								</tr>
								<tr>
									<td vAlign="top" background="../ancientimages/left-menu-bg.jpg">
										<P>
											<uc1:GW_LeftMenuComponent id="GW_LeftMenuComponent1" runat="server"></uc1:GW_LeftMenuComponent></P>
										<P>&nbsp;</P>
										<P><BR>
											<BR>
											&nbsp;</P>
									</td>
								</tr>
								<tr>
									<td vAlign="top" background="../ancientimages/left-menu-bg.jpg"><IMG height="61" src="../ancientimages/sponsered.jpg" width="168" border="0"></td>
								</tr>
								<tr>
									<td vAlign="top" background="../ancientimages/left-menu-bg.jpg"><IMG height="52" src="../ancientimages/left-menu-bottom.jpg" width="168" border="0"></td>
								</tr>
							</table>
						</div>
					</td>
					<td vAlign="top" width="688">
						<div align="center">
							<table id="table7" cellSpacing="0" cellPadding="0" border="0">
								<tr>
									<td>
										<p align="right"><IMG height="233" src="../ancientimages/spacer-left.jpg" width="13" border="0"><img border="0" src="../ancientimages/gameui/hilt-left.jpg" width="243" height="233"></p>
									</td>
									<td>
										<p align="center"><IMG height="233" src="../ancientimages/logo.jpg" width="176" border="0"></p>
									</td>
									<td>
										<img border="0" src="../ancientimages/gameui/hilt-right.jpg" width="243" height="233"><IMG height="233" src="../ancientimages/spacer-right.jpg" width="13" border="0"></td>
								</tr>
							</table>
							<table border="0" width="63%" id="table26" cellspacing="0" cellpadding="0">
								<tr>
									<td colspan="3">
										<p align="center">
											<img border="0" src="../ancientimages/gameui/ui-top.jpg" width="663" height="38"></p>
									</td>
								</tr>
								<tr>
									<td width="31" style="WIDTH: 31px; HEIGHT: 461px" align="right">
										<p align="right">
											<img border="0" src="../ancientimages/gameui/ui-left.jpg" width="27" height="467"></p>
									</td>
									<td width="603" background="../ancientimages/gameui/ui-bg.jpg" vAlign="middle" style="WIDTH: 603px; HEIGHT: 461px"
										align="center">
										<OBJECT height="449" width="589" align="middle" codeBase="http://72.29.77.23/GW_GAMES/axpcodebase.cab" classid="clsid:3C403675-B43C-410B-BF56-D4D1FB68356C" VIEWASTEXT>
											<PARAM NAME="_Version" VALUE="65536">
											<PARAM NAME="_ExtentX" VALUE="12467">
											<PARAM NAME="_ExtentY" VALUE="9504">
											<PARAM NAME="_StockProps" VALUE="0">
										</OBJECT>
									</td>
									<td width="4%" style="HEIGHT: 461px">
										<img border="0" src="../ancientimages/gameui/ui-right.jpg" width="28" height="467"></td>
								</tr>
								<tr>
									<td colspan="3">
										<p align="center">
											<img border="0" src="../ancientimages/gameui/ui-bottom.jpg" width="663" height="24"></p>
									</td>
								</tr>
							</table>
						</div>
					</td>
					<td vAlign="top" align="right">
						<div align="left">
							<table id="table25" cellSpacing="0" cellPadding="0" width="14%" border="0">
								<tr>
									<td><IMG height="73" src="../ancientimages/right-menu-top.jpg" width="168" border="0"></td>
								</tr>
								<tr>
									<td>
										<center><IMG height="11" src="../ancientimages/left-menu-arch.jpg" width="168" border="0"></center>
									</td>
								</tr>
								<tr>
									<td vAlign="top" width="60%" background="../ancientimages/left-menu-bg.jpg">
										<TABLE id="Table1" style="WIDTH: 106px; HEIGHT: 13px" cellSpacing="0" cellPadding="0" width="106"
											align="center" border="0">
											<TR>
												<TD colSpan="3">
													<P>
														<uc1:GW_VoteComponent id="GW_VoteComponent1" runat="server"></uc1:GW_VoteComponent></P>
												</TD>
											</TR>
										</TABLE>
									</td>
								</tr>
								<tr>
									<td vAlign="top" align="center" background="../ancientimages/menu-bg.jpg">
										<P><IMG height="61" src="../ancientimages/sponsered.jpg" width="168" border="0">
											<uc1:GW_BannerFrontEndComponent id="GW_BannerFrontEndComponent1" runat="server"></uc1:GW_BannerFrontEndComponent></P>
										<P>
											<uc1:GW_BannerFrontEndComponent id="GW_BannerFrontEndComponent2" runat="server"></uc1:GW_BannerFrontEndComponent></P>
										<P>
											<uc1:GW_BannerFrontEndComponent id="GW_BannerFrontEndComponent3" runat="server"></uc1:GW_BannerFrontEndComponent></P>
									</td>
								</tr>
								<tr>
									<td background="../ancientimages/left-menu-bg.jpg" valign="top">
										<img border="0" src="../ancientimages/left-menu-bottom.jpg" width="168" height="52"></td>
								</tr>
							</table>
						</div>
						<p>&nbsp;</p>
					</td>
				</tr>
			</table>
			<!-- End GWNetwork Ancient Code --></form>
	</body>
</HTML>

# EngineX: the client/editor executable - the top of the build chain.
# Originally ConfigurationType=1 (exe). Compiles 160 sources directly,
# including 103 AuraUI editor-dialog sources, 6 CBaseInterfaceModule and
# 2 MenuInterfaceModule - so those three modules are NOT linked here
# (their objects are built straight into the exe). Built as the AURAEDITOR
# flavor: the full editor (login, menus, housing zoner + all tools).
set(ENGINEX_SOURCES
    AXmediaFrame.cpp
    CrashReporter.cpp     # CrashLog.txt + message box instead of silent exit
    CFilePatchClass.cpp   # stub: original patch-builder source lost
    EngineXApp.cpp
    ocx2.cpp
    RMapp.cpp
    RMwin.cpp
    CurrentWorldDlg.cpp
    MergeOptionsDLG.cpp
    NewAccountsModule.cpp
    RuntimeEntityViewerDlg.cpp
    EngineXWin.cpp
    ../AuraUI/addmenu.cpp
    ../AuraUI/progressmeterdlg.cpp
    ../AuraUI/ServerSetupDlg.cpp
    ../AuraUI/NewAccountDlg.cpp
    ../AuraUI/objectlibrarydlg.cpp
    ../AuraUI/patchsystemdlg.cpp
    ../AuraUI/EditButton.cpp
    ../AuraUI/EditComboBox.cpp
    ../AuraUI/EditCursors.cpp
    ../AuraUI/EditEntity.cpp
    ../AuraUI/EditFont.cpp
    ../AuraUI/EditIcons.cpp
    ../AuraUI/EditImage.cpp
    ../AuraUI/EditInventory.cpp
    ../AuraUI/EditListBox.cpp
    ../AuraUI/EditMenuTitle.cpp
    ../AuraUI/EditProgBar.cpp
    ../AuraUI/EditScript.cpp
    ../AuraUI/EditScrollBar.cpp
    ../AuraUI/EditText.cpp
    ../AuraUI/EditTextBox.cpp
    ../AuraUI/EditTextureDlg.cpp
    ../AuraUI/EditToggle.cpp
    ../AuraUI/menuaddunitdlg.cpp
    LodSystemDlg.cpp
    ../AuraUI/MenuClickZoneDlg.cpp
    ../AuraUI/MenuEntityLinkDlg.cpp
    ../AuraUI/MenuTextObjectDlg.cpp
    ../AuraUI/AbilityEditorDlg.cpp
    ../AuraUI/AddAttachmentTypeDlg.cpp
    ../AuraUI/AddControlEventDlg.cpp
    ../AuraUI/AddDoTdlg.cpp
    ../AuraUI/AddEntityDlg.cpp
    ../AuraUI/AddHud.cpp
    ../AuraUI/additemdlg.cpp
    ../AuraUI/AddLightningStrikeDlg.cpp
    ../AuraUI/AddParticleSystemDLG.cpp
    ../AuraUI/addscripteventdlg.cpp
    AddSpawnGeneratorDlg.cpp
    AddTextureDlg.cpp
    ../AuraUI/advancednetworkdlg.cpp
    AIPathDlg.cpp
    ../AuraUI/AIRaidDbClass.cpp
    AIRaidDlg.cpp
    AISettingsDlg.cpp
    ../AuraUI/AITraceMapDlg.cpp
    ../AuraUI/AlphaTextureDlg.cpp
    ../AuraUI/AnimPropertiesDlg.cpp
    ../AuraUI/AnimSettingsDlg.cpp
    ../AuraUI/AnimTextureSwpAddDlg.cpp
    ../AuraUI/appendagecustdlg.cpp
    ../AuraUI/ArenaManagerDlg.cpp
    ../AuraUI/bankzonesdlg.cpp
    ../AuraUI/BarSettingsDlg.cpp
    CAISystemDlg.cpp
    ../AuraUI/CameraAdvDlg.cpp
    CEnvironmentSettingsDlg.cpp
    CExplosionsDatabaseDlg.cpp
    CharonaDlg.cpp
    CollisionResolutionDlg.cpp
    ColorDlg.cpp
    ../AuraUI/CommCrtlEditorDlg.cpp
    CommerceDlg.cpp
    ../AuraUI/ConnectionMethodDlg.cpp
    ../AuraUI/ContentPropertiesDlg.cpp
    ../AuraUI/controladdobjectdlg.cpp
    CSpawnGeneratorDlg.cpp
    CTextureDatabaseDlg.cpp
    ../AuraUI/CurrencyDlg.cpp
    ../AuraUI/CursorLimits.cpp
    ../AuraUI/CursorScaleDlg.cpp
    CWeaponSystemDlg.cpp
    ../AuraUI/DayStateDlg.cpp
    ../AuraUI/DmeshLodEditorDlg.cpp
    ../AuraUI/editstartinventorydlg.cpp
    effectSystemDlg.cpp
    ../AuraUI/ElementsDlg.cpp
    ../AuraUI/EntityItemTestDlg.cpp
    EntityLODSystemsDlg.cpp
    ../AuraUI/entitypropertiesdlg.cpp
    ExplosionBasicDlg.cpp
    ../AuraUI/FXDynPerametersDlg.cpp
    ../AuraUI/GenerateMeshDLG.cpp
    ../AuraUI/GetSignificantOptionsDlg.cpp
    ../AuraUI/GlobalAdvancedDlg.cpp
    ../AuraUI/GroupBoundingSetDlg.cpp
    ../AuraUI/groupmodifierdlg.cpp
    ../AuraUI/houseplacementzonedlg.cpp
    ../AuraUI/housingsysdlg.cpp
    ../AuraUI/HudAddDisplayUnitDlg.cpp
    ../AuraUI/IconSysDlg.cpp
    ItemConflictDlg.cpp
    ItemManagerDlg.cpp
    LaserDlg.cpp
    LightInterfaceDlg.cpp
    ../AuraUI/LightSettingsDlg.cpp
    ../AuraUI/LoginDlg.cpp
    MaterialDlg.cpp
    ../AuraUI/MaterialShaderLibDlg.cpp
    MissileAddBillboardDlg.cpp
    MissileBaseMeshEdit.cpp
    MotionSettingsDlg.cpp
    ../AuraUI/MovieSettingsDlg.cpp
    ../AuraUI/nodeimportdlg.cpp
    ParticleInterfaceDlg.cpp
    ../AuraUI/particlesystem_both.cpp
    ../AuraUI/particlesystem_directional.cpp
    ../AuraUI/particlesystem_nondirectional.cpp
    PathDialog.cpp
    ../AuraUI/placementdlg.cpp
    ../AuraUI/PortalsDlg.cpp
    ReferenceLibrary.cpp
    ../AuraUI/ResourceEditorDlg.cpp
    ../AuraUI/SafeZoneDlg.cpp
    ../AuraUI/ScriptAdd.cpp
    ../AuraUI/ScriptEventAdd.cpp
    ShaderLibDlg.cpp
    ../AuraUI/ShadowDlg.cpp
    SkeletalAnimationDlg.cpp
    ../AuraUI/skillbuilderdlg.cpp
    ../AuraUI/SkillEditorDlg.cpp
    ../AuraUI/SkillRangesDlg.cpp
    ../AuraUI/SkillsOnCharEditorDlg.cpp
    ../AuraUI/skillsystemdlg.cpp
    ../AuraUI/sounddatabasedlg.cpp
    SoundRangeDlg.cpp
    SoundSettingsDlg.cpp
    SpawnGenDlg.cpp
    ../AuraUI/StatisticsDBDlg.cpp
    ../AuraUI/stormdlg.cpp
    ../AuraUI/SymbolMapDlg.cpp
    ../AuraUI/testDlg.cpp
    ../AuraUI/TestExportDlg.cpp
    TimeSystemDlg.cpp
    ../AuraUI/TransformAnimationDlg.cpp
    ../AuraUI/transformationdlg.cpp
    ../AuraUI/TreeImportOptionsDlg.cpp
    ../AuraUI/TriggerEditorDlg.cpp
    ../AuraUI/upgradewormdlg.cpp
    VertexAnimSettingsDlg.cpp
    ../AuraUI/VisualManagerDlg.cpp
    ../AuraUI/WaterZoneDBDLG.cpp
    ../AuraUI/WorldChannelDlg.cpp
    ../AuraUI/WorldGroupDlg.cpp
    ../AuraUI/worldobjectpropertiesdlg.cpp
    ../CBaseInterfaceModule/ActionZonesDlg.cpp
    ../MenuInterfaceModule/CMalInterface.cpp
    ../CBaseInterfaceModule/CmenuAccessInterface.cpp
    ../CBaseInterfaceModule/LoadScriptsDlg.cpp
    ../CBaseInterfaceModule/MusicDlg.cpp
    ../MenuInterfaceModule/NavUIDLG.cpp
    PatchSystemUpdateDlg.cpp
    ../CBaseInterfaceModule/PostEffectDlg.cpp
    SqlConnectionDlg.cpp
    ../CBaseInterfaceModule/TestDlg.cpp
)
# Resource scripts. The original EngineX.vcproj compiled all four into the exe:
# EngineX.rc (133 dialogs, main menu, icons) plus AuraUI.rc (the editor tool
# dialogs - 230+ templates), MenuInterfaceModule.rc and CBaseInterfaceModule.rc.
# Each .rc pulls its OWN resource.h (quote-include resolves beside the .rc), and
# the numeric IDs of the templates do not collide across the four (audited).
# Without the extra three, every CDialogBar::Create(IDD_...) for an AuraUI
# dialog fails in EngineXWin::OnCreate, the frame returns -1 and the app exits
# with no window and no error.
list(APPEND ENGINEX_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/EngineX.rc"
    "${CMAKE_CURRENT_SOURCE_DIR}/../AuraUI/AuraUI.rc"
    "${CMAKE_CURRENT_SOURCE_DIR}/../MenuInterfaceModule/MenuInterfaceModule.rc"
    "${CMAKE_CURRENT_SOURCE_DIR}/../CBaseInterfaceModule/CBaseInterfaceModule.rc")

# AuraUI.rc / MenuInterfaceModule.rc / CBaseInterfaceModule.rc each end with
# #include "afxres.rc". With _AFXDLL defined for the resource compiler that file
# contributes nothing (MFC's own resources live in mfc140.dll); without it we
# would get three copies of the MFC string tables and CVT1100 duplicate errors.
set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} /D_AFXDLL")
add_executable(EngineX WIN32 ${ENGINEX_SOURCES})

target_compile_definitions(EngineX PRIVATE
    WIN32 NDEBUG _WINDOWS _ENGLISH AURAEDITOR _AFXDLL _AFXEXT
    _CRT_SECURE_NO_WARNINGS _CRT_SECURE_NO_DEPRECATE)

target_include_directories(EngineX PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../AuraUI
    ${CMAKE_CURRENT_SOURCE_DIR}/../CBaseInterfaceModule
    ${CMAKE_CURRENT_SOURCE_DIR}/../MenuInterfaceModule
    ${CMAKE_SOURCE_DIR}/AuraX
    ${CMAKE_SOURCE_DIR}/HeaderFiles/CoreStatic
    ${CMAKE_SOURCE_DIR}/HeaderFiles/ClientBaseHeaders
    ${CMAKE_SOURCE_DIR}/HeaderFiles/InterfaceHeaders
    ${CMAKE_SOURCE_DIR}/HeaderFiles/NetworkHeaders
    ${CMAKE_SOURCE_DIR}/HeaderFiles/RenderModules)

# Lower-level libs EngineX links (NOT AuraUI/CBase/Menu - compiled in above).
target_link_libraries(EngineX PRIVATE
    GameEngine_ClientBase
    AuraX
    CDxRenderModule
    NetworkExtension
    CContentManagementLibrary
    AXSocketsLib
    ABWebComm
    AXCabinetLib
    d3d9 d3dx9 d3dxof dxguid dinput8 dsound
    winmm Version Wininet Framedyn Cabinet)

# ---------------------------------------------------------------------------
# EAEngineClient.exe - the 2003-style client (not the current client): the same
# engine app built with _ClientOutput. The original EngineX.vcproj had this as
# its "ClientOutput" configuration. _ClientOutput strips every editor tool
# (dialog bars, Nav UI, editor-only handlers) and RMWin creates a full-screen
# WS_POPUP window with CClientModule in play mode (editOn = FALSE). Everything
# the client does is driven by autoexec.aut; the client version number comes
# from VERSIONINF.VER beside the exe. No web portal, no content downloads.
# ---------------------------------------------------------------------------
add_executable(EAEngineClient WIN32 ${ENGINEX_SOURCES})
target_compile_definitions(EAEngineClient PRIVATE
    WIN32 NDEBUG _WINDOWS _ENGLISH _ClientOutput _AFXDLL _AFXEXT
    _CRT_SECURE_NO_WARNINGS _CRT_SECURE_NO_DEPRECATE)
get_target_property(_enginex_incs EngineX INCLUDE_DIRECTORIES)
target_include_directories(EAEngineClient PRIVATE ${_enginex_incs})
get_target_property(_enginex_libs EngineX LINK_LIBRARIES)
target_link_libraries(EAEngineClient PRIVATE ${_enginex_libs})

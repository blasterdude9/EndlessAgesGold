# CBaseInterfaceModule: base editor interface dialogs (action zones, music,
# post-effect, load-scripts, menu-access, test). Static lib linked into the
# client/editor exe. (KingdomsManagerDLG.cpp is in the old .vcproj but not on
# disk, so it is omitted.)
set(CBIM_SOURCES
    ActionZonesDlg.cpp
    CBaseInterfaceModule.cpp
    CmenuAccessInterface.cpp
    LoadScriptsDlg.cpp
    MusicDlg.cpp
    PostEffectDlg.cpp
    stdafx.cpp
    TestDlg.cpp
)
add_library(CBaseInterfaceModule STATIC ${CBIM_SOURCES})
target_compile_definitions(CBaseInterfaceModule PRIVATE WIN32 _LIB)
target_include_directories(CBaseInterfaceModule PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/AuraX)

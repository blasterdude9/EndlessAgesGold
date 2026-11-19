# EndlessAges.exe - the 2008 game client host app (CGameApp) - THE client build.
# (EngineMainApp also has EAEngineClient: the 2003-style _ClientOutput engine build.)
# (originally CGameApp / AXengineApp.exe,
# the 2008 client). A thin MFC frame around the same engine chain the editor uses:
# reads coinit.tmp, shows the login, runs GameFiles\<game>.aut and hands the window
# to CClientModule. Content updates are the launcher's job, not the client's.
# A thin MFC frame around the same engine chain the editor uses: it reads
# coinit.tmp, shows the login, validates/downloads content, runs
# GameFiles\<game>.aut and hands the window to CClientModule.
set(EA_CLIENT_SOURCES
    CGameApp.cpp
    ChildView.cpp
    MainFrm.cpp
    stdafx.cpp
    ../../ActiveXPortal/ocx2.cpp          # Windows Media Player control wrapper
    ../../EngineMainApp/CrashReporter.cpp # same CrashLog.txt reporter as the editor
    CGameApp.rc)

# CGameApp.rc ends with #include "afxres.rc"; with _AFXDLL for rc.exe it adds nothing.
set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} /D_AFXDLL")
add_executable(EndlessAges WIN32 ${EA_CLIENT_SOURCES})
set_target_properties(EndlessAges PROPERTIES OUTPUT_NAME "EndlessAges")

target_compile_definitions(EndlessAges PRIVATE
    WIN32 NDEBUG _WINDOWS _ENGLISH _AFXDLL _AFXEXT
    _CRT_SECURE_NO_WARNINGS _CRT_SECURE_NO_DEPRECATE)

# BEFORE: this target's own folder and ActiveXPortal must win the "ocx2.h" /
# "stdafx.h" / "resource.h" quote-includes over the shared HeaderFiles copies
# (there are three different ocx2.h in the tree; ocx2.cpp must see its own).
target_include_directories(EndlessAges BEFORE PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ActiveXPortal
    ${CMAKE_CURRENT_SOURCE_DIR}/../../EngineMainApp)   # CrashReporter.h only

target_link_libraries(EndlessAges PRIVATE
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

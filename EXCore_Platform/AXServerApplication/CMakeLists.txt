# AxserverApp.exe - the game server with a small MFC dialog as its console.
# Thin host around CServerProc (AXServerModule); same crash reporter as the
# editor/client (CrashLog.txt beside the exe).
set(AXSERVERAPP_SOURCES
    AXServerApplication.cpp
    AXServerApplicationDlg.cpp
    stdafx.cpp
    ../EngineMainApp/CrashReporter.cpp
    AXServerApplication.rc)

set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} /D_AFXDLL")
add_executable(AxserverApp WIN32 ${AXSERVERAPP_SOURCES})
set_target_properties(AxserverApp PROPERTIES OUTPUT_NAME "AxserverApp")

target_compile_definitions(AxserverApp PRIVATE
    WIN32 NDEBUG _WINDOWS _AXSERVER _AFXDLL _AFXEXT
    _CRT_SECURE_NO_WARNINGS _CRT_SECURE_NO_DEPRECATE)

target_include_directories(AxserverApp BEFORE PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../EngineMainApp)   # CrashReporter.h only

target_link_libraries(AxserverApp PRIVATE
    AXServerModule
    AuraX
    NetworkExtension
    ABWebComm
    AXCabinetLib
    enet
    d3d9 d3dx9 d3dxof dxguid winmm Version Wininet)

# 32-bit server: let it use up to 4 GB on 64-bit Windows (default is 2 GB)
target_link_options(AxserverApp PRIVATE /LARGEADDRESSAWARE)

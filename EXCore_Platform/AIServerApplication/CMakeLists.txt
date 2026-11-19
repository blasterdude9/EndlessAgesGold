# AIServerApp.exe - The AI server host application.
# Runs CCentralProcessor (CAIServerModule) to manage AI worlds and simulation;
# communicates with AxserverApp over ENet.
set(AISERVERAPP_SOURCES
    AIServerApplication.cpp
    AIServerApplicationDlg.cpp
    stdafx.cpp
    ../EngineMainApp/CrashReporter.cpp
    AIServerApplication.rc)

set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} /D_AFXDLL")
add_executable(AIServerApp WIN32 ${AISERVERAPP_SOURCES})
set_target_properties(AIServerApp PROPERTIES OUTPUT_NAME "AIServerApp")

target_compile_definitions(AIServerApp PRIVATE
    WIN32 NDEBUG _WINDOWS _AFXDLL
    _CRT_SECURE_NO_WARNINGS _CRT_SECURE_NO_DEPRECATE)

target_include_directories(AIServerApp BEFORE PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../CAIServerModule
    ${CMAKE_CURRENT_SOURCE_DIR}/../EngineMainApp
    ${CMAKE_SOURCE_DIR}/HeaderFiles/ClientBaseHeaders
    ${CMAKE_SOURCE_DIR}/HeaderFiles/CoreStatic
    ${CMAKE_SOURCE_DIR}/HeaderFiles/NetworkHeaders)

target_link_libraries(AIServerApp PRIVATE
    CAIServerModule
    GameEngine_ClientBase
    AuraX
    CDxRenderModule
    NetworkExtension
    CContentManagementLibrary
    AXSocketsLib
    ABWebComm
    AXCabinetLib
    enet
    d3d9 d3dx9 d3dxof dxguid dinput8 dsound winmm Version Wininet Framedyn Cabinet)

# 32-bit server: let it use up to 4 GB on 64-bit Windows (default is 2 GB)
target_link_options(AIServerApp PRIVATE /LARGEADDRESSAWARE)

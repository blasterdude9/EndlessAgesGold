# CAIServerModule: The AI server module (world simulation, AI logic/spawns,
# spatial partitions, communication with the main game server).
# Static library, originally ConfigurationType=4.
set(CAISERVER_SOURCES
    AIServerInitialization.cpp
    CentralProcessor.cpp
    LogSys.cpp
    WorldManager.cpp
    stdafx.cpp)

add_library(CAIServerModule STATIC ${CAISERVER_SOURCES})

target_compile_definitions(CAIServerModule PRIVATE
    WIN32 NDEBUG _LIB _AFXDLL _CRT_SECURE_NO_WARNINGS _CRT_SECURE_NO_DEPRECATE)

target_include_directories(CAIServerModule PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/AuraX
    ${CMAKE_SOURCE_DIR}/EXCore_Platform/GameEngine_ClientBase
    ${CMAKE_SOURCE_DIR}/HeaderFiles/ClientBaseHeaders
    ${CMAKE_SOURCE_DIR}/HeaderFiles/CoreStatic
    ${CMAKE_SOURCE_DIR}/HeaderFiles/NetworkHeaders)

target_link_libraries(CAIServerModule PUBLIC
    GameEngine_ClientBase
    AuraX
    NetworkExtension
    CContentManagementLibrary
    AXSocketsLib
    ABWebComm
    AXCabinetLib
    enet)

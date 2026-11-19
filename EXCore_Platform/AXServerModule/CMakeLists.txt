# AXServerModule: THE game server (login/accounts, movement, combat, quests,
# spawns, commerce, housing, arenas). Static lib, originally ConfigurationType=4.
# Hosted by AXServerApplication (dialog) / AXServerService (Windows service).
# Talks ENet (UDP 25857) through NetworkExtension; persistence is SQL Server via
# ADO (AuraX/DBConnectionSystem). Server cfg files sit beside the exe:
#   serverstartup.cfg, authAccount.cfg, baseAccount.cfg
set(AXSERVER_SOURCES
    AiSystems.cpp
    ArenaSystems.cpp
    AttackSystemsManagement.cpp
    AttributeSystems.cpp
    CDataHandler.cpp
    CharacterCreationSystems.cpp
    CharacterPendingLoader.cpp
    DamageSystems.cpp
    DmeUnifiedCommerce.cpp
    EventSystems.cpp
    QuestSystems.cpp
    ServerMain.cpp
    ServerMovementSystems.cpp
    ServerProc.cpp
    ServerScripts.cpp
    SharedNet.cpp
    SpawnSystems.cpp
    stdafx.cpp
    SystemsLoader.cpp
    TextSystems.cpp)

add_library(AXServerModule STATIC ${AXSERVER_SOURCES})

# _AXSERVER: the original "SvrRelease" configuration (server-only code paths).
target_compile_definitions(AXServerModule PRIVATE WIN32 NDEBUG _LIB _AXSERVER
    _AFXDLL _AFXEXT _CRT_SECURE_NO_DEPRECATE)

target_include_directories(AXServerModule PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(AXServerModule PUBLIC
    AuraX
    NetworkExtension
    ABWebComm
    enet)

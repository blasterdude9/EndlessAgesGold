# AXSocketsLib: raw TCP/UDP sockets layer (packet, client, server).
# Originally ConfigurationType=4 (static lib), UseOfMFC=1, MBCS.
set(AXSOCKETS_SOURCES
    AXPacketClass.cpp
    AXSocketClient.cpp
    AXSocketServer.cpp
    stdafx.cpp
)
add_library(AXSocketsLib STATIC ${AXSOCKETS_SOURCES})
target_compile_definitions(AXSocketsLib PRIVATE WIN32 _LIB)
target_include_directories(AXSocketsLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/AuraX)

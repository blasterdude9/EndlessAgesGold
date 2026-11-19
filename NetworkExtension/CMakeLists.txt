# NetworkExtension: MFC extension DLL for the networking layer (accounts, clan
# system, DirectPlay player management, packet queue, server items, multiplayer).
# Originally ConfigurationType=2 (DLL), _AFXEXT, linking AuraXDynamic.lib.
# This (with DirectPlay) is the networking transport slated for a modern UDP rewrite.
file(GLOB NETEXT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
# DirectPlay8 GUID definitions (legacy dxguid.lib no longer provides them).
list(APPEND NETEXT_SOURCES "${CMAKE_SOURCE_DIR}/thirdparty/directplay/dpguids.cpp")

add_library(NetworkExtension SHARED ${NETEXT_SOURCES})
set_target_properties(NetworkExtension PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)  # original .def missing; export all so EngineX can link
set_target_properties(NetworkExtension PROPERTIES OUTPUT_NAME "NetworkExtension")

target_compile_definitions(NetworkExtension PRIVATE
    WIN32 _WINDOWS _AFXEXT _CRT_SECURE_NO_WARNINGS)

target_include_directories(NetworkExtension PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR})

# Links the engine core (AuraXDynamic.lib) plus D3D; MFC comes from CMAKE_MFC_FLAG.
target_link_libraries(NetworkExtension PRIVATE AuraX enet d3d9 dxguid)

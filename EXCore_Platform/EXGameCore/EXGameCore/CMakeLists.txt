# EXGameCore: core game/engine support library (CAxList, CItemManager, AI script
# lists, clan/POI/currency server systems). Originally a static lib (.vcproj
# ConfigurationType=4) linked into AuraXDynamic.dll; same here.
file(GLOB EXGAMECORE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
# CAILogicScriptClass.cpp lives in AuraX/ but the original EXGameCore.vcproj
# compiled it here (reaching up into AuraX), so include it in this lib.
list(APPEND EXGAMECORE_SOURCES "${CMAKE_SOURCE_DIR}/AuraX/CAILogicScriptClass.cpp")
list(APPEND EXGAMECORE_SOURCES "${CMAKE_SOURCE_DIR}/EXCore_Platform/EXGameCore/SelectionEffect.cpp")  # lives one dir up; glob misses it

add_library(EXGameCore OBJECT ${EXGAMECORE_SOURCES})

target_compile_definitions(EXGameCore PRIVATE
    WIN32 _LIB _CRT_SECURE_NO_WARNINGS)

target_include_directories(EXGameCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/HeaderFiles/CoreStatic
    ${CMAKE_SOURCE_DIR}/HeaderFiles/ClientBaseHeaders
    ${CMAKE_SOURCE_DIR}/HeaderFiles/InterfaceHeaders
    ${CMAKE_SOURCE_DIR}/HeaderFiles/NetworkHeaders
    ${CMAKE_SOURCE_DIR}/HeaderFiles/RenderModules
    ${CMAKE_SOURCE_DIR}/HeaderFiles/Zpingheaders
    ${CMAKE_SOURCE_DIR}/thirdparty/directplay)

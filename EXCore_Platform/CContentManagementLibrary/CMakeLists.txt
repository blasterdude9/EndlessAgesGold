# CContentManagementLibrary: content client/server (asset streaming/management).
# Originally ConfigurationType=4 (static lib), UseOfMFC=1, MBCS. Static lib that
# links into the client/editor exe (EngineX).
set(CCONTENT_SOURCES
    AXContentClient.cpp
    AXContentServer.cpp
    stdafx.cpp
)
add_library(CContentManagementLibrary STATIC ${CCONTENT_SOURCES})
target_compile_definitions(CContentManagementLibrary PRIVATE WIN32 _LIB)
target_include_directories(CContentManagementLibrary PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/AuraX)

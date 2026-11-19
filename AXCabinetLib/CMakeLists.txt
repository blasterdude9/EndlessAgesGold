# AXCabinetLib: cabinet (.cab) packaging/extraction wrapper (FDI/FCI).
# Originally ConfigurationType=4 (static lib), MBCS. Links into EngineX.
set(AXCABINET_SOURCES
    CCabinetControl.cpp
    stdafx.cpp
)
add_library(AXCabinetLib STATIC ${AXCABINET_SOURCES})
target_compile_definitions(AXCabinetLib PRIVATE WIN32 _LIB)
target_include_directories(AXCabinetLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/AuraX)

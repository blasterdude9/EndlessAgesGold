# MenuInterfaceModule: menu-access + navigation UI dialog layer. Static lib
# linked into the client/editor exe.
add_library(MenuInterfaceModule STATIC
    CMalInterface.cpp
    MenuInterfaceModule.cpp
    NavUIDLG.cpp
    stdafx.cpp
)
target_compile_definitions(MenuInterfaceModule PRIVATE WIN32 _LIB)
target_include_directories(MenuInterfaceModule PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/AuraX)

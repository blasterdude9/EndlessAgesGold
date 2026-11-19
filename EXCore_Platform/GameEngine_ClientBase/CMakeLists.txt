# GameEngine_ClientBase: client-side game logic + the in-game menu/UI widget
# system (buttons, listboxes, textboxes, inventory menu, windows, etc).
# Static lib (original ConfigurationType=4) linked into the client exe (EngineX).
set(GECB_SOURCES
    AiManager.cpp
    ClientInitialization.cpp
    ClientModule.cpp
    ClientNetworkingLogic.cpp
    ClientScripts.cpp
    EntityManagement.cpp
    getdxver.cpp
    GlobalStatusVariables.cpp
    MenuManagement.cpp
    MissileExplosionManager.cpp
    NetworkGameLogic.cpp
    RenderingManagement.cpp
    SceneAssembly.cpp
    stdafx.cpp
    UserController.cpp
)
# The CMenu* widgets live in AuraX/ but were compiled by this project in the
# original .vcproj (they are not part of the AuraX DLL build).
foreach(m CMenuButton CMenuComboBox CMenuEntity CMenuFont CMenuImage CMenuInventory
          CMenuListBox CMenuProgressBar CMenuScript CMenuScrollBar CMenuText
          CMenuTextBox CMenuToggle CMenuVisual CMenuWindow CXMLFileIO)
    list(APPEND GECB_SOURCES ${CMAKE_SOURCE_DIR}/AuraX/${m}.cpp)
endforeach()

add_library(GameEngine_ClientBase STATIC ${GECB_SOURCES})

target_compile_definitions(GameEngine_ClientBase PRIVATE WIN32 _LIB)

target_include_directories(GameEngine_ClientBase PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/AuraX)

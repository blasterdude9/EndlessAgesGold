cmake_minimum_required(VERSION 3.21)
project(EndlessAges C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)   # old code; relax as needed

# --- Assimp: modern 3D model import (FBX, OBJ, glTF/glb, ...) ----------------
# EngineX imports modern model formats through Assimp (Open Asset Import
# Library, BSD-3). AuraX's CAssimpModelImporter converts the aiScene into the
# engine's CEXMeshObj geometry, so every editor tool that loads a model accepts
# the new formats. Assimp is fetched at configure time and pinned by hash; to
# build fully offline, drop a source checkout into thirdparty/assimp instead.
#
# Configured BEFORE the legacy MFC / /FIwinsock2.h flags below so Assimp's
# modern C++17 sources do not inherit force-included <winsock2.h>/<windows.h>
# min/max macros or engine include directories.
include(FetchContent)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "assimp: static library" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "assimp: build tests" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "assimp: build tools" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "assimp: build samples" FORCE)
set(ASSIMP_BUILD_DOCS OFF CACHE BOOL "assimp: build docs" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "assimp: install rules" FORCE)
set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "assimp: install pdb" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "assimp: debug postfix" FORCE)
set(ASSIMP_BUILD_USE_CCACHE OFF CACHE BOOL "assimp: use ccache" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "assimp: warnings as errors" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "assimp: import-only build (EngineX never exports)" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "assimp: build bundled zlib (needed for .glb)" FORCE)
# Compile only the importers EngineX advertises (see CAssimpModelImporter):
# keeps the AuraXDynamic.dll small and the build fast.
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "assimp: importer whitelist" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "assimp: exporter whitelist" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "assimp: OBJ" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "assimp: FBX" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "assimp: glTF/glb" FORCE)
set(ASSIMP_BUILD_COLLADA_IMPORTER ON CACHE BOOL "assimp: Collada" FORCE)
set(ASSIMP_BUILD_STL_IMPORTER ON CACHE BOOL "assimp: STL" FORCE)
set(ASSIMP_BUILD_PLY_IMPORTER ON CACHE BOOL "assimp: PLY" FORCE)
set(ASSIMP_BUILD_3DS_IMPORTER ON CACHE BOOL "assimp: 3DS" FORCE)
set(ASSIMP_BUILD_BLEND_IMPORTER ON CACHE BOOL "assimp: Blender" FORCE)
set(ASSIMP_BUILD_ASE_IMPORTER ON CACHE BOOL "assimp: ASE" FORCE)
set(ASSIMP_BUILD_LWO_IMPORTER ON CACHE BOOL "assimp: LightWave" FORCE)
set(ASSIMP_BUILD_DXF_IMPORTER ON CACHE BOOL "assimp: DXF" FORCE)
if(EXISTS "${CMAKE_SOURCE_DIR}/thirdparty/assimp/CMakeLists.txt")
  add_subdirectory(thirdparty/assimp)
else()
  # CMP0135 (CMake 3.24+): stamp extracted files with the extraction time so
  # a changed URL always triggers a rebuild. Guarded so CMake 3.21 still works.
  if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
  endif()
  FetchContent_Declare(assimp
    URL https://github.com/assimp/assimp/archive/refs/tags/v6.0.5.tar.gz
    URL_HASH SHA256=edf3749559c2b7d1f758ffb66fc5bec62186221e623b7f2e8969f17ee46ecb6f)
  FetchContent_MakeAvailable(assimp)
endif()
if(TARGET assimp)
  target_compile_definitions(assimp PRIVATE NOMINMAX)
endif()

# MFC, dynamically linked (UseOfMFC=2). MBCS (CharacterSet=2) is the default (no UNICODE).
set(CMAKE_MFC_FLAG 2)
add_compile_definitions(
  _CRT_SECURE_NO_WARNINGS _WINSOCK_DEPRECATED_NO_WARNINGS
  WINVER=0x0601 _WIN32_WINNT=0x0601 _WIN32_WINDOWS=0x0601 NTDDI_VERSION=0x06010000
  _HAS_STD_BYTE=0)
# Force winsock2.h to be included before anything pulls in winsock.h (MFC requires it).
add_compile_options(/FIwinsock2.h)
# Release symbols: /Z7 (debug info in the .obj, no pdb-server contention) +
# /DEBUG at link so every target ships a .pdb. CrashLog.txt (EngineX's crash
# reporter) uses them to print function names/lines. /OPT:REF,ICF keeps the
# binaries as small/fast as before.
add_compile_options($<$<CONFIG:Release>:/Z7>)
add_link_options($<$<CONFIG:Release>:/DEBUG> $<$<CONFIG:Release>:/OPT:REF> $<$<CONFIG:Release>:/OPT:ICF>)

# Legacy DirectX SDK (June 2010): we need ONLY the D3DX headers (d3dx9*, dxerr, d3dxof).
# Copy just those into the build tree so the DX SDK's outdated dxgi/d3d10/d2d/wincodec/dwrite
# headers can NEVER shadow the modern Windows SDK (that was the wincodec C2061). d3d9.h and
# everything else resolve to the Windows SDK.
if(DEFINED ENV{DXSDK_DIR})
  file(TO_CMAKE_PATH "$ENV{DXSDK_DIR}" DXSDK)
  set(DXPRUNE "${CMAKE_BINARY_DIR}/dxsdk_d3dx")
  file(MAKE_DIRECTORY "${DXPRUNE}")
  file(GLOB _dxh
    "${DXSDK}/Include/d3dx9*.h" "${DXSDK}/Include/d3dx9*.inl"
    "${DXSDK}/Include/dxerr*.h" "${DXSDK}/Include/d3dxof.h"
    "${DXSDK}/Include/rmxf*.h"  "${DXSDK}/Include/dxfile.h"
    "${DXSDK}/Include/dp*.h"    "${DXSDK}/Include/dvoice*.h")   # DirectPlay (networking, transitional)
  file(COPY ${_dxh} DESTINATION "${DXPRUNE}")
  include_directories("${DXPRUNE}")
  link_directories("${DXSDK}/Lib/x86")
endif()


# --- Common include paths shared by ALL modules -------------------------------
# The engine's public headers live centrally under HeaderFiles/, not beside each
# project's sources. Make every target see all of them (plus the vendored
# DirectPlay headers) so we don't chase missing-include errors module by module.
include_directories(
    ${CMAKE_SOURCE_DIR}/HeaderFiles/ClientBaseHeaders
    ${CMAKE_SOURCE_DIR}/HeaderFiles/CoreStatic
    ${CMAKE_SOURCE_DIR}/HeaderFiles/InterfaceHeaders
    ${CMAKE_SOURCE_DIR}/HeaderFiles/NetworkHeaders
    ${CMAKE_SOURCE_DIR}/HeaderFiles/RenderModules
    ${CMAKE_SOURCE_DIR}/HeaderFiles/Zpingheaders
    ${CMAKE_SOURCE_DIR}/thirdparty/directplay)

# --- ENet: modern reliable-UDP transport (vendored) --------------------------
# Replaces the dead DirectPlay transport. Pure C, built as a static lib.
# unix.c is excluded on Windows; win32.c provides the platform layer.
add_library(enet STATIC
    ${CMAKE_SOURCE_DIR}/thirdparty/enet/callbacks.c
    ${CMAKE_SOURCE_DIR}/thirdparty/enet/compress.c
    ${CMAKE_SOURCE_DIR}/thirdparty/enet/host.c
    ${CMAKE_SOURCE_DIR}/thirdparty/enet/list.c
    ${CMAKE_SOURCE_DIR}/thirdparty/enet/packet.c
    ${CMAKE_SOURCE_DIR}/thirdparty/enet/peer.c
    ${CMAKE_SOURCE_DIR}/thirdparty/enet/protocol.c
    ${CMAKE_SOURCE_DIR}/thirdparty/enet/win32.c)
target_include_directories(enet PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/enet/include)
target_compile_definitions(enet PRIVATE _WINSOCK_DEPRECATED_NO_WARNINGS _CRT_SECURE_NO_WARNINGS)
target_link_libraries(enet PUBLIC ws2_32 winmm)

# Client build chain (added one at a time as each is made to build).
add_subdirectory(EXCore_Platform/EXGameCore/EXGameCore)
add_subdirectory(AuraX)
add_subdirectory(CDxRenderModule)
add_subdirectory(NetworkExtension)
add_subdirectory(EXCore_Platform/GameEngine_ClientBase)
add_subdirectory(EXCore_Platform/CBaseInterfaceModule)
add_subdirectory(EXCore_Platform/MenuInterfaceModule)
add_subdirectory(EXCore_Platform/AuraUI)
add_subdirectory(EXCore_Platform/CContentManagementLibrary)
add_subdirectory(EXCore_Platform/AXSocketsLib)
add_subdirectory(ABWebComm)
add_subdirectory(AXCabinetLib)
add_subdirectory(EXCore_Platform/EngineMainApp)
add_subdirectory(EXCore_Platform/CGameApp/CGameApp)   # EndlessAges.exe (2008 game client - the client build)

# Server chain (game server and AI server).
add_subdirectory(EXCore_Platform/AXServerModule)
add_subdirectory(EXCore_Platform/AXServerApplication)   # AxserverApp.exe (game server, dialog host)
add_subdirectory(EXCore_Platform/CAIServerModule)
add_subdirectory(EXCore_Platform/AIServerApplication)   # AIServerApp.exe (AI server, dialog host)
add_subdirectory(EXCore_Platform/EAContentServer)       # ContentServer.exe (patch server for EALauncher)
add_subdirectory(EXCore_Platform/EALauncher)            # EALauncher.exe (launcher/patcher, goes in Core)

# Platform-independent unit tests for importer safety and OBJ conversion.
include(CTest)
if(BUILD_TESTING)
  add_executable(AssimpEmbeddedTextureUtilsTests
      Tests/AssimpEmbeddedTextureUtilsTests.cpp)
  target_include_directories(AssimpEmbeddedTextureUtilsTests PRIVATE
      ${CMAKE_SOURCE_DIR}/HeaderFiles/CoreStatic)
  add_test(NAME AssimpEmbeddedTextureUtilsTests
           COMMAND AssimpEmbeddedTextureUtilsTests)

  add_executable(AssimpSceneConverterTests
      Tests/AssimpSceneConverterTests.cpp
      AuraX/AssimpSceneConverter.cpp)
  target_include_directories(AssimpSceneConverterTests PRIVATE
      ${CMAKE_SOURCE_DIR}/HeaderFiles/CoreStatic)
  target_link_libraries(AssimpSceneConverterTests PRIVATE assimp::assimp)
  add_test(NAME AssimpSceneConverterTests
           COMMAND AssimpSceneConverterTests)
endif()

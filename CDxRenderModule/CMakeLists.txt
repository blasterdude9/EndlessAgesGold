# CDxRenderModule: the DirectX 9 renderer DLL (device creation + draw).
# Originally a DLL (ConfigurationType=2, _USRDLL) exporting CDXRENDERMODULE_EXPORTS.
# This is the module we will later rewrite for DX11.
file(GLOB CDXRENDER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(CDxRenderModule SHARED ${CDXRENDER_SOURCES})
set_target_properties(CDxRenderModule PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)  # original .def missing; export all so EngineX can link
set_target_properties(CDxRenderModule PROPERTIES OUTPUT_NAME "CDxRenderModule")

target_compile_definitions(CDxRenderModule PRIVATE
    WIN32 _WINDOWS _USRDLL CDXRENDERMODULE_EXPORTS _CRT_SECURE_NO_WARNINGS)

# This module pulls in engine headers (Common.h / CStructuresMisl.h) that use
# MFC types (CString, CArchive, CFile). Force-include MFC core so they resolve,
# exactly as AuraX does via its stdafx. MFC linkage is already set globally
# (CMAKE_MFC_FLAG). afxwin.h comes after the global /FIwinsock2.h, which is the
# order MFC needs.
target_compile_options(CDxRenderModule PRIVATE /FIafxwin.h)

target_include_directories(CDxRenderModule PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/HeaderFiles/RenderModules
    ${CMAKE_SOURCE_DIR}/HeaderFiles/CoreStatic
    ${CMAKE_SOURCE_DIR}/HeaderFiles/ClientBaseHeaders
    ${CMAKE_SOURCE_DIR}/HeaderFiles/NetworkHeaders
    ${CMAKE_SOURCE_DIR}/thirdparty/directplay)

target_link_libraries(CDxRenderModule PRIVATE d3d9 d3dx9 dxguid winmm)  # winmm: timeBeginPeriod(1) for the vsync frame pacer

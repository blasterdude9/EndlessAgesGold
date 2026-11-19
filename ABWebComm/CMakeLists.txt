# ABWebComm: HTTP/web communication (downloaders, URL reader, registry wrapper).
# Originally ConfigurationType=4 (static lib), MBCS. Links into EngineX.
set(ABWEBCOMM_SOURCES
    DirectoryRegistryWrapper.cpp
    HttpFileDownloader.cpp
    UrlReader.cpp
    WinHTTPFileDownloader.cpp
    stdafx.cpp
)
add_library(ABWebComm STATIC ${ABWEBCOMM_SOURCES})
target_compile_definitions(ABWebComm PRIVATE WIN32 _LIB)
target_include_directories(ABWebComm PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/AuraX)

# ContentServer.exe - Endless Ages content (patch) server.
# Plain Win32 (no MFC): serves the Patch\ folder over HTTP to EALauncher, and new accounts.
set(CMAKE_MFC_FLAG 0)
add_executable(ContentServer WIN32 ContentServer.cpp)
set_target_properties(ContentServer PROPERTIES OUTPUT_NAME "ContentServer")
target_link_libraries(ContentServer PRIVATE ws2_32 shell32 odbc32)

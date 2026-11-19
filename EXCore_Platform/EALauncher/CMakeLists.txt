# EALauncher.exe - Endless Ages launcher / patcher (goes in C:\Program Files\Core).
# Plain Win32 + GDI+ (no MFC). Updates the game from ContentServer, then starts EndlessAges.exe.
set(CMAKE_MFC_FLAG 0)
add_executable(EALauncher WIN32 EALauncher.cpp axcrypt.c EALauncher.rc)
set_target_properties(EALauncher PROPERTIES OUTPUT_NAME "EALauncher")
target_link_libraries(EALauncher PRIVATE wininet gdiplus shlwapi ole32 shell32 advapi32 uuid)

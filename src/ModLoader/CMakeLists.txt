target_sources(ModLoader
    PRIVATE
        Filesystem.h
        HookDispatcher.h
        Logger.h
        Mod.h
        ModLoaderImpl.h
        SharedLibrary.h
    PRIVATE
        Filesystem.cpp
        HookDispatcher.cpp
        Logger.cpp
        Mod.cpp
        ModLoaderImpl.cpp
        SharedLibrary.cpp
)

if (WIN32)
    add_subdirectory(Windows)
elseif (UNIX)
    add_subdirectory(Linux)
endif()
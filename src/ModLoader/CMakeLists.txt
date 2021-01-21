target_sources(ModLoader
    PRIVATE
        HookDispatcher.h
        Logger.h
        Mod.h
        ModLoaderImpl.h
    PRIVATE
        HookDispatcher.cpp
        Logger.cpp
        Mod.cpp
        ModLoaderImpl.cpp
)

if (WIN32)
    add_subdirectory(Windows)
elseif (UNIX)
    add_subdirectory(Linux)
endif()
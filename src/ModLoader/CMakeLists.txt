target_sources(ModLoader
    PRIVATE
        Debugger.h
        Filesystem.h
        HookDispatcher.h
        Logger.h
        Mod.h
        ModLoaderImpl.h
        SharedLibrary.h
    PRIVATE
        Debugger.cpp
        Filesystem.cpp
        HookDispatcher.cpp
        Logger.cpp
        Mod.cpp
        ModLoaderImpl.cpp
        SharedLibrary.cpp
)

add_subdirectory(Wrappers)
if (WIN32)
    add_subdirectory(Windows)
elseif (UNIX)
    add_subdirectory(Linux)
endif()
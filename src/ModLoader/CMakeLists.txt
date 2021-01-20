target_sources(ModLoader
    PRIVATE
        ModLoader.cpp
        ModLoaderEntry.cpp
        ModLoaderEntry.h
)

if (WIN32)
    add_subdirectory(Windows)
elseif (UNIX)
    add_subdirectory(Linux)
endif()
target_sources(ModLoader
    PRIVATE
        DebuggerImpl.h
        FilesystemImpl.h
        SharedLibraryImpl.h
    PRIVATE
        DebuggerImpl.cpp
        entry.cpp
        FilesystemImpl.cpp
        SharedLibraryImpl.cpp
)
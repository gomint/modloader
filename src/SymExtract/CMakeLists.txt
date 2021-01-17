target_sources(SymExtract
    PRIVATE
        main.cpp
)

if (WIN32)
    add_subdirectory(Windows)
elseif(UNIX)

endif()
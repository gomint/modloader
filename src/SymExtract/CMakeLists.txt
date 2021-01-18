target_sources(SymExtract
    PRIVATE
        extract.h
        main.cpp
)

add_subdirectory(Generator)

if (WIN32)
    add_subdirectory(Windows)
elseif(UNIX)

endif()
target_sources(SymExtract
    PRIVATE
        extract.h
        FileWriter.cpp
        FileWriter.h
        main.cpp
)

add_subdirectory(Schema)
#add_subdirectory(Generator)

if (WIN32)
    add_subdirectory(Windows)
elseif(UNIX)

endif()
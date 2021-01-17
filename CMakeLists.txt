cmake_minimum_required(VERSION 3.13)
project(ModLoader VERSION 0.1.0)

# Global Settings
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

set(MODLOADER_BEDROCK_PDB "${PROJECT_SOURCE_DIR}/symbols/bedrock_server.pdb" CACHE FILEPATH "Please specify the path to your bedrock_server.pdb file")

#
# Target Declarations
#
add_executable(SymExtract)
add_library(ModLoader SHARED)

if (WIN32)
    add_executable(PreLoader)
endif ()

target_include_directories(ModLoader PUBLIC include)

#
# External Libraries
#
add_subdirectory(extern/funchook)

target_link_libraries(ModLoader PRIVATE funchook-static)

#
# Source Tree
#
add_subdirectory(include/Minecraft)
add_subdirectory(include/ModLoader)
add_subdirectory(src/ModLoader)
add_subdirectory(src/SymExtract)

if (WIN32)
    add_subdirectory(src/Preloader)
endif ()

#
# Generation
#
if (WIN32)
    file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/symbols/SymExtract)
    add_custom_command(
        OUTPUT ${PROJECT_SOURCE_DIR}/symbols/SymExtract/Symbols.h ${PROJECT_SOURCE_DIR}/symbols/SymExtract/Symbols.cpp
        COMMAND SymExtract ${PROJECT_SOURCE_DIR}/symbols/symbol_table_win32.slt ${MODLOADER_BEDROCK_PDB} ${PROJECT_SOURCE_DIR}/symbols/SymExtract/Symbols
        DEPENDS SymExtract
    )

    target_sources(ModLoader PRIVATE ${PROJECT_SOURCE_DIR}/symbols/SymExtract/Symbols.h ${PROJECT_SOURCE_DIR}/symbols/SymExtract/Symbols.cpp)
    target_include_directories(ModLoader PRIVATE ${PROJECT_SOURCE_DIR}/symbols)
endif ()

#
# Installation
#
install(TARGETS ModLoader EXPORT ModLoaderTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
    )
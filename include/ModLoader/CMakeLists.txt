target_sources(ModLoader
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ModLoader.h>
        $<INSTALL_INTERFACE:include/ModLoader/ModLoader.h>
)
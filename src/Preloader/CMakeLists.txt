target_sources(PreLoader PRIVATE main.c)
# set_target_properties(Preloader PROPERTIES WIN32_EXECUTABLE true)

install(TARGETS PreLoader EXPORT ModLoaderTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

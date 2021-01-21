add_library(SimpleMod SHARED mod.cpp)
target_link_libraries(SimpleMod PRIVATE ModLoader)
target_include_directories(SimpleMod PRIVATE ${generated_dir})
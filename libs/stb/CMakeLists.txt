add_library(stb_image INTERFACE)

target_include_directories(stb_image INTERFACE ${CMAKE_CURRENT_LIST_DIR}/)

set_target_properties(stb_image PROPERTIES LINKER_LANGUAGE CXX)

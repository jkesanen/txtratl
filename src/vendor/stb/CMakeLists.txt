add_library(stb_image "")

add_definitions(-DSTB_IMAGE_IMPLEMENTATION)
add_definitions(-DSTB_IMAGE_WRITE_IMPLEMENTATION)

target_sources(stb_image
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/stb_image.h
        ${CMAKE_CURRENT_LIST_DIR}/stb_image_write.h)

set_target_properties(stb_image PROPERTIES LINKER_LANGUAGE CXX)

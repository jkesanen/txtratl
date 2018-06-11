add_library(rectpack2d "")

target_sources(rectpack2d
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/pack.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/pack.h)

target_include_directories(rectpack2d
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src)

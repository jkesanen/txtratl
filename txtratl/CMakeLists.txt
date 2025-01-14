cmake_minimum_required(VERSION 3.12)

project(txtratl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(USE_STB_IMAGE "Use stb_image to load and save images." ON)

list(APPEND SOURCES
    src/atlas.cpp
    src/image.cpp
    src/imageblit.cpp
    include/txtratl/atlas.hpp
    include/txtratl/image.hpp
    include/txtratl/imageblit.hpp
    include/txtratl/imagerect.hpp
)

if(USE_STB_IMAGE)
    list(APPEND SOURCES
        src/image_stb.cpp)
endif()

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC include/)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /Zc:__cplusplus /W4 /Wall /wd4710 /wd4711)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    find_package(Threads REQUIRED)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -mssse3 -pthread)
    target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE rectpack2d stb_image)

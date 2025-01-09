cmake_minimum_required(VERSION 3.12)

project(txtratl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(USE_STB_IMAGE "Use stb_image to load and save images." ON)

include(vendor/rectpack2d/CMakeLists.txt)
include(vendor/stb/CMakeLists.txt)

list(APPEND SOURCES
    atlas.cpp
    atlas.hpp
    image.cpp
    image.hpp
    imageblit.cpp
    imageblit.hpp
    imagerect.hpp
    main.cpp)

if(USE_STB_IMAGE)
    list(APPEND SOURCES
        image_stb.cpp)
endif()

add_executable(txtratl
    ${SOURCES})

if(MSVC)
    target_compile_options(txtratl PRIVATE /Zc:__cplusplus /W4 /Wall /wd4710 /wd4711)
elseif(CMAKE_COMPILER_IS_GNUCXX)
    find_package(Threads REQUIRED)
    target_compile_options(txtratl PRIVATE -Wall -Wextra -mssse3 -pthread)
    target_link_libraries(txtratl PRIVATE Threads::Threads)
else()
    target_compile_options(txtratl PRIVATE -Wall -Wextra -Werror)
endif()

target_link_libraries(txtratl PRIVATE rectpack2d)

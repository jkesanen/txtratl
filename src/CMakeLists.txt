CMAKE_MINIMUM_REQUIRED(VERSION 3.12)

project(txtratl)

include(vendor/rectpack2d/CMakeLists.txt)
include(vendor/stb/CMakeLists.txt)

list(APPEND SOURCES
    main.cpp
    atlas.cpp
    atlas.hpp
    image.hpp
    imageblit.cpp
    imageblit.hpp
    imagerect.hpp
    image_stb.cpp)

add_executable(txtratl
    ${SOURCES})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

IF (MSVC)
    #target_compile_options(txtratl PRIVATE /W4 /Wall /WX)
    target_compile_options(txtratl PRIVATE /W4 /Wall)
    add_definitions(-D_ENABLE_ATOMIC_ALIGNMENT_FIX)
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
    find_package(Threads REQUIRED)
    target_compile_options(txtratl PRIVATE -Wall -Wextra -mssse3 -pthread)
    target_link_libraries(txtratl PRIVATE Threads::Threads)
ELSE()
    target_compile_options(txtratl PRIVATE -Wall -Wextra -Werror)
ENDIF()

target_link_libraries(txtratl PRIVATE rectpack2d)

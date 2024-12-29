CMAKE_MINIMUM_REQUIRED(VERSION 3.12)

project(txtratl)

list(APPEND SOURCES
    main.cpp
    atlas.cpp
    atlas.hpp
    image.hpp
    imageblit.cpp
    imageblit.hpp
    imagerect.hpp
    image_zupply.cpp)

add_executable(txtratl
    ${SOURCES})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(vendor/zupply/CMakeLists.txt)
include(vendor/rectpack2d/CMakeLists.txt)

IF (MSVC)
    #target_compile_options(txtratl PRIVATE /W4 /Wall /WX)
    target_compile_options(txtratl PRIVATE /W4 /WX)
    add_definitions(-D_ENABLE_ATOMIC_ALIGNMENT_FIX)
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
    #SET(CMAKE_EXE_LINKER_FLAGS "-lpthread")
    target_compile_options(txtratl PRIVATE -Wall -Wextra -Werror -mssse3 -pthread)
ELSE()
    target_compile_options(txtratl PRIVATE -Wall -Wextra -Werror)
ENDIF()

target_link_libraries(txtratl PRIVATE zupply rectpack2d)

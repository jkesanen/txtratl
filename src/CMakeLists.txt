CMAKE_MINIMUM_REQUIRED(VERSION 3.6)

project(txtratl)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

IF (WIN32)
    add_definitions(-D_ENABLE_ATOMIC_ALIGNMENT_FIX)
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "-mssse3 -std=c++11 -pthread")
    SET(CMAKE_EXE_LINKER_FLAGS "-lpthread")
ELSE()
    SET(CMAKE_CXX_FLAGS "-std=c++11")
ENDIF()

include(vendor/zupply/CMakeLists.txt)
include(vendor/rectpack2d/CMakeLists.txt)

list(APPEND SOURCES
    main.cpp
    atlas.cpp
    atlas.hpp
    imageblit.cpp
    imageblit.hpp
    imagerect.hpp)

add_executable(txtratl
    ${SOURCES})

target_link_libraries(txtratl zupply rectpack2d)

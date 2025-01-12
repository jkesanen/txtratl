cmake_minimum_required(VERSION 3.12)

project(txtratlapp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND SOURCES
    main.cpp)

add_executable(${PROJECT_NAME}
    ${SOURCES})

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /Zc:__cplusplus /W4 /Wall /wd4710 /wd4711)
elseif(CMAKE_COMPILER_IS_GNUCXX)
    find_package(Threads REQUIRED)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -mssse3 -pthread)
    target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE txtratl rectpack2d)

set(OUTPUT_NAME txtratl)


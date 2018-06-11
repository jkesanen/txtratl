add_library(zupply "")

target_sources(zupply
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/zupply.cpp
        ${CMAKE_CURRENT_LIST_DIR}/zupply.hpp)

target_include_directories(zupply
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.)

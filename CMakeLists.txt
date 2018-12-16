cmake_minimum_required (VERSION 3.9)

project (AI)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -Wpedantic -Werror")

include_directories (
    ${PROJECT_SOURCE_DIR}/include
)

add_subdirectory(examples)



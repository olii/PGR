cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0004 OLD)

project(BSSRDF CXX)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)

add_subdirectory(src)

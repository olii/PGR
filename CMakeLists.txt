cmake_minimum_required(VERSION 3.6)
project (bssrdf)
set(CMAKE_GENERATOR "Ninja")
find_package(SDL2 REQUIRED)
message("SDL2 directory: " ${SDL2_INCLUDE_DIRS})

# ultimatne flagy
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -g3 -O3")

include_directories(${SDL2_INCLUDE_DIRS})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../install)


add_subdirectory(src)

#add_executable(bssrdf src/main.cpp)
#target_link_libraries (bssrdf main)

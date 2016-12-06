cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0004 OLD)

project(BSSRDF CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)
find_package(jsoncpp REQUIRED)

add_subdirectory(src)

add_custom_target(pack
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    COMMAND rm -f xmilko01.zip
    COMMAND mkdir -p xmilko01
    COMMAND cp -r cmake/ scenes/ src/ CMakeLists.txt doc.pdf xmilko01/
    COMMAND zip -r xmilko01.zip xmilko01/
    COMMAND rm -rf xmilko01
)

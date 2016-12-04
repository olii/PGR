file(GLOB_RECURSE SOURCES *.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(bssrdf ${SOURCES})
target_link_libraries(bssrdf pthread ${SDL2_LIBRARIES})

set_property(TARGET bssrdf PROPERTY CXX_STANDARD 14)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_property(TARGET bssrdf PROPERTY COMPILE_FLAGS -O0)
else()
    set_property(TARGET bssrdf PROPERTY COMPILE_FLAGS -O3)
endif()

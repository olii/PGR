file(GLOB_RECURSE SOURCES *.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(bssrdf ${SOURCES})
target_link_libraries(bssrdf ${SDL2_LIBRARIES})

set_property(TARGET bssrdf PROPERTY CXX_STANDARD 14)

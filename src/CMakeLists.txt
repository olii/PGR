message("src directory")
include_directories(.)


add_executable(bssrdf
	main.cpp
	vector3.cpp
    ray.cpp
    plane.cpp
    objectgeometry.cpp
	)


target_link_libraries(bssrdf ${SDL2_LIBRARIES})

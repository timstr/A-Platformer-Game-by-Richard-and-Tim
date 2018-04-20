cmake_minimum_required(VERSION 3.9.0)

set(physics-headers
	vec2.h
	rigidbody.h
	physengine.h
	collision.h
)

set(physics-srcs
	vec2.cpp
	rigidbody.cpp
	physengine.cpp
	collision.cpp
)

add_library(physics STATIC ${physics-headers} ${physics-srcs})

set_target_properties(physics PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(physics
	PUBLIC "."
	PUBLIC "${CMAKE_SOURCE_DIR}/libs"	# for Eigen, etc
)

target_link_libraries(physics
	PUBLIC tims-gui
	PUBLIC graphics
)
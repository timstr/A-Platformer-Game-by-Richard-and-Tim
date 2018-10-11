cmake_minimum_required(VERSION 3.9.0)

set(physics-headers
	BoundingBox.hpp
	CircleBody.hpp
	Collision.hpp
	ConvexBody.hpp
	PhysicsEngine.hpp
	RasterBody.hpp
	RectangleBody.hpp
	RigidBody.hpp
	Vec2.hpp
)

set(physics-srcs
	BoundingBox.cpp
	CircleBody.cpp
	Collision.cpp
	ConvexBody.cpp
	PhysicsEngine.cpp
	RasterBody.cpp
	RectangleBody.cpp
	RigidBody.cpp
	Vec2.cpp
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
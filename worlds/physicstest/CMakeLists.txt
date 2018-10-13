cmake_minimum_required(VERSION 3.10.0)

set(physicstest-headers
	physicstest.h
)

set(physicstest-srcs
	
)

add_library(physicstest STATIC ${physicstest-headers} ${physicstest-srcs})

# use C++17 standard
set_property(TARGET physicstest PROPERTY CXX_STANDARD 17)

set_target_properties(physicstest PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(physicstest PUBLIC worlds-base)
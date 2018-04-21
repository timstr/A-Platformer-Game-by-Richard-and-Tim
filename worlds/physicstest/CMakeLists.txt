cmake_minimum_required(VERSION 3.9.0)

set(physicstest-headers
	physicstest.h
)

set(physicstest-srcs
	
)

add_library(physicstest STATIC ${physicstest-headers} ${physicstest-srcs})

set_target_properties(physicstest PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(physicstest PUBLIC worlds-base)
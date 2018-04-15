cmake_minimum_required(VERSION 3.9.0)

set(inventory-headers
	item.h
)

set(inventory-srcs
	item.cpp
)

add_library(inventory STATIC ${inventory-headers} ${inventory-srcs})

set_target_properties(inventory PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(inventory PUBLIC ".")

target_link_libraries(inventory
	PUBLIC mechanics
)
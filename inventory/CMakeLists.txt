cmake_minimum_required(VERSION 3.10.0)

set(inventory-headers
	item.h
)

set(inventory-srcs
	item.cpp
)

add_library(inventory STATIC ${inventory-headers} ${inventory-srcs})

# use C++17 standard
set_property(TARGET inventory PROPERTY CXX_STANDARD 17)

set_target_properties(inventory PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(inventory PUBLIC ".")

target_link_libraries(inventory
	PUBLIC mechanics
)
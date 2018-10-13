cmake_minimum_required(VERSION 3.10.0)

set(mechanics-headers
	character.h
	creature.h
	destructible.h
	entity.h
	event.h
	intermob.h
	map.h
	obstacle.h
	obstruction.h
	player.h
	space.h
)

set(mechanics-srcs
	character.cpp
	creature.cpp
	entity.cpp
	event.cpp
	map.cpp
	obstacle.cpp
	obstruction.cpp
	player.cpp
	space.cpp
)

add_library(mechanics STATIC ${mechanics-headers} ${mechanics-srcs})

# use C++17 standard
set_property(TARGET mechanics PROPERTY CXX_STANDARD 17)

set_target_properties(mechanics PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(mechanics PUBLIC ".")

target_link_libraries(mechanics
	PUBLIC tims-gui
	PUBLIC graphics
	PUBLIC physics
)
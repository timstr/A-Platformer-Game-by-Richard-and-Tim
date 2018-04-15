cmake_minimum_required(VERSION 3.9.0)

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
	vec2.h
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
	vec2.cpp
)

add_library(mechanics STATIC ${mechanics-headers} ${mechanics-srcs})

set_target_properties(mechanics PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(mechanics PUBLIC ".")

target_link_libraries(mechanics
	PUBLIC tims-gui
	PUBLIC graphics
)
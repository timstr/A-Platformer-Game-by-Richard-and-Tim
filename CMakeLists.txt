cmake_minimum_required(VERSION 3.9.0)

# use only MPL2-licensed features of Eigen
add_definitions(-DEIGEN_MPL2_ONLY)

set(GAME "A-Platformer-Game-by-Richard-and-Tim")

project(${GAME})

add_executable(${GAME} Platformer.cpp)

set(LIBRARIES
	graphics
	inventory
	physics
	mechanics
	tims-gui
	ui
	worlds
)

foreach(LIB ${LIBRARIES})
	
	add_subdirectory(${LIB})
	target_link_libraries(${GAME} ${LIB})
	target_include_directories(${GAME} PUBLIC ${GAME})
	
endforeach(LIB)

# copy openal32.dll to executable directory on windows
if(WIN32)
	add_custom_command(TARGET ${GAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${SFML_DIR}/../../../bin/openal32.dll"
			"$<TARGET_FILE_DIR:${GAME}>"
	)
endif()
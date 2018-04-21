cmake_minimum_required(VERSION 3.9.0)

add_library(worlds INTERFACE)
add_library(worlds-base INTERFACE)

# list of all header-only sub-libraries
set(worlds-libs
	physicstest
)

# sub-libraries that aren't header-only and thus need to be linked to
set(worlds-libs-to-link
	testworld
)

target_link_libraries(worlds-base
	INTERFACE graphics
	INTERFACE inventory
	INTERFACE mechanics
	INTERFACE tims-gui
	INTERFACE ui
)

foreach(lib ${worlds-libs})
	add_subdirectory(${lib})
	target_include_directories(worlds INTERFACE ${lib})
endforeach(lib)

foreach(lib ${worlds-libs-to-link})
	add_subdirectory(${lib})
	target_include_directories(worlds INTERFACE ${lib})
	target_link_libraries(worlds INTERFACE ${lib})
endforeach(lib)


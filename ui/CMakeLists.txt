cmake_minimum_required(VERSION 3.10.0)

set(FONTS_PATH "${CMAKE_SOURCE_DIR}/fonts")
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/fontspath.h.in"
	"${CMAKE_CURRENT_SOURCE_DIR}/fontspath.h"
)

set(ui-headers
	TestUI.h
	fontspath.h
	PhysicsTestUI.h
)

set(ui-srcs
	TestUI.cpp
)

add_library(ui STATIC ${ui-headers} ${ui-srcs})

# use C++17 standard
set_property(TARGET ui PROPERTY CXX_STANDARD 17)

set_target_properties(ui PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(ui
	PUBLIC "."
	PUBLIC worlds
)

target_link_libraries(ui
	PUBLIC tims-gui
	PUBLIC worlds
)
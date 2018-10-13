cmake_minimum_required(VERSION 3.10.0)


# configure images path
set(IMAGES_PATH "${CMAKE_SOURCE_DIR}/images")
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/imagespath.h.in
	${CMAKE_CURRENT_SOURCE_DIR}/imagespath.h
)


set(graphics-headers
	spritesheet.h
	ImageStore.h
	SpriteSheetStore.h
	imagespath.h
)

set(graphics-srcs
	spritesheet.cpp
	ImageStore.cpp
	SpriteSheetStore.cpp
)

add_library(graphics STATIC ${graphics-headers} ${graphics-srcs})

# use C++17 standard
set_property(TARGET graphics PROPERTY CXX_STANDARD 17)

set_target_properties(graphics PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(graphics PUBLIC ".")

target_link_libraries(graphics
	PUBLIC tims-gui
	PUBLIC mechanics
)
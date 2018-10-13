cmake_minimum_required(VERSION 3.10.0)

set(testworld-headers
	testworm.h
	TestSpace.h
	TestObstacles.h
	TestMap.h
	TreeZone.h
	TestEntities.h
	testcharacter.h
	testbug.h
	testbird.h
	sprudo.h
	bulbous.h
	HermannHorst.h
)

set(testworld-srcs
	testworm.cpp
	testcharacter.cpp
	testbug.cpp
	testbird.cpp
	HermannHorst.cpp
)

add_library(testworld STATIC ${testworld-headers} ${testworld-srcs})

# use C++17 standard
set_property(TARGET testworld PROPERTY CXX_STANDARD 17)

set_target_properties(testworld PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(testworld PUBLIC worlds-base)
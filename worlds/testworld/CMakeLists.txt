cmake_minimum_required(VERSION 3.9.0)

set(testworld-headers
	testworm.h
	TestSpace.h
	TestObstacles.h
	TestMap.h
	TestEntities.h
	testcharacter.h
	testbug.h
	testbird.h
	sprudo.h
	bulbous.h
)

set(testworld-srcs
	testworm.cpp
	testcharacter.cpp
	testbug.cpp
	testbird.cpp
)

add_library(testworld STATIC ${testworld-headers} ${testworld-srcs})

set_target_properties(testworld PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(testworld PUBLIC worlds-base)
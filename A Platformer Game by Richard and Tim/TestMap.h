#pragma once

#include "map.h"
#include "ImageStore.h"

struct TestMap : Map {
	TestMap(){
		sprite.setTexture(ImageStore::getTexture("testmapboundary3"));
		setBoundary(ImageStore::getImage("testmapboundary3"));
		friction = 1.0;
	}
};
#pragma once

#include "map.h"
#include "ImageStore.h"

struct TestMap : Map {
	TestMap(){
		sprite.setTexture(ImageStore::getTexture("testmapboundary3.png"));
		setBoundary(ImageStore::getImage("testmapboundary3.png"));
		friction = 1.0;
	}
};
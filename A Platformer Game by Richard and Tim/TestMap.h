#pragma once

#include "map.h"
#include "ImageStore.h"

struct TestMap : Map {
	TestMap(){
		setSprite(sf::Sprite(ImageStore::getTexture("testmapboundary3")));
		setBoundary(ImageStore::getImage("testmapboundary3"));
		setFriction(1);
	}
};
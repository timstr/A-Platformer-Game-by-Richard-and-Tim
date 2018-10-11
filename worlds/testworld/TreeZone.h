#pragma once

#include "map.h"
#include "ImageStore.h"

struct TreeZone : Map {
	TreeZone() {
		setSprite(sf::Sprite(ImageStore::getTexture("treezonebase")));
		setBoundary(ImageStore::getImage("treezoneboundary"));
		setFriction(0.8f);
	}
};

struct TreeZoneForeground : Map {
	TreeZoneForeground() {
		setSprite(sf::Sprite(ImageStore::getTexture("treezoneforeground")));
	}
};
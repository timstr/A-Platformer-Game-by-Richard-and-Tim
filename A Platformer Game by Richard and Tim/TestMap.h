#pragma once

#include "map.h"

struct TestMap : Map {
	TestMap(){
		image.loadFromFile("images/testmapboundary3.png");
		texture.loadFromImage(image);
		sprite.setTexture(texture);

		setImage(sprite);
		setBoundary(image);
		friction = 1.0;
	}

	private:
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
};
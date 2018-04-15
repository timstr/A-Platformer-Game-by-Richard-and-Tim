#pragma once

#include <SFML\Graphics.hpp>
#include "entity.h"

// TODO
struct Intermob : sf::Drawable, sf::Transformable {
	virtual ~Intermob(){

	}

	void interactWith(const std::vector<Entity*> entities){
		// test for collisions
	}
};
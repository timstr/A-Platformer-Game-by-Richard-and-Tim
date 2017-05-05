#pragma once

#include <SFML\Graphics.hpp>

typedef sf::Vector2f vec2;

// Renderable is the base class to all things having a visual presence
struct Renderable {
	virtual ~Renderable(){

	}

	virtual void render(sf::RenderWindow rw, vec2 offset) = 0;
};
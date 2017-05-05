#pragma once

#include "obstruction.h"
#include "entity.h"
#include <vector>

// a Space is all the above-mentioned components brought together
// where interactions and spatial relations are collectively managed
// A Space consists of a map, a number of obstacles, and a number of
// entities
struct Space {

	virtual ~Space();

	// display the space to the screen
	void render(sf::RenderWindow& rw);

	// simulate interactions
	void tick();

	// adopt (and assume memory management of) an obstruction
	void addObstruction(Obstruction* obstruction);

	// adopt (and assume memory management of) an entity
	void addEntity(Entity* entity);

	protected:

	std::vector<Obstruction*> obstructions;
	std::vector<Entity*> entities;
};
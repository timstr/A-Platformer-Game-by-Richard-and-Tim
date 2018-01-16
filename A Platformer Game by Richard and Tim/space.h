#pragma once

#include "SFML\Graphics.hpp"
#include "obstruction.h"
#include "entity.h"
#include "creature.h"
#include <vector>

// a Space is where obstructions and entities come together and
// where interactions and spatial relations are collectively managed
// A Space consists of a map, a number of obstructions, and a number of
// entities
struct Space : sf::Drawable, sf::Transformable {

	virtual ~Space();

	// simulate interactions
	void tick();

	// adopt (and assume memory management of) an obstruction
	void addObstruction(Obstruction* obstruction);

	// adopt (and assume memory management of) an entity
	void addEntity(Entity* entity);

	// adopt and assume memory management of a creature
	void addCreature(Creature* creature);

	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;

	protected:

	std::vector<Obstruction*> obstructions;
	std::vector<Entity*> entities;
	std::vector<Creature*> creatures;
};
#pragma once

#include "SFML/Graphics.hpp"
#include "obstruction.h"
#include "entity.h"
#include "creature.h"
#include <vector>
#include <memory>

#include "PhysicsEngine.hpp"

// a Space is where obstructions and entities come together and
// where interactions and spatial relations are collectively managed
// A Space consists of a map, a number of obstructions, and a number of
// entities
struct Space : sf::Drawable, sf::Transformable {

	virtual ~Space();

	// simulate interactions
	void tick();

	// custom per-frame function
	virtual void update();

	// add a new obstruction
	template<typename ObstructionType, typename ... ArgTypes>
	std::weak_ptr<ObstructionType> addObstruction(ArgTypes ... args) {
		static_assert(std::is_base_of<Obstruction, ObstructionType>::value, "ObstructionType must derive from Obstruction");
		std::shared_ptr<ObstructionType> obstruction = std::make_shared<ObstructionType>(args...);
		obstructions.push_back(obstruction);
		return obstruction;
	}

	// add a new entity
	template<typename EntityType, typename ... ArgTypes>
	std::weak_ptr<EntityType> addEntity(ArgTypes ... args) {
		static_assert(std::is_base_of<Entity, EntityType>::value, "EntityType must derive from Entity");
		std::shared_ptr<EntityType> entity = std::make_shared<EntityType>(args...);
		entities.push_back(entity);
		return entity;
	}

	// add a new creature
	template<typename CreatureType, typename ... ArgTypes>
	std::weak_ptr<CreatureType> addCreature(ArgTypes ... args) {
		static_assert(std::is_base_of<Creature, CreatureType>::value, "CreatureType must derive from Creature");
		std::weak_ptr<CreatureType> creature = addEntity<CreatureType>(args...);
		creatures.push_back(creature);
		return creature;
	}

	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;

protected:

	std::vector<std::shared_ptr<Obstruction>> obstructions;
	std::vector<std::shared_ptr<Entity>> entities;
	std::vector<std::weak_ptr<Creature>> creatures;
};
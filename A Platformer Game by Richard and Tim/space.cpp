#pragma once
#include "space.h"

Space::~Space(){
	for (Obstruction* obstruction : obstructions){
		delete obstruction;
	}
	for (Entity* entity : entities){
		delete entity;
	}
}

void Space::tick(){
	for (Obstruction* obstruction : obstructions){
		obstruction->update();
	}
	for (Entity* entity : entities){
		entity->tick();
	}
	for (Entity* const entity : entities){
		entity->moveAndCollide(obstructions);
	}
}

// adopt (and assume memory management of) an obstruction
void Space::addObstruction(Obstruction* obstruction){
	obstructions.push_back(obstruction);
};

// adopt (and assume memory management of) an entity
void Space::addEntity(Entity* entity){
	entities.push_back(entity);
}

void Space::draw(sf::RenderTarget& rt, sf::RenderStates states) const {
	states.transform *= getTransform();
	for (Obstruction* obstruction : obstructions){
		obstruction->draw(rt, states);
	}
	for (Entity* entity : entities){
		entity->draw(rt, states);
	}
}

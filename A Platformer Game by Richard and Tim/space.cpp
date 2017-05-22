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
	for (Entity* const entity : entities){
		entity->move(obstructions);
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

void Space::render(sf::RenderWindow& rw, vec2 offset){
	for (Obstruction* obstruction : obstructions){
		obstruction->render(rw, offset);
	}
	for (Entity* entity : entities){
		entity->render(rw, offset);
	}
}

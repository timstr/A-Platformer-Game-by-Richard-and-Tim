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

void Space::render(sf::RenderWindow& rw){

}

void Space::tick(){

}

// adopt (and assume memory management of) an obstruction
void Space::addObstruction(Obstruction* obstruction){

};

// adopt (and assume memory management of) an entity
void Space::addEntity(Entity* entity){

}

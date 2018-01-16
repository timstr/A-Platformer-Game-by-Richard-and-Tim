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
	for (Entity* const entity : entities){
		entity->moveAndCollide(obstructions);
	}
	for (Entity* entity : entities){
		entity->tick();
	}
	for (unsigned int i = 0; i < creatures.size(); i++){
		for (unsigned int j = i + 1; j < creatures.size(); j++){
			Creature* const creature1 = creatures[i];
			Creature* const creature2 = creatures[j];
			vec2 pos1 = creature1->getPosition();
			vec2 pos2 = creature2->getPosition();
			float dist_sqr = powf(pos1.x - pos2.x, 2.0f) + powf(pos1.y - pos2.y, 2.0f);
			float rad1 = creature1->getAwarenessRadius();
			float rad2 = creature2->getAwarenessRadius();

			// if creature1 might notice creature2
			if (dist_sqr < powf(rad1, 2.0f)){
				float dist = sqrt(dist_sqr);
				float prob = dist / rad1;
				if ((rand() % 1000) * 0.001f > prob){
					creature1->notice(creature2);
				}
			}

			// if creature2 might notice creature1
			if (dist_sqr < powf(rad2, 2.0f)){
				float dist = sqrt(dist_sqr);
				float prob = dist / rad2;
				if ((rand() % 1000) * 0.001f > prob){
					creature2->notice(creature1);
				}
			}
		}
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

void Space::addCreature(Creature* creature){
	entities.push_back(creature);
	creatures.push_back(creature);
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

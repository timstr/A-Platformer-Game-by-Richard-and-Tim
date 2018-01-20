#pragma once
#include "space.h"

Space::~Space(){

}

void Space::tick(){
	// remove any destroyed things
	for (auto it = obstructions.begin(); it != obstructions.end();){
		if ((*it)->isDestroyed()){
			it = obstructions.erase(it);
		} else {
			it++;
		}
	}
	for (auto it = entities.begin(); it != entities.end();){
		if ((*it)->isDestroyed()){
			it = entities.erase(it);
		} else {
			it++;
		}
	}
	for (auto it = creatures.begin(); it != creatures.end();){
		if (it->expired()){
			it = creatures.erase(it);
		} else {
			it++;
		}
	}


	for (auto& obstruction : obstructions){
		obstruction->update();
	}
	for (auto& entity : entities){
		entity->moveAndCollide(obstructions);
	}
	for (auto& entity : entities){
		entity->tick();
	}
	for (unsigned int i = 0; i < creatures.size(); i++){
		if (auto creature1 = creatures[i].lock()){
			for (unsigned int j = i + 1; j < creatures.size(); j++){
				if (auto creature2 = creatures[j].lock()){
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
	}

	update();
}

void Space::update(){

}

void Space::draw(sf::RenderTarget& rt, sf::RenderStates states) const {
	states.transform *= getTransform();
	for (auto& obstruction : obstructions){
		obstruction->draw(rt, states);
	}
	for (auto& entity : entities){
		entity->draw(rt, states);
	}
}
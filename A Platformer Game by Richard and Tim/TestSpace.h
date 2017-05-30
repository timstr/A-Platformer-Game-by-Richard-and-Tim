#pragma once
#include "space.h"

#include "TestObstacles.h"
#include "TestCreature.h"
#include "TestEntities.h"
#include "TestMap.h"


struct TestSpace : Space {
	TestSpace(){
		const int num_entities = 10;
		const int test_entites = 0;

		for (int i = 0; i < num_entities; i++){
			Entity* entity = new SimpleEntity();
			entity->velocity = vec2((((rand() % 100) - 50) / 10.0), (((rand() % 100) - 50) / 10.0));
			addEntity(entity);
			entities.push_back(entity);
		}

		for (int i = 0; i < test_entites; i++){
			Entity* entity = new ComplexEntity();
			entity->velocity = vec2((((rand() % 100) - 50) / 10.0), (((rand() % 100) - 50) / 10.0));
			addEntity(entity);
			entities.push_back(entity);
		}

		guy = new GuyEntity();
		addEntity(guy);
		entities.push_back(guy);

		addObstruction(map = new TestMap());

		addObstruction(tree = new TreeObstacle());
		tree->position = {250, 500};

		addObstruction(boost = new BoostObstacle());
		boost->position = {600, 500};

		addObstruction(new MovingObstacle({400, 350}, {400, 475}, 100));
		addObstruction(new MovingObstacle({600, 300}, {700, 300}, 100));

		for (int i = 0; i < 100; i++){
			TestCreature* creature = new TestCreature();
			creature->position = vec2(100, 100);
			addEntity(creature);
			entities.push_back(creature);
		}

		//addObstruction(new RampObstacle());
		//addObstruction(new MovingObstacle({50, 150}, {700, 300}, 250));
	}

	void render(sf::RenderWindow& rw, vec2 offset) override {
		Space::render(rw, offset);
	}

	void createEntity(vec2 position){
		SimpleEntity* entity = new SimpleEntity();
		entity->position = position;
		addEntity(entity);
		entities.push_back(entity);
	}

	std::vector<Entity*> entities;
	GuyEntity* guy;
	TestMap* map;
	TreeObstacle* tree;
	BoostObstacle* boost;
};
#pragma once
#include "space.h"

#include "TestObstacles.h"
#include "TestCreature.h"
#include "TestEntities.h"
#include "TestMap.h"

#include "sprudo.h"

struct TestSpace : Space {
	TestSpace(){
		const int num_entities = 10;
		const int test_entites = 10;
		const int num_sprudos = 10;

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

		for (int i = 0; i < num_sprudos; i++){
			Sprudo* sprudo = new Sprudo();
			addEntity(sprudo);
			entities.push_back(sprudo);
		}

		addObstruction(map = new TestMap());

		//addObstruction(tree = new TreeObstacle());
		//tree->position = {250, 500};

		addObstruction(boost = new BoostObstacle());
		boost->position = {500, 500};

		//addObstruction(new MovingObstacle({400, 350}, {400, 475}, 100));
		addObstruction(new MovingObstacle({600, 300}, {700, 300}, 100));

		for (int i = 0; i < 20; i++){
			TestCreature* creature = new TestCreature();
			creature->position = vec2(600, 100);
			creature->velocity = vec2((((rand() % 100) - 50) / 10.0), (((rand() % 100) - 50) / 10.0));
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
		/*SimpleEntity* entity = new SimpleEntity();
		entity->position = position;
		addEntity(entity);
		entities.push_back(entity);*/
		Sprudo* sprudo = new Sprudo();
		sprudo->position = position;
		addEntity(sprudo);
		entities.push_back(sprudo);
	}

	std::vector<Entity*> entities;
	GuyEntity* guy;
	TestMap* map;
	TreeObstacle* tree;
	BoostObstacle* boost;
};
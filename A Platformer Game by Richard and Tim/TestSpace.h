#pragma once
#include "space.h"

#include "TestObstacles.h"
#include "TestEntities.h"
#include "TestMap.h"

#include "sprudo.h"
#include "bulbous.h"
#include "testcharacter.h"

// TODO: rename to map
struct TestSpace : Space {
	TestSpace(){
		const int num_entities = 10;
		const int test_entites = 3;
		const int num_sprudos = 5;
		const int num_bulbous = 2;
		const int num_testchars = 2;

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

		for (int i = 0; i < num_bulbous; i++){
			Bulbous* bulbous = new Bulbous();
			addEntity(bulbous);
			entities.push_back(bulbous);
		}

		for (int i = 0; i < num_testchars; i++){
			TestCharacter *tc = new TestCharacter();
			tc->setPosition(vec2(300, 100));
			addEntity(tc);
			entities.push_back(tc);
		}

		addObstruction(map = new TestMap());

		//addObstruction(tree = new TreeObstacle());
		//tree->position = {270, 500};

		//addObstruction(boost = new BoostObstacle());
		//boost->setPosition(vec2(500, 500));

		//addObstruction(new MovingObstacle({400, 350}, {400, 475}, 100));
		//addObstruction(new MovingObstacle({600, 300}, {700, 300}, 100));

		//addObstruction(new RampObstacle());
		//addObstruction(new MovingObstacle({50, 150}, {700, 300}, 250));
	}

	void createEntity(vec2 position){
		Entity* ent = new ComplexEntity();
		ent->setPosition(position);
		addEntity(ent);
		entities.push_back(ent);
	}

	std::vector<Entity*> entities;
	GuyEntity* guy;
	TestMap* map;
	TreeObstacle* tree;
	BoostObstacle* boost;
};
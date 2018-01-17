#pragma once
#include "space.h"

#include "TestObstacles.h"
#include "TestEntities.h"
#include "TestMap.h"

#include "sprudo.h"
#include "bulbous.h"
#include "testcharacter.h"

#include "testbird.h"
#include "testworm.h"

// TODO: rename to map
struct TestSpace : Space {
	TestSpace(){
		vec2 place_to_be = vec2(300, 100);

		addEntities<SimpleEntity>(0, place_to_be, true);
		addEntities<TestEntity>(0, place_to_be, true);
		addEntities<ComplexEntity>(0, place_to_be, true);
		addEntities<Sprudo>(0, place_to_be);
		addEntities<Bulbous>(0, place_to_be);

		addCreatures<TestCharacter>(0, place_to_be);
		addCreatures<TestBird>(10, place_to_be);
		addCreatures<TestWorm>(20, place_to_be, true);

		guy = new GuyEntity();
		addEntity(guy);
		entities.push_back(guy);

		addObstruction(map = new TestMap());

		//addObstruction(tree = new TreeObstacle());
		//tree->position = {270, 500};

		//addObstruction(boost = new BoostObstacle());
		//boost->setPosition(vec2(500, 500));

		addObstruction(new MovingObstacle({400, 350}, {400, 475}, 100));
		addObstruction(new MovingObstacle({600, 300}, {700, 300}, 100));

		//addObstruction(new RampObstacle());
		//addObstruction(new MovingObstacle({50, 150}, {700, 300}, 250));
	}

	template<typename EntityT>
	void addEntities(size_t count, vec2 position, bool random_velo = false){
		static_assert(std::is_base_of<Entity, EntityT>::value, "Please give me an Entity instead.");
		for (size_t i = 0; i < count; i++){
			Entity* ent = new EntityT();
			ent->setPosition(position);
			if (random_velo){
				ent->velocity = vec2((((rand() % 100) - 50) / 10.0), (((rand() % 100) - 50) / 10.0));
			}
			addEntity(ent);
			entities.push_back(ent);
		}
	}

	template<typename EntityT>
	void addCreatures(size_t count, vec2 position, bool random_velo = false){
		static_assert(std::is_base_of<Creature, EntityT>::value, "Please give me an Creature instead.");
		for (size_t i = 0; i < count; i++){
			Creature* creature = new EntityT();
			creature->setPosition(position);
			if (random_velo){
				creature->velocity = vec2((((rand() % 100) - 50) / 10.0), (((rand() % 100) - 50) / 10.0));
			}
			addCreature(creature);
			entities.push_back(creature);
		}
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
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

		addThings<SimpleEntity>(10, place_to_be, true);
		addThings<TestEntity>(3, place_to_be, true);
		addThings<ComplexEntity>(3, place_to_be, true);
		addThings<Sprudo>(5, place_to_be);
		addThings<TestCharacter>(3, place_to_be);
		addThings<Bulbous>(2, place_to_be);
		addThings<TestBird>(15, place_to_be);
		addThings<TestWorm>(15, place_to_be, true);

		guy = new GuyEntity();
		addEntity(guy);
		entities.push_back(guy);

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

	template<typename EntityT>
	void addThings(size_t count, vec2 position, bool random_velo = false){
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
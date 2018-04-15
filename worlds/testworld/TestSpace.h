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
#include "testbug.h"

// TODO: rename to map?
struct TestSpace : Space {
	TestSpace(){
		vec2 place_to_be = vec2(470, 50);

		addEntities<SimpleEntity>(0, place_to_be, true);
		addEntities<TestEntity>(0, place_to_be, true);
		addEntities<ComplexEntity>(0, place_to_be, true);
		addEntities<Sprudo>(0, place_to_be);
		addEntities<Bulbous>(0, place_to_be);

		addCreatures<TestCharacter>(0, place_to_be, true);
		addCreatures<TestBird>(2, place_to_be, true);
		addCreatures<TestWorm>(5, place_to_be, true);
		addCreatures<TestBug>(1, place_to_be, true);

		
		entities.push_back(guy = addEntity<GuyEntity>());

		map = addObstruction<TestMap>();

		//addObstruction(tree = new TreeObstacle());
		//tree->position = {270, 500};

		//auto boost = addObstruction<BoostObstacle>().lock();
		//boost->setPosition(vec2(500, 500));

		//addObstruction<MovingObstacle>(vec2(400, 350), vec2(400, 475), 100.0f);
		addObstruction<MovingObstacle>(vec2(600, 300), vec2(700, 300), 100.0f);

		//addObstruction<RampObstacle>();
		//addObstruction(new MovingObstacle({50, 150}, {700, 300}, 250));
	}

	template<typename EntityT>
	void addEntities(size_t count, vec2 position, bool random_velo = false){
		static_assert(std::is_base_of<Entity, EntityT>::value, "Please give me an Entity instead.");
		for (size_t i = 0; i < count; i++){
			std::shared_ptr<EntityT> ent = addEntity<EntityT>().lock();
			ent->setPosition(position);
			if (random_velo){
				ent->velocity = vec2(
					((rand() % 100) - 50) * 0.1f,
					((rand() % 100) - 50) * 0.1f
				);
			}
			entities.push_back(ent);
		}
	}

	template<typename CreatureT>
	void addCreatures(size_t count, vec2 position, bool random_velo = false){
		static_assert(std::is_base_of<Creature, CreatureT>::value, "Please give me an Creature instead.");
		for (size_t i = 0; i < count; i++){
			std::shared_ptr<CreatureT> creature = addCreature<CreatureT>().lock();
			creature->setPosition(position);
			if (random_velo){
				creature->velocity = vec2(
					((rand() % 100) - 50) * 0.1f,
					((rand() % 100) - 50) * 0.1f
				);
			}
			entities.push_back(creature);
		}
	}

	void createEntity(vec2 position){
		auto ent = addEntity<ComplexEntity>().lock();
		ent->setPosition(position);
		entities.push_back(ent);
	}

	void update(){
		for (auto it = entities.begin(); it != entities.end();){
			if (it->expired()){
				it = entities.erase(it);
			} else {
				it++;
			}
		}
	}

	std::vector<std::weak_ptr<Entity>> entities;
	std::weak_ptr<GuyEntity> guy;
	std::weak_ptr<TestMap> map;
	std::weak_ptr<TreeObstacle> tree;
	std::weak_ptr<BoostObstacle> boost;
};
#pragma once

#include "TestEntities.h"
#include "ImageStore.h"
#include "SpriteSheetStore.h"

struct Bulbous : TestEntity {
	Bulbous() : sprite("bulbous") {
		addCircle(Circle(vec2(0, 0), 20));
		friction = 0.35;
		elasticity = 0.1;

		sprite.play("walking");

		int ticks = rand() % 10;
		for (int i = 0; i < ticks; i++){
			sprite.tick();
		}

		timer = 0;

		sprite.setScale(vec2(0.5, 0.5));
	}

	vec2 getContactAcceleration(const Obstruction* obstruction, vec2 normal) const {
		return vec2(direction * std::max(0.0, dot(normal, vec2(0, -1))), 0);
	}

	void tick() override {
		timer += 0.5;
		if (timer > 1){
			sprite.tick();
			timer = 0;
		}

		if (rand() % 100 == 0){
			direction = -direction;
			sprite.setScale(vec2(-direction * 0.5, 0.5));
		}
	}

	void render(sf::RenderWindow& rw, vec2 offset) override {
		sprite.render(rw, offset + position);
	}

	double timer;
	SpriteSheetPlayer sprite;
	sf::IntRect cliprect;
	int direction = 1;
};
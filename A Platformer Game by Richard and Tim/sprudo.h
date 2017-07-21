#pragma once

#include "TestEntities.h"
#include "ImageStore.h"
#include "SpriteSheetStore.h"

struct Sprudo : TestEntity {
	Sprudo() : sprite("sprudo") {
		addCircle(Circle(vec2(-3, -10), 25));
		addCircle(Circle(vec2(0, 45), 20));
		friction = 0.35;
		elasticity = 0.1;

		previous_position = position;

		sprite.play("walking");
	}

	vec2 getContactAcceleration(const Obstruction* obstruction, vec2 normal) const {
		return vec2(direction * std::max(0.0, dot(normal, vec2(0, -1))), 0);
	}

	void tick() override {
		timer += abs(position - previous_position);
		if (timer > 7){
			sprite.tick();
			timer = 0;
		}

		if (rand() % 100 == 0){
			direction = -direction;
			sprite.setScale(vec2(direction, 1));
		}

		previous_position = position;
	}

	void render(sf::RenderWindow& rw, vec2 offset) override {
		sprite.render(rw, offset + position);
	}

	SpriteSheetPlayer sprite;
	double timer = 0;
	sf::IntRect cliprect;
	int direction = 1;
	vec2 previous_position;
};
#pragma once

#include "TestEntities.h"
#include "ImageStore.h"
#include "SpriteSheetStore.h"

struct Bulbous : TestEntity {
	Bulbous() : sprite("bulbous", this) {
		addCircle(Circle(vec2(0, -15), 20));
		friction = 0.35;
		elasticity = 0.1;

		sprite.play("walking");

		sprite.setScale(vec2(0.5, 0.5));
	}

	vec2 getContactAcceleration(const Obstruction* obstruction, vec2 normal) const {
		return vec2(direction * std::max(0.0, dot(normal, vec2(0, -0.2))), 0);
	}

	void tick() override {
		sprite.tick();

		if (rand() % 100 == 0){
			direction = -direction;
			sprite.setScale(vec2(-direction * 0.5, 0.5));
		}
	}

	void render(sf::RenderWindow& rw, vec2 offset) override {
		sprite.render(rw, offset + position);
	}

	SpriteSheetPlayer sprite;
	sf::IntRect cliprect;
	int direction = 1;
};
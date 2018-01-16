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

		sprite.play("walking");
	}

	vec2 getContactAcceleration(const Obstruction* obstruction, vec2 normal) const {
		return vec2(direction * 0.6 * std::max(0.0f, dot(normal, vec2(0, -1))), 0);
	}

	void tick() override {
		sprite.tick();

		if (rand() % 100 == 0){
			direction = -direction;
			sprite.setScale(vec2(direction, 1));
		}
	}

	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override {
		states.transform *= getTransform();
		rt.draw(sprite, states);
	}

	SpriteSheetPlayer sprite;
	sf::IntRect cliprect;
	int direction = 1;
};
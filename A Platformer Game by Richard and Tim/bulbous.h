#pragma once

#include "TestEntities.h"
#include "ImageStore.h"
#include "SpriteSheetStore.h"

struct Bulbous : TestEntity {
	Bulbous() : sprite("bulbous") {
		addCircle(Circle(vec2(0, -15), 20));
		friction = 0.35f;
		elasticity = 0.1f;

		sprite.play("walking");

		sprite.setScale(vec2(0.5, 0.5));
	}

	vec2 getContactAcceleration(const Obstruction* obstruction, vec2 normal) const {
		return vec2(direction * std::max(0.0f, dot(normal, vec2(0, -0.2f))), 0);
	}

	void tick() override {
		sprite.tick();

		if (rand() % 100 == 0){
			direction = -direction;
			sprite.setScale(vec2(-direction * 0.5f, 0.5f));
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
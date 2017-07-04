#pragma once

#include "TestEntities.h"
#include "ImageStore.h"

struct Sprudo : TestEntity {
	Sprudo(){
		sprite.setTexture(ImageStore::getTexture("sprudo.png"));
		cliprect.width = 132;
		cliprect.height = 132;
		sprite.setTextureRect(cliprect);

		addCircle(Circle(vec2(-3, -10), 35));
		addCircle(Circle(vec2(0, 45), 20));
		friction = 0.5;
		elasticity = 0.1;
	}

	vec2 getContactAcceleration(const Obstruction* obstruction, vec2 normal) const {
		return vec2(2 * direction * std::max(0.0, dot(normal, vec2(0, -1))), 0);
	}

	void tick() override {
		cliprect.left = cliprect.width * frame;
		sprite.setTextureRect(cliprect);

		timer += 1;
		if (timer > 5){
			frame = (frame + 1) % 8;
			timer = 0;
		}

		if (rand() % 100 == 0){
			direction = -direction;
		}
	}

	void render(sf::RenderWindow& rw, vec2 offset) override {

		if (direction == 1){
			sprite.setScale(vec2(1, 1));
		} else {
			sprite.setScale(vec2(-1, 1));
		}

		sprite.setPosition(position + offset - vec2(direction * cliprect.width / 2, cliprect.height / 2));

		rw.draw(sprite);
	}

	sf::Sprite sprite;
	int frame = 0;
	int timer = 0;
	sf::IntRect cliprect;
	int direction = 1;
};
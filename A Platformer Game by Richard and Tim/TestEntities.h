#pragma once

#include "entity.h"
#include "SpriteSheetStore.h"

struct TestEntity : Entity {
	TestEntity(){
		position = vec2(470 + (rand() % 100) * 0.01, 50 + (rand() % 100) * 0.01);
		elasticity = (rand() % 100) * 0.01;
		friction = (rand() % 100) * 0.01;
	}

	void render(sf::RenderWindow& rw, vec2 offset) override {
		sf::CircleShape circle;
		circle.setFillColor(sf::Color(255 * friction, 255 * elasticity, 127, 255));
		//circle.setFillColor(sf::Color((uint32_t)std::hash<TestEntity*>{}(this) | 0xFF));
		circle.setOutlineThickness(0.5);
		circle.setOutlineColor(sf::Color(0xFF));
		circle.setScale(vec2(getScale(), getScale()));
		for (Circle& c : circles){
			circle.setRadius(c.radius);
			circle.setPointCount(2 * pi * c.radius);
			circle.setPosition(offset + position + (c.center - vec2(c.radius, c.radius)) * getScale());
			rw.draw(circle);
		}
	}
};

struct SimpleEntity : TestEntity {
	SimpleEntity(){
		addCircle(Circle({0, 0},5 + rand() % 25));
		mass = 10.0;
	}
};

struct ComplexEntity : TestEntity {
	ComplexEntity(){
		int count = (rand() % 5) + 1;
		for (int i = 0; i < count; i++){
			addCircle(Circle(vec2(-25 + rand() % 51, -25 + rand() % 51), 5 + rand() % 25));
		}
		mass = 20.0;
	}
};

struct GuyEntity : TestEntity {
	GuyEntity() : sprite("char1", this){
		addCircle(Circle({0, -30}, 25));
		addCircle(Circle({0, 0}, 20));
		addCircle(Circle({0, 30}, 25));
		friction = 0.3;
		elasticity = 0.1;
		sprite.setScale(vec2(0.25, 0.25));
		sprite.play("walking");
	}

	vec2 getContactAcceleration(const Obstruction* obstruction, vec2 normal) const override {
		vec2 accel = {0, 0};
		accel += run_speed * vec2(1, 0);
		accel += vec2(0, jump_speed * -std::max(0.0, dot(normal, {0, -1})));
		return accel;
	}

	void tick() override {
		sprite.tick();
	}

	void render(sf::RenderWindow& rw, vec2 offset) override {
		sprite.render(rw, offset + position);
	}

	void updateMoves(float _run_speed, float _jump_speed){
		run_speed = _run_speed;
		jump_speed = _jump_speed;
	}

	SpriteSheetPlayer sprite;
	float run_speed = 0.0f;
	float jump_speed = 0.0f;
};
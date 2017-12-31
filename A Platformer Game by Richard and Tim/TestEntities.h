#pragma once

#include "entity.h"
#include "SpriteSheetStore.h"

struct TestEntity : Entity {
	TestEntity(){
		setPosition(vec2(470 + (rand() % 100) * 0.01, 50 + (rand() % 100) * 0.01));
		elasticity = (rand() % 100) * 0.01;
		friction = (rand() % 100) * 0.01;
	}

	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override {
		states.transform *= getTransform();
		sf::CircleShape circle;
		circle.setFillColor(sf::Color(255 * friction, 255 * elasticity, 127, 255));
		//circle.setFillColor(sf::Color((uint32_t)std::hash<TestEntity*>{}(this) | 0xFF));
		circle.setOutlineThickness(0.5);
		circle.setOutlineColor(sf::Color(0xFF));


		for (const Circle& c : circles){
			circle.setRadius(c.radius);
			circle.setPointCount(2 * pi * c.radius);
			circle.setPosition(c.center - vec2(c.radius, c.radius));
			rt.draw(circle, states);
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
	GuyEntity() : sprite("playerchar", this){
		addCircle(Circle(vec2(0, -10), 20));
		addCircle(Circle({0, -40}, 30));
		friction = 0.3;
		elasticity = 0.1;
		sprite.setScale(vec2(0.25, 0.25));
		sprite.play("idle");
		sprite.scale(2.0f, 2.0f);
	}

	vec2 getContactAcceleration(const Obstruction* obstruction, vec2 normal) const override {
		vec2 accel = {0, 0};
		accel += run_speed * vec2(1, 0);
		accel += vec2(0, jump_speed * -std::max(0.0, dot(normal, {0, -1})));
		return accel;
	}

	void onEvent(const Event& e) override {
		if (e == AnimationEnd){
			sprite.play("idle");
		}
	}

	void tick() override {
		sprite.tick();
		jump_speed = 0.0f;
		run_speed = 0.0f;
	}

	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override {
		TestEntity::draw(rt, states);
		states.transform *= getTransform();
		rt.draw(sprite, states);
	}

	void runRight(){
		sprite.faceRight();
		sprite.play("running");
		run_speed = 1.0f;
	}
	void runLeft(){
		sprite.faceLeft();
		sprite.play("running");
		run_speed = -1.0f;
	}
	void jump(){
		sprite.play("jump");
		jump_speed = 10.0f;
	}
	void attack(){
		sprite.play("swordslash");
	}

	private:
	SpriteSheetPlayer sprite;
	float run_speed = 0.0f;
	float jump_speed = 0.0f;
};
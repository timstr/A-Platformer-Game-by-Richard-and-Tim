#pragma once

#include "entity.h"
#include "SpriteSheetStore.h"

struct TestEntity : Entity {
	TestEntity(){
		setPosition(vec2(470 + (rand() % 100) * 0.01f, 50.0f + (rand() % 100) * 0.01f));
		elasticity = (rand() % 100) * 0.01f;
		friction = (rand() % 100) * 0.01f;
	}

	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override {
		states.transform *= getTransform();
		sf::CircleShape circle;
		circle.setFillColor(sf::Color(
			(uint8_t)(255 * friction),
			(uint8_t)(255 * elasticity),
			127,
			255
		));
		//circle.setFillColor(sf::Color((uint32_t)std::hash<TestEntity*>{}(this) | 0xFF));
		circle.setOutlineThickness(0.5);
		circle.setOutlineColor(sf::Color(0xFF));


		for (const Circle& c : circles){
			circle.setRadius(c.radius);
			circle.setPointCount((unsigned)(2 * pi * c.radius));
			circle.setPosition(c.center - vec2(c.radius, c.radius));
			rt.draw(circle, states);
		}
	}
};

struct SimpleEntity : TestEntity {
	SimpleEntity(){
		addCircle(Circle(vec2(0.0f, 0.0f), 5.0f + (rand() % 25)));
		mass = 10.0;
	}
};

struct ComplexEntity : TestEntity {
	ComplexEntity(){
		int count = (rand() % 5) + 1;
		for (int i = 0; i < count; i++){
			addCircle(Circle(vec2(-25.0f + (rand() % 51), -25.0f + (rand() % 51)), 5.0f + (rand() % 25)));
		}
		mass = 20.0;
	}
};

struct GuyEntity : TestEntity {
	GuyEntity() : sprite("HermannHorst"){
		addCircle(Circle({0, -250}, 220));
		friction = 0.3f;
		elasticity = 0.1f;
		//sprite.setScale(vec2(0.2f, 0.2f));
		setScale(vec2(0.2f, 0.2f));
		sprite.play("standing");
		//sprite.scale(0.2f, 0.2f);
	}

	vec2 getContactAcceleration(const Obstruction& obstruction, vec2 normal) const override {
		vec2 accel = {0, 0};
		accel += run_speed * vec2(1, 0);
		accel += vec2(0, jump_speed * -std::max(0.0f, dot(normal, {0, -1})));
		return accel;
	}

	void onEvent(const Event& e) override {
		if (e == AnimationEnd){
			sprite.play("standing");
		}
		if (e == Standing && sprite.currentAnimation() == "leaping" && velocity.y < 0.0f){
			sprite.play("standing");
		}
	}

	void tick() override {
		sprite.tick();
		jump_speed = 0.0f;
		run_speed = 0.0f;
	}

	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override {
		//TestEntity::draw(rt, states);
		states.transform *= getTransform();
		rt.draw(sprite, states);
	}

	void runRight(){
		sprite.faceRight();
		sprite.play("running");
		run_speed = 5.0f;
		velocity.x += 0.1f;
	}
	void runLeft(){
		sprite.faceLeft();
		sprite.play("running");
		run_speed = -5.0f;
		velocity.x -= 0.1f;
	}
	void jump(){
		sprite.play("leaping");
		jump_speed = 70.0f;
		velocity.y -= 0.5f;
	}

	private:
	SpriteSheetPlayer sprite;
	float run_speed = 0.0f;
	float jump_speed = 0.0f;
};
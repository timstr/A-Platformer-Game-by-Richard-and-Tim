#pragma once

#include "obstacle.h"
#include "ImageStore.h"

struct TreeObstacle : Obstacle {
	TreeObstacle(){
		setBoundary(ImageStore::getImage("testobstacle1"));
		setSprite(sf::Sprite(ImageStore::getTexture("testobstacle1")));
		setFriction(0);
	}
};

struct BoostObstacle : Obstacle {
	BoostObstacle(){
		setBoundary(ImageStore::getImage("testobstacle2"));
		setSprite(sf::Sprite(ImageStore::getTexture("testobstacle2")));
		setFriction(0);
	}

	vec2 getContactAcceleration(const Entity& entity, vec2 normal) const override {
		float align = std::max(0.0f, dot(normal, vec2(0, -1)));
		return vec2(2.0f * align, 0.0f);
	}
};

struct MovingObstacle : Obstacle {
	MovingObstacle(vec2 _position1, vec2 _position2, float frames){
		speed = 2 * pi / frames;

		position1 = _position1;
		position2 = _position2;

		setBoundary(ImageStore::getImage("testobstacle3"));
		setSprite(sf::Sprite(ImageStore::getTexture("testobstacle3")));

		setFriction(1);
	}

	void tick() override {
		phase += speed;
		setPosition(calcPosition(phase));
	}

	private:

	vec2 calcPosition(float _phase) const {
		return position1 + (position2 - position1) * (float)(0.5 + 0.5 * sin(_phase));
	}

	float phase = 0;
	float speed;
	vec2 position1, position2;
};

struct RampObstacle : Obstacle {
	RampObstacle(){
		setBoundary(ImageStore::getImage("testobstacle4"));
		setSprite(sf::Sprite(ImageStore::getTexture("testobstacle4")));

		setFriction(0);
	}
};
#pragma once

#include "obstacle.h"
#include "ImageStore.h"

struct TreeObstacle : Obstacle {
	TreeObstacle(){
		setBoundary(ImageStore::getImage("testobstacle1"));
		sprite.setTexture(ImageStore::getTexture("testobstacle1"));
		friction = 0.0;
	}
};

struct BoostObstacle : Obstacle {
	BoostObstacle(){
		setBoundary(ImageStore::getImage("testobstacle2"));
		sprite.setTexture(ImageStore::getTexture("testobstacle2"));
		friction = 0.0;
	}

	vec2 getContactAcceleration(const Entity* entity, vec2 normal) const override {
		double align = std::max(0.0, dot(normal, vec2(0, -1)));
		return vec2(2.0 * align, 0.0);
	}
};

struct MovingObstacle : Obstacle {
	MovingObstacle(vec2 _position1, vec2 _position2, double frames){
		speed = 2 * pi / frames;

		position1 = _position1;
		position2 = _position2;

		setBoundary(ImageStore::getImage("testobstacle3"));
		sprite.setTexture(ImageStore::getTexture("testobstacle3"));

		friction = 1.0;
	}

	void tick() override {
		phase += speed;
		setPosition(getPosition(phase));
	}

	private:

	vec2 getPosition(double _phase) const {
		return position1 + (position2 - position1) * (float)(0.5 + 0.5 * sin(_phase));
	}

	double phase = 0;
	double speed;
	vec2 position1, position2;
};

struct RampObstacle : Obstacle {
	RampObstacle(){
		setBoundary(ImageStore::getImage("testobstacle4"));
		sprite.setTexture(ImageStore::getTexture("testobstacle4"));

		friction = 0.0;
	}
};
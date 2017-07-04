#pragma once

#include "obstacle.h"

struct TreeObstacle : Obstacle {
	TreeObstacle(){
		image.loadFromFile("images/testobstacle1.png");
		texture.loadFromImage(image);
		sprite.setTexture(texture);

		setImage(sprite);
		setBoundary(image);
		friction = 0.0;
	}

	private:
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
};

struct BoostObstacle : Obstacle {
	BoostObstacle(){
		image.loadFromFile("images/testobstacle2.png");
		texture.loadFromImage(image);
		sprite.setTexture(texture);

		setImage(sprite);
		setBoundary(image);
		friction = 0.0;
	}

	vec2 getContactAcceleration(const Entity* entity, vec2 normal) const override {
		double align = std::max(0.0, dot(normal, vec2(0, -1)));
		return vec2(2.0 * align, 0.0);
	}

	private:
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
};


struct RampObstacle : Obstacle {
	RampObstacle(){
		image.loadFromFile("images/testobstacle4.png");
		texture.loadFromImage(image);
		sprite.setTexture(texture);

		setImage(sprite);
		setBoundary(image);
		friction = 0.0;
	}

	private:
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
};

struct MovingObstacle : Obstacle {
	MovingObstacle(vec2 _position1, vec2 _position2, double frames){
		speed = 2 * pi / frames;

		position1 = _position1;
		position2 = _position2;

		image.loadFromFile("images/testobstacle3.png");
		texture.loadFromImage(image);
		sprite.setTexture(texture);

		setImage(sprite);
		setBoundary(image);
		friction = 1.0;
	}

	void tick() override {
		phase += speed;
		position = getPosition(phase);
	}

	private:

	vec2 getPosition(double _phase) const {
		return position1 + (position2 - position1) * (float)(0.5 + 0.5 * sin(_phase));
	}

	double phase;
	double speed;
	vec2 position1, position2;

	private:
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
};
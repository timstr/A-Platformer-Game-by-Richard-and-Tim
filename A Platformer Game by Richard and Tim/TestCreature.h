#pragma once

#include "creature.h"

namespace Event {
	Event impact;
}

struct TestCreature : Creature {

	TestCreature(){
		addStateTransition(redstate, greenstate, Event::impact);
		addStateTransition(greenstate, bluestate, Event::impact);
		addStateTransition(bluestate, redstate, Event::impact);
		setState(redstate);

		radius = 5 + (rand() % 25);

		addCircle(Circle({0, 0}, radius));
	}

	void tick() override {
		if (abs(velocity - prev_velocity) > 2){
			onEvent(Event::impact);
		}
		prev_velocity = velocity;
	}

	void render(sf::RenderWindow& rw, vec2 offset) override {
		sf::CircleShape circle;
		circle.setPosition(position + offset - vec2(radius, radius));
		circle.setRadius(radius);
		circle.setPointCount(radius * 2 * pi);
		uint32_t color = 0x808080FF;
		switch (getState()){
			case redstate:
				color = 0xFF0000FF;
			break;
			case greenstate:
				color = 0x00FF00FF;
			break;
			case bluestate:
				color = 0x0000FFFF;
			break;
		}
		circle.setFillColor(sf::Color(color));
		circle.setOutlineColor(sf::Color(0xFF));
		circle.setOutlineThickness(1.0f);
		rw.draw(circle);
	}

	enum States {
		redstate, greenstate, bluestate
	};

	int radius;

	vec2 prev_velocity = {0, 0};
};
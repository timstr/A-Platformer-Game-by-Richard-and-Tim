#pragma once

#include "creature.h"
#include "spritesheet.h"

#include <iostream>

struct TestCharacter : Creature {
	// TODO: move the spritesheetplayer into the Creature class and couple animations and state machinery more tightly?
	TestCharacter() : Creature("char1") {
		addCircle(Circle({0, -30}, 30));
		friction = 0.3;
		elasticity = 0.1;
		sprite.setScale(vec2(-0.3, 0.3));

		addStateTransition(idle, idle, AnimationEnd, 3);
		addStateTransition(idle, walking, AnimationEnd, 1);

		addStateTransition(walking, walking, AnimationEnd, 3);
		addStateTransition(walking, idle, AnimationEnd, 1);
		addStateTransition(walking, running, AnimationEnd, 1);

		addStateTransition(running, running, AnimationEnd, 2);
		addStateTransition(running, walking, AnimationEnd, 1);

		setState(idle);
	}

	vec2 getContactAcceleration(const Obstruction* obstruction, vec2 normal) const override {
		switch (getState()){
			case walking:
				return vec2(0.5, 0);
			case running:
				return vec2(2, 0);
			default:
				return vec2(0, 0);
		}
	}

	void update() override {
		sprite.tick();
		switch (getState()){
			case idle:
				sprite.play("idle");
				break;
			case walking:
				sprite.play("walking");
				break;
			case running:
				sprite.play("running");
				break;
			case leaping:
				sprite.play("leaping");
				break;
		}
	}

	private:

	enum States {
		idle,
		walking,
		running,
		leaping
	};
};
#pragma once

#include "creature.h"
#include "spritesheet.h"

#include <iostream>

struct TestCharacter : Creature {
	// TODO: move the spritesheetplayer into the Creature class and couple animations and state machinery more tightly?
	TestCharacter() : Creature("char1") {
		addCircle(Circle({0, -150}, 50));
		addCircle(Circle({0, 0}, 50));
		addCircle(Circle({0, 170}, 50));
		friction = 0.3;
		elasticity = 0.1;

		float scale = 0.1f + (rand() % 20) * 0.01f;

		setScale(scale, scale);

		addStateTransition(idle, idle, AnimationEnd, 3);
		addStateTransition(idle, walking, AnimationEnd, 1);
		addStateTransition(idle, walking, AnimationEnd, 1, [this]{
			direction *= -1;
			flip();
		});

		addStateTransition(walking, walking, AnimationEnd, 3);
		addStateTransition(walking, walking, AnimationEnd, 0.5,  [this]{
			direction *= -1;
			flip();
		});
		addStateTransition(walking, idle, AnimationEnd, 1);
		addStateTransition(walking, running, AnimationEnd, 1);

		addStateTransition(running, running, AnimationEnd, 2);
		addStateTransition(running, walking, AnimationEnd, 1);

		addStateTransition(walking, jump, AnimationEnd, 0.5);
		addStateTransition(running, jump, AnimationEnd, 1);

		addStateTransition(jump, leaping, Tick);

		addStateTransition(idle, leaping, Flying);
		addStateTransition(walking, leaping, Flying);
		addStateTransition(running, leaping, Flying);

		addStateTransition(leaping, walking, Standing, 2);
		addStateTransition(leaping, running, Standing, 2);


		setAnimation(idle, "idle");
		setAnimation(walking, "walking");
		setAnimation(running, "running");
		setAnimation(leaping, "leaping");

		setState(idle);
	}

	vec2 getContactAcceleration(const Obstruction* obstruction, vec2 normal) const override {
		switch (getState()){
			case walking:
				return vec2(2 * direction, 0);
			case running:
				return vec2(8 * direction, 0);
			case jump:
				return vec2(0, -(20 + rand() % 60));
			default:
				return vec2(0, 0);
		}
	}

	void update() override {
		if (getState() == leaping){
			velocity += vec2(direction * 0.1, 0);
		}
	}

	private:

	int direction = 1;

	enum States {
		idle,
		walking,
		running,
		leaping,
		jump
	};
};
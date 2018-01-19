#pragma once

#include "creature.h"
#include "spritesheet.h"

#include <iostream>

struct TestCharacter;

CreatureTypeT<TestCharacter> TestCharacterType;

struct TestCharacter : Creature {
	TestCharacter() : Creature("char1") {
		setType(TestCharacterType);
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
			flip();
		});

		addStateTransition(walking, walking, AnimationEnd, 3);
		addStateTransition(walking, walking, AnimationEnd, 0.5,  [this]{
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


		setStateAnimation(idle, "idle");
		setStateAnimation(walking, "walking");
		setStateAnimation(running, "running");
		setStateAnimation(leaping, "leaping");

		setState(idle);
	}

	vec2 getContactAcceleration(const Obstruction* obstruction, vec2 normal) const override {
		switch (getState()){
			case walking:
				return vec2(2 * getDirection(), 0);
			case running:
				return vec2(8 * getDirection(), 0);
			case jump:
				return vec2(0, -(20 + rand() % 60));
			default:
				return vec2(0, 0);
		}
	}

	void update() override {
		if (getState() == leaping){
			velocity += vec2(getDirection() * 0.1, 0);
		}
	}

	private:

	enum States {
		idle,
		walking,
		running,
		leaping,
		jump
	};
};
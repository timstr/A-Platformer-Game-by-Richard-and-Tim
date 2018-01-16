#pragma once

#include "creature.h"

struct TestBird;

CreatureTypeT<TestBird> TestBirdType;

struct TestBird : Creature {
	TestBird() : Creature("bird") {
		setType(TestBirdType);

		elasticity = 0.05;
		friction = 0.7;

		addCircle(Circle({0, -50}, 50));

		setScale(0.5f, 0.5f);

		addStateTransition(Idle, Idle, Tick, 20);
		addStateTransition(Idle, Idle, Tick, 0.2, [this]{
			flip();
		});
		addStateTransition(Idle, Flying, Tick, 0.2);
		addStateTransition(Flying, Flying, Tick, 15);
		addStateTransition(Flying, Flying, Tick, 0.2, [this]{
			flip();
		});
		addStateTransition(Flying, Idle, Tick, 1);

		setStateAnimation(Idle, "idle");
		setStateAnimation(Flying, "flying");

		setState(Idle);
	}

	void update() override {
		switch (getState()){
			case Idle:

				break;
			case Flying:
				velocity.y += (rand() % 201 - 100) * 0.01f - 0.6f;
				velocity.x += (rand() % 101) * 0.01f * getDirection();
				break;
		}
	}

	enum State {
		Idle,
		Flying
	};
};
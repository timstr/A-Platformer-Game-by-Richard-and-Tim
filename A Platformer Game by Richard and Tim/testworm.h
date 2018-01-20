#pragma once

#include "creature.h"

struct TestWorm;

CreatureTypeT<TestWorm> TestWormType;

struct TestWorm : Creature {
	TestWorm() : Creature("woim") {
		elasticity = 0.05f;
		friction = 0.5f;

		setScale(0.7f, 0.7f);

		setType(TestWormType);
		addCircle(Circle(vec2(0, -10), 10));

		setStateAnimation(Idle, "idle");
		setStateAnimation(Worming, "worming");

		addStateTransition(Idle, Idle, Tick, 20);
		addStateTransition(Idle, Idle, Tick, 0.3, [this]{
			this->flip();
		});
		addStateTransition(Idle, Worming, Tick, 0.1);
		addStateTransition(Worming, Worming, Tick, 20);
		addStateTransition(Worming, Idle, Tick, 0.3);

		setState(Idle);
	}

	vec2 getContactAcceleration(const Obstruction& obstruction, vec2 normal) const override {
		if (getState() == Worming){
			return vec2(getDirection() * 0.5f, 0) * std::max(0.0f, dot(normal, vec2(0, -1)));
		}
		return vec2(0, 0);
	}

	enum States {
		Idle,
		Worming
	};
};
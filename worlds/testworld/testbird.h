#pragma once

#include "creature.h"

struct TestBird;

extern CreatureTypeT<TestBird> TestBirdType;

#include "testworm.h"
#include "testbug.h"


struct TestBird : Creature {
	TestBird();

	vec2 getContactAcceleration(const Obstruction& obstruction, vec2 normal) const override;

	void update() override;

	private:

	std::weak_ptr<Creature> prey;

	enum State {
		Idle,
		Flying,
		Hunting,
		Watching,
		Eating
	};
};
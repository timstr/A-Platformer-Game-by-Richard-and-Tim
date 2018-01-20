#pragma once

#include "creature.h"
#include "testbird.h"

struct TestBug;

CreatureTypeT<TestBug> TestBugType;

struct TestBug : Creature {
	TestBug() : Creature("bug") {
		setType(TestBugType);

		addCircle(Circle(vec2(0, -20), 20));

		setAwarenessRadius(80);

		setScale(0.6f, 0.6f);
		elasticity = 0.3f;
		friction = 0.9f;

		addStateTransition(Idle, Idle, Tick, 10);
		addStateTransition(Idle, Idle, Tick, 0.05, [this]{
			this->flip();
		});
		addStateTransition(Idle, Running, Tick, 0.1);
		addStateTransition(Running, Running, Tick, 10);
		addStateTransition(Running, Running, Tick, 0.3, [this]{
			this->flip();
		});
		addStateTransition(Running, Idle, AnimationEnd, 0.5);

		addStateTransition(PlayDead, PlayDead, Tick, 20);

		setStateAnimation(Idle, "idle");
		setStateAnimation(Running, "running");
		setStateAnimation(PlayDead, "playdead");

		onNotice(TestBirdType, [this](std::weak_ptr<Creature> creature){
			setState(PlayDead);
			birdclock.restart();
		});

		setState(Idle);
	}

	vec2 getContactAcceleration(const Obstruction& obstruction, vec2 normal) const override {
		if (getState() == Running){
			return vec2(getDirection() * 2, 0) * dot(normal, vec2(0, -1));
		}
		return vec2(0, 0);
	}

	void update() override {
		if (getState() == PlayDead && birdclock.getElapsedTime().asSeconds() > 2.0f){
			setState(Idle);
		}
	}

	enum States {
		Idle,
		Running,
		PlayDead
	};

	private:
	sf::Clock birdclock;
};
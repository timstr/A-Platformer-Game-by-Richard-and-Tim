#pragma once

#include "creature.h"
#include "testworm.h"

struct TestBird;

CreatureTypeT<TestBird> TestBirdType;

struct TestBird : Creature {
	TestBird() : Creature("bird") {
		setType(TestBirdType);
		setAwarenessRadius(250);
		onNotice(TestWormType, [this](Creature* creature){
			if (getState() != Hunting && getState() != Watching && getState() != Eating){
				TestWorm* worm = dynamic_cast<TestWorm*>(creature);
				this->prey = worm;
				setState(Hunting);
			}
		});

		elasticity = 0.05;
		friction = 0.7;

		addCircle(Circle({0, -50}, 50));

		setScale(0.9f, 0.9f);

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
		addStateTransition(Hunting, Hunting, Tick, 10);
		addStateTransition(Hunting, Idle, Tick, 0.1, [this]{
			prey = nullptr;
		});
		addStateTransition(Watching, Watching, Tick, 10);
		addStateTransition(Watching, Eating, Tick, 0.1);
		addStateTransition(Eating, Idle, AnimationEnd);

		setStateAnimation(Idle, "idle");
		setStateAnimation(Flying, "flying");
		setStateAnimation(Hunting, "sneaking");
		setStateAnimation(Watching, "watching");
		setStateAnimation(Eating, "eating");

		setState(Idle);
	}

	vec2 getContactAcceleration(const Obstruction* obstruction, vec2 normal) const override {
		if (getState() == Hunting){
			return vec2(getDirection() * 1.0f, 0) * std::max(0.0f, dot(normal, vec2(0, -1)));
		}
		return vec2(0, 0);
	}

	void update() override {
		switch (getState()){
			case Idle:

				break;
			case Flying:
				velocity.y += (rand() % 201 - 100) * 0.01f - 0.8f;
				velocity.x += (rand() % 101) * 0.01f * getDirection();
				break;
			case Hunting:
				if (prey){
					float diff = prey->getPosition().x - getPosition().x;
					if (abs(diff) > 20.0f){
						setDirection(diff);
					} else {
						setState(Watching);
					}
				}
				break;
			case Watching:
				if (prey){
					float diff = prey->getPosition().x - getPosition().x;
					if (abs(diff) > 50.0f){
						setState(Hunting);
					}
				} else {
					setState(Idle);
				}
				break;
		}
	}

	private:

	Creature* prey = nullptr;

	enum State {
		Idle,
		Flying,
		Hunting,
		Watching,
		Eating
	};
};
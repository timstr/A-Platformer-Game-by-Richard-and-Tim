#pragma once

struct TestBird;

CreatureTypeT<TestBird> TestBirdType;

#include "creature.h"
#include "testworm.h"
#include "testbug.h"

struct TestBird : Creature {
	TestBird() : Creature("bird") {
		setType(TestBirdType);
		setAwarenessRadius(250);

		onNotice(TestWormType, [this](std::weak_ptr<Creature> creature){
			if (getState() == Idle || getState() == Flying){
				this->prey = creature;
				setState(Hunting);
			}
		});
		onNotice(TestBugType, [this](std::weak_ptr<Creature> creature){
			if (getState() == Idle || getState() == Flying){
				if (auto c = creature.lock()){
					if (c->getState() != TestBug::PlayDead){
						this->prey = creature;
						setState(Hunting);
					}
				}
			}
		});
		

		elasticity = 0.05f;
		friction = 0.7f;

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
			prey.reset();
		});
		addStateTransition(Watching, Watching, Tick, 10);
		addStateTransition(Watching, Idle, Tick, 0.05);
		addStateTransition(Watching, Eating, Tick, 0.1, [this]{
			if (auto p = prey.lock()){
				if (p->hasType(TestBugType)){
					// whoops, don't eat the bug
					setState(Watching);
				}
			}
		});
		addStateTransition(Eating, Idle, AnimationEnd, 1, [this]{
			if (auto p = prey.lock()){
				p->destroy();
				prey.reset();
			}
		});

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
				if (auto p = prey.lock()){
					float diff = p->getPosition().x - getPosition().x;
					if (abs(diff) > 20.0f){
						setDirection(diff);
					} else {
						setState(Watching);
					}
				} else {
					setState(Idle);
					prey.reset();
				}
				break;
			case Watching:
				if (auto p = prey.lock()){
					float diff = p->getPosition().x - getPosition().x;
					if (abs(diff) > 50.0f){
						setState(Hunting);
					} else if (abs(diff) > 20.0f){
						setDirection(diff);
					}
				} else {
					setState(Idle);
					prey.reset();
				}
				break;
		}
	}

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
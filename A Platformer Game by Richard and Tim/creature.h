#pragma once

#include <map>
#include "character.h"
#include "spritesheet.h"
#include "event.h"

// Creature is an autonomous character that moves around
// independently and can interact with other characters
struct Creature : Character {

	Creature(std::string name);

	void onEvent(Event e) override;

	void tick() override final;

	virtual void update();

	void addStateTransition(int from_state, int to_state, Event trigger_event, double relative_probability = 1.0);

	void setState(int state);

	int getState() const;

	void render(sf::RenderWindow& rw, vec2 offset) override;

	protected:

	SpriteSheetPlayer sprite;

	private:

	int current_state = -1;

	struct Trigger {
		Trigger(int _from_state, const Event& _event);

		int from_state;
		int event_id;
		//const Event::Event& event;

		friend bool operator<(const Trigger& l, const Trigger& r){
			if (l.from_state < r.from_state){
				return true;
			} else if (l.from_state == r.from_state){
				return l.event_id < r.event_id;
			} else {
				return false;
			}
		}
	};

	struct Transition {
		Transition(int _to_state, double _relative_probability);

		int to_state;
		double relative_probability;
	};

	std::multimap<Trigger, Transition> transitions;
};
#pragma once

#include <map>
#include "character.h"
#include "spritesheet.h"
#include "event.h"

// Creature is an autonomous character that moves around
// independently and can interact with other characters
struct Creature : Character {

	Creature(const std::string& name);

	void onEvent(const Event& e) override;

	void tick() override final;

	virtual void update();

	void addStateTransition(int from_state, int to_state, const Event& trigger_event, double relative_probability = 1.0, const std::function<void()>& onComplete = {});
	void addStateTransition(int to_state, const Event& trigger_event, const std::function<void()> onComplete = {});

	template<typename CreatureType>
	CreatureType* as() const {
		static_assert(std::is_base_of<Creature, CreatureType>::value, "The provided CreatureType must derive from Creature");
		return dynamic_cast<CreatureType*>(this);
	}



	void setStateAnimation(int state, const std::string& animation);

	void setState(int state);
	int getState() const;

	void flip();

	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;

	protected:

	SpriteSheetPlayer sprite;

	private:

	int current_state = -1;
	int direction = 1;

	struct Trigger {
		Trigger(int _from_state, const Event& _event);

		int from_state;
		int event_id;

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
		Transition(int _to_state, double _relative_probability, const std::function<void()>& _onComplete);

		int to_state;
		double relative_probability;
		std::function<void()> onComplete;
	};

	std::multimap<Trigger, Transition> transitions;
	std::map<int, std::string> animations;
};
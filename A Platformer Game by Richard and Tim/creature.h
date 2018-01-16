#pragma once

#include <map>
#include "character.h"
#include "spritesheet.h"
#include "event.h"

struct CreatureType {
	CreatureType(const CreatureType* _base_type) : base_type(_base_type) {

	}

	const CreatureType* getBaseType() const {
		return base_type;
	}

	bool hasBase(const CreatureType& type) const {
		const CreatureType* bt = this;
		while (bt){
			if (bt == &type){
				return true;
			}
			bt = bt->base_type;
		}
		return false;
	}

	private:
	const CreatureType* const base_type;
};

template<typename Type>
struct CreatureTypeT : CreatureType {
	CreatureTypeT() : CreatureType(nullptr) {
		static_assert(std::is_base_of<Creature, Type>::value, "Type must derive from Creature");
	}
	template<typename BaseType>
	CreatureTypeT(const CreatureTypeT<BaseType>& _base_type) : CreatureType(&_base_type) {
		static_assert(std::is_base_of<BaseType, Type>::value, "BaseType must be a base type of Type");
	}
};

// TODO: use singleton CRTP to hold common data like state transitions and creature notice handlers?
// TODO: add awareness radius, within which likelihood of noticing another creature increases
// TODO: is it safe to call setState() inside state transition callback?

// Creature is an autonomous character that moves around
// independently and can interact with other characters
struct Creature : Character {
	Creature(const std::string& name);

	void setType(const CreatureType& type);

	void onEvent(const Event& e) override;

	void tick() override final;

	virtual void update();

	void addStateTransition(uint8_t from_state, uint8_t to_state, const Event& trigger_event, double relative_probability = 1.0, const std::function<void()>& onComplete = {});
	void addStateTransition(uint8_t to_state, const Event& trigger_event, const std::function<void()> onComplete = {});

	void setStateAnimation(uint8_t state, const std::string& animation);

	void setState(uint8_t state);
	int getState() const;

	void notice(Creature* creature);

	void onNotice(const CreatureType& creaturetype, const std::function<void(Creature*)>& handler);

	template<typename Type>
	Type* as(const CreatureTypeT<Type>& basetype){
		if (type->hasBase(basetype)){
			return dynamic_cast<Type>(this);
		} else {
			return nullptr;
		}
	}

	void flip();

	void setDirection(float dir);
	float getDirection() const;

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

	// TODO: can these be made static, per creature subtype?
	std::multimap<Trigger, Transition> transitions;
	std::map<int, std::string> animations;
	std::map<const CreatureType*,std::function<void(Creature*)>> type_handlers;
	const CreatureType* type;
};
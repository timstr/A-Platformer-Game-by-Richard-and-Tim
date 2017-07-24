#pragma once

#include "creature.h"

Creature::Creature(std::string name) : sprite(name, this) {

}

void Creature::onEvent(Event e){
	Trigger trigger = Trigger(current_state, e);

	auto begin = transitions.lower_bound(trigger);
	auto end = transitions.upper_bound(trigger);

	if (begin == end){
		return;
	}

	double total = 0.0;

	for (auto it = begin; it != end; it++){
		total += it->second.relative_probability;
	}

	double rand_val = rand() * total / (double)RAND_MAX;

	for (auto it = begin; it != end; it++){
		rand_val -= it->second.relative_probability;
		if (rand_val <= 0){
			current_state = it->second.to_state;
			return;
		}
	}
}

void Creature::tick(){
	onEvent(Tick);
	update();
}

void Creature::update(){

}

void Creature::addStateTransition(int from_state, int to_state, Event trigger_event, double relative_probability){
	transitions.insert(std::make_pair(Trigger(from_state, trigger_event), Transition(to_state, relative_probability)));
}

void Creature::setState(int state){
	current_state = state;
}

int Creature::getState() const {
	return current_state;
}

void Creature::render(sf::RenderWindow& rw, vec2 offset){
	sprite.render(rw, offset + position);
}

Creature::Trigger::Trigger(int _from_state, const Event& _event){
	from_state = _from_state;
	event_id = _event.id;
}

Creature::Transition::Transition(int _to_state, double _relative_probability){
	to_state = _to_state;
	relative_probability = _relative_probability;
}
#pragma once

#include "creature.h"

Creature::Creature(const std::string& name) : sprite(name, this) {

}

void Creature::onEvent(const Event& e){
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
			setState(it->second.to_state);
			if (it->second.onComplete){
				it->second.onComplete();
			}
			return;
		}
	}
}

void Creature::tick(){
	onEvent(Tick);
	sprite.tick();
	update();
}

void Creature::update(){

}

void Creature::addStateTransition(int from_state, int to_state, const Event& trigger_event, double relative_probability, const std::function<void()>& onComplete){
	transitions.insert(std::make_pair(Trigger(from_state, trigger_event), Transition(to_state, relative_probability, onComplete)));
}

void Creature::setAnimation(int state, const std::string& animation){
	animations[state] = animation;
}

void Creature::setState(int state){
	auto it = animations.find(state);
	if (it != animations.end()){
		sprite.play(it->second);
	}
	current_state = state;
}

int Creature::getState() const {
	return current_state;
}

void Creature::flip(){
	direction *= -1;
}

void Creature::render(sf::RenderWindow& rw, vec2 offset){
	sprite.setScale(vec2(getScale() * direction, getScale()));
	sprite.render(rw, offset + position);
}

Creature::Trigger::Trigger(int _from_state, const Event& _event){
	from_state = _from_state;
	event_id = _event.id;
}

Creature::Transition::Transition(int _to_state, double _relative_probability, const std::function<void()> &_onComplete){
	to_state = _to_state;
	relative_probability = _relative_probability;
	onComplete = _onComplete;
}
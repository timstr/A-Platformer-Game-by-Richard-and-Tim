#pragma once

#include "creature.h"

Creature::Creature(const std::string& name) : sprite(name) {
	type = nullptr;
	sprite.setOnComplete([this]{
		this->onEvent(AnimationEnd);
	});
}

void Creature::setType(const CreatureType& _type){
	type = &_type;
}

void Creature::onEvent(const Event& e){
	auto tr = transitions.find(Trigger(-1, e));
	if (tr != transitions.end()){
		setState(tr->second.to_state);
		if (tr->second.onComplete){
			tr->second.onComplete();
		}
		return;
	}

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

void Creature::addStateTransition(uint8_t from_state, uint8_t to_state, const Event& trigger_event, double relative_probability, const std::function<void()>& onComplete){
	transitions.insert(std::make_pair(Trigger(from_state, trigger_event), Transition(to_state, relative_probability, onComplete)));
}
void Creature::addStateTransition(uint8_t to_state, const Event& trigger_event, const std::function<void()> onComplete){
	transitions.insert(std::make_pair(Trigger(-1, trigger_event), Transition(to_state, 1.0, onComplete)));
}

void Creature::setStateAnimation(uint8_t state, const std::string& animation){
	animations[state] = animation;
}

void Creature::setState(uint8_t state){
	auto it = animations.find(state);
	if (it != animations.end()){
		sprite.play(it->second);
	}
	current_state = state;
}

int Creature::getState() const {
	return current_state;
}

void Creature::notice(std::weak_ptr<Creature> crtr){
	if (auto creature = crtr.lock()){
		const CreatureType* t = creature->type;
		while (t){
			auto it = type_handlers.find(t);
			if (it != type_handlers.end()){
				it->second(creature);
				return;
			}
			t = t->getBaseType();
		}
	}
}

void Creature::onNotice(const CreatureType& creaturetype, const std::function<void(std::weak_ptr<Creature>)>& handler){
	type_handlers[&creaturetype] = handler;
}

void Creature::flip(){
	sprite.flip();
	direction *= -1;
}

void Creature::setDirection(float dir){
	if (dir >= 0.0f && direction != 1){
		sprite.flip();
		direction = 1;
	} else if (dir < 0 && direction != -1){
		sprite.flip();
		direction = -1;
	}
}

float Creature::getDirection() const {
	return (float)direction;
}

void Creature::setAwarenessRadius(float rad){
	awareness_radius = abs(rad);
}
float Creature::getAwarenessRadius() const {
	return awareness_radius;
}

void Creature::draw(sf::RenderTarget& rt, sf::RenderStates states) const {
	states.transform *= getTransform();
	rt.draw(sprite, states);
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
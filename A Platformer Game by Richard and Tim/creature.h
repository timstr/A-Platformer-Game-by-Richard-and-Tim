#pragma once

#include <map>
#include "character.h"


namespace Event {
	struct Event{
		Event() : id(getNextId()) {

		}

		const int id;

		private:
		int getNextId(){
			static int next_id;
			next_id += 1;
			return next_id;
		}
	};

	bool operator<(const Event& l, const Event& r);

	/////////////////////////////



}

// Creature is an autonomous character that moves around
// independently and can interact with other characters
struct Creature : Character {

	// TODO: figure out an extensible and portable method for distinguishing different events

	// possibly change states from a given event
	virtual void onEvent(Event::Event e){
		auto begin = state_map.lower_bound(std::make_pair(current_state, e));
		auto end = state_map.upper_bound(std::make_pair(current_state, e));

		if (begin == end){
			return;
		}

		double total = 0.0;

		for (auto it = begin; it != end; it++){
			total += it->second.second;
		}

		double rand_val = rand() * total / (double)RAND_MAX;

		for (auto it = begin; it != end; it++){
			rand_val -= it->second.second;
			if (rand_val <= 0){
				current_state = it->second.first;
				return;
			}
		}
	}

	void addStateTransition(int from_state, int to_state, Event::Event trigger_event, double relative_probability = 1.0){
		state_map.insert(std::make_pair(std::make_pair(from_state, trigger_event), std::make_pair(to_state, relative_probability)));
	}

	void setState(int state){
		current_state = state;
	}

	int getState(){
		return current_state;
	}

	private:

	int current_state = -1;

	// The mapping of states is represented as:
	// (int from_state, EventType trigger_event) -> (int to_state, double relative_probability)
	std::multimap<std::pair<int, Event::Event>, std::pair<int, double>> state_map;
};
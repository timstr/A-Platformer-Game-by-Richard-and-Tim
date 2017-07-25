#pragma once

#include "event.h"

Event::Event() : id(getNextId()) {

}

int Event::next_id = 0;

int Event::getNextId(){
	next_id += 1;
	return next_id;
}

bool operator<(const Event& l, const Event& r){
	return l.id < r.id;
}

bool operator==(const Event& l, const Event& r){
	return l.id == r.id;
}

Event Tick; // every frame
Event Flying; // when airborne
Event Standing; // when grounded
Event Impact; // upon collision

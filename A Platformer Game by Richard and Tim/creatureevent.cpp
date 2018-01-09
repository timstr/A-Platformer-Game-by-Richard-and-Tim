#include "creatureeevent.h"

CreatureEvent::CreatureEvent() : id(getNextId()) {

}

int CreatureEvent::next_id = 0;

int CreatureEvent::getNextId(){
	next_id += 1;
	return next_id;
}

bool operator<(const CreatureEvent& l, const CreatureEvent& r){
	return l.id < r.id;
}
bool operator==(const CreatureEvent& l, const CreatureEvent& r){
	return l.id == r.id;
}

//////////////////////////

CreatureEvent Tick; // every frame
CreatureEvent Flying; // when airborne
CreatureEvent Standing; // when grounded
CreatureEvent Impact; // upon collision

#pragma once

struct CreatureEvent {
	CreatureEvent();

	const int id;

	private:

	static int next_id;

	int getNextId();
};

bool operator<(const CreatureEvent& l, const CreatureEvent& r);
bool operator==(const CreatureEvent& l, const CreatureEvent& r);

/////////////////////////////


extern CreatureEvent Tick; // every frame
extern CreatureEvent Flying; // when airborne
extern CreatureEvent Standing; // when grounded
extern CreatureEvent Impact; // upon collision

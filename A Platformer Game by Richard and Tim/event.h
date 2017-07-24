#pragma once

struct Event {
	Event();

	const int id;

	private:

	static int next_id;

	int getNextId();
};

bool operator<(const Event& l, const Event& r);

/////////////////////////////


extern Event Tick; // every frame
extern Event Flying; // when airborne
extern Event Standing; // when grounded
extern Event Impact; // upon collision
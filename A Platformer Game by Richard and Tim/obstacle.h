#pragma once

#include "obstruction.h"

// Obstacle is a component of a space with unique physical
// properties different from the map they may be found with
struct Obstacle : Obstruction {
	bool destructible;
	int healthpoints;
};

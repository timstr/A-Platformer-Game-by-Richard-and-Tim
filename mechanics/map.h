#pragma once

#include "obstruction.h"

// TODO: remove
// Map is the static physical boundary of a space
struct Map : Obstruction {
	Map() : Obstruction(false) {

	}
};
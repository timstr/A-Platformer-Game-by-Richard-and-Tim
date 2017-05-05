#pragma once
#include "obstruction.h"

bool Obstruction::hitTest(vec2 point) const {
	return false; //TODO
}

vec2 Obstruction::getCollisionForce(vec2 point, vec2 velocity, double mass) const {
	return {0, 0}; //TODO
}
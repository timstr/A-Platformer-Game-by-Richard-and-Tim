#pragma once

#include "renderable.h"

// Obstruction is the base class to all things that have
// a complicated raster-image physical boundary with 
// which entities are to interact physically
struct Obstruction : Renderable {
	// test whether a point collides with the obstruction
	bool hitTest(vec2 point) const;

	// get the force exerted on a body colliding at the given point
	// moving at the given velocity having the given mass
	vec2 getCollisionForce(vec2 point, vec2 velocity, double mass) const;

	protected:

	// the spatial position
	vec2 pos;

	// the visual appearance
	sf::Sprite image;

	// the physical boundary
	sf::Image boundary;

	// the coefficient of friction
	// 0 for totally slippy
	// 1 for totally grippy
	double friction;
};
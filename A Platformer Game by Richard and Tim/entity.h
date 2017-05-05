#pragma once

#include <vector>
#include "renderable.h"
#include "obstruction.h"

// Entity is the base class to all things that interact with such
// obstructions as the map and obstacles
// an Entity has a visual appearance, a rigid physical boundary
// comprised of one or more circles, and physical mass
struct Entity : Renderable {

	// possibly collide with obstruction and change
	// velocity accordingly
	void tryCollisionWith(const Obstruction& obstruction);

	// translate the entity according to its velocity
	void move();

	protected:

	vec2 position;
	vec2 velocity;
	double mass;

	struct Circle {
		Circle(vec2 _center = {0, 0}, double _radius = 20.0){
			center = _center;
			radius = _radius;
		}
		vec2 center;
		double radius;
	};

	std::vector<Circle> circles;

};
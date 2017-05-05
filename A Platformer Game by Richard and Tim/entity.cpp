#pragma once
#include "entity.h"
#include <math.h>

void Entity::tryCollisionWith(const Obstruction& obstruction){
	const double PI = 3.14159265358979323846264338327950288;
	const int divisions = 20;
	const double angle_delta = 2 * PI / divisions;
	vec2 total_force = {0, 0};

	for (const Circle& circle : circles){
		for (double angle = 0; angle < 2 * PI; angle += angle_delta){
			vec2 point = position + circle.center + vec2(cos(angle) * circle.radius, sin(angle) * circle.radius);
			if (obstruction.hitTest(point)){
				total_force += obstruction.getCollisionForce(point, velocity, mass);
			}
		}
	}

	//TODO: is this right?
	velocity += total_force / (float)(circles.size() * divisions);
}
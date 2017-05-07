#pragma once
#include "entity.h"
#include <math.h>

void Entity::tryCollisionWith(Obstruction* obstruction){
	vec2 total_force = {0, 0};
	int hit_points = 0;

	vec2 center = {0, 0};
	for (const Circle& circle : circles){
		center += circle.center;
	}
	center /= (float)circles.size();

	for (const Circle& circle : circles){
		double angle_delta = 2 * pi / circle.radius;
		for (double angle = 0; angle < 2 * pi; angle += angle_delta){
			vec2 point = position + circle.center + vec2(cos(angle) * circle.radius, sin(angle) * circle.radius);

			if (obstruction->hitTest(point)){
				hit_points += 1;
				total_force += obstruction->getCollisionForce(point, position + center, velocity, mass);
			}
		}
	}

	if (std::isnan(total_force.x + total_force.y)){
		total_force = {0, 0};
	}
	
	if (hit_points > 0){
		//TODO: is this right?
		velocity += total_force / (float)(circles.size() * hit_points * mass);
	}
}

bool Entity::collidesWith(Obstruction* obstruction){
	for (const Circle& circle : circles){
		double angle_delta = 2 * pi / circle.radius;
		for (double angle = 0; angle < 2 * pi; angle += angle_delta){
			vec2 point = position + circle.center + vec2(cos(angle) * circle.radius, sin(angle) * circle.radius);

			if (obstruction->hitTest(point)){
				return true;
			}
		}
	}
	return false;
}

void Entity::move(){
	const double min_velocity = 0.1;
	if (abs(velocity.x) < min_velocity && abs(velocity.y) < min_velocity){
		velocity = {0, 0};
	}
	position += velocity;
}

void Entity::addCircle(const Entity::Circle& circle){
	circles.push_back(circle);
}
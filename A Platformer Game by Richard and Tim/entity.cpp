#pragma once
#include "entity.h"
#include <math.h>

namespace {
	const double precision = 1.0;
}

Entity::Entity(){
	position = {0, 0};
	velocity = {0, 0};
	mass = 1.0;
	friction = 0.5;
	elasticity = 0.5;
}

bool Entity::collidesWith(const Obstruction* obstruction) const {
	for (const Circle& circle : circles){
		double slices = circle.radius * precision;
		double angle_delta = 2 * pi / slices;

		vec2 radvec = vec2(circle.radius, 0);
		mat2x2 mat = rotationMatrix(angle_delta);

		for (double slice = 0; slice < slices; slice += 1, radvec = mat * radvec){
			vec2 point = position + circle.center + radvec;

			if (obstruction->hitTest(point)){
				return true;
			}
		}
	}

	return false;
}

bool Entity::collidesWith(const std::vector<Obstruction*>& obstructions) const {
	for (Obstruction* const obstruction : obstructions){
		if (collidesWith(obstruction)){
			return true;
		}
	}
	return false;
}

void Entity::tick(){

}

void Entity::onEvent(Event e){

}

void Entity::move(const std::vector<Obstruction*>& obstructions){

	// This is where gravity happens
	velocity.y += 0.25f;

	for (Obstruction* const obstruction : obstructions){
		performCollision(obstruction);
	}

	position += velocity;
}

vec2 Entity::getContactAcceleration(const Obstruction* obstruction, vec2 normal) const {
	return {0, 0};
}

void Entity::performCollision(Obstruction* obstruction){
	vec2 center = {0, 0};
	for (const Circle& circle : circles){
		center += circle.center;
	}
	center /= (float)circles.size();

	vec2 delta_velocity = {0, 0};
	vec2 sum_normals = {0, 0};

	vec2 total_contact_accel = {0, 0};

	if (collidesWith(obstruction)){
		vec2 total_force = {0, 0};
		int hit_points = 0;

		for (const Circle& circle : circles){
			double slices = circle.radius * precision;
			double angle_delta = 2 * pi / slices;

			vec2 radvec = vec2(circle.radius, 0);
			mat2x2 mat = rotationMatrix(angle_delta);

			for (double slice = 0; slice < slices; slice += 1, radvec = mat * radvec){
				vec2 point = position + circle.center + radvec;

				if (obstruction->hitTest(point)){
					vec2 normal = obstruction->getNormalAt(point, position + center - point - velocity);
					total_force += obstruction->getImpulse(point, normal, this);
					sum_normals += normal;
					hit_points += 1;

					double depth = obstruction->getDistanceToBoundary(point, normal);
					position += (float)(std::max(0.0, depth * 1.0 - 1.0)) * normal;
				}
			}
		}

		if (hit_points > 0){

			vec2 delta_velocity = total_force / (float)(hit_points * mass);

			delta_velocity += total_contact_accel / (float)(hit_points);

			velocity += delta_velocity;
		}
	}
}

void Entity::addCircle(const Entity::Circle& circle){
	circles.push_back(circle);
}

Entity::Circle::Circle(vec2 _center, double _radius){
	center = _center;
	radius = _radius;
}
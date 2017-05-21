#pragma once
#include "entity.h"
#include <math.h>

namespace {
	const double precision = 0.25;
}

bool Entity::collidesWith(Obstruction* const obstruction) const {
	for (const Circle& circle : circles){
		double angle_delta = 2 * pi / circle.radius / precision;
		for (double angle = 0; angle < 2 * pi; angle += angle_delta){
			vec2 point = position + circle.center + vec2(cos(angle) * circle.radius, sin(angle) * circle.radius);

			if (obstruction->hitTest(point)){
				return true;
			}
		}
	}

	return false;
}

bool Entity::collidesWith(std::vector<Obstruction*>& const obstructions) const {
	for (Obstruction* const obstruction : obstructions){
		if (collidesWith(obstruction)){
			return true;
		}
	}
	return false;
}

void Entity::move(std::vector<Obstruction*>& const obstructions){

	velocity.y += 0.1f;

	if (collidesWith(obstructions)){
		for (Obstruction* const obstruction : obstructions){
			performCollision(obstruction);
		}
	}

	position += velocity;
}

// TODO: fix entities sliding through small crevices
void Entity::performCollision(Obstruction* obstruction){
	vec2 center = {0, 0};
	for (const Circle& circle : circles){
		center += circle.center;
	}
	center /= (float)circles.size();

	vec2 delta_velocity = {0, 0};
	vec2 sum_normals = {0, 0};

	if (collidesWith(obstruction)){
		vec2 total_force = {0, 0};
		int hit_points = 0;

		for (const Circle& circle : circles){
			double angle_delta = 2 * pi / circle.radius / precision;
			for (double angle = 0; angle < 2 * pi; angle += angle_delta){
				vec2 point = position + circle.center + vec2(cos(angle) * circle.radius, sin(angle) * circle.radius);

				if (obstruction->hitTest(point)){
					vec2 normal = obstruction->getNormalAt(point, position + center - point - velocity);
					sum_normals += normal;
					total_force += obstruction->getImpulse(point, normal, this);
					hit_points += 1;
				}
			}
		}

		if (hit_points > 0){

			vec2 delta_velocity = total_force / (float)(hit_points * mass);

			velocity += delta_velocity;
		}
	}
}

void Entity::addCircle(const Entity::Circle& circle){
	circles.push_back(circle);
}
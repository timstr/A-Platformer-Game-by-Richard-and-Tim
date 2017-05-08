#pragma once
#include "entity.h"
#include <math.h>

namespace {
	const double precision = 0.25;
}
void Entity::tryCollisionWith(Obstruction* obstruction){
	vec2 center = {0, 0};
	for (const Circle& circle : circles){
		center += circle.center;
	}
	center /= (float)circles.size();

	nanCheck(center);

	vec2 delta_velocity = {0, 0};
	vec2 sum_normals = {0, 0};

	if (collidesWith(obstruction)){
		vec2 total_force = {0, 0};
		int hit_points = 0;

		
		for (const Circle& circle : circles){
			double angle_delta = 2 * pi / std::max(circle.radius * precision, 4.0);
			for (double angle = 0; angle < 2 * pi; angle += angle_delta){
				vec2 point = position + circle.center + vec2(cos(angle) * circle.radius, sin(angle) * circle.radius);

				if (obstruction->hitTest(point)){
					vec2 normal = obstruction->getNormalAt(point, position + center - point - velocity);
					nanCheck(normal);
					sum_normals += normal;
					total_force += obstruction->getCollisionForce(point, normal, this);
					hit_points += 1;
				}
			}
		}

		if (hit_points > 0){
			nanCheck(total_force);

			vec2 velocity_normal = projectOnto(velocity, sum_normals);
			vec2 velocity_tangent = velocity - velocity_normal;
			nanCheck(velocity_normal);
			nanCheck(velocity_tangent);

			delta_velocity = (velocity_tangent * (float)(1 - friction * obstruction->friction)) + ((float)elasticity * velocity_normal) - velocity;

			nanCheck(delta_velocity);

			delta_velocity += total_force / (float)(hit_points * mass);

			nanCheck(delta_velocity);

			velocity += delta_velocity;
		}
	}

	// TODO: zero velocity when appropriate

	nanCheck(velocity);
}

bool Entity::collidesWith(Obstruction* obstruction){
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

bool Entity::almostCollidesWith(Obstruction* obstruction){
	for (const Circle& circle : circles){
		double rad = circle.radius + 2.0;
		double angle_delta = 2 * pi / rad / precision;
		for (double angle = 0; angle < 2 * pi; angle += angle_delta){
			vec2 point = position + circle.center + vec2(cos(angle) * rad, sin(angle) * rad);

			if (obstruction->hitTest(point)){
				return true;
			}
		}
	}

	return false;
}

void Entity::move(){
	const double min_velocity = 0.1f;
	const float max_velocity = 30.0f;

	double mag = abs(velocity);
	
	//if (mag < min_velocity){
	//	velocity = {0, 0};
	//}
	
	if (mag > max_velocity){
		velocity /= (float)mag;
	}

	position += velocity;
}

void Entity::addCircle(const Entity::Circle& circle){
	circles.push_back(circle);
}
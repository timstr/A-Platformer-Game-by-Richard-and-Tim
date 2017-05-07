#pragma once
#include "entity.h"
#include <math.h>

void Entity::tryCollisionWith(Obstruction* obstruction){

	// TODO: instead of testing for collisions along every point in here,
	// just calculate collision force over all points regardlessly and sum
	// Modify getCollisionForce to return zero vector if no collision

	vec2 center = {0, 0};
	for (const Circle& circle : circles){
		center += circle.center;
	}
	center /= (float)circles.size();

	vec2 hit_point;

	if (collidesWith(obstruction, &hit_point)){
		if (abs(velocity) > 50){
			backUpFrom(obstruction, -velocity);
		}
		
		vec2 total_force = {0, 0};
		int hit_points = 0;

		
		for (const Circle& circle : circles){
			double angle_delta = 2 * pi / std::max(circle.radius, 4.0);
			for (double angle = 0; angle < 2 * pi; angle += angle_delta){
				vec2 point = position + circle.center + vec2(cos(angle) * circle.radius, sin(angle) * circle.radius);

				if (obstruction->hitTest(point)){
					hit_points += 1;
					total_force += obstruction->getCollisionForce(point, position + center, velocity, mass);
				}
			}
		}
		
		if (hit_points > 0){
			//TODO: is this right?
			velocity += total_force / (float)(circles.size() * hit_points * mass);
		}
	}
}

bool Entity::collidesWith(Obstruction* obstruction, vec2* hit_point){
	double sumx = 0;
	double sumy = 0;
	int count = 0;

	for (const Circle& circle : circles){
		double angle_delta = 2 * pi / circle.radius;
		for (double angle = 0; angle < 2 * pi; angle += angle_delta){
			vec2 point = position + circle.center + vec2(cos(angle) * circle.radius, sin(angle) * circle.radius);

			if (obstruction->hitTest(point)){
				sumx += point.x;
				sumy += point.y;
				count += 1;
			}
		}
	}

	if (count == 0){
		return false;
	}

	if (hit_point){
		hit_point->x = (sumx / (double)count) - position.x;
		hit_point->y = (sumy / (double)count) - position.y;
	}
	return true;
}

void Entity::backUpFrom(Obstruction* obstruction, vec2 direction){
	const int max_steps = 15;

	// take a step back...
	vec2 path = -direction;
	position -= path;

	if (collidesWith(obstruction)){
		return;
	}

	// and take a couple baby steps there again
	for (int i = 0; i < max_steps; i++){
		path *= 0.5f;
		// the entity has not collided yet
		// move forward
		position += path;
		// if it collides:
		if (collidesWith(obstruction) && i != max_steps - 1){
			// move back
			position -= path;
		}
	}

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
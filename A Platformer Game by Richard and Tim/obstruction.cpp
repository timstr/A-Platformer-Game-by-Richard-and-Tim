#pragma once
#include "obstruction.h"

bool Obstruction::hitTest(vec2 point) const {
	int x = floor(point.x - this->position.x);
	int y = floor(point.y - this->position.y);

	// test whether point is outside the map
	if ((x < 0) || (x >= boundary.getSize().x) || (y < 0) || (y >= boundary.getSize().y)){
		return !open_boundary;
	}

	sf::Color pixel = boundary.getPixel(x, y);

	// the boundary image is solid where its colour isn't white
	return (pixel.r + pixel.g + pixel.b) <= 255 * 2.5;
}

vec2 Obstruction::getImpulse(vec2 point, vec2 normal, Entity* entity) const {

	float velocity_normal = std::max(0.0, -dot(entity->velocity, normal));

	float impulse_normal = entity->mass * velocity_normal * (1 + entity->elasticity);

	vec2 tangent = vec2(normal.y, -normal.x);

	float velocity_tangent = dot(entity->velocity, tangent);

	float impulse_tangent = -velocity_tangent * entity->mass * entity->friction * friction;

	vec2 impulse_contact = ((getContactAcceleration(entity) * (float)entity->friction) + (entity->getContactAcceleration(this) * (float)friction)) * (float)entity->mass;

	return impulse_normal * normal + impulse_tangent * tangent + impulse_contact;
}

void Obstruction::setPos(vec2 _pos){
	position = _pos;
}

void Obstruction::setImage(const sf::Sprite& _image){
	image = _image;
}

void Obstruction::setBoundary(const sf::Image& _boundary){
	boundary = _boundary;
}

void Obstruction::render(sf::RenderWindow& rw, vec2 offset){
	image.setPosition(offset + position);
	rw.draw(image);
}

vec2 Obstruction::getNormalAt(vec2 point, vec2 hint) const {

	const double probe_radius = 10.0;
	const double angle_delta = 2 * pi / probe_radius * 0.5;

	double hint_angle = atan2(hint.y, hint.x);

	if (std::isnan(hint_angle)){
		hint_angle = 0;
	}

	double min_angle = hint_angle;
	double max_angle = hint_angle;

	bool max_last = false;

	// test if the first point is inside or outside the boundary
	bool inside = hitTest(point + vec2(probe_radius * cos(hint_angle), probe_radius * sin(hint_angle)));
	bool first_hit = inside;

	for (double a = angle_delta; a <= 2 * pi; a += angle_delta){
		double angle = a + hint_angle;

		bool hit = hitTest(point + vec2(probe_radius * cos(angle), probe_radius * sin(angle)));
		
		// if the boundary is crossed during this probe:
		if (inside != hit){
			if (inside){
				// if the probe left the boundary
				max_angle = angle;
				max_last = true;
			} else {
				// if the probe entered the boundary
				min_angle = angle - angle_delta;
				max_last = false;
			}
			inside = hit;
		}
	}

	if (min_angle == max_angle){
		return hint / (float)hypot(hint.x, hint.y);
	}

	double normal = (min_angle + max_angle) * 0.5;

	if (std::isnan(normal)){
		normal = 0;
	}

	return vec2(cos(normal), sin(normal)) * (max_last ? -1.0f : 1.0f);
}

double Obstruction::getDistanceToBoundary(vec2 point, vec2 direction) const {
	double length = abs(direction);
	if (length == 0.0){
		return 0.0;
	}

	direction /= (float)length;

	const float max_distance = 5.0;
	const int max_steps = 10;

	double distance = 0;

	vec2 path = direction * max_distance;
	double path_length = max_distance;

	for (int i = 0; i < max_steps; i++){
		if (hitTest(point + path)){
			point += path;
			distance += path_length;
		}
		path *= 0.5f;
		path_length *= 0.5;
	}

	return distance;

	/*double mag = abs(direction);
	if (mag == 0){
		return 0;
	}
	direction /= (float)mag;
	double dist = 0;

	double max_steps = 50;

	for (int i = 0; i < max_steps; i++){
		point += direction;
		dist += 1;

		if (!hitTest(point)){
			return dist;
		}
	}
	return 0;*/
}
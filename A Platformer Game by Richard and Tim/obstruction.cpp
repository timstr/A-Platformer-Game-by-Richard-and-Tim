#pragma once
#include "obstruction.h"

bool Obstruction::hitTest(vec2 point) const {
	int x = round(point.x + this->pos.x);
	int y = round(point.y + this->pos.y);

	// outside the map shall be a solid boundary
	if ((x < 0) || (x >= boundary.getSize().x) || (y < 0) || (y >= boundary.getSize().y)){
		return true;
	}


	sf::Color pixel = boundary.getPixel(x, y);

	// the boundary image is solid where its colour is dark
	return pixel.r + pixel.g + pixel.b <= 1.5;
}

vec2 Obstruction::getCollisionForce(vec2 point, vec2 normal, Entity* entity) const {

	nanCheck(point);
	nanCheck(normal);

	vec2 velocity_normal = projectOnto(entity->velocity, normal);

	double depth = getDistanceToBoundary(point, normal);

	float force = entity->mass * depth * 0.25 * (1 + entity->elasticity);

	return force * normal;
}

void Obstruction::setPos(vec2 _pos){
	pos = _pos;
}

void Obstruction::setImage(const sf::Sprite& _image){
	image = _image;
}

void Obstruction::setBoundary(const sf::Image& _boundary){
	boundary = _boundary;
}

void Obstruction::render(sf::RenderWindow& rw, vec2 offset){
	image.setPosition(offset + pos);
	rw.draw(image);
}

vec2 Obstruction::getNormalAt(vec2 point, vec2 hint) const {
	nanCheck(point);
	nanCheck(hint);

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
	double mag = abs(direction);
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
	return 0;
}
#pragma once
#include "obstruction.h"

bool Obstruction::hitTest(vec2 point) const {
	int x = floor(point.x - this->position.x);
	int y = floor(point.y - this->position.y);

	// test whether point is outside the map
	if ((x < 0) || (x >= boundary->getSize().x) || (y < 0) || (y >= boundary->getSize().y)){
		return !open_boundary;
	}

	sf::Color pixel = boundary->getPixel(x, y);

	if (pixel.a < 255){
		return false;
	}

	// the boundary image is solid where its colour isn't solid white
	return (pixel.r + pixel.g + pixel.b) <= 255 * 2.5;
}

vec2 Obstruction::getImpulse(vec2 point, vec2 normal, Entity* entity) const {

	vec2 velocity = entity->velocity - position + previous_position;

	float velocity_normal = std::max(0.0, -dot(velocity, normal));

	float impulse_normal = entity->mass * velocity_normal * (1 + entity->elasticity);

	vec2 tangent = vec2(normal.y, -normal.x);

	float velocity_tangent = dot(velocity, tangent);

	float impulse_tangent = -velocity_tangent * entity->mass * entity->friction * friction;

	// TODO: use normal and tangent components of contact acceleration to make it dependent on friction and elasticity

	vec2 ca_obs = getContactAcceleration(entity, normal);
	vec2 ca_obs_normal = projectOnto(ca_obs, normal);
	vec2 ca_obs_tangent = ca_obs - ca_obs_normal;
	//ca_obs_normal = normal * (float)std::max(0.0, dot(ca_obs, normal));
	ca_obs_tangent *= (float)entity->friction;

	vec2 ca_ent = entity->getContactAcceleration(this, normal) * entity->getScale();
	vec2 ca_ent_normal = projectOnto(ca_ent, normal);
	vec2 ca_ent_tangent = ca_ent - ca_ent_normal;
	//ca_ent_normal = normal * (float)std::max(0.0, dot(ca_ent, normal));
	ca_ent_tangent *= (float)friction;


	vec2 contact_accel_impulse = (ca_obs_normal + ca_obs_tangent + ca_ent_normal + ca_ent_tangent) * (float)entity->mass;

	return impulse_normal * normal + impulse_tangent * tangent + contact_accel_impulse;
}

void Obstruction::setPos(vec2 _pos){
	position = _pos;
}

void Obstruction::setSprite(const sf::Sprite& _sprite){
	sprite = _sprite;
}

void Obstruction::setBoundary(const sf::Image& _boundary){
	boundary = &_boundary;
}

void Obstruction::render(sf::RenderWindow& rw, vec2 offset){
	sprite.setPosition(offset + position);
	rw.draw(sprite);
}

void Obstruction::update(){
	previous_position = position;
	tick();
}

void Obstruction::tick() {

}

vec2 Obstruction::getNormalAt(vec2 point, vec2 hint) const {

	const double probe_radius = 10.0;
	const double slices = probe_radius * 2;
	const double angle_delta = 2 * pi / slices;

	double hint_angle = atan2(hint.y, hint.x);

	if (std::isnan(hint_angle)){
		hint_angle = 0;
	}

	double min_angle = hint_angle;
	double max_angle = hint_angle;

	bool max_last = false;

	// test if the first point is inside or outside the boundary
	vec2 rayvec = vec2(probe_radius * cos(hint_angle), probe_radius * sin(hint_angle));
	mat2x2 mat = rotationMatrix(angle_delta);

	bool inside = hitTest(point + rayvec);
	bool first_hit = inside;

	for (double angle = angle_delta + hint_angle; angle <= 2 * pi + hint_angle; angle += angle_delta){

		rayvec = mat * rayvec;

		bool hit = hitTest(point + rayvec);
		
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
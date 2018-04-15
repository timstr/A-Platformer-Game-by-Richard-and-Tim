#pragma once

struct Obstruction;

#include "vec2.h"
#include "obstruction.h"
#include "SFML\Graphics.hpp"
#include "event.h"
#include "destructible.h"
#include <vector>
#include <memory>

// Entity is the base class to all things that interact with such
// obstructions as the map and obstacles
// an Entity has a visual appearance, a rigid physical boundary
// comprised of one or more circles, and physical mass

// TODO: why inherit from Destructible?
struct Entity : sf::Drawable, sf::Transformable, Destructible {
	Entity();

	// possibly collide with obstruction and change
	// velocity accordingly
	void collide(const Obstruction& obstruction);

	void move();

	// get the acceleration exerted on the entity when it contacts the given obstruction
	// normal is assumed to be a unit vector
	virtual vec2 getContactAcceleration(const Obstruction& obstruction, vec2 normal) const;

	virtual void draw(sf::RenderTarget& rt, sf::RenderStates states) const override = 0;

	vec2 velocity;
	float mass;
	float friction;
	float elasticity;

	// tick is called once every frame and should be overridden to update the entity's state
	virtual void tick();

	virtual void onEvent(const Event& e);

	bool standing() const;

	protected:

	struct Circle {
		Circle(vec2 _center = {0, 0}, float _radius = 20.0);

		vec2 center;
		float radius;
	};

	void addCircle(const Circle& circle);

	std::vector<Circle> circles;

	private:

	bool is_standing = false;
	int flying_timer = 0;

	// returns true if the entity collides with the given obstruction
	bool collidesWith(const Obstruction& obstruction) const;

};
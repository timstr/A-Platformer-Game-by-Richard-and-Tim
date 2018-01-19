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
struct Entity : sf::Drawable, sf::Transformable, Destructible {
	Entity();

	// translate the entity according to its velocity and possibly collide with obstructions
	void moveAndCollide(const std::vector<std::shared_ptr<Obstruction>>& obstructions);

	// get the acceleration exerted on the entity when it contacts the given obstruction
	// normal is assumed to be a unit vector
	virtual vec2 getContactAcceleration(const Obstruction* obstruction, vec2 normal) const;

	virtual void draw(sf::RenderTarget& rt, sf::RenderStates states) const override = 0;

	vec2 velocity;
	double mass;
	double friction;
	double elasticity;

	// tick is called once every frame and should be overridden to update the entity's state
	virtual void tick();

	virtual void onEvent(const Event& e);

	bool standing() const;

	protected:

	struct Circle {
		Circle(vec2 _center = {0, 0}, double _radius = 20.0);

		vec2 center;
		double radius;
	};

	void addCircle(const Circle& circle);

	std::vector<Circle> circles;

	private:

	bool is_standing = false;
	int flying_timer = 0;

	// returns true if the entity collides with the given obstruction
	bool collidesWith(const std::shared_ptr<Obstruction>& obstruction) const;

	// possibly collide with obstruction and change
	// velocity accordingly
	void performCollision(const std::shared_ptr<Obstruction>& obstruction);

};
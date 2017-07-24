#pragma once

struct Obstruction;

#include "obstruction.h"
#include "renderable.h"
#include "event.h"
#include <vector>

// Entity is the base class to all things that interact with such
// obstructions as the map and obstacles
// an Entity has a visual appearance, a rigid physical boundary
// comprised of one or more circles, and physical mass
struct Entity : Renderable {
	Entity();

	// translate the entity according to its velocity and possibly collide with obstructions
	void move(const std::vector<Obstruction*>& obstructions);

	// get the acceleration exerted on the entity when it contacts the given obstruction
	// normal is assumed to be a unit vector
	virtual vec2 getContactAcceleration(const Obstruction* obstruction, vec2 normal) const;

	vec2 position;
	vec2 velocity;
	double mass;
	double friction;
	double elasticity;

	// tick is called once every frame and should be overridden to update the entity's state
	virtual void tick();

	virtual void onEvent(Event e);

	protected:

	struct Circle {
		Circle(vec2 _center = {0, 0}, double _radius = 20.0);

		vec2 center;
		double radius;
	};

	void addCircle(const Circle& circle);

	std::vector<Circle> circles;

	private:

	// returns true if the entity collides with the given obstruction(s)
	bool collidesWith(const Obstruction* obstruction) const;
	bool collidesWith(const std::vector<Obstruction*>& obstructions) const;

	// possibly collide with obstruction and change
	// velocity accordingly
	void performCollision(Obstruction* obstruction);

};
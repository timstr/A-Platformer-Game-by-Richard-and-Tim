#pragma once

struct Obstruction;

#include "obstruction.h"
#include <vector>
#include "renderable.h"

// Entity is the base class to all things that interact with such
// obstructions as the map and obstacles
// an Entity has a visual appearance, a rigid physical boundary
// comprised of one or more circles, and physical mass
struct Entity : Renderable {
	Entity(){
		position = {0, 0};
		velocity = {0, 0};
		mass = 1.0;
		friction = 0.5;
		elasticity = 0.5;
	}

	// translate the entity according to its velocity and possibly collide with obstructions
	void move(std::vector<Obstruction*>& const obstructions);

	virtual vec2 getContactAcceleration(const Obstruction* obstruction) const {
		return {0, 0};
	}

	vec2 position;
	vec2 velocity;
	double mass;
	double friction;
	double elasticity;

	// tick is called once every frame and should be overridden to update the entity's state
	virtual void tick(){

	}

	protected:

	struct Circle {
		Circle(vec2 _center = {0, 0}, double _radius = 20.0){
			center = _center;
			radius = _radius;
		}
		vec2 center;
		double radius;
	};

	void addCircle(const Circle& circle);

	std::vector<Circle> circles;

	private:

	// returns true if the entity collides with the given obstruction(s)
	bool collidesWith(Obstruction* const obstruction) const;
	bool collidesWith(std::vector<Obstruction*>& const obstructions) const;

	// possibly collide with obstruction and change
	// velocity accordingly
	void performCollision(Obstruction* obstruction);

};
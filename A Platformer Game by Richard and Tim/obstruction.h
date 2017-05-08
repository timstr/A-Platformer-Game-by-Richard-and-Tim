#pragma once

struct Entity;

#include "entity.h"
#include "renderable.h"
#include "vec2.h"

// Obstruction is the base class to all things that have
// a complicated raster-image physical boundary with 
// which entities are to interact physically
struct Obstruction : Renderable {
	Obstruction(){
		pos = {0, 0};
		friction = 0.5;
	}

	// test whether a point collides with the obstruction
	bool hitTest(vec2 point) const;

	// get the force exerted on a body colliding at the given point
	// moving at the given velocity having the given mass
	vec2 getCollisionForce(vec2 point, vec2 normal, Entity* entity) const;

	void setPos(vec2 _pos);

	void setImage(const sf::Sprite& _image);

	void setBoundary(const sf::Image& _boundary);

	void render(sf::RenderWindow& rw, vec2 offset) override;

	//private:

	// the spatial position
	vec2 pos;

	// the visual appearance
	sf::Sprite image;

	// the physical boundary
	sf::Image boundary;

	// the coefficient of friction
	// 0 for totally slippy
	// 1 for totally grippy
	double friction;

	// returns the normal at a point on a boundary, or the hint direction if
	// the point is not near a boundary. Always returns a unit vector
	vec2 getNormalAt(vec2 point, vec2 hint) const;

	// returns the distance along the given direction to the nearest point in
	// open space, or 0 if the given point is already in open space
	double getDistanceToBoundary(vec2 point, vec2 direction) const;
};
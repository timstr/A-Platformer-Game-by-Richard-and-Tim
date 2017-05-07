#pragma once

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
	vec2 getCollisionForce(vec2 point, vec2 center, vec2 velocity, double mass) const;

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

	vec2 getNormalAt(vec2 point, vec2 hint) const;
};
#pragma once

#include "RigidBody.hpp"
#include "CircleBody.hpp"
#include "RectangleBody.hpp"
#include "ConvexBody.hpp"
#include "RasterBody.hpp"

namespace phys {

	struct Collision {
		Collision(RigidBody& _a, RigidBody& _b, vec2 _normal, vec2 _radius_a, vec2 _radius_b);

		RigidBody& a;
		RigidBody& b;
		const vec2 normal;
		const vec2 radius_a; // vector from a's center of mass to point of collision (closest point to b), in a's local coordinates
		const vec2 radius_b; // vector from b's center of mass to point of collision (closest point to a), in b's local coordinates
	};

	// TODO: refactor these functions so they can optionally return no collision

	Collision collideCircleCircle(RigidBody& a, RigidBody& b);
	
	Collision collideCircleRectangle(RigidBody& a, RigidBody& b);
	
	Collision collideCircleConvex(RigidBody& a, RigidBody& b);
	
	Collision collideCircleRaster(RigidBody& a, RigidBody& b);



	Collision collideRectangleCircle(RigidBody& a, RigidBody& b);
	
	Collision collideRectangleRectangle(RigidBody& a, RigidBody& b);
	
	Collision collideRectangleConvex(RigidBody& a, RigidBody& b);
	
	Collision collideRectangleRaster(RigidBody& a, RigidBody& b);


	
	Collision collideConvexCircle(RigidBody& a, RigidBody& b);
	
	Collision collideConvexRectangle(RigidBody& a, RigidBody& b);
	
	Collision collideConvexConvex(RigidBody& a, RigidBody& b);
	
	Collision collideConvexRaster(RigidBody& a, RigidBody& b);


	
	Collision collideRasterCircle(RigidBody& a, RigidBody& b);
	
	Collision collideRasterRectangle(RigidBody& a, RigidBody& b);
	
	Collision collideRasterConvex(RigidBody& a, RigidBody& b);
	
	Collision collideRasterRaster(RigidBody& a, RigidBody& b);

} // namespace phys
#pragma once

#include "RigidBody.hpp"
#include "CircleBody.hpp"
#include "RectangleBody.hpp"
#include "ConvexBody.hpp"
#include "RasterBody.hpp"

#include <optional>

namespace phys {

	struct Collision {
		Collision(RigidBody& _a, RigidBody& _b, vec2 _normal, vec2 _radius_a, vec2 _radius_b);

		RigidBody& a;
		RigidBody& b;
		const vec2 normal; // normal at point of collision, pointing from a to b
		const vec2 radius_a; // vector from a's center of mass to point of collision (closest point to b), in world coordinates
		const vec2 radius_b; // vector from b's center of mass to point of collision (closest point to a), in world coordinates
	};

	// TODO: refactor these functions so they can optionally return no collision
	using MaybeCollision = std::optional<Collision>;

	MaybeCollision collideCircleCircle(RigidBody& a, RigidBody& b);
	
	MaybeCollision collideCircleRectangle(RigidBody& a, RigidBody& b);
	
	MaybeCollision collideCircleConvex(RigidBody& a, RigidBody& b);
	
	MaybeCollision collideCircleRaster(RigidBody& a, RigidBody& b);



	MaybeCollision collideRectangleCircle(RigidBody& a, RigidBody& b);
	
	MaybeCollision collideRectangleRectangle(RigidBody& a, RigidBody& b);
	
	MaybeCollision collideRectangleConvex(RigidBody& a, RigidBody& b);
	
	MaybeCollision collideRectangleRaster(RigidBody& a, RigidBody& b);


	
	MaybeCollision collideConvexCircle(RigidBody& a, RigidBody& b);
	
	MaybeCollision collideConvexRectangle(RigidBody& a, RigidBody& b);
	
	MaybeCollision collideConvexConvex(RigidBody& a, RigidBody& b);
	
	MaybeCollision collideConvexRaster(RigidBody& a, RigidBody& b);


	
	MaybeCollision collideRasterCircle(RigidBody& a, RigidBody& b);
	
	MaybeCollision collideRasterRectangle(RigidBody& a, RigidBody& b);
	
	MaybeCollision collideRasterConvex(RigidBody& a, RigidBody& b);
	
	MaybeCollision collideRasterRaster(RigidBody& a, RigidBody& b);

} // namespace phys
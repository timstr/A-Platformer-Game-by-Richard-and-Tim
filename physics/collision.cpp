#include "Collision.hpp"

namespace phys {

	Collision::Collision(RigidBody& _a, RigidBody& _b, vec2 _normal, vec2 _radius_a, vec2 _radius_b) :
		a(_a), b(_b), normal(_normal), radius_a(_radius_a), radius_b(_radius_b) {

	}


	MaybeCollision collideCircleCircle(RigidBody& a, RigidBody& b) {
		CircleBody& ca = static_cast<CircleBody&>(a);
		CircleBody& cb = static_cast<CircleBody&>(b);

		vec2 diff = cb.getPosition() - ca.getPosition();
		float dist = abs(diff);

		float penetration = dist - ca.radius() - cb.radius();

		vec2 normal = diff / dist;

		vec2 r_a = normal * ca.radius();
		vec2 r_b = -normal * cb.radius();

		return Collision(ca, cb, normal, r_a, r_b);
	}

	MaybeCollision collideCircleRectangle(RigidBody& a, RigidBody& b){
		// TODO
		throw std::runtime_error("Not implemented");
	}

	MaybeCollision collideCircleConvex(RigidBody& a, RigidBody& b){
		// TODO
		throw std::runtime_error("Not implemented");
	}

	MaybeCollision collideCircleRaster(RigidBody& a, RigidBody& b){
		// TODO
		throw std::runtime_error("Not implemented");
	}

	MaybeCollision collideRectangleCircle(RigidBody& a, RigidBody& b){
		return collideCircleRectangle(b, a);
	}

	MaybeCollision collideRectangleRectangle(RigidBody& a, RigidBody& b){
		// TODO
		throw std::runtime_error("Not implemented");
	}

	MaybeCollision collideRectangleConvex(RigidBody& a, RigidBody& b){
		// TODO
		throw std::runtime_error("Not implemented");
	}

	MaybeCollision collideRectangleRaster(RigidBody& a, RigidBody& b){
		// TODO
		throw std::runtime_error("Not implemented");
	}

	MaybeCollision collideConvexCircle(RigidBody& a, RigidBody& b){
		return collideCircleConvex(b, a);
	}

	MaybeCollision collideConvexRectangle(RigidBody& a, RigidBody& b){
		return collideRectangleConvex(b, a);
	}

	MaybeCollision collideConvexConvex(RigidBody& a, RigidBody& b){
		// TODO
		throw std::runtime_error("Not implemented");
	}

	MaybeCollision collideConvexRaster(RigidBody& a, RigidBody& b){
		// TODO
		throw std::runtime_error("Not implemented");
	}

	MaybeCollision collideRasterCircle(RigidBody& a, RigidBody& b){
		return collideCircleRaster(b, a);
	}

	MaybeCollision collideRasterRectangle(RigidBody& a, RigidBody& b){
		return collideRectangleRaster(b, a);
	}

	MaybeCollision collideRasterConvex(RigidBody& a, RigidBody& b){
		return collideConvexRaster(b, a);
	}

	MaybeCollision collideRasterRaster(RigidBody& a, RigidBody& b){
		// TODO
		throw std::runtime_error("Not implemented");
	}

} // namespace phys

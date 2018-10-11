#include "Collision.hpp"

namespace phys {

	Collision::Collision(RigidBody& _a, RigidBody& _b, vec2 _normal, vec2 _radius_a, vec2 _radius_b) :
		a(_a), b(_b), normal(_normal), radius_a(_radius_a), radius_b(_radius_b) {

	}


	Collision collideCircleCircle(RigidBody& a, RigidBody& b) {
		CircleBody& ca = static_cast<CircleBody&>(a);
		CircleBody& cb = static_cast<CircleBody&>(b);

		vec2 diff = cb.getPosition() - ca.getPosition();
		float dist = abs(diff);

		float penetration = dist - ca.radius() - cb.radius();

		vec2 normal = diff / dist;

		vec2 r_a = ca.getInverseTransform() * (normal * ca.radius());
		vec2 r_b = cb.getInverseTransform() * (-normal * cb.radius());

		return Collision(ca, cb, normal, r_a, r_b);
	}

	Collision collideCircleRectangle(RigidBody& a, RigidBody& b){
		// TODO
		throw std::runtime_error("Not implemented");
	}

	Collision collideCircleConvex(RigidBody& a, RigidBody& b){
		// TODO
		throw std::runtime_error("Not implemented");
	}

	Collision collideCircleRaster(RigidBody& a, RigidBody& b){
		// TODO
		throw std::runtime_error("Not implemented");
	}

	Collision collideRectangleCircle(RigidBody& a, RigidBody& b){
		return collideCircleRectangle(b, a);
	}

	Collision collideRectangleRectangle(RigidBody& a, RigidBody& b){
		// TODO
		throw std::runtime_error("Not implemented");
	}

	Collision collideRectangleConvex(RigidBody& a, RigidBody& b){
		// TODO
		throw std::runtime_error("Not implemented");
	}

	Collision collideRectangleRaster(RigidBody& a, RigidBody& b){
		// TODO
		throw std::runtime_error("Not implemented");
	}

	Collision collideConvexCircle(RigidBody& a, RigidBody& b){
		return collideCircleConvex(b, a);
	}

	Collision collideConvexRectangle(RigidBody& a, RigidBody& b){
		return collideRectangleConvex(b, a);
	}

	Collision collideConvexConvex(RigidBody& a, RigidBody& b){
		// TODO
		throw std::runtime_error("Not implemented");
	}

	Collision collideConvexRaster(RigidBody& a, RigidBody& b){
		// TODO
		throw std::runtime_error("Not implemented");
	}

	Collision collideRasterCircle(RigidBody& a, RigidBody& b){
		return collideCircleRaster(b, a);
	}

	Collision collideRasterRectangle(RigidBody& a, RigidBody& b){
		return collideRectangleRaster(b, a);
	}

	Collision collideRasterConvex(RigidBody& a, RigidBody& b){
		return collideConvexRaster(b, a);
	}

	Collision collideRasterRaster(RigidBody& a, RigidBody& b){
		// TODO
		throw std::runtime_error("Not implemented");
	}

} // namespace phys

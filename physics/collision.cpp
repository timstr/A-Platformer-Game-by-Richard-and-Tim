#include "Collision.hpp"
#include <cassert>

namespace phys {

	Collision::Collision(RigidBody& _a, RigidBody& _b, vec2 _normal, vec2 _radius_a, vec2 _radius_b) :
		a(_a),
		b(_b),
		normal(_normal),
		radius_a(_radius_a),
		radius_b(_radius_b) {

	}


	MaybeCollision collideCircleCircle(RigidBody& body_a, RigidBody& body_b) {
		assert(static_cast<bool>(dynamic_cast<CircleBody*>(&body_a)));
		assert(static_cast<bool>(dynamic_cast<CircleBody*>(&body_b)));
		auto& a = static_cast<CircleBody&>(body_a);
		auto& b = static_cast<CircleBody&>(body_b);

		vec2 diff = b.getPosition() - a.getPosition();
		float dist = abs(diff);

		float penetration = dist - a.radius() - b.radius();

		vec2 normal = diff / dist;

		vec2 r_a = normal * a.radius();
		vec2 r_b = -normal * b.radius();

		return Collision(a, b, normal, r_a, r_b);
	}

	MaybeCollision collideCircleRectangle(RigidBody& body_a, RigidBody& body_b){
		assert(static_cast<bool>(dynamic_cast<CircleBody*>(&body_a)));
		assert(static_cast<bool>(dynamic_cast<RectangleBody*>(&body_b)));
		auto& a = static_cast<CircleBody&>(body_a);
		auto& b = static_cast<RectangleBody&>(body_b);
		// TODO
		return {};
	}

	MaybeCollision collideCircleConvex(RigidBody& body_a, RigidBody& body_b){
		assert(static_cast<bool>(dynamic_cast<CircleBody*>(&body_a)));
		assert(static_cast<bool>(dynamic_cast<ConvexBody*>(&body_b)));
		auto& a = static_cast<CircleBody&>(body_a);
		auto& b = static_cast<ConvexBody&>(body_b);
		// TODO
		throw std::runtime_error("Not implemented");
	}

	MaybeCollision collideCircleRaster(RigidBody& body_a, RigidBody& body_b){
		assert(static_cast<bool>(dynamic_cast<CircleBody*>(&body_a)));
		assert(static_cast<bool>(dynamic_cast<RasterBody*>(&body_b)));
		auto& a = static_cast<CircleBody&>(body_a);
		auto& b = static_cast<RasterBody&>(body_b);
		// TODO
		throw std::runtime_error("Not implemented");
	}

	MaybeCollision collideRectangleCircle(RigidBody& body_a, RigidBody& body_b){
		return collideCircleRectangle(body_b, body_a);
	}

	MaybeCollision collideRectangleRectangle(RigidBody& body_a, RigidBody& body_b){
		assert(static_cast<bool>(dynamic_cast<RectangleBody*>(&body_a)));
		assert(static_cast<bool>(dynamic_cast<RectangleBody*>(&body_b)));
		auto& a = static_cast<RectangleBody&>(body_a);
		auto& b = static_cast<RectangleBody&>(body_b);
		
		// corner points of a, in world space, in clockwise order
		const vec2 a_p[4] = {
			a.getPosition() + (a.getInverseTransform() * vec2{-a.size().x, -a.size().y} * 0.5f),
			a.getPosition() + (a.getInverseTransform() * vec2{a.size().x, -a.size().y} * 0.5f),
			a.getPosition() + (a.getInverseTransform() * vec2{a.size().x, a.size().y} * 0.5f),
			a.getPosition() + (a.getInverseTransform() * vec2{-a.size().x, a.size().y} * 0.5f)
		};

		// corner points of b, in world space, in clockwise order
		const vec2 b_p[4] = {
			b.getPosition() + (b.getInverseTransform() * vec2{-b.size().x, -b.size().y} * 0.5f),
			b.getPosition() + (b.getInverseTransform() * vec2{b.size().x, -b.size().y} * 0.5f),
			b.getPosition() + (b.getInverseTransform() * vec2{b.size().x, b.size().y} * 0.5f),
			b.getPosition() + (b.getInverseTransform() * vec2{-b.size().x, b.size().y} * 0.5f)
		};

		vec2 closest_to_a; // closest point on b from a
		vec2 closest_to_b; // closest point on a from b
		float min_disp_to_a = -1e10f; // smallest displacement from one of b's corners to a
		float min_disp_to_b = -1e10f; // smallest displacement from one of a's corners to b
		vec2 min_disp_normal_a; // normal on a with the smallest displacement
		vec2 min_disp_normal_b; // normal on b with the smallest displacement

		// TODO: play around with min_disp_* and find a way to choose
		// the best surface from which to create the collision

		// separating axis test for surfaces of a
		for (int i = 0; i < 4; ++i){
			// surface normal between points i and i+1
			vec2 normal = orthogonalClockwise(a_p[(i + 1) % 4] - a_p[i]);

			bool all_outside = true;
			for (int j = 0; j < 4; ++j){
				float disp = displacementFromLinePN(a_p[i], normal, b_p[j]);
				all_outside = all_outside && disp > 0.0f;
				if (disp <= 0.0f && disp > min_disp_to_a){
					closest_to_a = b_p[j];
					min_disp_normal_a = normal;
					min_disp_to_a = disp;
				}
			}
			if (all_outside){
				// return empty collision if a separating axis is found
				return {};
			}
		}
		
		// separating axis test for surfaces of b
		for (int i = 0; i < 4; ++i){
			// surface normal between points i and i+1
			vec2 normal = orthogonalClockwise(b_p[(i + 1) % 4] - b_p[i]);

			bool all_outside = true;
			for (int j = 0; j < 4; ++j){
				float disp = displacementFromLinePN(b_p[i], normal, a_p[j]);
				all_outside = all_outside && disp > 0.0f;
				if (disp <= 0.0f && disp > min_disp_to_b){
					closest_to_b = a_p[j];
					min_disp_normal_b = normal;
					min_disp_to_b = disp;
				}
			}
			if (all_outside){
				// return empty collision if a separating axis is found
				return {};
			}
		}

		if (min_disp_to_a > min_disp_to_b){
			vec2 v_a = a.getTransform() * (closest_to_a - a.getPosition());
			v_a.x = std::clamp(v_a.x, -a.size().x * 0.5f, a.size().x * 0.5f);
			v_a.y = std::clamp(v_a.y, -a.size().y * 0.5f, a.size().y * 0.5f);
			closest_to_a = a.getInverseTransform() * v_a;
			vec2 normal = unit(-min_disp_normal_a);
			return Collision{a, b, normal, closest_to_a, closest_to_a + a.getPosition() - b.getPosition()};
		} else {
			vec2 v_b = b.getTransform() * (closest_to_b - b.getPosition());
			v_b.x = std::clamp(v_b.x, -b.size().x * 0.5f, b.size().x * 0.5f);
			v_b.y = std::clamp(v_b.y, -b.size().y * 0.5f, b.size().y * 0.5f);
			closest_to_b = b.getInverseTransform() * v_b;
			vec2 normal = unit(min_disp_normal_b);
			return Collision{a, b, normal, closest_to_b + a.getPosition() - b.getPosition(), closest_to_b};
		}
	}

	MaybeCollision collideRectangleConvex(RigidBody& body_a, RigidBody& body_b){
		assert(static_cast<bool>(dynamic_cast<RectangleBody*>(&body_a)));
		assert(static_cast<bool>(dynamic_cast<ConvexBody*>(&body_b)));
		auto& a = static_cast<RectangleBody&>(body_a);
		auto& b = static_cast<ConvexBody&>(body_b);
		// TODO
		throw std::runtime_error("Not implemented");
	}

	MaybeCollision collideRectangleRaster(RigidBody& body_a, RigidBody& body_b){
		assert(static_cast<bool>(dynamic_cast<RectangleBody*>(&body_a)));
		assert(static_cast<bool>(dynamic_cast<RasterBody*>(&body_b)));
		auto& a = static_cast<RectangleBody&>(body_a);
		auto& b = static_cast<RasterBody&>(body_b);
		// TODO
		throw std::runtime_error("Not implemented");
	}

	MaybeCollision collideConvexCircle(RigidBody& body_a, RigidBody& body_b){
		return collideCircleConvex(body_b, body_a);
	}

	MaybeCollision collideConvexRectangle(RigidBody& body_a, RigidBody& body_b){
		return collideRectangleConvex(body_b, body_a);
	}

	MaybeCollision collideConvexConvex(RigidBody& body_a, RigidBody& body_b){
		assert(static_cast<bool>(dynamic_cast<ConvexBody*>(&body_a)));
		assert(static_cast<bool>(dynamic_cast<ConvexBody*>(&body_b)));
		auto& a = static_cast<ConvexBody&>(body_a);
		auto& b = static_cast<ConvexBody&>(body_b);
		// TODO
		throw std::runtime_error("Not implemented");
	}

	MaybeCollision collideConvexRaster(RigidBody& body_a, RigidBody& body_b){
		assert(static_cast<bool>(dynamic_cast<ConvexBody*>(&body_a)));
		assert(static_cast<bool>(dynamic_cast<RasterBody*>(&body_b)));
		auto& a = static_cast<ConvexBody&>(body_a);
		auto& b = static_cast<RasterBody&>(body_b);
		// TODO
		throw std::runtime_error("Not implemented");
	}

	MaybeCollision collideRasterCircle(RigidBody& body_a, RigidBody& body_b){
		return collideCircleRaster(body_b, body_a);
	}

	MaybeCollision collideRasterRectangle(RigidBody& body_a, RigidBody& body_b){
		return collideRectangleRaster(body_b, body_a);
	}

	MaybeCollision collideRasterConvex(RigidBody& body_a, RigidBody& body_b){
		return collideConvexRaster(body_b, body_a);
	}

	MaybeCollision collideRasterRaster(RigidBody& body_a, RigidBody& body_b){
		assert(static_cast<bool>(dynamic_cast<RasterBody*>(&body_a)));
		assert(static_cast<bool>(dynamic_cast<RasterBody*>(&body_b)));
		return {}; // raster bodies do not collide with other raster bodies
	}

} // namespace phys

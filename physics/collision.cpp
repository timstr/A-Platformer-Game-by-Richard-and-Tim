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

		if (dist > a.radius() + b.radius()){
			return {};
		}

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

		vec2 intersection_sum = {0.0f, 0.0f};
		int count = 0;
		bool a_inside_b = true;
		bool b_inside_a = true;

		// any corners of a that hit b
		for (const vec2& p : a_p){
			if (b.hit(p)){
				intersection_sum += p;
				++count;
			} else {
				a_inside_b = false;
			}
		}

		// any corners of b that hit a
		for (const vec2& p : b_p){
			if (a.hit(p)){
				intersection_sum += p;
				++count;
			} else {
				b_inside_a = false;
			}
		}

		vec2 the_point;
		vec2 the_normal;

		if (a_inside_b || b_inside_a){
			the_point = (a.getPosition() + b.getPosition()) / 2.0f;
			the_normal = b.getPosition() - a.getPosition();
		} else {
			// any line segment intersections between a and b
			for (int i = 0; i < 4; ++i){
				for (int j = 0; j < 4; ++j){
					if (auto p = lineSegmentIntersection(a_p[i], a_p[(i + 1) % 4], b_p[j], b_p[(j + 1) % 4])){
						intersection_sum += *p;
						++count;
					}
				}
			}

			if (count == 0){
				return {};
			}
			the_point = intersection_sum / (float)count;

			float min_dist = 1e6f;

			for (int i = 0; i < 4; ++i){
				const vec2 n = orthogonalClockwise(a_p[(i + 1) % 4] - a_p[i]);
				const float dist = distanceFromLinePN(a_p[i], n, the_point);
				if (dist < min_dist){
					the_normal = n;
					min_dist = dist;
				}
			}
			for (int i = 0; i < 4; ++i){
				const vec2 n = orthogonalClockwise(b_p[(i + 1) % 4] - b_p[i]);
				const float dist = distanceFromLinePN(b_p[i], n, the_point);
				if (dist < min_dist){
					the_normal = -n;
					min_dist = dist;
				}
			}
		}

		return Collision{a, b, unit(the_normal), the_point - a.getPosition(), the_point - b.getPosition()};
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

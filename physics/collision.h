#pragma once

#include "rigidbody.h"

namespace phys {

	// TODO: remove penetration depth, add collision points in local coordinates of _a and _b
	struct Collision {
		Collision(RigidBody& _a, RigidBody& _b, vec2 _normal, float _penetration) :
			a(_a), b(_b), normal(_normal), penetration(_penetration) {

		}

		RigidBody& a;
		RigidBody& b;
		vec2 normal;
		float penetration;
	};

	inline Collision collisionCircleCircle(RigidBody& a, RigidBody& b){
		Circle& ca = static_cast<Circle&>(a);
		Circle& cb = static_cast<Circle&>(b);

		vec2 diff = cb.getPosition() - ca.getPosition();
		float dist = abs(diff);

		float penetration = dist - ca.radius - cb.radius;

		vec2 normal = diff / dist;

		return Collision(ca, cb, normal, penetration);
	}

	inline Collision collisionRectangleCircle(RigidBody& a, RigidBody& b){
		Rectangle& ra = static_cast<Rectangle&>(a);
		Circle& cb = static_cast<Circle&>(b);

		vec2 diff = cb.getPosition() - ra.getPosition();
		vec2 closest = diff;

		float x_extent = ra.size.x * 0.5f;
		float y_extent = ra.size.y * 0.5f;

		closest.x = std::min(std::max(closest.x, -x_extent), x_extent);
		closest.y = std::min(std::max(closest.y, -y_extent), y_extent);

		bool inside = false;

		if (diff == closest){
			inside = true;

			if (std::abs(diff.x) > std::abs(diff.y)){
				closest.x = (closest.x > 0 ? x_extent : -x_extent);
			} else {
				closest.y = (closest.y > 0 ? y_extent : -y_extent);
			}
		}

		vec2 normal = diff - closest;
		float dist = abs(normal);

		if (inside){
			return Collision(ra, cb, -normal / dist, cb.radius - dist);
		} else {
			return Collision(ra, cb, normal / dist, cb.radius - dist);
		}
	}

	inline Collision collisionCircleRectangle(RigidBody& a, RigidBody& b){
		return collisionRectangleCircle(b, a);
	}

	inline Collision collisionRectangleRectangle(RigidBody& a, RigidBody& b){
		Rectangle& ra = static_cast<Rectangle&>(a);
		Rectangle& rb = static_cast<Rectangle&>(b);

		vec2 diff = rb.getPosition() - ra.getPosition();

		float x_overlap = (ra.size.x + rb.size.x) * 0.5f - std::abs(diff.x);

		if (x_overlap > 0){
			float y_overlap = (ra.size.y + rb.size.y) * 0.5f - std::abs(diff.y);

			if (y_overlap > 0){
				if (x_overlap > y_overlap){
					vec2 normal { (diff.x < 0 ? -1.0f : 1.0f), 0 };
					return Collision(ra, rb, normal, x_overlap);
				} else {
					vec2 normal { 0, (diff.y < 0 ? -1.0f : 1.0f) };
					return Collision(ra, rb, normal, y_overlap);
				}
			}
		}

		// not a collision, return a negative penetration
		return Collision(a, b, {0.0f, 0.0f}, -1.0f);
	}

} // namespace phys
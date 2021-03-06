#include "CircleBody.hpp"

namespace phys {

	CircleBody::CircleBody(float radius, float density, float elasticity)
		: RigidBody(
			RigidBody::Circle,
			(density * 2.0f * pi * radius * radius),
			(density * pi * radius * radius * radius * radius),
			elasticity),
		m_radius(radius) {

	}

	float CircleBody::radius() const {
		return m_radius;
	}

	BoundingBox CircleBody::getBoundingBox() const {
		return BoundingBox(getPosition() - vec2{m_radius, m_radius}, getPosition() + vec2{m_radius, m_radius});
	}

	bool CircleBody::hit(vec2 point_world_space) const {
		return abs(point_world_space - getPosition()) <= m_radius;
	}

} // namespace phys
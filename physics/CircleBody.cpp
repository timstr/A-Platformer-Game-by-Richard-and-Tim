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
		return BoundingBox(position.x - m_radius, position.y - m_radius, position.x + m_radius, position.y + m_radius);
	}

} // namespace phys
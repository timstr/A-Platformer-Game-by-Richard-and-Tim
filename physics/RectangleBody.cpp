#include "RectangleBody.hpp"

namespace phys {

	RectangleBody::RectangleBody(vec2 size, float density, float elasticity)
		: RigidBody(
			RigidBody::Rectangle,
			(density * size.x * size.y),
			(density * size.x * size.y * (size.x * size.x + size.y * size.y) / 12.0f),
			elasticity),
		m_size(size) {

	}

	BoundingBox RectangleBody::getBoundingBox() const {
		vec2 topleft = getInverseTransform() * (-m_size * 0.5f);
		vec2 bottomleft = getInverseTransform() * (vec2(-m_size.x, m_size.y) * 0.5f);
		vec2 topright = getInverseTransform() * (vec2(m_size.x, -m_size.y) * 0.5f);
		vec2 bottomright = getInverseTransform() * (m_size * 0.5f);

		vec2 min = {
			std::min<float>({topleft.x, bottomleft.x, topright.x, bottomright.x}),
			std::min<float>({topleft.y, bottomleft.y, topright.y, bottomright.y})
		};

		vec2 max = {
			std::max<float>({topleft.x, bottomleft.x, topright.x, bottomright.x}),
			std::max<float>({topleft.y, bottomleft.y, topright.y, bottomright.y})
		};

		return BoundingBox(getPosition() + min, getPosition() + max);
	}

	vec2 RectangleBody::size() const {
		return m_size;
	}

	bool RectangleBody::hit(vec2 point_world_space) const {
		const vec2 p = getTransform() * (point_world_space - getPosition());
		const vec2 hs = size() * 0.5f;
		return p.x >= -hs.x && p.x <= hs.x && p.y >= -hs.y && p.y <= hs.y;
	}

} // namespace phys
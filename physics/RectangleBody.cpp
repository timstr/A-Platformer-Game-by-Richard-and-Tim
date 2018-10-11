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

		vec2 topleft = getTransform() * (-m_size * 0.5f);
		vec2 bottomleft = getTransform() * (vec2(-m_size.x, m_size.y) * 0.5f);
		vec2 topright = getTransform() * (vec2(-m_size.x, m_size.y) * 0.5f);
		vec2 bottomright = getTransform() * (m_size * 0.5f);

		vec2 min = {
			std::min<float>({topleft.x, bottomleft.x, topright.x, bottomright.x}),
			std::min<float>({topleft.y, bottomleft.y, topright.y, bottomright.y})
		};

		vec2 max = {
			std::max<float>({topleft.x, bottomleft.x, topright.x, bottomright.x}),
			std::max<float>({topleft.y, bottomleft.y, topright.y, bottomright.y})
		};

		return BoundingBox(position + min, position + max);

	}

	vec2 RectangleBody::size() const {
		return m_size;
	}

} // namespace phys
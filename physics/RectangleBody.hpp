#pragma once

#include "RigidBody.hpp"

namespace phys {

	struct RectangleBody : RigidBody {
		RectangleBody(vec2 size, float density, float elasticity);

		BoundingBox getBoundingBox() const override;

		vec2 size() const;

	private:
		const vec2 m_size;
	};

} // namespace phys
#pragma once

#include "RigidBody.hpp"

namespace phys {

	struct RectangleBody : RigidBody {
		RectangleBody(vec2 size, float density, float elasticity);

		BoundingBox getBoundingBox() const override;

		vec2 size() const;

		bool hit(vec2 point_world_space) const;

	private:
		const vec2 m_size;
	};

} // namespace phys